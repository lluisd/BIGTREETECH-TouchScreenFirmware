#ifndef _SPEED_CONTROL_H_
#define _SPEED_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define SPEED_NUM 2
#define SPEED_MIN 10
#define SPEED_MAX 999

void speedSetPercent(const uint8_t tool, const uint16_t per);
uint16_t speedGetSetPercent(const uint8_t tool);
void speedSetCurPercent(const uint8_t tool, const uint16_t per);
uint16_t speedGetCurPercent(const uint8_t tool);
void loopSpeed(void);
void speedQuerySetUpdateWaiting(const bool isWaiting);
void speedQuery(void);

#ifdef __cplusplus
}
#endif

#endif
