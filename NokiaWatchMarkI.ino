// This project is initiated by Bogdan Ionescu AKA BogdanTheGeek
// it is meant to be opensource

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Time.h>

#define aref_voltage 3.3 //set the analog reference to 3.3v

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - display chip select (CS)
// pin 3 - display reset (RST)
// pin 12 - the backlight

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int state=0;    //variable for "next" button
int state2 = 0; // variable for 2nd button
int page=0;     // variable for the current page 
int state3=0;   // variable for 3rd button
int setPage=0;  // variable for the current page in settings menu
int NoOfPages=4;// ...pretty explanitory 
int ClockFace=0;// variable for the which clock face you are on 
int stateB=0;   // a second variable for the next button...for when you are in the settings menu

//int y=48;

int tempReading;  //variable for the temperature
int contrast = 60;// set teh initial contrast


static const unsigned char PROGMEM Ther[] ={ // bitmap for temperature sensor
B00000011,B11000000,
B00000011,B11000000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001100,B00110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00001111,B11110000,
B00111111,B11111100,
B00111111,B11111100,
B11111111,B11111111,
B11111111,B11111111,
B11111111,B11111111,
B11111111,B11111111,
B11111111,B11111111,
B11111111,B11111111,
B00111111,B11111100,
B00111111,B11111100,
B00001111,B11110000,
B00001111,B11110000,
};

static const unsigned char PROGMEM Degree[] ={ //bitmap for degree symbol
B00111100,
B01111110,
B11100111,
B11000011,
B11000011,
B11100111,
B01111110,
B00111100
  
};

void setup (){
  
  pinMode(9, INPUT_PULLUP);  //set the buttons as an input with an internal pullup so when it connects to GND it registers a low
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  analogReference(EXTERNAL);  //set the analog reference to external 3.3
   pinMode(12,OUTPUT); //this is the pin for the backlight 
  digitalWrite(12,HIGH);
  setTime(0,0,0,14,9,14);//set the time
  
  display.begin();//initialize the display
  display.setContrast(contrast); 
  
  
  display.display(); // show splashscreen
  delay(5000);
  display.clearDisplay();
  
  
}

void loop(){
  
  display.clearDisplay();
  
  
  if((second()%10)==0){  //makes a reading every time the no of secons ,divides by 10....so every 10 secons
   
   tempReading = analogRead(1);
  }
  
  
  switch (page){ // switch between pages
    case 0:
    button();
    showClock();
    break;
    
    case 1:
    button();
    temp();
    break;
    
    case 2:
    button();
    Contrast();
    break;
    
    case 3:
    settings();
    break;
  }
  
  display.display(); //write it to the screen
  
  
   
}

void showClock(){ 
  

  state3=digitalRead(11);//read the button
  
  if (ClockFace==2){ //when it gets to the last clock face, it goes back to the first page
   ClockFace=0; 
  }
  
  if ( state3==LOW){
    ClockFace++;
    delay(200);
    
  }

  
  switch ( ClockFace ){  //this switches between the clock faces
    
    case 0:
    Digital();
    break;
    case 1:
    classic();
    break;
  }
  
  
}


void Digital(){ //the digital clock face
  
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(15,3);
  
  printDigits(hourFormat12());
  
  int x=second()%2; // make the colon tick every even nomber of seconds
  if (x==0){
  display.print(":");
  }
  else {
  display.print(" ");
  }
  
  printDigits(minute());
  
  //display.print(":");
  //printDigits(second());
  
  
  display.setTextSize(1);
  display.setCursor(37,18);
  PmAm();
  
  
  display.setCursor(0,display.height()-9);
  display.setTextSize(1);
  
  switch (weekday()){ // print the mane of the day based on the value from the time library
    case 1:
    display.print("Sun,");
    break;
    case 2:
    display.print("Mon,");
    break;
    case 3:
    display.print("Tue,");
    break;
    case 4:
    display.print("Wed,");
    break;
    case 5:
    display.print("Thu,");
    break;
    case 6:
    display.print("Fri,");
    break;
    case 7:
    display.print("Sat,");
    break;
  }
    
  
  display.print(day());
  display.print("/");
  display.print(month());
  display.print("/");
  display.print(year());
  
  
}






