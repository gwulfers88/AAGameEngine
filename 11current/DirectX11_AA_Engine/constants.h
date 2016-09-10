// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 3 constants.h v1.0

#ifndef _CONSTANTS_H            // prevent multiple definitions if this 
#define _CONSTANTS_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }

#define SAFE_ON_LOST_DEVICE(ptr) { if(ptr) { ptr->onLostDevice(); } }

#define SAFE_ON_RESET_DEVICE(ptr) { if(ptr) { ptr->onResetDevice(); } }

#define TRANSCOLOR SETCOLOR_ARGB( 0, 255, 0, 255 ) // transparency color (magenta)

// These are my data type definitions (George)
typedef char		i8;		// 1 - byte
typedef short		i16;	// 2 - byte
typedef int			i32;	// 4 - byte
typedef long long	i64;	// 8 - byte

typedef unsigned char		u8;		// 1 - byte
typedef unsigned short		u16;	// 2 - byte
typedef unsigned int		u32;	// 4 - byte
typedef unsigned long long	u64;	// 8 - byte

typedef i32			b32;			// 4 - byte
typedef float		r32;			// 4 - byte
typedef double		r64;			// 8 - byte

#define MAX_PATH_SIZE	256
#define ArrayCount(a) (sizeof(a) / sizeof(a[0]))

enum GraphicsLibrary
{
	GL_DX11 = (1 << 0),
	GL_GL = (1 << 1),
};

struct ConfigFile
{
	u8 name[4];			// Used to make sure that the file being read is a valid file!
	i32 windowWidth;	// Application Resolution
	i32 windowHeight;
	u32 graphicsLib;
};

extern ConfigFile *config;	// We want this to be persistent through out or program so we can access the new width and height of the application

//-----------------------------------------------
//                  Constants	
//-----------------------------------------------

// window
// (WEI) GIVE SOME OF THESE A BETTER NAME!!! GAME_WIDTH? WINDOW_WIDTH or SCREEN_WIDTH would be better!! Also make them #defines because it will not be bound to a single type of data!!
const char CLASS_NAME[] = "AAEngine";
const char GAME_TITLE[] = "SpaceWarI";
static bool FULLSCREEN = false;              // windowed or fullscreen ( This MUST NOT be constant! )
const UINT  GAME_WIDTH = 1024;              // width of game in pixels
const UINT  GAME_HEIGHT = 512;              // height of game in pixels
//================================================================================
//game proporties
//================================================================================
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY = VK_ESCAPE;         // escape key
const UCHAR WARRIOR_LEFT_KEY = VK_LEFT;
const UCHAR WARRIOR_UP_KEY = VK_UP;
const UCHAR WARRIOR_DOWN_KEY = VK_DOWN;
const UCHAR WARRIOR_RIGHT_KEY = VK_RIGHT;

//graphic images
//const char NEBULA_IMAGE[] = "pictures\\back_ground.jpg";
const char BACK_GROUND_IMAGE[] = "pictures\\grass_background.jpg";
//const char PLANET_IMAGE[] = "pictures\\planet.png";
//const char SHIP_IMAGE[] = "pictures\\ship.png";
const char TEXTURE_IMAGE[] = "pictures\\textures.png";
const char WARRIOR_IMAGE[] = "pictures\\15_tank_set.png";
const char PLANET_IMAGE[] = "pictures\\planet2.png";

//weapon types
enum WEAPON { TORPEDO, SHIP, PLANET };

const RECT	BULLET_BOX = { -8, -8, 8, 8 };

#endif
