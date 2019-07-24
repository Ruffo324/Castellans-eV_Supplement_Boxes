// #include <RCSwitch.h>
// #define ESP32

// RCSwitch mySwitch = RCSwitch();

// void setup()
// {
//   Serial.begin(9600);
//   //attachInterrupt(13);
//   mySwitch.enableReceive(1); // Receiver on interrupt 0 => that is pin #2
//   Serial.println("Setup done.");
// }

// void loop()
// {
//   if (mySwitch.available())
//   {

//     int value = mySwitch.getReceivedValue();

//     if (value == 0)
//     {
//       Serial.print("Unknown encoding");
//     }
//     else
//     {
//       Serial.print("Received ");
//       Serial.print(mySwitch.getReceivedValue());
//       Serial.print(" / ");
//       Serial.print(mySwitch.getReceivedBitlength());
//       Serial.print("bit ");
//       Serial.print("Protocol: ");
//       Serial.println(mySwitch.getReceivedProtocol());
//     }

//     mySwitch.resetAvailable();
//   }
//   else
//   {
//     mySwitch.enableReceive(1); // Receiver on interrupt 0 => that is pin #2

//     int value = mySwitch.getReceivedValue();
//     Serial.println(value);
//     delay(100); // Wait 500ms
//   }
// }
