//Robo Sumo 2017.2
//Fabricio Silva

//Libraries
#include <Servo.h>

//Defines
#define SERVO 11
#define buttonStart 12
#define frontStrip 0
#define rearStrip 1
#define IN1 4
#define IN2 6
#define velocidadeA 5
#define IN3 7
#define IN4 8
#define velocidadeB 9

//Variables
Servo servoControl;
int position;
int buttonPressed = 0;
int buttonStartFlag = 0;
int frontStripFlag = 0;
int rearStripFlag = 0;

//Functions
void frontDetected(){
	frontStripFlag = 1;
}

void rearDetected(){
	rearStripFlag = 1;
}

void parar(){
	analogWrite(velocidadeA,0);
	analogWrite(velocidadeB,0);
	digitalWrite(IN1,LOW);
	digitalWrite(IN2,LOW);
	digitalWrite(IN3,LOW);
	digitalWrite(IN4,LOW);
}

void moveFrenteFaixa()
{
	analogWrite(velocidadeA,100);
	digitalWrite(IN1,LOW);
	digitalWrite(IN2,HIGH);
	analogWrite(velocidadeB,100);
	digitalWrite(IN3,LOW);
	digitalWrite(IN4,HIGH);
}

void moveTrasFaixa()
{
	analogWrite(velocidadeA,100);
	digitalWrite(IN1,HIGH);
	digitalWrite(IN2,LOW);
	analogWrite(velocidadeB,100);
	digitalWrite(IN3,HIGH);
	digitalWrite(IN4,LOW);
}

//Common arduino functions
void setup() {
	Serial.begin(9600);

	//Setup servo
	servoControl.attach(SERVO);
	servoControl.write(100);

	//Setup start button
	pinMode(buttonStart, INPUT);

	//Setup engine
	pinMode(IN1,OUTPUT);
	pinMode(IN2,OUTPUT);
	pinMode(IN3,OUTPUT);
	pinMode(IN4,OUTPUT);
	pinMode(velocidadeA,OUTPUT);
	pinMode(velocidadeB,OUTPUT);

	//Setup interrupt Strip sensors
	attachInterrupt(frontStrip,frontDetected,FALLING);
	attachInterrupt(rearStrip,rearDetected,FALLING);
}

void loop() {
	//Turn on
	buttonPressed = digitalRead(buttonStart);
	if(buttonStartFlag == 0 && buttonPressed == 1){
		Serial.println("Button pressed");
		buttonStartFlag = 1;
		Serial.println("5 seconds delay");
		delay(5000);
		Serial.println("Servo down");
		servoControl.write(25);
	}
	
	if(frontStripFlag == 1){
		Serial.println("Back");
		moveTrasFaixa();
		delay(3000);
		frontStripFlag = 0;
		parar();
	}

	if(rearStripFlag == 1){
		Serial.println("Front");
		moveFrenteFaixa();
		delay(3000);
		rearStripFlag = 0;
		parar();
	}

	
}

