#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#define in1 D7
#define in2 D6
#define in3 D5
#define in4 D4  
#define enA D3
#define enB D3

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Nusantara"
#define WIFI_PASSWORD "T4nahairku"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAWJ5be1p8gxQkv9x0DPTpMo67wpcAVlQE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://robotsumo-ef091-default-rtdb.asia-southeast1.firebasedatabase.app" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool maju, mundur, kiri, kanan;
uint16_t pot;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
bool signupOK = false;
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
void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  Firebase.RTDB.getBool(&fbdo, "/test/maju");
  maju = fbdo.boolData();
  Firebase.RTDB.getBool(&fbdo, "/test/mundur");
  mundur = fbdo.boolData();
  Firebase.RTDB.getBool(&fbdo, "/test/kiri");
  kiri = fbdo.boolData();
  Firebase.RTDB.getBool(&fbdo, "/test/kanan");
  kanan = fbdo.boolData();
  Firebase.RTDB.getInt(&fbdo, "/test/pot");
  pot = fbdo.intData();
  
  
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
