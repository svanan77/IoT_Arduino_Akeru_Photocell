#include <SoftwareSerial.h>
#define PHOTO_PIN A0                            //Wire connected to Analog Pin A0 at Dev board to do the Photoresistor reading 
#define SAMPLING_RATE 30000                     //Sample every 30 sec. 1 sec = 1000 ms 

SoftwareSerial sigfox(5,4);                     //RX_Pin = 5, TX_Pin = 4. The pins are reserved on the SnootLab Akeru board to serially communicate with the embedded TD1208 Sigfox module.
int brightness;
int i = 0;
String data_0,data_1,data_2,data_3,data_4,data_5,data_6,data_7;
                  
void setup(void) {
  Serial.begin(9600);                           //Baud rate for talking with the COM port  
  sigfox.begin(9600);                           //Baud rate for talking with the embedded TD1208 module          
}

void loop(void) {
  brightness = analogRead(PHOTO_PIN);           //Dev board has 10 bit A2D converter. analogRead maps 0-5 V analog input to 0-1023 digital output. 1 unit step = 0.0049 volts (4.9 mV) 
  Serial.print("Brightness = 4.9 mV x ");
  Serial.println(brightness);
  equalSizeData(String(brightness, HEX));
  delay(SAMPLING_RATE);
}

void equalSizeData(String data){                //Send equal size frame. Pad with "0" to make it 3 symbols (max output 1024 -> 3FF Hex) if less.
  if (data.length() == 1){     
    data = "00"+data;
  }
  else if (data.length() == 2){  
    data = "0"+data;
  }
  
  makeFrame(data);                              //Regulation allows SigFox to send upto 140 messages/day/device. Each message can have max 12 bytes (1 byte = 2 Hex). To max utilization we send 8 frames with 3 Hex symbols in each message.
  return; 
}

void makeFrame(String data){  
  Serial.print("i: ");  
  Serial.print(i); 
  Serial.print(",data: ");              
  Serial.print(data);   
  switch (i) {
    case 0:
      data_0 = data;
      Serial.print(",data_0: ");         
      Serial.println(data_0);            
      i++;
      break;
    case 1:
      data_1 = data;
      Serial.print(",data_1: "); 
      Serial.println(data_1);                  
      i++;
      break;
    case 2:
      data_2 = data;
      Serial.print(",data_2: ");       
      Serial.println(data_2);                  
      i++;
      break;
    case 3:
      data_3 = data;
      Serial.print(",data_3: ");       
      Serial.println(data_3);        
      i++;      
      break;
    case 4:
      data_4 = data;
      Serial.print(",data_4: ");       
      Serial.println(data_4);        
      i++;      
      break;
    case 5:
      data_5 = data;
      Serial.print(",data_5: ");       
      Serial.println(data_5);        
      i++;      
      break;                        
    case 6:
      data_6 = data;
      Serial.print(",data_6: ");       
      Serial.println(data_6);        
      i++;      
      break;                        
    case 7:
      data_7 = data;
      Serial.print(",data_7: ");       
      Serial.println(data_7);        
      i++;          
      break;                        
    break;
  }

   if (i==8){
       String frame=data_0+data_1+data_2+data_3+data_4+data_5+data_6+data_7;
       sendFrame(frame);
       i=0;
   }
  return;    
}

void sendFrame(String frame){                   //Actual transmission of full 12 bytes (8 Hex frames) message

  Serial.print("Sending Hex Frame: ");
  Serial.println(frame);

  sigfox.write("AT$SF=");                     //AT (Attention) SF (SigFox) command. Modem/Hayes command
  sigfox.print(frame);
  sigfox.write("\n");
  return;
}

  
