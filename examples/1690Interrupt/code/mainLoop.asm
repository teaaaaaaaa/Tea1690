.NAME Interrupt.bin

.SECTION controllerInput 0x0000
.SECTION spriteX 0x0001
.SECTION spriteY 0x0002
.SECTION spriteFrame 0x0003
.SECTION OAMDefault 0x0004

//initialize palette read write

LD #256, R2 				//store high 16 bits of address for palette write

LD QI #0, R1				//clear RAM with DMA transfer
LD #0xFFFF, R0
STRI R0, %R1
LD #L|RAMClearValue, R0
STRI R0, %R1
LD #H|RAMClearValue, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1

LD QI #1, R1				//set R1 back to 1

LD #L|BGData, R0				//DMA copy background tiles
STRI R0, %R1
LD #H|BGData + 2048, R0
STRI R0, %R1
LD #32768, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #2289, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy map data
LD QI #0, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #1921, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy palette data
LD QI #0, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1
LD QI #17, R0
STRI R0, %R1

LD QI #0x6A, R1 	//store low 16 bits of address for GPU activation IO write
LD #L|InterruptRoutine, R0
STRI R0, %R1
LD #H|InterruptRoutine + 32768, R0
STRI R0, %R1

LD QI #.L|OAMDefault, R0				//store OAM clear value in RAM
LD QI #0, R1
LD #0xFFFF, R2
STR R2, %R0

LD #256, R2				//Fill OAM with default value of 65535
LD QI #0, R1
LD QI #1, R0
STRI R0, %R1
LD #.L|OAMDefault, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #1024, R0
STRI R0, %R1
LD #2048, R0
STRI R0, %R1

LD QI #0, R1				//initialize OAM address
LD #1024, R2

                //write OAM data for background
LD #512, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1
LD #7455, R0
STRI R0, %R1
LD QI #0, R0
STRI R0, %R1

$Idle

LD QI #6, R2 	//store low 16 bits of address for GPU activation IO write
LD #256, R3 				//store high 16 bits of address for GPU activation IO write
STR #1, %R2

SYNC
JMPR $Idle

$InterruptRoutine
LD QI #3, R1				//initialize OAM address
LD #1024, R2

                //write OAM data for background
LD #960, R0
STRI R0, %R1
RETI

$BGData
include data\background.img.bin*
include data\background.map.bin*
include data\background.pal.bin*

$RAMClearValue
.INSERT #0