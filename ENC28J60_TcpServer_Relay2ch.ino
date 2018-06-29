/*
 * UIPEthernet EchoServer example.
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This Hello World example sets up a server at 192.168.1.6 on port 1000.
 * Telnet here to access the service.  The uIP stack will also respond to
 * pings to test if you have successfully established a TCP connection to
 * the Arduino.
 *
 * This example was based upon uIP hello-world by Adam Dunkels <adam@sics.se>
 * Ported to the Arduino IDE by Adam Nielsen <malvineous@shikadi.net>
 * Adaption to Enc28J60 by Norbert Truchsess <norbert.truchsess@t-online.de>
 */

#include <UIPEthernet.h>
#include <EEPROM.h>

EthernetServer server = EthernetServer(21111);

//////////////////////
int pinout1 = 8;
int pinout2 = 9;
String line,selector;

int addressout1 = 0;
int addressout2 = 1;

int valueout1,valueout2;

//////////////////////
void setup()
{
//  EEPROM.begin(512);
  pinMode(pinout1, OUTPUT);
  pinMode(pinout2, OUTPUT);

  valueout1 = EEPROM.read(addressout1);
  valueout2 = EEPROM.read(addressout2);

  if(valueout1==1){
    digitalWrite(pinout1, LOW); // ให้ LED ติด
  }

  if(valueout1==0){
    digitalWrite(pinout1, HIGH); // ให้ LED ติด
  }

  if(valueout2==1){
    digitalWrite(pinout2, LOW); // ให้ LED ติด
  }

  if(valueout2==0){
    digitalWrite(pinout2, HIGH); // ให้ LED ติด
  }
  //start Ethernet
  
  Serial.begin(9600);

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,1,42);

  Ethernet.begin(mac,myIP);

  server.begin();
}

void loop()
{
  valueout1 = EEPROM.read(addressout1);
  valueout2 = EEPROM.read(addressout2);

  if(valueout1==1){
    digitalWrite(pinout1, LOW); // ให้ LED ติด
  }

  if(valueout1==0){
    digitalWrite(pinout1, HIGH); // ให้ LED ติด
  }

  if(valueout2==1){
    digitalWrite(pinout2, LOW); // ให้ LED ติด
  }

  if(valueout2==0){
    digitalWrite(pinout2, HIGH); // ให้ LED ติด
  }

        
  // Create a client connection
  EthernetClient client = server.available();
  if (!client) // ถ้าไม่มีการเชื่อมต่อมาใหม่
  return; // ส่งลับค่าว่าง ทำให้ลูปนี้ถูกยกเลิก
     while (client.connected()) { // วนรอบไปเรื่อย ๆ หากยังมีการเชื่อมต่ออยู่
    if (client.available()) { // ถ้ามีการส่งข้อมูลเข้ามา
      char c = client.read(); // อ่านข้อมูลออกมา 1 ไบต์
      if (c == '\r') { // ถ้าเป็น \r (return)
        Serial.println(line); // แสดงตัวแปร line ไปที่ Serial Monitor
        if (line == "OUTPUT1ON") { // ถ้าสงข้อความเข้ามาว่า LEDON
         // digitalWrite(pin, LOW); // ให้ LED ติด
          EEPROM.write(addressout1, 1);
        } 

        if (line == "OUTPUT1OFF") { // ถ้าสงข้อความเข้ามาว่า LEDOFF
         // digitalWrite(pin, HIGH); // ให้ LED ดับ
          EEPROM.write(addressout1, 0);
        }
       
        if (line == "OUTPUT2ON") { // ถ้าสงข้อความเข้ามาว่า LEDON
         // digitalWrite(pin, LOW); // ให้ LED ติด
          EEPROM.write(addressout2, 1);
        } 

        if (line == "OUTPUT2OFF") { // ถ้าสงข้อความเข้ามาว่า LEDOFF
         // digitalWrite(pin, HIGH); // ให้ LED ดับ
          EEPROM.write(addressout2, 0);
        }

  //      EEPROM.commit();


        selector = line;
        line = ""; // ล้างค่าตัวแปร line
        break; // ออกจากลูป
      } else if (c == '\n') { // ถ้าเป็น \n (new line)
        // Pass {new line}
   
      } else { // ถ้าไม่ใช่
        line += c; // เพิ่มข้อมูล 1 ไบต์ ไปต่อท้ายในตัวแปร line
      }

      
    }                    
  }
  //client.println("1");
  if(selector=="UPDATE"){
    client.println("%UPDATE%,"+String(valueout1)+","+String(valueout2));
  }else{
    client.println("%OK%");
  }
  selector = "";
  delay(10);//หน่วยเวลาเพื่อให้ส่ง client.println("%OK%");
  client.stop(); // ปิดการเชื่อมต่อกับ Client
  Serial.println("Client disconnect"); // ส่งข้อความว่า Client disconnect ไปที่ Serial Monitor
}

