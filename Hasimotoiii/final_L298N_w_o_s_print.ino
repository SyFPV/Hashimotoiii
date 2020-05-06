const int ch1 = 54; // left stick vertical channel       ←アナログ
const int ch2 = 55; // left stick horizontal channel
const int ch4 = 56; // right stick horizontal channel
const int aux1 = 57; 
const int aux2 = 58;
const int aux3 = 59;

const int outPin1_1 = 62; // IN4 motor1 3HIGH 4LOWで正転　3LOW 4LOWでストップ
const int outPin1_2 = 63; // IN3　

const int outPin1_3 = 64; // IN2 motor2 1HIGH 2LOWで正転　1LOW 2LOWでストップ
const int outPin1_4 = 65; // IN1

const int outPin2_1 = 66; // IN1 motor3 1HIGH 2LOWで正転　1LOW 2LOWでストップ
const int outPin2_2 = 67; // IN2

const int outPin2_3 = 68; // IN3 motor4 3HIGH 4LOWで正転　3LOW 4LOWでストップ
const int outPin2_4 = 69; // IN4

const int PWM1 = 8; // motor1 PWM
const int PWM2 = 9; // motor2 PWM
const int PWM3 = 10; // motor3 PWM
const int PWM4 = 11; // motor4 PWM

const int ledR = 5; //RBG LEDのRピン RED GREEN BLUEです。PWM 255がMin0がMax出力
const int ledG = 6; //RBG LEDのGピン
const int ledB = 7; //RBG LEDのBピン

int pulseNum1 = 0;
int pulseNum2 = 0;
int pulseNum4 = 0;
int pulseNum5 = 0;
int pulseNum6 = 0;
int pulseNum7 = 0;

int speedVal1 = 0;
int speedVal2 = 0;
int speedVal4 = 0;

int PM1 = 0;
int PM2 = 0;
int PM3 = 0;
int PM4 = 0;

int maxPM = 255;
int minPM = -255;

int H, C1, C2, C3;
int R, G, B;

int val = 0;
int old_val = 0;
int state = 0;

void setup() { //setup pin mode
  pinMode(ch1, INPUT); 
  pinMode(ch2, INPUT); 
  pinMode(ch4, INPUT); 
  pinMode(aux1, INPUT);
  pinMode(aux2, INPUT);
  pinMode(aux3, INPUT);

  pinMode(outPin1_1, OUTPUT); 
  pinMode(outPin1_2, OUTPUT); 
  pinMode(outPin1_3, OUTPUT); 
  pinMode(outPin1_4, OUTPUT);
  pinMode(outPin2_1, OUTPUT); 
  pinMode(outPin2_2, OUTPUT); 
  pinMode(outPin2_3, OUTPUT); 
  pinMode(outPin2_4, OUTPUT); 

  Serial.begin(9600); 
}

void rx() {//受信関数
  pulseNum1 = pulseIn(ch1, HIGH); 
  pulseNum2 = pulseIn(ch2, HIGH); 
  pulseNum4 = pulseIn(ch4, HIGH); 
  pulseNum5 = pulseIn(aux1, HIGH);
  pulseNum6 = pulseIn(aux2, HIGH);
  pulseNum7 = pulseIn(aux3, HIGH);
}

