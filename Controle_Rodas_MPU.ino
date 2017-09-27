#include <Wire.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define MPU_ADDR 0x68
#define MEM_START_ADDR 0x6E
#define MEM_R_W 0x6F

long lastRead = 0;
byte processed_packet[8];
byte readd_packet[50];
byte temp = 0;
byte fifoCountL = 0;
byte fifoCountL2 = 0;
byte packetCount = 0x00;
boolean longPacket = false;
boolean firstPacket = true;
float q[4];
float Euler[3]; // psi, theta, phi
float hq[4];
float a[4];
float b[4];
int first = 0;

//A partir daqui são variaveis de controle do carrinho
int ENA = 3; //PWM
int IN1 = 5;
int IN2 = 6;
int IN3 = 9;
int IN4 = 10;
int ENB = 11; //PWM
float set_point = 0;
float angulo_atual = 0;
float angulo_anterior = 0;
int erro = 0;
double kp = 2.3;
double ki = 0.00005;
double kd = 10;
int cons = 120;
int x = 0;
int y = 0;
boolean reset_coord = false;
boolean ok_coord = false;
float temp_anterior = 0;
float temp_atual = 0;
float dt;
float p = 0;
float i = 0;
float d = 0;
float pid = 0;

void setup(){
  
  Serial.begin(9600);
  //Serial.begin(115200);
  sensor_setup();
  for (int i = 3; i <= 11; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop(){

  while(first <= 400){
    sensor_loop();
  }
  sensor_loop();
  angulo_anterior = angulo_atual;
  angulo_atual = Euler[0];
  erro = set_point - angulo_atual;
  if(erro == 0 && ok_coord){
    set_point = 0;
    reset_coord = true;
    ok_coord = false;
  }
  //controlador_loop();
  temp_anterior = temp_atual;
  temp_atual = millis();
  dt = temp_atual - temp_anterior;
  p = kp*erro;
  i += ki*erro*dt;
  if(i > 50 || i < -50)
    if (i>50) i = 50;
    if (i<50) i = -50;
  d = (kd*(angulo_atual - angulo_anterior))/dt;
  pid = p+i+d;
  Serial.println(pid);
  x = cons + pid;
  y = (cons-35) - pid;
  if( x < 0 || y < 0){
    if(x < 0) x=0;
    if(y < 0) y=0;
  }
  if (x > 255 || y > 255){
    if(x > 255) x=255 ;
    if(y > 255) y=255;
  }
  frente(y,x);
  //Serial.print(y);Serial.print(" , "); Serial.println(x);
  if (Serial.available() > 0) {
    char letra = Serial.read();
    switch (letra){
      case 'e':
        set_point += 30;
        ok_coord = true;
      break;
      case 'w':
        set_point += 60;
        ok_coord = true;
      break;
      case 'q':
        set_point += 90;
        ok_coord = true;
      break;
      case 'r':
        set_point -= 30;
        ok_coord = true;
      break;
      case 't':
        set_point -= 60;
        ok_coord = true;
      break;
      case 'y':
        set_point -= 90;
        ok_coord = true;
      break;
      default:
      break;
    }
  }
}//Fim do loop
  
void writeQuat(){
  q[0] = (long) ((((unsigned long) processed_packet[0]) << 8) + ((unsigned long) processed_packet[1]));
  q[1] = (long) ((((unsigned long) processed_packet[2]) << 8) + ((unsigned long) processed_packet[3]));
  q[2] = (long) ((((unsigned long) processed_packet[4]) << 8) + ((unsigned long) processed_packet[5]));
  q[3] = (long) ((((unsigned long) processed_packet[6]) << 8)  + ((unsigned long) processed_packet[7]));
  for(int i = 0; i < 4; i++ ) {
    if( q[i] > 32767 ) {
      q[i] -= 65536;
    }
    q[i] = ((float) q[i]) / 16384.0f;
  }

  if (first >= 400 ){
    if(first == 400 || reset_coord == true){
      a[0] = q[0];
      a[1] = -q[1];
      a[2] = -q[2];
      a[3] = -q[3];
      reset_coord = false;
    }
    b[0] = q[0];
    b[1] = q[1];
    b[2] = q[2];
    b[3] = q[3];
    q[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
    q[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
    q[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
    q[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
    }
    if(first <= 400){
      first++;
    }
  
  Euler[0] = (atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1))*180/3.1415926; // psi
  //Euler[1] = (-asin(2 * q[1] * q[3] + 2 * q[0] * q[2]))*180/3.1415926; // theta
  //Euler[2] = (atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1))*180/3.1415926; // phi
/*
  Serial.print("Angulo Atual: ");Serial.print(Euler[0]); Serial.print(" , ");
  Serial.print("Erro: ");Serial.print(erro);Serial.print(" , ");
  Serial.print("Set Point: ");Serial.println(set_point);
*/
}

/*void controlador_loop(){
  temp_anterior = temp_atual;
  temp_atual = millis();
  dt = temp_atual - temp_anterior;
  p = kp*erro;
  i += ki*erro*dt;
  d = (kd*(angulo_atual - angulo_anterior))/dt;
  pid = p+i+d;
  Serial.println(pid);
  delay(50);
  x = cons + pid;
  y = cons - pid;
  if( x < 0 || y < 0){
    if(x < 0) x=0 ;
    if(y < 0) y=0;
  }
  if (x > 100 || y > 100){
    if(x > 100) x=100 ;
    if(y > 100) y=100;
  }
}*/

void frente(int a, int b){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, a);
  analogWrite(ENB, b);
}
