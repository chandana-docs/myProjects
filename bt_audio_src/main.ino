#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "BluetoothA2DPSource.h"

// Change these pins according to your ESP32 board
#define SD_CS_PIN 5

BluetoothA2DPSource a2dp_source;

File audioFile;

#define MAX_FILES 10
String pcmFiles[MAX_FILES];
int pcmFilesCount = 0;
int currentFileIndex = 0;
bool is_playing = false;

void openAudioFileByIndex(int index) {
  if (audioFile) audioFile.close();

  if (index < 0 || index >= pcmFilesCount) {
    Serial.println("Invalid file index");
    is_playing = false;
    return;
  }

  String filename = pcmFiles[index];
  Serial.printf("Opening file: %s\n", filename.c_str());
  audioFile = SD.open(filename.c_str());

  if (!audioFile) {
    Serial.println("Failed to open PCM file!");
    is_playing = false;
  } else {
    is_playing = true;
  }
}

int32_t audioDataCallback(uint8_t* data, int32_t len) {
  if (!is_playing || !audioFile) return 0;

   int monoBytesToRead = len / 2;  // because output buffer is stereo (2 channels), input is mono (1 channel)
  static uint8_t monoBuffer[2048]; // temporary buffer for mono data
  if (monoBytesToRead > sizeof(monoBuffer)) monoBytesToRead = sizeof(monoBuffer);

   int bytesRead = audioFile.read(monoBuffer, monoBytesToRead);
  if (bytesRead <= 0) {
    Serial.println("End of file reached, switching to next file...");
    currentFileIndex = (currentFileIndex + 1) % pcmFilesCount;
    openAudioFileByIndex(currentFileIndex);
    return 0;
  }

  int samplesRead = bytesRead / 2; // 2 bytes per sample (16-bit)
  int16_t* monoSamples = (int16_t*)monoBuffer;
  int16_t* stereoSamples = (int16_t*)data;

  for (int i = 0; i < samplesRead; i++) {
    stereoSamples[i * 2] = monoSamples[i];       // Left channel
    stereoSamples[i * 2 + 1] = monoSamples[i];   // Right channel (duplicate)
  }

  return samplesRead * 4;  // stereo output: samples * 2 channels * 2 bytes/sample
}

// AVRCP passthrough command callback (handle remote control buttons)
void onAvrcPassthruCommand(uint8_t key, bool isReleased) {
  if (isReleased) return; // Handle only key press events

  switch (key) {
    case 0x4B: // Next Track
      Serial.println("Next track command received");
      currentFileIndex++;
      if (currentFileIndex >= pcmFilesCount) currentFileIndex = 0;
      openAudioFileByIndex(currentFileIndex);
      break;

    case 0x4C: // Previous Track
      Serial.println("Previous track command received");
      if (currentFileIndex == 0) currentFileIndex = pcmFilesCount - 1;
      else currentFileIndex--;
      openAudioFileByIndex(currentFileIndex);
      break;

    case 0x44: // Play
      Serial.println("Play command received");
      if (!is_playing && audioFile) {
        is_playing = true;
      }
      break;

    case 0x46: // Pause
      Serial.println("Pause command received");
      is_playing = false;
      break;

    default:
      Serial.printf("Unhandled command: 0x%02X\n", key);
      break;
  }
}

void setup() {
  Serial.begin(115200);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    while (true)
      ;
  }
  Serial.println("SD card initialized successfully.");

  // List all PCM files on SD card
  File root = SD.open("/");
  pcmFilesCount = 0;
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break; // no more files

    String fname = entry.name();
    if (fname.endsWith(".pcm") && pcmFilesCount < MAX_FILES) {

      if (!fname.startsWith("/")) {
        fname = "/" + fname;
      }
      pcmFiles[pcmFilesCount++] = fname;
      Serial.printf("Found PCM file: %s\n", fname.c_str());
    }
    entry.close();
  }
  root.close();

  if (pcmFilesCount == 0) {
    Serial.println("No PCM files found on SD card!");
    while (true)
      ;
  }

  currentFileIndex = 0;
  openAudioFileByIndex(currentFileIndex);

  a2dp_source.set_data_callback(audioDataCallback);
  a2dp_source.set_avrc_passthru_command_callback(onAvrcPassthruCommand);

  a2dp_source.start("LBS");

  Serial.println("Bluetooth A2DP source started.");
}

void loop() {
  // Nothing here — all handled in callbacks
}
