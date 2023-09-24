// Library yang dibutuhkan
#include <ESP8266WiFi.h>              // Library untuk menghubungkan ESP8266 ke jaringan WiFi
#include <FirebaseESP8266.h>          // Library untuk berkomunikasi dengan Firebase
#include <Wire.h>                     // Library untuk komunikasi melalui I2C
#include <LiquidCrystal_I2C.h>        // Library untuk menampilkan data di LCD
#include <DHT.h>                      // Library untuk sensor DHT

// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D4       // Pin pada ESP8266 yang terhubung dengan sensor DHT
#define DHTTYPE DHT11   // Tipe sensor DHT yang digunakan (DHT11/DHT22)
DHT dht11(DHTPIN, DHTTYPE);

// Isikan sesuai pada Firebase
#define FIREBASE_HOST "monitoring-suhu-esp8266-8e079-default-rtdb.firebaseio.com/"  // URL host Firebase
#define FIREBASE_AUTH "TMfy8tlhFoDW2R4UOp3lQsc1eyTJ3euQ9JC9WODw"                    // Token autentikasi Firebase

// Nama Wifi
#define WIFI_SSID "SUBIANTO"          // SSID (nama) WiFi
#define WIFI_PASSWORD "americano"     // Kata sandi WiFi

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);
  
  dht11.begin();
  
  // Koneksi ke Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   // Memulai koneksi WiFi dengan SSID dan kata sandi yang ditentukan
  Serial.print("Menghubungkan");
  while (WiFi.status() != WL_CONNECTED) { // Menunggu hingga ESP8266 terhubung ke WiFi
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Terhubung dengan IP: ");
  Serial.println(WiFi.localIP());         // Menampilkan alamat IP lokal ESP8266 yang terhubung ke WiFi
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Memulai koneksi ke Firebase menggunakan URL host dan token autentikasi
}

void loop(){
  
  // Sensor DHT11 membaca suhu dan kelembaban
  float t = dht11.readTemperature();
  float h = dht11.readHumidity();

  // Memeriksa apakah sensor berhasil membaca suhu dan kelembaban
  if (isnan(t) || isnan(h)) {
    Serial.println("Gagal membaca sensor DHT11");
    delay(1000);
    return;
  }

  // Menampilkan suhu dan kelembaban ke serial monitor
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.println();

  // Memberikan status suhu dan kelembaban ke Firebase
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/suhu", t)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 
    
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/kelembapan", h)){
      Serial.println("Kelembaban terkirim");
      Serial.println();
    } else{
      Serial.println("Kelembaban tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
    
  delay(1000);
}