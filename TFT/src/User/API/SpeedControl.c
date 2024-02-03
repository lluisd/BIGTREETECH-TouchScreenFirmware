#include "SpeedControl.h"
#include "includes.h"

#define SPEED_REFRESH_TIME 500  // 1 second is 1000

const char * const speedCmd[SPEED_NUM] = {"M220", "M221"};

static uint16_t setPercent[SPEED_NUM] = {100, 100};
static uint16_t curPercent[SPEED_NUM] = {100, 100};
static uint8_t needSetPercent = 0;

static bool speedQueryWait = false;

void speedSetPercent(uint8_t tool, uint16_t per)
{
  uint16_t value = NOBEYOND(SPEED_MIN, per, SPEED_MAX);

  SET_BIT_VALUE(needSetPercent, tool, value != curPercent[tool]);
  setPercent[tool] = value;
}

uint16_t speedGetSetPercent(uint8_t tool)
{
  return setPercent[tool];
}

void speedSetCurPercent(uint8_t tool, uint16_t per)
{
  curPercent[tool] = per;
}

uint16_t speedGetCurPercent(uint8_t tool)
{
  return curPercent[tool];
}

void loopSpeed(void)
{
  static uint32_t nextUpdateTime = 0;

  if (OS_GetTimeMs() < nextUpdateTime)  // avoid rapid fire, clogging the queue
    return;

  nextUpdateTime = OS_GetTimeMs() + SPEED_REFRESH_TIME;

  for (uint8_t i = 0; i < SPEED_NUM; i++)
  {
    if (infoSettings.ext_count == 0 && i > 0)  // don't poll M221 if there are no extruders
      continue;

    if (GET_BIT(needSetPercent, i))
    {
      if (storeCmd("%s S%d D%d\n", speedCmd[i], setPercent[i], heatGetToolIndex()))
        SET_BIT_OFF(needSetPercent, i);
    }
  }
}

void speedQuerySetWait(bool wait)
{
  speedQueryWait = wait;
}

void speedQuery(void)
{ // following conditions ordered by importance
  if (!speedQueryWait && infoHost.tx_slots != 0 && infoHost.connected && infoMachineSettings.firmwareType != FW_REPRAPFW)
  {
    speedQueryWait = storeCmd("M220\n");

    if (infoSettings.ext_count > 0)
      speedQueryWait |= storeCmd("M221\n");  // speedQueryWait set to "true" if at least one command will be sent
  }
}
