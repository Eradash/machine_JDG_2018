//#include <PS3BT.h>
//#include <SPI.h>
#include <Servo.h>

//USB Usb;
//BTD Btd(&Usb);
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0xEA, 0x84, 0x97);



#include <PS3USB.h>
#include <SPI.h>

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00, 0x15, 0x83, 0xEA, 0x84, 0x97);

int M1_EN = 23;
int M2_EN = 22;
int M1_PWM = 7;
int M2_PWM = 6;

int P_PINCE = 2;
int P_PINCE_1 = 3;
int P_PINCE_2 = 5;
int P_PINCE_BOUT = 4;

int ML_EN = 25;
int ML_PWM = 24;

int D_PULSE = 27;
int D_DIR = 28;
int D_EN = 26;

Servo PINCE;
Servo PONT_1;
Servo PONT_2;
Servo PINCE_BOUT; 

bool pince_haute = false;

int pince_HAUT = 0;
int pince_BAS = 125;

void setup() {
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  else
  {
    Serial.print(F("\r\nThis is a test"));
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  pinMode(M1_EN, OUTPUT);
  pinMode(M2_EN, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_PWM, OUTPUT);

  pinMode(ML_EN, OUTPUT);
  pinMode(ML_PWM, OUTPUT);

  PINCE.attach(P_PINCE);
  PONT_1.attach(P_PINCE_1);
  PONT_2.attach(P_PINCE_2);
  PINCE_BOUT.attach(P_PINCE_BOUT);

  pinMode(D_PULSE, OUTPUT);
  pinMode(D_DIR, OUTPUT);
  pinMode(D_EN, OUTPUT);

  digitalWrite(M1_EN, 0);
  digitalWrite(M2_EN, 0);
  analogWrite(M1_PWM, 0);
  analogWrite(M2_PWM, 0);

  digitalWrite(D_EN, LOW);

  PINCE_BOUT.write(75);
}

void monterPince(){
  Serial.print(F("\r\nMontage de la pince: "));
  for(int i = pince_BAS; i > pince_HAUT; i--){
    PINCE.write(i);
    delay(10);
  }
}

void descendrePince(){
  Serial.print(F("\r\nPince en bas: "));
   for(int i = pince_HAUT; i < pince_BAS; i++){
    PINCE.write(i);
    delay(10);
  }
}


void loop() {
  Usb.Task();
  
  ////// MOTEURS
  if (true) {
    int M1 = map(PS3.getAnalogHat(LeftHatY), 255, 0, -255, 255);
    int M2 = map(PS3.getAnalogHat(RightHatY), 255, 0, -255, 255);

    if(M1 > 20){
      digitalWrite(M1_EN, HIGH);
      analogWrite(M1_PWM, M1);
    }
    else if (M1 < -20){
      digitalWrite(M1_EN, LOW);
      analogWrite(M1_PWM, abs(M1));
    }
    else {
      analogWrite(M1_PWM, 0);
    }

    if(M2 > 20){
      digitalWrite(M2_EN, HIGH);
      analogWrite(M2_PWM, M2);
    }
    else if (M2 < -20){
      digitalWrite(M2_EN, LOW);
      analogWrite(M2_PWM, abs(M2));
    }
    else{
      analogWrite(M2_PWM, 0);
    }
  }

  //////PINCEE (UP/DOWN)
  if(true){
    if(PS3.getButtonClick(TRIANGLE)){
      monterPince();
    }

    if(PS3.getButtonClick(CIRCLE)){
      descendrePince();
    }
  }

  ////// PINCE BOUT
  if(true){
    int pince = map(PS3.getAnalogButton(R2), 0, 255, 75, 115);
    PINCE_BOUT.write(pince);
  }

  ////// PONT
  if(false){
    int pont = map(PS3.getAnalogButton(L2), 0, 255, 30, 60);
    
    PONT_1.write(pont);
    PONT_2.write(pont);

    Serial.print(F("\r\nPont: "));
    Serial.print(pont);
    
  }

  ////// PINCE (ACTUATEUR LINERAIRE)
  if(true){

    if (PS3.getButtonPress(DOWN)) {
      digitalWrite(ML_EN, HIGH);
      digitalWrite(ML_PWM, HIGH);
    }
    
    else if (PS3.getButtonPress(UP)) {
      digitalWrite(ML_EN, LOW);
      digitalWrite(ML_PWM, HIGH);
    }
    
    else {
      digitalWrite(ML_PWM, LOW);
    }
  }

  ////// DRUM
  if(true){
    int pulse_delay = 800;
    if(PS3.getButtonPress(CROSS)){
      digitalWrite(D_EN, HIGH);
      digitalWrite(D_DIR, LOW);
      
      digitalWrite(D_PULSE, HIGH);
      delayMicroseconds(pulse_delay);
      digitalWrite(D_PULSE, LOW);
      delayMicroseconds(pulse_delay);
    }
    else if(PS3.getButtonPress(SQUARE)){
      digitalWrite(D_EN, HIGH);
      digitalWrite(D_DIR, HIGH);
      
      digitalWrite(D_PULSE, HIGH);
      delayMicroseconds(pulse_delay);
      digitalWrite(D_PULSE, LOW);
      delayMicroseconds(pulse_delay);
    }
    else{
      digitalWrite(D_EN, LOW);
    }
    
  }
}
