
// Charger la bibliothèque Wi-Fi
#include <ESP8266WiFi.h>

// Remplacez par vos identifiants réseau
const char* ssid     = "SUBIANTO";
const char* password = "americano";

// Définir le numéro de port du serveur Web sur 80
WiFiServer server(80);

// Variable pour stocker la requête HTTP
String header;

// Variables auxiliaires pour stocker l'état actuel de la sortie
String output5State = "off";
String output4State = "off";
String output16State = "off";
String output0State = "off";
String output2State = "off";
String output14State = "off";
String output12State = "off";
String output13State = "off";

// Attribuer des variables de sortie aux broches GPIO
const int output5 = 5;
const int output4 = 4;
const int output16 = 16;
const int output0 = 0;
const int output2 = 2;
const int output14 = 14;
const int output12 = 12;
const int output13 = 13;

void setup() {
  Serial.begin(115200);
  // Initialiser les variables de sortie en tant que sorties
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output16, OUTPUT);
  pinMode(output0, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output14, OUTPUT);
  pinMode(output12, OUTPUT);
  pinMode(output13, OUTPUT);
  // Set outputs to HIGH
  digitalWrite(output5, HIGH);
  digitalWrite(output4, HIGH);
  digitalWrite(output16, HIGH);
  digitalWrite(output0, HIGH);
  digitalWrite(output2, HIGH);
  digitalWrite(output14, HIGH);
  digitalWrite(output12, HIGH);
  digitalWrite(output13, HIGH);

  // Connectez-vous au réseau Wi-Fi avec SSID et mot de passe
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprimer l'adresse IP locale et démarrer le serveur Web
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Écoutez les clients entrants

  if (client) {                             // Si un nouveau client se connecte,
    Serial.println("New Client.");          // imprimer un message sur le port série
    String currentLine = "";                // faire une chaîne pour contenir les données entrantes du client
    while (client.connected()) {            // boucle alors que le client est connecté
      if (client.available()) {             // s'il y a des octets à lire du client,
        char c = client.read();             // lire un octet, puis
        Serial.write(c);                    // Imprimez le moniteur série
        header += c;
        if (c == '\n') {                    // si l'octet est un caractère de nouvelle ligne
          // si la ligne en cours est vide, vous avez deux caractères de nouvelle ligne à la suite.
          // c'est la fin de la requête HTTP du client, alors envoyez une réponse:
          if (currentLine.length() == 0) {
            // Les en-têtes HTTP commencent toujours par un code de réponse (par exemple, HTTP / 1.1 200 OK)
            // et un type de contenu pour que le client sache ce qui va arriver, puis une ligne vide:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // allume et éteint les GPIO
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, LOW);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, HIGH);
      } else if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("GPIO 16 on");
              output16State = "on";
              digitalWrite(output16, LOW);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("GPIO 16 off");
              output16State = "off";
              digitalWrite(output16, HIGH);
      } else if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("GPIO 16 on");
              output16State = "on";
              digitalWrite(output16, LOW);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("GPIO 16 off");
              output16State = "off";
              digitalWrite(output16, HIGH);
      } else if (header.indexOf("GET /0/on") >= 0) {
              Serial.println("GPIO 0 on");
              output0State = "on";
              digitalWrite(output0, LOW);
            } else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("GPIO 0 off");
              output0State = "off";
              digitalWrite(output0, HIGH);
      } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              output2State = "on";
              digitalWrite(output2, LOW);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              output2State = "off";
              digitalWrite(output2, HIGH);
      } else if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              output14State = "on";
              digitalWrite(output14, LOW);
            } else if (header.indexOf("GET /14/off") >= 0) {
              Serial.println("GPIO 14 off");
              output14State = "off";
              digitalWrite(output14, HIGH);
      } else if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output12State = "on";
              digitalWrite(output12, LOW);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              output12State = "off";
              digitalWrite(output12, HIGH);
      } else if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              output13State = "on";
              digitalWrite(output13, LOW);
            } else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              output13State = "off";
              digitalWrite(output13, HIGH);
        
            }
            
            // Afficher la page Web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS pour styler les boutons on / off
            // N'hésitez pas à modifier les attributs de couleur de fond et de taille de police pour les adapter à vos préférences.
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // En-tête de page Web
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Affiche l'état actuel et les boutons ON / OFF pour GPIO 5
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // Si output5State est désactivé, il affiche le bouton ON       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Affiche l'état actuel et les boutons ON / OFF pour GPIO 4  
            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            // Si output4State est désactivé, il affiche le bouton ON       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
      
      // Affiche l'état actuel et les boutons ON / OFF pour GPIO 16  
            client.println("<p>GPIO 16 - State " + output16State + "</p>");
            // Si output16State est désactivé, il affiche le bouton ON       
            if (output16State=="off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");     
            
      // Affiche l'état actuel et les boutons ON / OFF pour GPIO 0  
            client.println("<p>GPIO 0 - State " + output0State + "</p>");
            // Si output0State est désactivé, il affiche le bouton ON       
            if (output0State=="off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
      client.println("</body></html>");
      
      // Affiche l'état actuel et les boutons ON / OFF pour GPIO 2  
            client.println("<p>GPIO 2 - State " + output2State + "</p>");
            // Si output2State est désactivé, il affiche le bouton ON       
            if (output2State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
      client.println("</body></html>");
      
      // Affiche l'état actuel et les boutons ON / OFF pour GPIO 14  
            client.println("<p>GPIO 14 - State " + output14State + "</p>");
            // Si output14State est désactivé, il affiche le bouton ON       
            if (output14State=="off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            }  
      client.println("</body></html>");
      
      // Affiche l'état actuel et les boutons ON / OFF pour GPIO 12  
            client.println("<p>GPIO 12 - State " + output12State + "</p>");
            // Si output12State est désactivé, il affiche le bouton ON       
            if (output12State=="off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
      // Affiche l'état actuel et les boutons ON / OFF pour GPIO 13  
            client.println("<p>GPIO 13 - State " + output13State + "</p>");
            // Si output13State est désactivé, il affiche le bouton ON       
            if (output13State=="off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("</body></html>");     
            // La réponse HTTP se termine par une autre ligne vide
            client.println();
            // Sortez de la boucle while
            break;
          } else { // si vous avez une nouvelle ligne, effacez currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si vous avez autre chose que un caractère de retour de carriage,
          currentLine += c;      // l'ajouter à la fin de la currentLine
        }
      }
    }
    // Effacer la variable d'en-tête
    header = "";
    // Fermer la connexion
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
