char cL;
int N1 = 12, N2 = 11, N3 = 10, N4 = 9;

void turnR(int pin1, int pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void turnL(int pin1, int pin2) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void Stop(int pin1, int pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);

  digitalWrite(N1, HIGH);
  digitalWrite(N2, HIGH);
  digitalWrite(N3, HIGH);
  digitalWrite(N4, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  cL = Serial.read();
  switch (cL) {
    case 'f':
      Serial.println("GOING FORWARD");
      Stop(N1, N2); Stop(N3, N4);
      turnR(N1, N2); turnR(N3, N4);
      break;
      
    case 'b':
      Serial.println("GOING BACKWARD");
      Stop(N1, N2); Stop(N3, N4);
      turnL(N1, N2); turnL(N3, N4);
      break;
      
    case 'l':
      Serial.println("GOING LEFT");
      Stop(N1, N2); Stop(N3, N4);
      turnR(N1, N2); turnL(N3, N4);
      break;

    case 'r':
      Serial.println("GOING RIGHT");
      Stop(N1, N2); Stop(N3, N4);
      turnL(N1, N2); turnR(N3, N4);
      break;

    case 's':
      Serial.println("STOPING");
      Stop(N1, N2); Stop(N3, N4);
      break;   
  }
  
}
