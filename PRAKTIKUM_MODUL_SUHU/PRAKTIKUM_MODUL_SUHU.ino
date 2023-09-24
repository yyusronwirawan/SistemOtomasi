// Library yang dibutuhkan
#include <ESP8266WiFi.h>              // Library untuk menghubungkan ESP8266 ke jaringan WiFi
#include <FirebaseESP8266.h>          // Library untuk berkomunikasi dengan Firebase
#include <Wire.h>                     // Library untuk komunikasi melalui I2C
#include <LiquidCrystal_I2C.h>        // Library untuk menampilkan data di LCD
#include <DHT.h>                      // Library untuk sensor DHT

// Isikan sesuai dengan detail Firebase Anda
#define FIREBASE_HOST "monitoringsuhumodul3-default-rtdb.firebaseio.com/"  // URL host Firebase
#define FIREBASE_AUTH "zvZWRLhVQv0Mi5ZZsZyHe4ocjHEDO72KkGb443qs"           // Token autentikasi Firebase

// Nama WiFi yang akan digunakan
#define WIFI_SSID "SUBIANTO"        // SSID (nama) WiFi
#define WIFI_PASSWORD "americano"   // Kata sandi WiFi

// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D4       // Pin pada ESP8266 yang terhubung dengan sensor DHT
#define DHTTYPE DHT11   // Tipe sensor DHT yang digunakan (DHT11/DHT22)
DHT dht11(DHTPIN, DHTTYPE);  // Inisialisasi objek sensor DHT

// Mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);

  // Koneksi ke WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   // Memulai koneksi WiFi dengan SSID dan kata sandi yang ditentukan
  Serial.print("Menghubungkan");
  while (WiFi.status() != WL_CONNECTED) {   // Menunggu hingga ESP8266 terhubung ke WiFi
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Terhubung dengan IP: ");
  Serial.println(WiFi.localIP());   // Menampilkan alamat IP lokal ESP8266 yang terhubung ke WiFi
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // Memulai koneksi ke Firebase menggunakan URL host dan token autentikasi
  dht11.begin();   // Memulai sensor DHT11
}

void loop() {
  // Sensor DHT11 membaca suhu
  float t = dht11.readTemperature();

  // Set Value
  if (!Firebase.setFloat(firebaseData, "/Suhu", t)) {   // Mengirimkan data suhu ke Firebase dengan nama "Suhu"
    Serial.print("Gagal mengirimkan data. Alasan: ");
    Serial.println(firebaseData.errorReason());
  } else {
    Serial.print("Suhu : ");
    Serial.print(t);
    Serial.println(" *C");
  }
  delay(1000);
}
