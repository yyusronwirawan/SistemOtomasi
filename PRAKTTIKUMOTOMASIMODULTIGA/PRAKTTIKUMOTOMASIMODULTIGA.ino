#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 4  //D1 pin of NodeMCU

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);  // Pass the oneWire reference to Dallas Temperature.

void setup(void)
{
  Serial.begin(9600);  // Inisialisasi komunikasi serial dengan kecepatan 9600 bps
  sensors.begin();     // Memulai sensor Dallas Temperature
}

void loop(void)
{
  sensors.requestTemperatures();  // Mengirim perintah untuk mendapatkan suhu
  Serial.println("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0));  // Mengambil dan mencetak suhu dalam Celsius dari sensor dengan indeks 0
  delay(500);  // Memberikan jeda 500 milidetik
}