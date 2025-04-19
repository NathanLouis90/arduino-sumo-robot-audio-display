#include <SPI.h> 
#include <SdFat.h> 
#include <UTFT.h> 
#include <UTFT_SdRaw.h> 
#include <URTouch.h> 
#include <Servo.h> 
#include "DFRobotDFPlayerMini.h" 

#include "SoftwareSerial.h" 
#define SD_CHIP_SELECT 53 
/*Servo Left_Servo; 
 Servo Right_Servo;*/
SoftwareSerial motorserial(9,8);
SdFat sd;
UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
UTFT_SdRaw myFiles(&myGLCD);
URTouch myTouch( 6, 5, 4, 3, 2);

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern unsigned short ROBODEMO[];
extern unsigned short SUMOFIGHT[];
extern unsigned short INFRA[];
static const uint8_t PIN_MP3_TX = 10;
static const uint8_t PIN_MP3_RX = 11;

SoftwareSerial softwareSerial( PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

int x, y;
char currentPage, secondcurrentPage;

#define IRPin_1 A0 // Set A0 as input to be used for Analog Sensor 1 Frontal Left Sensor
#define IRPin_2 A1 // Set A1 as input to be used for Analog Sensor 2 Frontal Center Sensor
#define IRPin_3 A2 // Set A2 as input to be used for Analog Sensor 3 Frontal Right Sensor
#define IRPin_4 A3 // Set A3 as input to be used for Analog Sensor 4 Middle Left Sensor
#define IRPin_5 A4 // Set A4 as input to be used for Analog Sensor 5 Middle Right Sensor
#define IRPin_6 A5 // Set A5 as input to be used for Analog Sensor 6 Back Left Sensor 
#define IRPin_7 A6 // Set A6 as input to be used for Analog Sensor 7 Back Right Sensor
#define IRPin_8 A7 // Set A7 as input to be used for Analog Sensor 8 Back Sensor 
int Distance_FL, Distance_FC, Distance_FR, Distance_ML, Distance_MR, Distance_BL, Distance_BR, Distance_BK;
#define HOA_LD1 11 
#define HOA_LD2 12 
void setup() {
	Serial.begin(9600);
	Serial3.begin(9600);
	softwareSerial.begin(9600);
	motorserial.begin(38400);
	/*Left_Servo.attach(9); 
	 Right_Servo.attach(8);*/
	pinMode(HOA_LD1, INPUT);
	pinMode(HOA_LD2, INPUT);
	player.begin(softwareSerial);

	//player.volume(0); 

	myGLCD.InitLCD();
	myGLCD.clrScr();
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED);
	delay(1000);
	myFiles.load(0, 0, 320, 240, "NYPLG.RAW", 1, 0); // Display NYP Logo 
	delay(2000);
	myGLCD.clrScr();
	BootUp_Screen();
	myGLCD.clrScr();
	Home_Menu();
	currentPage = '0';
}

