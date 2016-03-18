#define ARDUINO2_MAIN

#include <string>
#include <time.h>

#include "arduino.h"
#include "Serial.hpp"
#include "arduino2.hpp"

//////////////////////////////////////////
// Arduino

void pinMode(int, int)
{}

void digitalWrite(int, int)
{}

int digitalRead(int)
{
	return 0;
}

void analogWrite(int, int)
{}

int analogRead(int)
{
	return 0;
}

unsigned long millis()
{
	time_t t;
	time(&t);
	return (unsigned long)t;
}

unsigned long micros()
{
	time_t t;
	time(&t);
	return (unsigned long)t;
}

void delay(int)
{}

void interrupts()
{}

void noInterrupts()
{}

void attachInterrupt(uint8_t, void(*)(void), int mode)
{}

void detachInterrupt(uint8_t)
{}

//////////////////////////////////////////
// Memory

int freeMemory()
{
	return 20000;
}

//////////////////////////////////////////
// Serial

HardwareSerial Serial;
HardwareSerial Serial1;
HardwareSerial Serial2;
HardwareSerial Serial3;

std::string memo;

void HardwareSerial::print(const char *line)
{
	memo.append(line);
}

void HardwareSerial::println(const char *line)
{
	memo.append(line);
	printf(memo.c_str());
	printf("\n");
	memo.clear();
}

void HardwareSerial::print(int value, int i)
{
	memo.append(std::to_string(value));
}

void HardwareSerial::println(int value, int i)
{
	memo.append(std::to_string(value));
	printf(memo.c_str());
	printf("\n");
	memo.clear();
}

//////////////////////////////////////////
// Arduino 2

int GPIO_to_Arduino_pin(GPIO_pin_t inPin)
{
	int i;
	for (i = 0; i < GPIO_PINS_NUMBER; i++)
	{
		if (inPin == gpio_pins_progmem[i])
			return i;
	}

	return -1;
}

GPIO_pin_t Arduino_to_GPIO_pin(int inPin)
{
	return gpio_pins_progmem[inPin];
}

int map(int, int, int, int, int)
{
	return 0;
}

byte pgm_read_byte(const byte *str)
{
	return (byte)*str;
}
