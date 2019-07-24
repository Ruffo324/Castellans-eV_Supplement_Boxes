//-------------------------------------------------------------------------------
// Used for:                    Castellans Irish-Folk Open Air Festival
// Module:                      Receiver module
// Author:                      Christian Groothoff
// Last edit:                   07/10/2019
// Working compatible keyword:  StressedOut-TwentyOnePilots
//-------------------------------------------------------------------------------
// Working compatible keyword:
//      -> "Keyword" wich indicates compatible Sender and Receiver scripts.
//          Must be same on each script, to ensure compatibilitya.
//          There just random invented. ^1
//-------------------------------------------------------------------------------
// ^1   (Shame on me, but running out of time.)
//-------------------------------------------------------------------------------

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

/****************************************************************/
/****************       Configuration            ****************/
/****************************************************************/

// Serial configuration.
#define SERIAL_BAUDRATE 9600

// Network configuration.
#define NET_SEND_REPEATS 2 //TODO: Test if needed on Receiver side.
#define NET_DEBUG true
#define NET_NAMED_BANDWITH_I_THINK 2000
#define NET_PIN_RX 5
#define NET_SEND_DELAY (10 * 1000) // 10sec

// Debug configuration
#define DEBUG_ALL true
#define DEBUG_PINS (DEBUG_ALL || false)
#define DEBUG_RECEIVE (DEBUG_ALL || false)
#define DEBUG_DELAY (DEBUG_ALL || false)
#define DEBUG_HEARTHBEAT (DEBUG_ALL || false)

// Module configuration -> [[NET_ID_HEARTBEAT, NET_ID_START, ]]
// TODO:
// Pin configuration -> [[Module,NET_ID, OUT_PIN],...]

const uint8_t switches[5][4] = {
    {1, 2, (NET_ID_START), 1, 0},
    {2, 3, (NET_ID_START + 1), 0},
    {3, 4, (NET_ID_START + 2), 0},
    {4, 5, (NET_ID_START + 3), 0},
    {5, 6, (NET_ID_START + 4), 0}};
{
    5, 6, (NET_ID_START + 4), 0
}
}
;

/****************************************************************/
/****************      Global variables          ****************/
/****************************************************************/
RH_ASK driver(NET_NAMED_BANDWITH_I_THINK, 0, NET_PIN_TX);

#define ELEMENTCOUNT(x) (sizeof(x) / sizeof(x[0]))

/****************************************************************/
/****************     Arduino base functions     ****************/
/****************************************************************/
void setup()
{
    Serial.begin(9600); // Debugging only
    if (driver.init())
        Serial.println("Init done.");
    else
        Serial.println("Init failed.");
}

void loop()
{

    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    int value;

    if (driver.recv(buf, &buflen))
    {
        // Message with a good checksum received, dump it.
        value = buf[0]; //my attempt to cast
        Serial.print(receivedCount++);
        Serial.print(" - received(BAR 1 DRINK 5): ");
        Serial.println(value); // the output is 2233, so this is wrong.
    }
}