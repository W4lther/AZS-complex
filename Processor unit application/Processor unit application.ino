
//keyboard
#include <Keypad.h> // Подключаем библиотеку 
const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 
byte rowPins[ROWS] = {23, 25, 27, 29}; 
byte colPins[COLS] = {31,33, 35, 37}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

bool volume_done=false;


bool pin_done=false;

//key



//rfid
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53        // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key; 

byte nuidPICC[4];

//rfid






//lcd
#include "U8glib.h"
U8GLIB_ST7920_128X64_4X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
#include "rusUnifont.h"
void u8g_prepare(void) {
  u8g.setFont(rusUnifont);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
 // u8g.setFontPosTop();
}





void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0

//lcd

//-- relay
const int pulsePin = 3;     
const int ledPin =  13;     
const int relay1 =  22;      
const int relay2 =  24;     
const int relay3 =  26;  

//relay


// variables will change:
int buttonState = 0;       
int counter;
const int resetPin =  2;  
float timerlcd;
float timer_time;

int pin_counter;

float limitset;   // переменная для хранения полученного байта
void setup() {
  pin_counter=0;
  u8g_prepare();
  limitset=0;
timer_time=millis();
timerlcd=500;
    Serial.begin(9600);   // Initialize serial communications with the PC
      Serial.setTimeout(50);

      
 // while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
 
    rfid.PCD_Init(); // Init MFRC522 
//    Serial.println(F("Ready to ACTION"));

   for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
     //printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  }
  
  counter=0;
  // initialize the LED pin as an output:
  // digitalWrite(resetPin, HIGH);
   pinMode(resetPin, OUTPUT);
        pinMode(ledPin, OUTPUT);
        pinMode(relay1, OUTPUT);
        pinMode(relay2, OUTPUT);
         pinMode(relay3, OUTPUT);

        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
       
         pinMode(pulsePin, INPUT);
         
 // Serial.begin(9600);
  // initialize the pushbutton pin as an input:
 
}
float limitset_serial=0;
bool nocard=true;
float timer=0;
bool unstart=true;
bool down=false;
bool start=true;
bool pinread=false;

void loop() {

  
 if(nocard)
     {
draw_awaiting();
     }
     /*
    u8g.firstPage();  
  do {
  u8g.drawStr( 0, 22, "Приложите карту");

u8g.drawStr( 15, 40, "АЗС ПОГАТ");

  } while( u8g.nextPage() );
*/
    // }
float ID=0;
  bool new_serial=false;
  int pincodeserial=0;
//rfid
  // Verify if the NUID has been readed
    if (  rfid.PICC_IsNewCardPresent())
 {  // return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 // Serial.println(rfid.PICC_GetTypeName(piccType));
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //Serial.println(F("READ ERROR"));
    return;
  }
   
     for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   // printHex(rfid.uid.uidByte, rfid.uid.size);
//    printDec(rfid.uid.uidByte, rfid.uid.size);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  //rfid---
int i=0;
String stringVar;
 for (byte i = 0; i < rfid.uid.size; i++) {
   // Serial.print(buffer[i] < 0x10 ? " 0" : " ");
  // ID =ID*10+ ((rfid.uid.uidByte[i])-'O');
  stringVar =stringVar+  String(rfid.uid.uidByte[i], DEC);
 
  }
ID=(float)stringVar.toInt();
printDec(rfid.uid.uidByte, rfid.uid.size);
//Serial.println(ID);
/*if(ID==691031983)
{*/

  //limitset=10;
  
 // do_azs(limitset);
//}





  // read the state of the pushbutton value:
 
 
 }
pinread=false;





int pinserial=0;


 if(Serial.available()>0)
    {
        while(Serial.available()>0)
        {
          char reader=Serial.read();
    new_serial=true;
    
    if(reader!='!')
    {
        limitset_serial =limitset_serial*10+ (int(reader)- 48);
    }
    else
    {
      for(int i=0;i<4;i++)
      {
        reader=Serial.read();
      pinserial=pinserial*10+ (int(reader)- 48);
      }
    }
       // Serial.println(limitset_serial);
        }
        

      
        
             if(new_serial)
        {
          limitset=(float)limitset_serial/(float)100;//+(int(limitset_serial)%100);
          limitset_serial=0;
         // Serial.println(limitset);
           // Serial.println(pinserial);
          new_serial=false;
          nocard=false;
          volume_done=false;
           pin_done=false;
int a=do_pin();
if(a!=pinserial)
{
  draw_wrong();
}
else
{
  float aaa=do_volume(limitset);
  
  do_azs(aaa);
}
          
        //  do_azs(limitset);
        }
     }
    
    }



