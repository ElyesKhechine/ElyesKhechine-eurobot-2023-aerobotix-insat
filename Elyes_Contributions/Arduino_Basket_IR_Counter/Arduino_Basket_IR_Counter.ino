//capteur IR
const int cap = 3;
int counter = 0;
int c_old = 0;

void count() {
  int c_new = digitalRead(cap);
  if (!c_old && c_new)
    ++counter;
 
  Serial.print("c_new= ");
  Serial.println(c_new);
  Serial.print("c_old= ");
  Serial.println(c_old);
  c_old=c_new;
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(cap, INPUT);
  attachInterrupt(digitalPinToInterrupt(cap), count, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("counter= ");
  Serial.println(counter);
}
