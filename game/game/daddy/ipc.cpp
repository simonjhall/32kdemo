#include "../common.h"
#include "../pad.h"

struct Arduino
{
	union {
		unsigned char raw[256];
		struct {
			unsigned char low[254];

			unsigned char cmd;	//254
			unsigned char data;	//255
		};
	};
};

struct Pad
{
	unsigned char bits;
};

static_assert(sizeof(Pad) == 1, "pad is not one byte");

static const unsigned char kStart = 1;
static const unsigned char kWriteString = 2;
static const unsigned char kWriteHex = 3;
static const unsigned char kWritePort = 4;
static const unsigned char kReadEntry = 5;
static const unsigned char kEnableVblankInterrupt = 6;
static const unsigned char kDisableVblankInterrupt = 7;
static const unsigned char kSetInterruptAddr = 8;
static const unsigned char kReadPad = 9;
static const unsigned char kHalt = 69;

static volatile Arduino *pArduino = (Arduino *)(0 * 1024);

static inline void Stop(void)
{
	pArduino->cmd = kHalt;
}

unsigned char read_pad(unsigned int id)
{
	pArduino->cmd = kReadPad;
	struct Pad pad;
	pad.bits = pArduino->data;

	return pad.bits;
}
