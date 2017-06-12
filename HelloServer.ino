#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Net Virtua Jonatan";
const char* password = "16807864";

//Pin connected to ST_CP of 74HC595
int latchPin = 4;
//Pin connected to SH_CP of 74HC595
int clockPin = 14;
////Pin connected to DS of 74HC595
int dataPin = 5;


ESP8266WebServer server(80);


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message  += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup(void){
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(9600);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/pisca_led", []{
    server.send(200, "text/plain", "Leds Piscando");
    for (int i=0; i<3; i++){
      for (int numberToDisplay = 1; numberToDisplay < 8; numberToDisplay++) {
        // take the latchPin low so 
        // the LEDs don't change while you're sending in bits:
        digitalWrite(latchPin, LOW);
        // shift out the bits:
        shiftOut(dataPin, clockPin, LSBFIRST, pow(2,numberToDisplay));  
    
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        delay(20);
      }
      for (int numberToDisplay = 7; numberToDisplay > 0; numberToDisplay--) {
        // take the latchPin low so 
        // the LEDs don't change while you're sending in bits:
        digitalWrite(latchPin, LOW);
        // shift out the bits:
        shiftOut(dataPin, clockPin, LSBFIRST, pow(2,numberToDisplay));  
    
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        delay(20);
      }
    }
  });

  server.on("/37", []{
    server.send(200, "text/plain", "Número 37");
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 37); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
  });

  server.on("/todos", []{
    server.send(200, "text/plain", "Todos acesos");
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 255); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
  });
  server.on("/1", []{
    server.send(200, "text/plain", "numero 1");
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 1); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
  });
  server.on("/128", []{
    server.send(200, "text/plain", "numero 128");
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 128); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
  });

  server.on("/1024", []{
    server.send(200, "text/plain", "numero 1024");
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 1024); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
  });

  server.on("/512", []{
    server.send(200, "text/plain", "numero 512");
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 512); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
  });
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
