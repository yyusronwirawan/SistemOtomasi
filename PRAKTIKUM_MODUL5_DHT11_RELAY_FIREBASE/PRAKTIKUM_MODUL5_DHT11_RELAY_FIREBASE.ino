#include <DHT.h>
#include <FirebaseESP8266.h>   // Library Firebase untuk ESP8266

#define DHTPIN D2      // Pin yang terhubung dengan output sensor DHT
#define relay D7    // Pin yang terhubung dengan relay

#define DHTTYPE DHT11   // Tipe sensor DHT yang digunakan (DHT11 atau DHT22)

DHT dht(DHTPIN, DHTTYPE);

// Konfigurasi Firebase
#define FIREBASE_HOST "pengukuran-suhu-modul-5-tugas-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "fKGHLX8IgOFXanSP6x2P00gmrO5tJqeFRCd2uEDc"

// Nama Wifi
#define WIFI_SSID "SUBIANTO"          // SSID (nama) WiFi
#define WIFI_PASSWORD "americano"     // Kata sandi WiFi

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);   // Menginisialisasi komunikasi serial dengan kecepatan 9600 bps
  Serial.println(F("DHT11 test!"));   // Menampilkan pesan "DHT11 test!" pada Serial Monitor
  pinMode(relay, OUTPUT);   // Mengatur pin relay sebagai output

  dht.begin();   // Memulai sensor DHT

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

void loop() {
  delay(2000);   // Menunda eksekusi program selama 2 detik (2000 milidetik)

  float h = dht.readHumidity();   // Membaca nilai kelembaban dari sensor DHT
  float t = dht.readTemperature();   // Membaca nilai suhu dalam Celsius dari sensor DHT
  float f = dht.readTemperature(true);   // Membaca nilai suhu dalam Fahrenheit dari sensor DHT

  // Memeriksa apakah sensor berhasil membaca suhu dan kelembaban
  if (isnan(h) || isnan(t) || isnan(f))  {
    Serial.println(F("Failed to read from DHT sensor!"));   // Menampilkan pesan kesalahan jika sensor gagal membaca
    return;
  }

  float hif = dht.computeHeatIndex(f, h);   // Menghitung indeks kepanasan berdasarkan suhu dalam Fahrenheit
  float hic = dht.computeHeatIndex(t, h, false);   // Menghitung indeks kepanasan berdasarkan suhu dalam Celsius

  // Menampilkan nilai suhu dan kelembapan pada Serial Monitor
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F Heat Index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.print(F("°F"));

  if (t <= 30) {
    digitalWrite(relay, LOW);   // Mengatur pin relay menjadi LOW (OFF)
    Serial.println("Relay OFF");   // Menampilkan pesan "Relay OFF" pada Serial Monitor
  } else {
    digitalWrite(relay, HIGH);   // Mengatur pin relay menjadi HIGH (ON)
    Serial.println("Relay ON");   // Menampilkan pesan "Relay ON" pada Serial Monitor
  }

  // Mengirim data ke Firebase
  if (Firebase.setFloat(firebaseData, "/humidity", h) &&
      Firebase.setFloat(firebaseData, "/temperature", t) &&
      Firebase.setFloat(firebaseData, "/heatIndex", hic)) {
    Serial.println("Data sent to Firebase successfully!");
  } else {
    Serial.println("Failed to send data to Firebase!");
  }
}
