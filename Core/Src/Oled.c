#include "Oled.h"
#include "main.h"

SPI_HandleTypeDef* SpiHandle;
char LastMessage[MAX_MESSAGE_SIZE];
uint8_t SizeOfLastMessage;

void SmallDelay()
{
	for(uint64_t i = 0; i < 100; i++);
}

void OLED_BitBangWriteBit(int bit)
{
	HAL_GPIO_WritePin(BB_MOSI_GPIO_Port, BB_MOSI_Pin, bit);

	SmallDelay();
	HAL_GPIO_WritePin(BB_CLOCK_GPIO_Port, BB_CLOCK_Pin, 1);
	SmallDelay();
	HAL_GPIO_WritePin(BB_CLOCK_GPIO_Port, BB_CLOCK_Pin, 0);
}

void OLED_BitBangWriteByte(int byte)
{
	OLED_BitBangWriteBit((byte >> 7) & 1);
	OLED_BitBangWriteBit((byte >> 6) & 1);
	OLED_BitBangWriteBit((byte >> 5) & 1);
	OLED_BitBangWriteBit((byte >> 4) & 1);
	OLED_BitBangWriteBit((byte >> 3) & 1);
	OLED_BitBangWriteBit((byte >> 2) & 1);
	OLED_BitBangWriteBit((byte >> 1) & 1);
	OLED_BitBangWriteBit((byte >> 0) & 1);
}

void OLED_BitBangCmd(int cmd)
{
	HAL_GPIO_WritePin(BB_SS_GPIO_Port, BB_SS_Pin, 0);
	SmallDelay();

	OLED_BitBangWriteBit(0);
	OLED_BitBangWriteBit(0);
	OLED_BitBangWriteByte(cmd);
	SmallDelay();

	HAL_GPIO_WritePin(BB_SS_GPIO_Port, BB_SS_Pin, 1);
	SmallDelay();
}

void OLED_BitBangData(int data)
{
	HAL_GPIO_WritePin(BB_SS_GPIO_Port, BB_SS_Pin, 0);
	SmallDelay();

	OLED_BitBangWriteBit(1);
	OLED_BitBangWriteBit(0);
	OLED_BitBangWriteByte(data);
	SmallDelay();

	HAL_GPIO_WritePin(BB_SS_GPIO_Port, BB_SS_Pin, 1);
	SmallDelay();
}

void OLED_Init()
{
	for(uint8_t i = 0; i < sizeof(LastMessage); i++)
	{
		LastMessage[i] = 0;
	}

	HAL_Delay(1);
	OLED_BitBangCmd(0x38);
	OLED_BitBangCmd(0x08);
	OLED_BitBangCmd(0x01);

	HAL_Delay(2);
	OLED_BitBangCmd(0x06);
	OLED_BitBangCmd(0x02);
	OLED_BitBangCmd(0x0c);
}

void OLED_Display1(const char* message, uint8_t len)
{
	OLED_BitBangCmd(0x02);
	for(uint8_t i = 0; i < len && i < 0xF; i++)
	{
		OLED_BitBangData(message[i]);
	}
}

void OLED_AddMessage(const char* message, uint32_t len)
{
	SizeOfLastMessage = len;

	for(uint8_t i = 0; i < sizeof(LastMessage); i++)
	{
		LastMessage[i] = 0;
	}

	for(uint8_t i = 0; i < len && i < sizeof(LastMessage); i++)
	{
		LastMessage[i] = message[i];
	}
}

void OLED_DisplayLastMessage()
{
	OLED_Display1(LastMessage, 16);
}
