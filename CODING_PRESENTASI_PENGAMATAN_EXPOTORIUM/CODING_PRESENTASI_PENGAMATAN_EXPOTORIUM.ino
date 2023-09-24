// Library yang dibutuhkan
#include <ESP8266WiFi.h>         // Library untuk menghubungkan ESP8266 ke jaringan WiFi
#include <SPI.h>                 // Library untuk komunikasi melalui protokol SPI.
#include <Wire.h>                // Library untuk komunikasi melalui I2C
#include <ThingSpeak.h>          // Library untuk mengirim data ke ThingSpeak
#include <Adafruit_GFX.h>        // Library untuk menyediakan dasar grafik pada layar OLED.
#include <Adafruit_SSD1306.h>    // Library untuk mengendalikan layar OLED dengan driver SSD1306.
#include <DHT.h>                 // Library untuk sensor DHT
#include <OneWire.h>             // Library OneWire 
#include <DallasTemperature.h>   // Library DallasTemperature 
#include <FirebaseESP8266.h>     // Library untuk mengirim data ke Firebase

// Define screen dimensions
#define SCREEN_WIDTH 128 // Lebar layar OLED
#define SCREEN_HEIGHT 64 // Tinggi layar OLED
#define OLED_RESET -1 // Pin reset OLED

// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D4 // Pin pada ESP8266 yang terhubung dengan sensor DHT
DHT dht(DHTPIN, DHT11);  // Inisialisasi sensor DHT11

const int oneWireBus = D3;             // Untuk kebutuhan DS18B20 (pin D0 / GPIO16)
OneWire oneWire(oneWireBus);          // Untuk kebutuhan DS18B20 
DallasTemperature sensors(&oneWire);  // Untuk kebutuhan DS18B20 

String apiKey = "3EIOONNYGJ76OS5J";  // Masukkan API Key dari ThingSpeak
const char* ssid = "SUBIANTO";  // Nama SSID WiFi
const char* pass = "americano";  // Password WiFi
const char* server = "api.thingspeak.com";  // Alamat server ThingSpeak

const char* firebaseHost = "sistemotomasiexpotorium-default-rtdb.firebaseio.com/";  // Host Firebase
const char* firebaseAuth = "qrm079aAFiCcjpPxrWVFiNFMXTtGXVK0rxu1HCwf";  // Auth Firebase

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Inisialisasi objek display OLED

const int AirValue = 790;  // Nilai bacaan sensor ketika di udara
const int WaterValue = 390;  // Nilai bacaan sensor ketika di air
const int SensorPin = A0;  // Pin analog untuk membaca sensor tanah
const int LdrPin = D8;  // Pin digital untuk membaca sensor LDR
int soilMoistureValue = 0;  // Nilai bacaan sensor tanah
int soilMoisturePercent = 0;  // Nilai kelembaban tanah dalam persen
int ldrValue = 0;  // Nilai bacaan sensor LDR
int lightIntensity = 0;  // Intensitas cahaya dalam lux

float temperatureWater = 0;  // Variabel untuk menyimpan nilai suhu air

WiFiClient client;  // Objek WiFi Client untuk koneksi ke ThingSpeak
FirebaseData firebaseData;  // Objek FirebaseData untuk koneksi ke Firebase

