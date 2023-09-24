#include <ESP8266WiFi.h>

const char* ssid = "SUBIANTO"; // Nama SSID jaringan WiFi
const char* password = "americano"; // Kata sandi jaringan WiFi

WiFiServer server(80); // Membuat objek server untuk menerima koneksi pada port 80

String header;

String output1State = "off";

const int relay1 = D3; // Menggunakan pin D0 sebagai output untuk relay

unsigned long currentTime = millis();
unsigned long previousTime = 0;

const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  pinMode(relay1, OUTPUT);
  
  digitalWrite(relay1, HIGH); // Mengatur pin relay ke status HIGH (off)

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Menampilkan alamat IP WiFi modul ESP8266
  server.begin();
}

void loop() {
  // Place your code here for continuous execution
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /0/on") >= 0) {
              Serial.println("GPIO 0 on");
              output1State = "on";
              digitalWrite(relay1, LOW);
            } else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("GPIO 0 off");
              output1State = "off";
              digitalWrite(relay1, HIGH);
            }

            client.println("<!DOCTYPE HTML><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display; inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>Simulasi web server relay</h1>");
            client.println("<p>Relay 1 " + output1State + "</p>");
            
            if (output1State == "off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">MURUP</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">KETAM</button></a></p>");
            }

            client.println("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