void loop() {
  rx();
  if (pulseNum1 != 0) {
    pulseNum1 = constrain(pulseNum1, 1000, 2000);
    pulseNum2 = constrain(pulseNum2, 1000, 2000);
    pulseNum4 = constrain(pulseNum4, 1000, 2000);

    int speedVal1 = map(pulseNum1, 2000, 1000, -255, 255);
    int speedVal2 = map(pulseNum2, 2000, 1000, -255, 255);
    int speedVal4 = map(pulseNum4, 2000, 1000, -255, 255);

    speedVal1 = constrain(speedVal1, -255, 255);
    speedVal2 = constrain(speedVal2, -255, 255);
    speedVal4 = constrain(speedVal4, -255, 255);

    PM1 = speedVal1 - speedVal2 + speedVal4;
    PM2 = speedVal1 + speedVal2 - speedVal4;
    PM3 = speedVal1 - speedVal2 - speedVal4;
    PM4 = speedVal1 + speedVal2 + speedVal4;

    float max = PM1;
    if (max < PM2) max = PM2;
    if (max < PM3) max = PM3;
    if (max < PM4) max = PM4;

    if (max > maxPM) {
      PM1 = PM1 / max * maxPM;
      PM2 = PM2 / max * maxPM;
      PM3 = PM3 / max * maxPM;
      PM4 = PM4 / max * maxPM;
      }

    float min = PM1;
    if (min > PM2) min = PM2;
    if (min > PM3) min = PM3;
    if (min > PM4) min = PM4;
    if (min < minPM) {
      PM1 = PM1 / min * minPM;
      PM2 = PM2 / min * minPM;
      PM3 = PM3 / min * minPM;
      PM4 = PM4 / min * minPM;
      }

    pulseNum5 = constrain(pulseNum5, 1000, 2000);
    pulseNum6 = constrain(pulseNum6, 1000, 2000);
    pulseNum7 = constrain(pulseNum7, 1000, 2000);

    H = map(pulseNum5, 2000, 1000, 0, 360) ; //HSVのH値(0-360)にスケール変換を行う

    if (H <= 120) { //H値(0-120) 赤-黄-緑 
      R = map(H, 0, 120, 0, 255); //赤LED R←→G
      G = map(H, 0, 120, 255, 0); //緑LED G←→R
      B = 255;
      }

    else if (H <= 240) { //H値(120-240) 緑-水色-青
      R = 255;
      G = map(H, 120, 240, 0, 255); //緑LED G←→B
      B = map(H, 120, 240, 255, 0); //青LED B←→G
      }

    else { //H値(240-360) 青-紫-赤 
      R = map(H, 240, 360, 255, 0); // 青LED R←→B
      G = 255;
      B = map(H, 240, 360, 0, 255); // 青LED B←→R
      }
      
    val = pulseNum6;
    if ((val >= 1200) && (old_val < 1200)) {
      state = 1 - state;
    }
    
    old_val = val;
    if (state == 1) {
      if (pulseNum7 <= 1400) {
        analogWrite(ledR, R);            
        analogWrite(ledG, G);           
        analogWrite(ledB, B);         
      }
 
      else {
        C1 = random(0, 255); // 0から255の乱数を生成
        C2 = random(0, 255); // 0から255の乱数を生成
        C3 = random(0, 255); // 0から255の乱数を生成

        analogWrite(ledR, C1);
        analogWrite(ledG, C2); 
        analogWrite(ledB, C3);
      }
    }

    else {
      analogWrite(ledR, 255); //OFF
      analogWrite(ledG, 255);
      analogWrite(ledB, 255);
    }

    if (PM1 >= 35) {
      digitalWrite(outPin1_1, LOW);
      digitalWrite(outPin1_2, HIGH);
      analogWrite(PWM1, abs(PM1));
    }

    else if (PM1 <= -35) {
      digitalWrite(outPin1_1, HIGH);
      digitalWrite(outPin1_2, LOW);
      analogWrite(PWM1, abs(PM1));
    }

    else {
      digitalWrite(outPin1_1, LOW);
      digitalWrite(outPin1_2, LOW);
      analogWrite(PWM1, 0);
    }

    if (PM2 >= 35) {
      digitalWrite(outPin1_3, LOW);
      digitalWrite(outPin1_4, HIGH);
      analogWrite(PWM2, abs(PM2));
    }

    else if (PM2 <= -35) {
      digitalWrite(outPin1_3, HIGH);
      digitalWrite(outPin1_4, LOW);
      analogWrite(PWM2, abs(PM2));
    }

    else {
      digitalWrite(outPin1_3, LOW);
      digitalWrite(outPin1_4, LOW);
      analogWrite(PWM2, 0);
    }

    if (PM3 >= 35) {
      digitalWrite(outPin2_1, LOW);
      digitalWrite(outPin2_2, HIGH);
      analogWrite(PWM3, abs(PM3));
    }

    else if (PM3 <= -35) {
      digitalWrite(outPin2_1, HIGH);
      digitalWrite(outPin2_2, LOW);
      analogWrite(PWM3, abs(PM3));
    }

    else {
      digitalWrite(outPin2_1, LOW);
      digitalWrite(outPin2_2, LOW);
      analogWrite(PWM3, 0);
    }

    if (PM4 >= 35) {
      digitalWrite(outPin2_3, LOW);
      digitalWrite(outPin2_4, HIGH);
      analogWrite(PWM4, abs(PM4));
    }

    else if (PM4 <= -35) {
      digitalWrite(outPin2_3, HIGH);
      digitalWrite(outPin2_4, LOW);
      analogWrite(PWM4, abs(PM4));
    }

    else {
      digitalWrite(outPin2_3, LOW);
      digitalWrite(outPin2_4, LOW);
      analogWrite(PWM4, 0);
    }
  }
}
