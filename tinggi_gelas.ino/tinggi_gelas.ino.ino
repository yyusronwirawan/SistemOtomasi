int echo_pin = D7;  //variabel echo pin menggunakan pin D7
int trig_pin = D8;  //variabel trig pin menggunakan pin D8
long echotime;

void setup() {
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  digitalWrite(trig_pin, LOW);
}

void loop() {
  float jarak_pantul, tinggi_gelas; 
  digitalWrite(trig_pin, HIGH); //perintah untuk memastikan sensor ultrasonik belum memancarkan gelombang
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW); //sensor ultrasonik memancarkan gelombang ultrasoniknya
  echotime = pulseIn(echo_pin, HIGH);

  jarak_pantul = 0.034 * (float)echotime / 2;

  Serial.print("tinggi_gelas = ");
  Serial.print(jarak_pantul);
  Serial.println(" cm");  
  Serial.println("========================");

  delay(2000);
}