void draw_max()
{
  float timerwrong=millis();
  while(millis()<(timerwrong+3000))
  {
   u8g.firstPage();  
  do {
      u8g.drawStr( 5,20, "Введено больше ");
  u8g.drawStr( 10,40, "вашего лимита");
  //u8g.drawFrame(30,35,65,20);

  } while( u8g.nextPage() );
}

}

bool nulled=true;


void draw_wrong()
{
  float timerwrong=millis();
  while(millis()<(timerwrong+3000))
  {
   u8g.firstPage();  
  do {
      u8g.drawStr( 5,20, "Введен неверный код");
  u8g.drawStr( 50,40, "код");
  //u8g.drawFrame(30,35,65,20);

  } while( u8g.nextPage() );
}
resetFunc();
}

int do_pin()
{
  String pincode="";
   while (!pin_done)
   {

    char key = keypad.getKey();
    
 
    if((key=='A')&&(pincode.length()==4))
    {
    pin_done=true;
    return pincode.toInt();
    }
    
    else
    {
      switch(key)
      {
        case '1' :
        {
         pincode=pincode+"1";
          break;
        }
          case '2' :
        {  pincode=pincode+"2";
          break;
        }
          case '3' :
        { pincode=pincode+"3";
          break;
        }
          case '4' :
        { pincode=pincode+"4";
          break;
        }
          case '5' :
        { pincode=pincode+"5";
          break;
        }
          case '6' :
        { pincode=pincode+"6";
          break;
        }
          case '7' :
        { pincode=pincode+"7";
          break;
        }
          case '8' :
        {  pincode=pincode+"8";
          break;
        }
          case '9' :
        {  pincode=pincode+"9";
          break;
        }
          case '0' :
        {
          pincode=pincode+"0";
          break;
        }
     /*       case 'A' :
        {
          temp=temp*10;
          break;
        }*/
            case 'B' :
        {
         resetFunc();
          break;
        }
            case 'C' :
        {
         pincode="";
          break;
        }
        
        
        }

        if(pincode.length()>4)
        {
         pincode="";
        }
      draw_pincode(pincode.length());
    }
   }
}

float do_volume(float litres)
{
  float temp=0;
float counter=0;
   while (!volume_done)
   {

    char key = keypad.getKey();
    

 if((key=='D'))
    {

      volume_done=true;
    return litres;
    }
    if((key=='A')&&(temp>0))
    {
      if((litres>temp)||(litres==temp))
      {
    volume_done=true;
    return temp;
      }
    else
    {
      draw_max();
      temp=0;
      nulled=true;
    }
    }
    else
    {
      switch(key)
      {
        case '1' :
        {
          if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.01;
          break;
        }
          case '2' :
        { if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.02;
          break;
        }
          case '3' :
        {if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.03;
          break;
        }
          case '4' :
        { if(temp>0)
          {
            temp=temp*10;
          }
          
          temp=temp+.04;
          break;
        }
          case '5' :
        {if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.05;
          break;
        }
          case '6' :
        {if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.06;
          break;
        }
          case '7' :
        {if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.07;
          break;
        }
          case '8' :
        { if(temp>0)
          {
            temp=temp*10;
          }
          temp=temp+.08;
          break;
        }
          case '9' :
        { if(temp>0)
          {
            temp=temp*10;
          }
          
          temp=temp+.09;
          break;
        }
          case '0' :
        {
          temp=temp*10;
          break;
        }
     /*       case 'A' :
        {
          temp=temp*10;
          break;
        }*/
            case 'B' :
        {
         resetFunc();
          break;
        }
            case 'C' :
        {
          temp=0;
          nulled=true;
          break;
        }
        
        
        }
if(temp>0)
{
if(nulled)
{
 // temp=temp*0.01;
  nulled=false;
}}
        if(temp>10000)
        {
          temp=0;
        }
        draw_litres(litres,temp);
    }
   }
  
}
/*void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
   // Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i]);
  }
  Serial.println();
}
/*
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}*/
bool pulse;
float pulse_timer;
void draw_pincode(int pinlength)
{



    u8g.firstPage();  
  do {
      u8g.drawStr( 5,20, "Введите пин-код");
//u8g.drawStr( 10, 45, "Введите объем");
  u8g.drawFrame(30,35,65,20);
  for(int i=0;i<pinlength;i++)
  {
   u8g.drawStr( 35+i*15, 49, "*");
  }
  } while( u8g.nextPage() );
}

