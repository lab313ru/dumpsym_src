/*
===========================================================
                Sony PlayStation 1 Source Code
===========================================================
                         FONT EXAMPLE
Displays text on the screen using the built in GPU routines
-----------------------------------------------------------

    Developer / Programmer..............: SCEI & PSXDEV.net
    Software Ddevelopment Kit...........: PSY-Q
	Last Updated........................: 04/MAY/2017

    Original code by SCEI | Edited by PSXDEV.net
    
	NOTE: This example uses double buffering.

  Copyright (C) 1994,1995 by Sony Computer Entertainment Inc.
   Sony Computer Entertainment Inc. Development Department.
                   All Rights Reserved.
				   
                    http://psxdev.net/

-----------------------------------------------------------*/

#include <stdlib.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH 1 // the ordertable length
#define PACKETMAX 18 // the maximum number of objects on the screen
#define SCREEN_WIDTH  320 // screen width
#define	SCREEN_HEIGHT 240 // screen height (240 NTSC, 256 PAL)

GsOT myOT[2]; // ordering table header
GsOT_TAG myOT_TAG[2][1<<OT_LENGTH]; // ordering table unit
PACKET GPUPacketArea[2][PACKETMAX]; // GPU packet data

u_long _ramsize   = 0x00200000; // force 2 megabytes of RAM
u_long _stacksize = 0x00004000; // force 16 kilobytes of stack

// --------
// INTEGERS
// --------
short CurrentBuffer = 0; // holds the current buffer number

// ----------
// PROTOTYPES
// ----------
void graphics(); // inits the GPU
void display(); // updates the GPU (IE: VRAM/SGRAM/framebuffer)

const DEBUG = 1; // debugging (1=on, 0=off)

// ----
// MAIN
// ----
int main() 
{
	graphics(); // setup the graphics (seen below)
	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
	
	if (DEBUG) // should debug be true (equal 1)...
	{
		// print to the TTY stream (only visible if you're using one)
		printf("\n\nHello World\n");
		printf("\nhttp://psxdev.net/");
	}

	while (1) // draw and display forever
	{
		FntPrint("             HELLO WORLD\n\n          HTTP://PSXDEV.NET/");
		display();
	}

	return 0; // this will never be reached because we're in a while loop above
}

void graphics()
{
	SetVideoMode(1); // PAL mode
	//SetVideoMode(0); // NTSC mode
	
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); // set the graphics mode resolutions (GsNONINTER for NTSC, and GsINTER for PAL)
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT); // tell the GPU to draw from the top left coordinates of the framebuffer
	
	// init the ordertables
	myOT[0].length = OT_LENGTH;
	myOT[1].length = OT_LENGTH;
	myOT[0].org = myOT_TAG[0];
	myOT[1].org = myOT_TAG[1];
	
	// clear the ordertables
	GsClearOt(0,0,&myOT[0]);
	GsClearOt(0,0,&myOT[1]);
}


void display()
{
	// refresh the font
	FntFlush(-1);
	
	// get the current buffer
	CurrentBuffer = GsGetActiveBuff();
	
	// setup the packet workbase
	GsSetWorkBase((PACKET*)GPUPacketArea[CurrentBuffer]);
	
	// clear the ordering table
	GsClearOt(0,0, &myOT[CurrentBuffer]);
	
	// wait for all drawing to finish
	DrawSync(0);
	
	// wait for v_blank interrupt
	VSync(0);
	
	// flip the double buffers
	GsSwapDispBuff();
	
	// clear the ordering table with a background color (R,G,B)
	GsSortClear(50,50,50, &myOT[CurrentBuffer]);
	
	// draw the ordering table
	GsDrawOt(&myOT[CurrentBuffer]);
}
