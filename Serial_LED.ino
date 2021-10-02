void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
void loop() {
  if (Serial.available()) {
    byte input = Serial.read();
    char c = (char)input;
    Serial.println(c);
    if (c == '1') {
      digitalWrite(13, HIGH);
    }
    if (c == '0') {
      digitalWrite(13, LOW);
    }
  }
}
