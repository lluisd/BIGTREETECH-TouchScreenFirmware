#include <Mainboard_CmdControl.h>
#include <includes.h>

// line number of last command properly processed by mainboard and line number of last command sent by TFT respectively.
// Requires command line number and checksun feature enabled in TFT
static uint32_t cmd_line_number_ok = 0xFFFFFFFF;  // no processed line number
static uint32_t cmd_line_number = 0;

uint32_t getCmdLineNumberOk(void)
{
  return cmd_line_number_ok;
}

uint32_t getCmdLineNumber(void)
{
  return cmd_line_number;
}

void setCmdLineNumber(const uint32_t lineNumber)
{
  cmd_line_number_ok = cmd_line_number = lineNumber;
}

void addCmdLineNumberAndChecksum(char * str)
{
  CMD cmd;  // temporary working buffer

  strcpy(cmd, str);       // e.g. "N1 G28*46\n" -> "G28*46\n"
  stripCmdChecksum(cmd);  // e.g. "G28*46\n" -> "G28"

  if (strlen(cmd) + 16 > CMD_MAX_SIZE)  // we consider extra bytes for line number, checksum, "\n" and "\0"
  {
    addNotification(DIALOG_TYPE_ERROR, "Command too long", "Checksum not applied", true);

    return;
  }

  cmd_line_number++;  // next command line number to be used

  sprintf(str, "N%lu %s", cmd_line_number, cmd);             // e.g. "G28" -> "N2 G28"
  sprintf(&str[strlen(str)], "*%u\n", getCmdChecksum(str));  // e.g. "N2 G28" -> "N2 G28*56\n"
}

const char * stripCmdHead(const char * str)
{
  // example: ":    /test/cap2.gcode\n" -> "test/cap2.gcode\n"

  while (*str == ' ' || *str == '/' || *str == ':')
  {
    str++;
  }

  return str;
}

void stripCmdChecksum(char * str)
{
  // examples:
  //
  // "/test/cap2.gcode  *36\n\0" -> "/test/cap2.gcode"
  // "/test/cap2.gcode  \n\0" -> "/test/cap2.gcode"

  char * strPtr = strrchr(str, '*');  // e.g. "/test/cap2.gcode  *36\n\0" -> "*36\n\0"

  if (strPtr == NULL)
    strPtr = str + strlen(str);       // e.g. "/test/cap2.gcode  \n\0" -> "\0"

  while (strPtr != str)
  {
    // e.g. "*36\n\0" -> " *36\n\0"
    // e.g. "\0" -> "\n\0"
    //
    --strPtr;

    if (*strPtr != ' ' && *strPtr != '\t' && *strPtr != '\n' && *strPtr != '\r')
    {
      strPtr++;  // next char has to be set to "\0"
      break;
    }
  }

  // e.g. "  *36\n\0" -> "\0 *36\n\0"
  // e.g. "  \n\0" -> "\0 \n\0"
  //
  *strPtr = '\0';
}

uint8_t getCmdChecksum(const char * str)
{
  uint8_t checksum = 0;

  while (*str != '\0')
  {
    checksum ^= *(str++);
  }

  return checksum;
}

bool validateCmdChecksum(const char * str)
{
  char * strPtr = strrchr(str, '*');  // e.g. "N1 G28*18\n\0" -> "*18\n\0"

  if (strPtr == NULL)
    return false;

  uint8_t checksum = 0;
  uint8_t value = strtol(&strPtr[1], NULL, 10);

  while (strPtr != str)
  {
    checksum ^= *(--strPtr);
  }

  return (checksum == value ? true : false);
}

const char * parseM118(char * str, bool * hasE, bool * hasA)
{
  stripCmdChecksum(str);
  str = (char *) stripCmdHead(str);

  *hasE = false;
  *hasA = false;

  for (uint8_t i = 3; i--;)
  {
    // A1, E1 and Pn are always parsed out
    if (!(((str[0] == 'A' || str[0] == 'E') && str[1] == '1') || (str[0] == 'P' && NUMERIC(str[1]))))
      break;

    switch (str[0])
    {
      case 'A':
        *hasA = true;
        break;

      case 'E':
        *hasE = true;
        break;
    }

    str += 2;

    while (*str == ' ')
    {
      ++str;
    }
  }

  return str;
}