void printDigits(int digits){//prints a zero before the digits, it is less than 10
  
  if(digits < 10)
    display.print('0');
  display.print(digits);
}






 void PmAm(){ // shows pm or am
  if (isPM() == 1){
   
   display.print("pm");
   
  }
  if (isPM() == 0){
   
   display.print("am");
   
  }  
 }
 
 
 
 
 
 void button(){  //switch between pages 
   state=digitalRead(9);
   
   if (state == LOW){
     
     page=page+1;
     delay(300);
     
   }
    if( page >= NoOfPages){
   page = 0;
 }
 }
 
 
 
 
 
 void temp(){
 
   
   
   float voltage = tempReading * aref_voltage;
  voltage /= 1024.0;
  
   float temperatureC = (voltage - 0.5) * 100 ;
   
  
     
   display.setCursor(20,12);
   display.drawBitmap(0,6,Ther,16,36,1);
   display.setTextSize(3),
   display.drawBitmap(56,12,Degree,8,8,1);
   display.print(int(temperatureC));
   display.setCursor(67,12);
   display.print("C");
   
   
   
   
   
 }
 
 void settings(){
   
   
   state2=digitalRead(10);
   state3=digitalRead(11);
   stateB=digitalRead(9);
   
   if(stateB==LOW){
   setPage++;
   delay(300);
   }
   
   
   if (setPage==4){ 
     setPage=0;
     page=0;
   }
   int k=second()%2;
   
   switch (setPage){
     
     case 0:
   display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(BLACK);
   
  display.clearDisplay();
  display.println("Set hour:");
  
  display.setTextSize(2);
  display.setCursor(28,14);
  
  if (k==0){ // this makes the text flicker
  display.setTextColor(WHITE,BLACK);
  }
  else {
  display.setTextColor(BLACK);
  }
  
  
  printDigits(hour());
  
  if (state3==LOW){ //adds one hour in seconds
    adjustTime(3600);
    delay(200);
  }
  if (state2==LOW){ //subtracts one hour in seconds
    adjustTime(-3600);
    delay(200);
  }
    break;
    
    case 1: 
   display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(BLACK); 
   
  display.clearDisplay(); 
  display.println("Set minutes:");
  
  display.setTextSize(2);
  display.setCursor(28,14);
  if (k==0){
  display.setTextColor(WHITE,BLACK);
  }
  else {
  display.setTextColor(BLACK);
  }
  
  
  printDigits(minute());
  
  if (state3==LOW){ //adds one min in seconds
    adjustTime(60);
    delay(200);
    }
    if (state2==LOW){ // subtracts one min in seconds
    adjustTime(-60);
    delay(200);
    }
    
    break;
    
    case 2:
   display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(BLACK);
   
  display.clearDisplay(); 
  display.println("Set day:");
  
  display.setTextSize(2);
  display.setCursor(10,14);
  if (k==0){
  display.setTextColor(WHITE,BLACK);
  }
  else {
  display.setTextColor(BLACK);
  }
  
  
  printDigits(day());
  display.print(".");
  printDigits(month());
  
  if (state3==LOW){ //adds one day in seconds
    adjustTime(86400);
    delay(200);
 }
 if (state2==LOW){
    adjustTime(-86400);//substracts one day in secons
    delay(200);
 }

   break;
  
    case 3:
   display.setCursor(0,0);
   display.setTextSize(1);
   display.setTextColor(BLACK);
   
  display.clearDisplay(); 
  display.println("Set year:");
  
  display.setTextSize(2);
  display.setCursor(16,14);
  if (k==0){
  display.setTextColor(WHITE,BLACK);
  }
  else {
  display.setTextColor(BLACK);
  }
  
  
  display.println(year());
  
  if (state3==LOW){  //adds one year in seconds :)
    adjustTime(31536000);
    delay(200);
 }
 if (state2==LOW){
    adjustTime(-31536000); //subtracts one year in seconds
    delay(200);
 }

   break; 
 }

}

void Contrast(){
  
  int bar1 = contrast-33; // lenght of the bar
  constrain( contrast ,33,75); // constrain the contrast variable between 33 in 75
  
  
  state2=digitalRead(10);
  state3=digitalRead(11);
   
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Contrast");
  display.drawRect(10,display.height()-20,display.width()-20,10,BLACK);
  
  for (int i=0;i<6;i++){      // this is a very fast way of drawing many parallel lines 
  display.drawFastHLine(12,display.height()-18+i,bar1,BLACK);
  }
  
  if ( state3==LOW){//increase contrast
    contrast++;
    delay(200);
    display.setContrast(contrast);
  }
  if ( state2==LOW){// decrease contrast
    contrast--;
    delay(200);
    display.setContrast(contrast);
  }
  
  
}


void classic(){    //the analogue clock face
   display.setTextSize(1);
   display.setTextColor(BLACK);
   display.setCursor(33,0);
   display.print("XII");
   display.setCursor(66,21);
   display.print("III");
   display.setCursor(0,21);
   display.print("IX");
   display.setCursor(36,40);
   display.print("VI");
   
   
  int xs = 42 + 50 * sin(PI/30 * second()); //this sets the co-ordinates of the clock hands ...the 50 is the lenght...the rest is just trigonometry ( you dont have to understand it ..it just works :))
  int ys = 24 - 50 * cos(PI/30 * second());
  int xm = 42 + 20 * sin(PI/30 * minute());
  int ym = 24 - 20 * cos(PI/30 * minute());
  int xh = 42 + 15 * sin(PI/6 * hour());
  int yh = 24 - 15 * cos(PI/6 * hour());
   
   
 
   display.fillCircle(display.width()/2,display.height()/2,2,BLACK); //the center of the clock
   
   display.drawLine(display.width()/2,display.height()/2,xs,ys,BLACK); //draw the hands of the clock
   display.drawLine(display.width()/2,display.height()/2,xm,ym,BLACK);
   display.drawLine(display.width()/2,display.height()/2,xh,yh,BLACK);
   
  
  
}




/*void Sleep(){   // this is a beta code for a CRT shutting down animation ...it is in beta becouse it doesn't look like a CRT :)
  
  
  
  
  if (y<=0){
    
    y=48;
    delay(500);
  }
  else{
  display.fillCircle(display.width()/2,display.height()/2,y,BLACK);
  display.display();
  delay(5);
 
  y=y-2;
  }
}
*/
