#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <FirebaseESP8266.h>
#include <Wire.h>

// Konfigurasi WiFi
const char* ssid = "SUBIANTO";
const char* password = "americano";

// Konfigurasi ThingSpeak
const char* server = "api.thingspeak.com";
const char* apiKey = "U6CMMQ8IR8I66CGD";

// Konfigurasi Firebase
#define FIREBASE_HOST "pengamatanexpoldr-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "vUiJnJbPcMhF0a96YQbOX9FkB7bEpsdjeDP6TILL"

// Konfigurasi Pin
const int ldrPin = A0;

// Objek Firebase
FirebaseData firebaseData;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  
  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Inisialisasi ThingSpeak
  ThingSpeak.begin(client);
  
  // Inisialisasi Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Membaca nilai sensor LDR
  int ldrValue = analogRead(ldrPin);
  
  // Mengukur intensitas cahaya dalam Lux
  float lux = map(ldrValue, 0, 1023, 0, 1000);
  
  // Mengirim data ke ThingSpeak
  ThingSpeak.writeField(channelNumber, 1, lux, apiKey);
  
  // Mengirim data ke Firebase
  String firebasePath = "/intensitas_cahaya";
  Firebase.setString(firebaseData, firebasePath, String(lux));
  
  // Menampilkan nilai intensitas cahaya pada Serial Monitor
  Serial.print("Intensitas Cahaya (Lux): ");
  Serial.println(lux);
  
  delay(5000); // Jeda 5 detik sebelum pengukuran berikutnya
}
