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

#include <RH_ASK.h> // 433mhz libary.
#include <SPI.h>    // Needed for compile.

/****************************************************************/
/****************       Configuration            ****************/
/****************************************************************/

// DON'T CHANGE. NEED'S THE SAME ON EACH MODULE! [^1]
#define MODULE_SWITCHES 6                         // Amount of input switches on the module.
#define MODULE_NET_IDS_NEEDED MODULE_SWITCHES + 1 // (Switches + Heartbeat(1))

// Module based configuration. (PLEASE CHANGE [^1])
#define MODULE_NUMBER 1 // Number of the Module, needed to calculate NET_IDS.

// Module special pins configuration. (CAN BE CHANGED)
#define PIN_NET_TX 13
#define PIN_STATUS A0

// Calculated Network-ID stuff. (CALCULATED, DON'T CHANGE ALSO. [^1])
#define NET_ID_START (MODULE_NUMBER * MODULE_NET_IDS_NEEDED) - MODULE_NET_IDS_NEEDED // Start of modules NET_ID'S.
#define NET_ID_HEARTHBEAT NET_ID_START

// Status configuration.
#define STATUS_WRONG_CONFIGURATION_FLASHING 100

// Network configuration.
#define NET_SEND_REPEATS 5 //TODO: Few testings if needed anymore.
#define NET_NAMED_BANDWITH_I_THINK 2000
#define NET_SEND_DELAY (1 * 1000)  // 1sec
#define NET_HEARTBEAT_EACH_SENDS 6 // Send hearthbeat

// Serial configuration.
#define SERIAL_BAUDRATE 9600

// Debug configuration
#define DEBUG_ALL true
#define DEBUG_PINS (DEBUG_ALL && true)
#define DEBUG_SEND (DEBUG_ALL && true)
#define DEBUG_DELAY (DEBUG_ALL && false)
#define DEBUG_HEARTHBEAT (DEBUG_ALL && true)

// Pin configuration -> [[SWITCH, PIN, NET_ID],...]
const uint8_t switches[MODULE_SWITCHES][3] = {
    {1, 2, (NET_ID_START)},
    {2, 3, (NET_ID_START + 1)},
    {3, 4, (NET_ID_START + 2)},
    {4, 5, (NET_ID_START + 3)},
    {5, 6, (NET_ID_START + 4)},
    {6, 7, (NET_ID_START + 5)},
};

/****************************************************************/
/****************      Global variables          ****************/
/****************************************************************/
RH_ASK driver(NET_NAMED_BANDWITH_I_THINK, 0, PIN_NET_TX);

#define ELEMENTCOUNT(x) (sizeof(x) / sizeof(x[0]))

int _countHeartbeatSkipped = 0;

/****************************************************************/
/****************     Arduino base functions     ****************/
/****************************************************************/
void setup()
{
    pinMode(PIN_STATUS, OUTPUT); // Needed for status output.

    initSerialMonitor();
    initRadio();
    initSwitchPins();
}

void loop()
{
    processSwitches();
    processHearthbeat();

    processNetSendDelay();

    _countHeartbeatSkipped++;
}

/****************************************************************/
/****************        Init Stuff              ****************/
/****************************************************************/
void initSerialMonitor()
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("[SERIAL]    Serial connection initialized.");
}

void initRadio()
{
    if (driver.init())
        Serial.println("[NET]       Network initialized.");
    else
        Serial.println("[NET]       Network failed to initialize.");
}

// Initialize the pinMode for the switch pins.
void initSwitchPins()
{
    if (ELEMENTCOUNT(switches) != MODULE_SWITCHES)
    {
        Serial.print("[ERR]    IVALID AMOUNT OF SWITCH-PINS! '");
        Serial.print(MODULE_SWITCHES);
        Serial.println("' exactly needed.");
        wrongConfiguration();
    }

    for (int i = 0; i < ELEMENTCOUNT(switches); i++)
        pinMode(switches[i][1], INPUT);

    Serial.println("[PIN]    Pins initialized.");
}

// Flashes LED with STATUS_WRONG_CONFIGURATION_FLASHING delay.
void wrongConfiguration()
{
    while (true)
    {
        digitalWrite(PIN_STATUS, HIGH);
        delay(STATUS_WRONG_CONFIGURATION_FLASHING);
        digitalWrite(PIN_STATUS, LOW);
        delay(STATUS_WRONG_CONFIGURATION_FLASHING);
    }
}

/****************************************************************/
/****************        Processing              ****************/
/****************************************************************/

// Handles the NET_SEND_DELAY, and debug messages for it.
void processNetSendDelay()
{
    if (DEBUG_DELAY)
    {
        Serial.print("[DELAY]   Started (");
        Serial.print(NET_SEND_DELAY);
        Serial.print("ms) ... ");
    }

    delay(NET_SEND_DELAY);

    if (DEBUG_DELAY)
        Serial.println("done!");
}

void processSwitches()
{
    for (int i = 0; i < ELEMENTCOUNT(switches); i++)
    {
        int pinValue = digitalRead(switches[i][1]);

        if (pinValue == LOW)
            AgressiveSend(switches[i][2]);

        if (DEBUG_PINS)
        {
            Serial.print("[PIN]     Switch: ");
            Serial.print(switches[i][0]);
            Serial.print("\tPin: ");
            Serial.print(switches[i][1]);
            Serial.print("\tNET-ID: ");
            Serial.print(switches[i][2]);
            Serial.print("\tVALUE: ");
            Serial.println(pinValue);
        }
    }
}

void processHearthbeat()
{
    digitalWrite(PIN_STATUS, _countHeartbeatSkipped % 2 == 0); // EACH LOOP ON/OFF
    if (_countHeartbeatSkipped >= NET_HEARTBEAT_EACH_SENDS)
    {
        if (DEBUG_HEARTHBEAT)
            Serial.println("[HEARTH]    Pulsed.");

        AgressiveSend(NET_ID_HEARTHBEAT);
        _countHeartbeatSkipped = 0;
    }
}
/****************************************************************/
/****************        Network Stuff           ****************/
/****************************************************************/

// Sends the given number multiple times (NET_SEND_REPEATS), to avoid signal loss.
bool AgressiveSend(uint8_t number)
{
    bool success = true;
    int timesSend = 0;

    uint8_t *data = &number;
    for (int i = 0; i < NET_SEND_REPEATS; i++)
    {
        digitalWrite(PIN_STATUS, HIGH); // Blink

        if (!driver.send(data, sizeof(number)))
            success = false;
        else
            timesSend++;
        digitalWrite(PIN_STATUS, LOW); // Blink
    }
    driver.waitPacketSent();

    if (DEBUG_SEND)
    {
        Serial.print("[SEND]    '");
        Serial.print(number);
        Serial.print("' (");
        Serial.print(timesSend);
        Serial.println("x)");
    }

    return success;
}
