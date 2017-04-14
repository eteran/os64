
#include "kernel.h"
#include "multiboot.h"
#include "boot.h"

namespace {

const __u64 VideoBuffer  = KERNEL_VMA + 0x000b8000;
const __u16  VideoWidth  = 80;
const __u16  VideoHeight = 25;
const __u16  VideoSize   = VideoWidth * VideoHeight;

const auto video = reinterpret_cast<__u16 *>(VideoBuffer);


//+-----------+--------------+--------------+---------------+
//| Blink (1) | BG Color (3) | FG Color (4) | Character (8) |
//+-----------+--------------+--------------+---------------+
enum TerminalAttributes : __u16 {
	BlinkCursor             = 0x8000,
	
	BackgroundBlack         = 0x0000,
	BackgroundBlue          = 0x1000,
	BackgroundGreen         = 0x2000,
	BackgroundCyan          = 0x3000,
	BackgroundRed           = 0x4000,
	BackgroundMagenta       = 0x5000,
	BackgroundBrown         = 0x6000,
	BackgroundGray          = 0x7000,
	
	ForegroundBlack         = 0x0000,
	ForegroundBlue          = 0x0100,
	ForegroundGreen         = 0x0200,
	ForegroundCyan          = 0x0300,
	ForegroundRed           = 0x0400,
	ForegroundMagenta       = 0x0500,
	ForegroundBrown         = 0x0600,
	ForegroundGray          = 0x0700,
	
	ForegroundDarkGray      = 0x0800,
	ForegroundBrightBlue	= 0x0900,
	ForegroundBrightGreen	= 0x0a00,
	ForegroundBrightCyan	= 0x0b00,
	ForegroundBrightRed 	= 0x0c00,
	ForegroundBrightMagenta = 0x0d00,
	ForegroundYellow        = 0x0e00,
	ForegroundWhite	        = 0x0f00,
};


void clear_screen(__u16 attribute) {
	for(int i = 0; i < VideoSize; ++i) {
		video[i] = attribute;	
	}
}

}

//------------------------------------------------------------------------------
// Name: kmain
//------------------------------------------------------------------------------
int kmain(unsigned long magic, unsigned long mbi) {

	(void)mbi;

	__u16 attribute = (BackgroundBlue | ForegroundWhite | BlinkCursor);
	
	
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		attribute = (BackgroundRed | ForegroundWhite);
	}
	
	clear_screen(attribute);
	
	// manually put a basic message into the video buffer
	video[0]  = 'H' | attribute;
	video[1]  = 'e' | attribute;
	video[2]  = 'l' | attribute;
	video[3]  = 'l' | attribute;
	video[4]  = 'o' | attribute;
	video[5]  = ',' | attribute;
	video[6]  = ' ' | attribute;
	video[7]  = 'W' | attribute;
	video[8]  = 'o' | attribute;
	video[9]  = 'r' | attribute;
	video[10] = 'l' | attribute;
	video[11] = 'd' | attribute;
	
	while(1) {
		// loop forever...
	}

	return 0;
}
