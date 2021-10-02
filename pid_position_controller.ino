
int En = 9;
int Pp = 10;
int Np = 11;

void setup() {
  Serial.begin(115200);

}

void loop() {
  double s = map(analogRead(A0), 0, 1023, 0, 255);
  pidAlgo(s);
}

double prevErr = 0;
double totErr = 0;
int kp = 5;
int kd = 10;
float ki = 0.01;

void pidAlgo(double sY) {
  
  double sp = -1 * map(analogRead(A1), 0, 1023, 0, 90); //Get the set point from the potentiometer
  double error = sp - sY; //obtain the error from substracting

  double Pterm = error * kp;
  double Dterm = (error - prevErr) * kd;
  double Iterm = (double)ki * totErr;

  double pidVal =  Pterm + Dterm + Iterm ; 

  int mspeed = (int)pidVal;
  drM(-mspeed);

  prevErr = error;
  totErr += error;
  Serial.print(sp);
  Serial.print(",");
  Serial.print(sY);
  Serial.print(",");
  Serial.println(pidVal);
}

void drM(int m1) {
  if (m1 >= 255) {
    m1 = 255;
  }
  if (m1 <= -255) {
    m1 = -255;
  }
  if (m1 < 0) {
    m1 = -1 * m1;
    digitalWrite(Pp, LOW); digitalWrite(Np, HIGH);
    analogWrite(En, m1);
  } else {
    digitalWrite(Pp, HIGH); digitalWrite(Np, LOW);
    analogWrite(En, m1);
  }
}
