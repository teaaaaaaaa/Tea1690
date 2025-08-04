#pragma once
#include "memory.h"

#define mode (3)
#define XFlipFlag (1 << 2)
#define YFlipFlag (1 << 3)
#define dimensionFlag (1 << 4)
#define affineMode (3 << 5)

#define transparency 1
#define affineTilemapEnable 2
#define colorAverage (1 << 5)
#define affineDefaultFlag (1 << 6)
#define tileBankFlag (1 << 15)

#define tileXFlipFlag (1)
#define tileYFlipFlag (1 << 1)

unsigned int tileBaseOffset;
unsigned char tilePixelXOffset;
unsigned char tilePixelYOffset;
unsigned short tileSubX;
unsigned char tileX;
unsigned char tileY;
unsigned short baseTile;
unsigned char tileWidth;
unsigned char tileHeight;
unsigned short basePixelX;
unsigned short basePixelY;
unsigned short getTile;
unsigned short OAMAddress;
unsigned char getPixel;
unsigned char getPixel2;
unsigned short getPalette;
unsigned short getPalette2;
signed char red;
signed char green;
signed char blue;
signed char subPalette;
signed char subGlobal;
unsigned long paletteBDRegs;
unsigned char flags;
unsigned char flags2;
unsigned int VPUtick;
unsigned int VRAMAddress;
unsigned int totalCycleCount;
unsigned short pixelX;
unsigned short pixelY;
unsigned char tileFlags;
unsigned char perTileXOffset;
unsigned char perTileYOffset;
unsigned char bitOrFlag;
unsigned short affineWidth;
unsigned short affineHeight;
unsigned char tickCount = 4;
signed short horizontalScale;
signed short verticalScale;
signed int horizontalScaleCounter;
signed int verticalScaleCounter;
unsigned int widthCounter;
unsigned int heightCounter;
signed short horizontalShear;
signed short verticalShear;
signed int getHorizontalOffset;
signed int getVerticalOffset;
signed int horizontalOrigin;
signed int verticalOrigin;
signed int affineHOffset;
signed int affineVOffset;
unsigned int getHorizontalOffsetUnsigned;
unsigned int getVerticalOffsetUnsigned;
bool finishedRendering;
bool startDrawing;
signed char transparencyCompare;
unsigned char currentFrame;
unsigned char currentPaletteOffset;
unsigned char getScrollset;
bool isBufferSwapped;
signed short interruptY = -1;
unsigned char currentIntEntry = 0;
unsigned short GPUIOOffset = 0x70;
bool isIntDelayFetched;
unsigned char intDelayCounter;

unsigned short* paletteFrontBuffer = paletteBuffer1;
unsigned short* paletteBackBuffer = paletteBuffer2;

unsigned short paletteCopyIndex;

unsigned char paletteLookupTable[65535][3];

unsigned short intBufferPointers[8];

unsigned short TilemapANDValues[] =
{
	2047,
	4095,
	8191,
	16383
};

unsigned char TilemapShiftValues[] =
{
	11,
	12,
	13,
	14
};

void tick1()
{
	if (isBufferSwapped == false)
	{
		if (currentFrame & 1)
		{
			paletteFrontBuffer = paletteBuffer2;
			paletteBackBuffer = paletteBuffer1;
		}
		else
		{
			paletteFrontBuffer = paletteBuffer1;
			paletteBackBuffer = paletteBuffer2;
		}
	}
	paletteCopyIndex = 0;
	pixelX = 0;
	pixelY = 0;
	tileX = 0;
	tileY = 0;
	tileBaseOffset = 0;
	basePixelX = (OAM[OAMAddress] & 511);
	flags = (OAM[OAMAddress] >> 9);
	OAMAddress ++;
}

void tick2()
{
	basePixelY = (OAM[OAMAddress] & 511);
	flags2 = (OAM[OAMAddress] >> 9);
	affineHOffset = IO[0x39 + (((flags2 >> 2) & 7) * 6)] + (IO[0x3A + (((flags2 >> 2) & 7) * 6)] << 16);
	affineVOffset = IO[0x3B + (((flags2 >> 2) & 7) * 6)] + (IO[0x3C + (((flags2 >> 2) & 7) * 6)] << 16);
	horizontalOrigin = (IO[0x3D + (((flags2 >> 2) & 7) * 6)]);
	verticalOrigin = (IO[0x3E + (((flags2 >> 2) & 7) * 6)]);
	currentPaletteOffset = ((flags2 >> 2) & 7) << 4;
	OAMAddress ++;
}

