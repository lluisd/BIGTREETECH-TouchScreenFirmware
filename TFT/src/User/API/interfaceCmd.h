#ifndef _INTERFACE_CMD_H_
#define _INTERFACE_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define CMD_SIZE 100

typedef const char GCODE_CMD[CMD_SIZE];

bool isFullCmdQueue(void);      // condition callback for loopProcessToCondition()
bool isNotEmptyCmdQueue(void);  // condition callback for loopProcessToCondition()
bool isEnqueued(const char *cmd);

bool storeCmd(const char * format, ...);
void mustStoreCmd(const char * format, ...);
void mustStoreScript(const char * format, ...);
bool storeCmdFromUART(uint8_t portIndex, GCODE_CMD gcode);
void mustStoreCacheCmd(const char * format, ...);
bool moveCacheToCmd(void);
void clearCmdQueue(void);
void sendQueueCmd(void);

#ifdef __cplusplus
}
#endif

#endif
