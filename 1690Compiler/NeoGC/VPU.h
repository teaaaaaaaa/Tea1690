#pragma once

#define spriteTilemapFlag (1)
#define XFlipFlag (1 << 1)
#define YFlipFlag (1 << 2)
#define dimensionFlag (1 << 3)
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
unsigned char flags;
unsigned int VPUtick;
unsigned int VRAMAddress;
unsigned int totalCycleCount;
unsigned short pixelX;
unsigned short pixelY;
unsigned char tileFlags;
unsigned char perTileXOffset;
unsigned char perTileYOffset;
unsigned char bitOrFlag;
bool finishedRendering;

unsigned char paletteLookupTable[65535][3];

void tick1()
{
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
	OAMAddress ++;
}

void tick3()
{
	tileWidth = (OAM[OAMAddress] & 63) + 1;
	tileHeight = ((OAM[OAMAddress] >> 8) & 63) + 1;
	totalCycleCount = (tileWidth << 6) * tileHeight;
	OAMAddress ++;
}

void tick4()
{
	baseTile = (OAM[OAMAddress]);
	OAMAddress ++;
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
	getPixel = ((VRAM[VRAMAddress]) >> (((tileBaseOffset & 1) ^ bitOrFlag) << 3));
	//getPixel = rand();
	if (getPixel > 0 && pixelX < 256 && pixelY < 240)
	{
		getPalette = palettes[getPixel];
		//getPalette = (tileBaseOffset >> 2);
		secondaryFrameBuffer[(pixelX + (pixelY << 8))][0] = paletteLookupTable[getPalette][0];
		secondaryFrameBuffer[(pixelX + (pixelY << 8))][1] = paletteLookupTable[getPalette][1];
		secondaryFrameBuffer[(pixelX + (pixelY << 8))][2] = paletteLookupTable[getPalette][2];
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

void (*VPUtickFunctions[])() =
{
tick1,
tick2,
tick3,
tick4,
};

inline void VPULogic()
{
	if (VPUtick < 4)
	{
		VPUtickFunctions[VPUtick]();
	}
	if (VPUtick > 3)
	{
		if ((flags & spriteTilemapFlag) == 0)
		{
				if (VPUtick > 3 && tileY >= tileHeight)
				{
					VPUtick = 0;
					return;
				}
				if ((flags & dimensionFlag) == 0)
					VRAMAddress = ((baseTile << 5) + (tileBaseOffset >> 1)) & 65535;
				else
					VRAMAddress = ((baseTile << 5) + ((tileX << 5) + (tileY << 11))) & 65535;
				drawPixel();
		}
		else
		{
			if (VPUtick == 4)
			{
				if ((flags & dimensionFlag) == 0)
				{
					getTile = (VRAM[(baseTile + (tileBaseOffset >> 6)) & 65535]) & 2047;
					tileFlags = (VRAM[(baseTile + (tileBaseOffset >> 6)) & 65535] >> 12);
				}
				else
				{
					getTile = (VRAM[(((baseTile + tileX) & 63) + ((tileY + (baseTile >> 6)) << 6)) & 65535]) & 2047;
					tileFlags = (VRAM[(baseTile + tileX + (tileY << 6)) & 65535] >> 12);
				}
				bitOrFlag = (tileFlags & tileXFlipFlag);
				VPUtick++;
				return;
			}
			if (VPUtick > 4)
			{
				if (VPUtick > 68 && tileY < tileHeight)
				{
					VPUtick = 4;
					return;
				}
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
				VRAMAddress = ((getTile << 5) + ((perTileXOffset >> 1) + (perTileYOffset << 2))) & 65535;
				drawPixel();
			}
		}
	}
	if (OAMAddress > 1024)
	{
		finishedRendering = true;
		std::cout << "current sprite: " << (OAMAddress >> 2) << std::endl;
		OAMAddress = 0;
		VPUtick = 0;
		return;
	}
	VPUtick++;
	//std::cout << "current tick: " << VPUtick << std::endl;
}