void loop() {
	if (currentPage == '0') // Home Menu 
			{
		if (myTouch.dataAvailable()) {
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();

			if ((x >= 35) && (x <= 285) && (y >= 50) && (y <= 90)) {
				drawFrame(35, 50, 285, 90);
				myGLCD.clrScr();

				currentPage == '1';
				Intro_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 100) && (y <= 140)) {
				drawFrame(35, 100, 285, 140);
				myGLCD.clrScr();
				currentPage == '2';
				SelectMusic_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 150) && (y <= 190)) {
				drawFrame(35, 150, 285, 190);
				myGLCD.clrScr();
				currentPage == '4';
				SelectGame_Menu();
			}
			if ((x >= 190) && (x <= 319) && (y >= 220) && (y <= 239)) {
				drawBackFrame(190, 220, 319, 239);
				myGLCD.clrScr();
				currentPage == '5';
				Settings_Menu();
			}
		}
	}
	if (currentPage == '1') { // Introductory Menu 

		if (myTouch.dataAvailable()) {
			myTouch.read();

			x = myTouch.getX();
			y = myTouch.getY();
			if ((x >= 0) && (x <= 34) && (y >= 0) && (y <= 20)) {
				drawBackFrame(0, 0, 34, 20);
				myGLCD.clrScr();
				currentPage == '0';
				Home_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 50) && (y <= 90)) // Goes to Robot Intro Menu 
					{
				drawFrame(35, 50, 285, 90);
				myGLCD.clrScr();
				currentPage == '8';
				myGLCD.clrScr();
				RobotIntro_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 100) && (y <= 140)) // Goes to Infrared Demo Menu 
					{
				drawFrame(35, 100, 285, 140);
				myGLCD.clrScr();
				currentPage == '9';
				secondcurrentPage == '2';
				InfraDemo_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 150) && (y <= 190)) // Goes to Motion Demo Menu 
					{
				drawFrame(35, 150, 285, 190);
				myGLCD.clrScr();

				drawFrame(35, 100, 285, 140);
				myGLCD.clrScr();
				player.volume(20);
				player.play(7); // Plays the intro for the Infrared Demo 
				myGLCD.fillScr(255, 255, 255);
				myGLCD.drawBitmap(110, 60, 100, 100, ROBODEMO);
				delay(14500);
				myGLCD.clrScr();
				secondcurrentPage = '0';
				currentPage = '9';
				MotionDemo_Menu();
			}
		}
	}
	if (currentPage == '2') { // Select Music Menu      
		if (myTouch.dataAvailable()) {
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();
			if ((x >= 0) && (x <= 34) && (y >= 0) && (y <= 20)) {
				drawBackFrame(0, 0, 34, 20);
				currentPage = '0';
				myGLCD.clrScr();
				Home_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 50) && (y <= 90)) {
				drawFrame(35, 50, 285, 90);
				myGLCD.clrScr();
				currentPage == '3';

				myGLCD.clrScr();
				PlayMusic1_Menu();
			}
		}
	}
	if (currentPage == '3') { // Play Music Menu 
	}

	if (currentPage == '4') { // Select Game Menu 
		if (myTouch.dataAvailable()) {
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();
			if ((x >= 0) && (x <= 34) && (y >= 0) && (y <= 20)) {
				drawBackFrame(0, 0, 34, 20);
				myGLCD.clrScr();
				currentPage == '0';
				Home_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 50) && (y <= 90)) // Goes to Bluetooth Control Menu 
					{
				drawFrame(35, 50, 285, 90);
				myGLCD.clrScr();
				currentPage == '6';
				BLEControl_Menu();
			}
		}
	}
	if (currentPage == '5') { // Settings Menu 

		/*Left_Servo.writeMicroseconds(1500); 
		 Right_Servo.writeMicroseconds(1500);*/
		motorserial.write(64);
		motorserial.write(192);
		if (myTouch.dataAvailable()) {
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();
			if ((x >= 0) && (x <= 34) && (y >= 0) && (y <= 20)) { // Goes to Home Menu 
				drawBackFrame(0, 0, 34, 20);
				myGLCD.clrScr();
				currentPage == '0';
				Home_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 50) && (y <= 90)) // Goes to Sensor Check Menu 
					{
				drawFrame(35, 50, 285, 90);
				myGLCD.clrScr();
				currentPage == '7';
				SensorCheck_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 100) && (y <= 140)) // Goes to Tire Cleaning Menu 
					{
				drawFrame(35, 100, 285, 140);
				myGLCD.clrScr();
				currentPage == '9';
				secondcurrentPage == '1';
				TireCleaning_Menu();
			}
		}

	}

	if (currentPage == '6' && Serial3.available() > 0) { // Bluetooth Control Menu 
		char data = Serial3.read();
		if (data == '1') { // Forward Direction 
			Serial3.println("Moving forward");
			Forward_BLE();
		} else if (data == '2') { // Anticlockwise Direction 
			Serial3.println("Moving Anticlockwise");
			Anticlockwise_BLE();
		} else if (data == '3') { // Clockwise Direction 
			Serial3.println("Moving Clockwise");
			Clockwise_BLE();
		} else if (data == '4') { // Backward Direction 
			Backward_BLE();
			Serial3.println("Moving Backward");
		} else if (data == '5') { // Stop Direction 
			Stop_BLE();
			Serial3.println("Halting Robot");
		}
	}

	if (currentPage == '7') { // Sensor Check Menu 
		float Volts_FL = analogRead(IRPin_1) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_FL = 13 * pow(Volts_FL, -1); // worked out from datasheet graph 

		if (Distance_FL <= 30) {
			myGLCD.printNumI(Distance_FL, RIGHT, 50);
			delay(1200);
		} else if (Distance_FL >= 30) {
			myGLCD.print(">30", RIGHT, 50);
		}
		float Volts_FC = analogRead(IRPin_2) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_FC = 13 * pow(Volts_FC, -1); // worked out from datasheet graph 
		if (Distance_FC <= 30) {
			myGLCD.printNumI(Distance_FC, RIGHT, 70);
			delay(1200);
		} else if (Distance_FC >= 30) {
			myGLCD.print(">30", RIGHT, 70);
		}
		float Volts_FR = analogRead(IRPin_3) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_FR = 13 * pow(Volts_FR, -1); // worked out from datasheet graph 
		if (Distance_FR <= 30) {
			myGLCD.printNumI(Distance_FR, RIGHT, 90);
			delay(1200);
		} else if (Distance_FR >= 30) {
			myGLCD.print(">30", RIGHT, 90);
		}
		float Volts_ML = analogRead(IRPin_4) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_ML = 13 * pow(Volts_ML, -1); // worked out from datasheet graph 
		if (Distance_ML <= 30) {

			myGLCD.printNumI(Distance_ML, RIGHT, 110);
			delay(1200);
		} else if (Distance_ML >= 30) {
			myGLCD.print(">30", RIGHT, 110);
		}
		float Volts_MR = analogRead(IRPin_5) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_MR = 13 * pow(Volts_MR, -1); // worked out from datasheet graph 
		if (Distance_MR <= 30) {
			myGLCD.printNumI(Distance_MR, RIGHT, 130);
			delay(1200);
		} else if (Distance_MR >= 30) {
			myGLCD.print(">30", RIGHT, 130);
		}
		float Volts_BL = analogRead(IRPin_6) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_BL = 13 * pow(Volts_BL, -1); // worked out from datasheet graph 
		if (Distance_BL <= 30) {
			myGLCD.printNumI(Distance_BL, RIGHT, 150);
			delay(1200);
		} else if (Distance_BL >= 30) {
			myGLCD.print(">30", RIGHT, 150);
		}
		float Volts_BR = analogRead(IRPin_7) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_BR = 13 * pow(Volts_BR, -1); // worked out from datasheet graph 
		if (Distance_BR <= 30) {
			myGLCD.printNumI(Distance_BR, RIGHT, 170);

			delay(1200);
		} else if (Distance_BR >= 30) {
			myGLCD.print(">30", RIGHT, 170);
		}
		float Volts_BK = analogRead(IRPin_8) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_BK = 13 * pow(Volts_BK, -1); // worked out from datasheet graph 
		if (Distance_BK <= 30) {
			myGLCD.printNumI(Distance_BK, RIGHT, 190);
			delay(1200);
		} else if (Distance_BK >= 30) {
			myGLCD.print(">30", RIGHT, 190);
		}
		delay(2500);
		myGLCD.clrScr();
		currentPage == '5';
		Settings_Menu();
	}

	if (currentPage == '8') { // Robot Intro Menu 
	}

	if (secondcurrentPage == '0') { // Motion Demo Menu [CurrentPage 9 is occupied by a blankpage to avoid unnecessary confusion] 
		if (myTouch.dataAvailable()) {
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();

			if ((x >= 35) && (x <= 285) && (y >= 100) && (y <= 140)) // Goes to Infrared Demo Menu 
					{
				drawFrame(35, 100, 285, 140);
				myGLCD.clrScr();

				myGLCD.fillScr(255, 255, 255);
				myGLCD.drawBitmap(110, 60, 100, 100, INFRA);

				player.volume(20);
				player.play(8); // Plays "I can move forward" 
				motorserial.write(74); // Left_Servo.writeMicroseconds(1600); 
				motorserial.write(202); // Right_Servo.writeMicroseconds(1600); 
				delay(3000);
				motorserial.write(64); // Left_Servo.writeMicroseconds(1500); 
				motorserial.write(192); // Right_Servo.writeMicroseconds(1500); 

				player.volume(20);
				player.play(9); // Plays "Backward" 
				motorserial.write(54); // Left_Servo.writeMicroseconds(1400); 
				motorserial.write(182); // Right_Servo.writeMicroseconds(1400); 
				delay(3000);
				motorserial.write(64); // Left_Servo.writeMicroseconds(1500); 
				motorserial.write(192); // Right_Servo.writeMicroseconds(1500); 

				player.volume(20);
				player.play(10); // Plays "Clockwise" 
				motorserial.write(74); // Left_Servo.writeMicroseconds(1600); 
				motorserial.write(182); // Right_Servo.writeMicroseconds(1400); 
				delay(3000);

				motorserial.write(64); // Left_Servo.writeMicroseconds(1500); 
				motorserial.write(192); // Right_Servo.writeMicroseconds(1500); 

				player.volume(20);
				player.play(11); // Plays "AntiClockwise" 
				motorserial.write(54); // Left_Servo.writeMicroseconds(1400); 
				motorserial.write(202); // Right_Servo.writeMicroseconds(1600); 
				delay(3000);

				motorserial.write(64); // Left_Servo.writeMicroseconds(1500); 
				motorserial.write(192); // Right_Servo.writeMicroseconds(1500); 
				player.volume(20);
				player.play(12); // Plays Final Message 
				myGLCD.fillScr(255, 255, 255);
				myGLCD.drawBitmap(110, 60, 100, 100, SUMOFIGHT);
				delay(3500);
				myGLCD.clrScr();
				currentPage = '1';
				secondcurrentPage = '9';
				Intro_Menu();
			}
		}
	}
	if (secondcurrentPage == '1') { // Tire Cleaning Menu 
		if (myTouch.dataAvailable()) {
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();
			if ((x >= 0) && (x <= 34) && (y >= 0) && (y <= 20)) {
				drawBackFrame(0, 0, 34, 20);

				myGLCD.clrScr();
				currentPage == '5';
				Settings_Menu();
			}
			if ((x >= 35) && (x <= 285) && (y >= 50) && (y <= 90)) // Start motor for tire cleaning 
					{
				makeFrame(35, 50, 285, 90);
				resetFrame(35, 100, 285, 140);
				motorserial.write(79); // Left_Servo.writeMicroseconds(1700); 
				motorserial.write(207); // Right_Servo.writeMicroseconds(1700);      
			}
			if ((x >= 35) && (x <= 285) && (y >= 100) && (y <= 140)) // Stop motor 
					{
				makeFrame(35, 100, 285, 140);
				resetFrame(35, 50, 285, 90);
				motorserial.write(64); // Left_Servo.writeMicroseconds(1500); 
				motorserial.write(192); // Right_Servo.writeMicroseconds(1500);        
			}
		}
	}
	if (secondcurrentPage == '2') { // Infrared Demo Menu     
		float Volts_FL = analogRead(IRPin_1) * 0.0048828125; // value from sensor * (5/1024) 
		int Distance_FL = 13 * pow(Volts_FL, -1); // worked out from datasheet graph 
		if (Distance_FL > 10 && Distance_FL < 15) {
			float Volts_FC = analogRead(IRPin_2) * 0.0048828125; // value from sensor * (5/1024) 
			int Distance_FC = 13 * pow(Volts_FC, -1); // worked out from datasheet graph 
			if (Distance_FC > 10 && Distance_FC < 15) {
				float Volts_FR = analogRead(IRPin_3) * 0.0048828125; // value from sensor * (5/1024) 

				int Distance_FR = 13 * pow(Volts_FR, -1); // worked out from datasheet graph 
				if (Distance_FC > 10 && Distance_FC < 15) {
					InfraDemo_Sense();
				}
			}
		}
	}
}

