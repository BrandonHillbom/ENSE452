#include "stdint.h"
#include "usart.h"
#include "string.h"

//cursor ascii commands
#define ANSI_MOVE_CURSOR_TOP "\x1b[1;1H"  
#define ANSI_MOVE_CURSOR_MIDDLE "\x1b[6;1H" 
#define ANSI_SAVE_CURSOR "\x1b[s"
#define ANSI_RESTORE_CURSOR "\x1b[u"

//screen ascii commands
#define ANSI_SCROLL_REGION "\x1b[5;15r"
#define ANSI_POSITION_CURSOR "\x1b[5;0H"
#define ANSI_CLEAR_SCREEN "\x1b[2J"
#define ANSI_CLEAR_LINE "\x1b[K"

void CLI_Transmit(uint8_t* pData, uint16_t Size);
void CLI_Receive(uint8_t *pData);