void setup() {
  Serial.begin(115200);
  sensors.begin();            // Inisialisasi untuk Sensor DS18B20
  pinMode(LdrPin, INPUT);  // Set pin LDR sebagai input
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inisialisasi display OLED
  display.clearDisplay();
  dht.begin();  // Memulai sensor DHT11
  WiFi.begin(ssid, pass);  // Mencoba untuk terhubung ke jaringan WiFi

  while (WiFi.status() != WL_CONNECTED) {     // Menunggu hingga ESP8266 terhubung ke WiFi
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");   // Terhubung ke jaringan WiFi
  delay(4000);

  // Menghubungkan ke Firebase
  Firebase.begin(firebaseHost, firebaseAuth);
}

void loop() {
  float h = dht.readHumidity();  // Membaca kelembaban udara
  float t = dht.readTemperature();  // Membaca suhu

  sensors.requestTemperatures();                    // Cek sensor DS18B20
  float temperatureC = sensors.getTempCByIndex(0);  // Ambil Data sensor DS18B20 dalam satuan Celcius

  temperatureWater = temperatureC;  // Menyimpan nilai suhu air ke variabel temperatureWater

  ldrValue = digitalRead(LdrPin);  // Membaca nilai sensor LDR
  lightIntensity = map(ldrValue, 0, 1023, 0, 1000);  // Mengkonversi nilai sensor menjadi intensitas cahaya dalam lux

  // Menampilkan data suhu udara, kelembapan udara, kelembapan tanah, suhu air, dan intensitas cahaya
  Serial.print("Suhu Udara: ");
  Serial.print(t);
  Serial.print(" C, Kelembapan Udara: ");
  Serial.print(h);
  Serial.print(" %, Kelembapan Tanah: ");
  Serial.print(soilMoisturePercent);
  Serial.print(" %, Suhu Air: ");
  Serial.print(temperatureWater);
  Serial.print(" C, Intensitas Cahaya: ");
  Serial.print(lightIntensity);
  Serial.println(" Lux");

  soilMoistureValue = analogRead(SensorPin);  // Membaca nilai sensor tanah

  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);  // Mengkonversi nilai sensor menjadi persentase kelembaban tanah

  if (soilMoisturePercent > 100) { // If soil moisture is above 100% 
    soilMoisturePercent = 100;  // Set soil moisture percentage to 100

    // Menampilkan informasi pada display OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Soil RH: 100%");
    display.print("Air RH: ");
    display.print(h);
    display.println(" %");
    display.print("Temp Air: ");
    display.print(t);
    display.println(" C");
    display.print("Temp Water: ");
    display.print(temperatureWater);
    display.println(" C");
    display.print("Light Intensity: ");
    display.print(lightIntensity);
    display.println(" Lux");
    display.display();

    // Wait for 250 milliseconds
    delay(250);
  }
  else if (soilMoisturePercent < 0) {   // If soil moisture is below 0%
    soilMoisturePercent = 0;  // Set soil moisture percentage to 0

    // Menampilkan informasi pada display OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Soil RH: 0%");
    display.print("Air RH: ");
    display.print(h);
    display.println(" %");
    display.print("Temp Air: ");
    display.print(t);
    display.println(" C");
    display.print("Temp Water: ");
    display.print(temperatureWater);
    display.println(" C");
    display.print("Light Intensity: ");
    display.print(lightIntensity);
    display.println(" Lux");
    display.display();

    // Wait for 250 milliseconds
    delay(250);
  }
  else if (soilMoisturePercent >= 0 && soilMoisturePercent <= 100) {
    // Menampilkan informasi pada display OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Soil RH: ");
    display.print(soilMoisturePercent);
    display.println(" %");
    display.print("Air RH: ");
    display.print(h);
    display.println(" %");
    display.print("Temp Air: ");
    display.print(t);
    display.println(" C");
    display.print("Temp Water: ");
    display.print(temperatureWater);
    display.println(" C");
    display.print("Light Intensity: ");
    display.print(lightIntensity);
    display.println(" Lux");
    display.display();

    // Wait for 250 milliseconds
    delay(250);
  }

  // Mengirim data ke ThingSpeak
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(soilMoisturePercent);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "&field3=";
    postStr += String(t);
    postStr += "&field4=";
    postStr += String(temperatureWater);
    postStr += "&field5=";
    postStr += String(lightIntensity);
    postStr += "\r\n\r\n\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");      // Mengirim permintaan POST ke alamat URL "/update" menggunakan protokol HTTP versi 1.1.
    client.print("Host: api.thingspeak.com\n");   // Menentukan tujuan permintaan adalah "api.thingspeak.com".
    client.print("Connection: close\n");          // Menentukan bahwa koneksi akan ditutup setelah permintaan selesai.
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");  // Mengirim API key sebagai header permintaan.
    client.print("Content-Type: application/x-www-form-urlencoded\n");   // Menentukan tipe konten permintaan adalah "application/x-www-form-urlencoded".
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(2000);                   // Tunggu beberapa saat agar data terkirim sebelum koneksi ditutup
    Serial.println("Data dikirim ke ThingSpeak");
  }
  client.stop();

  // Mengirim data ke Firebase
  if (Firebase.setFloat(firebaseData, "/Kelembapan Tanah", soilMoisturePercent)) {
    Serial.println("Data dikirim ke Firebase (Kelembapan Tanah)");
  }
  else {
    Serial.println("Gagal mengirim data ke Firebase (Kelembapan Tanah)");
  }
  
  if (Firebase.setFloat(firebaseData, "/Kelembapan Udara", h)) {
    Serial.println("Data dikirim ke Firebase (Kelembapan Udara)");
  }
  else {
    Serial.println("Gagal mengirim data ke Firebase (Kelembapan Udara)");
  }
  
  if (Firebase.setFloat(firebaseData, "/Suhu Udara", t)) {
    Serial.println("Data dikirim ke Firebase (Suhu Udara)");
  }
  else {
    Serial.println("Gagal mengirim data ke Firebase (Suhu Udara)");
  }
  
  if (Firebase.setFloat(firebaseData, "/Suhu Air", temperatureWater)) {
    Serial.println("Data dikirim ke Firebase (Suhu Air)");
  }
  else {
    Serial.println("Gagal mengirim data ke Firebase (Suhu Air)");
  }
  
  if (Firebase.setFloat(firebaseData, "/lightIntensity", lightIntensity)) {
    Serial.println("Data dikirim ke Firebase (lightIntensity)");
  }
  else {
    Serial.println("Gagal mengirim data ke Firebase (lightIntensity)");
  }
  
  delay(2000);   // Jeda selama 2 detik sebelum membaca dan mengirim data lagi
}
