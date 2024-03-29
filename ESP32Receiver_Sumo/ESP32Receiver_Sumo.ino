#include <esp_now.h>
#include <WiFi.h>

#define in1 18
#define in2 19
#define in3 33
#define in4 32



bool maju, mundur, kiri, kanan;
uint16_t pot;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool nilai_maju, nilai_mundur, nilai_kiri, nilai_kanan;
  uint16_t nilai_pot;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  maju        = myData.nilai_maju;
  mundur      = myData.nilai_mundur;
  kiri        = myData.nilai_kiri;
  kanan       = myData.nilai_kanan;
  pot         = myData.nilai_pot;
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  Serial.println(
    "Nilai maju : " + String(maju) +
    "   | Nilai mundur : " + String(mundur) +
    "  | Nilai kiri : " + String(kiri) +
    "  | Nilai kanan : " + String(kanan) +
    "  | Nilai Pot : " + String(pot)
  );
  if (maju == HIGH) {
    gerak_maju();
  }
  if (mundur == HIGH) {
    gerak_mundur();
  }
  if (kiri == HIGH) {
    gerak_kiri();
  }
  if (kanan == HIGH) {
    gerak_kanan();
  }
  if ((maju == LOW) and (mundur == LOW) and (kanan == LOW) and (kiri == LOW)) {
    berhenti();
  }

}


void gerak_maju() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Maju");
}
void gerak_kiri() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Kiri");
}
void gerak_kanan() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Kanan");
}
void gerak_mundur() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Mundur");
}

void berhenti() {
  Serial.println("Berhenti");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