// --- Basic Setup --- // 
void BootUp_Screen() {
	myGLCD.clrScr();
	myGLCD.fillScr(255, 255, 255);
	myGLCD.setColor(0, 0, 0);
	myGLCD.setFont(SmallFont);
	myGLCD.setBackColor(255, 255, 255);
	myGLCD.print("Property of Nanyang Polytechnic", CENTER, 0);
	myGLCD.print("Made by Adiel Firqin", CENTER, 228);
	myGLCD.setColor(200, 0, 0);
	myGLCD.drawLine(0, 227, 319, 227);
	myGLCD.drawLine(0, 14, 319, 14);
	myGLCD.setFont(BigFont);
	myGLCD.setColor(0, 0, 200);
	delay(500);
	myGLCD.print("Entertaining", CENTER, 80);
	delay(500);
	myGLCD.print("Sumo Robot", CENTER, 110);
	delay(500);
	myGLCD.print("for Outreach", CENTER, 140);
	delay(2000);
	myGLCD.clrScr();
}
void Home_Menu() {
	currentPage = '0';
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Home Menu", CENTER, 0);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawLine(0, 20, 319, 20);
	myGLCD.setColor(80, 220, 100);
	myGLCD.fillRoundRect(35, 50, 285, 90);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 50, 285, 90);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(80, 220, 100);
	myGLCD.print("INTRODUCTORY", CENTER, 62);
	myGLCD.setColor(125, 0, 125);
	myGLCD.fillRoundRect(35, 100, 285, 140);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 100, 285, 140);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(125, 0, 125);
	myGLCD.print("MUSIC", CENTER, 112);
	myGLCD.setColor(0, 125, 255);
	myGLCD.fillRoundRect(35, 150, 285, 190);
	75 | P
	a g
	e myGLCD
	.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 150, 285, 190);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 125, 255);
	myGLCD.print("GAMES", CENTER, 162);
	Settings_Button();
	myGLCD.drawLine(0, 220, 319, 220);
}
void Back_Button() {
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRect(0, 0, 34, 20);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("<-", 1, 1);
}
void Next_Button() {
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRect(282, 222, 319, 239);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("->", 283, 223);
}

