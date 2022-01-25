#include "LED_Colors.h"
#include "includes.h"

#ifdef LED_COLOR_PIN

const uint32_t led_colors[LED_COLOR_COUNT] = {
  GRB_OFF,
  GRB_WHITE,
  GRB_RED,
  GRB_ORANGE,
  GRB_YELLOW,
  GRB_GREEN,
  GRB_BLUE,
  GRB_INDIGO,
  GRB_VIOLET,
};

const LABEL led_color_names[LED_COLOR_COUNT] = {
  // label
  LABEL_OFF,
  LABEL_WHITE,
  LABEL_RED,
  LABEL_ORANGE,
  LABEL_YELLOW,
  LABEL_GREEN,
  LABEL_BLUE,
  LABEL_INDIGO,
  LABEL_VIOLET,
};

#endif  // LED_COLOR_PIN

const LED_VECT ledRed =   {0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF};
const LED_VECT ledGreen = {0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF};
const LED_VECT ledBlue =  {0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF};
const LED_VECT ledWhite = {0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF};
const LED_VECT ledOff =   {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

LED_VECT ledColor = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};

void LED_SendColor(const LED_VECT * led)
{
  if (infoMachineSettings.firmwareType != FW_REPRAPFW)
    storeCmd("M150 R%u U%u B%u W%u P%u I%u\n", (*led)[0], (*led)[1], (*led)[2], (*led)[3], (*led)[4], (*led)[5]);
  else
    storeCmd("M150 X2 R%u U%u B%u P%u\n", (*led)[0], (*led)[1], (*led)[2], (*led)[4]);
}

uint16_t LED_GetRGBColor(LED_VECT * led)
{
  uint8_t r = (*led)[0] >> 3;
  uint8_t g = (*led)[1] >> 2;
  uint8_t b = (*led)[2] >> 3;
  return ((r & 0x001F) << 11) | ((g & 0x003F) << 5) | ((b & 0x001F));  // RGB color in RGB 565 16 bit format
}

void LED_SetRGBColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t p, uint8_t i)
{
  ledColor[0] = r;
  ledColor[1] = g;
  ledColor[2] = b;
  ledColor[3] = w;
  ledColor[4] = p;
  ledColor[5] = i;
}

void LED_GetColor(LED_VECT * led)
{
  for (int i = 0; i < LED_VECT_SIZE; i++)
    (*led)[i] = ledColor[i];
}

void LED_SetColor(const LED_VECT * led, bool skipNeopixel)
{
  int size = skipNeopixel ? LED_VECT_SIZE - 3 : LED_VECT_SIZE;

  for (int i = 0; i < size; i++)
    ledColor[i] = (*led)[i];
}

void LED_ChangeColor(const LED_VECT * ledColor, bool skipPrinterLed)
{
  if (!skipPrinterLed)
    LED_SendColor(ledColor);

  #ifdef LED_COLOR_PIN
    uint32_t knobLedColor = 0;  // GRB format

    knobLedColor |= (uint32_t)(ledColor[1]) << 16;  // g
    knobLedColor |= (uint32_t)(ledColor[0]) << 8;   // r
    knobLedColor |= (uint32_t)(ledColor[2]);        // b

    KNOB_LED_SET_COLOR(knobLedColor, infoSettings.neopixel_pixels);
  #endif
}
