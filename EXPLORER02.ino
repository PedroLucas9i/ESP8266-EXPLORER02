#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP8266-EXPLORER02";
const char* password = "87654321";

ESP8266WebServer server(80);

String header;

#define pinEnableMotorA 14
#define pinEnableMotorB 12

#define pinSentido1MotorA 16
#define pinSentido2MotorA 5

#define pinSentido1MotorB 4
#define pinSentido2MotorB 2

int front_back = 0;
int sentido = 0;

int ajusteMotorA = 0;
int ajusteMotorB = 0;

void setup() {
  
  pinMode(pinEnableMotorA, OUTPUT);
  pinMode(pinEnableMotorB, OUTPUT);

  pinMode(pinSentido1MotorA, OUTPUT);
  pinMode(pinSentido2MotorA, OUTPUT);
  pinMode(pinSentido1MotorB, OUTPUT);
  pinMode(pinSentido2MotorB, OUTPUT);

  //parar motores no início
  analogWrite(pinEnableMotorA, 0);
  analogWrite(pinEnableMotorB, 0);
  
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/front", Vfront);
  server.on("/back", Vback);
  server.on("/left", Vleft);
  server.on("/right", Vright);
  server.on("/stop", Vstop);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  command();
}
void handleRoot() {
  String htmlPage = "<!DOCTYPE html><html lang=\"pt\">";
  htmlPage += "<head><meta charset=\"UTF-8\">";
  htmlPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  htmlPage += "<title>EXPLORER02</title>";
  htmlPage += "<style>body button { background-color: black; width: 10vw; height: 10vw; font-size: 5vw}";
  htmlPage += "#A { position: absolute; margin-left: 45vw; margin-top: 20vw;}";
  htmlPage += "#B { position: absolute; margin-left: 35vw; margin-top: 30vw;}";
  htmlPage += "#C { position: absolute; margin-left: 55vw; margin-top: 30vw;}";
  htmlPage += "#D { position: absolute; margin-left: 45vw; margin-top: 40vw;}";
  htmlPage += "#E { position: absolute; margin-left: 45vw; margin-top: 30vw;}</style>";
  htmlPage += "</head>";
  htmlPage += "<body>";
  htmlPage += "<button id=\"A\" onclick=\"handleButtonClick('/front')\">⬆️</button>";
  htmlPage += "<button id=\"B\" onclick=\"handleButtonClick('/right')\">⬅️</button>";
  htmlPage += "<button id=\"C\" onclick=\"handleButtonClick('/left')\">➡️</button>";
  htmlPage += "<button id=\"D\" onclick=\"handleButtonClick('/back')\">⬇️</button>";
  htmlPage += "<button id=\"E\" onclick=\"handleButtonClick('/stop')\">⏹️</button>";
  htmlPage += "<script>";
  htmlPage += "function handleButtonClick(link) {";
  htmlPage += "  var xhttp = new XMLHttpRequest();";
  htmlPage += "  xhttp.open('GET', link, true);";
  htmlPage += "  xhttp.send();";
  htmlPage += "}";
  htmlPage += "</script>";
  htmlPage += "</body></html>";

  server.send(200, "text/html", htmlPage);
}

void Vfront() {
  front_back += 51;
  if (front_back > 255) {
    front_back = 255;
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void Vback() {
  front_back -= 51;
  if (front_back < -255) {
    front_back = -255;
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void Vleft() {
  sentido -= 30;
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void Vright() {
  sentido += 30;
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}
void Vstop() {
  front_back = 0;
  sentido = 0;
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}
void command() {
  if (front_back > 0) {
    digitalWrite(pinSentido1MotorA, HIGH);
    digitalWrite(pinSentido2MotorA, LOW);
    digitalWrite(pinSentido1MotorB, HIGH);
    digitalWrite(pinSentido2MotorB, LOW);
    analogWrite(pinEnableMotorB, front_back + ajusteMotorB);
    analogWrite(pinEnableMotorA, front_back + ajusteMotorA);
  } else {
    digitalWrite(pinSentido1MotorA, LOW);
    digitalWrite(pinSentido2MotorA, HIGH);
    digitalWrite(pinSentido1MotorB, LOW);
    digitalWrite(pinSentido2MotorB, HIGH);
    analogWrite(pinEnableMotorB, abs(front_back) + ajusteMotorB);
    analogWrite(pinEnableMotorA, abs(front_back) + ajusteMotorA);
  }
  if (sentido > 0) {
    digitalWrite(pinSentido1MotorA, HIGH);
    digitalWrite(pinSentido2MotorA, LOW);
    digitalWrite(pinSentido1MotorB, LOW);
    digitalWrite(pinSentido2MotorB, HIGH);
    analogWrite(pinEnableMotorB, sentido + ajusteMotorB);
    analogWrite(pinEnableMotorA, sentido + ajusteMotorA);
  }
  if (sentido < 0) {
    digitalWrite(pinSentido1MotorA, LOW);
    digitalWrite(pinSentido2MotorA, HIGH);
    digitalWrite(pinSentido1MotorB, HIGH);
    digitalWrite(pinSentido2MotorB, LOW);
    analogWrite(pinEnableMotorB, abs(sentido) + ajusteMotorB);
    analogWrite(pinEnableMotorA, abs(sentido) + ajusteMotorA);
  }
  delay(20);
}
