#include "SerialConnection.h"
#include "includes.h"

#define SERIAL_PORT_QUEUE_SIZE NOBEYOND(512, RAM_SIZE * 64, 4096)

#ifdef SERIAL_PORT_2
  #define SERIAL_PORT_2_QUEUE_SIZE 512
  #define SERIAL_PORT_3_QUEUE_SIZE 512
  #define SERIAL_PORT_4_QUEUE_SIZE 512

  SERIAL_PORT_INFO serialPort[PORT_COUNT] = {
    {SERIAL_PORT_2, SERIAL_PORT_2_QUEUE_SIZE, false},
    #ifdef SERIAL_PORT_3
      {SERIAL_PORT_3, SERIAL_PORT_3_QUEUE_SIZE, false},
    #endif
    #ifdef SERIAL_PORT_4
      {SERIAL_PORT_4, SERIAL_PORT_4_QUEUE_SIZE, false}
    #endif
  };
#endif

char serialPortId[_UART_CNT][2] =                  {0};
const uint32_t baudrateValues[BAUDRATE_COUNT] =    { 0,     2400,   9600,   19200,   38400,   57600,   115200,   250000,   500000,   1000000};
const char * const baudrateNames[BAUDRATE_COUNT] = {"OFF", "2400", "9600", "19200", "38400", "57600", "115200", "250000", "500000", "1000000"};
bool serialHasBeenInitialized = false;

void Serial_ReSourceInit(void)
{
  if (serialHasBeenInitialized)
    return;

  memset(&infoHost, 0, sizeof(infoHost));
  reminderSetUnConnected();  // reset connect status

  Serial_Config(SERIAL_PORT, SERIAL_PORT_QUEUE_SIZE, baudrateValues[infoSettings.baudrate]);

  #ifdef SERIAL_PORT_2
    SERIAL_PORT_INFO portInfo;

    for (uint8_t i = 0; i < PORT_COUNT; i++)
    {
      portInfo = serialPort[i];

      // the supplementary serial ports should be enabled according to config.ini.
      // Disable the serial port when it is not in use and floating to avoid to receive
      // and process wrong data due to possible electromagnetic interference (EMI).
      if (infoSettings.serial_port[i] >= 0)  // if serial port is enabled
        Serial_Config(portInfo.port, portInfo.cacheSize, baudrateValues[infoSettings.serial_port[i]]);

      sprintf(serialPortId[portInfo.port], "%d", i + 2);  // "2" for SERIAL_PORT_2 etc...
    }
  #endif

  serialHasBeenInitialized = true;
}

void Serial_ReSourceDeInit(void)
{
  if (!serialHasBeenInitialized)
    return;

  Serial_DeConfig(SERIAL_PORT);

  #ifdef SERIAL_PORT_2
    for (uint8_t i = 0; i < PORT_COUNT; i++)
    {
//      if (infoSettings.serial_port[i] >= 0)  // if serial port is enabled
        Serial_DeConfig(serialPort[i].port);
    }
  #endif

  serialHasBeenInitialized = false;
}
