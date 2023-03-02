#define enA 10//Enable1 L298 Pin enA 
#define in1 9 //Motor1  L298 Pin in1 
#define in2 8 //Motor1  L298 Pin in1 
#define in3 7 //Motor2  L298 Pin in1 
#define in4 6 //Motor2  L298 Pin in1 
#define enB 5 //Enable2 L298 Pin enB 

#define servo A4

#define R_S A0 //ir sensor Right
#define L_S A1 //ir sensor Left

#define echo A2    //Echo pin
#define trigger A3 //Trigger pin

int distance_L, distance_F = 30, distance_R;
long distance;
int set = 20;

int bt_data; // variable to receive data from the serial port
int Speed = 200;  
int mode=0;
char value;
void setup(){ // put your setup code here, to run once

pinMode(R_S, INPUT); // declare if sensor as input  
pinMode(L_S, INPUT); // declare ir sensor as input

pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  

pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

Serial.begin(9600); // start serial communication at 9600bps

pinMode(servo, OUTPUT);

 for (int angle = 89; angle <= 160; angle += 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 160; angle >= 15; angle -= 5)  {
   servoPulse(servo, angle);  }

 for (int angle = 15; angle <= 89; angle += 5)  {
   servoPulse(servo, angle);  }
delay(500);
}


void loop(){  

if(Serial.available() > 0){  //if some date is sent, reads it and saves in value  
value = Serial.read(); 
Serial.println(value);     
if(bt_data > 20)
{
  Speed = bt_data;
  }      
}

else if(value == 8){mode=0; Stop();}    //Manual Android Application and IR Remote Control Command   
else if(value == 9){mode=1; Speed=130;} //Auto Line Follower Command
else if(value ==10){mode=2; Speed=255;} //Auto Obstacle Avoiding Command

analogWrite(enA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 

if(mode==0){     
//===============================================================================
//                          Key Control Command
//=============================================================================== 
     if(value == 1){forword(); }  // if the bt_data is '1' the DC motor will go forward
else if(value == 2){backword();}  // if the bt_data is '2' the motor will Reverse
else if(value == 3){turnLeft();}  // if the bt_data is '3' the motor will turn left
else if(value == 4){turnRight();} // if the bt_data is '4' the motor will turn right
else if(value == 5){Stop(); }     // if the bt_data '5' the motor will Stop

//===============================================================================
//                          Voice Control Command
//===============================================================================    
if(value == 6){turnLeft();  delay(400);  value = 5;}
else if(value == 7){turnRight(); delay(400);  value = 5;}
}

if(mode==1){    
//===============================================================================
//                          Line Follower Control
//===============================================================================     
if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){forword();}  //if Right Sensor and Left Sensor are at White color then it will call forword function
if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){turnRight();}//if Right Sensor is Black and Left Sensor is White then it will call turn Right function  
if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){turnLeft();} //if Right Sensor is White and Left Sensor is Black then it will call turn Left function
if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){Stop();}     //if Right Sensor and Left Sensor are at Black color then it will call Stop function
} 

if(mode==2){    
//===============================================================================
//                          Obstacle Avoiding Control
//===============================================================================     
 distance_F = Ultrasonic_read();
 Serial.print("S=");
 Serial.println(distance_F);
  if (distance_F > set){
    forword();
    }
    else{
      Check_side();
      }
}

delay(10);
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50);                   // Refresh cycle of servo
}


//**********************Ultrasonic_read****************************
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echo, HIGH);
  return distance / 29 / 2;
}

void compareDistance(){
       if (distance_L > distance_R){
  turnLeft();
  delay(350);
  }
  else if (distance_R > distance_L){
  turnRight();
  delay(350);
  }
  else{
  backword();
  delay(300);
  turnRight();
  delay(600);
  }
}

void Check_side(){
    Stop();
    delay(100);
 for (int angle = 89; angle <= 160; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    delay(100);
  for (int angle = 160; angle >= 15; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = Ultrasonic_read();
    delay(100);
 for (int angle = 15; angle <= 89; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();
}

void forword(){  //forword
digitalWrite(in1, LOW); //Right Motor backward Pin 
digitalWrite(in2, HIGH);  //Right Motor forward Pin 
digitalWrite(in3, HIGH);  //Left Motor forward Pin 
digitalWrite(in4, LOW); //Left Motor backward Pin 
}

void backword(){ //backword
digitalWrite(in1, HIGH);  //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, HIGH);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, HIGH);  //Right Motor backward Pin 
digitalWrite(in2, LOW); //Right Motor forward Pin  
digitalWrite(in3, HIGH);  //Left Motor forward Pin 
digitalWrite(in4, LOW); //Left Motor backward Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, LOW); //Right Motor backward Pin 
digitalWrite(in2, HIGH);  //Right Motor forward Pin 
digitalWrite(in3, LOW); //Left Motor forward Pin 
digitalWrite(in4, HIGH);  //Left Motor backward Pin n 
}

void Stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}
