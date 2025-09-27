#include "BluetoothA2DPSource.h"
#include <math.h> 
#include "SdFat.h"
#include <SPI.h>
#include <SD.h>


static float m_time = 0.0;
const float m_amplitude = 10000.0;        // Range for 16-bit audio
const float m_deltaTime = 1.0 / 44100.0;  // Sample rate: 44.1 kHz
const float m_phase = 0.0;
const float pi_2 = PI * 2.0;
const float c3_frequency = 261.63;        // C3 note frequency in Hz
BluetoothA2DPSource a2dp_source;

#define SD_CS 5  // You can use any free GPIO
File root;
File audioFile;
bool is_playing = true;

void listFiles(File dir, int numTabs = 0) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;

    for (int i = 0; i < numTabs; i++) Serial.print('\t');
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      listFiles(entry, numTabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size());
    }
    entry.close();
  }
}

void initSDCard(void) {
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

// The supported audio codec in ESP32 A2DP is SBC. SBC audio stream is encoded
// from PCM data normally formatted as 44.1kHz sampling rate, two-channel 16-bit sample data
// int32_t get_data_frames(Frame *frame, int32_t frame_count) {
//     for (int sample = 0; sample < frame_count; ++sample) {
//         float angle = pi_2 * c3_frequency * m_time + m_phase;
//         frame[sample].channel1 = m_amplitude * sin(angle); // Only one channel
//         m_time += m_deltaTime;
//     }

//     delay(1); // Prevent watchdog

//     return frame_count;
// }
int32_t get_pcm_frames(Frame *frame, int32_t frame_count) {
  if (!audioFile || !is_playing) return 0;

  // Temporary buffer for mono samples
  static const int MAX_MONO_SAMPLES = 512;
  int16_t mono_buffer[MAX_MONO_SAMPLES];

  // Cap read to buffer size
  int samples_to_read = (frame_count < MAX_MONO_SAMPLES) ? frame_count : MAX_MONO_SAMPLES;


  // Each mono sample = 2 bytes
  int bytes_to_read = samples_to_read * sizeof(int16_t);
  int bytes_read = audioFile.read((uint8_t*)mono_buffer, bytes_to_read);

  if (bytes_read <= 0) {
    Serial.println("End of file or read error");
    is_playing = false;
    audioFile.close();
    return 0;
  }

  int samples_read = bytes_read / sizeof(int16_t);

  // Duplicate mono sample to both left and right channels
  for (int i = 0; i < samples_read; i++) {
    int16_t sample = mono_buffer[i];
    frame[i].channel1 = sample;  // Left
    frame[i].channel2 = sample;  // Right
  }

  return samples_read;  // Number of stereo frames generated
  delay(1);
}


void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if(!entry) {
      // Nor more files
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();    
  }
}

void rdCard(void) {
  Serial.println("Files in the Card:");
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  delay(500);

  initSDCard(); // Initialize the sd card
  Serial.println("Files on SD:");
  listFiles(SD.open("/"));

  // Open the PCM file
  audioFile = SD.open("/test_mono.pcm");
  if (!audioFile) {
    Serial.println("Failed to open PCM file.");
    return;
  }
  
  a2dp_source.set_auto_reconnect(false);
  a2dp_source.set_data_callback_in_frames(get_pcm_frames);
  a2dp_source.set_volume(30);
  a2dp_source.start("LBS");  
}

void loop() {
  // to prevent watchdog in release > 1.0.6
  delay(1000);
}
