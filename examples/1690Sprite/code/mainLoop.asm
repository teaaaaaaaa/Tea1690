.NAME Sprite.bin

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
LD #273, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy map data
LD QI #0, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #961, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy palette data
LD QI #0, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1
LD QI #17, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy sprite graphics
LD #49152, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #2161, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy sprite tilemap
LD #8192, R0
STRI R0, %R1
LD #768, R0
STRI R0, %R1
LD #3073, R0
STRI R0, %R1

LD QI #3, R1				//DMA copy sprite palette
LD QI #16, R0
STRI R0, %R1
LD #512, R0
STRI R0, %R1
LD QI #17, R0
STRI R0, %R1

LD QI #.L|OAMDefault, R0				//store OAM clear value in RAM
LD QI #0, R1
LD #0xFFFF, R2
STR R2, %R0

$Idle

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

LD QI #8, R1

LD QI #0, R4
LD QI #.L|spriteX, R3
LDI %R3, R5
LDI %R3, R6
LDI %R3, R7

AND #511, R5, R5
OR #8704, R5, R5

AND #511, R6, R6
OR #2560, R6, R6

AND #3, R7, R4
SHR #4, R4, R4
SHL #2, R7, R7
SHR #10, R7, R7
ADD R7, R4, R7
ADD #40960, R7, R7

				//write OAM data for sprite
STRI R5, %R1
STRI R6, %R1
LD #3855, R0
STRI R0, %R1
STRI R7, %R1

LD QI #6, R2 	//store low 16 bits of address for GPU activation IO write
LD #256, R3 				//store high 16 bits of address for GPU activation IO write
STR #1, %R2



LD QI #0x38, R0         //copy controller input to RAM
LD #256, R1
LD QI #.L|controllerInput, R2
LD QI #0, R3
STR %R0, %R2

LD %R2, R4              //check for left input (decrement horizontal velocity if left pressed)
LD QI #.L|spriteX, R2
AND #1, R4, R5
JMPR EQR #0, R5, $EndLeftInput
LD %R2, R6
SUB #8, R6, %R2
$EndLeftInput

AND #2, R4, R5              //check for right input (increment horizontal velocity if right pressed)
JMPR EQR #0, R5, $EndRightInput
LD %R2, R6
ADD #8, R6, %R2
$EndRightInput

LD QI #.L|spriteY, R2

AND #4, R4, R5              //check for up input (zoom in if up pressed)
JMPR EQR #0, R5, $EndUpInput
LD %R2, R6
SUB #8, R6, %R2
$EndUpInput

AND #8, R4, R5              //check for down input (zoom out if up pressed)
JMPR EQR #0, R5, $EndDownInput
LD %R2, R6
ADD #8, R6, %R2
$EndDownInput

LD QI #.L|spriteFrame, R2

LD %R2, R6

AND #16, R4, R5              //check for up input (zoom in if up pressed)
JMPR EQR #0, R5, $EndAInput
JMPR EQR #0, R6, $EndAInput
DEC %R2
$EndAInput

AND #32, R4, R5              //check for down input (zoom out if up pressed)
JMPR EQR #0, R5, $EndBInput
JMPR LTR #8, R6, $EndBInput
INC %R2
$EndBInput

SYNC
JMPR $Idle

$BGData
include data\background.img.bin*
include data\background.map.bin*
include data\background.pal.bin*
include data\flyingCreatureGFX.img.bin*
include data\flyingCreatureGFX.map.bin*
include data\flyingCreatureGFX.pal.bin*

$RAMClearValue
.INSERT #0