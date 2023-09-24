#define echopin D7  // variabel echo pin memakai pin D7
#define trigpin D8  // variabel trigpin memakai pin D8
#define led1 D1     // variabel led1 memakai pin D1 (LED GREEN)
#define led2 D3     // variabel led2 memakai pin D3 (LED BLUE)
#define led3 D2     // variabel led3 memakai pin D2 (LED RED)

void setup() {
  // put your setup code here, to run once;
 pinMode(echopin, INPUT);    // variabel echopin di pakai sebagai input
 pinMode(trigpin, OUTPUT);   // variabel trigpin di pakai sebagai output

 pinMode(led1, OUTPUT);     // variabel led1 di pakai sebagai output
 pinMode(led2, OUTPUT);     // variabel led2 di pakai sebagai output
 pinMode(led3, OUTPUT);     // variabel led3 di pakai sebagai output

 Serial.begin(9600);
 }

void loop() {
  // put your main code here, to run repeatedly:
float luas_alas = 113.024;    // variabel luas_alas bernilai 113.024
float tinggi_gelas = 6.22;  // variabel tinggi_gelas bernilai 6.22

digitalWrite(trigpin, LOW);  // memastikan netral
delayMicroseconds(2);

digitalWrite(trigpin, HIGH); // memberi input
delayMicroseconds(10);

digitalWrite(trigpin, LOW);  // menerima input

int echotime = pulseIn(echopin, HIGH); // echotime bertipe data int 

float jarakpantul =  echotime * 0.034 / 2;    // jarak pantul
float tinggiair = tinggi_gelas - jarakpantul; // tinggi air
float volume = luas_alas * tinggiair;         // volume

// KELUARAN PROGRAM //

Serial.println("KELOMPOK : 20533388 YUSRON WIRAWANTO & 20533367 RAFI NUR ANNAS 🥷🥷🥷"); // nama kelompok

Serial.print("jarak permukaan : ");
Serial.print(jarakpantul);
Serial.print(" cm ");

Serial.print("tinggi permukaan : ");
Serial.print(tinggiair);
Serial.print(" cm ");

Serial.print("volume : ");
Serial.print(volume);
Serial.println(" cm3 ");

// PROGRAM IF UNTUK MENYALAHKAN LED //
/*
    Pertama, program memeriksa apakah tinggi air lebih besar dari 2.00 cm. Jika benar, maka pernyataan dalam blok if pertama akan dieksekusi:
        digitalWrite(led3, HIGH); // Menyalakan LED 3
        digitalWrite(led2, LOW); // Mematikan LED 2
        digitalWrite(led1, LOW); // Mematikan LED 1

    Hal ini berarti jika tinggi air lebih dari 2.00 cm, hanya LED 3 yang akan menyala, sementara LED 1 dan LED 2 akan mati.

    Jika tinggi air tidak memenuhi kondisi pada if pertama, program akan melanjutkan ke blok else if berikutnya. Pada blok ini, program memeriksa apakah tinggi air lebih dari 1.00 cm. Jika benar, maka pernyataan dalam blok else if kedua akan dieksekusi:
        digitalWrite(led3, LOW); // Mematikan LED 3
        digitalWrite(led2, HIGH); // Menyalakan LED 2
        digitalWrite(led1, LOW); // Mematikan LED 1

    Jadi, jika tinggi air lebih dari 1.00 cm, hanya LED 2 yang akan menyala, sementara LED 1 dan LED 3 akan mati.

    Jika tinggi air tidak memenuhi kondisi pada blok else if kedua, program akan melanjutkan ke blok else if terakhir. Pada blok ini, program memeriksa apakah tinggi air lebih dari 0.10 cm. Jika benar, maka pernyataan dalam blok else if ketiga akan dieksekusi:
        digitalWrite(led3, LOW); // Mematikan LED 3
        digitalWrite(led2, LOW); // Mematikan LED 2
        digitalWrite(led1, HIGH); // Menyalakan LED 1

    Dengan demikian, jika tinggi air lebih dari 0.10 cm, hanya LED 1 yang akan menyala, sementara LED 2 dan LED 3 akan mati.

    Jika tinggi air tidak memenuhi semua kondisi pada blok if dan else if sebelumnya, maka program akan menjalankan blok else terakhir:
        digitalWrite(led3, LOW); // Mematikan LED 3
        digitalWrite(led2, LOW); // Mematikan LED 2
        digitalWrite(led1, LOW); // Mematikan LED 1

    Ini berarti jika tinggi air tidak memenuhi kondisi pada if dan else if sebelumnya, maka semua LED (LED 1, LED 2, dan LED 3) akan mati.

Dengan demikian, program ini mengendalikan penyalakan LED berdasarkan tinggi air yang diukur, dengan prioritas penyalakan LED 3, LED 2, dan LED 1, sesuai dengan kondisi yang terpenuhi.
*/

if (tinggiair > 2.00) {
  digitalWrite(led3, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led1, LOW);
} else if (tinggiair > 1.00) {
  digitalWrite(led3, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led1, LOW);
} else if (tinggiair > 0.10) {
  digitalWrite(led3, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led1, HIGH);
} else {
  digitalWrite(led3, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led1, LOW);
}

Serial.println("");
Serial.println("LEVEL KETINGGAN AIR");

// PROGRAM IF UNTUK MENAMPILKAN OUTPUT TEKS SESUAI TINGGI AIR //
/*
apabila tinggi air > 0.10 cm maka program mengeluarkan output Tinggi air rendah I (siaga)??
apabila tinggi air > 1 cm maka program mengeluarkan output Tinggi air sedang II (waspada)??
apabila tinggi air > 2 cm maka program mengeluarkan output Tinggi air tinggi III (bahaya)??
*/

if (tinggiair > 2.00) {
  Serial.println("Tinggi air tinggi III (bahaya)⛔");
} else if (tinggiair > 1.00) {
  Serial.println("Tinggi air sedang II (waspada)⚠");
} else if (tinggiair > 0.10) {
  Serial.println("Tinggi air rendah I (siaga)‼");
} else {
  Serial.println(" 🚨 UPS WADAH KOSONG 🚨 ");
  Serial.println(" MOHON ISI AIR DI WADAH ");
}

Serial.println("=============================");

delay(3000);
}