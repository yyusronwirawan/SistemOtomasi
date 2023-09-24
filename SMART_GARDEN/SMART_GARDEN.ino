// Library yang dibutuhkan
#include <ESP8266WiFi.h>         // Library untuk menghubungkan ESP8266 ke jaringan WiFi
#include <SPI.h>                 // Library untuk komunikasi melalui protokol SPI.
#include <Wire.h>                // Library untuk komunikasi melalui I2C
#include <ThingSpeak.h>          // Library untuk mengirim data ke ThingSpeak
#include <Adafruit_GFX.h>        // Library untuk menyediakan dasar grafik pada layar OLED.
#include <Adafruit_SSD1306.h>    // Library untuk mengendalikan layar OLED dengan driver SSD1306.
#include <DHT.h>                 // Library untuk sensor DHT
#include <OneWire.h>             //--Library tambahan untuk keperluan DS18B20
#include <DallasTemperature.h>   //--Library tambahan untuk keperluan DS18B20

// Define screen dimensions
#define SCREEN_WIDTH 128 // Lebar layar OLED
#define SCREEN_HEIGHT 64 // Tinggi layar OLED
#define OLED_RESET -1 // Pin reset OLED

// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D4 ////--Diubah menjadi 2, sesuai dengan pin esp8266 D4         // Pin pada ESP8266 yang terhubung dengan sensor DHT
DHT dht(DHTPIN, DHT11);  // Inisialisasi sensor DHT11

const int oneWireBus = D3;             //--Untuk kebutuhan DS18B20 (pin D0 / GPIO16)
OneWire oneWire(oneWireBus);          //--Untuk kebutuhan DS18B20 
DallasTemperature sensors(&oneWire);  //--Untuk kebutuhan DS18B20 

String apiKey = "";  // Masukkan API Key dari Thingspeak
const char* ssid = "SUBIANTO";  // Nama SSID WiFi
const char* pass = "americano";  // Password WiFi
const char* server = "api.thingspeak.com";  // Alamat server Thingspeak

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Inisialisasi objek display OLED

const int AirValue = 790;  // Nilai bacaan sensor ketika di udara
const int WaterValue = 390;  // Nilai bacaan sensor ketika di air
const int SensorPin = A0;  // Pin analog untuk membaca sensor tanah
int soilMoistureValue = 0;  // Nilai bacaan sensor tanah
int soilMoisturePercent = 0;  // Nilai kelembaban tanah dalam persen
int relayPin = D7;//--Diubah menjadi 14, sesuai dengan pin esp8266 D5;  // Pin untuk mengendalikan relay

WiFiClient client;  // Objek WiFi Client untuk koneksi ke Thingspeak