void Settings_Button() {
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRect(190, 220, 319, 239);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 0);

	myGLCD.print("SETTINGS", 191, 223);
}

void drawFrame(int x1, int y1, int x2, int y2) {
	myGLCD.setColor(255, 0, 0);
	myGLCD.drawRoundRect(x1, y1, x2, y2);
	while (myTouch.dataAvailable())
		myTouch.read();
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(x1, y1, x2, y2);
}

void drawBackFrame(int x1, int y1, int x2, int y2) {
	myGLCD.setColor(255, 0, 0);
	myGLCD.drawRect(x1, y1, x2, y2);
	while (myTouch.dataAvailable())
		myTouch.read();
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRect(x1, y1, x2, y2);
}

void resetFrame(int x1, int y1, int x2, int y2) {
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(x1, y1, x2, y2);
}

void makeFrame(int x1, int y1, int x2, int y2) {
	myGLCD.setColor(255, 0, 0);
	myGLCD.drawRoundRect(x1, y1, x2, y2);
}

// Menu Screens & Miscellaneous 
void Intro_Menu() {
	currentPage = '1';
	Next_Button();
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Introductory", CENTER, 0);
	myGLCD.setColor(80, 220, 100);
	myGLCD.drawLine(0, 20, 319, 20);
	Back_Button();
	myGLCD.setColor(80, 220, 100);
	myGLCD.fillRoundRect(35, 50, 285, 90);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 50, 285, 90);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(80, 220, 100);
	myGLCD.print("ROBOT INTRO", CENTER, 62);
	myGLCD.setColor(80, 220, 100);
	myGLCD.fillRoundRect(35, 100, 285, 140);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 100, 285, 140);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(80, 220, 100);
	myGLCD.print("INFRARED DEMO", CENTER, 112);

	myGLCD.setColor(80, 220, 100);
	myGLCD.fillRoundRect(35, 150, 285, 190);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 150, 285, 190);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(80, 220, 100);
	myGLCD.print("MOTION DEMO ", CENTER, 162);
}

