#define triggerPin D8   // Pin untuk mengirimkan sinyal trigger ke sensor ultrasonik
#define echoPin D7      // Pin untuk menerima sinyal echo dari sensor ultrasonik
#define relayPin D2     // Pin untuk mengontrol relay

void setup() {
  Serial.begin(9600);   // Memulai komunikasi serial dengan baud rate 9600
  pinMode(relayPin, OUTPUT);   // Mengatur pin relay sebagai OUTPUT
  pinMode(triggerPin, OUTPUT);   // Mengatur pin trigger sebagai OUTPUT
  pinMode(echoPin, INPUT);   // Mengatur pin echo sebagai INPUT
}

void loop() {
  long durasi, jarak;
  
  digitalWrite(triggerPin, LOW);   // Mengatur pin trigger menjadi LOW
  delayMicroseconds(2);   // Memberikan jeda 2 mikrodetik
  digitalWrite(triggerPin, HIGH);   // Mengatur pin trigger menjadi HIGH
  delayMicroseconds(10);   // Memberikan jeda 10 mikrodetik
  digitalWrite(triggerPin, LOW);   // Mengatur pin trigger menjadi LOW
  
  durasi = pulseIn(echoPin, HIGH);   // Membaca durasi dari pin echo ketika sinyal HIGH diterima
  jarak = (durasi/2)  / 29.1;   // Menghitung jarak berdasarkan durasi yang diukur
  
  Serial.print("Jarak: ");   // Mencetak teks "Jarak: "
  Serial.print(jarak);   // Mencetak nilai jarak
  Serial.println(" cm");   // Mencetak teks " cm"
  
  if (jarak >= 5) {   // Jika jarak lebih besar atau sama dengan 5
    digitalWrite(relayPin, LOW);   // Mengatur pin relay menjadi LOW
  } else {   // Jika jarak kurang dari 5
    digitalWrite(relayPin, HIGH);   // Mengatur pin relay menjadi HIGH
  }
  
  delay(1000);   // Memberikan jeda 1 detik
}