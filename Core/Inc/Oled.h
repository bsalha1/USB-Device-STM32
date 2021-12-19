#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

#define MAX_MESSAGE_SIZE 0x10

void OLED_Init();

void OLED_Display1(const char* message, uint8_t len);

void OLED_AddMessage(const char* message, uint32_t len);

void OLED_DisplayLastMessage();

#endif /* INC_OLED_H_ */
