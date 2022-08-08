#include <ESP8266WiFi.h>

const char* ssid = "Nusantara";
const char* password = "T4nahairku";

int led = 16;
int in1 = 5;  //KIRI
int in2 = 4;  //KIRI
int in3 = 0;  //KANAN
int in4 = 2;  //KANAN

WiFiServer ESPserver(80);

void setup() {
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  delay(5000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(led, HIGH);

  ESPserver.begin();
  Serial.println("Server started");

  Serial.print("IP address: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
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

void maju() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void mundur() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void kiri() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void kanan() {
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
