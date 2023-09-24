#include <DHT.h>

#define DHTPIN D2      // Pin yang terhubung dengan output sensor DHT
#define relay D7    // Pin yang terhubung dengan relay

#define DHTTYPE DHT11   // Tipe sensor DHT yang digunakan (DHT11 atau DHT22)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);   // Menginisialisasi komunikasi serial dengan kecepatan 9600 bps
  Serial.println(F("DHT11 test!"));   // Menampilkan pesan "DHT11 test!" pada Serial Monitor
  pinMode(relay, OUTPUT);   // Mengatur pin relay sebagai output

  dht.begin();   // Memulai sensor DHT
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
}