void setup() {
  Serial.begin(115200);
  sensors.begin();            //--Inisialisasi untuk Sensor DS18B20
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inisialisasi display OLED
  display.clearDisplay();
  pinMode(relayPin, OUTPUT);
  dht.begin();  // Memulai sensor DHT11
  WiFi.begin(ssid, pass);  // Mencoba untuk terhubung ke jaringan WiFi

  while (WiFi.status() != WL_CONNECTED) {     // Menunggu hingga ESP8266 terhubung ke WiFi
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");   // Terhubung ke jaringan WiFi
  delay(4000);
}

void loop() {
  float h = dht.readHumidity();  // Membaca kelembaban udara
  float t = dht.readTemperature();  // Membaca suhu

  sensors.requestTemperatures();                    //--Cek sensor DS18B20
  float temperatureC = sensors.getTempCByIndex(0);  //--Ambil Data sensor DS18B20 dalam satuan Celcius

//--Tambahan untuk sensor DS18B20
  Serial.print("Suhu Udara: ");
  Serial.print(t);
  Serial.print(" C, Kelembapan Udara: ");
  Serial.print(h);
  Serial.print(" %, Kelembapan Tanah: ");
  Serial.print(soilMoisturePercent);
  Serial.print(" %, Suhu Air: ");
  Serial.print(temperatureC);
  Serial.println(" C");
//--End Tambahan untuk sensor DS18B20

//  Serial.print("Humidity: ");
//  Serial.println(h);
//  Serial.print("Temperature: ");
//  Serial.println(t);

  soilMoistureValue = analogRead(SensorPin);  // Membaca nilai sensor tanah
  //Serial.println(soilMoistureValue);

  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);  // Mengkonversi nilai sensor menjadi persentase kelembaban tanah

  if (soilMoisturePercent > 100) { // If soil moisture is above 100% 
    //Serial.println("100 %"); //Print "100%" to the serial monitor

    // Menampilkan informasi pada display OLED
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Soil RH:");
    display.setTextSize(1);
    display.print("100");
    display.println(" %");
    display.setCursor(0, 20);
    display.setTextSize(2);
    display.print("Air RH:");
    display.setTextSize(1);
    display.print(h);
    display.println(" %");
    display.setCursor(0, 40);
    display.setTextSize(2);
    display.print("Temp:");
    display.setTextSize(1);
    display.print(t);
    display.println(" C");
    display.display();

    // Wait for 250 milliseconds
    delay(250);

    // Clear the display
    display.clearDisplay();
  }
  
  else if (soilMoisturePercent < 0) {   // If soil moisture is below 0%
    //Serial.println("0 %");    // Print "0%" to the serial monitor

    // Menampilkan informasi pada display OLED
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Soil RH:");
    display.setTextSize(1);
    display.print("0");
    display.println(" %");
    display.setCursor(0, 20);
    display.setTextSize(2);
    display.print("Air RH:");
    display.setTextSize(1);
    display.print(h);
    display.println(" %");
    display.setCursor(0, 40);
    display.setTextSize(2);
    display.print("Temp:");
    display.setTextSize(1);
    display.print(t);
    display.println(" C");
    display.display();

    // Wait for 250 milliseconds
    delay(250);

    // Clear the display
    display.clearDisplay();
  }

  else if (soilMoisturePercent >= 0 && soilMoisturePercent <= 100) {
    ///Serial.print(soilMoisturePercent);    // Print the value of soilMoisturePercent
    //Serial.println("%");    // Print the percentage sign

    // Menampilkan informasi pada display OLED
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("Soil RH:");
    display.setTextSize(1);
    display.print(soilMoisturePercent);
    display.println(" %");
    display.setCursor(0, 20);
    display.setTextSize(2);
    display.print("Air RH:");
    display.setTextSize(1);
    display.print(h);
    display.println(" %");
    display.setCursor(0, 40);
    display.setTextSize(2);
    display.print("Temp:");
    display.setTextSize(1);
    display.print(t);
    display.println(" C");
    display.display();

    // Wait for 250 milliseconds
    delay(250);

    // Clear the display
    display.clearDisplay();
  }

//  if (soilMoisturePercent >= 0 && soilMoisturePercent <= 30) {
//    digitalWrite(relayPin, HIGH);  // Mengaktifkan pompa
//    Serial.println("Pompa is ON");
//  }
//  else if (soilMoisturePercent > 30 && soilMoisturePercent <= 100) {
//    digitalWrite(relayPin, LOW);  // Mematikan pompa
//    Serial.println("Pompa is OFF");
//  }

  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(soilMoisturePercent);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "&field3=";
    postStr += String(t);
    postStr += "&field4=";
    postStr += String(relayPin);
    postStr += "\r\n\r\n\r\n\r\n";

    // Mengirim data ke Thingspeak
    client.print("POST /update HTTP/1.1\n");      // Mengirim permintaan POST ke alamat URL "/update" menggunakan protokol HTTP versi 1.1.
    client.print("Host: api.thingspeak.com\n");   // Menentukan tujuan permintaan ke "api.thingspeak.com" sebagai host.
    client.print("Connection: close\n");  // Mengatur header koneksi untuk menutup koneksi setelah permintaan selesai
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");     // Mengirim kunci API ke Thingspeak dalam header permintaan menggunakan header khusus "X-THINGSPEAKAPIKEY".
    client.print("Content-Type: application/x-www-form-urlencoded\n");  // Mengatur tipe konten permintaan sebagai
    client.print("Content-Length: "); // Mengatur panjang konten permintaan sesuai dengan panjang dari variabel "postStr" yang akan dikirim.
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);          // Mengirimkan data yang akan dikirim dalam permintaan POST. Data ini sesuai dengan isi dari variabel "postStr".

    delay(2000);                   // Tunggu beberapa saat agar data terkirim sebelum koneksi ditutup
  }
  client.stop();                  // Menutup koneksi dengan server setelah permintaan selesai dikirim.
  Serial.println("Waiting...");   // Menampilkan teks "Menunggu..." pada monitor serial
  delay(1000);                    // Menunda eksekusi program selama 1000 milidetik (1 detik)
}
