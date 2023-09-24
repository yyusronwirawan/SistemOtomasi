// Definisi pin menggunakan macro
#define echopin D7      // Variabel echo pin menggunakan pin D7
#define trigpin D8      // Variabel trigpin menggunakan pin D8
#define led1 D1         // Variabel led1 menggunakan pin D1 (LED Hijau)
#define led2 D3         // Variabel led2 menggunakan pin D3 (LED Merah)
#define led3 D2         // Variabel led3 menggunakan pin D2 (LED Kuning)
#define relaybahaya D5  // Pin Modul Relay untuk peringatan bahaya

void setup() {
  // Konfigurasi pin sebagai input atau output
  pinMode(echopin, INPUT);    // Konfigurasi echopin sebagai input
  pinMode(trigpin, OUTPUT);   // Konfigurasi trigpin sebagai output
  pinMode(led1, OUTPUT);     // Konfigurasi led1 sebagai output
  pinMode(led2, OUTPUT);     // Konfigurasi led2 sebagai output
  pinMode(led3, OUTPUT);     // Konfigurasi led3 sebagai output
  pinMode(relaybahaya, OUTPUT);  // Konfigurasi relaybahaya sebagai output

  // Inisialisasi komunikasi serial
  Serial.begin(9600);
}

void loop() {
  // Kode utama yang dijalankan secara berulang

  // Deklarasi variabel dan pengukuran jarak menggunakan ultrasonik
  float luas_alas = 113.024;    // Deklarasi variabel luas_alas dengan nilai 113.024
  float tinggi_gelas = 6.22;  // Deklarasi variabel tinggi_gelas dengan nilai 6.22

  digitalWrite(trigpin, LOW);  // Menetapkan trigpin menjadi LOW (netral)
  delayMicroseconds(2);

  digitalWrite(trigpin, HIGH); // Memberikan input HIGH ke trigpin
  delayMicroseconds(10);

  digitalWrite(trigpin, LOW);  // Menetapkan trigpin menjadi LOW (menerima input)

  int echotime = pulseIn(echopin, HIGH); // Mengukur waktu pantulan gelombang suara menggunakan pulseIn()

  float jarakpantul =  echotime * 0.034 / 2;    // Menghitung jarak pantul berdasarkan waktu yang diukur
  float tinggiair = tinggi_gelas - jarakpantul; // Menghitung tinggi air berdasarkan jarak pantul
  float volume = luas_alas * tinggiair;         // Menghitung volume air

  // Output yang ditampilkan melalui komunikasi serial

  Serial.print("jarak permukaan : ");
  Serial.print(jarakpantul);
  Serial.print(" cm ");

  Serial.print("tinggi permukaan : ");
  Serial.print(tinggiair);
  Serial.print(" cm ");

  Serial.print("volume : ");
  Serial.print(volume);
  Serial.println(" cm3 ");

  // Mengontrol LED berdasarkan tinggi air
  if (tinggiair > 2.00) {
    digitalWrite(led3, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(relaybahaya, HIGH); // Mengaktifkan modul relay untuk tanda bahaya air
  } else if (tinggiair > 1.00) {
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(relaybahaya, LOW);
  } else if (tinggiair > 0.10) {
    digitalWrite(led3, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(relaybahaya, LOW);
  } else {
    digitalWrite(led3, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(relaybahaya, LOW);
  }

  Serial.println("");
  Serial.println("LEVEL KETINGGAN AIR");

  // Menampilkan output teks sesuai tinggi air
  if (tinggiair > 2.00) {
    Serial.println("Tinggi air tinggi III (bahaya)⛔");
  } else if (tinggiair > 1.00) {
    Serial.println("Tinggi air sedang II (waspada)⚠");
  } else if (tinggiair > 0.10) {
    Serial.println("Tinggi air rendah I (aman)‼");
  } else {
    Serial.println(" 🚨 UPS WADAH KOSONG 🚨 ");
    Serial.println(" MOHON ISI AIR DI WADAH ");
  }

  Serial.println("=============================");

  delay(3000); // Delay selama 3 detik sebelum pengulangan berikutnya
}