///////////////////////////////draw





void do_azs(float litres)
{   
  
  float litres_done=litres;
  pulse_timer=0;
  pulse=false;
float timer_stop=millis();
  
  float done=0;
    //delay(1000);
      digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
  while((done-0.01f)<litres)
  
  {
if((timer_stop+(2*60*1000))<millis())
{
litres_done=done;
        break;
}
    
      char key = keypad.getKey();
      if(key=='D')
      {
        litres_done=done;
        break;
      }
     
     buttonState = digitalRead(pulsePin);
     if((millis()-pulse_timer)>100){
     /*if ((buttonState == HIGH)) {
      if(!pulse)
      {
        pulse_timer=millis();
      done=done+0.01f*99;
      pulse=true;
     // Serial.println(done);
      }
     }
     else

    {
     pulse=false;
    }*/
     done=done+0.01f;
    
     }
    
       
        if((litres-done)<1)
        {
        digitalWrite(relay3, HIGH);
        }
         litres_done=litres;
  }
Serial.print("divi!");
    Serial.print(litres_done);
     Serial.print("!");  
        Serial.println();
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
draw_thank();
//Serial.println("Done");

delay(1000);
      resetFunc(); 
  
  }


void draw_thank()
{
  float timerwrong=millis();
  while(millis()<(timerwrong+3000))
  {
   u8g.firstPage();  
  do {
     u8g.drawStr( 15, 22, "Спасибо, что");

u8g.drawStr( 5, 40, "воспользовались");

u8g.drawStr( 30 ,40+18, "нашей АЗС");
  } while( u8g.nextPage() );
}
resetFunc();
}




void draw_awaiting()
{




    u8g.firstPage();  
  do {
  u8g.drawStr( 5, 22, "Приложите карту");

u8g.drawStr( 30, 40, "АЗС ПОГАТ");

u8g.drawStr( 40 ,40+18, "v1.3.5");
  } while( u8g.nextPage() );
}

void draw_litres(float litres, float volume)
{


//u8g.drawStr( 0, 64-10, "0.00 ");
 enum {BufSize=9}; // If a is short use a smaller number, eg 5 or 6 
char buf[BufSize];
char buf1[BufSize];
char t[10];
dtostrf(litres, 5, 2, t);
  sprintf(buf, "%s", t);


dtostrf(volume, 5, 2, t);
  sprintf(buf1, "%s", t);
 

    u8g.firstPage();  
  do {
      u8g.drawStr( 20, 11, "Ваш лимит");
u8g.drawStr( 10, 45, "Введите объем");
   u8g.drawStr( 30, 29, buf);
        u8g.drawStr( 30, 59, buf1);
  } while( u8g.nextPage() );
}

void draw(void) {

  
  // graphic commands to redraw the complete screen should be placed here  

  //u8g.setFont(u8g_font_osb21);

  if(!nocard)
  {
  //String temp="Ваш баланс: " +String(limitset);


u8g.drawStr( 701, 64-10, "литров");
  enum {BufSize=9}; // If a is short use a smaller number, eg 5 or 6 
char buf[BufSize];
char t[10];
dtostrf(limitset, 5, 2, t);
  sprintf(buf, "%s", t);
//snprintf (buf, BufSize, "%'.2f",double(limitset) );
//Serial.println(buf);
//Serial.println(buf);
   u8g.drawStr( 86, 17, buf);
    //  Serial.println(limitset);
      }

   else
   {

   }
   }

   void printDec(byte *buffer, byte bufferSize) {
    Serial.print("rfid!");
  for (byte i = 0; i < bufferSize; i++) {
 //   Serial.print(buffer[i] < 0x10 ? " 0" : " ");
  
    Serial.print(buffer[i], DEC);

  }
   Serial.print("!");
      Serial.println();
      
}

