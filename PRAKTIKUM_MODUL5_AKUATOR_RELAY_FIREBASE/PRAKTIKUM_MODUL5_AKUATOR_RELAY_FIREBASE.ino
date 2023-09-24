#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define ONE_WIRE_BUS D6
#define RELAY_PIN D7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Konfigurasi Firebase
#define FIREBASE_HOST "relaydanakuator-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "a92jl0VQi8pZrdY5gG18EVELbXxhsu3SJd3EYIcz"

// Nama Wifi
#define WIFI_SSID "SUBIANTO"       // SSID (nama) WiFi
#define WIFI_PASSWORD "americano"   // Kata sandi WiFi

FirebaseData firebaseData;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  sensors.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inisialisasi display OLED
  display.display();
  delay(2000);
  display.clearDisplay();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Menghubungkan ke jaringan WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Memulai koneksi ke Firebase menggunakan URL host dan token autentikasi
}

void loop(void)
{
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Serial.print("Temperature is: ");
  Serial.println(temperature);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temperature:");
  display.setCursor(0, 10);
  display.print(temperature);
  display.print(" C");

  if (temperature > 31)
  {
    digitalWrite(RELAY_PIN, HIGH);  // Menghidupkan relay
    Serial.println("Relay is ON");
    display.setCursor(0, 30);
    display.println("Relay is ON");
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);  // Mematikan relay
    Serial.println("Relay is OFF");
    display.setCursor(0, 30);
    display.println("Relay is OFF");
  }

  display.display();
  delay(500);

  // Mengirim data ke Firebase
  if (digitalRead(RELAY_PIN) == HIGH)
  {
    Firebase.setString(firebaseData, "/relay", "ON");
  }
  else
  {
    Firebase.setString(firebaseData, "/relay", "OFF");
  }
  Firebase.setFloat(firebaseData, "/temperature", temperature);

  if (firebaseData.dataAvailable())
  {
    Serial.println("Data sent to Firebase successfully!");
  }
  else
  {
    Serial.println("Failed to send data to Firebase!");
  }
}
