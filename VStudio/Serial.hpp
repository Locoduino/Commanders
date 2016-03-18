//-------------------------------------------------------------------
#ifndef __serial_H__
#define __serial_H__
//-------------------------------------------------------------------


class HardwareSerial
{
public:
	inline HardwareSerial() {}
	void begin(unsigned long baud) { }
	void begin(unsigned long, uint8_t) {}
	void end() {}
	virtual int available(void) { return 0; }
	virtual int peek(void) { return 0; }
	virtual int read(void) { return 0; }
	int availableForWrite(void) { return 0;	}
	void flush(void) {}
	operator bool() { return true; }

	void print(const char *line);
	void println(const char *line);
	void print(int value, int i = 0);
	void println(int value, int i = 0);
};

extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------