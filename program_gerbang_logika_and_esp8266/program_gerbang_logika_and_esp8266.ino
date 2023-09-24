//Keterangan pin untuk penggunaan LED
const int output_led = D8;

//Keterangan pin untuk penggunaan input
const int input1 = 2;
const int input2 = 3;
const int input3 = 4;
const int input4 = 5;

//Kebutuhan variabel-variabel yang akan digunakan saat proses perhitungan
int flag_data_serial = 0;
int read_input1 = 0;
int read_input2 = 0;
int read_input3 = 0;
int read_input4 = 0;

void proses_gerbang_logika_and();
void proses_menampilkan_data_serial();

void setup() 
{
  //Inisialisasi Serial
  Serial.begin(9600);             //Bitrate 9600bps
  
  //Pin LED dijadikan OUTPUT
  pinMode(output_led, OUTPUT);

  //Pin Saklar dijadikan INPUT
  pinMode(input1,INPUT);
  pinMode(input2,INPUT);
  pinMode(input3,INPUT);
  pinMode(input4,INPUT);
}

void loop() 
{
  //Pembacaan Input
  read_input1 = digitalRead(input1);
  read_input2 = digitalRead(input2);
  read_input3 = digitalRead(input3);
  read_input4 = digitalRead(input4);

  //Memanggil fungsi untuk menjalankan proses penyaringan gerbang logika AND dengan 4 input
  proses_gerbang_logika_and();
  
  //Memanggil fungsi untuk menjalankan proses penyaringan gerbang logika AND dengan 4 input
  proses_menampilkan_data_serial();
  delay(1000);
}

//Fungsi untuk memproses inputan yang diterima untuk dikeluarkan jadi output LED
void proses_gerbang_logika_and()
{
  // Tabel Kebenaran Logika AND untuk 4 Input dan 1 Output
  // INPUT 1  |INPUT 2  |INPUT 3  |INPUT 4  |OUTPUT|
  // 0  |0  |0  |0  |0|
  // 0  |0  |0  |1  |0|
  // 0  |0  |1  |0  |0|
  // 0  |0  |1  |1  |0|
  // 0  |1  |0  |0  |0|
  // 0  |1  |0  |1  |0|
  // 0  |1  |1  |0  |0|
  // 0  |1  |1  |1  |0|
  // 1  |0  |0  |0  |0|
  // 1  |0  |0  |1  |0|
  // 1  |0  |1  |0  |0|
  // 1  |0  |1  |1  |0|
  // 1  |1  |0  |0  |0|
  // 1  |1  |0  |1  |0|
  // 1  |1  |1  |0  |0|
  // 1  |1  |1  |1  |1|
  //Pemilihan Gerbang Logika (16case)
  if((read_input1 == LOW)&&(read_input2 == LOW)&&(read_input3 == LOW)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == LOW)&&(read_input3 == LOW)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == LOW)&&(read_input3 == HIGH)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == LOW)&&(read_input3 == HIGH)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == HIGH)&&(read_input3 == LOW)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == HIGH)&&(read_input3 == LOW)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == HIGH)&&(read_input3 == HIGH)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == LOW)&&(read_input2 == HIGH)&&(read_input3 == HIGH)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == LOW)&&(read_input3 == LOW)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == LOW)&&(read_input3 == LOW)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == LOW)&&(read_input3 == HIGH)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == LOW)&&(read_input3 == HIGH)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == HIGH)&&(read_input3 == LOW)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == HIGH)&&(read_input3 == LOW)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == HIGH)&&(read_input3 == HIGH)&&(read_input4 == LOW))
  {
    digitalWrite(output_led, LOW);          //Lampu LED mati
  }
  else if((read_input1 == HIGH)&&(read_input2 == HIGH)&&(read_input3 == HIGH)&&(read_input4 == HIGH))
  {
    digitalWrite(output_led, HIGH);          //Lampu LED Menyala
  }
}

//Fungsi untuk memproses data serial
void proses_menampilkan_data_serial()
{
  if(flag_data_serial == 0)
  {
    Serial.println("Bandwidth Allocation:");
    Serial.println("Port A 80   : 320bps");
    Serial.println("Port B 8080 : 0 bps");
    Serial.println("Port C 76   : 0 bps");
    Serial.println("Port D 3308 : 0 bps");
  }
  else if(flag_data_serial == 1)
  {
    Serial.println("Bandwidth Allocation:");
    Serial.println("Port A 80   : 0 bps");
    Serial.println("Port B 8080 : 320 bps");
    Serial.println("Port C 76   : 0 bps");
    Serial.println("Port D 3308 : 0 bps");
  }
  else if(flag_data_serial == 2)
  {
    Serial.println("Bandwidth Allocation:");
    Serial.println("Port A  80  : 0 bps");
    Serial.println("Port B 8080 : 0 bps");
    Serial.println("Port C 76   : 320 bps");
    Serial.println("Port D 3308 : 0 bps");
  }
  else if(flag_data_serial == 3)
  {
    Serial.println("Bandwidth Allocation:");
    Serial.println("Port A 80   : 0 bps");
    Serial.println("Port B 8080 : 0 bps");
    Serial.println("Port C 76   : 0 bps");
    Serial.println("Port D 3308 : 320 bps");
  }
  flag_data_serial++;
  if(flag_data_serial > 3)
  {
    flag_data_serial = 0;
  }
}
