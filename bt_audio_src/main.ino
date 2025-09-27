#include "BluetoothA2DPSource.h"
#include <math.h> 
#include "SdFat.h"
#include <SPI.h>
#include <SD.h>

BluetoothA2DPSource a2dp_source;

#define SD_CS 5  // You can use any free GPIO

File root;
File audioFile;
bool is_playing = false;

#define MAX_FILES 20
String pcmFiles[MAX_FILES];
int totalPcmFiles = 0;
int currentFileIndex = 0;

static float m_time = 0.0;
const float m_amplitude = 10000.0;        // Range for 16-bit audio
const float m_deltaTime = 1.0 / 44100.0;  // Sample rate: 44.1 kHz
const float m_phase = 0.0;
const float pi_2 = PI * 2.0;

// Scan root directory and store .pcm file names
void scanPcmFiles() {
  totalPcmFiles = 0;
  root = SD.open("/");
  File entry;
  while ((entry = root.openNextFile())) {
    String fname = entry.name();
    if (!entry.isDirectory() && fname.endsWith(".pcm")) {
      if (totalPcmFiles < MAX_FILES) {
        // prepend "/" to open from root explicitly
        pcmFiles[totalPcmFiles++] = "/" + fname;
        Serial.print("Found PCM file: ");
        Serial.println(pcmFiles[totalPcmFiles-1]);
      }
    }
    entry.close();
  }
  root.close();
}


void openNextFile() {
  if (audioFile) {
    audioFile.close();
    Serial.println("Closed previous file.");
  }

  if (totalPcmFiles == 0) {
    Serial.println("No PCM files found to play!");
    is_playing = false;
    return;
  }

  currentFileIndex++;
  if (currentFileIndex >= totalPcmFiles) currentFileIndex = 0;

  Serial.print("Opening file: ");
  Serial.println(pcmFiles[currentFileIndex]);

  audioFile = SD.open(pcmFiles[currentFileIndex]);
  if (!audioFile) {
    Serial.println("Failed to open PCM file!");
    is_playing = false;
  } else {
    is_playing = true;
  }
}

// Your callback for A2DP PCM data
int32_t get_pcm_frames(Frame *frame, int32_t frame_count) {
  if (!audioFile || !is_playing) return 0;

  static const int MAX_MONO_SAMPLES = 512;
  int16_t mono_buffer[MAX_MONO_SAMPLES];

  int samples_to_read = (frame_count < MAX_MONO_SAMPLES) ? frame_count : MAX_MONO_SAMPLES;

  int bytes_to_read = samples_to_read * sizeof(int16_t);
  int bytes_read = audioFile.read((uint8_t*)mono_buffer, bytes_to_read);

  if (bytes_read <= 0) {
    Serial.println("End of file reached, switching to next file...");
    openNextFile();
    return 0;
  }

  int samples_read = bytes_read / sizeof(int16_t);

  for (int i = 0; i < samples_read; i++) {
    int16_t sample = mono_buffer[i];
    frame[i].channel1 = sample;  // Left
    frame[i].channel2 = sample;  // Right
  }

  return samples_read;
}

void initSDCard() {
  Serial.println("Initializing SD card (SPI)...");

  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card initialization failed");
    return;
  }

  Serial.println("SD card initialized successfully.");

  uint32_t cardSize = SD.cardSize() / (1024 * 1024); // in MB
  Serial.print("Card Size: ");
  Serial.print(cardSize);
  Serial.println(" MB");
}

void setup() {
  Serial.begin(9600);
  delay(500);

  initSDCard();

  scanPcmFiles();

  if (totalPcmFiles == 0) {
    Serial.println("No PCM files found on SD card.");
  } else {
    currentFileIndex = -1; // So openNextFile sets it to 0
    openNextFile();
  }

  a2dp_source.set_auto_reconnect(false);
  a2dp_source.set_data_callback_in_frames(get_pcm_frames);
  a2dp_source.set_volume(50);
  a2dp_source.start("LBS");
}

void loop() {
  delay(1000);
}
