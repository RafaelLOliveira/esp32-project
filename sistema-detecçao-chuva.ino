/*
 * Atividade prática 4 - Arandu - RESPOSTA
 */

//Inclusão das bibliotecas
#include <WiFi.h>  
#include <WebServer.h>
//#include <Wire.h>

int pino_d = 21; //Pino ligado ao D0 do sensor
int pino_a = 35; //Pino ligado ao A0 do sensor  
int val_d = 0; //Armazena o valor lido do pino digital
int val_a = 0; //Armazena o valor lido do pino analogico
// Portas ligadas aos leds
int pino_led_verde = 5;
int pino_led_vermelho = 4;

String statusChuva ; 

const char* ssid     = "nome-da-rede";
const char* password = "senha-da-rede";

WebServer server(80);


void conectado() {                                                                //Sub-rotina para caso o servidor fique online
  server.send(200, "text/html", html(statusChuva));     //Envia ao servidor, em formato HTML, o nosso script, com os parâmetros de pressão e temperatura
}

void nao_encontrado() {                                                           //Sub-rotina para caso seja retornado um erro
  server.send(404, "text/plain", "Não encontrado");                                   //Retorna a mensagem de erro em caso de um retorno 404
}

void setup()
{
  Serial.begin(115200);
  delay(3000);
  Serial.println("Lendo dados do sensor…");

  // Define os pinos do sensor como entrada
  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);

  // Define os pinos dos leds como saida
  pinMode(pino_led_verde, OUTPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", conectado);
  server.onNotFound(nao_encontrado);

  server.begin();

  delay(5000);

}

void loop()
{
  server.handleClient();  

  
  //Le e arnazena o valor do pino digital
  val_d = digitalRead(pino_d);
  //Le e armazena o valor do pino analogico
  val_a = analogRead(pino_a);
  //Envia as informacoes para o serial monitor
  Serial.print("Valor digital : ");
  Serial.print(val_d);
  Serial.print(" - Valor analogico : ");
  Serial.println(val_a);



  // Acende o led de acordo com a intensidade
  if (val_a > 1300)
  {
    // Acende led verde - intensidade baixa
    digitalWrite(pino_led_verde, HIGH);
    digitalWrite(pino_led_vermelho, LOW);
    statusChuva = "Normal";
  }
  if (val_a < 1300)
  {
    // Acende led vermelho - intensidade alta
    digitalWrite(pino_led_verde, LOW);
    digitalWrite(pino_led_vermelho, HIGH);
    statusChuva = "Está chovendo, feche as janelas";
  }
  delay(1000);
}






String html(String statusChuva) {                                   //Variável que armazenará o script HTML
  String cd = "<!DOCTYPE html>\n";
  cd += "<html lang=\"pt-br\">\n";
  cd += "<head>\n";
  cd += "<meta charset=\"UTF-8\">\n";
  cd += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  cd += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

  cd += "<title>Dados dos sensores</title>\n";

  cd += "<style>\n";
  cd += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  cd += "body{margin-top: 50px;} \n";
  cd += "h1 {color: #444444; margin: 50px auto 30px;}\n";
  cd += "p {font-size: 24px; color: #444444; margin-bottom: 10px;}\n";
  cd += "</style>\n";

  cd += "</head>\n";
  cd += "<body>\n";
  cd += "<div id=\"webpage\">\n";
  cd += "<h1>Dados do sensor</h1>\n";
  cd += "<p>Status: ";
  cd += (String)statusChuva;
  cd += "</p>\n";
  cd += "</div>\n";
  cd += "</body>\n";  
  cd += "</html>\n";

  return cd;                                                                      //Retorna o script                                             
} 
