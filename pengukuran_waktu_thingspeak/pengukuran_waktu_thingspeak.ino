#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Konfigurasi WiFi
const char* ssid = "SUBIANTO";
const char* password = "americano";

// Konfigurasi Thingspeak
unsigned long channelID = 2184611; // Ganti dengan Channel ID Anda
const char* server = "api.thingspeak.com";
const char* writeAPIKey = "9Z0DH4UKZ0VQ6YN9";

// Pin Ultrasonik
const int trigPin = D8;
const int echoPin = D7;

WiFiClient client;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Inisialisasi koneksi ke Thingspeak
  ThingSpeak.begin(client);

  // Konfigurasi pin ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Mengukur jarak dengan sensor ultrasonik
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Mengirim data ke Thingspeak
  if (distance > 0) {
    // Mengukur waktu respon dalam detik dan milidetik
    float responseTime = (float)millis() / 1000.0;

    // Menampilkan data di Serial Monitor
    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.print(" cm\t");
    Serial.print("Waktu Respon: ");
    Serial.print(responseTime, 3); // Menampilkan 3 angka desimal
    Serial.println(" s");

    // Mengirim data ke Thingspeak
    ThingSpeak.setField(1, distance);
    ThingSpeak.setField(2, responseTime);
    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (response == 200) {
      Serial.println("Data berhasil dikirim ke Thingspeak!");
    } else {
      Serial.println("Gagal mengirim data ke Thingspeak :(");
    }
  } else {
    Serial.println("Sensor ultrasonik tidak mendeteksi objek");
  }

  // Jeda antara pengiriman data
  delay(2000);
}