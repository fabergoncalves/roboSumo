//Robo Sumo 2017.2
//Fabricio Silva

//Libraries
#include <Servo.h>

//Defines
#define SERVO 11
#define buttonStart 12
#define frontStrip 0
#define rearStrip 1


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

void setup() {
	Serial.begin(9600);

	//Setup servo
	servoControl.attach(SERVO);
	servoControl.write(100);

	//Setup start button
	pinMode(buttonStart, INPUT);

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
		delay(3000);
		frontStripFlag = 0;
	}

	if(rearStripFlag == 1){
		Serial.println("Front");
		delay(3000);
		rearStripFlag = 0;	
	}

	
}

