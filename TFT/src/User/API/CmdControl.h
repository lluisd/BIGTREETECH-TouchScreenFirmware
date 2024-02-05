#ifndef _CMD_CONTROL_
#define _CMD_CONTROL_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

uint32_t getCmdLineNumberOk(void);                 // get line number of last command properly processed by mainboard
uint32_t getCmdLineNumber(void);                   // get line number of last command sent by TFT
void setCmdLineNumber(const uint32_t lineNumber);  // set command line number to the provided value
void addCmdLineNumberAndChecksum(char * str);      // add line number and checksum to the provided command

const char * stripCmdHead(const char * str);       // strip out any leading " ", "/" or ":" character that might be in the string

void stripCmdChecksum(char * str);                 // strip out any trailing checksum that might be in the string
uint8_t getCmdChecksum(const char * str);          // return checksum for the provided string
bool validateCmdChecksum(const char * str);        // validate checksum for the provided string

const char * parseM118(char * str, bool * hasE, bool * hasA);  // parse M118 gcode

#ifdef __cplusplus
}
#endif

#endif