void SelectMusic_Menu() {
	currentPage = '2';
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Select Music", CENTER, 0);
	myGLCD.setColor(125, 0, 125);
	myGLCD.drawLine(0, 20, 319, 20);
	Back_Button();
	myGLCD.setColor(125, 0, 125);
	myGLCD.fillRoundRect(35, 50, 285, 90);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 50, 285, 90);
	myGLCD.setFont(SmallFont);
	myGLCD.setBackColor(125, 0, 125);
	myGLCD.print("Can't Stop This Feeling", CENTER, 62);
}

void PlayMusic1_Menu() {
	currentPage = '3';

	Back_Button();
	myGLCD.fillScr(255, 255, 255);
	player.volume(11);
	player.play(4);
}

void SelectGame_Menu() {
	currentPage = '4';
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Select Game", CENTER, 0);
	myGLCD.setColor(0, 125, 255);
	myGLCD.drawLine(0, 20, 319, 20);
	Back_Button();
	myGLCD.setColor(0, 125, 255);
	myGLCD.fillRoundRect(35, 50, 285, 90);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 50, 285, 90);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 125, 255);
	myGLCD.print("BLE CONTROL", CENTER, 62);
	myGLCD.setColor(0, 125, 255);
	myGLCD.fillRoundRect(35, 100, 285, 140);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 100, 285, 140);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 125, 255);

	myGLCD.print("IR DETECTOR", CENTER, 112);
}

