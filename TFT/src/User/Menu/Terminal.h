#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
  TERMINAL_GCODE,
  TERMINAL_ACK,
} TERMINAL_SRC;

void menuTerminal(void);
void terminalCache(char *stream, uint16_t streamLen, uint8_t portIndex, TERMINAL_SRC src);

#ifdef __cplusplus
}
#endif

#endif
