#define IRQ 2
#define GREENLED 6
#define YELLOWLED 16

/*********************************************************************************************************/
/*                                                 Timing                                                */
/*********************************************************************************************************/
short               tries                = 0;
unsigned long       timerValue           = 0;
unsigned long       timer                = 0;

/*********************************************************************************************************/
/*                                                 Codes                                                 */
/*********************************************************************************************************/
boolean             connectionFail       = false;
boolean             dataRec              = false;
byte                receiveID            = B00000010;
byte                devID                = B00000000;

byte                command              = B00000000;
byte                receiveCommand       = B00000000;
byte                packetNum            = B00000000;
byte                maxNum               = B00000000;
boolean             buttonValue          = false;
char                results[20];

/*********************************************************************************************************/
/*                                             Measured data                                             */
/*********************************************************************************************************/
float               dhtTemp              = 0;
float               dhtHum               = 0;
float               val                  = 0;
float               bmpTemp              = 0;
float               bmpPreshPa           = 0;
float               bmpSeaLev            = 0;
float               GPSAlt               = 0;

/*********************************************************************************************************/
/*                                             MySQL server                                              */
/*********************************************************************************************************/
byte                mac_addr[]            = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress           server_addr(81,0,104,144);
char                server_name[]         = "www.moszerviz.com";
char                user[]                = "paranyosi";
char                password[]            = "paranyosi2015";


//const char        TEST_SELECT_QUERY[]   = "SELECT * FROM world.city LIMIT 10";
//const char        QUERY_POP[]           = "SELECT population FROM world.city WHERE name = 'New York'";
//const char        INSERT_TEXT[]         = "INSERT INTO test_arduino.hello VALUES ('Hello, MySQL!', NULL)";
//const char        INSERT_DATA[]         = "INSERT INTO arduino.sensor_data VALUES (%d, %s, %d)";
const char          UPDATE[]              = "UPDATE ynfhocom_s.sensors SET value=%d WHERE id=%d";  
const char          SELECT_RELAY[]        = "SELECT * FROM ynfhocom_s.relays;";
//const char        HELLO_SQL[]           = "SELECT * from test_arduino.hello";
//const char        HELLO_DATA[]          = "SELECT * from test_arduino.temps";



