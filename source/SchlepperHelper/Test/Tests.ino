//-------------------------------------------------------------------------------
// Used for:                    Castellans Irish-Folk Open Air Festival
// Module:                      Bar(Sender) module
// Author:                      Christian Groothoff
// Last edit:                   07/10/2019
// Working compatible keyword:  StressedOut-TwentyOnePilots
//-------------------------------------------------------------------------------
// Working compatible keyword:
//      -> "Keyword" wich indicates compatible Sender and Receiver scripts.
//          Must be same on each script, to ensure compatibilitya.
//          There just random invented. [^1]
//-------------------------------------------------------------------------------
// [^1]   (Shame on me, but running out of time.)
//-------------------------------------------------------------------------------

#include <SPI.h> // Needed for compile.

// Serial configuration.
#define SERIAL_BAUDRATE 9600

// Pin configuration -> [[SWITCH, PIN, NET_ID],...]
const byte input_pins[4] = {
    2, 3, 4, 5};
// 2,
// 3,
// 4,
// 5,
// 6,
// 7,
// 8,
// 9,
// 10,
// 11,
// 12,
// A1,
// A2,
// A3,
// A4,

/****************************************************************/
/****************      Global variables          ****************/
/****************************************************************/

#define ELEMENTCOUNT(x) (sizeof(x) / sizeof(x[0]))

/****************************************************************/
/****************     Arduino base functions     ****************/
/****************************************************************/
void setup()
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("[SERIAL]    Serial connection initialized.");

    for (int i = 0; i < ELEMENTCOUNT(input_pins); i++)
        pinMode(input_pins[i], INPUT);

    Serial.println("[PIN]       Pins initialized.");

    //pinMode(PIN_STATUS, OUTPUT); // Needed for status output.
}

void loop()
{
    for (int i = 0; i < ELEMENTCOUNT(input_pins); i++)
    {
        int pinValue = digitalRead(input_pins[i]);

        Serial.print("[PIN]     PIN: ");
        Serial.print(input_pins[i]);
        Serial.print("\tVALUE: ");
        if (pinValue == LOW)
            Serial.println("LOW");
        else
            Serial.println("HIGH");
    }

    Serial.println("--------------------------------");

    delay(1000);
}