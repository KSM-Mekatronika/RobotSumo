int x_Pin = 36; //Nama alias pin A1 yaitu xPin
int y_Pin = 39; //Nama alias pin A2 yaitu xPin
int hasilX;
int hasilY;
void setup() {
Serial.begin(9600);
pinMode(x_Pin, INPUT);
pinMode(y_Pin, INPUT);
}
void loop() {
int x_Position = analogRead(x_Pin);
int y_Position = analogRead(y_Pin);
hasilX=map(x_Position,0,4095,0,255);
hasilY=map(y_Position,0,4095,0,255);
Serial.print("Nilai X : " + String(hasilX) + "\t" + "Nilai Y : " + String(hasilY) + "\n");
delay(100);
}
