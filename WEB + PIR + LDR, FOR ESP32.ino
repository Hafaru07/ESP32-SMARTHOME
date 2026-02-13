#include <WiFi.h>
#include <WebServer.h>

//////////////////////
// WIFI CONFIG
//////////////////////
const char* ssid = "OPERATOR";
const char* password = "khususoperatorsaja";

WebServer server(80);

//////////////////////
// PIN DECLARATION
//////////////////////

#define LDR_PIN 33
#define PIR_PIN 25

#define LAMP1 13
#define LAMP2 12
#define LAMP3 14
#define LAMP4 27
#define LAMP5 26

//////////////////////
// GLOBAL VARIABLES
//////////////////////

bool lamp1State = false;
bool lamp2State = false;

bool pirState = false;
bool lastPirRead = LOW;

bool lamp5State = false;

unsigned long darkStartTime = 0;

//////////////////////
// WEB PAGE
//////////////////////

void handleRoot() {

  String page = "<!DOCTYPE html><html>";
  page += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<style>";
  page += "body{font-family:Arial;text-align:center;background:#0f172a;color:white;}";
  page += ".card{background:#1e293b;padding:20px;margin:20px;border-radius:15px;}";
  page += "button{padding:15px 30px;margin:10px;font-size:18px;border:none;border-radius:10px;cursor:pointer;}";
  page += ".on{background:#22c55e;color:white;}";
  page += ".off{background:#ef4444;color:white;}";
  page += "</style></head><body>";

  page += "<h1>ESP32 SMART HOME</h1>";

  page += "<div class='card'>";
  page += "<h2>Lamp 1</h2>";
  page += "<p>Status: " + String(lamp1State ? "ON" : "OFF") + "</p>";
  page += "<a href='/lamp1/on'><button class='on'>ON</button></a>";
  page += "<a href='/lamp1/off'><button class='off'>OFF</button></a>";
  page += "</div>";

  page += "<div class='card'>";
  page += "<h2>Lamp 2</h2>";
  page += "<p>Status: " + String(lamp2State ? "ON" : "OFF") + "</p>";
  page += "<a href='/lamp2/on'><button class='on'>ON</button></a>";
  page += "<a href='/lamp2/off'><button class='off'>OFF</button></a>";
  page += "</div>";

  page += "</body></html>";

  server.send(200, "text/html", page);
}

//////////////////////
// SETUP
//////////////////////

void setup() {

  Serial.begin(115200);

  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);
  pinMode(LAMP4, OUTPUT);
  pinMode(LAMP5, OUTPUT);

  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  // Default PIR standby
  digitalWrite(LAMP3, HIGH);
  digitalWrite(LAMP4, LOW);

  //////////////////////
  // WIFI CONNECT
  //////////////////////
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //////////////////////
  // ROUTING
  //////////////////////
  server.on("/", handleRoot);

  server.on("/lamp1/on", [](){
    lamp1State = true;
    digitalWrite(LAMP1, HIGH);
    server.sendHeader("Location","/");
    server.send(303);
  });

  server.on("/lamp1/off", [](){
    lamp1State = false;
    digitalWrite(LAMP1, LOW);
    server.sendHeader("Location","/");
    server.send(303);
  });

  server.on("/lamp2/on", [](){
    lamp2State = true;
    digitalWrite(LAMP2, HIGH);
    server.sendHeader("Location","/");
    server.send(303);
  });

  server.on("/lamp2/off", [](){
    lamp2State = false;
    digitalWrite(LAMP2, LOW);
    server.sendHeader("Location","/");
    server.send(303);
  });

  server.begin();
}

//////////////////////
// LOOP
//////////////////////

void loop() {

  server.handleClient();

  //////////////////////
  // PIR TOGGLE SYSTEM
  //////////////////////
  bool pirRead = digitalRead(PIR_PIN);

  if (pirRead == HIGH && lastPirRead == LOW) {

    pirState = !pirState;

    if (pirState) {
      digitalWrite(LAMP3, LOW);
      digitalWrite(LAMP4, HIGH);
    } else {
      digitalWrite(LAMP3, HIGH);
      digitalWrite(LAMP4, LOW);
    }

    delay(300);
  }

  lastPirRead = pirRead;

  //////////////////////
  // LDR SMART SYSTEM
  //////////////////////

  int total = 0;
  for(int i=0;i<10;i++){
    total += analogRead(LDR_PIN);
    delay(2);
  }
  int ldrValue = total / 10;

  Serial.println(ldrValue);

  // ===== SETTING LEVEL =====
  int darkLevel = 3300;   // benar-benar gelap
  int brightLevel = 2600; // terang

  if (ldrValue > darkLevel) {

    if (darkStartTime == 0) {
      darkStartTime = millis();
    }

    // Harus gelap 3 detik
    if (millis() - darkStartTime > 3000) {
      lamp5State = true;
    }

  } 
  else if (ldrValue < brightLevel) {
    lamp5State = false;
    darkStartTime = 0;
  }

  digitalWrite(LAMP5, lamp5State ? HIGH : LOW);

}
