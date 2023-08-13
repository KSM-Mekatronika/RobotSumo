#include <WiFi.h>

const char* ssid = "IndieHome";        // Change this to the desired Access Point name
const char* password = "pastimenang23";  // Change this to the desired Access Point password
const int channel = 10;                 // WiFi Channel number between 1 and 13
const bool hide_SSID = true;            // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
const int max_connection = 1;           // Maximum simultaneous connected clients on the AP

int in1 = 5;   //KIRI
int in2 = 17;  //KIRI
int in3 = 16;  //KANAN
int in4 = 4;   //KANAN

WiFiServer ESPserver(80);

void setup() {
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Setting up Access Point: ");
  Serial.println(ssid);

  // Set ESP32 as an Access Point
  WiFi.softAP(ssid, password, channel, hide_SSID, max_connection);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  ESPserver.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = ESPserver.available();
  if (!client) {
    return;
  }

  Serial.print("New Client");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = 0;
  if (request.indexOf("/MAJU") != -1) {
    Serial.println("Maju");
    maju();
    value = 1;
  }
  if (request.indexOf("/MUNDUR") != -1) {
    Serial.println("Mundur");
    mundur();
    value = 2;
  }
  if (request.indexOf("/KIRI") != -1) {
    Serial.println("Kiri");
    kiri();
    value = 3;
  }
  if (request.indexOf("/KANAN") != -1) {
    Serial.println("Kanan");
    kanan();
    value = 4;
  }
  if (request.indexOf("/BERHENTI") != -1) {
    Serial.println("Berhenti");
    berhenti();
    value = 5;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Status: ");

  if (value == 1) {
    client.print("Maju");
  }
  if (value == 2) {
    client.print("Mundur");
  }
  if (value == 3) {
    client.print("Kiri");
  }
  if (value == 4) {
    client.print("Kanan");
  }
  if (value == 5) {
    client.print("Berhenti");
  }

  delay(1);
  Serial.println("Client Disconnected");
  Serial.println("");
}

void mundur() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void maju() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void kanan() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void kiri() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void berhenti() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}