void Settings_Menu() {
	currentPage = '5';
	Back_Button();
	myGLCD.setFont(BigFont);
}

void Backward_BLE() {
	motorserial.write(54); // Left_Servo.writeMicroseconds(1400); 
	motorserial.write(182); // Right_Servo.writeMicroseconds(1400); 
}

void Anticlockwise_BLE() {
	motorserial.write(54); // Left_Servo.writeMicroseconds(1400); 
	motorserial.write(202); // Right_Servo.writeMicroseconds(1600); 
}

void Clockwise_BLE() {
	motorserial.write(74); // Left_Servo.writeMicroseconds(1600); 
	motorserial.write(182); // Right_Servo.writeMicroseconds(1400); 
}

void Stop_BLE() {
	motorserial.write(64); // Left_Servo.writeMicroseconds(1500); 
	motorserial.write(192); // Right_Servo.writeMicroseconds(1500); 
}

void SensorCheck_Menu() {
	currentPage = '7';
	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.print("Reminder:Plug in power to sensors!!!", LEFT, 228);
	myGLCD.setColor(200, 0, 0);
	myGLCD.drawLine(0, 227, 319, 227);
	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.print("IR Distances(in cm)", CENTER, 0);
	myGLCD.setFont(BigFont);
	myGLCD.print("Frontal Left:", LEFT, 50);
	myGLCD.print("Frontal Center:", LEFT, 70);
	myGLCD.print("Frontal Right:", LEFT, 90);
	myGLCD.print("Middle Left:", LEFT, 110);
	myGLCD.print("Middle Right:", LEFT, 130);
	myGLCD.print("Back Left:", LEFT, 150);
	myGLCD.print("Back Right:", LEFT, 170);
	myGLCD.print("Back:", LEFT, 190);
	myGLCD.setColor(255, 0, 0);
	myGLCD.drawLine(0, 20, 319, 20);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setFont(BigFont);
	delay(800);
}

void RobotIntro_Menu() {
	currentPage = '8';
	player.volume(20);
	player.play(1); // Plays the audio for the robot intro 

	myGLCD.fillScr(255, 255, 255);
	myGLCD.drawBitmap(110, 60, 100, 100, ROBODEMO);
	delay(10500);
	myGLCD.clrScr();
	currentPage = '1';
	Intro_Menu();
}

void MotionDemo_Menu() {
	currentPage = '9';
	secondcurrentPage = '0';
	myGLCD.setColor(82, 102, 54);
	myGLCD.fillRoundRect(35, 100, 285, 140);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 100, 285, 140);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(82, 102, 54);
	myGLCD.print("START DEMO", CENTER, 112);
}

void TireCleaning_Menu() {
	currentPage = '9';
	secondcurrentPage = '1';
	Back_Button();
	myGLCD.setFont(SmallFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.print("Reminder:Plug in power to motors!!!", LEFT, 228);
	myGLCD.drawLine(0, 227, 319, 227);

	myGLCD.setFont(BigFont);
	myGLCD.setColor(255, 255, 255);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Tire Cleaning", CENTER, 0);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawLine(0, 20, 319, 20);
	myGLCD.setColor(82, 102, 54);
	myGLCD.fillRoundRect(35, 50, 285, 90);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 50, 285, 90);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(82, 102, 54);
	myGLCD.print("START", CENTER, 62);
	myGLCD.setColor(225, 25, 49);
	myGLCD.fillRoundRect(35, 100, 285, 140);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(35, 100, 285, 140);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(225, 25, 49);
	myGLCD.print("STOP", CENTER, 112);
}

void InfraDemo_Menu() {
	currentPage = '9';
	secondcurrentPage = '2';
	player.volume(20);
	player.play(2); // Plays the intro for the Infrared Demo 
	myGLCD.fillScr(255, 255, 255);

	myGLCD.drawBitmap(110, 60, 100, 100, ROBODEMO);
	delay(10500);
	currentPage = '9';
	secondcurrentPage = '2';
}

void InfraDemo_Sense() {
	player.volume(20);
	player.play(3);
	myGLCD.fillScr(255, 255, 255);
	myGLCD.drawBitmap(110, 60, 100, 100, INFRA);
	delay(14500);
	myGLCD.clrScr();
	secondcurrentPage = '9';
	currentPage = '1';
	Intro_Menu();
}
