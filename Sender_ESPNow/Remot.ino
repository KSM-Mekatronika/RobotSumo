#include <espnow.h>
#include <ESP8266WiFi.h>

#define led_indikator D6
#define pin_maju D3
#define pin_mundur D8
#define pin_kiri D4
#define pin_kanan D0
#define pin_pot A0

bool maju, mundur, kiri, kanan;
uint16_t pot;

uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0x6A, 0xA4, 0x69};

typedef struct struct_message{
  bool nilai_maju, nilai_mundur, nilai_kiri, nilai_kanan, nilai_capit_gigit, nilai_capit_lepas;
  uint16_t nilai_pot;
}struct_message;

// Buat Objek
struct_message myData;


// Callback kalo data udah terkirim
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led_indikator,OUTPUT);
  digitalWrite(led_indikator,HIGH);
  // WiFi station mode
  WiFi.mode(WIFI_STA);
  
  // Initilize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  

  pinMode(pin_maju        , INPUT_PULLUP);
  pinMode(pin_mundur      , INPUT_PULLUP);
  pinMode(pin_kiri        , INPUT_PULLUP);
  pinMode(pin_kanan       , INPUT_PULLUP);
  pinMode(pin_pot, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  maju        = digitalRead(pin_maju);
  mundur      = digitalRead(pin_mundur);
  kiri        = digitalRead(pin_kiri);
  kanan       = digitalRead(pin_kanan);
  pot         = analogRead(pin_pot);
  pot         = map(pot, 0, 1023, 0, 335);

  Serial.print("Nilai maju : " + String(maju) 
  + "   | Nilai mundur : " + String(mundur) 
  + "  | Nilai kiri : " + String(kiri)
  + "  | Nilai kanan : " + String(kanan)
  );
  Serial.println();
  myData.nilai_maju         = maju;
  myData.nilai_mundur       = mundur;
  myData.nilai_kiri         = kiri;
  myData.nilai_kanan        = kanan;
  myData.nilai_pot          = pot;
  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  
  delay(100);
} 
