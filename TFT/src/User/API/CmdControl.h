#ifndef _CMD_CONTROL_
#define _CMD_CONTROL_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

uint32_t getCmdLineNumberMismatch(void);
uint32_t getCmdLineNumber(void);
void setCmdLineNumberAndMismatch(uint32_t lineNumber);
void addCmdLineNumberAndChecksum(char * str);

const char * stripCmdHead(const char * str);  // strip out any leading " ", "/" or ":" character that might be in the string

void stripCmdChecksum(char * str);     // strip out any trailing checksum that might be in the string
uint8_t getCmdChecksum(char * str);    // return checksum for the provided string
bool validateCmdChecksum(char * str);  // validate checksum for the provided string

const char * parseM118(char * str, bool * hasE, bool * hasA);

#ifdef __cplusplus
}
#endif

#endif
