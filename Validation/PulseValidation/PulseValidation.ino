#define SENSOR 12
#define TIME_DELAY 0x0505
void setup() {
  // put your setup code here, to run once:
  delay(100);
  Serial.begin(9600);
  digitalWrite(SENSOR, HIGH);
  pinMode(SENSOR, OUTPUT);
  
  for(int i=0; i<215; i++)
  {
    delay(TIME_DELAY);
    digitalWrite(SENSOR, HIGH);
    digitalWrite(SENSOR, LOW);
  }
}

void loop() {
}
