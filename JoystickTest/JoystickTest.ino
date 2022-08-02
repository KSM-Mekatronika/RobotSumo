#define xpin 36
uint16_t x_value;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(xpin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  x_value = analogRead(xpin);
  Serial.print(x_value);
}
