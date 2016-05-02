//
// DCC_Decoder.h - Arduino library for NMRA DCC Decoding.
// Written by Kevin Snow, MynaBay.com, November, 2011. 
// Questions: dcc@mynabay.com
// Released into the public domain.
//

#ifndef __DCC_DECODER_H__
#define __DCC_DECODER_H__

#include "../../DIO2/VStudio/arduino.h"

///////////////////////////////////////////////////////////////////////////////////////

#define DCC_DECODER_VERSION			  "1.01"

#define kDCC_STOP_SPEED     0xFE
#define kDCC_ESTOP_SPEED    0xFF

// Multifunction Decoders
#define kCV_PrimaryAddress            1
#define kCV_Vstart                    2
#define kCV_AccelerationRate          3
#define kCV_Deceleration Rate         4
#define kCV_ManufacturerVersionNo     7 
#define kCV_ManufacturedID            8
#define kCV_ExtendedAddress1          17
#define kCV_ExtendedAddress2          18
#define kCV_ConfigurationData1        29

// Accessory Decoders
#define kCV_AddressLSB                1
#define kCV_AddressMSB                9


// DCC_Decoder results/errors
#define kDCC_OK                       0
#define kDCC_OK_UNHANDLED             1
#define kDCC_OK_BOOT                  2
#define kDCC_OK_IDLE                  3
#define kDCC_OK_RESET                 4
#define kDCC_OK_RAW                   5
#define kDCC_OK_BASELINE              6 
#define kDCC_OK_BASIC_ACCESSORY       7 
#define kDCC_OK_EXTENDED_ACCESSORY    8
#define kDCC_OK_MAX                   99

#define kDCC_ERR_DETECTION_FAILED     100
#define kDCC_ERR_BASELINE_ADDR        101             
#define kDCC_ERR_BASELINE_INSTR       102         // Baseline packet instruction isn't 0x01DCSSSS
#define kDCC_ERR_MISSED_BITS          103
#define kDCC_ERR_NOT_0_OR_1           104
#define kDCC_ERR_INVALID_LENGTH       105
#define kDCC_ERR_MISSING_END_BIT      106

// Min and max valid packet lengths
#define kPACKET_LEN_MIN               3
#define kPACKET_LEN_MAX               6

// CV 1..256 are supported
#define kCV_MAX                       257

typedef void(*BasicAccDecoderPacket)(int address, boolean activate, byte data);
typedef void(*InterruptMonitor)();

class DCC_Decoder
{
public:
	DCC_Decoder() {}

	void beginDecoder(byte mfgID, byte mfgVers, byte interrupt) {}    // Used for Decoder
	void beginMonitor(byte interrupt) {}                              // Used when building a monitor

	void SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket func, boolean allPackets) {}
	void SetInterruptMonitor(InterruptMonitor func) {}

	void loop() {}

	static int GetRealAddress(int inAddress, int inData) { return -1; }
	static bool IsEnabled(int inData) { return false; }

	static DCC_Decoder DCCInstance;
};

#define DCC		DCC_Decoder::DCCInstance

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

#endif