void tick3()
{
	tileWidth = (OAM[OAMAddress] & 63) + 1;
	tileHeight = ((OAM[OAMAddress] >> 8) & 63) + 1;
	affineWidth = (OAM[OAMAddress] & 255) + 1;
	affineHeight = ((OAM[OAMAddress] >> 8)) + 1;
	totalCycleCount = (tileWidth << 6) * tileHeight;
	OAMAddress ++;
}

void tick4()
{
	baseTile = (OAM[OAMAddress]);
	OAMAddress += 5;
}

void tick5()
{
	OAMAddress -= 4;
	horizontalScale = (OAM[OAMAddress]);
	horizontalScaleCounter = 0;
	OAMAddress++;
}

void tick6()
{
	verticalScale = (OAM[OAMAddress]);
	OAMAddress++;
}

void tick7()
{
	horizontalShear = (OAM[OAMAddress]);
	OAMAddress++;
}

void tick8()
{
	verticalShear = (OAM[OAMAddress]);
	OAMAddress++;
}

inline void clampPalettes()
{
	if (red < 0) red = 0;
	if (red > 31) red = 31;
	if (green < 0) green = 0;
	if (green > 31) green = 31;
	if (blue < 0) blue = 0;
	if (blue > 31) blue = 31;
}

inline unsigned short colorAverageFunc(unsigned short input1, unsigned short input2)
{
	red = ((input1 & 31) + (input2 & 31)) >> 1;
	green = (((input1 >> 5) & 31) + ((input2 >> 5) & 31)) >> 1;
	blue = (((input1 >> 10) & 31) + ((input2 >> 10) & 31)) >> 1;
	return red + (green << 5) + (blue << 10);
}

inline void fetchPalette(unsigned char index)
{
	getPalette = palettes[index];
	paletteBDRegs = IO[0x6D] + (IO[0x6E] << 16) + (IO[0x6F] << 32);
	subPalette = ((paletteBDRegs >> ((paletteCopyIndex >> 4) * 5)) & 31);
	if (IO[0x6C] & (256 << (paletteCopyIndex >> 4)))
		subPalette = -subPalette;
	red = (getPalette & 31) + subPalette;
	green = ((getPalette >> 5) & 31) + subPalette;
	blue = ((getPalette >> 10) & 31) + subPalette;
	clampPalettes();
	getPalette = red | (green << 5) | (blue << 10);
}

inline unsigned short fetchVRAMNibble(unsigned short baseAddress, unsigned short offset)
{
	return VRAM[((((baseAddress + (offset >> 2)) >> ((offset & 3) << 2) & 15)) & 32767) + 32768];
}

inline void drawPixel()
{
	//tilePixelXOffset = ((tileBaseOffset & 7));
	//tilePixelYOffset = (((tileBaseOffset >> 3) & 7));
	if ((flags & XFlipFlag) == 0)
		pixelX = (basePixelX + ((tileBaseOffset & 7)) + (tileX << 3)) & 511;
	else
		pixelX = ((tileWidth << 3) - (basePixelX + ((tileBaseOffset & 7)) + (tileX << 3)) - 1) & 511;
	if ((flags & YFlipFlag) == 0)
		pixelY = (basePixelY + (((tileBaseOffset >> 3) & 7)) + (tileY << 3)) & 511;
	else
		pixelY = ((tileHeight << 3) - (basePixelY + (((tileBaseOffset >> 3) & 7)) + (tileY << 3)) - 1) & 511;
	getPixel = (((VRAM[VRAMAddress]) >> (((tileBaseOffset & 3) ^ bitOrFlag) << 2)) & 15);
	//getPixel = rand();
	if (getPixel > transparencyCompare && pixelX < 256 && pixelY < 240)
	{
		fetchPalette(getPixel + currentPaletteOffset);
		if ((currentFrame & 1) == 0)
		{
			if (flags2 & colorAverage)
				intermediateBuffer2[(pixelX + (pixelY << 8))] = colorAverageFunc(getPalette, intermediateBuffer2[(pixelX + (pixelY << 8))]);
			else
				intermediateBuffer2[(pixelX + (pixelY << 8))] = getPalette;
		}
		else
		{
			if (flags2 & colorAverage)
				intermediateBuffer[(pixelX + (pixelY << 8))] = colorAverageFunc(getPalette, intermediateBuffer[(pixelX + (pixelY << 8))]);
			else
				intermediateBuffer[(pixelX + (pixelY << 8))] = getPalette;
		}
	}
	tileBaseOffset++;
	tileSubX++;
	tileX = (tileSubX >> 6);
	if (tileX >= tileWidth)
	{
		tileSubX = 0;
		tileX = 0;
		tileY++;
	}
}

