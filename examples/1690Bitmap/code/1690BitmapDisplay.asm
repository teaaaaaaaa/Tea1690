.NAME Bitmap.bin
.SECTION XScroll 0x0000
.SECTION YScroll 0x0001

//program start:
//initialize palette read write
LD QI #1, R1				//initialize R1-R2 with IO address
LD #256, R2 				//store high 16 bits of address for palette write

LD #L|PaletteDat, R0				//DMA copy palette data to CRAM
STRI R0, %R1
LD #H|PaletteDat + 2048, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1
LD QI #16, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy the bitmap data to VRAM
LD #32768, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #30720, R0
STRI R0, %R1

LD QI #0, R1	 		    //write to OAM to display the bitmap on-screen	
LD #1024, R2
LD #1024, R0				
STRI R0, %R1
LD QI #0, R0				
STRI R0, %R1
LD #65535, R0				
STRI R0, %R1
LD QI #0, R0				
STRI R0, %R1
LD #256, R0				
STRI R0, %R1
LD #256, R0				
STRI R0, %R1
LD QI #0, R0				
STRI R0, %R1
LD QI #0, R0				
STRI R0, %R1

$Idle
LD QI #6, R2 	//store low 16 bits of address for GPU activation IO write
LD #256, R3 				//store high 16 bits of address for GPU activation IO write
STR #1, %R2
LD #L|Idle, R0	//load address for source tilemap
LD #H|Idle, R1
SYNC
JMPR $Idle

.INSERT #0X656D
.INSERT #0X7373
.INSERT #0X6761
.INSERT #0X0065
.INSERT $Idle
.INSERT $L|Idle

$PaletteDat
include data\strawberry16.pal.bin*
$imgDat
include data\strawberry16.img.bin*