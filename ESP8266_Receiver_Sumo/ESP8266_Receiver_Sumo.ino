#include <ESP8266WiFi.h>
#include <espnow.h>
                                        
#define in1 D7
#define in2 D6
#define in3 D5
#define in4 D4  
#define enA D3
#define enB D3


bool maju, mundur, kiri, kanan;
uint16_t pot;

typedef struct struct_message{
  bool nilai_maju, nilai_mundur, nilai_kiri, nilai_kanan;
  uint16_t nilai_pot;
}struct_messange;

struct_message myData;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  maju        = myData.nilai_maju;
  mundur      = myData.nilai_mundur;
  kiri        = myData.nilai_kiri;
  kanan       = myData.nilai_kanan;
  pot         = myData.nilai_pot;
}

// Callback


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  digitalWrite(enA, 0);
  digitalWrite(enB, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(
    "Nilai maju : " + String(maju) + 
    "   | Nilai mundur : " + String(mundur) + 
    "  | Nilai kiri : " + String(kiri)+ 
    "  | Nilai kanan : " + String(kanan)+ 
    "  | Nilai Pot : " + String(pot)
   );
 
  if(maju == LOW){
    gerak_maju();
  }
  if(mundur == LOW){
    gerak_mundur();
  }
  if(kiri == LOW){
    gerak_kiri();
  }
  if(kanan == LOW){
    gerak_kanan();
  }
  if((maju == HIGH) and (mundur == HIGH) and (kanan == HIGH) and (kiri == HIGH)){
    berhenti();
  }
}

void gerak_maju(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, pot);
  analogWrite(enB, pot);
  Serial.println("Maju");
}
void gerak_kiri(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, pot-50);
  analogWrite(enB, pot-50);
  Serial.println("Kiri");
}
void gerak_kanan(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, pot-50);
  analogWrite(enB, pot-50);
  Serial.println("Kanan");
}
void gerak_mundur(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, pot);
  analogWrite(enB, pot);
  Serial.println("Mundur");
}

void berhenti(){
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  Serial.println("Berhenti");
}