inline int fetchVRAMByte(unsigned int X, unsigned int Y)
{
	return ((VRAM[(((X >> 10) + ((Y >> 8) << 6)) & 32767) + 32768]) >> (((X >> 8) & 3) << 2)) & 15;
}

inline int fetchVRAMByteWide(unsigned int X, unsigned int Y)
{
	return ((VRAM[(((X >> 10) + ((Y >> 8) << 7)) & 32767) + 32768]) >> (((X >> 8) & 3) << 2)) & 15;
}

inline void calculateHV()
{
	if (flags2 & affineDefaultFlag)
	{
		horizontalOrigin = affineWidth >> 1;
		verticalOrigin = affineHeight >> 1;
		affineHOffset = 0;
		affineVOffset = 0;
	}
	getHorizontalOffsetUnsigned = affineHOffset + (horizontalShear * (heightCounter - verticalOrigin)) + horizontalScaleCounter - (horizontalScale * horizontalOrigin) + (horizontalOrigin << 8);
	getVerticalOffsetUnsigned = affineVOffset + (verticalScaleCounter + (verticalShear * (widthCounter - horizontalOrigin)) - (verticalScale * verticalOrigin)) + (verticalOrigin << 8);
}

inline void drawAffinePixel()
{
		pixelX = (basePixelX + widthCounter) & 511;
		pixelY = (basePixelY + heightCounter) & 511;
		calculateHV();
		if (flags2 & affineTilemapEnable)
		{
			getPixel = ((VRAM[((getTile << 4) + ((getHorizontalOffsetUnsigned >> 10) & 1) + (((getVerticalOffsetUnsigned >> 8) << 1) & 15) & 32767) + 32768]) >> (((getHorizontalOffsetUnsigned >> 8) & 3) << 2)) & 15;
		}
		else
		{
			if (flags & dimensionFlag)
			{
				getHorizontalOffsetUnsigned &= (65535 << 1);
				if (((flags & affineMode) >> 5) == 0 || ((flags & affineMode) >> 5) == 1)
				{
					getPixel = fetchVRAMByteWide(getHorizontalOffsetUnsigned + ((baseTile & 63) << 10), getVerticalOffsetUnsigned + ((baseTile >> 6) << 8));
				}
				if (((flags & affineMode) >> 5) == 2)
				{
					getPixel = fetchVRAMByteWide((getHorizontalOffsetUnsigned % ((affineWidth << 8) + 1)) + ((baseTile & 63) << 10), (getVerticalOffsetUnsigned % ((affineHeight << 8) + 1)) + ((baseTile >> 6) << 8));
				}
			}
			else
			{
				getHorizontalOffsetUnsigned &= 65535;
				if (((flags & affineMode) >> 5) == 0 || ((flags & affineMode) >> 5) == 1)
				{
					getPixel = fetchVRAMByte(getHorizontalOffsetUnsigned + ((baseTile & 63) << 10), getVerticalOffsetUnsigned + ((baseTile >> 6) << 8));
				}
				if (((flags & affineMode) >> 5) == 2)
				{
					getPixel = fetchVRAMByte((getHorizontalOffsetUnsigned % ((affineWidth << 8) + 1)) + ((baseTile & 63) << 10), (getVerticalOffsetUnsigned % ((affineHeight << 8) + 1)) + ((baseTile >> 6) << 8));
				}
			}
		}
	//getPixel = rand();
	if (((flags & affineMode) >> 5) == 1)
	{
		if ((getHorizontalOffsetUnsigned >> 8) >= affineWidth || (getVerticalOffsetUnsigned >> 8) >= affineHeight)
			getPixel = 0;
	}
	if (getPixel > transparencyCompare && pixelX < 256 && pixelY < 240)
	{
		fetchPalette(getPixel + currentPaletteOffset);
		if ((currentFrame & 1) == 0)
		{
			if (flags2 & colorAverage)
				intermediateBuffer2[(pixelX + (pixelY << 8))] = colorAverageFunc(getPalette,intermediateBuffer2[(pixelX + (pixelY << 8))]);
			else
				intermediateBuffer2[(pixelX + (pixelY << 8))] = getPalette;
		}
		else
		{
			if (flags2 & colorAverage)
				intermediateBuffer[(pixelX + (pixelY << 8))] = colorAverageFunc(getPalette, intermediateBuffer[(pixelX + (pixelY << 8))]);
			else
				intermediateBuffer[(pixelX + (pixelY << 8))] = getPalette;
		}
	}
	widthCounter++;
	horizontalScaleCounter += horizontalScale;
	if (widthCounter >= affineWidth)
	{
		widthCounter = 0;
		horizontalScaleCounter = 0;
		heightCounter++;
		verticalScaleCounter += verticalScale;
	}
}

