//REMOT
#include <esp_now.h>
#include <WiFi.h>

#define led_indikator 22
#define pin_maju 23
#define pin_mundur 25
#define pin_kiri 26
#define pin_kanan 27
#define pin_pot 34

bool maju, mundur, kiri, kanan;
uint16_t pot;

uint8_t broadcastAddress[] = {0x24, 0xD7, 0xEB, 0x10, 0x4F, 0x20};

typedef struct struct_message{
  bool nilai_maju, nilai_mundur, nilai_kiri, nilai_kanan;
  uint16_t nilai_pot;
}struct_message;

// Buat Objek
struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // WiFi station mode
  WiFi.mode(WIFI_STA);
  
  // Initilize ESP-NOW
if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
    
  pinMode(led_indikator   , OUTPUT);
  pinMode(pin_maju        , INPUT_PULLUP);
  pinMode(pin_mundur      , INPUT_PULLUP);
  pinMode(pin_kiri        , INPUT_PULLUP);
  pinMode(pin_kanan       , INPUT_PULLUP);
  pinMode(pin_pot         , INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  maju        = digitalRead(pin_maju);
  mundur      = digitalRead(pin_mundur);
  kiri        = digitalRead(pin_kiri);
  kanan       = digitalRead(pin_kanan);
  pot         = analogRead(pin_pot);
  pot         = map(pot, 0, 1023, 150, 255);
  

  Serial.print("Nilai maju : " + String(maju) 
  + "   | Nilai mundur : " + String(mundur) 
  + "  | Nilai kiri : " + String(kiri)
  + "  | Nilai kanan : " + String(kanan)
  + "  | Pot : " + String(pot)
  );
  Serial.println();
  myData.nilai_maju         = maju;
  myData.nilai_mundur       = mundur;
  myData.nilai_kiri         = kiri;
  myData.nilai_kanan        = kanan;
  myData.nilai_pot          = pot;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}
