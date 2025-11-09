#include "log.h"
#include "usart.h"

char loggerBuf[LOGBUFFER_SIZE];

void _log_process_msg(uint8_t* data, int cnt) {
	USART1_send(data, cnt);
}
