#include <DHT.h>                // Library Firebase untuk DHT 11
#include <FirebaseESP8266.h>   // Library Firebase untuk ESP8266

#define triggerPin D2   // Pin untuk mengirimkan sinyal trigger ke sensor ultrasonik
#define echoPin D1      // Pin untuk menerima sinyal echo dari sensor ultrasonik
#define DHTPIN D4      // Pin yang terhubung dengan output sensor DHT
#define relayPin D7    // Pin yang terhubung dengan relay

#define DHTTYPE DHT11   // Tipe sensor DHT yang digunakan DHT11

DHT dht(DHTPIN, DHTTYPE);

// Konfigurasi Firebase
#define FIREBASE_HOST "prakmodul5dht11ultrasonik-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AeFwUJLPLUkpkp3QoT3xg7jDSHvz2MF0TLVcXtMF"

// Nama Wifi
#define WIFI_SSID "SUBIANTO"          // SSID (nama) WiFi
#define WIFI_PASSWORD "americano"     // Kata sandi WiFi

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);   // Menginisialisasi komunikasi serial dengan kecepatan 9600 bps
  Serial.println(F("DHT11 and Ultrasonic Sensor Test!"));   // Menampilkan pesan "DHT11 and Ultrasonic Sensor Test" pada Serial Monitor
  pinMode(relayPin, OUTPUT);   // Mengatur pin relay sebagai output
  pinMode(triggerPin, OUTPUT);   // Mengatur pin trigger sebagai output
  pinMode(echoPin, INPUT);   // Mengatur pin echo sebagai input

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
  if (isnan(h) || isnan(t) || isnan(f)) {
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
    digitalWrite(relayPin, LOW);   // Mengatur pin relay menjadi LOW (OFF)
    Serial.println("Relay OFF");   // Menampilkan pesan "Relay OFF" pada Serial Monitor
  } else {
    digitalWrite(relayPin, HIGH);   // Mengatur pin relay menjadi HIGH (ON)
    Serial.println("Relay ON");   // Menampilkan pesan "Relay ON" pada Serial Monitor
  }

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

  if (jarak >= 5) {   // Jika jarak lebih besar atau sama dengan 5
    digitalWrite(relayPin, LOW);   // Mengatur pin relay menjadi LOW
    Serial.println("Relay OFF");   // Menampilkan pesan "Relay OFF" pada Serial Monitor
  } else {   // Jika jarak kurang dari 5
    digitalWrite(relayPin, HIGH);   // Mengatur pin relay menjadi HIGH
    Serial.println("Relay ON");   // Menampilkan pesan "Relay ON" pada Serial Monitor
  }

  // Mengirim data ke Firebase
  if (Firebase.setFloat(firebaseData, "/humidity", h) &&
      Firebase.setFloat(firebaseData, "/temperature", t) &&
      Firebase.setFloat(firebaseData, "/heatIndex", hic) &&
      Firebase.setInt(firebaseData, "/jarak", jarak)) {
    Serial.println("Data berhasil dikirim ke Firebase!");
  } else {
    Serial.println("Gagal mengirim data ke Firebase!");
  }
}
