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
#define signalPanel "+---------------------+\r\n|    Signal Elevator  |\r\n|  [  up  ]  [ down ] |\r\n+---------------------+\r\n\r\n"
#define prompt "What would you like to do? (type the command in the square brackets and hit enter): "
#define signalUpPanel "+---------------------+\r\n|    Signal Elevator  |\r\n|       [  up  ]      |\r\n+---------------------+\r\n\r\n"
#define signalDownPanel "+---------------------+\r\n|    Signal Elevator  |\r\n|      [  down  ]     |\r\n+---------------------+\r\n\r\n"
#define elevatorPanel "+-------------------------+\r\n|       Elevator Panel    |\r\n+-------------------------+\r\n|  [b]  [1]  [2]  [3]  [4]|\r\n|                         |\r\n|   <             >       |\r\n| Door Open   Door Close  |\r\n+-------------------------+\r\n"


void CLI_Transmit(uint8_t* pData, uint16_t Size);
void CLI_Receive(uint8_t *pData);
