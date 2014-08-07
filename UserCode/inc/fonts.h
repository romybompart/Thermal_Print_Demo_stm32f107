#ifndef __FONTS_H
#define __FONTS_H
#include "stm32f10x_conf.h"
#include <string.h>
#include "header_hardware.h"

#define int16								uint16_t
#define int8								uint8_t

#define asciistart 					((uint8_t)'A')
#define netxcharacter				((uint8_t)23)
#define Fontrows						((uint8_t)24)
#define FontColums					((uint8_t)16)
#define Maxdotsperline			((uint16_t)384)
#define	nextcharactercolum	((uint8_t)Fontrows/8)
#define Maxcharacterperline	((uint16_t)Maxdotsperline/FontColums)


extern void ThermalPrint(char text[150]);
static int16 pixelsfont(int8 row, int8 letter);

#endif
				
