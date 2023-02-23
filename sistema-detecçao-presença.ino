//Placa ESP32 Dev Module
#include <Ultrasonic.h>
#include <WiFi.h>
/*
 * PINAGEM:
 * 1)Ultrassom:
 * trigger : D4
 * echo: D5
 * 2)LED:
 * pino D22
 * 3)Buzzer:
 * pino D21
 */
 
//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5

//Colocar nome e senha da rede para conectar ESP32 com WiFi
const char* ssid     = "nome-da-rede";
const char* password = "senha-da-rede";
WiFiServer server(80);

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonico(pino_trigger, pino_echo);

void setup()
{
  Serial.begin(115200);
  delay(3000);
  Serial.println("Lendo dados do sensor…");

  //pino do LED
  pinMode(22, OUTPUT);

  //pino do buzzer
  pinMode(21, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //aguarda conexão WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(5000);

  server.begin();


}

int value = 0;
int a = 0;
int b = 0;
float cmMsec;
//long microsec = ultrasonico.timing();

void loop()
{
  //Le as informacoes do sensor em cm
  cmMsec = ultrasonico.distanceRead(CM); 
  //cmMsec = ultrasonico.convert(microsec, Ultrasonic::CM);
  Serial.println(cmMsec);

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            //            client.print("Clique <a href=\"/H\">ACENDER LED</a> . <br>");
            //            client.print("Clique <a href=\"/L\">DESLIGAR LED</a> . <br>");

            client.println("<html>");
            client.println("<head><meta content=\"width=device-width, inicial-scale=1\">");
            client.println("<style>html{ margin:0px auto;text-allign:center;}");
            client.println(".botao_liga{background-color: #00FF00; color:black; font-weight: bold; padding:15px 40px; border-radius:25px;}");
            client.println(".botao_desliga{background-color: #FF0000; color:black; font-weight: bold; padding:15px 40px; border-radius:25px;}</style></head>");
            client.println("<body><h1>Web Server com ESP32</h1>");
            client.println("<body><h2>Sistema de alarme domiciliar</h2>");

            client.print("<p><a href=\"/H\"><button class=\"botao_liga\">LIGA</button></a></p>");
            client.print("<p><a href=\"/L\"><button class=\"botao_desliga\">DESLIGA</button></a></p>");

            //client.println("<p>Sistema ligado.</p>");

            client.println("</body></html>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        //Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //digitalWrite(22, HIGH);               // GET /H turns the LED on
          b = 1;
          a = 0;

        }
        if (currentLine.endsWith("GET /L")) {
          //digitalWrite(22, LOW);                // GET /L turns the LED off
          a = 0;
          b = 0;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

  if (cmMsec < 30) {
    a = 1;
  }
  if (a == 1 && b == 1) {
    int tempo = 1000;
    //Acende o led
    digitalWrite(22, HIGH);
    tone(21, 400, tempo);
    delay(tempo);
    digitalWrite(22, LOW);
    tone(21, 250, tempo);
  }
  delay(1000);
}