void (*VPUtickFunctions[])() =
{
tick1,
tick2,
tick3,
tick4,
tick5,
tick6,
tick7,
tick8,
};

inline void VPULogic()
{
	if ((flags & mode) == 2)
		tickCount = 8;
	else
		tickCount = 4;
	if ((flags2 & transparency) == 0)
		transparencyCompare = -1;
	else
		transparencyCompare = 0;
		if (VPUtick < tickCount)
		{
			VPUtickFunctions[VPUtick]();
		}
		if (VPUtick >= 4)
		{
			if ((flags & mode) == 0)
			{
				if (VPUtick > 3 && tileY >= tileHeight)
				{
					VPUtick = 0;
					return;
				}
				if ((flags & dimensionFlag) == 0)
					VRAMAddress = (((baseTile << 4) + (tileBaseOffset >> 2)) & 16383) + 32768;
				else
					VRAMAddress = ((((baseTile << 4) + ((tileX << 4) + (tileY << 9))) + (tileBaseOffset >> 2)) & 16383) + 32768;
				if (baseTile & tileBankFlag)
					VRAMAddress += 16384;
				drawPixel();
			}
			if ((flags & mode) == 1)
			{
				if (VPUtick > 3)
				{
					if ((flags & dimensionFlag) == 0)
					{
							getTile = (VRAM[(((baseTile + (tileBaseOffset >> 6)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767]) & 1023;
							currentPaletteOffset = (((VRAM[(((baseTile + (tileBaseOffset >> 6)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 10) & 7) << 4);
							tileFlags = (VRAM[(((baseTile + (tileBaseOffset >> 6)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 13);
						if (baseTile & tileBankFlag)
							getTile += 1024;
					}
					else
					{
							getTile = (VRAM[(((((baseTile + tileX) & 63) + ((tileY + (baseTile >> 6)) << 6)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767]) & 1023;
							currentPaletteOffset = (((VRAM[(((((baseTile + tileX) & 63) + ((tileY + (baseTile >> 6)) << 6)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 10) & 7) << 4);
							tileFlags = (VRAM[(((((baseTile + tileX) & 63) + ((tileY + (baseTile >> 6)) << 6)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 13);
						if (baseTile & tileBankFlag)
							getTile += 1024;
					}
					bitOrFlag = (tileFlags & tileXFlipFlag);
					if (VPUtick > 4 && tileY >= tileHeight)
					{
						VPUtick = 0;
						return;
					}
					if ((tileFlags & tileXFlipFlag) == 0)
						perTileXOffset = (tileBaseOffset & 7);
					else
						perTileXOffset = ((7 - tileBaseOffset) & 7);
					if ((tileFlags & tileYFlipFlag) == 0)
						perTileYOffset = ((tileBaseOffset >> 3) & 7);
					else
						perTileYOffset = ((7 - (tileBaseOffset >> 3)) & 7);
					VRAMAddress = (((getTile << 4) + ((perTileXOffset >> 2) + (perTileYOffset << 1))) & 32767) + 32768;
					drawPixel();
				}
			}
		}
		if ((flags & mode) == 2)
		{
			if (VPUtick >= 8)
			{
				if (((basePixelY + heightCounter) & 511) < 240)
				{
					if (IO[6] & 2)
					{
						if (((basePixelY + heightCounter) & 511) != interruptY)
						{
							if (IO[GPUIOOffset] & 32768)
							{
								switch ((IO[GPUIOOffset] >> 11) & 3)
								{
								case 0:
									if (componentIOMap[(IO[GPUIOOffset]) & 127] == CVPU)
										IO[(IO[GPUIOOffset]) & 127] = intRAM[(interruptY & 255) + (currentIntEntry << 8)];
									break;
								case 1:
										OAM[(IO[GPUIOOffset]) & 2047] = intRAM[(interruptY & 255) + (currentIntEntry << 8)];
									break;
								case 2:
										palettes[(IO[GPUIOOffset]) & 255] = intRAM[(interruptY & 255) + (currentIntEntry << 8)];
									break;
								}
							}
							currentIntEntry++;
							GPUIOOffset += 2;
							if (intDelayCounter < 8)
							{
								intDelayCounter++;
								return;
							}
							else
							{
								currentIntEntry = 0;
								GPUIOOffset = 0x70;
								intDelayCounter = 0;
								OAMAddress -= 8;
								VPUtick = 0;
								interruptY = ((basePixelY + heightCounter) & 511);
								return;
							}
						}
					}
				}
				if (flags2 & affineTilemapEnable)
				{
					calculateHV();
					if (flags & dimensionFlag)
					{
							getTile = (VRAM[(((((baseTile + (getHorizontalOffsetUnsigned >> 11)) & 127) + ((baseTile >> 6) << 6) + ((getVerticalOffsetUnsigned >> 11) << 7)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767]) & 1023;
							currentPaletteOffset = (((VRAM[(((((baseTile + (getHorizontalOffsetUnsigned >> 11)) & 127) + ((baseTile >> 6) << 6) + ((getVerticalOffsetUnsigned >> 11) << 7)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 10) & 7) << 4);
							tileFlags = (VRAM[(((((baseTile + (getHorizontalOffsetUnsigned >> 11)) & 127) + ((baseTile >> 6) << 6) + ((getVerticalOffsetUnsigned >> 11) << 7)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 13);
					}
					else
					{
							getTile = (VRAM[(((((baseTile + (getHorizontalOffsetUnsigned >> 11)) & 63) + ((baseTile >> 6) << 6) + ((getVerticalOffsetUnsigned >> 11) << 7)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767]) & 1023;
							currentPaletteOffset = (((VRAM[(((((baseTile + (getHorizontalOffsetUnsigned >> 11)) & 63) + ((baseTile >> 6) << 6) + ((getVerticalOffsetUnsigned >> 11) << 7)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 10) & 7) << 4);
							tileFlags = (VRAM[(((((baseTile + (getHorizontalOffsetUnsigned >> 11)) & 63) + ((baseTile >> 6) << 6) + ((getVerticalOffsetUnsigned >> 11) << 7)) & TilemapANDValues[((flags & affineMode) >> 5) & 3]) + ((baseTile >> TilemapShiftValues[((flags & affineMode) >> 5) & 3]) << TilemapShiftValues[((flags & affineMode) >> 5) & 3])) & 32767] >> 13);
					}
					if (baseTile & tileBankFlag)
						getTile += 1024;
					drawAffinePixel();
				}
				else
				{
					drawAffinePixel();
				}
				if (heightCounter >= affineHeight)
				{
					VPUtick = 0;
					interruptY = 256;
					widthCounter = 0;
					heightCounter = 0;
					verticalScaleCounter = 0;
					return;
				}
			}
		}
		if ((flags & mode) == 3)
		{
			VPUtick = 0;
		}
	if (OAMAddress > 2048)
	{
		finishedRendering = true;
		isBufferSwapped = false;
		//std::cout << "current sprite: " << (OAMAddress >> 3) << std::endl;
		OAMAddress = 0;
		VPUtick = 0;
		interruptY = 256;
		return;
	}
	VPUtick++;
	//std::cout << "current tick: " << VPUtick << std::endl;
}