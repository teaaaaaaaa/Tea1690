#pragma once

//RWA is short for read write address. Only use this for writing to or reading from memory.
//do not read from multiple memory locations in the same component at the same time, as that's not how hardware works IRL.
//additionally, a cycle takes a cycle, there's no changing that. Only one read/write per cycle.

#define CPUToAPULatency 5
#define unmapped 0
#define writeOnly 1
#define readOnly 2
#define readAndWrite 3

#define INVA 0
#define CCPU 1
#define CAPU 2
#define CVPU 3

unsigned char isBusy[8] =
{
	0,0,0,0,0,0,0,0,
};

unsigned char isComponentBusy[3] =
{
	0,0,0,
};

unsigned char latencyAmountInCycles[] =
{
	1,0,6,6,6,8,6,0,
	5,0,3,3,3,8,3,0
};

unsigned char IOlatencyAmountInCycles[] =
{
	0,1,5,6,
	0,5,1,3,
};

unsigned char componentIOMap[] =
{
	CCPU,CCPU,CCPU,CCPU,CCPU,CCPU,CVPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CCPU,CCPU,INVA,INVA,INVA,
	INVA,INVA,INVA,INVA,INVA,INVA,INVA,INVA,
	CCPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,INVA,CCPU,CCPU,CVPU,CVPU,CVPU,CVPU,
	CVPU,INVA,CVPU,INVA,CVPU,INVA,CVPU,INVA,
	CVPU,INVA,CVPU,INVA,CVPU,INVA,CVPU,INVA,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,CAPU,
	INVA,INVA,INVA,INVA,INVA,INVA,INVA,INVA,
	INVA,INVA,INVA,INVA,INVA,INVA,INVA,INVA,
	INVA,INVA,INVA,INVA,INVA,INVA,INVA,INVA,
	INVA,INVA,INVA,INVA,INVA,INVA,INVA,INVA,
};

bool isAlwaysAccessible[] =
{
	false,false,false,false,false,false,true,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,true,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
	false,false,false,false,false,false,false,false,
};

//sizes for each component (ROM size is set upon load)
unsigned int memSize[] =
{
65535,255,255,65535,2047,16777215,2047,0
};

unsigned int memSizeNAND[] =
{
0xFFFFFFFF,0xFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0
};

bool memAccessFlags[8];

//general purpose RAM
unsigned short RAM[65536];

//IO registers
unsigned short IO[256];

unsigned char shadowIO[256];

//15bpp palettes
unsigned short palettes[256];
unsigned short paletteBuffer1[256];
unsigned short paletteBuffer2[256];

//stores 8bpp character data and tilemaps (write only)
unsigned short VRAM[65536];

//read only memory (up to 32 megabytes in size)
unsigned short intRAM[2048];

//read only memory (up to 32 megabytes in size)
unsigned short ROM[16777216];

//object attribute memory
unsigned short OAM[2048];

//first stage of rendered output
unsigned short intermediateBuffer[256 * 240];

unsigned short intermediateBuffer2[256 * 240];

//final stage of rendered output
unsigned char secondaryFrameBuffer[256 * 240][3];

unsigned char VWFrameBuffer[512 * 256][3];

//pointers to arrays
unsigned short* components[] =
{
RAM,
IO,
palettes,
VRAM,
OAM,
ROM,
intRAM,
ROM,
};