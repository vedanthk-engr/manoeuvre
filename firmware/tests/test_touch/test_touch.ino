const int TOUCH1 = 4;
const int TOUCH2 = 15;

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH1, INPUT);
  pinMode(TOUCH2, INPUT);
}

void loop() {
  int t1 = digitalRead(TOUCH1);
  int t2 = digitalRead(TOUCH2);

  Serial.print("T1: ");
  Serial.print(t1);
  Serial.print("   T2: ");
  Serial.println(t2);

  delay(50);
}



