// Library yang dibutuhkan
#include <ESP8266WiFi.h>              // Library untuk menghubungkan ESP8266 ke jaringan WiFi
#include <FirebaseESP8266.h>          // Library untuk berkomunikasi dengan Firebase

#define FIREBASE_HOST "https://sensorultrasonik-7f8a7-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "FcsAn15JZMERcboGlOL5Iwy7LKSjTjTdeBYqxKtS"

// Nama Wifi
#define WIFI_SSID "SUBIANTO"          // SSID (nama) WiFi
#define WIFI_PASSWORD "americano"     // Kata sandi WiFi

#define triggerPin D8   // Pin untuk mengirimkan sinyal trigger ke sensor ultrasonik
#define echoPin D7      // Pin untuk menerima sinyal echo dari sensor ultrasonik
#define relayPin D2     // Pin untuk mengontrol relay

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);   // Memulai komunikasi serial dengan baud rate 9600
  pinMode(relayPin, OUTPUT);   // Mengatur pin relay sebagai OUTPUT
  pinMode(triggerPin, OUTPUT);   // Mengatur pin trigger sebagai OUTPUT
  pinMode(echoPin, INPUT);   // Mengatur pin echo sebagai INPUT

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   // Memulai koneksi WiFi dengan SSID dan kata sandi yang ditentukan
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Memulai koneksi ke Firebase menggunakan URL host dan token autentikasi
}

void loop() {
  long durasi, jarak;

  digitalWrite(triggerPin, LOW);   // Mengatur pin trigger menjadi LOW
  delayMicroseconds(2);   // Memberikan jeda 2 mikrodetik
  digitalWrite(triggerPin, HIGH);   // Mengatur pin trigger menjadi HIGH
  delayMicroseconds(10);   // Memberikan jeda 10 mikrodetik
  digitalWrite(triggerPin, LOW);   // Mengatur pin trigger menjadi LOW

  durasi = pulseIn(echoPin, HIGH);   // Membaca durasi dari pin echo ketika sinyal HIGH diterima
  jarak = (durasi / 2) / 29.1;   // Menghitung jarak berdasarkan durasi yang diukur

  Serial.print("Jarak: ");   // Mencetak teks "Jarak: "
  Serial.print(jarak);   // Mencetak nilai jarak
  Serial.println(" cm");   // Mencetak teks " cm"

  if (Firebase.setString(firebaseData, "/jarak", String(jarak))) {  // Mengirim data jarak ke Firebase
    Serial.println("Data jarak berhasil dikirim ke Firebase.");
  } else {
    Serial.println("Gagal mengirim data jarak ke Firebase.");
    Serial.println(firebaseData.errorReason());
  }

  if (jarak >= 5) {   // Jika jarak lebih besar atau sama dengan 5
    digitalWrite(relayPin, LOW);   // Mengatur pin relay menjadi LOW
  } else {   // Jika jarak kurang dari 5
    digitalWrite(relayPin, HIGH);   // Mengatur pin relay menjadi HIGH
  }

  delay(1000);   // Memberikan jeda 1 detik
}
