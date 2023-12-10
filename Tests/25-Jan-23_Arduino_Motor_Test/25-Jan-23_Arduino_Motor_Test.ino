const int moteurAvant = 11;
const int moteurDeriere = 10;

void Forward(int pwm)
{
  analogWrite(moteurAvant, pwm);
  analogWrite(moteurDeriere, 0);
}

void Backward(int pwm)
{
  analogWrite(moteurAvant, 0);
  analogWrite(moteurDeriere, pwm);
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(moteurAvant, OUTPUT);
  pinMode(moteurDeriere, OUTPUT);

  digitalWrite(moteurAvant, HIGH);
  digitalWrite(moteurDeriere, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Forward (255); 
}
