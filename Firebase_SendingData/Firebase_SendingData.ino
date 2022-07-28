#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

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

#define led_indikator D0
#define pin_maju D3
#define pin_mundur D7
#define pin_kiri D4
#define pin_kanan D1
#define pin_pot A0

bool maju, mundur, kiri, kanan;
uint16_t pot;

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  pinMode(led_indikator   , OUTPUT);
  pinMode(pin_maju        , INPUT_PULLUP);
  pinMode(pin_mundur      , INPUT_PULLUP);
  pinMode(pin_kiri        , INPUT_PULLUP);
  pinMode(pin_kanan       , INPUT_PULLUP);
  pinMode(pin_pot         , INPUT);
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
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
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  maju        = digitalRead(pin_maju);
  mundur      = digitalRead(pin_mundur);
  kiri        = digitalRead(pin_kiri);
  kanan       = digitalRead(pin_kanan);
  pot         = analogRead(pin_pot);
  pot         = map(pot, 0, 1023, 150, 255);
  

  Serial.println("Nilai maju : " + String(maju) 
  + "   | Nilai mundur : " + String(mundur) 
  + "  | Nilai kiri : " + String(kiri)
  + "  | Nilai kanan : " + String(kanan)
  + "  | Pot : " + String(pot)
  );
  Firebase.RTDB.setBool(&fbdo, "test/maju", maju);
  Firebase.RTDB.setBool(&fbdo, "test/mundur", mundur);
  Firebase.RTDB.setBool(&fbdo, "test/kiri", kiri);
  Firebase.RTDB.setBool(&fbdo, "test/kanan", kanan);
  Firebase.RTDB.setInt(&fbdo, "test/pot", pot);
  

  
}
