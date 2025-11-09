#ifndef INC_LOG_H_
#define INC_LOG_H_

#define LOGLEVEL_OFF        (99)
#define LOGLEVEL_TRACE      (1)
#define LOGLEVEL_DEBUG      (2)
#define LOGLEVEL_WARN       (3)
#define LOGLEVEL_ERROR      (4)

#define USELOGLEVEL LOGLEVEL_TRACE

#include <stdio.h>
#include "stm32f4xx_hal.h"

#define LOGBUFFER_SIZE 1000
extern char loggerBuf[LOGBUFFER_SIZE];
void _log_process_msg(uint8_t* data, int cnt);

#define LOG_TRACE(FORMAT, ...)   LOG_AT_LOG_LEVEL(LOGLEVEL_TRACE, "TRACE", FORMAT, ##__VA_ARGS__)
#define LOG_DEBUG(FORMAT, ...)   LOG_AT_LOG_LEVEL(LOGLEVEL_DEBUG, "DEBUG", FORMAT, ##__VA_ARGS__)
#define LOG_WARNING(FORMAT, ...) LOG_AT_LOG_LEVEL(LOGLEVEL_WARN,  "WARN", FORMAT, ##__VA_ARGS__)
#define LOG_ERROR(FORMAT, ...)   LOG_AT_LOG_LEVEL(LOGLEVEL_ERROR, "ERROR", FORMAT, ##__VA_ARGS__)


#define LOG_AT_LOG_LEVEL(LOGLEVEL, TYPE, FORMAT, ...) {                             \
    if (LOGLEVEL >= USELOGLEVEL) {                                                  \
        int cnt = snprintf(loggerBuf, LOGBUFFER_SIZE, "%s:%u:%s: at time %lu: " FORMAT "\n", __FILE__, __LINE__, __func__, HAL_GetTick(), ##__VA_ARGS__); 	\
        _log_process_msg((uint8_t*)loggerBuf, cnt);                       \
    }                                                                               \
}

#endif /* INC_LOG_H_ */
