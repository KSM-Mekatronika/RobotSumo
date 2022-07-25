#define pb 36
bool val;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pb, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(pb);
  Serial.println(val);
}
