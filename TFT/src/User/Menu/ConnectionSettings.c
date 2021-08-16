#include "ConnectionSettings.h"
#include "includes.h"

const MENUITEMS connectionSettingsItems = {
  // title
  LABEL_CONNECTION_SETTINGS,
  // icon                          label
  {
    {ICON_BAUD_RATE,               LABEL_BAUDRATE},
    {ICON_BAUD_RATE,               LABEL_SERIAL_PORTS},
    {ICON_STOP,                    LABEL_EMERGENCYSTOP},
    {ICON_SHUT_DOWN,               LABEL_SHUT_DOWN},
    {ICON_DISCONNECT,              LABEL_DISCONNECT},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACK,                    LABEL_BACK},
  }
};

uint8_t * baudratePtr = NULL;
uint8_t baudrateMinIndex = 0;

// Set uart pins to input, free uart
void menuDisconnect(void)
{
  GUI_Clear(infoSettings.bg_color);
  GUI_DispStringInRect(20, 0, LCD_WIDTH - 20, LCD_HEIGHT, textSelect(LABEL_DISCONNECT_INFO));
  GUI_DispStringInRect(20, LCD_HEIGHT - (BYTE_HEIGHT * 2), LCD_WIDTH - 20, LCD_HEIGHT, textSelect(LABEL_TOUCH_TO_EXIT));

  Serial_ReSourceDeInit();
  while (!isPress())
  {
    #ifdef LCD_LED_PWM_CHANNEL
      LCD_CheckDimming();
    #endif
  }
  while (isPress())
  {
    #ifdef LCD_LED_PWM_CHANNEL
      LCD_CheckDimming();
    #endif
  }
  Serial_ReSourceInit();

  infoMenu.cur--;
}

void menuBaudrate(void)
{
  LABEL title = {LABEL_BAUDRATE};
  uint8_t size = BAUDRATE_COUNT - baudrateMinIndex;
  LISTITEM totalItems[size];
  KEY_VALUES curIndex = KEY_IDLE;
  uint8_t curItem = 0;
  uint16_t curPage;
  SETTINGS now = infoSettings;

  // fill baudrate items
  for (uint8_t i = 0; i < size; i++)
  {
    if (*baudratePtr == i + baudrateMinIndex)
    {
      totalItems[i].icon = CHARICON_CHECKED;
      curItem = i;
    }
    else
    {
      totalItems[i].icon = CHARICON_UNCHECKED;
    }
    totalItems[i].itemType = LIST_LABEL;
    totalItems[i].titlelabel.address = (uint8_t *) baudrateNames[i + baudrateMinIndex];
  }

  curPage = curItem / LISTITEM_PER_PAGE;

  listViewCreate(title, totalItems, size, &curPage, true, NULL, NULL);

  while (infoMenu.menu[infoMenu.cur] == menuBaudrate)
  {
    curIndex = listViewGetSelectedIndex();

    if (curIndex < size && curIndex != curItem)
    {  // has changed
      totalItems[curItem].icon = CHARICON_UNCHECKED;
      listViewRefreshItem(curItem);  // refresh unchecked status
      curItem = curIndex;
      totalItems[curItem].icon = CHARICON_CHECKED;
      listViewRefreshItem(curItem);  // refresh checked status

      *baudratePtr = curItem + baudrateMinIndex;
      Serial_ReSourceDeInit();  // Serial_Init() will malloc a dynamic memory, so Serial_DeInit() first to free, then malloc again.
      Serial_ReSourceInit();
      reminderMessage(LABEL_UNCONNECTED, STATUS_UNCONNECT);
    }

    loopProcess();
  }

  if (memcmp(&now, &infoSettings, sizeof(SETTINGS)))
  {
    storePara();
  }
}

void menuSerialPort(void)
{
  LABEL title = {LABEL_SERIAL_PORTS};
  LISTITEM totalItems[PORT_COUNT];
  KEY_VALUES curIndex = KEY_IDLE;

  for (uint8_t i = 0; i < PORT_COUNT; i++)
  {
    totalItems[i].icon = CHARICON_EDIT;
    totalItems[i].itemType = LIST_CUSTOMVALUE;
    totalItems[i].titlelabel.address = (uint8_t *) serialPortId[serialPort[i].port];
    totalItems[i].valueLabel.index = LABEL_DYNAMIC;  // must be LABEL_DYNAMIC or LABEL_CUSTOM_VALUE in order to use dynamic text
    setDynamicTextValue(i, (char *) baudrateNames[infoSettings.serial_port[i]]);
  }

  listViewCreate(title, totalItems, PORT_COUNT, NULL, true, NULL, NULL);

  while (infoMenu.menu[infoMenu.cur] == menuSerialPort)
  {
    curIndex = listViewGetSelectedIndex();
    switch (curIndex)
    {
      #ifdef SERIAL_PORT_2
        case 0:
          baudratePtr = &infoSettings.serial_port[0];
          baudrateMinIndex = 0;
          infoMenu.menu[++infoMenu.cur] = menuBaudrate;
          break;
      #endif

      #ifdef SERIAL_PORT_3
        case 1:
          baudratePtr = &infoSettings.serial_port[1];
          baudrateMinIndex = 0;
          infoMenu.menu[++infoMenu.cur] = menuBaudrate;
          break;
      #endif

      #ifdef SERIAL_PORT_4
        case 2:
          baudratePtr = &infoSettings.serial_port[2];
          baudrateMinIndex = 0;
          infoMenu.menu[++infoMenu.cur] = menuBaudrate;
          break;
      #endif

      default:
        break;
    }

    loopProcess();
  }
}

void menuConnectionSettings(void)
{
  KEY_VALUES curIndex = KEY_IDLE;

  menuDrawPage(&connectionSettingsItems);

  while (infoMenu.menu[infoMenu.cur] == menuConnectionSettings)
  {
    curIndex = menuKeyGetValue();
    switch (curIndex)
    {
      case KEY_ICON_0:
        baudratePtr = &infoSettings.baudrate;
        baudrateMinIndex = 1;
        infoMenu.menu[++infoMenu.cur] = menuBaudrate;
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuSerialPort;
        break;

      case KEY_ICON_2:
        // Emergency Stop : Used for emergency stopping, a reset is required to return to operational mode.
        // it may need to wait for a space to open up in the command queue.
        // Enable EMERGENCY_PARSER in Marlin Firmware for an instantaneous M112 command.
        Serial_Puts(SERIAL_PORT, "M112\n");
        break;

      case KEY_ICON_3:
        storeCmd("M81\n");
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuDisconnect;
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        break;

      default :
        break;
    }

    loopProcess();
  }
}
