#ifndef _MARLIN_MODE_H_
#define _MARLIN_MODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Configuration.h"

// User-defined colors for Marlin mode from Configuration.h
#ifndef MARLIN_BACKGROUND_COLOR
  #define MARLIN_BACKGROUND_COLOR LCD_BLACK
#endif

#ifndef MARLIN_FONT_COLOR
  #define MARLIN_FONT_COLOR LCD_GREEN
#endif

void menuMarlinMode(void);

#ifdef __cplusplus
}
#endif

#endif
