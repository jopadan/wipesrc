#include "standard.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include "malloc.h"
#include "tim.h"
#include "object.h"
#include "file.h"
#include "skeleton.h"
#include "track.h"
#include "ships.h"
#include "weapon.h"
#include "control.h"
#include "wtl.h"
#include "combat.h"
#include "menus.h"
#include "options.h"
#include "hud.h"
#include "droid.h"
#include "camera.h"	   
#include "fileops.h"
#include "hiscores.h"
#include "card.h"
#include "sound.h"
#include "sparks2.h"
#include "global.h"
#include "vramsize.h"				 	// PCwipeout
#include "c:\wc10\h\direct.h"

#define _SIZE_T_DEFINED_				// PCwipeout
#include "c:\wc10\h\stdio.h"			// PCwipeout
#include "c:\wc10\h\stdlib.h"		// PCwipeout
#include "..\andyy\pallette.h"
#include "..\chrise\utypes.h"
#include "..\chrise\sfx.h"

#include "..\markk\multiwo.h"
#include "..\markk\merror.h"
#define	FN_DEBUG
//#include "..\markk\debug.h"

#include "dos.h"

typedef char BOOL;
extern short trackNum,screenres,memorySize; //SJR
extern float upres;

#define	PALETTE				1
#define	PICTURE				2
#define	DEBUG_FLAG			4
#define	DONT_CHECK_SIZE	8
#define	ALLOCATE_MEM		16

#pragma aux Pic2Buf parm [edx]

typedef struct {
	int	width;
	int	height;
	int	vram_xpos;
	int	vram_ypos;
	char	*bitmap;
	char	*palette;
} bm;

#define MyLoad16Bit	1	// PCwipeout

#define LightSourced 1	// Bad primitives if this is off (keep it on, removing dead code anyway);
 
char	andybodge=1;
char	andybodge2=1;
extern short endtrack, starttrack;
extern int menuTexturesIn;
short globalvol;

TimData	*menuScreen ;

// temporary test stuff
#include	"markk\libetc.h"	// PCwipeout


#define FULL		0
#define HIGH		3
#define	MEDIUM 		2
#define LOW 		1
#define SMALL		0

#define	HIGH_SENS	8
#define	MEDIUM_SENS	16
#define	LOW_SENS	32

// MARK'S EXTERNS

extern long CommNetSocket=0;
extern long CommNetPlayers=2;
extern long	CommSerialDevice=1;
extern long	CommSerialBaudRate=1;
//extern long global_mark_error;


//WIN 95
extern char					*backscreen;

// OTHER FUCKERS
extern float halfxf;
extern float halfyf;
extern char  pixdouble;

char	ScreenSize=FULL;
char	Resolution=MEDIUM;
extern 	short ft;
extern 	char MipMap;
extern  char PersCorr;
extern  char PersCorrLvl;

char my_ScreenSize;
char my_Resolution;
char	my_Sky;
char 	my_PC;//SJR
long	my_DrawDistance;
long	my_TextureTrack;
long	my_SkyTrackFlags;

char	mouse_sensitivity=MEDIUM_SENS;
char cur_mouse_setting=MEDIUM_SENS;

char	controller = _KEYBOARD;
char	orig_controller;
char	changecontroller = 1;
// end of temporary stuff
  
extern	char *VRam;
extern	char *CurBuffer;

extern TexChar CharSet16[] ;
extern HiScores scoreTable[] ;
extern long fr,fw;		/* file descriptor */
extern unsigned long ev0 ;
int inmenutest=0;

extern int short shipShapeOrder[];
int short pilotShapeOrder [8] = {0, 4, 6, 7, 2, 5, 1, 3} ;
int short teamShapeOrder [4] = {2, 3, 1, 0} ;			// jed
ChampionShip	winTable [NO_OF_SHIPS] ;
int	modelHdg = 0 ;
int	zoomInOut = 0 ;
int	downUp = 0 ;
char	twist = 0 ;
char	negtwist = 0 ;
Skeleton		*root	;
unsigned short	Keycons[10];
unsigned short	NewOriginalKeyCodes[16]={0};
unsigned short	OriginalKeyCodes[16] =
{
		_Z_KEY,	   			/* FrontLeftBottom	*/	// left airbreak
		_X_KEY,	  			/* FrontRightBottom	*/	// right airbreak
		_F1_KEY,		   	/* FrontRightTop	*/	// Draw distance
		_ESC_KEY,		   	/* FrontLeftTop		*/
		_TAB_KEY,		  	/* Triangle			*/ 	// change view
		_LEFT_ALT_KEY,	   	/* Circle			*/	// shoot weapon
		_LEFT_CTRL_KEY,   	/* Cross			*/ 	// accelerate
		_F2_KEY,			/* Square			*/	// Screen size
		_ENTER_KEY,		   	/* Select			*/	// editor
		_F3_KEY,			/* Unused			*/	// Resolution
		_F4_KEY,			/* Unused			*/	// Sky
		_P_KEY,		   		/* Start			*/ 	// pause
		_UP_KEY,		   	/* Up				*/	// up
		_RIGHT_KEY,		   	/* Right			*/ 	// right
		_DOWN_KEY,		   	/* Down				*/ 	// down
		_LEFT_KEY	   		/* Left			 	*/ 	// left
};


//POLY_FT4		buttons[18][2] ;
SPRT		buttons[18][2] ;		// DLS
Texture		*buttonTex[18] ;
DR_MODE		drmode[18][2];				// DLS
DR_MODE		drmode2[18][2];				// DLS

TimData *AJSLoad16BitTexture(char *filename, char location);
TimData *JJSLoad16BitTexture(char *filename, char location);
BOOL JJSNewTexture(char *filename);
extern char *GetCdTrackName(int no);
void MostRecentSaveFile();	// returns the name of the last file to be modified in "c:\wipeout"
char CheckKey(unsigned short code);


char	*ScanCodes[89] =
{
	"        ",		// 0	
	"ESC     ",		// 1	
	"1       ",		// 2	
	"2       ",		// 3	
	"3       ",		// 4	
	"4       ",		// 5	
	"5       ",		// 6	
	"6       ",		// 7	
	"7       ",		// 8	
	"8       ",		// 9	
	"9       ",		// 10	
	"0       ",		// 11	
	"MINUS   ",		// 12
	"EQUALS  ",		// 13	
	"BACKSPCE",		// 14	
	"TAB     ",		// 15
	"Q       ",		// 16
	"W       ",		// 17
	"E       ",		// 18
	"R       ",		// 19
	"T       ",		// 20
	"Y       ",		// 21
	"U       ",		// 22
	"I       ",		// 23
	"O       ",		// 24
	"P       ",		// 25
	"L BRCKET",		// 26
	"R BRCKET",		// 27
	"ENTER   ",		// 28
	"LFT CTRL",		// 29
	"A       ",		// 30
	"S       ",		// 31
	"D       ",		// 32
	"F       ",		// 33
	"G       ",		// 34
	"H       ",		// 35
	"J       ",		// 36
	"K       ",		// 37
	"L       ",		// 38
	"SEMICOLN",		// 39
	"QUOTE   ",		// 40
	"GRAVE   ",		// 41
	"L SHIFT ",		// 42
	"BACKSLSH",		// 43
	"Z       ",		// 44
	"X       ",		// 45
	"C       ",		// 46
	"V       ",		// 47
	"B       ",		// 48
	"N       ",		// 49
	"M       ",		// 50
	"COMMA   ",		// 51
	"PERIOD  ",		// 52
	"SLASH   ",		// 53
	"R SHIFT ",		// 54
	"ASTERISK",		// 55
	"LEFT ALT",		// 56
	"SPACE   ",		// 57
	"CAPSLOCK",		// 58
	"F1      ",		// 59
	"F2      ",		// 60
	"F3      ",		// 61
	"F4      ",		// 62
	"F5      ",		// 63
	"F6      ",		// 64
	"F7      ",		// 65
	"F8      ",		// 66
	"F9      ",		// 67
	"F10     ",		// 68
	"NUM LOCK",		// 69
	"SCR LOCK",		// 70
	"HOME    ",		// 71
	"UP      ",		// 72
	"PAGE UP ",		// 73
	"XXXXXXXX",		// 74
	"LEFT    ",		// 75
	"KEYPAD 5",		// 76
	"RIGHT   ",		// 77
	"XXXXXXXX",		// 78
	"END     ",		// 79
	"DOWN    ",		// 80
	"PAGE DN ",		// 81
	"INSERT  ",		// 82
	"DELETE  ",		// 83
	"XXXXXXXX",		// 84
	"XXXXXXXX",		// 85
	"XXXXXXXX",		// 86
	"F11     ",		// 87
	"F12     "		// 88
};

char	ScanToAscii[110] =
{
	0,
	1,				
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0,
	1,				
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0,
	1,				
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0,
	1,				
	1,
	32,				// SPACE
	1,
	1,
	1,
	1,
	1,
	1,
	0,
	1,				
	1,
	1,
	1,
	1,
	1,
	1,
	48,				// 0
	49,				// 1
	50,				// 2
	51,				// 3
	52,				// 4
	53,				// 5
	54,				// 6
	55,				// 7
	56,				// 8
	57,				// 9
	1,
	1,
	0,
	1,				
	1,
	1,
	1,
	65,				// A
	66,				// B
	67,				// C
	68,				// D
	69,				// E
	70,				// F
	71,				// G
	72,				// H
	73,				// I
	74,				// J
	75,				// K
	76,				// L
	77,				// M
	78,				// N
	79,				// O
	80,				// P
	81,				// Q
	82,				// R
	83,				// S
	84,				// T
	85,				// U
	86,				// V
	87,				// W
	88,				// X
	89,				// Y
	90,				// Z
	1,				
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0,
	1,				
	1,
	1,
	1,
	1,
	1,
	43				// +
};



#define	NONE	0
#define	LEFT	1
#define	RIGHT	2
#define	UP		3
#define	DOWN	4
#define	BRAKEL	5
#define	BRAKER	6
#define	ACCEL	7
#define	FIRE	8
#define	VIEW_KEY	9
#define	PAUSE_KEY	10

#define	MAXSTRING	10

#define	TOPLEFT		0
#define	BOTTOMRIGHT	1
#define	CENTRE		2

#define	JOY_BODGE	8
//#define SAVEDIR			"wipeout\\"
#define	FILE_EXTENSION	".wsf"

extern char *SaveDirBase;


short	JoyMinX = 0, JoyMaxX = 255;
short	JoyMinY = 0, JoyMaxY = 255;
short	JoyCentreX = 128, JoyCentreY = 128;
short	left_scale = 127, right_scale = 127;
float	left_scale_factor = 1.0, right_scale_factor = 1.0;
short	JoyCentreFudge = 16;


char	JoystickCalibrationMode = TOPLEFT;

char	DelayCounter = 0;
char	KeyToChange = NONE;
char	davebodge = 1;
char	davebodge2 = 0;
int	andyTimeout=0;

short	TextEntry = 0;
short	TextPos = 0;
char	LoadFilename[MAXSTRING];
char	SaveFilename[MAXSTRING];
char	DaveSave[250];

void DrawControllerMenu (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay);
void DrawControllerEdit (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay);
void EnterLoadGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData);
void KeyConfig0(void);
void KeyConfig(void);
void DrawGraphicDetail (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay);


void Debugit(char *t)
{
	FILE *fp;

	fp=fopen("LOGFILE","a");
	fprintf(fp,"%s\n",t);
	fclose(fp);
}


void reloadScr(void)
{
#if USE_ATI
	menuScreen = JJSLoad16BitTexture	("wipeout\\textures\\wipeout1.tim", VRAM) ;
#else
	menuScreen = AJSLoad16BitTexture	("wipeout\\textures\\wipeout1.pcx", VRAM) ;
#endif	
}

void MenuControl (ConfigData *gameData, TimData *titleScreen, int *mode, int timeout)
{

   BlkFill*       clear[ 2 ];
	Menus				menuSystem[MAX_MENUS] ;
	POLY_FT4			menuPrim[2] ;
	POLY_FT4			menu1Prim[2] ;
	POLY_F4			fadeDownPrim[2] ;
	POLY_F4			fadeUpPrim[2] ;
	int				currMenu= -2 ;
	int				currOption = 0 ;
	int				oldMenu= 0 ;
	int				oldOption = 0 ;
	static char		firstTime = 1 ;
	int				i ;
	Object			*shipShapes[ NO_OF_SHIPS ];
	Object			*audioShapes[ 3 ];
	Object			*pilotShapes[ 8 ];
	Object			*teamShapes[ 4 ];

	Object			*clsShapes[ 3 ];
	Object			**classShapes = clsShapes;

	Object			*gameShapes[ 4 ];
	Object			**gameTypeShapes = gameShapes;

	Object			*trackShapes[ 7 ];

	Object			*optShapes[ 7 ];
	Object			**optionShapes = optShapes;

	Object			*padShapes[ 3 ];
	Object			**joypadShapes = padShapes;

	Object			*negShapes[ 3 ];
	Object			**negconShapes = negShapes;

	TimData 			*hiScoreScreen ;
	SlideTextures	musicTex ;
	MusicPrims		musicPrims ;
	static SelectionData	selectData ;
	char				loadGames [MAX_GAMES][9] ;
	char				loadFlag = 1 ;
	Texture			*copyrightSymbol ;
	POLY_FT4			copyPrim[2] ;
	Texture			*trackTex [7] ;
	POLY_FT4			trackPrims[2] ;
	int				buttonPressed = 0 ;
	char				trackList[15] = {2, 4, 1, 12, 8, 9, 10, 3, 5, 6, 7, 11, 13, 14, 15} ;
	long				length;
   Prm         	prm;
   char 	      	*start;
   Object	     	*object;
	u_char			param;
	int				frame_cnt = 0 ;
	int				x = 0, y = 0 ;
	int				TOut = 180;
	unsigned long start_time, end_time, duration, ctrl_remainder, ctrl_frame;
	unsigned long fr_count;


#if USE_ATI
	menuTexturesIn = 1;
	//JJSNewTexture ("wipeout\\newgraph\\menu\\menus.bmp");
	SJRNewTexture ("wipeout\\newgraph\\menu\\menus.sjr");
#endif
	trackNum = 15;//pretend track for restoring - SJR
	LoadVRam("wipeout\\newgraph\\menu\\menus.pcx",0);

	LoadTexModel (shipShapes, "allsh") ;
	LoadTexModel (pilotShapes, "pilot") ;
	LoadTexModel (teamShapes, "team") ;	 			// jed
	LoadTexModel (audioShapes, "cdobj") ;			 /*audio = headphones*/
	LoadTexModel (classShapes, "leeg") ;			// jed
	LoadTexModel (joypadShapes, "cntrl") ;			// jed
	LoadTexModel (gameTypeShapes, "msdos") ;		// jed
	LoadTexModel (optionShapes, "misc") ;	// jed

	length = FileLength( "wipeout\\common\\pad1.prm");

//	LoadTexModel (shipShapes, "allsh") ;
//	LoadTexModel (pilotShapes, "pilot") ;
//	LoadModel (teamShapes, "teams") ;
//	LoadTexModel (audioShapes, "cdobj") ;			 /*audio = headphones*/
//	LoadModel (classShapes, "class") ;
//	classShapes += 1;			// if lightsourced
//	LoadNegConTexModel (negconShapes, "neg2") ;
//	negconShapes += 1;		// if lightsourced
//	LoadTexModel (joypadShapes, "pad1") ;
//	LoadModel (gameTypeShapes, "champ") ;
//   gameTypeShapes += 1;	// if lightsourced

//	length = FileLength( "wipeout\\common\\pad1.prm");

//	start = DAlloc( heap, length + 32 );
//	prm.ptr = start;
//	joypadShapes[1] = prm.object;
//	memcpy(start, joypadShapes[0], length);
//	object = prm.object++;
//	object->skeleton = prm.skeleton++;
//	object->vertices = prm.svector;
//	prm.svector += object->vertexCount;
//	object->normals = prm.svector;
//	prm.svector += object->normalCount;
//	object->primitives = prm.primitive;
//

	InitTrackTex (trackTex) ;

// following inits were in the #if lightsourced section, this is now all we have (PCwipeout)
	menuSystem[ MAIN_MENU ].menuModel = shipShapes ;	
	menuSystem[ MAIN_MENU ].menu2Model = gameTypeShapes ;	
	menuSystem[ TRACK_MENU ].menuModel = shipShapes ;	
	//menuSystem[ AUDIO_MENU ].menuModel = optionShapes ;
	menuSystem[ OPTION_MENU ].menuModel = optionShapes ;	
	menuSystem[ CLASS_MENU ].menuModel = classShapes ;	
	menuSystem[ TEAM_MENU ].menuModel = teamShapes ;	
	menuSystem[ TEAM_MENU ].menu2Model = shipShapes ;	
	menuSystem[ PILOT_MENU ].menuModel = pilotShapes ;	
	menuSystem[ PILOT_MENU ].menu2Model = shipShapes ;	
	menuSystem[ RACE_TYPE_MENU ].menuModel = gameTypeShapes ;	
	menuSystem[ RACE_TYPE_MENU ].menu2Model = optionShapes ;	
	menuSystem[ SERIAL_RACE_TYPE_MENU ].menuModel = shipShapes ;	
	menuSystem[ NEGCON_MENU ].menuModel = negconShapes ;	
	menuSystem[ NEGCON_MENU ].menu2Model = negconShapes ;
	menuSystem[ TWIST_MENU ].menuModel = negconShapes ;	
	menuSystem[ TWIST_MENU ].menu2Model = negconShapes ;	
	menuSystem[ BEST_TIMES_MENU ].menuModel = optionShapes ;	
	menuSystem[ CONTROLLER_MENU ].menuModel = joypadShapes ;	
	menuSystem[ CONTROLLER_MENU ].menu2Model = joypadShapes ;	
	menuSystem[ CONTROL_EDIT_MENU ].menuModel = joypadShapes ;	
	menuSystem[ CONTROL_EDIT_MENU ].menu2Model = joypadShapes ;	
	menuSystem[ DETAILS_MENU ].menuModel = shipShapes ;	

	InitMenuDisplay ( 90 ) ;	
	InitLoadSave (&(loadGames[0])) ;
	InitMusicVSfx (&musicTex) ;
	InitButtons ("buttons") ;

	if (firstTime)
		InitSelectData (&selectData) ;
 
	selectData.fadeHeight = 0 ;
	selectData.select = 0 ;
	selectData.combatInit = 0 ;
	selectData.combatComplete = 0 ;

	if (gameData->defaultMenu != -2)
		loadFlag = 0 ;

	currMenu = gameData->defaultMenu ;

	menuSystem[1].numOptions = NumTracks ;

	copyrightSymbol = LoadTexture ("wipeout\\textures\\copyrigt.tim", 1) ;

	InitMenuSystem (&(menuSystem[0])) ;

	selectData.menuSystem = &(menuSystem[0]) ;

  clear[ 0 ] = BlockFill( 0, 0, 320, 240, 0x00, 0x00, 0x00 );
  clear[ 1 ] = BlockFill( 0, 240, 320, 240, 0x00, 0x00, 0x00 );

	ClearOTagR( OT[ CurrentScreen ], OT_SIZE );

 	AddPrim( OT[ CurrentScreen ] + ( OT_SIZE - 1 ), ( ulong* ) clear[ CurrentScreen ] ); // PCwipeout
 	UpdateNotes(NOT_PAUSED);
		Swap () ;

 	currMenu = -2;

#if USE_ATI
	menuScreen = JJSLoad16BitTexture	("wipeout\\textures\\wipeout1.tim", VRAM) ;
#else
	menuScreen = AJSLoad16BitTexture	("wipeout\\textures\\wipeout1.pcx", VRAM) ;
#endif	
#if CD_AUDIO

  c_attr.cd.volume.left  = 150 * gameData->currSliderPos;	/* Master volume (left) */
  c_attr.cd.volume.right  = 150 * gameData->currSliderPos;	/* Master volume (right) */
		globalvol = 218 - gameData->currSliderPos;
	CdSetVol(gameData->currSliderPos);

  SpuSetCommonAttr (&c_attr);
#ifdef PORT_SOUND
  SfxSetCdVolume(ConvCDVol(c_attr.cd.volume.left,c_attr.cd.volume.right));
#endif

	param = CdlModeRept|CdlModeDA;	
	CdControlB(CdlSetmode, &param, 0);	
	if(gameData->cdTrack == 1)
	{
	   CdControlB(CdlSetloc, (u_char *)&loc[(randy()%(endtrack-1))+2], 0);
	}
	else														 
	   CdControlB(CdlSetloc, (u_char *)&loc[gameData->cdTrack], 0);	

	CdControlB(CdlPlay, 0, 0);  
#endif

	inmenu=0;
	for( i = 0; i < 2; i++)
	{
		ClearOTagR( OT[ CurrentScreen ], OT_SIZE );
	    Swap( );
	}
	inmenu=1;
	CopyPal(main_palette,0,255);	// PCwipeout
	for( i = 0; i < 2; i++)
		Swap( );

	andyTimeout=0;
	while (1)
	{
	  	opad=pad;
		pad = neg1.bits;
		andyTimeout++;

		if(andyTimeout >= TOut * 30)
		{
			andybodge=2;
			break;
		}

		if(neg1.head != 0xff)
			pad = ~pad;

		xpad=(pad^opad)&pad;

		UpdateNotes(NOT_PAUSED);

		textPrim = CurrentScreen * 300 ;

 		ClearOTagR( OT[ CurrentScreen ], OT_SIZE );

		oldMenu = currMenu ;
		oldOption = currOption ;

		if (neg1.head == 8960)
		{
			if ((currMenu == PADCONFIG_MENU) && ((menuSystem[PADCONFIG_MENU].displayObject == DrawPlayPad) ||
				(menuSystem[PADCONFIG_MENU].displayObject == 0)))
				currMenu = NEGCON_MENU ;

			menuSystem[OPTION_MENU].menu2Model = negconShapes ;	
			menuSystem[17].menuModel = negconShapes ;	
			menuSystem[PADCONFIG_MENU].displayMenu = MenuNegCon ;	
			menuSystem[PADCONFIG_MENU].displayObject = NegConButtonDisplay ;
			menuSystem[PADCONFIG_MENU].prevMenu = NEGCON_MENU ;
			gameData->oldPadPreset = gameData->normpadPreset ;
		}
		else
		{
			if ((currMenu == NEGCON_MENU)	||	 (currMenu == TWIST_MENU))	
				currMenu = PADCONFIG_MENU ;

			menuSystem[OPTION_MENU].menu2Model = joypadShapes ;	
			menuSystem[17].menuModel = joypadShapes ;	
			menuSystem[PADCONFIG_MENU].displayMenu = MenuPlayPad ;	
			menuSystem[PADCONFIG_MENU].displayObject = DrawPlayPad ;
			menuSystem[PADCONFIG_MENU].prevMenu = OPTION_MENU ;
			gameData->oldNegPreset = gameData->negconPreset ;
		}


		if (currMenu != -2)
		{
			MenuErrors (&currMenu, &(menuSystem[0])) ;
			if (menuSystem[currMenu].errorFlag != NO_ERROR)
			{
				selectData.fadeHeight = 0 ;
				selectData.select = 0 ;
			}
		}

		if (menuSystem[currMenu].updateCount != 0 && menuSystem[currMenu].errorFlag == NO_ERROR) 
			menuSystem[currMenu].updateCount-- ;

//		ShowMenuBackground (&(menuPrim[0]), &(menu1Prim[0]), menuScreen) ; // PCwipeout

		if (currMenu == -2)
			currMenu = 0 ;

		if (menuSystem[currMenu].errorFlag == NO_ERROR)
		{
			if (currMenu >= 0)
				CallMenuSelector (&(menuSystem[0]), &currMenu, &currOption, shipShapes, gameData, &musicPrims, &musicTex, &selectData, &(loadGames[0])) ;

	 		if (currMenu == TRACK_MENU) 
				DisplayWinLose (trackTex[currOption], &(trackPrims[0]), 96, 50, 74, MENU) ;

			if (((currMenu == SAVE_MENU) || (currMenu == LOAD_MENU))	&& (selectData.formatCard == 1))
			{
			 	currMenu 	= FORMAT_CONFIRM ;
				currOption 	= 0 ;
			}

			if (selectData.select) 
			{
				if (currMenu == MAIN_MENU && currOption == 0)
				{
						// CHEAT ACTIVATION BITS - need to be changed for PCwipeout

						if( (pad & PadLeft0) && (pad & PadCross0))
						{
							gameData->RapierClass = 1 ;
						}

						if( (pad & PadRight0) && (pad & PadTriangle0))
						{
							totalTracks = 14 ;
							NumTracks = 7 ;
							gameData->showBonusTrack = 1 ;			
						}
				}

				if (menuSystem[currMenu].updateCount == 0)
					CallMenuActivator (gameData, &currMenu, &currOption, &(menuSystem[0]), &selectData, &(loadGames[0])) ;
			}

			if(davebodge == 2 || davebodge == 3 || davebodge == 4)
				CallMenuActivator (gameData, &currMenu, &currOption, &(menuSystem[0]), &selectData, &(loadGames[0])) ;
		}

		if (currMenu == TRACK_MENU)
		{
			if (gameData->class == 0)
				CentreText ("VENOM CLASS",text_data (20, 40, 8),RedText) ;  
			else
				CentreText ("RAPIER CLASS",text_data (20, 40, 8),RedText) ;
		}

		if (currMenu == CLASS_MENU && currOption == 1 && (!gameData->RapierClass))
			CentreText ("NOT AVAILABLE",text_data (20, 190, 12),RedText) ;

/*** Serial link up within menu structure ***/

		if (gameType == SINGLE)
		{
			if (selectData.combatComplete == 32)
			{
			 	currMenu = MAIN_MENU ;
				currOption = 0 ;
				selectData.combatInit = 0 ;
				selectData.combatComplete = 0 ;
		 	}
		}

		if (gameType == MASTER || gameType == SLAVE)
		{
			if (selectData.combatComplete == 32)
			{
				currMenu = TRACK_MENU ;
				currOption = 0 ;
				selectData.combatInit = 0 ;
				selectData.combatComplete = 0 ;

				if (gameType == SINGLE)
					currMenu = MAIN_MENU ;
				else if (gameType == MASTER || gameType == SLAVE)
				{
					if (gameType == MASTER)
						currMenu = CLASS_MENU ;

					if (gameType == SLAVE)
					{
						currMenu = TEAM_MENU ;
						menuSystem[TEAM_MENU].prevMenu = TEAM_MENU ;	
					}
				}
			}

			if (currMenu == -1)
				gameData->exitMenu = 1 ;
		 	if (currMenu == CLASS_MENU || currMenu == TEAM_MENU
				|| currMenu == PILOT_MENU 
				|| currMenu == SERIAL_RACE_TYPE_MENU || currMenu == TRACK_MENU
				|| currMenu == START_TWO_PLAYER_MENU || currMenu == -1)
			{
//				if (gameType == MASTER)
//					MenuMaster (gameData, &currMenu) ;
//				else
//					CaptainBirdsEye (gameData, &currMenu) ;
			}

//			if (gameData->exit2Menu)
//				{
//				fn_track("other machine asked to exit")
//				currMenu = -1 ;
//				}	
		}

/********************************************/

		negSelect = neg1.i; 
		negQuit = neg1.ii;
		negBack = neg1.ii;
		Swap() ;

		firstTime = 0 ;

		if (menuSystem[currMenu].updateCount == 0 && 
			 menuSystem[currMenu].errorFlag == NO_ERROR &&
			 selectData.select == 1)
		{ 
			selectData.select = 0 ;
			menuSystem[currMenu].updateCount = 0 ;
		}

		selectData.trackNum = gameData->trackNum ;

		if (currMenu == -1)
		{
			DrawSync(0);
			InitDynamicMem(heap, HEAP_SIZE*1024);		/* The left over bit in the middle */
			InitGameDisplay  ( ) ;
			gameData->exitMenu = 0 ;
			gameData->exit2Menu = 0 ;

			for(i = 0; i < 10; i++)
			{
//				VSync(0);
	   			c_attr.cd.volume.left  -= c_attr.cd.volume.left >> 3 ;	/* Master volume (left) */
	   			c_attr.cd.volume.right -= c_attr.cd.volume.right >> 3;	/* Master volume (right) */
	   			SpuSetCommonAttr (&c_attr);
#ifdef PORT_SOUND
  				SfxSetCdVolume(ConvCDVol(c_attr.cd.volume.left,c_attr.cd.volume.right));
#endif
				UpdateNotes(NOT_PAUSED);
			}

			CdControlB(CdlStop, 0, 0);  
			return;
		}
	}
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/

void CallMenuActivator (ConfigData *gameData, int *currMenu, int *currOption, Menus *menuSystem, SelectionData *selectData, char loadGames[][9]) 
{
	short		i;
	long		cardStatus, slotStatus ;
	long		cnt = 0 ;
	long		ret ;
	char		trackList[15] = {2, 4, 1, 12, 8, 9, 10, 3, 5, 6, 7, 11, 13, 14, 15} ;
	FILE		*fd;


	/******************/
	/***** Menu 1 *****/
	/******************/

	if (*currMenu == MAIN_MENU)
	{
		menuSystem[TEAM_MENU].prevMenu = RACE_TYPE_MENU ;	
		menuSystem[CLASS_MENU].prevMenu = MAIN_MENU ;	

		if (*currOption == SINGLE_GAME)
		{
//			CommMultiPlaySelect=_SINGLE_PLAY;
			gameType = SINGLE ;
			*currMenu = CLASS_MENU ;
			*currOption = 0 ;
			menuSystem[TEAM_MENU].prevMenu = RACE_TYPE_MENU ;	
			menuSystem[CLASS_MENU].prevMenu = MAIN_MENU ;
		}

//		if (*currOption == MULTI_GAME)
//		{
//			*currMenu = MULTI_MENU;
//			*currOption = 0;
//		}								// jed


		if (*currOption == GAME_OPTIONS)
		{
			*currMenu = OPTION_MENU ;
			*currOption = 0 ;
		}

		if (*currOption == QUIT_TO_DOS)
		{
			*currMenu = QUIT_MENU;
			*currOption = 0;
		}
		return ;
	}
	
	/*******************/
	/***** Menu 28 *****/
	/*******************/

	if (*currMenu == MULTI_MENU)
	{
		if (*currOption == DUAL_GAME)
		{
//			*currMenu = INIT_TWO_PLAYER ;
			andybodge=1;
			*currMenu = SERIAL_MENU;
			*currOption = 0 ;

//			menuSystem[CLASS_MENU].prevMenu = CLASS_MENU ;
//			if (gameType == SLAVE)
//				menuSystem[TEAM_MENU].prevMenu = TEAM_MENU ;	
//
//			gameType = SINGLE ;
//			gameData->shipNum = 0 ;
//			gameData->ship2Num = 1 ;
//			selectData->combatInit = 0 ;
//			selectData->combatComplete = 0 ;
//			InitializeCombatLink () ;
		}

		if (*currOption == NETWORK_GAME)
		{
			andybodge=1;
			*currMenu = NETWORK_MENU;
			*currOption = 0;
		}
		return ;
	}

	/*******************/
	/***** Menu 29 *****/
	/*******************/

	if (*currMenu == NETWORK_MENU)
	{
		if(davebodge == 2)
		{
			davebodge = 1;
			*currMenu = INIT_TWO_PLAYER;
		}
		return;
	}

	/*******************/
	/***** Menu 30 *****/
	/*******************/

	if (*currMenu == SERIAL_MENU)
	{
		if(davebodge == 2)
		{
			davebodge = 1;
			*currMenu = INIT_TWO_PLAYER;
		}
		return;
	}



	/******************/
	/***** Menu 2 *****/
	/******************/

	if (*currMenu == TRACK_MENU)
	{
		if (gameData->class)
			gameData->trackNum = (*currOption  + 1) + 7 ;	
		else
			gameData->trackNum = (*currOption  + 1) ;	
		gameData->actTrackNum = trackList[(gameData->trackNum-1)] ;
		*currMenu = -1 ;
		*currOption = 0 ;
		gameData->newLapRecord = 0 ;

		if (gameType == SINGLE)
		{
			ownShip = gameData->shipNum ;
			serialShip = -1 ;
		}
		else
		{
			if (gameType == MASTER)
			{
				if (gameData->oppMenu == START_TWO_PLAYER_MENU)
					*currMenu = -1 ;
				else
					*currMenu = START_TWO_PLAYER_MENU ;

				ownShip = gameData->shipNum ;
				serialShip = gameData->ship2Num ;
			}

		}
		return ;
	}
	
	/******************/
	/***** Menu 3 *****/
	/******************/
	
	if (*currMenu == DUAL_GAME_MENU)
	{
		if (*currOption == MASTER_PSX)
		{
			gameType = MASTER ;
			*currMenu = TRACK_MENU ;
			*currOption = 0 ;
			menuSystem[1].prevMenu = DUAL_GAME_MENU ;	
		}

		if (*currOption == SLAVE_PSX)
		{
			gameType = SLAVE ;
			*currMenu = TRACK_MENU ;
			*currOption = 0 ;
			menuSystem[1].prevMenu = DUAL_GAME_MENU ;	
		}
		return ;
	}

	/******************/
	/***** Menu 4 *****/
	/******************/
	
	/*if (*currMenu == AUDIO_MENU)
	{
		gameData->currSliderPos = gameData->oldSliderPos ;
		gameData->cdTrack = gameData->oldCdTrack ;
		
   	c_attr.cd.volume.left  = 150 * gameData->currSliderPos;	// Master volume (left) 
   	c_attr.cd.volume.right  = 150 * gameData->currSliderPos;	// Master volume (right) 
		sampleLevel = 0x3fff - (75 * gameData->currSliderPos);
		SfxSetVolume((char)(sampleLevel>>6));
//		KillNote(SHIELDS); // AJS
		for(i = 0; i < 24; i++)
		{
	   		chanTable[i].s_attr.volume.left  = sampleLevel;		// Left volume  0x3fff 
	   		chanTable[i].s_attr.volume.right = sampleLevel;		// Right volume
			SpuSetVoiceAttr(&(chanTable[i].s_attr));
		}	

	   	SpuSetCommonAttr (&c_attr);
		CdSetVol(gameData->currSliderPos);
		globalvol = 218 - gameData->currSliderPos;
		SfxSetVolume((char)(sampleLevel>>6));
#ifdef PORT_SOUND
  		SfxSetCdVolume(ConvCDVol(c_attr.cd.volume.left,c_attr.cd.volume.right));
#endif
		*currMenu = menuSystem[(*currMenu)].prevMenu ;
		*currOption = 0 ;
		return ;
	}*/

	/******************/
	/***** Menu 5 *****/
	/******************/
	
	if (*currMenu == OPTION_MENU)
	{
		if (*currOption == LOAD_GAME)
		{
			selectData->formatConfirm = 0 ;
			selectData->formatCard = 0 ;
			selectData->slotRow = 0 ;
			selectData->slotColumn = 0 ;
			selectData->slotsFilled = 0 ;

			for (i=0; i < 2; i++)
			{
				slotStatus = GetCardInfo (i) ;

				switch (slotStatus)
				{
				 	case 0:	break ;					 /* CARD	EXISTS */
				 	case 1:	break ;					 /* ERROR */
				 	case 2:	break ;					 /* NO CARD */
				 	case 3:	ClearEventHW () ;
//								if (i	== 0)
//									_card_clear (0) ; // PCwipeout
//								else
//									_card_clear (16) ; // PCwipeout
								cardStatus = CheckHWCardEvents () ;
								break ;					 /* NEW CARD */
				 }
			
				LoadCard (i) ;
				cardStatus = CardContentStatus () ;

				switch (cardStatus)
				{
					case 0:	if (i == 0)
									selectData->card1 = FORMATTED ;
								else
									selectData->card2 = FORMATTED ;
								break ;
				  	case 3:	if (i == 0)
									selectData->card1 = UNFORMATTED ;
								else
									selectData->card2 = UNFORMATTED ;
				 				break ;					 /* UNFORMATTED */
				}
			}
			LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum) ;
			*currMenu = LOAD_MENU ;
			*currOption = 0 ;
		}
 
		if (*currOption == SAVE_GAME)
		{
			selectData->formatCard = 0 ;
			selectData->formatConfirm = 0 ;
			selectData->slotRow = 0 ;
			selectData->slotColumn = 0 ;
			selectData->slotsFilled = 0 ;

			for (i=0; i < 2; i++)
			{
				slotStatus = GetCardInfo (i) ;

				switch (slotStatus)
				{
				 	case 0:	break ;					 /* CARD	EXISTS */
				 	case 1:	break ;					 /* ERROR */
				 	case 2:	break ;
				 	case 3:	ClearEventHW () ;
//								if (i	== 0)
//									_card_clear (0) ; // PCwipeout
//								else
//									_card_clear (16) ; // PCwipeout
								cardStatus = CheckHWCardEvents () ;
								break ;					 /* NEW CARD */
				}

				LoadCard (i) ;
				cardStatus = CardContentStatus () ;

				switch (cardStatus)
				{
				 	case 0:	if (i == 0)
									selectData->card1 = FORMATTED ;
								else
									selectData->card2 = FORMATTED ;
								break ;					 /* FORMATTED */
				 	case 1:	break ;					 /* ERROR */
				 	case 2:
				 	case 3:	if (i == 0)
									selectData->card1 = UNFORMATTED ;
								else
									selectData->card2 = UNFORMATTED ;
				}
			}

			LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum)  ;
			*currMenu = ENTER_GAME_NAME;
			*currOption = 0 ;
		}

		if (*currOption == CONTROLS)
		{
				*currMenu = CONTROLLER_MENU;
				*currOption = 0;
				changecontroller = 1;
		}

		if (*currOption == DETAILS)
		{
			*currMenu = DETAILS_MENU;
			*currOption = 0;
		}

		/*if (*currOption == AUDIO_CONFIG)
		{
//			PlayNote(SHIELDS, 0, -1);
			*currMenu = AUDIO_MENU ;
			*currOption = 0 ;
			selectData->optionNo = 0 ;

			InitMenuDisplay ( 180 ) ;	
			gameData->oldSliderPos = gameData->currSliderPos ;
			gameData->oldCdTrack = gameData->cdTrack ;
		}*/
		if (*currOption == HIGH_SCORES)
		{
			selectData->hiscoreTrack = 0 ;
			*currMenu = BEST_TIMES_MENU ;
			*currOption = 0 ;
		}
		return ;
	}

	/******************/
	/***** Menu 6 *****/
	/******************/
	
	if (*currMenu == CLASS_MENU)
	{
		gameData->class = *currOption ;
		if (gameData->RapierClass == 0 && *currOption == 1)
			return ;
		*currMenu = RACE_TYPE_MENU ;
		
		if (gameType == SINGLE || gameType == SLAVE)
		{
			*currMenu = RACE_TYPE_MENU ;
			menuSystem[CLASS_MENU].prevMenu = MAIN_MENU ;
		}
		else
		{
			*currMenu = SERIAL_RACE_TYPE_MENU ;
		}

		if (*currOption == 0)
		{
		 	gameData->trackNum = 1 ;  	  /* 1 */
		}

		if (*currOption == 1)
		{
			if (gameData->showBonusTrack == 0)
			 	gameData->trackNum = (NumTracks+2) ;/*	(NumTracks+2)*/
			else
			 	gameData->trackNum = (NumTracks+1) ;/*	(NumTracks+1)*/

		}

		*currOption = 0 ;
		return ;
	}

	/******************/
	/***** Menu 7 *****/
	/******************/
	
	if (*currMenu == TEAM_MENU)
	{
		gameData->team = *currOption ;
		SetUpPilots (gameData->team, menuSystem) ;
		*currMenu = PILOT_MENU ;
		*currOption = 0 ;

		if (gameType == SINGLE)
		{
				menuSystem[TEAM_MENU].prevMenu = RACE_TYPE_MENU ;
		}
		else
			if (gameType == SLAVE)
			{
				menuSystem[TEAM_MENU].prevMenu = TEAM_MENU ;
			}
			else
			{
				menuSystem[TEAM_MENU].prevMenu = SERIAL_RACE_TYPE_MENU ;
			}	

		return ;
	}

	/******************/
	/***** Menu 8 *****/
	/******************/
	
	if (*currMenu == PILOT_MENU)
	{
		gameData->shipNum = (gameData->team * 2) + *currOption ;
		*currOption = 0 ;
		gameData->newLapRecord = 0 ;

		if (gameType == SINGLE)
		{
			*currMenu = TRACK_MENU ;
			if (gameData->gameType == CHAMPIONSHIP)
			{
				*currMenu = -1 ;
				*currOption = 0 ;
				ownShip = gameData->shipNum ;
				serialShip = -1 ;
			}
		}
		else
			if (gameType == MASTER || gameType == SLAVE)
			{
				if (gameType == MASTER)
					*currMenu = TRACK_MENU ;

				if (gameType == SLAVE)
				{
					if (gameData->oppMenu == START_TWO_PLAYER_MENU)
					{
						*currMenu = -1 ;
						ownShip = gameData->shipNum ;
						serialShip = gameData->ship2Num ;
					}
					else
						*currMenu = START_TWO_PLAYER_MENU ;
				}
			}
		return ;
	}

	/******************/
	/***** Menu 9 *****/
	/******************/
	
	if (*currMenu == RACE_TYPE_MENU)
	{
		*currMenu = TEAM_MENU ;
		menuSystem[1].prevMenu = PILOT_MENU ;	
		menuSystem[TEAM_MENU].prevMenu = RACE_TYPE_MENU ;			

		gameData->prevLap = 0 ;

		raceType = 0 ;

		if (((*currOption)+1) == CHAMPIONSHIP)
		{
			*currOption = 0 ;

		/***** Initialize Championship Data *****/

		 	for (i=0; i < NO_OF_SHIPS; i++)
			{
				winTable[i].points = 0 ;
				winTable[i].position = 1 ;
			}

			InitChampTable () ;

		/***** Set track to play *****/

 			menuSystem[1].numOptions = NumTracks ;
			gameData->actTrackNum = trackList[(gameData->trackNum-1)] ;
			gameData->gameType = CHAMPIONSHIP ;
		}

		if (((*currOption)+1) == TIME_TRIAL)
		{
			*currOption = 0 ;
 			menuSystem[1].numOptions = NumTracks ;
			gameData->gameType = TIME_TRIAL ;
			raceType = TIME_TRIAL ;
		}

		if ((*currOption+1) == SINGLE_RACE)
		{
			*currOption = 0 ;
 			menuSystem[1].numOptions = NumTracks ;
			gameData->gameType = SINGLE_RACE ;
		}
		return ;
	}


	/*******************/
	/***** Menu 10 *****/
	/*******************/
	
	if (*currMenu == LOAD_MENU)
	{
		if(TextPos < 1)
			return;

		LoadFilename[TextPos] = '\0';
		TextEntry = 0;
		TextPos = 0;

		if (!LoadCardData(LoadFilename, selectData->cardNum, gameData, selectData))
		{
			menuSystem[(*currMenu)].updateCount = 120 ;
			menuSystem[(*currMenu)].errorFlag = LOAD_ERROR ;
			return ;
		}

		strcpy(LoadFilename, "         ");
		LoadFilename[0] = 101;

		*currMenu = MAIN_MENU ;
		*currOption = 0 ;

		menuSystem[1].numOptions = NumTracks ;
		return ;
	}

	/*******************/
	/***** Menu 11 *****/
	/*******************/
	
	if (*currMenu == SAVE_MENU)
	{
		if ((selectData->slotsFilled == 0) && (selectData->numCardFiles == 15))
			return ;
		*currMenu = ENTER_GAME_NAME ;
		*currOption = 0 ;
		selectData->firstTime = 1 ;
		return ;
	}

	/*******************/
	/***** Menu 12 *****/
	/*******************/
	

	if (*currMenu == ENTER_GAME_NAME)
	{
		if(TextPos < 1)
			return;

		SaveFilename[TextPos] = '\0';
		TextEntry = 0;
		TextPos = 0;

//		if (strlen(selectData->nameEntered) <= 1)
//			return ;

//		selectData->nameEntered[strlen(selectData->nameEntered)-1] = 0 ;
//		selectData->confirm = 1 ;

		CheckSaveType (selectData, &(loadGames[0]), gameData) ;

		strcpy(DaveSave, SaveDirBase);
		strcat(DaveSave,"\\");
		strcat(DaveSave, SaveFilename);
		strcat(DaveSave, FILE_EXTENSION);

		fd = fopen(DaveSave, "r");

	 	if(fd == NULL)
	 	{
			// Save
			if (!SaveCardFile (SaveFilename, selectData->cardNum, gameData, gameData->currSliderPos))
			{
	 			menuSystem[(*currMenu)].updateCount = 120 ;
				menuSystem[(*currMenu)].errorFlag = SAVE_ERROR ;
				return ;
			}

			strcpy (loadGames[(selectData->slotRow*2)+selectData->slotColumn], selectData->nameEntered) ;
			strcpy(SaveFilename, "         ");
			SaveFilename[0] = 101;
			*currMenu = OPTION_MENU;
		}
		else
		{
			// Confirm overwrite
			fclose(fd);
			*currMenu = CONFIRM_SAVE ;
		}

		*currOption = 0;
		return ;
	}


	/*******************/
	/***** Menu 14 *****/
	/*******************/
	
	if (*currMenu == CONFIRM_SAVE)
	{
		if((!selectData->confirm) && (SaveFilename != 0))
		{
			// This bit actually does the saving!

			if ( !SaveCardFile (SaveFilename, selectData->cardNum, gameData, gameData->currSliderPos) ) 
			{
	 			menuSystem[(*currMenu)].updateCount = 120 ;
				menuSystem[(*currMenu)].errorFlag = SAVE_ERROR ;
				return ;
			}

			strcpy (loadGames[(selectData->slotRow*2)+selectData->slotColumn], selectData->nameEntered) ;
		}

		strcpy(SaveFilename, "         ");
		SaveFilename[0] = 101;
	 	*currMenu = OPTION_MENU ;
	 	*currOption = 0 ;	

		return ;
	}

	/*******************/
	/***** Menu 15 *****/
	/*******************/
	
	if (*currMenu == INIT_TWO_PLAYER)
	{
		if(davebodge == 3)
		{
			davebodge = 4;
//			menuSystem[(*currMenu)].errorFlag = global_mark_error;
			menuSystem[(*currMenu)].updateCount = 400;
			gameType = SINGLE;
			return;
		}

//		*currMenu = TRACK_MENU ;
		*currOption = 0 ;
		selectData->combatInit = 0 ;
		selectData->combatComplete = 0 ;

		if (gameType == SINGLE)
		{
			*currMenu = MAIN_MENU;
			davebodge = 1;
		}
		else if (gameType == MASTER || gameType == SLAVE)
		{
			if (gameType == MASTER)
			{
				*currMenu = CLASS_MENU ;
				menuSystem[CLASS_MENU].prevMenu = CLASS_MENU ;
			}
			if (gameType == SLAVE)
			{
				*currMenu = TEAM_MENU ;
				menuSystem[TEAM_MENU].prevMenu = TEAM_MENU ;	
			}
		}

		return;
	}

	/*******************/
	/***** Menu 16 *****/
	/*******************/
	
	if (*currMenu == SERIAL_RACE_TYPE_MENU)
	{
		*currMenu = TEAM_MENU ;
		menuSystem[1].prevMenu = PILOT_MENU ;	
		menuSystem[TEAM_MENU].prevMenu = SERIAL_RACE_TYPE_MENU ;

		if (*currOption == 0)
		{
			gameData->gameType = TWO_PLAYER_RACE ;
			raceType = TWO_PLAYER_RACE;
		}
	
		if (*currOption == 1)
		{
			raceType = HEAD_TO_HEAD ;
			gameData->gameType = HEAD_TO_HEAD ;
		}
		*currOption = 0 ;
		return ;
	}


	/*******************/
	/***** Menu 18 *****/
	/*******************/
	
	if (*currMenu == PADCONFIG_MENU)
	{
		if (neg1.head == 8960)
		{
			SetNegconPresets (gameData) ;
			gameData->negconPreset = gameData->oldNegPreset ;
			*currMenu = NEGCON_MENU ;
		}
		else
		{
			SetNormPadPresets (gameData) ;
			gameData->normpadPreset = gameData->oldPadPreset ;
			*currMenu = OPTION_MENU ;
		}

		*currOption = 0 ;
		return ;
	}

	/*******************/
	/***** Menu 19 *****/
	/*******************/
	
	if (*currMenu == FORMAT_CONFIRM)
	{
		if (selectData->formatConfirm == 1)
		{
			if (selectData->cardNum == 0)
			{
				ret = format ("bu00:") ;

				if (ret == 0)
				{
					menuSystem[(*currMenu)].updateCount = 120 ;
					menuSystem[(*currMenu)].errorFlag = FORMAT_ERROR ;
					*currMenu = selectData->calledMenu ; 
					selectData->formatCard = 0 ;
					selectData->formatConfirm = 0 ;
					*currOption = 0 ;
					return ;
				}
				else
				{
					*currMenu = selectData->calledMenu ; 
					selectData->formatCard = 0 ;
					*currOption = 0 ;
					selectData->card1 = FORMATTED ;
					InitLoadSave (&(loadGames[0])) ;
					selectData->slotRow = 0 ;
					selectData->slotColumn = 0 ;
					selectData->slotsFilled = 0 ;
				}
			}
			else
			{
			 	ret = format ("bu10:") ;
				if (ret == 0)
				{
					menuSystem[(*currMenu)].updateCount = 120 ;
					menuSystem[(*currMenu)].errorFlag = FORMAT_ERROR ;
					*currMenu = selectData->calledMenu ; 
					selectData->formatCard = 0 ;
					selectData->formatConfirm = 0 ;
					*currOption = 0 ;
					return ;
				}
				else
				{
					*currMenu = selectData->calledMenu ; 
					selectData->formatCard = 0 ;
					*currOption = 0 ;
					selectData->card2 = FORMATTED ;
					InitLoadSave (&(loadGames[0])) ;
					selectData->slotRow = 0 ;
					selectData->slotColumn = 0 ;
					selectData->slotsFilled = 0 ;
				}
			}
		}

		selectData->formatCard = 0 ;

		*currMenu = selectData->calledMenu ; 
		*currOption = 0 ;
	
		return ;
	}

	/*******************/
	/***** Menu 20 *****/
	/*******************/
	
	if (*currMenu == NEGCON_MENU)
	{
		if (*currOption == 0)
		{
			*currOption = 0 ;
			*currMenu = PADCONFIG_MENU ;
			gameData->oldNegPreset = gameData->negconPreset ;
		}

		if (*currOption == 1)
		{
			*currOption = 0 ;
			*currMenu = TWIST_MENU ;
			twist = gameData->negTwist ;
			nCon.twistFactor = abs(gameData->negTwist-128) ;
		}
		return ;
	}


	/*******************/
	/***** Menu 21 *****/
	/*******************/
	
	if (*currMenu == TWIST_MENU)
	{
		*currOption = 0 ;
		*currMenu = NEGCON_MENU ;
		gameData->negTwist = twist ;
		negtwist = gameData->negTwist ;
		nCon.twistFactor = abs(gameData->negTwist-128) ;
		if(nCon.twistFactor < 30) nCon.twistFactor = 30;
		return ;
	}


	/*******************/
	/***** Menu 22 *****/
	/*******************/
	
	if (*currMenu == BEST_TIMES_MENU)
	{
		if (*currOption == 0)
		{
			selectData->hiscoreTrack = 0 ;
		 	*currMenu = HIGH_SCORES_MENU ;
		 	*currOption = 0 ;
		}

		if (*currOption == 1)
		{
			selectData->hiscoreTrack = 14 ;
		 	*currMenu = TIME_TRIAL_MENU ;
		 	*currOption = 0 ;
		}
	}


	/*******************/
	/***** Menu 23 *****/
	/*******************/
	
	if (*currMenu == QUIT_MENU)
	{
		if (selectData->confirm)
		{
		 	*currMenu = MAIN_MENU ;
		 	*currOption = 0 ;
		}
		else
		{
//	   	DumpVram16Bit("c:\\wipeout\\newgraph\\venom\\menus.rgb",&VRam);
			QuitToDos();
		}
	}

	/*******************/
	/***** Menu 24 *****/
	/*******************/
	
	if (*currMenu == DETAILS_MENU)
	{
//	 	*currMenu = MAIN_MENU ;
	 	*currMenu = OPTION_MENU ;
	 	*currOption = 0 ;
	
		Reallyinmenu = 0;
		inmenu = 1;
	}

	/*******************/
	/***** Menu 26 *****/
	/*******************/
	
	if (*currMenu == CONTROL_EDIT_MENU)
	{
	 	*currMenu = CONTROLLER_MENU ;
	 	*currOption = 0 ;

		if(davebodge == 2)
			davebodge = 1;

		return;
	}

	/*******************/
	/***** Menu 25 *****/
	/*******************/
	
	if (*currMenu == CONTROLLER_MENU)
	{
		// store new variables
		controller = orig_controller;
	 	*currOption = 0 ;

		if (changecontroller)
		{
			// change the controller to new settings
			etc_InputDeviceSelect=controller;
			if(etc_InputDeviceSelect != _KEYBOARD)
				StartPAD();

			if((etc_InputDeviceSelect == _JOYSTICK1) && (JoyMinX == 0))
			 	/* SJR*currMenu = CONTROL_EDIT_MENU;
			else*/
			{
				if (JJSJoyConfig())
				{
					etc_Joystick1Switch = _DEVICE_ON;
					etc_InputDeviceSelect = _JOYSTICK1;
				} else {
					etc_Joystick1Switch = _DEVICE_OFF;
					etc_InputDeviceSelect = _KEYBOARD;
					orig_controller = _KEYBOARD;
					StartPAD();
				}

				*currMenu = OPTION_MENU;
			} else
				*currMenu = OPTION_MENU;
		}
		else
		{
		 	*currMenu = CONTROL_EDIT_MENU ;
		}
	}
}



/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

void CallMenuSelector (Menus *menuSystem, int *currMenu, int *currOption, Object **shipShapes, ConfigData *gameData, MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9]) 
{
	int		offSet = 12, i ;			 
	int		textX, textY ;
	int		startOption = 0, menuOffset = 0, dispOption = 0 ;
	Texture	*menuPointer ;
	static int	swapDelay =  0, toggleDisplay = 0 ;
	
	if (GetMenuKey(M_UP))
	{
		PlayNote(MENU_MOVE, 1, 20);
		(*currOption) -- ;
		if ((*currOption) < 0)
		{
			(*currOption) = (menuSystem[(*currMenu)].numOptions - 1) ;
		}	
	}
	
	if (GetMenuKey(M_DOWN))
	{
		PlayNote(MENU_MOVE, 1, 20);
		(*currOption) ++ ;
		if ((*currOption) > (menuSystem[(*currMenu)].numOptions - 1))
			(*currOption) = 0 ;
	}

	if (menuSystem[(*currMenu)].numOptions != 0)
	{	

		if (*currMenu == PILOT_MENU && (gameType == MASTER) && *currMenu == gameData->oppMenu)
		{
			gameData->shipNum = *currOption ;

			if ((gameData->team == gameData->team2) && (gameData->shipNum == gameData->ship2Num))
				*currOption	= (1 - gameData->ship2Num) ;
		}
		else		
			if (*currMenu == PILOT_MENU && (gameType == SLAVE))
				gameData->shipNum = *currOption ;


		if ((gameType == MASTER || gameType == SLAVE))
		{
		 	if (gameType == SLAVE && (gameData->oppMenu == START_TWO_PLAYER_MENU ||	gameData->oppMenu	== TRACK_MENU)
				&& *currMenu == PILOT_MENU)
			{
				if (gameData->team2 == gameData->team)
				{
					*currOption = (1 - (gameData->ship2Num % 2)) ;
					gameData->shipNum = *currOption ;
				}
			}

		 	if (gameType == MASTER && gameData->oppMenu == START_TWO_PLAYER_MENU	&& *currMenu == PILOT_MENU)
			{
				if (gameData->team2 == gameData->team)
				{
					*currOption = (1 - (gameData->ship2Num % 2)) ;
					gameData->shipNum = *currOption ;
				}
			}
		}

		if (neg1.head == 8960)
		{
			if (((gameData->gameType == CHAMPIONSHIP) && (*currMenu == PILOT_MENU)) || (*currMenu == TRACK_MENU) ||
					(gameType == SLAVE && *currMenu == PILOT_MENU))
				DisplayButtons ( 140, 210, NSTART) ;
			else
			{
				DisplayButtons ( 30, 210, NSELECT) ;
				if ((*currMenu != MAIN_MENU) && (!(((gameType == SLAVE) &&	(*currMenu == TEAM_MENU)) ||
						((gameType == MASTER) && (*currMenu == CLASS_MENU)))))
					DisplayButtons ( 240 , 210, NBACK) ;
			}
		}
		else
		{
			if (((gameData->gameType == CHAMPIONSHIP) && (*currMenu == PILOT_MENU)) || (*currMenu == TRACK_MENU) ||
					(gameType == SLAVE && *currMenu == PILOT_MENU))
			{
//				DisplayButtons ( 140, 210 , PSTART) ;
				DisplayButtons ( 30 , 210, PSELECT) ;
				DisplayButtons ( 240, 210, PBACK) ;
			}
			else
			{
				DisplayButtons ( 30, 210, PSELECT) ;
				if ((*currMenu != MAIN_MENU) && (!(((gameType == SLAVE) &&	(*currMenu == TEAM_MENU)) ||
						((gameType == MASTER) && (*currMenu == CLASS_MENU)))))
					DisplayButtons ( 240, 210, PBACK) ;
			}
		}

		startOption = (*currOption / NumTracks) * NumTracks ;

		dispOption = menuSystem[(*currMenu)].numOptions - startOption ;
		
		if  (dispOption >= NumTracks)
			dispOption = NumTracks ;

		CentreText (menuSystem[(*currMenu)].menuTitle, text_data (menuSystem[(*currMenu)].titleX, 
						menuSystem[(*currMenu)].titleY, 8), RedText) ;

		for (i=startOption; i < (startOption + dispOption) ; i++)
		{
			if (i == (*currOption))
			{
#if !USE_ATI
				if ((toggleDisplay % 2) == 0)
#endif
					CentreText (menuSystem[(*currMenu)].menuOptions[i].optionText, text_data (menuSystem[(*currMenu)].menuOptions[i].textX, 
								menuSystem[(*currMenu)].menuOptions[i].textY + (offSet*menuOffset), 8), RedText) ;
#if !USE_ATI
				else
					CentreText (menuSystem[(*currMenu)].menuOptions[i].optionText, text_data (menuSystem[(*currMenu)].menuOptions[i].textX, 
								menuSystem[(*currMenu)].menuOptions[i].textY + (offSet*menuOffset), 8), WhiteText) ;
#endif
			}
			else
			{
				CentreText (menuSystem[(*currMenu)].menuOptions[i].optionText, text_data (menuSystem[(*currMenu)].menuOptions[i].textX, 
							menuSystem[(*currMenu)].menuOptions[i].textY + (offSet*menuOffset), 8), WhiteText) ;
			}

			menuOffset ++ ;

		}

		textX = menuSystem[(*currMenu)].menuOptions[(*currOption)].textX ;
		textY = menuSystem[(*currMenu)].menuOptions[(*currOption)].textY ;

		menuPointer = (Texture *)TextureTable [DRTEXT_8+1] ;	

		SetPolyFT4(&(musicPrims->pointer));			
		musicPrims->pointer.r0=230;	// PCwipeout
		musicPrims->pointer.g0=0;
		musicPrims->pointer.b0=BLACK_NOT_DRAWN;

		setXY4(&(musicPrims->pointer), textX-29, textY+((*currOption%5)*offSet),
												 textX-5, textY+((*currOption%5)*offSet),
												 textX-29, textY+16+((*currOption%5)*offSet),
												 textX-5, textY+16+((*currOption%5)*offSet));

	 	setUV4(&(musicPrims->pointer), menuPointer->u0, menuPointer->v0,
	 											 menuPointer->u1, menuPointer->v1, 
 												 menuPointer->u2, menuPointer->v2+1,
								 				 menuPointer->u3, menuPointer->v3+1) ;

	 	setTPage(&(musicPrims->pointer),0,0,menuPointer->textureX,menuPointer->textureY);
	   musicPrims->pointer.clut = menuPointer->cba;
 
	}
	else
	{
		if (menuSystem[(*currMenu)].displayMenu != 0)
			(menuSystem[(*currMenu)].displayMenu) (musicPrims, musicTex, selectData, &(loadGames[0]), gameData) ; 
	}
  

 	if (menuSystem[(*currMenu)].displayObject != 0)
 	{
 		if (*currMenu == PILOT_MENU) 
 			(menuSystem[(*currMenu)].displayObject) (menuSystem, currMenu, (gameData->team * 2) + *currOption, &swapDelay) ;
 		else
			if (*currMenu == TEAM_MENU)
	 			(menuSystem[(*currMenu)].displayObject) (menuSystem, currMenu, *currOption , &swapDelay) ;
			else
 				(menuSystem[(*currMenu)].displayObject) (menuSystem, currMenu, *currOption, &swapDelay) ;
 	}
 

	if (GetMenuKey(M_SELECT))
	{
		if (menuSystem[(*currMenu)].updateCount == 0 && menuSystem[(*currMenu)].errorFlag == NO_ERROR)
		{
			if (!(*currMenu == ENTER_GAME_NAME && (selectData->nameEntered[strlen(selectData->nameEntered)-1] >= 'A' &&
				 selectData->nameEntered[strlen(selectData->nameEntered)-1] <= ('Z'+1))))
			{
				selectData->select = 1 ;
				menuSystem[(*currMenu)].updateCount = 0 ;
			}
			PlayNote(MENU_SELECT, 1, 20);
			InitMenuDisplay ( 90 ) ;	
		}
	}

	if (GetMenuKey(M_BACK))			// ESCAPE
	{
		if (menuSystem[(*currMenu)].prevMenu != -1)
		{
//			if(*currMenu == AUDIO_MENU)
//				KillNote(SHIELDS);

			if(*currMenu == CONTROL_EDIT_MENU)// && controller == _KEYBOARD)
			{
				// JJS new bit 0606 for exiting menus
				{
					switch(controller)
					{
						case _JOYSTICK1:
							if (JJSJoyConfig())
							{
								etc_Joystick1Switch = _DEVICE_ON;
								etc_Mouse1Switch = _DEVICE_OFF;
								etc_InputDeviceSelect = _JOYSTICK1;
							}
							else
							{
								etc_Joystick1Switch = _DEVICE_OFF;
								etc_InputDeviceSelect = _KEYBOARD;
								orig_controller = _KEYBOARD;
							}
							break;
						case _MOUSE1:
							etc_Joystick1Switch = _DEVICE_OFF;
							etc_Mouse1Switch = _DEVICE_ON;
							etc_InputDeviceSelect = _MOUSE1;
							break;
						default:
							etc_Mouse1Switch = _DEVICE_OFF;
							etc_Joystick1Switch = _DEVICE_OFF;
							etc_InputDeviceSelect = _KEYBOARD;
							break;
					}
				ResetKeys();
				}
			}
			if((Reallyinmenu) && (*currMenu == DETAILS_MENU))
			{
				ScreenSize = my_ScreenSize;
				MipMap = my_Sky;
				Resolution = my_Resolution;
				DrawDistance = my_DrawDistance;
				ft = my_TextureTrack;
				SkyTrackFlags = my_SkyTrackFlags;


			}

			*currMenu = menuSystem[(*currMenu)].prevMenu ;
			*currOption = 0 ;

			SetUpPilots (gameData->team, menuSystem) ;

			inmenu = 1;
			Reallyinmenu = 0;

			strcpy(LoadFilename, "         ");
			LoadFilename[0] = 101;
			strcpy(SaveFilename, "         ");
			SaveFilename[0] = 101;
			TextEntry = 0;
			TextPos = 0;

			KeyToChange = NONE;

			PlayNote(MENU_SELECT, 1, 20);
			InitMenuDisplay ( 90 ) ;	
		}
	}

	toggleDisplay ++ ;
}


void InitMenuSystem (Menus *menuSystem)
{
	char			i ;
	Object			*tmpObj;

	for (i=0; i < MAX_MENUS; i++)
	{
		menuSystem[i].updateCount = 0 ;	
		menuSystem[i].errorFlag = NO_ERROR ;	
		menuSystem[i].menuTitle[0] = 0 ;	
		menuSystem[i].titleY = 21 ;
	}

	/******************/
	/***** Menu 1 *****/
	/******************/
	
	/* Menu title details */

	menuSystem[0].titleX = 85 ;
	strcpy (menuSystem[0].menuTitle,"WIPEOUT MAIN MENU") ;

	/* Option details */

	menuSystem[0].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[0].menuOptions[0].optionText,"START GAME") ;

//	menuSystem[0].menuOptions[1].textY = 130 ;
//	strcpy (menuSystem[0].menuOptions[1].optionText,"MULTI PLAYER") ;

	menuSystem[0].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[0].menuOptions[1].optionText,"OPTIONS") ;

	menuSystem[0].menuOptions[2].textY = 130 ;						// PCwipeout (ditto)
#if 0
	strcpy (menuSystem[0].menuOptions[2].optionText,"QUIT TO WINDOWS") ;	// PCwipeout
#else
	strcpy (menuSystem[0].menuOptions[2].optionText,"QUIT") ;	// PCwipeout
#endif
	/* Menu details */

	menuSystem[0].numOptions = 3;
	menuSystem[0].displayObject = DrawMainMenu ;	
	menuSystem[0].displayMenu = 0 ;	
	menuSystem[0].prevMenu = -1 ;	

	/******************/
	/***** Menu 2 *****/
	/******************/

	/* Menu title details */

	menuSystem[1].titleX = 75 ;
	strcpy (menuSystem[1].menuTitle,"SELECT RACING CIRCUIT") ;
	
	/* Option details */

	menuSystem[1].menuOptions[0].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[0].optionText,"ALTIMA VII") ;

	menuSystem[1].menuOptions[1].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[1].optionText,"KARBONIS V") ;

	menuSystem[1].menuOptions[2].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[2].optionText,"TERRAMAX") ;

	menuSystem[1].menuOptions[3].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[3].optionText,"KORODERA") ;

	menuSystem[1].menuOptions[4].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[4].optionText,"ARRIDOS IV") ;

	menuSystem[1].menuOptions[5].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[5].optionText,"SILVERSTREAM") ;

	menuSystem[1].menuOptions[6].textY = 127 ;
	strcpy (menuSystem[1].menuOptions[6].optionText,"FIRESTAR") ;

	/* Menu details */

/*	menuSystem[1].displayObject = DrawAudio ;*/
	menuSystem[1].displayObject = 0 ;
	menuSystem[1].displayMenu = 0 ;	
	menuSystem[1].prevMenu = PILOT_MENU ;	


	/******************/
	/***** Menu 3 *****/
	/******************/
	
	/* Menu title details */

	menuSystem[2].titleX = 55 ;
	strcpy (menuSystem[2].menuTitle,"SELECT TWO PLAYER GAME") ;

	/* Option details */

	menuSystem[2].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[2].menuOptions[0].optionText,"MASTER") ;

	menuSystem[2].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[2].menuOptions[1].optionText,"SLAVE") ;

	/* Menu details */

	menuSystem[2].numOptions = 2 ;
	menuSystem[2].displayObject = 0 ;
	menuSystem[2].displayMenu = 0;	
	menuSystem[2].prevMenu = MAIN_MENU ;	


	/******************/
	/***** Menu 4 *****/
	/******************/
	
	/* Option details */

	/* Menu details */

	//menuSystem[3].numOptions = 0 ;
	//menuSystem[3].displayObject = DrawAudio ;
	//menuSystem[3].displayMenu = MusicVSfx ;
	//menuSystem[3].prevMenu = OPTION_MENU ;

					   
	/******************/
	/***** Menu 5 *****/
	/******************/
	
	/* Menu title details */

	menuSystem[4].titleX = 60 ;
	strcpy (menuSystem[4].menuTitle,"GAME CONFIGURATION") ;

	
	/* Option details */

	menuSystem[4].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[4].menuOptions[0].optionText,"LOAD DATA") ;

	menuSystem[4].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[4].menuOptions[1].optionText,"SAVE DATA") ;

	menuSystem[4].menuOptions[2].textY = 130 ;
	strcpy (menuSystem[4].menuOptions[2].optionText,"CONTROLLER CONFIG") ;

	menuSystem[4].menuOptions[3].textY = 130 ;
	strcpy (menuSystem[4].menuOptions[3].optionText,"GRAPHICS DETAIL") ;

	//menuSystem[4].menuOptions[4].textY = 130 ;
	//strcpy (menuSystem[4].menuOptions[4].optionText,"AUDIO CONFIG") ;

	menuSystem[4].menuOptions[4].textY = 130 ;
	strcpy (menuSystem[4].menuOptions[4].optionText,"VIEW BEST TIMES") ;

	/* Menu details */

	menuSystem[4].numOptions = 5 ;
	menuSystem[4].displayObject = DrawOptionMenus ;
	menuSystem[4].displayMenu = 0 ;	
	menuSystem[4].prevMenu = MAIN_MENU ;


	/******************/
	/***** Menu 6 *****/
	/******************/
	
	/* Menu title details */

	menuSystem[5].titleX = 75 ;
	strcpy (menuSystem[5].menuTitle,"SELECT RACING CLASS") ;

	/* Option details */

	menuSystem[5].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[5].menuOptions[0].optionText,"VENOM CLASS") ;

	menuSystem[5].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[5].menuOptions[1].optionText,"RAPIER CLASS") ;

	/* Menu details */

	menuSystem[5].numOptions = 2 ;
	menuSystem[5].displayObject = DrawClass ;
	menuSystem[5].displayMenu = 0 ;	
	menuSystem[5].prevMenu = MAIN_MENU ;	


	/******************/
	/***** Menu 7 *****/
	/******************/
	
	/* Menu title details */

	menuSystem[6].titleX = 75 ;
	strcpy (menuSystem[6].menuTitle,"SELECT YOUR TEAM") ;

	
	/* Option details */

	menuSystem[6].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[6].menuOptions[0].optionText,"AG SYSTEMS") ;

	menuSystem[6].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[6].menuOptions[1].optionText,"AURICOM") ;

	menuSystem[6].menuOptions[2].textY = 130 ;
	strcpy (menuSystem[6].menuOptions[2].optionText,"QIREX") ;

	menuSystem[6].menuOptions[3].textY = 130 ;
	strcpy (menuSystem[6].menuOptions[3].optionText,"FEISAR") ;

	/* Menu details */

	menuSystem[6].numOptions = 4 ;
	menuSystem[6].displayObject = DrawTeams ;
	menuSystem[6].displayMenu = 0 ;	
	menuSystem[6].prevMenu = RACE_TYPE_MENU ;	

	/******************/
	/***** Menu 8 *****/
	/******************/

	/* Menu title details */

	menuSystem[7].titleX = 75 ;
	strcpy (menuSystem[7].menuTitle,"CHOOSE YOUR PILOT") ;

	/* Option details */

	menuSystem[7].menuOptions[0].textX = 50 ;
	menuSystem[7].menuOptions[0].textY = 130 ;

	menuSystem[7].menuOptions[1].textX = 50 ;
	menuSystem[7].menuOptions[1].textY = 130 ;

	/* Menu details */

	menuSystem[7].numOptions = 2 ;
	menuSystem[7].displayObject = DrawPilot ;
	menuSystem[7].displayMenu = 0 ;	
	menuSystem[7].prevMenu = TEAM_MENU ;	


	/******************/
	/***** Menu 9 *****/
	/******************/

	/* Menu title details */

	menuSystem[8].titleX = 75 ;
	strcpy (menuSystem[8].menuTitle,"SELECT RACE TYPE") ;
	
	/* Option details */

	menuSystem[8].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[8].menuOptions[0].optionText,"CHAMPIONSHIP RACE") ;

	menuSystem[8].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[8].menuOptions[1].optionText,"SINGLE RACE") ;

	menuSystem[8].menuOptions[2].textY = 130 ;
	strcpy (menuSystem[8].menuOptions[2].optionText,"TIME TRIAL") ;

	/* Menu details */

	menuSystem[8].numOptions = 3 ;
	menuSystem[8].displayObject = DrawGames ;
	menuSystem[8].displayMenu = 0 ;	
	menuSystem[8].prevMenu = CLASS_MENU ;	

	/*******************/
	/***** Menu 10 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[9].numOptions = 0 ;
	menuSystem[9].displayObject = 0 ;
	menuSystem[9].displayMenu = EnterLoadGame;	
	menuSystem[9].prevMenu = OPTION_MENU ;	

	/*******************/
	/***** Menu 11 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[10].numOptions = 0 ;
	menuSystem[10].displayObject = 0 ;
	menuSystem[10].displayMenu = EnterSaveGame ;	// DLSSave
	menuSystem[10].prevMenu = OPTION_MENU ;	


	/*******************/
	/***** Menu 12 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[11].numOptions = 0 ;
	menuSystem[11].displayObject = 0 ;
	menuSystem[11].displayMenu = EnterSaveGame ;	
	menuSystem[11].prevMenu = OPTION_MENU ;			// DLSSave

	/*******************/
	/***** Menu 13 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[12].numOptions = 0 ;
	menuSystem[12].displayObject = 0 ;
	menuSystem[12].displayMenu = ShowHiScores ;	
	menuSystem[12].prevMenu = BEST_TIMES_MENU ;	

	/*******************/
	/***** Menu 14 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[13].numOptions = 0 ;
	menuSystem[13].displayObject = 0 ;
	menuSystem[13].displayMenu =  ConfirmSaveGame ;	
	menuSystem[13].prevMenu = OPTION_MENU ;	


	/*******************/
	/***** Menu 15 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[14].numOptions = 0 ;
	menuSystem[14].displayObject = 0 ;
//JJS	menuSystem[14].displayMenu = SerialNetworkHandling ;	
	menuSystem[14].prevMenu = MAIN_MENU;	

	/******************/
	/***** Menu 16 *****/
	/******************/

	/* Menu title details */

	menuSystem[15].titleX = 75 ;
	strcpy (menuSystem[15].menuTitle,"SELECT RACE TYPE") ;
	
	/* Option details */

	menuSystem[15].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[15].menuOptions[0].optionText,"MULTI PLAYER RACE") ;

	menuSystem[15].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[15].menuOptions[1].optionText,"HEAD TO HEAD") ;

	/* Menu details */

	menuSystem[15].numOptions = 2 ;
	menuSystem[15].displayObject = DrawShip ;
	menuSystem[15].displayMenu = 0 ;	
	menuSystem[15].prevMenu = CLASS_MENU ;	


	/*******************/
	/***** Menu 17 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[16].numOptions = 0 ;
	menuSystem[16].displayObject = 0 ;
//	menuSystem[16].displayMenu = StartTwoPlayer ;	
	menuSystem[16].prevMenu = START_TWO_PLAYER_MENU ;	

	/*******************/
	/***** Menu 18 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[17].numOptions = 0 ;
	menuSystem[17].displayObject = 0 ;
	menuSystem[17].displayMenu = 0 ;	
	menuSystem[17].prevMenu = OPTION_MENU ;	

	/*******************/
	/***** Menu 19 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[18].numOptions = 0 ;
	menuSystem[18].displayObject = 0 ;
	menuSystem[18].displayMenu = ConfirmFormat ;	
	menuSystem[18].prevMenu = SAVE_MENU ;	

	/******************/
	/***** Menu 20 *****/
	/******************/

	/* Menu title details */

	menuSystem[19].titleX = 75 ;
	strcpy (menuSystem[19].menuTitle,"CONFIGURE NEGCON") ;

	
	/* Option details */

	menuSystem[19].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[19].menuOptions[0].optionText,"DEFINE BUTTON PRESETS") ;

	menuSystem[19].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[19].menuOptions[1].optionText,"DEFINE NEGCON TWIST") ;

	/* Menu details */

	menuSystem[19].numOptions = 2 ;
	menuSystem[19].displayObject = DisplayNegConMenu ;
	menuSystem[19].displayMenu = 0 ;	
	menuSystem[19].prevMenu = OPTION_MENU ;	

	/*******************/
	/***** Menu 21 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[20].numOptions = 0 ;
	menuSystem[20].displayObject = DrawNegCon ;
	menuSystem[20].displayMenu = 0 ;	
	menuSystem[20].prevMenu = NEGCON_MENU ;	


	/******************/
	/***** Menu 22 *****/
	/******************/

	/* Menu title details */

	menuSystem[21].titleX = 75 ;
	strcpy (menuSystem[21].menuTitle,"VIEW BEST TIMES") ;
	
	/* Option details */

	menuSystem[21].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[21].menuOptions[0].optionText,"VIEW BEST RACE TIMES") ;

	menuSystem[21].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[21].menuOptions[1].optionText,"VIEW BEST TIME TRIAL TIMES") ;

	/* Menu details */

	menuSystem[21].numOptions = 2 ;
	menuSystem[21].displayObject = DrawHiScoreMenu ;
	menuSystem[21].displayMenu = 0 ;	
	menuSystem[21].prevMenu = OPTION_MENU ;	

	/*******************/
	/***** Menu 23 *****/
	/*******************/
	
	/* Option details */

	/* Menu details */

	menuSystem[22].numOptions = 0 ;
	menuSystem[22].displayObject = 0 ;
	menuSystem[22].displayMenu = ShowTimeTrialScores ;	
	menuSystem[22].prevMenu = BEST_TIMES_MENU ;	

	// set what we want to call when option selected

	/*******************/
	/***** Menu 24 *****/
	/*******************/
	
	menuSystem[23].numOptions = 0 ;
	menuSystem[23].displayObject = 0 ;
	menuSystem[23].displayMenu = QuitYesNo ;	
	menuSystem[23].prevMenu = MAIN_MENU ;	

	/*******************/
	/***** Menu 25 *****/
	/*******************/
	
	menuSystem[24].numOptions = 0 ;
	menuSystem[24].displayObject = DrawGraphicDetail;
	menuSystem[24].displayMenu = GraphicOptions ;
	menuSystem[24].prevMenu = OPTION_MENU ;	

	/*******************/
	/***** Menu 26 *****/
	/*******************/
	
	menuSystem[25].numOptions = 0 ;
	menuSystem[25].displayObject = DrawControllerMenu ;
	menuSystem[25].displayMenu = ControllerOptions ;	
	menuSystem[25].prevMenu = OPTION_MENU ;	

	/*******************/
	/***** Menu 27 *****/
	/*******************/
	
	menuSystem[26].numOptions = 0 ;
	menuSystem[26].displayObject = DrawControllerEdit ;
	menuSystem[26].displayMenu = ControllerEdit ;	
	menuSystem[26].prevMenu = CONTROLLER_MENU ;	

	/*******************/
	/***** Menu 28 *****/
	/*******************/
	
	menuSystem[27].titleX = 85 ;
	strcpy (menuSystem[27].menuTitle,"MULTI PLAYER OPTIONS") ;

	/* Option details */

	menuSystem[27].menuOptions[0].textY = 130 ;
	strcpy (menuSystem[27].menuOptions[0].optionText,"TWO PLAYER SERIAL LINK") ;

	menuSystem[27].menuOptions[1].textY = 130 ;
	strcpy (menuSystem[27].menuOptions[1].optionText,"NETWORK") ;

	menuSystem[27].numOptions = 2 ;
	menuSystem[27].displayObject = 0 ;
	menuSystem[27].displayMenu = 0 ;	
	menuSystem[27].prevMenu = MAIN_MENU ;	

	/*******************/
	/***** Menu 29 *****/
	/*******************/
	
	menuSystem[28].numOptions = 0 ;
	menuSystem[28].displayObject = 0 ;
//	menuSystem[28].displayMenu = NetworkOptions ;	
	menuSystem[28].prevMenu = MULTI_MENU ;	

	/*******************/
	/***** Menu 30 *****/
	/*******************/

	menuSystem[29].numOptions = 0 ;
	menuSystem[29].displayObject = 0 ;
//	menuSystem[29].displayMenu = SerialOptions;	
	menuSystem[29].prevMenu = MULTI_MENU;	

}



void ShowMenuBackground (POLY_FT4 *menuPrim, POLY_FT4 *menu1Prim, TimData *timSize)
{
	// THIS IS BODGED SO THAT WE GET A SINGLE PIXEL WHERE STEVE CLIPS THEM 'CAUSE I COULDN'T FIX IT (CRAP HUH)
	char	tpageOffset = 32 ;
	/* Display 16 bit image but split over two primitives, lower half of VRAM */
 
	SetPolyFT4(&(menuPrim[CurrentScreen]));
	menuPrim[CurrentScreen].code = 0x2d ;
 	setXY4(&(menuPrim[CurrentScreen]),0,0,timSize->textureW/2,0,0,timSize->textureH,timSize->textureW/2,timSize->textureH);
 	setUV4(&(menuPrim[CurrentScreen]),0,0,timSize->textureW/2,0,0,timSize->textureH-1,timSize->textureW/2,timSize->textureH-1);
	setTPage(&(menuPrim[CurrentScreen]),2,0,timSize->textureX,timSize->textureY);

	menuPrim[CurrentScreen].r0=230;
	menuPrim[CurrentScreen].g0=0;
	menuPrim[CurrentScreen].b0=BLACK_NOT_DRAWN;
	AddPrim(OT[CurrentScreen] + 4095, (ulong *)&(menuPrim[CurrentScreen]));
	
	SetPolyFT4(&(menu1Prim[CurrentScreen]));
	menu1Prim[CurrentScreen].code = 0x2d ;

	setXY4(&(menu1Prim[CurrentScreen]),timSize->textureW/2,0,timSize->textureW,0,timSize->textureW/2,timSize->textureH,timSize->textureW,timSize->textureH);	  
 	setUV4(&(menuPrim[CurrentScreen]),0,0,timSize->textureW/2,0,0,timSize->textureH-1,timSize->textureW/2,timSize->textureH-1);
//setUV4(&(menu1Prim[CurrentScreen]),tpageOffset,0,(timSize->textureW/2)+tpageOffset,0,tpageOffset,timSize->textureH-1,(timSize->textureW/2)+tpageOffset,timSize->textureH-1);

	setTPage(&(menu1Prim[CurrentScreen]),2,0,timSize->textureX+128,timSize->textureY);

	menu1Prim[CurrentScreen].r0=230;
	menu1Prim[CurrentScreen].g0=0;
	menu1Prim[CurrentScreen].b0=BLACK_NOT_DRAWN;
	AddPrim(OT[CurrentScreen] + 4095, (ulong *) &(menu1Prim[CurrentScreen]));
}

#ifndef USE_ATI


void LoadModel (Object** modelShapes, char *fileName)
{
	short 	modelTextures=0;
	Object* 	obj;
	int 		count = 0 ;
	char 		modelFile[256];

	strcpy( modelFile, "wipeout\\common\\" );
	strcat( modelFile, fileName) ;
	strcat( modelFile, ".prm");

	modelShapes[0] = (Object *)LoadPrm( modelFile, modelTextures );

	obj = modelShapes[0] ;
	while ( obj )
	{
		obj->skeleton = NewSkeleton();

		modelShapes[count] = obj;

		count ++ ;

		obj = obj->next;
	}
}
#endif //USE_ATI
  
void LoadTexModel (Object** modelShapes, char *fileName)
{
	short 	modelTextures= 0;
	Object* 	obj;
	int 		count = 0 ;
	TIMlist  *timPtr;
	char 		modelFile[256];

	/******************************************************/
	/** Load compressed texture files, first to allow    **/
	/** temporary memory to be allocated					  **/
	/******************************************************/

	strcpy( modelFile, "wipeout\\common\\" );
	strcat( modelFile, fileName) ;
	strcat( modelFile, ".cmp");
  modelTextures = TextureTableCount;
	timPtr = LoadCompressedTextureSequence( modelFile );
  LoadCmpFiles( timPtr ); 

	strcpy( modelFile, "wipeout\\common\\" );
	strcat( modelFile, fileName) ;
	strcat( modelFile, ".prm");

	modelShapes[0] = (Object *)LoadPrm( modelFile, modelTextures );

	obj = modelShapes[0] ;

	while ( obj )
	{
		obj->skeleton = NewSkeleton();
		modelShapes[count] = obj;
		count ++ ;
		obj = obj->next;
	}
}

#ifndef USE_ATI


void LoadNegConTexModel (Object** modelShapes, char *fileName)
{
	short 	modelTextures= 0;
	Object* 	obj;
	int 		count = 0 ;
	TIMlist  *timPtr;
	char 		modelFile[256];

	/******************************************************/
	/** Load compressed texture files, first to allow    **/
	/** temporary memory to be allocated					  **/
	/******************************************************/

	strcpy( modelFile, "wipeout\\common\\" );
	strcat( modelFile, fileName) ;
	strcat( modelFile, ".cmp");
  modelTextures = TextureTableCount;
	timPtr = LoadCompressedTextureSequence( modelFile );
  LoadCmpFiles( timPtr ); 

	strcpy( modelFile, "wipeout\\common\\" );
	strcat( modelFile, fileName) ;
	strcat( modelFile, ".prm");

	modelShapes[0] = (Object *)LoadPrm( modelFile, modelTextures );

	root = NewSkeleton();

	obj = modelShapes[0] ;

	while ( obj )
	{
		obj->skeleton = NewSkeleton();
		obj->skeleton->super = root ;
		modelShapes[count] = obj;
		count ++ ;
		obj = obj->next;
	}
}
#endif //USE_ATI

void DrawMainMenu (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static int heading = 0, hdg = 0 ;
	andybodge=1;

	if (shapeNo == 0)
		DrawTeamShips (0, 0, MODELSIZE, &heading, menuSystem[(*currMenu)].menuModel[shipShapeOrder[0]]) ;

//	if (shapeNo == 1)
//	{
//		DrawTeamShips (-450, 0, 1100, &heading, menuSystem[(*currMenu)].menuModel[shipShapeOrder[3]]) ;
//		DrawTeamShips (450, 0, 1100, &heading, menuSystem[(*currMenu)].menuModel[shipShapeOrder[1]]) ;
//	}		

	if (shapeNo == 1)
	{
		SetSkeletonPosition(menuSystem[(*currMenu)].menu2Model[3]->skeleton,	0,	0,	MODELSIZE) ;	 /*1800*/
		SetSkeletonDirectionHPR(menuSystem[(*currMenu)].menu2Model[3]->skeleton,hdg,0,0);
		TransformStaticMenuObject(menuSystem[(*currMenu)].menu2Model[3], NULL);
		hdg += 40;
	}	

	//if (shapeNo ==2)
	//{
		//SetSkeletonPosition(menuSystem[(*currMenu)].menu2Model[1]->skeleton,	0,	0,	MODELSIZE) ;	 /*1800*/
	 	//SetSkeletonDirectionHPR(menuSystem[(*currMenu)].menu2Model[1]->skeleton,hdg,0,0);
		//TransformStaticMenuObject(menuSystem[(*currMenu)].menu2Model[1], NULL);
		//hdg += 40;
	//}//SJR-msdos

}

void DrawAudio (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0;

	//if ((*swapDelay) == 0)
	//{
		//SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[4]->skeleton),50,0,	500) ;
		//SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[4]->skeleton),hdg,	0,	0);
	  	//TransformStaticObjectHole(menuSystem[(*currMenu)].menuModel[4], NULL);
	//}//SJR
	hdg += 40; 
}

void DrawTeams (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0 ;
	static heading = 0 ;

	SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[teamShapeOrder[shapeNo]]->skeleton),0,0,9000) ; 
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[teamShapeOrder[shapeNo]]->skeleton),	hdg,0,0);
 	TransformStaticObject(menuSystem[(*currMenu)].menuModel[teamShapeOrder[shapeNo]], NULL);

	hdg += 40;

	DrawTeamShips (-900, 800, 1500, &heading, menuSystem[(*currMenu)].menu2Model[shipShapeOrder[shapeNo*2]]) ;
	DrawTeamShips (900, 800, 1500, &heading, menuSystem[(*currMenu)].menu2Model[shipShapeOrder[shapeNo*2+1]]) ;
}

void DrawTeamShips (int x, int y, int size, int *heading, Object *shipModel) 
{
	SetSkeletonPosition(shipModel->skeleton,x,y,	size) ;	
	SetSkeletonDirectionHPR(shipModel->skeleton,	*heading,0,0);

	TransformStaticMenuObject(shipModel, NULL);

	*heading -= 10;
}

void DrawPilot (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;

	SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[pilotShapeOrder[shapeNo]]->skeleton),	0,	-400,	11000) ;	
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[pilotShapeOrder[shapeNo]]->skeleton),hdg,0,0);
 	TransformStaticObject(menuSystem[(*currMenu)].menuModel[pilotShapeOrder[shapeNo]], NULL);

	DrawTeamShips (0, 700, 1300, &shdg, menuSystem[(*currMenu)].menu2Model[shipShapeOrder[shapeNo]]) ;

	hdg += 40;
}

void DrawPlayPad (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;
	static pitch = 0, roll = 0, zoom = 0, x = 0, y = 0 ;

	InitMenuDisplay (120) ;

	SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[0]->skeleton),0,	-1140,6220+zoomInOut);
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[0]->skeleton),(modelHdg),0,	0);
 	TransformStaticObject(menuSystem[(*currMenu)].menuModel[0], NULL);
	SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[1]->skeleton),0,	2260+downUp,6670) ;
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[1]->skeleton),250,	-3440,10);
 	TransformStaticObject(menuSystem[(*currMenu)].menuModel[1], NULL);

	if (pad & PadUp0)			pitch += 10 ;
	if (pad & PadDown0)		pitch -= 10 ;
	if (pad & PadLeft0)		hdg -= 10 ;
	if (pad & PadRight0)		hdg += 10 ;
	if (pad & PadSquare0)	roll -= 10 ;
	if (pad & PadCircle0)	roll += 10 ;
	if (pad & PadTopLeft1)	zoom += 10 ;
	if (pad & PadTopRight1)	zoom -= 10 ;
	if (pad & PadTopLeft2) 	x -= 10 ;
	if (pad & PadTopRight2)	x += 10 ;
	if (pad & PadStart)	  	y += 10 ;
	if (pad & PadSelect)	  	y -= 10 ;
}

void DrawNegCon (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;
	int	rotation = 0 ;
	int	dispRot = 0 ;

	Object*	a;
	Object*	b;

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}

	CentreText ("NEGCON CONFIGURATION ",text_data (60, 21, 8),RedText) ;

	a = menuSystem[(*currMenu)].menuModel[0];
	b = menuSystem[(*currMenu)].menuModel[1];

	SetSkeletonPosition(root,0,150,1100) ;	 
	SetSkeletonDirectionHPR(root,	1024,	0,	0);

	SetSkeletonPosition( a->skeleton, 0, 0, -8 ); 
	SetSkeletonPosition( b->skeleton, 0, 0, 8 );	 

	twist = neg1.twist ;

	if (twist <= 128)
		rotation = 	 180 - ((((4096*180)/128) * twist) >> 12) ;
	else
		rotation =  (((4096*180)/128) * (twist - 128)) >> 12 ;

	AddText ("ANGLE",text_data (120, 60, 8),RedText) ; 
	AddNumber (rotation,text_data (190, 60, 8),RedText) ;

	CentreText ("DEFINE MAXIMUM LOCK",text_data (120, 180, 8),RedText) ;
	if (negtwist == 0)
	{
		AddText ("DEFAULT",text_data (100, 195, 8),RedText) ;
		AddNumber (178,text_data (190, 195, 8),RedText) ;
	}
	else
	{
		if (negtwist <= 128)
			dispRot = 	 180 - ((((4096*180)/128) * negtwist) >> 12) ;
		else
			dispRot =  (((4096*180)/128) * (negtwist - 128)) >> 12 ;

		AddText ("FULL LOCK IS",text_data (90, 195, 8),RedText) ;
		AddNumber (dispRot,text_data (210, 195, 8),RedText) ;
	}

	SetSkeletonDirection( a->skeleton, 0, 0, ((neg1.twist-128)<<3) );
	SetSkeletonDirection( b->skeleton, 0, 0, -((neg1.twist-128)<<3) );

	a->skeleton->update = 1 ;
	b->skeleton->update = 1 ;

 	TransformStaticObject(a, NULL);
 	TransformStaticObject(b, NULL);
	
	hdg += 40;
}

void NegConButtonDisplay (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;

	Object*	a;
	Object*	b;

	a = menuSystem[(*currMenu)].menuModel[0];
	b = menuSystem[(*currMenu)].menuModel[1];

	SetSkeletonPosition(root,0,100,900+zoomInOut) ;	 
	SetSkeletonDirectionHPR(root,	1024+modelHdg,	0,	0);

	SetSkeletonPosition( a->skeleton, 0, 0, -8 );  
	SetSkeletonPosition( b->skeleton, 0, 0, 8 );	  

	SetSkeletonDirection( a->skeleton, 0, 0, 0);
	SetSkeletonDirection( b->skeleton, 0, 0, 0);

	a->skeleton->update = 1 ;
	b->skeleton->update = 1 ;

 	TransformStaticObject(a, NULL);
 	TransformStaticObject(b, NULL);
}

void DrawShip(Menus *menuSystem, int *currMenu, int shipNo, int *swapDelay)
{
	static hdg = 0;

	SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[shipShapeOrder[shipNo]]->skeleton),0,	0,	MODELSIZE) ;
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[shipShapeOrder[shipNo]]->skeleton),hdg,0,0);
 	TransformStaticObject(menuSystem[(*currMenu)].menuModel[shipShapeOrder[shipNo]], NULL);
	hdg += 10;
}

void DrawClass (Menus *menuSystem, int *currMenu, int shipNo, int *swapDelay)
{
	static hdg = 0;

	if (shipNo==0)
	{
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[0]->skeleton),	0,	0,500);
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[0]->skeleton),hdg,0,0);
 		TransformStaticObject(menuSystem[(*currMenu)].menuModel[0], NULL);
	}
	
	if (shipNo==1)
	{
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[1]->skeleton),	0,	-25,500);
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[1]->skeleton),hdg,0,0);
 		TransformStaticObject(menuSystem[(*currMenu)].menuModel[1], NULL);
	}

	hdg += 40;
}

void InitSelectData (SelectionData *selectData) 
{
	selectData->slotRow = 0 ;
	selectData->optionNo = 0 ;
	selectData->slotColumn = 0 ;
	selectData->slotsFilled = 0 ;
	selectData->select = 0 ;
	selectData->firstTime = 1 ;
	selectData->numCardFiles = 0 ;
	selectData->hiscoreTrack = 0 ;
	selectData->speechMode = 0 ;
	selectData->toggle = 0 ;
	selectData->cardNum = 0 ;
	selectData->card1 = FORMATTED ;
	selectData->card2 = FORMATTED ;
	selectData->formatCard = 0 ;
	selectData->formatConfirm = 0 ;
}


void InitMusicVSfx (SlideTextures *musicTex)
{
	musicTex->slideTex = LoadTexture ( "wipeout\\textures\\slider1.tim", 1 ) ; /*slide*/
	musicTex->sliderTex = LoadTexture ( "wipeout\\textures\\slide1.tim", 1 ) ;
}


void MusicVSfx (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{																						   
	int 	slidePosx = 48 ;
	int 	slidePosy = 85 ;
	int	slideWidth = 224 ;
	int	slideHeight = 28 ;		 /*28*/
	int	sliderLeftOffset = 0 ;
	int	sliderRightOffset = 24 ;
	int	sliderRange = 218 ;

	static int	ShieldOn = 0;

	static int	toggle = 0 ;

	extern	short	CurrTrkNo;

	if (xpad & PadUp0)
	{
		PlayNote(MENU_MOVE, 1, 2);	  
		selectData->optionNo -- ;
		if (selectData->optionNo < 0)
			selectData->optionNo = 1 ;
	}

	if (xpad & PadDown0)
	{
		PlayNote(MENU_MOVE, 1, 2);	 
		selectData->optionNo ++ ;
		if (selectData->optionNo > 1)
			selectData->optionNo = 0 ;
	}

	AddText ("AUDIO CONFIGURATION",text_data (60, 20, 8),RedText) ;

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}

/*	Slider overlay knob */
	SetPolyFT4(&(musicPrims->sliderPrim));			
	musicPrims->sliderPrim.r0=230; // PCwipeout
	musicPrims->sliderPrim.g0=0;
	musicPrims->sliderPrim.b0=BLACK_NOT_DRAWN;
  	
	setXY4(&(musicPrims->sliderPrim), (slidePosx-9) + sliderLeftOffset + gameData->oldSliderPos, slidePosy - 24,
										  	  (slidePosx-9) + sliderRightOffset + gameData->oldSliderPos, slidePosy - 24,
										  	  (slidePosx-9) + sliderLeftOffset + gameData->oldSliderPos, slidePosy + 52,
										  	  (slidePosx-9) + sliderRightOffset + gameData->oldSliderPos, slidePosy + 52) ;
  	
 	setUV4(&(musicPrims->sliderPrim), musicTex->sliderTex->u0, musicTex->sliderTex->v0, 
												musicTex->sliderTex->u1+1, musicTex->sliderTex->v1, 
												musicTex->sliderTex->u2, musicTex->sliderTex->v2+1, 
												musicTex->sliderTex->u3+1, musicTex->sliderTex->v3+1) ;

	setTPage(&(musicPrims->sliderPrim),0,0,musicTex->sliderTex->textureX,musicTex->sliderTex->textureY);

	musicPrims->sliderPrim.clut = musicTex->sliderTex->cba;
 
	AddPrim( OT[ CurrentScreen ] , ( ulong* ) &(musicPrims->sliderPrim) );

/*	Slider background */
	SetPolyFT4(&(musicPrims->slidePrim));			
	musicPrims->slidePrim.r0=230;	// PCwipeout
	musicPrims->slidePrim.g0=0;
	musicPrims->slidePrim.b0=BLACK_NOT_DRAWN;

	setXY4(&(musicPrims->slidePrim), slidePosx, slidePosy,
					  	  slidePosx + slideWidth, slidePosy,
					  	  slidePosx, slidePosy + slideHeight,
					  	  slidePosx + slideWidth, slidePosy + slideHeight) ;

	setUV4(&(musicPrims->slidePrim),	musicTex->slideTex->u0, musicTex->slideTex->v0, 
						musicTex->slideTex->u1, musicTex->slideTex->v1, 
						musicTex->slideTex->u2, musicTex->slideTex->v2+1, 
						musicTex->slideTex->u3, musicTex->slideTex->v3+1) ;

	setTPage(&(musicPrims->slidePrim),0,0,musicTex->slideTex->textureX,musicTex->slideTex->textureY);

	musicPrims->slidePrim.clut = musicTex->slideTex->cba;
 
	AddPrim( OT[ CurrentScreen ] , ( ulong* ) &(musicPrims->slidePrim) );

	AddText (GetCdTrackName(gameData->oldCdTrack-1), text_data (130, 175, 8), RedText) ; 

	if (selectData->optionNo == 0)
	{
#if !USE_ATI
		if ((toggle % 2) == 0)
		{
#endif
			AddText ("SFX",text_data (33, 51, 8),RedText) ;
			AddText ("MUSIC",text_data (250, 139, 8),RedText) ;
#if !USE_ATI
		}
		else
		{
			AddText ("SFX",text_data (33, 51, 8),WhiteText) ;
			AddText ("MUSIC",text_data (250, 139, 8),WhiteText) ;
		}
#endif

		if (pad & PadLeft0)
		{
			andyTimeout = 0;
			if (!ShieldOn)
			{
				PlayNote(SHIELDS,0,-1);
				ShieldOn = 1;
			}
			chanTable[vagTable[SHIELDS].channelID].s_attr.volume.left = 0x3fff - (75 * gameData->oldSliderPos);
			chanTable[vagTable[SHIELDS].channelID].s_attr.volume.right = 0x3fff - (75 * gameData->oldSliderPos);
			SpuSetVoiceAttr(&(chanTable[vagTable[SHIELDS].channelID].s_attr));
			gameData->oldSliderPos -- ;
			if (gameData->oldSliderPos < 0)
				gameData->oldSliderPos = 0 ;
			SfxSetVolume((char)((0x3fff - (75 * gameData->oldSliderPos))>>6));
		globalvol = 218 - gameData->oldSliderPos;
			CdSetVol(gameData->oldSliderPos);
			SfxSetVolume((char)((0x3fff - (75 * gameData->oldSliderPos))>>6));
		}
		else if (pad & PadRight0)
		{
			andyTimeout = 0;
			if (!ShieldOn)
			{
				PlayNote(SHIELDS,0,-1);
				ShieldOn = 1;
			}
			chanTable[vagTable[SHIELDS].channelID].s_attr.volume.left = 0x3fff - (75 * gameData->oldSliderPos);
			chanTable[vagTable[SHIELDS].channelID].s_attr.volume.right = 0x3fff - (75 * gameData->oldSliderPos);
			SpuSetVoiceAttr(&(chanTable[vagTable[SHIELDS].channelID].s_attr));
			gameData->oldSliderPos ++ ;
			if (gameData->oldSliderPos > sliderRange)
				gameData->oldSliderPos = sliderRange ;
		globalvol = 218 - gameData->oldSliderPos;
			CdSetVol(gameData->oldSliderPos);
			SfxSetVolume((char)((0x3fff - (75 * gameData->oldSliderPos))>>6));
#ifdef PORT_SOUND
			SfxSetVolume((char)((0x3fff - (75 * gameData->oldSliderPos))>>6));
#endif
		}
		else
		{
			if (ShieldOn)
			{
				KillNote(SHIELDS);
				ShieldOn = 0;
			}
			chanTable[vagTable[SHIELDS].channelID].s_attr.volume.left = 0;
			chanTable[vagTable[SHIELDS].channelID].s_attr.volume.right = 0;
			SpuSetVoiceAttr(&(chanTable[vagTable[SHIELDS].channelID].s_attr));
		}

		globalvol = 218 - gameData->oldSliderPos;
	CdSetVol(gameData->oldSliderPos);
   	c_attr.cd.volume.left  = 150 * gameData->oldSliderPos;	/* Master volume (left) */
   	c_attr.cd.volume.right  = 150 * gameData->oldSliderPos;	/* Master volume (right) */
   	SpuSetCommonAttr (&c_attr);
#ifdef PORT_SOUND
    SfxSetCdVolume(ConvCDVol(c_attr.cd.volume.left,c_attr.cd.volume.right));
#endif
	}
	else
	{
			AddText ("SFX",text_data (33, 51, 8),WhiteText) ;
			AddText ("MUSIC",text_data (250, 139, 8),WhiteText) ;
	}

	if (selectData->optionNo == 1)
	{
#if !USE_ATI
		if ((toggle % 2) == 0)
#endif
			AddText ("SELECT CD" , text_data (15, 175, 8), RedText) ; 
#if !USE_ATI
		else
			AddText ("SELECT CD" , text_data (15, 175, 8), WhiteText) ; 
#endif
		if (ShieldOn)
		{
			KillNote(SHIELDS);
			ShieldOn = 0;
		}
		if (xpad & PadLeft0)
		{
			andyTimeout = 0;
			PlayNote(MENU_MOVE, 1, 2);

			if (gameData->oldCdTrack == starttrack)
				gameData->oldCdTrack = 1;
			else
				gameData->oldCdTrack -- ;

			if (gameData->oldCdTrack < 1)
				gameData->oldCdTrack = endtrack ;



			CdControlB(CdlStop, 0, 0);  

			if(gameData->oldCdTrack != 1)
			{
//		  		CdControlB(CdlSetloc, (u_char *)&(loc[gameData->oldCdTrack]), 0);	
				CurrTrkNo = gameData->oldCdTrack;
				CdControlB(CdlPlay, 0, 0);
			}  
			else
			{
				CurrTrkNo = (randy() % (endtrack - 1)) + 2;
				CdControlB(CdlPlay, 0, 0);
			}
		}
	   
		if (xpad & PadRight0)
		{
			andyTimeout = 0;
			PlayNote(MENU_MOVE, 1, 2);

			if (gameData->oldCdTrack == 1)
				gameData->oldCdTrack = starttrack;
			else
				gameData->oldCdTrack ++ ;

			if (gameData->oldCdTrack > endtrack)
				gameData->oldCdTrack = 1 ;


			CdControlB(CdlStop, 0, 0);  

			if(gameData->oldCdTrack != 1)
			{
//		  		CdControlB(CdlSetloc, (u_char *)&(loc[gameData->oldCdTrack]), 0);
				CurrTrkNo = gameData->oldCdTrack;
				CdControlB(CdlPlay, 0, 0);
			}  
			else
			{
				CurrTrkNo = (randy() % (endtrack - 1)) + 2;
				CdControlB(CdlPlay, 0, 0);
			}
				
		}
	}
	else
	{
		AddText ("SELECT CD" , text_data (15, 175, 8), WhiteText) ; 
	}

	toggle ++ ;
}

void InitLoadSave (char loadGames[][9])
{
	short i ;

	for (i=0; i < MAX_GAMES; i++)
		loadGames[i][0] = 0 ;
}

#ifndef USE_ATI


void LoadGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	char				spacing = 12 ;
	char				barSpaceY = 12, barSpaceX = 150 ;
	short				lineNum = 0, i ;
	short				startOption, dispOption ;
	int				cardStatus, slotStatus ;
	static int		toggle = 0 ;			
	static char		prevCard = 0 ;

	slotStatus = GetCardInfo (selectData->cardNum) ;

	switch (slotStatus)
	{
	 	case 0:
					if (prevCard != selectData->cardNum)
					{
						ClearEventHW () ;
//						if (selectData->cardNum == 0)
//							_card_clear (selectData->cardNum) ; // PCwipeout
//						else
//							_card_clear (16) ; // PCwipeout

						cardStatus = CheckHWCardEvents () ;
						LoadCard (selectData->cardNum) ;
						cardStatus = CardContentStatus () ;

						if (cardStatus == 0)
						{
							if (selectData->cardNum == 0)
								selectData->card1 = FORMATTED ;
							else
								selectData->card2 = FORMATTED ;

						 	/* FORMATTED */
						}

						if (cardStatus == 3)
						{
							if (selectData->cardNum == 0)
								selectData->card1 = UNFORMATTED ;
							else
								selectData->card2 = UNFORMATTED ;
						}

					}					
					break ;					 /* CARD	EXISTS */

	 	case 1:	break ;					 /* ERROR */

	 	case 2:	selectData->slotRow = 0 ;
					selectData->slotColumn = 0 ;
					selectData->slotsFilled = 0 ;
					InitLoadSave (&(loadGames[0])) ;
					gameData->cardPresent = 1 ;
					break ;					 /* NO CARD */

	 	case 3:	ClearEventHW () ;
//					if (selectData->cardNum	== 0)
//						_card_clear (0) ; // PCwipeout
//					else
//						_card_clear (16) ; // PCwipeout

					cardStatus = CheckHWCardEvents () ;
					
					LoadCard (selectData->cardNum) ;
					cardStatus = CardContentStatus () ;

					switch (cardStatus)
					{
					 	case 0:	if (slotStatus == 3)
									{
										selectData->slotRow = 0 ;
										selectData->slotColumn = 0 ;
										selectData->slotsFilled = 0 ;
			
										LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum) ;

										if ( !LoadCardData ( loadGames[(selectData->slotRow*2)+selectData->slotColumn] , selectData->cardNum, gameData, selectData ))
										{
											printf ("LOAD ERROR \n") ;
											printf ("in here \n") ;
										}
									}
				
									if (selectData->cardNum == 0)
										selectData->card1 = FORMATTED ;
									else
										selectData->card2 = FORMATTED ;

									break ;					 /* FORMATTED */

					 	case 1:	break ;					 /* ERROR */
					 	case 2:	printf ("NO CARD \n") ;
									break ;					 /* NO CARD */
					 	case 3:	printf ("UNFORMATTED \n") ;
									if (selectData->cardNum == 0)
										selectData->card1 = UNFORMATTED ;
									else
										selectData->card2 = UNFORMATTED ;
									break ;					 /* UNFORMATTED */
					}
					break ;					 /* NEW CARD */
	}
	
	if (gameData->cardPresent == 1)
		CentreText ("NO CARD PRESENT", text_data (70, 116, 8), RedText) ; 
	else
	{
		if (selectData->slotsFilled == 0)
			CentreText ("NO SAVED GAMES", text_data (70, 116, 8), RedText) ; 

		if (((selectData->card1 == UNFORMATTED) && (selectData->cardNum == 0))
				|| ((selectData->card2 == UNFORMATTED) && (selectData->cardNum == 1)))
				CentreText ("CARD UNFORMATTED", text_data (70, 128, 8), RedText) ; 
	}

	gameData->cardPresent = 0 ;

	if (GetMenuKey(M_LEFT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->slotColumn -- ;
		if (selectData->slotColumn < 0)
		{
			selectData->slotColumn = 0 ;
		}
	}

	if (GetMenuKey(M_RIGHT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->slotColumn ++ ;
		if (selectData->slotColumn > 1)
		{
			selectData->slotColumn = 1 ;
		}
	}

	if (GetMenuKey(M_UP))
	{
		selectData->slotRow -- ;
		if (selectData->slotRow < 0)
			selectData->slotRow = ((selectData->slotsFilled-1) / 2) ;
	}

	if (GetMenuKey(M_DOWN))
	{
		selectData->slotRow ++ ;
		if (selectData->slotRow > ((selectData->slotsFilled-1) / 2))
			selectData->slotRow = 0 ;
	}

	if (GetMenuKey(M_START))
	{
	 	selectData->formatCard = 1 ;
	 	selectData->calledMenu = LOAD_MENU ;
	}

	prevCard = selectData->cardNum ;


	if (GetMenuKey(M_QUIT))
	{
		selectData->cardNum  ^= 1 ;
		selectData->slotRow = 0 ;
		selectData->slotColumn = 0 ;
		selectData->slotsFilled = 0 ;
			
		LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum) ;

		if ( !LoadCardData ( loadGames[(selectData->slotRow*2)+selectData->slotColumn] , selectData->cardNum, gameData, selectData ))
		{
			printf ("LOAD ERROR \n") ;
		}
	}

	CentreText ("LOAD GAME FROM DISK", text_data (70, 20, 8), RedText) ; 
	AddText ("SELECT A SLOT", text_data (95, 60, 8), WhiteText) ; 
 	
	if (neg1.head == 8960)
	{
		DisplayButtons ( 20, 210, NSELECT) ;

		DisplayButtons ( 20, 40, NFORMAT) ;
		DisplayButtons ( 185, 40, NCHANGE) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 20, 210, PSELECT) ;

		DisplayButtons ( 20, 40, PFORMAT) ;
		DisplayButtons ( 185, 40, PCHANGE) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}
#if !USE_ATI
	if ((toggle %2) == 0)
	{
#endif
		CentreText ("CARD", text_data (70, 210, 8), RedText) ; 

		if (selectData->cardNum == 0)
			AddNumber (1, text_data (200, 210, 8), RedText) ; 
		else
			AddNumber (2, text_data (200, 210, 8), RedText) ; 
#if !USE_ATI
	}
	else
	{
		CentreText ("CARD", text_data (70, 210, 8), WhiteText) ; 

		if (selectData->cardNum == 0)
			AddNumber (1, text_data (200, 210, 8), WhiteText) ; 
		else
			AddNumber (2, text_data (200, 210, 8), WhiteText) ; 
	}	
#endif	

	if (selectData->slotColumn == 0)
	{
#if !USE_ATI
		if ((toggle % 2) == 0)
#endif
			AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
#if !USE_ATI
		else
			AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
#endif
	}
	else
		if ((selectData->slotsFilled/2) == selectData->slotRow && (selectData->slotsFilled % 2) != 0)
		{
#if !USE_ATI
			if ((toggle % 2) == 0)
			{
#endif
				AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
				selectData->slotColumn = 0 ;
#if !USE_ATI
			}
			else
			{
				AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
				selectData->slotColumn = 0 ;
			}
#endif
		}
		else
		{
#if !USE_ATI
			if ((toggle % 2) == 0)
			{
#endif
				AddText (&(loadGames[(selectData->slotRow*2)+1][0]), text_data (200, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
#if !USE_ATI
			}
			else
			{
				AddText (&(loadGames[(selectData->slotRow*2)+1][0]), text_data (200, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
			}
#endif
		}


	startOption = ((selectData->slotRow*2) / 20) * 20 ;
	
	dispOption = selectData->slotsFilled - startOption ;

	if  (dispOption >= 20)
		dispOption = 20 ;
	
	for (i=startOption; i < (startOption + dispOption) ; i++)
	{

		if (((i+1) % 2) == 0 )
		{
			AddText (&(loadGames[i][0]), text_data (200, 80+(spacing*lineNum), 8), WhiteText) ; 
			AddNumber (i+1, text_data (170, 80+(spacing*lineNum), 8), WhiteText) ; 
			lineNum ++ ;
		}
		else
		{
			AddText (&(loadGames[i][0]), text_data (50, 80+(spacing*lineNum), 8), WhiteText) ; 
			AddNumber (i+1, text_data (20, 80+(spacing*lineNum), 8), WhiteText) ; 	
		}
	}
	toggle ++ ;
}
#endif //USE_ATI

#ifndef USE_ATI



void SaveGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	char				spacing = 12 ;
	char				barSpaceY = 12, barSpaceX = 150 ;
	short				lineNum = 0, i ;
	short				startOption, dispOption ;
	int				slotStatus, cardStatus ;
	static int		toggle = 0 ;
	static char		prevCard = 0 ;
	
	if (selectData->numCardFiles != 15)
		strcpy (loadGames[selectData->slotsFilled], "NEW FILE") ;
	else
		CentreText ("CARD IS FULL", text_data (70, 180, 8), RedText) ; 

	slotStatus = GetCardInfo (selectData->cardNum) ;

	switch (slotStatus)
	{
	 	case 0:	if (prevCard != selectData->cardNum)
					{
						ClearEventHW () ;
//						if (selectData->cardNum == 0)
//							_card_clear (selectData->cardNum) ; // PCwipeout
//						else
//							_card_clear (16) ; // PCwipeout

						cardStatus = CheckHWCardEvents () ;
						LoadCard (selectData->cardNum) ;
						cardStatus = CardContentStatus () ;

						if (cardStatus == 0)
						{
							if (selectData->cardNum == 0)
								selectData->card1 = FORMATTED ;
							else
								selectData->card2 = FORMATTED ;

						 	/* FORMATTED */
						}

						if (cardStatus == 3)
						{
							if (selectData->cardNum == 0)
								selectData->card1 = UNFORMATTED ;
							else
								selectData->card2 = UNFORMATTED ;
						}
					}					
					break ;					 /* CARD	EXISTS */

	 	case 1:	break ;					 /* ERROR */

	 	case 2:	printf ("NO CARD \n") ;

					selectData->slotRow = 0 ;
					selectData->slotColumn = 0 ;
					selectData->slotsFilled = 0 ;
					InitLoadSave (&(loadGames[0])) ;
					gameData->cardPresent = 1 ;

					break ;					 /* NO CARD */

	 	case 3:	ClearEventHW () ;
//					if (selectData->cardNum == 0)
//						_card_clear (selectData->cardNum) ; // PCwipeout
//					else
//						_card_clear (16) ; // PCwipeout

					cardStatus = CheckHWCardEvents () ;
					LoadCard (selectData->cardNum) ;
					cardStatus = CardContentStatus () ;

					switch (cardStatus)
					{
					 	case 0:	if (slotStatus == 3)
									{
										selectData->slotRow = 0 ;
										selectData->slotColumn = 0 ;
										selectData->slotsFilled = 0 ;
			
										LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum) ;

										if ( !LoadCardData ( loadGames[(selectData->slotRow*2)+selectData->slotColumn] , selectData->cardNum, gameData, selectData ))
										{
											printf ("LOAD ERROR \n") ;
											printf ("in here \n") ;

										}
	
									}
									if (selectData->cardNum == 0)
										selectData->card1 = FORMATTED ;
									else
										selectData->card2 = FORMATTED ;

									break ;					 /* FORMATTED */

					 	case 1:	break ;					 /* ERROR */
					 	case 2:	break ;					 /* NO CARD */
					 	case 3:	if (selectData->cardNum == 0)
										selectData->card1 = UNFORMATTED ;
									else
										selectData->card2 = UNFORMATTED ;

									break ;					 /* UNFORMATTED */
					}
					break ;					 /* NEW CARD */
	}
	


	if (GetMenuKey(M_LEFT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->slotColumn -- ;
		if (selectData->slotColumn < 0)
		{
			selectData->slotColumn = 0 ;
		}
	}

	if (GetMenuKey(M_RIGHT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->slotColumn ++ ;
		if (selectData->slotColumn > 1)
		{
			selectData->slotColumn = 1 ;
		}
	}

	if (GetMenuKey(M_UP))
	{
		selectData->slotRow -- ;
		if (selectData->slotRow < 0)
		{
			if ( (selectData->numCardFiles == 15) && (selectData->slotsFilled != 15) && ((selectData->slotsFilled % 2) == 0))
			{
				selectData->slotRow = (((selectData->slotsFilled) / 2) - 1) ;
			}
			else
			{
				selectData->slotRow = ((selectData->slotsFilled) / 2) ;
			}
		}
	}

	if (GetMenuKey(M_DOWN))
	{
		selectData->slotRow ++ ;
		if ( (selectData->numCardFiles == 15) && (selectData->slotsFilled != 15) && ((selectData->slotsFilled % 2) == 0))
		{
			if (selectData->slotRow > ((selectData->slotsFilled) / 2)-1)
				selectData->slotRow = 0 ;
		}
		else
		{
			if (selectData->slotRow > ((selectData->slotsFilled) / 2))
				selectData->slotRow = 0 ;
		}
	}

	if (GetMenuKey(M_START))
	{
	 	selectData->formatCard = 1 ;
	 	selectData->calledMenu = SAVE_MENU ;
	}


	prevCard = selectData->cardNum ;


	if (GetMenuKey(M_QUIT))
	{
		selectData->cardNum  ^= 1 ;
		selectData->slotRow = 0 ;
		selectData->slotColumn = 0 ;
		selectData->slotsFilled = 0 ;

		LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum) ;

		if ( !LoadCardData ( loadGames[(selectData->slotRow*2)+selectData->slotColumn] , selectData->cardNum, gameData, selectData ))
		{
			printf ("LOAD ERROR \n") ;
		}
	}
	

	if (gameData->cardPresent == 1)
		CentreText ("NO CARD PRESENT", text_data (70, 116, 8), RedText) ; 
	else
	{
		if (selectData->slotsFilled == 0)
			CentreText ("NO SAVED GAMES", text_data (70, 116, 8), RedText) ; 

		if (((selectData->card1 == UNFORMATTED) && (selectData->cardNum == 0))
				|| ((selectData->card2 == UNFORMATTED) && (selectData->cardNum == 1)))
				CentreText ("CARD UNFORMATTED", text_data (70, 128, 8), RedText) ; 
	}

	gameData->cardPresent = 0 ;

	CentreText ("OVERWRITE EXISTING FILE", text_data (70, 20, 8), RedText) ; 
	AddText ("SELECT A SLOT", text_data (95, 60, 8), WhiteText) ; 

	if (neg1.head == 8960)
	{
		if ((selectData->slotsFilled == 0) && (selectData->numCardFiles == 15))
		{
		}
		else
		{
			DisplayButtons ( 20, 210, NSELECT) ;
		}

		DisplayButtons ( 20, 40, NFORMAT) ;
		DisplayButtons ( 185, 40, NCHANGE) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		if ((selectData->slotsFilled == 0) && (selectData->numCardFiles == 15))
		{
		}
		else
		{
			DisplayButtons ( 20, 210, PSELECT) ;
		}
		DisplayButtons ( 20, 40, PFORMAT) ;
		DisplayButtons ( 185, 40, PCHANGE) ;

		DisplayButtons ( 240, 210, PBACK) ;
	}

#if !USE_ATI
	if ((toggle %2) == 0)
	{
#endif
		CentreText ("CARD", text_data (70, 210, 8), RedText) ; 

		if (selectData->cardNum == 0)
			AddNumber (1, text_data (200, 210, 8), RedText) ; 
		else
			AddNumber (2, text_data (200, 210, 8), RedText) ; 
#if !USE_ATI
	}
	else
	{
		CentreText ("CARD", text_data (70, 210, 8), WhiteText) ; 

		if (selectData->cardNum == 0)
			AddNumber (1, text_data (200, 210, 8), WhiteText) ; 
		else
			AddNumber (2, text_data (200, 210, 8), WhiteText) ; 
	}
#endif
	if (selectData->slotColumn == 0)
	{
#if !USE_ATI
		if ((toggle % 2) == 0)
#endif
			AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
#if !USE_ATI
		else
			AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
#endif
	}
	else
		if (selectData->numCardFiles == 15)
		{
			if (((selectData->slotsFilled)/2) == selectData->slotRow && ((selectData->slotsFilled) % 2) != 0)
			{
#if !USE_ATI
				if ((toggle % 2) == 0)
				{
#endif
					AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
					selectData->slotColumn = 0 ;
#if !USE_ATI
				}
				else
				{
					AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
					selectData->slotColumn = 0 ;
				}
#endif
			}
			else
			{
#if !USE_ATI
				if ((toggle % 2) == 0)
#endif
					AddText (&(loadGames[(selectData->slotRow*2)+1][0]), text_data (200, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
#if !USE_ATI
				else
					AddText (&(loadGames[(selectData->slotRow*2)+1][0]), text_data (200, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
#endif
			}
		}
		else
		{
			if (((selectData->slotsFilled+1)/2) == selectData->slotRow && ((selectData->slotsFilled+1) % 2) != 0)
			{
#if !USE_ATI
				if ((toggle % 2) == 0)
				{
#endif
					AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
					selectData->slotColumn = 0 ;
#if !USE_ATI
				}
				else
				{
					AddText (&(loadGames[selectData->slotRow*2][0]), text_data (50, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
					selectData->slotColumn = 0 ;
				}
#endif
			}
			else
			{
#if !USE_ATI
				if ((toggle % 2) == 0)
#endif
					AddText (&(loadGames[(selectData->slotRow*2)+1][0]), text_data (200, 80+(spacing*(selectData->slotRow%10)), 8), RedText) ; 
#if !USE_ATI
				else
					AddText (&(loadGames[(selectData->slotRow*2)+1][0]), text_data (200, 80+(spacing*(selectData->slotRow%10)), 8), WhiteText) ; 
#endif
			}
		}


	if (selectData->numCardFiles != 15)
	{
		startOption = ((selectData->slotRow*2) / 15) * 15 ;
	
		dispOption = (selectData->slotsFilled+1) - startOption ;

		if  (dispOption >= 15)
			dispOption = 15 ;
	}
	else
	{
		startOption = 0 ;
		dispOption = selectData->slotsFilled ;
	}
	
	if ((slotStatus != 2) && (cardStatus != 3))
	{
		for (i=startOption; i < (startOption + dispOption) ; i++)
		{
			if (((i+1) % 2) == 0 )
			{
				AddText (&(loadGames[i][0]), text_data (200, 80+(spacing*lineNum), 8), WhiteText) ; 
				AddNumber (i+1, text_data (170, 80+(spacing*lineNum), 8), RedText) ; 
				lineNum ++ ;
			}
			else
			{
				AddText (&(loadGames[i][0]), text_data (50, 80+(spacing*lineNum), 8), WhiteText) ; 
				AddNumber (i+1, text_data (20, 80+(spacing*lineNum), 8), RedText) ; 	
			}
		}
	}
	toggle ++ ;
}
#endif //USE_ATI


/*
void EnterSaveGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static short			charX=40 ;
	char						charY=175 ;
	static char				currChar = 'A' ;
	static signed char 	charVal = 0, numChars = 0 ;
	static char				dispChar[2] ;
	static char				toggleDisplay = 0 ;
	int						i, j ;
	static char				userEntry = 1;
	static char 			padDelay = 0;
	char						outputName[9] ;
	short						select;


	CentreText ("SAVE GAME TO DISK", text_data (70, 20, 8), RedText) ; 
	AddText ("ENTER NAME OF FILE", text_data (65, 60, 8), WhiteText) ; 

	if (selectData->firstTime)
	{
		if ((strcmp (loadGames[(selectData->slotRow*2)+selectData->slotColumn], "NEW FILE")) == 0)
		{
			selectData->nameEntered[0] = 0 ;
			currChar = 'A'	;
			charX = 40 ;
			charVal = 0 ; 
			numChars = 0 ;
		}
		else
		{
			selectData->nameEntered[0] = 0 ;
			currChar = 'A'	;
			charX = 40 ;
			charVal = 0 ; 
			numChars = 0 ;
			i = 0 ;

			while (loadGames[(selectData->slotRow*2)+selectData->slotColumn][i] != 0 )
			{
				currChar = loadGames[(selectData->slotRow*2)+selectData->slotColumn][i] ;
				charVal = currChar - 'A' ;		
				selectData->nameEntered[numChars] = currChar ; 
				selectData->nameEntered[numChars+1] = 0 ;
				numChars ++ ;
				charX += CharSet16[charVal].width ;
				i ++ ;
			}

			if (numChars == 6)
				currChar = ('Z'+2) ; 
			else
				currChar = 'A' ;
			dispChar[0] = currChar ;
			dispChar[1] = 0 ;
		}
		selectData->firstTime = 0 ;
	}

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}

	if (userEntry)
	{

		dispChar[0] = currChar ;
		dispChar[1] = 0 ;

		if (userEntry)
		{
			if ((toggleDisplay % 3) == 0)
			{
				if (charVal == 26 || charVal == 27)
					AddDelEnd ((charVal-26), text_data (charX, charY, 16), WhiteText) ;
				else
					AddText (&(dispChar[0]), text_data (charX, charY, 16), WhiteText) ;
			}
			else
			{
				if (charVal == 26 || charVal == 27)
					AddDelEnd ((charVal-26), text_data (charX, charY, 16), RedText) ;
				else
					AddText (&(dispChar[0]), text_data (charX, charY, 16), RedText) ;
			}
		}
   								
		charVal = currChar - 'A' ;		

		if ((pad & PadRight0) && (padDelay == 0) )
		{
			PlayNote(MENU_MOVE, 1, 20);
			padDelay = 5; 
			charVal ++ ;
			if (charVal > 27)	 // 25
			{
				if (numChars != 6)
		 			charVal = 0 ;
				else
					charVal = 26 ;
			}		
		}
		
		if ((pad & PadLeft0) && (padDelay == 0) )
		{
			PlayNote(MENU_MOVE, 1, 20);
			padDelay = 5; 
			charVal -- ;
			if (numChars != 6)
			{
				if (charVal < 0)
				{
		 			charVal = 27 ;		 // 25
				} 
			}
			else
			{
				if (charVal < 26)
				{
		 			charVal = 27 ;		 // 25
				} 
			}		
		}

		if(padDelay > 0) padDelay--;
			
			select = GetMenuKey(M_SELECT);

			if ((select) && (charVal == 26))
			{
				if (numChars >= 1)
				{
					numChars -- ;
					charX -= CharSet16[(selectData->nameEntered[numChars] - 'A')].width ;
					selectData->nameEntered[numChars] = 0 ;
					charVal = 0 ;
				}		
			}
			else 
			{
				currChar = charVal + 'A' ;
				if ((select) && (numChars != 6) && (charVal != 26) && (charVal != 27) )
				{
				 	selectData->nameEntered[numChars] = currChar ;
				 	selectData->nameEntered[numChars+1] = 0 ;
				 	numChars ++ ;
				 	charX += CharSet16[charVal].width ;
					if (numChars == 6)
					{
						charVal = 27 ;
						currChar = ('Z'+2) ;
					}
				}
				else
					if ((select) && (charVal == 27))
					{
						selectData->nameEntered[numChars] = currChar ;
				 	 	selectData->nameEntered[numChars+1] = 0 ;
					}
			}
	}
	else
	{
		if (userEntry)
		{
		  	userEntry = 0 ;
			selectData->nameEntered[(--numChars)] = 0 ;
			numChars = 0 ;
			selectData->nameEntered[0] = 0 ;
			charX = 80 ;
			charVal = 0 ;
		}
	}

	if (userEntry)
	{
		strcpy (outputName,selectData->nameEntered) ;
 
		if (selectData->nameEntered[numChars] == ('Z'+2))
		{
				outputName[numChars] = 0 ;
		}
		AddText (&(outputName[0]), text_data (40, charY, 16), WhiteText) ;
	}
	
	toggleDisplay ++ ;

	if (!userEntry)
	{
		selectData->select = 1 ;
		selectData->firstTime = 1 ;
		return ;
	}
}
*/


void EnterLoadGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	unsigned short			KeyCode = 0;
	short					AsciiVal;
//	extern signed char		etc_KeyPressed;
//	extern unsigned char	etc_Key;
//	extern unsigned long	etc_KeyboardMap[256];


	CentreText ("LOAD GAME FROM DISK", text_data (70, 20, 8), RedText) ; 
	AddText ("ENTER NAME OF FILE", text_data (65, 60, 8), WhiteText) ; 

	if(TextEntry)
	{
		if(etc_KeyPressed)
		{
			KeyCode = GetKeyNoBlock();

			if((KeyCode > 0x40 && KeyCode < 0x5b))//A - Z
			// || 		// Q - P

			 //  (KeyCode > 29 && KeyCode < 39) || 		// A - L

			   //(KeyCode > 43 && KeyCode < 51))			// Z - M
			{
				LoadFilename[TextPos] = ScanToAscii[KeyCode];

				if(TextPos < (MAXSTRING - 2))
					TextPos++;

				LoadFilename[TextPos] = 101;		// Cursor
			}
		
			if((KeyCode == 0x08) && (TextPos > 0)) 	// Backspace
			{
				LoadFilename[TextPos] = ' ';
				LoadFilename[--TextPos] = 101;
			}
		}
	}
	else
	{
		if(davebodge2 == 0)
		{
			strcpy(LoadFilename, "         ");
			LoadFilename[0] = 101;
			TextEntry = 1;
			TextPos = 0;
		}
	}

	AddText (LoadFilename, text_data (40, 80, 16), WhiteText) ; 
}



void EnterSaveGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	unsigned short			KeyCode = 0;
	short					AsciiVal;
//	extern signed char		etc_KeyPressed;
//	extern unsigned char	etc_Key;
//	extern unsigned long	etc_KeyboardMap[256];


	CentreText ("SAVE GAME TO DISK", text_data (70, 20, 8), RedText) ; 
	AddText ("ENTER NAME OF FILE", text_data (65, 60, 8), WhiteText) ; 

	if(TextEntry)
	{
		if(etc_KeyPressed)
		{
			KeyCode = GetKeyNoBlock();

			if((KeyCode > 0x40 && KeyCode < 0x5b))//A - Z
			//if((KeyCode > 15 && KeyCode < 26) || 		// Q - P
			   //(KeyCode > 29 && KeyCode < 39) || 		// A - L
			   //(KeyCode > 43 && KeyCode < 51))			// Z - M
			{
				SaveFilename[TextPos] = ScanToAscii[KeyCode];

				if(TextPos < (MAXSTRING - 2))
					TextPos++;

				SaveFilename[TextPos] = 101;		// Cursor
			}
		
			if((KeyCode == 0x08) && (TextPos > 0)) 	// Backspace
			{
				SaveFilename[TextPos] = ' ';
				SaveFilename[--TextPos] = 101;
			}
		}
	}
	else
	{
		if(davebodge2 == 0)
		{
			strcpy(SaveFilename, "         ");
			SaveFilename[0] = 101;
			TextEntry = 1;
			TextPos = 0;
		}
	}

	AddText (SaveFilename, text_data (40, 80, 16), WhiteText) ; 
}



void MenuErrors (int *currMenu, Menus *menuSystem) 
{
		int i ;

		if (menuSystem[*currMenu].updateCount > 0 && menuSystem[*currMenu].errorFlag != NO_ERROR)
		{
			AddText ("ERROR", text_data (105,20, 16), RedText) ;
			
			switch ( menuSystem[*currMenu].errorFlag )
			{
				case CARD_FULL: AddText ("CARD IS FULL", 
									text_data (90,112, 12), RedText) ;
									break ;

				case DUP_NAME:	AddText ("DUPLICATE NAME", 
									text_data (50,112, 12), RedText) ; 
									AddText ("PLEASE ENTER NEW NAME", 
									text_data (40,142, 8), RedText) ; 
									break ;

				case LOAD_ERROR:	AddText ("ERROR DURING LOAD", 
									text_data (25,112, 12), RedText) ;
									break ;

				case SAVE_ERROR:	AddText ("ERROR DURING SAVE", 
									text_data (30,112, 12), RedText) ;
									break ;

				case RENAME_ERROR:AddText ("ERROR DURING RENAME", 
									text_data (10,112, 12), RedText) ;
									break ;

				case NO_CARD:	CentreText ("NO CARD PRESENT", 
									text_data (10,112, 12), RedText) ;
									break ;

				case NO_FILES:	CentreText ("NO FILES PRESENT", 
									text_data (10,112, 12), RedText) ;
									break ;

				case NO_FORMAT:CentreText ("CARD NOT FORMATTED", 
									text_data (10,112, 12), RedText) ;
									break ;

				case FORMAT_ERROR:CentreText ("ERROR DURING FORMAT", 
									text_data (10,112, 12), RedText) ;
									break ;

				case _ERR_BAD_SERIAL_PORT:
									CentreText("BAD SERIAL PORT", text_data (10,112,8), RedText);
									break;

				case _ERR_SERIAL_PORT_NOT_FOUND	:
									CentreText("SERIAL PORT NOT FOUND", text_data (10,112,8), RedText);
									break;
														
														
														
				case _ERR_INSTALLING_SERIAL		:
									CentreText("CANT INSTALL SERIAL DEVICE", text_data (10,112,8), RedText);
									break;
														
														
				case _ERR_HANDSHAKING_SERIAL		:
									CentreText("SERIAL HANDSHAKE FAILURE", text_data (10,112,8), RedText);
									break;
														
														
				case _ERR_SYNCHRONISING_SERIAL	:
									CentreText("SERIAL SYNC FAILURE", text_data (10,112,8), RedText);
									break;
														
														
				case _ERR_TRANSFERRING_SERIAL	:
									CentreText("SERIAL TRANSFER ERROR", text_data (10,112,8), RedText);
									break;
														
														
				case _ERR_TIMEOUT_SERIAL			:
									CentreText("SERIAL TIMED OUT", text_data (10,112,8), RedText);
									break;
														
														
				case _ERR_RESOLVING_SERIAL		:
									CentreText("CANT RESOLVE SERIAL", text_data (10,112,8), RedText);
									break;
														
														
								
				// modem errors
				case	_ERR_INSTALLING_MODEM		:
									CentreText("CANT INSTALL MODEM", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_INITIALIZING_MODEM		:
									CentreText("CANT INITIALIZE MODEM", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_DIALLING_MODEM			:
									CentreText("CANT DIAL MODEM", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_CONNECTING_MODEM		:
									CentreText("CONNECT FAILED", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_ANSWERING_MODEM		:
									CentreText("CANT ANSWER ON MODEM", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_TIMEOUT_MODEM			:
									CentreText("MODEM NOT RESPONDING", text_data (10,112,8), RedText);
									break;
														
														

				// network errors
				case	_ERR_IPX_NOT_PRESENT		:
									CentreText("IPX NOT FOUND", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_SYNCHRONISING		:
									CentreText("IPX SYNC FAILURE", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_SOCKET_FULL		:
									CentreText("IPX SOCKET FULL", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_SOCKET_OPEN		:
									CentreText("SOCKET IN USE", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_OPEN_ERROR			:
									CentreText("OPEN SOCKET FAILURE", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_TOO_MANY_EVENTS	:
									CentreText("TOO MANY EVENT BLOCKS", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_NETWARE_NOT_LOADED		:
									CentreText("NETWARE NOT FOUND", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_DATA_TOO_BIG		:
									CentreText("IPX DATA TOO LARGE", text_data (10,112,8), RedText);
									break;
														
														
				case	_ERR_IPX_TIMEOUT_NETWORK	:
									CentreText("NETWORK TIMED OUT", text_data (10,112,8), RedText);
									break;
														
														
				// unknown errors
				default								:
									CentreText("ERROR CONNECTING", text_data (10,112,8), RedText);
									break;
			}

			menuSystem[*currMenu].updateCount -- ;
		}
		else
		{
			for (i=0; i < MAX_MENUS; i++)
			{
				menuSystem[i].errorFlag = NO_ERROR ;
			}
		}
}	



void ShowHiScores (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int				toggleDisplay = 0 ;
	short						actualTrack = 0 ;


	if (GetMenuKey(M_RIGHT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->hiscoreTrack ++ ;
		if (gameData->RapierClass == 0)
		{
			if (selectData->hiscoreTrack == NumTracks)
				selectData->hiscoreTrack = (NumTracks-1) ;	
		}
		else
		{
			if (gameData->showBonusTrack == 0)
			{
			 	if (selectData->hiscoreTrack == NumTracks)
					selectData->hiscoreTrack = (NumTracks + 1) ;
			}

			if (selectData->hiscoreTrack == totalTracks)
				selectData->hiscoreTrack = (totalTracks-1) ;	
		}
	}

	if (GetMenuKey(M_LEFT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->hiscoreTrack -- ;
		if (gameData->RapierClass == 0)
		{
			if (selectData->hiscoreTrack < 0)
				selectData->hiscoreTrack = 0 ;
		}
		else
		{
			if (gameData->showBonusTrack == 0)
			{
			 	if (selectData->hiscoreTrack == NumTracks)
					selectData->hiscoreTrack = (NumTracks - 1) ;
			}


			if (selectData->hiscoreTrack < 0)
				selectData->hiscoreTrack = 0 ;
		}
	}											

	CentreText ("BEST TIMES", text_data (1, 21, 12), RedText) ;

	actualTrack = selectData->hiscoreTrack ;

	if (selectData->hiscoreTrack >= NumTracks)
	{
		if (gameData->showBonusTrack == 0)
		 	actualTrack -= (NumTracks+1) ;
		else
		 	actualTrack -= NumTracks ;
		CentreText ("RAPIER", text_data (1, 40, 8), RedText) ;
	}
	else
	{
		CentreText ("VENOM", text_data (1, 40, 8), RedText) ;
	}

	if (neg1.head == 8960)
	{
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 240, 210, PBACK) ;
	}


	if (((gameData->RapierClass == 0) && (selectData->hiscoreTrack == (NumTracks-1)))
		 || ((gameData->RapierClass == 1) && (selectData->hiscoreTrack == (totalTracks-1))))		
		DisplayButtons ( 290, 116, ARROWR) ;
	else
	{
		if ((toggleDisplay % 30) <= 20)
			DisplayButtons ( 290, 116, ARROWR) ;
	}

	if (selectData->hiscoreTrack == 0)
		DisplayButtons ( 20, 116, ARROWL) ;
	else
	{
		if ((toggleDisplay % 30) <= 20)
			DisplayButtons ( 20, 116, ARROWL) ;
	}

	toggleDisplay ++ ;

	if (gameData->trackNum <= NumTracks)
	{
		CentreText (selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].optionText, text_data (	selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textX, 
					selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textY-70, 12), RedText) ;
	}
	else
	{
		CentreText (selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].optionText, text_data (	selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textX, 
					selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textY-70, 12), RedText) ;
	}

	if ((toggleDisplay % 30) <= 20)
	{
		AddText ("LAP RECORD", text_data (40,195, 8), WhiteText) ;
		AddTime(scoreTable[selectData->hiscoreTrack].lapRecord,
 				 	text_data (165, 188, 16), RedText);
	}
	toggleDisplay ++ ;

	DisplayHiScore (selectData->hiscoreTrack) ;	
}


void ShowTimeTrialScores (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int				toggleDisplay = 0 ;
	short						actualTrack = 0 ;


	if (GetMenuKey(M_RIGHT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->hiscoreTrack ++ ;
		if (gameData->RapierClass == 0)
		{
			if (selectData->hiscoreTrack == (NumTracks+HiScoreTracks))
				selectData->hiscoreTrack = ((NumTracks-1)+HiScoreTracks) ;	
		}
		else
		{
			if (gameData->showBonusTrack == 0)
			{
			 	if (selectData->hiscoreTrack == (NumTracks+HiScoreTracks))
					selectData->hiscoreTrack = ((NumTracks + 1)+HiScoreTracks) ;
			}

			if (selectData->hiscoreTrack == (totalTracks+HiScoreTracks))
				selectData->hiscoreTrack = ((totalTracks-1)+HiScoreTracks) ;	
		}
	}

	if (GetMenuKey(M_LEFT))
	{
		PlayNote(MENU_MOVE, 1, 20);
		selectData->hiscoreTrack -- ;
		if (gameData->RapierClass == 0)
		{
			if (selectData->hiscoreTrack < 14)
				selectData->hiscoreTrack = 14 ;
		}
		else
		{
			if (gameData->showBonusTrack == 0)
			{
			 	if (selectData->hiscoreTrack == (NumTracks+HiScoreTracks))
					selectData->hiscoreTrack = ((NumTracks - 1)+HiScoreTracks) ;
			}

			if (selectData->hiscoreTrack < 14)
				selectData->hiscoreTrack = 14 ;
		}
	}											

	CentreText ("BEST TIMES", text_data (1, 21, 12), RedText) ;

	actualTrack = (selectData->hiscoreTrack - HiScoreTracks) ;

	if ((selectData->hiscoreTrack-HiScoreTracks) >= (NumTracks))
	{
		if (gameData->showBonusTrack == 0)
		 	actualTrack -= ((NumTracks+1)) ;
		else
		 	actualTrack -= (NumTracks) ;
		CentreText ("RAPIER", text_data (1, 40, 8), RedText) ;
	}
	else
	{
		CentreText ("VENOM", text_data (1, 40, 8), RedText) ;
	}

	if (neg1.head == 8960)
	{
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 240, 210, PBACK) ;
	}

	if (((gameData->RapierClass == 0) && ((selectData->hiscoreTrack-HiScoreTracks) == (NumTracks-1)))
		 || ((gameData->RapierClass == 1) && ((selectData->hiscoreTrack-HiScoreTracks) == (totalTracks-1))))		
		DisplayButtons ( 290, 116, ARROWR) ;
	else
	{
		if ((toggleDisplay % 30) <= 20)
			DisplayButtons ( 290, 116, ARROWR) ;
	}

	if ((selectData->hiscoreTrack-HiScoreTracks) == 0)
		DisplayButtons ( 20, 116, ARROWL) ;
	else
	{
		if ((toggleDisplay % 30) <= 20)
			DisplayButtons ( 20, 116, ARROWL) ;
	}

	toggleDisplay ++ ;

	if (gameData->trackNum <= NumTracks)
	{
		CentreText (selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].optionText, text_data (	selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textX, 
					selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textY-70, 12), RedText) ;
	}
	else
	{
		CentreText (selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].optionText, text_data (	selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textX, 
					selectData->menuSystem[TRACK_MENU].menuOptions[actualTrack].textY-70, 12), RedText) ;
	}

	
	if ((toggleDisplay % 30) <= 20)
	{
		AddText ("LAP RECORD", text_data (40,195, 8), WhiteText) ;
		AddTime(scoreTable[selectData->hiscoreTrack].lapRecord,
 				 	text_data (165, 188, 16), RedText);
	}
	toggleDisplay ++ ;

	DisplayHiScore (selectData->hiscoreTrack) ;	
}
								
#ifndef USE_ATI


void FadeScreen (POLY_F4 *fadeDownPrim, POLY_F4 *fadeUpPrim, SelectionData *selectData)
{
	char 					value = 20 ; /*14*/
	short					x1=0, y1=0 ;
	short					x2=0, y2=240 ;
	short					width = 320 ;


	SetPolyF4(&(fadeDownPrim[CurrentScreen]));			
	fadeDownPrim[CurrentScreen].r0=230; // PCwipeout
	fadeDownPrim[CurrentScreen].g0=0;
	fadeDownPrim[CurrentScreen].b0=BLACK_NOT_DRAWN;

	setXY4(&(fadeDownPrim[CurrentScreen]),
				x1, y1,
				(x1+width), y1,
				x1, (y1+selectData->fadeHeight),
				(x1+width), (y1+selectData->fadeHeight)) ; 

	AddPrim( OT[ CurrentScreen ] , ( ulong* ) &(fadeDownPrim[CurrentScreen]) );

	SetPolyF4(&(fadeUpPrim[CurrentScreen]));			
	fadeUpPrim[CurrentScreen].r0=230; // PCwipeout
	fadeUpPrim[CurrentScreen].g0=0;
	fadeUpPrim[CurrentScreen].b0=BLACK_NOT_DRAWN;

	setXY4(&(fadeUpPrim[CurrentScreen]),
				x2, y2,
				(x2+width), y2,
				x2, (y2-selectData->fadeHeight),
				(x2+width), (y2-selectData->fadeHeight)) ; 

	AddPrim( OT[ CurrentScreen ] , ( ulong* ) &(fadeUpPrim[CurrentScreen]) );

	if (selectData->select == 1)
	{
		selectData->fadeHeight += value ;
	}
	else
	{
		selectData->fadeHeight -= value ;
	}
}
#endif //USE_ATI


void InitGameData (ConfigData *gameData)
{
	gameData->shipNum = randy () % 8 ;
	gameData->trackNum =	gameData->actTrackNum = gameData->ship2Num = 0 ;
	gameData->oppMenu = gameData->exitMenu = gameData->exit2Menu = gameData->class = 0 ;
	gameData->team = gameData->team2 = gameData->gameType = 0 ;
	gameData->Lives = 3 ;
	gameData->EndOfGame = 0 ;
	gameData->continueConfirm = 1 ;
	gameData->NumTracks = 1 ;
	gameData->Position = 8 ;
	gameData->defaultMenu = -2 ;
	gameData->RapierClass = 0 ;
	gameData->CheatMode = 0 ;
	gameData->cdTrack = 1 ;
	gameData->gameComplete = 0 ;
	gameData->showPilotCount = 0 ;
	gameData->gameOverCount = 0 ;
	gameData->venomComplete = 0 ;
	gameData->newLapRecord = 0 ;
	gameData->masterWins = 0 ;
	gameData->slaveWins = 0 ;
	gameData->prevLap = 0 ;
	gameData->gotoMenu = 0 ;
	gameData->negconPreset = 1 ;
	gameData->normpadPreset = 0 ;
	gameData->cardPresent = 0 ;
	gameData->negTwist = 255 ;
	gameData->currSliderPos = 150 ;
	gameData->showBonusTrack = 0 ; 
	gameData->trackInc = 1 ;
	gameData->lostGame = 0 ;
	gameData->showGameOver = 0 ;
}


void ConfirmSaveGame (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int	toggle = 0 ;

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}

	CentreText ("OVERWRITE EXISTING FILE", text_data (70, 50, 8), RedText) ; 
	if (selectData->saveType == NORM)
		CentreText ("PERFORM SAVE", text_data (55, 60, 8), WhiteText) ; 

	if ((selectData->saveType == OVERWRITE) ||
		 (selectData->saveType == RENAME))
		CentreText ("OVERWRITE EXISTING FILE", text_data (55, 60, 8), WhiteText) ; 
	
	AddText (selectData->nameEntered, text_data (140, 74, 8), RedText) ; 

	if ((GetMenuKey(M_LEFT)) || (GetMenuKey(M_RIGHT)))
	{
		selectData->confirm ^= 1 ;
		PlayNote(MENU_MOVE, 1, 20);
	}

	if (selectData->confirm)
	{
		AddText ("YES", text_data (80, 120, 16), WhiteText) ; 
		//if ((toggle % 2) == 0)
			AddText ("NO", text_data (210, 120, 16), RedText) ; 
		//else
			//AddText ("NO", text_data (210, 120, 16), WhiteText) ;

	}
	else
	{
		//if ((toggle % 2) == 0)
			AddText ("YES", text_data (80, 120, 16), RedText) ; 
		//else
			//AddText ("YES", text_data (80, 120, 16), WhiteText) ;
		AddText ("NO", text_data (210, 120, 16), WhiteText) ; 
	}	

	toggle ++ ;
}



void	ContinueGameConfirm (char *confirm, short xpad, char *lConfirm, ulong *toggle)  
{
	CentreText ("CONTINUE QUALIFYING OR QUIT", text_data (77, 95, 8), WhiteText) ; 


	if (*lConfirm)
	{
		//if ((*toggle %2) == 0)
			AddText ("QUIT", text_data (215, 120, 16), RedText) ; 
		//else
			//AddText ("QUIT", text_data (215, 120, 16), WhiteText) ;
		AddText ("QUALIFY", text_data (45, 120, 16), WhiteText) ; 
	}
	else
	{
		AddText ("QUIT", text_data (215, 120, 16), WhiteText) ; 
		//if ((*toggle %2) == 0)
			AddText ("QUALIFY", text_data (45, 120, 16), RedText) ; 
		//else
			//AddText ("QUALIFY", text_data (45, 120, 16), WhiteText) ;
	}	

	(*toggle) ++ ;
}


void	RestartGameConfirm (char *confirm, short xpad, char *lConfirm, ulong *toggle)  
{
	CentreText ("RESTART RACE", text_data (77, 95, 16), WhiteText) ; 


	if (*lConfirm)
	{
		//if ((*toggle %2) == 0)
			AddText ("NO", text_data (215, 120, 16), RedText) ; 
		//else
			//AddText ("NO", text_data (215, 120, 16), WhiteText) ;
		AddText ("YES", text_data (68, 120, 16), WhiteText) ; 
	}
	else
	{
		AddText ("NO", text_data (215, 120, 16), WhiteText) ; 
		//if ((*toggle %2) == 0)
			AddText ("YES", text_data (68, 120, 16), RedText) ; 
		//else
			//AddText ("YES", text_data (68, 120, 16), WhiteText) ;
	}	

	(*toggle) ++ ;
}

void SetUpPilots (int teamNum, Menus *menuSystem)
{

	if (teamNum == AG_SYSTEMS)
	{
		strcpy (menuSystem[7].menuOptions[0].optionText,"JOHN DEKKA") ;
		strcpy (menuSystem[7].menuOptions[1].optionText,"DANIEL CHANG") ;
	}

	if (teamNum == AURICOM)
	{
		strcpy (menuSystem[7].menuOptions[0].optionText,"ARIAL TETSUO") ;
		strcpy (menuSystem[7].menuOptions[1].optionText,"ANASTASIA CHEROVOSKI") ;
	}

	if (teamNum == QIREX)
	{
		strcpy (menuSystem[7].menuOptions[0].optionText,"KEL SOLAAR") ;
		strcpy (menuSystem[7].menuOptions[1].optionText,"ARIAN TETSUO") ;
	}

	if (teamNum == FEISAR)
	{
		strcpy (menuSystem[7].menuOptions[0].optionText,"SOFIA DE LA RENTE") ;
		strcpy (menuSystem[7].menuOptions[1].optionText,"PAUL JACKSON") ;
	}
}

#if 0 // Mark - now in PCcombat.c
void MenuMaster (ConfigData *gameData, int *currMenu)
{
	short		i ;
	u_long	cts ;
	char		send [96], receive [96] ;


	send[0] = gameData->class ;
	send[1] = gameData->gameType ; 
	send[2] = gameData->team ; 
	send[3] = gameData->shipNum ; 
	send[4] = gameData->trackNum ; 
	send[5] = gameData->actTrackNum ;
	send[6] = *currMenu ;
	send[7] = gameData->exitMenu ;
	send[8] = gameData->gameType ;


//	SetRTSDTR( SIOCL_DTRON | SIOCL_RTSON );	/* Signal that data is waiting */
	
//	cts = ReadCTS();
//	while( cts == 0 )		 							/* wait for OK to send */
//	{
//		cts = ReadCTS();
//	}

//	cts = 0;
//	while( cts < 200 )
//		cts++;

	WriteSIOData( &(send[0]), 96);

//	cts = ReadCTS();
//	while( cts == 1 ) 								/* wait for acknowledge */
//	{
//		cts = ReadCTS();
//	}

//	SetRTSDTR( 0 );									/* remove signals */


//	cts = ReadDSR();
//	while( cts == 0 )
//	{
//		cts = ReadDSR();
//	}

	/* Read Data */
//	EnterCriticalSection();

//	SetRTSDTR( SIOCL_DTRON | SIOCL_RTSON );		/* Set OK to send */

	ReadSIOData( &(receive[0]), 96);

//	SetRTSDTR( 0 );										/* remove OK to send */

//	cts = ReadCTS();
//	while( cts == 1 ) 									/* wait for acknowledge */
//	{
//		cts = ReadCTS();
//	}

//	ExitCriticalSection();

	gameData->class = receive[0] ;
	gameData->gameType = receive[1] ;
	gameData->team2 = receive[2] ;
	gameData->ship2Num = receive[3] ;
	gameData->trackNum = receive[4] ;
	gameData->actTrackNum = receive[5] ;
	gameData->oppMenu = receive[6] ;
	gameData->exit2Menu = receive[7] ;
	raceType = gameData->gameType = receive[8] ;

}


void CaptainBirdsEye (ConfigData *gameData, int *currMenu)
{
	short		i ;
	u_long	cts ;
	char		send [96], receive [96] ;

	/* Read Data */
//	cts = ReadDSR();
//	while( cts == 0 )
//		cts = ReadDSR();

//	EnterCriticalSection();

//	SetRTSDTR( SIOCL_DTRON | SIOCL_RTSON );		/* Set OK to send */

	ReadSIOData( &(receive[0]), 96);

//	SetRTSDTR( 0 );										/* remove OK to send */

//	cts = ReadCTS();
//	while( cts == 1 ) 									/* wait for acknowledge */
//	{
//		cts = ReadCTS();
//	}

//	ExitCriticalSection();


	gameData->class = receive[0] ;
	gameData->gameType = receive[1] ;
	gameData->team2 = receive[2] ;
	gameData->ship2Num = receive[3] ;
	gameData->trackNum = receive[4] ;
	gameData->actTrackNum = receive[5] ;
	gameData->oppMenu = receive[6] ;
	gameData->exit2Menu = receive[7] ;
	raceType = gameData->gameType = receive[8] ;


	send[0] = gameData->class ;
	send[1] = gameData->gameType ; 
	send[2] = gameData->team ; 
	send[3] = gameData->shipNum ; 
	send[4] = gameData->trackNum ; 
	send[5] = gameData->actTrackNum ;
	send[6] = *currMenu ;
	send[7] = gameData->exitMenu ;
	send[8] = gameData->gameType ;


//	SetRTSDTR( SIOCL_DTRON | SIOCL_RTSON );	/* Signal that data is waiting */

//	cts = ReadCTS();
//	while( cts == 0 )		 							/* wait for OK to send */
//	{
//		cts = ReadCTS();
//	}

//	cts = 0;
//	while( cts < 200 )
//		cts++;

	WriteSIOData( &(send[0]), 96);

//	cts = ReadCTS();
//	while( cts == 1 ) 								/* wait for acknowledge */
//	{
//		cts = ReadCTS();
//	}

//	SetRTSDTR( 0 );									/* remove signals */

}

void StartTwoPlayer (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{

	if (gameData->oppMenu != START_TWO_PLAYER_MENU)
	{
		ownShip = gameData->shipNum ;
		serialShip = gameData->ship2Num ;

		if (gameType == MASTER)
		{
		 	CentreText ("WAITING FOR PLAYER TWO", 
		 		text_data (1,116, 8), RedText) ;
		}

		if (gameType == SLAVE)
		{
		 	CentreText ("WAITING FOR PLAYER ONE", 
		 		text_data (1,116, 8), RedText) ;
		}
	}
}
#endif // end of mark comment

/*************************************************************/
/******************* NEGCON & PLAY PAD ***********************/
/*************************************************************/


void MenuNegCon (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int	delay = 0 ;	
	static char	key = 0 ;	
	static int	toggleDisplay = 0 ;	
	char		temp = 0 ;

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}


	if ((toggleDisplay % 30) <= 20)
	{
		DisplayButtons ( 290, 116, ARROWR) ;
		DisplayButtons ( 20, 116, ARROWL) ;
	}


	toggleDisplay ++ ;


 	CentreText ("NEGCON CONFIGURATION", 
	 		text_data (1,21, 8), RedText) ;

	temp = 0 ;

	if (delay == 0)
	{
		if ((GetMenuKey(M_RIGHT)))
		{
			delay = 32 ;
			key = 1 ;
			temp = 1 ;
			PlayNote(MENU_MOVE, 1, 20);
		 	gameData->oldNegPreset ++ ;
			if (gameData->oldNegPreset == NEGCON_PRESETS)
				gameData->oldNegPreset = 1 ;
		}

		if ((GetMenuKey(M_LEFT)))
		{
			delay = 32 ;
			key = 2 ;
			temp = 1 ;
			PlayNote(MENU_MOVE, 1, 20);
			gameData->oldNegPreset -- ;
			if (gameData->oldNegPreset < 1)
				gameData->oldNegPreset = (NEGCON_PRESETS-1) ;
		}

		if (temp == 0)
		{
			DisplayButtons ( 22, 47, LBRAKE) ;
			DisplayButtons ( 230, 47, RBRAKE) ;

			switch (gameData->oldNegPreset)
			{
				case 0:	DrawLine (221, 93, 252, 88, musicPrims->lines, 0) ; /* B */
							DrawLine (216, 125, 178, 181, musicPrims->lines, 2) ; /* I */
							DrawLine (206, 110, 118, 170, musicPrims->lines, 3) ; /* II */
							DisplayButtons ( 255, 88, VIEW) ;
							DisplayButtons ( 118, 175, PADFIRE) ;
							DisplayButtons ( 178, 186, THRUST) ;
							break ;

				case 1: 	CentreText ("PRESET A", text_data (1,40, 8), RedText) ;
							DrawLine (221, 93, 252, 88, musicPrims->lines, 0) ; /* B */
							DrawLine (216, 125, 178, 181, musicPrims->lines, 2) ; /* I */
							DrawLine (206, 110, 118, 170, musicPrims->lines, 3) ; /* II */
							DisplayButtons ( 255, 88, VIEW) ;
							DisplayButtons ( 118, 175, PADFIRE) ;
							DisplayButtons ( 178, 186, THRUST) ;
							break ;

				case 2: 	CentreText ("PRESET B", text_data (1,40, 8), RedText) ;
							DrawLine (221, 93, 252, 88, musicPrims->lines, 0) ; /* B */
							DrawLine (216, 125, 178, 181, musicPrims->lines, 2) ; /* I */
							DrawLine (206, 110, 118, 170, musicPrims->lines, 3) ; /* II */
							DisplayButtons ( 255, 88, VIEW) ;
							DisplayButtons ( 118, 175, THRUST) ;
							DisplayButtons ( 178, 186, PADFIRE) ;
							break ;

				case 3: 	CentreText ("PRESET C", text_data (1,40, 8), RedText) ;
							DrawLine (221, 93, 252, 88, musicPrims->lines, 0) ; /* B */
							DrawLine (237, 109, 252, 144, musicPrims->lines, 1) ; /* A */
							DrawLine (206, 110, 118, 170, musicPrims->lines, 3) ; /* II */
							DisplayButtons ( 252, 147, THRUST) ;
							DisplayButtons ( 255, 88, PADFIRE) ;
							DisplayButtons ( 118, 175, VIEW) ;
							break ;

				case 4: 	CentreText ("PRESET D", text_data (1,40, 8), RedText) ;
							DrawLine (221, 93, 252, 88, musicPrims->lines, 0) ; /* B */
							DrawLine (237, 109, 252, 144, musicPrims->lines, 1) ; /* A */
							DrawLine (206, 110, 118, 170, musicPrims->lines, 3) ; /* II */
							DisplayButtons ( 252, 147, PADFIRE) ;
							DisplayButtons ( 255, 88, THRUST) ;
							DisplayButtons ( 118, 175, VIEW) ;
							break ;
			}
		}
	}
	else
	{
		delay -- ;

		if (key == 1)
			modelHdg += 256 ;

		if (key == 2)
			modelHdg -= 256 ;

		if (delay > 15)
		{
			zoomInOut += 500 ;
			downUp += 500 ;
		}

		if (delay <= 15)
		{
			zoomInOut -= 500 ;
			downUp -= 500 ;
		}
	}
}

void MenuPlayPad (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int	delay = 0 ;	
	static char	key = 0 ;	
	static int	toggleDisplay = 0 ;
	char	 temp = 0 ;

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}

	if ((toggleDisplay % 30) <= 20)
	{
		DisplayButtons ( 290, 116, ARROWR) ;
		DisplayButtons ( 20, 116, ARROWL) ;
	}

	toggleDisplay ++ ;

 	CentreText ("JOYPAD CONFIGURATION", text_data (1,21, 8), RedText) ;

	temp = 0 ;

	if (delay == 0)
	{
		if ((GetMenuKey(M_RIGHT)))
		{
			key = 1 ;
			temp = 1 ;
			delay = 32 ;
			PlayNote(MENU_MOVE, 1, 20);
		 	gameData->oldPadPreset ++ ;
			if (gameData->oldPadPreset == NORMPAD_PRESETS)
				gameData->oldPadPreset = 0 ;
		}

		if ((GetMenuKey(M_LEFT)))
		{
			key = 2 ;
			temp = 1 ;
			delay = 32 ;
			PlayNote(MENU_MOVE, 1, 20);
			gameData->oldPadPreset -- ;
			if (gameData->oldPadPreset < 0)
				gameData->oldPadPreset = (NORMPAD_PRESETS-1) ;
		}

		if (temp == 0)
		{
			DisplayButtons ( 17, 153, LBRAKE) ; /* Left */
			DisplayButtons ( 237, 153, RBRAKE) ; /* Right */

			switch (gameData->oldPadPreset)
			{
				case 0: 	CentreText ("PRESET A", text_data (1,210, 8), RedText) ;

							DrawLine (204, 63, 245, 39, musicPrims->lines, 0) ; /* Triangle */
							DrawLine (216, 76, 251, 68, musicPrims->lines, 1) ; /* Circle */
							DrawLine (204, 87, 230, 97, musicPrims->lines, 3) ; /* Cross */
							DrawLine (210, 153, 252, 153, musicPrims->lines, 6) ; /* Right Top */
							DrawLine (130, 167, 81, 163, musicPrims->lines, 4) ; /* Left Top */

							DisplayButtons ( 248, 39, VIEW) ; /* Triangle */
							DisplayButtons ( 254, 68, PADFIRE) ;  /* Circle */
							DisplayButtons ( 234, 97, THRUST) ; /* Cross */

							break ;

				case 1: 	CentreText ("PRESET B", text_data (1,210, 8), RedText) ;

							DrawLine (204, 63, 245, 39, musicPrims->lines, 0) ; /* Triangle */
							DrawLine (204, 87, 230, 97, musicPrims->lines, 3) ; /* Cross */
							DrawLine (192, 75, 132, 113, musicPrims->lines, 2) ; /* Square */
							DrawLine (210, 153, 252, 153, musicPrims->lines, 6) ; /* Right Top */
							DrawLine (130, 167, 81, 163, musicPrims->lines, 4) ; /* Left Top */

							DisplayButtons ( 132, 117, PADFIRE) ; /* Square */
							DisplayButtons ( 234, 97, THRUST) ; /* Cross */
							DisplayButtons ( 248, 39, VIEW) ; /* Triangle */

							break ;

				case 2: 	CentreText ("PRESET C", text_data (1,210, 8), RedText) ;

							DrawLine (204, 87, 230, 97, musicPrims->lines, 3) ; /* Cross */
							DrawLine (210, 153, 252, 153, musicPrims->lines, 6) ; /* Right Top */
							DrawLine (130, 167, 81, 163, musicPrims->lines, 4) ; /* Left Top */
							DrawLine (130, 183, 81, 185, musicPrims->lines, 5) ; /* Left Bottom */
							DrawLine (214, 168, 252, 185, musicPrims->lines, 7) ; /* Right Bottom */

							DisplayButtons ( 234, 97, THRUST) ; /* Cross */
							DisplayButtons ( 22, 185, VIEW) ;  /* Circle */
							DisplayButtons ( 252, 185, PADFIRE) ; /* Cross */
							break ;

				case 3: 	CentreText ("PRESET D", text_data (1,210, 8), RedText) ;

							DrawLine (216, 76, 251, 68, musicPrims->lines, 1) ; /* Circle */
							DrawLine (192, 75, 132, 113, musicPrims->lines, 2) ; /* Square */
							DrawLine (204, 87, 230, 97, musicPrims->lines, 3) ; /* Cross */
							DrawLine (210, 153, 252, 153, musicPrims->lines, 6) ; /* Right Top */
							DrawLine (130, 167, 81, 163, musicPrims->lines, 4) ; /* Left Top */

							DisplayButtons ( 254, 68, VIEW) ;  /* Circle */
							DisplayButtons ( 132, 117, THRUST) ; /* Square */
							DisplayButtons ( 234, 97, PADFIRE) ; /* Cross */

							break ;

				case 4: 	CentreText ("PRESET E", text_data (1,210, 8), RedText) ;

							DrawLine (216, 76, 251, 68, musicPrims->lines, 1) ; /* Circle */
							DrawLine (204, 63, 245, 39, musicPrims->lines, 0) ; /* Triangle */
							DrawLine (204, 87, 230, 97, musicPrims->lines, 3) ; /* Cross */
							DrawLine (126, 183, 81, 163, musicPrims->lines, 5) ; /* Left Bottom */
							DrawLine (214, 168, 252, 153, musicPrims->lines, 7) ; /* Right Bottom */

							DisplayButtons ( 248, 39, VIEW) ; /* Triangle */
							DisplayButtons ( 254, 68, PADFIRE) ;  /* Circle */
							DisplayButtons ( 234, 97, THRUST) ; /* Cross */

							break ;

				case 5: 	CentreText ("PRESET F", text_data (1,210, 8), RedText) ;

							DrawLine (204, 63, 245, 39, musicPrims->lines, 0) ; /* Triangle */
							DrawLine (204, 87, 230, 97, musicPrims->lines, 3) ; /* Cross */
							DrawLine (192, 75, 132, 113, musicPrims->lines, 2) ; /* Square */
							DrawLine (126, 183, 81, 163, musicPrims->lines, 5) ; /* Left Bottom */
							DrawLine (214, 168, 252, 153, musicPrims->lines, 7) ; /* Right Bottom */

							DisplayButtons ( 132, 117, PADFIRE) ; /* Square */
							DisplayButtons ( 234, 97, THRUST) ; /* Cross */
							DisplayButtons ( 248, 39, VIEW) ; /* Triangle */

							break ;

				case 6: 	CentreText ("PRESET G", text_data (1,210, 8), RedText) ;

							DrawLine (204, 63, 245, 39, musicPrims->lines, 0) ; /* Triangle */
							DrawLine (216, 76, 251, 68, musicPrims->lines, 1) ; /* Circle */
							DrawLine (192, 75, 132, 113, musicPrims->lines, 2) ; /* Square */
							DrawLine (130, 167, 81, 163, musicPrims->lines, 4) ; /* Left Top */
							DrawLine (210, 153, 252, 153, musicPrims->lines, 6) ; /* Right Top */
							DisplayButtons ( 248, 39, THRUST) ; /* Triangle */
							DisplayButtons ( 254, 68, PADFIRE) ;  /* Circle */
							DisplayButtons ( 132, 117, VIEW) ; /* Square */

							break ;

				case 7: 	CentreText ("PRESET H", text_data (1,210, 8), RedText) ;

							DrawLine (204, 63, 245, 39, musicPrims->lines, 0) ; /* Triangle */
							DrawLine (216, 76, 251, 68, musicPrims->lines, 1) ; /* Circle */
							DrawLine (192, 75, 132, 113, musicPrims->lines, 2) ; /* Square */
							DrawLine (130, 167, 81, 163, musicPrims->lines, 4) ; /* Left Top */
							DrawLine (210, 153, 252, 153, musicPrims->lines, 6) ; /* Right Top */
							DisplayButtons ( 248, 39, PADFIRE) ; /* Triangle */
							DisplayButtons ( 254, 68, THRUST) ;  /* Circle */
							DisplayButtons ( 132, 117, VIEW) ; /* Square */

							break ;
			}
 		}
	}
	else
	{
		delay -- ;
		if (key == 1)
			modelHdg += 256 ;

		if (key == 2)
			modelHdg -= 256 ;

		if (delay > 15)
		{
			zoomInOut += 500 ;
			downUp += 500 ;
		}

		if (delay <= 15)
		{
			zoomInOut -= 500 ;
			downUp -= 500 ;
		}
	}
}

void SetNormPadPresets (ConfigData *gameData)
{
	switch (gameData->normpadPreset)
	{

		case 0 :	pCon.thrust 	= PadCross0;
					pCon.view 		= PadTriangle0;
					pCon.fire 		= PadCircle0;
					pCon.lBrake 	= PadTopRight2;
					pCon.rBrake 	= PadTopLeft2;
					break ;

		case 1 :	pCon.thrust 	= PadCross0;
					pCon.view 		= PadTriangle0;
					pCon.fire 		= PadSquare0;
					pCon.lBrake 	= PadTopRight2;
					pCon.rBrake 	= PadTopLeft2;
					break ;

		case 2 :	pCon.thrust 	= PadCross0;
					pCon.view 		= PadTopLeft1;
					pCon.fire 		= PadTopRight1;
					pCon.lBrake 	= PadTopRight2;
					pCon.rBrake 	= PadTopLeft2;
					break ;

		case 3 :	pCon.thrust 	= PadSquare0;
					pCon.view 		= PadCircle0;
					pCon.fire 		= PadCross0;
					pCon.lBrake 	= PadTopRight2;
					pCon.rBrake 	= PadTopLeft2;
					break ;

		case 4 :	pCon.thrust 	= PadCross0;
					pCon.view 		= PadTriangle0;
					pCon.fire 		= PadCircle0;
					pCon.lBrake 	= PadTopRight1;
					pCon.rBrake 	= PadTopLeft1;
					break ;

		case 5 :	pCon.thrust 	= PadCross0;
					pCon.view 		= PadTriangle0;
					pCon.fire 		= PadSquare0;
					pCon.lBrake 	= PadTopRight1;
					pCon.rBrake 	= PadTopLeft1;
					break ;

		case 6 :	pCon.thrust 	= PadTriangle0;
					pCon.view 		= PadSquare0;
					pCon.fire 		= PadCircle0;
					pCon.lBrake 	= PadTopRight2;
					pCon.rBrake 	= PadTopLeft2;
					break ;

		case 7 :	pCon.thrust 	= PadCircle0;
					pCon.view 		= PadSquare0;
					pCon.fire 		= PadTriangle0;
					pCon.lBrake 	= PadTopRight2;
					pCon.rBrake 	= PadTopLeft2;
					break ;
	}
}


void SetNegconPresets (ConfigData *gameData)
{
	switch (gameData->negconPreset)
	{

		case 0 :	nCon.type 			= 0;
					nCon.type 			|= A_THRUST_I;
					nCon.type 			|= D_VIEW;
					nCon.type 			|= A_FIRE_II;

					nCon.thrust 		= NEG_A;
					nCon.view 			= NEG_B;
					nCon.fire 			= PadCircle0;
					break ;

		case 1 :	nCon.type 			= 0;
					nCon.type 			|= A_THRUST_I;
					nCon.type 			|= D_VIEW;
					nCon.type 			|= A_FIRE_II;

					nCon.thrust 	= PadCross0;
					nCon.view 		= NEG_B;
					nCon.fire 		= PadSquare0;
					break ;

		case 2 :	nCon.type 			= 0;
					nCon.type 			|= A_THRUST_II;
					nCon.type 			|= D_VIEW;
					nCon.type 			|= A_FIRE_I;

					nCon.thrust 	= PadCross0;
					nCon.view 		= NEG_B ;
					nCon.fire 		= PadTopRight1;
					break ;

		case 3 :	nCon.type 			= 0;
					nCon.type 			|= D_THRUST;
					nCon.type 			|= A_VIEW;
					nCon.type 			|= D_FIRE;

					nCon.thrust 	= NEG_A;
					nCon.view 		= PadCircle0;
					nCon.fire 		= NEG_B;
					break ;

		case 4 :	nCon.type 			= 0;
					nCon.type 			|= D_THRUST;
					nCon.type 			|= A_VIEW;
					nCon.type 			|= D_FIRE;

					nCon.thrust 	= NEG_B;
					nCon.view 		= PadTriangle0;
					nCon.fire 		= NEG_A;
					break ;
	}
}


void DrawGames (Menus *menuSystem, int *currMenu, int shipNo, int *swapDelay)
{
	static hdg = 0;

	InitMenuDisplay (90) ;

	// jed 

	if (shipNo == 0)
	{
	// jed 

		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[0]->skeleton),0,	-50,600) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[0]->skeleton),hdg,	0,	0);
	  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[0], NULL);
	}

	if (shipNo == 1)
	{
	// jed 

		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[2]->skeleton),0,	-50,600) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[2]->skeleton),hdg,	0,0);
	  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[2], NULL);
	}


	if (shipNo == 2)
	{
	// jed 

		SetSkeletonPosition((menuSystem[(*currMenu)].menu2Model[2]->skeleton),0,-50,600) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menu2Model[2]->skeleton),	hdg, 0,0);
	  	TransformStaticMenuObject(menuSystem[(*currMenu)].menu2Model[2], NULL);
	}

	hdg += 40;
}



void DrawOptionMenus(Menus *menuSystem, int *currMenu, int shipNo, int *swapDelay)
{
	static hdg = 0, heading = 0;

	andybodge=1;	// pc wipeout - horrid but i don't care !!! (used in detail & controller configs)

	InitMenuDisplay (90) ;

	if (shipNo == 0)
	{
	// jed 
		Reallyinmenu = 0;
		inmenu = 1;
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[1]->skeleton),0,	0,	400) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[1]->skeleton), hdg,0,	0);
	  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[1], NULL);
	}

	if (shipNo == 1)
	{
	// jed 

		Reallyinmenu = 0;
		inmenu = 1;
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[0]->skeleton),0,	0,	400) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[0]->skeleton), hdg,0,	0);
	  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[0], NULL);
	}

	if (shipNo == 2)
	{
		// jed

		Reallyinmenu = 0;
		inmenu = 1;
		switch(controller)
		{
			case _KEYBOARD:
				DisplayPadMenu1(menuSystem,currMenu,shipNo,swapDelay);
				break;
			case _MOUSE1:
				DisplayPadMenu(menuSystem,currMenu,shipNo,swapDelay);
				break;
			case _JOYSTICK1:
				DisplayPadMenu2(menuSystem,currMenu,shipNo,swapDelay);
				break;
		}
	}

	if (shipNo == 3)				// Graphics Detail
	{
		// jed

		Reallyinmenu = 1;
		inmenu = 0;
		DrawTeamShips (0, 0, MODELSIZE, &heading, menuSystem[DETAILS_MENU].menuModel[shipShapeOrder[0]]) ;
//		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[2]->skeleton),0,	0,	600) ;	 /*1800*/
//		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[2]->skeleton), hdg,0,	0);
//	  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[2], NULL);
	}

	//silly me - I thought shipNo meant it was drawing ships...
	//it was drawing headphones all the time. SJR

	/*if (shipNo == 4)
	{
		// jed
		Reallyinmenu = 0;
		inmenu = 1;
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[3]->skeleton),0,	0,	600) ;	 //1800
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[3]->skeleton), 	hdg,0,0);
	  	TransformStaticMenuObject(menuSystem[(*currMenu)].menuModel[3], NULL);
	}*/

	//now i'tll not draw the headphones and draw the stopwatch instead.
	if (shipNo == 4)
	{
		// jed
		Reallyinmenu = 0;
		inmenu = 1;
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[2]->skeleton),0,	0,	600) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[2]->skeleton),	hdg,0,0);
	  	TransformStaticMenuObject(menuSystem[(*currMenu)].menuModel[2], NULL);
	}

	hdg += 40;
}


void DrawHiScoreMenu (Menus *menuSystem, int *currMenu, int shipNo, int *swapDelay)
{
	static hdg = 0;

	InitMenuDisplay (90) ;

	if ((shipNo == 0) || (shipNo == 1))
	{
		SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[2]->skeleton),0,0,600) ;	 /*1800*/
		SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[2]->skeleton),hdg,	0,0);
	  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[2], NULL);
	}
	hdg += 40;
}

void DisplayPadMenu (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;
	// jed
	SetSkeletonPosition((menuSystem[(*currMenu)].menu2Model[0]->skeleton),0,0,	6500) ; 
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menu2Model[0]->skeleton),	hdg,0,0);
 	TransformStaticObject(menuSystem[(*currMenu)].menu2Model[0], NULL);
	hdg += 40;
}

void DisplayPadMenu1 (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;
	// jed

	SetSkeletonPosition((menuSystem[(*currMenu)].menu2Model[1]->skeleton),0,-5,	4500) ; 
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menu2Model[1]->skeleton),	hdg,0,0);
 	TransformStaticMenuObject(menuSystem[(*currMenu)].menu2Model[1], NULL);
	hdg += 20;
}

void DisplayPadMenu2 (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;
	// jed

	SetSkeletonPosition((menuSystem[(*currMenu)].menu2Model[2]->skeleton),0,0,	4500) ; 
	SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menu2Model[2]->skeleton),	hdg,0,0);
 	TransformStaticObject(menuSystem[(*currMenu)].menu2Model[2], NULL);
	hdg += 40;
}

void DisplayNegConMenu (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0, shdg = 0 ;

	Object*	a;
	Object*	b;

	InitMenuDisplay ( 90 ) ;	
	

	a = menuSystem[(*currMenu)].menu2Model[0];
	b = menuSystem[(*currMenu)].menu2Model[1];

	SetSkeletonPosition(root,0,-100,	1500) ;	 
	SetSkeletonDirectionHPR(root,	hdg,0,0);

	SetSkeletonPosition( a->skeleton, 0, 0, -8 );	 /*8*/
	SetSkeletonPosition( b->skeleton, 0, 0, 8 ); /*-8*/

	SetSkeletonDirection( a->skeleton, 0, 0, 0 );
	SetSkeletonDirection( b->skeleton, 0, 0, 0 );

	a->skeleton->update = 1 ;
	b->skeleton->update = 1 ;

 	TransformStaticObject(a, NULL);
 	TransformStaticObject(b, NULL);
	
	hdg += 40;
}


void ConfirmFormat (MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int	toggle = 0 ;

	if (neg1.head == 8960)
	{
		DisplayButtons ( 30, 210, NSELECT) ;
		DisplayButtons ( 240, 210, NBACK) ;
	}
	else
	{
		DisplayButtons ( 30, 210, PSELECT) ;
		DisplayButtons ( 240, 210, PBACK) ;
	}

	CentreText ("FORMAT DISK", text_data (70, 20, 8), RedText) ; 
	CentreText ("DO YOU WISH TO FORMAT CARD", text_data (55, 60, 8), WhiteText) ; 
	if ((GetMenuKey(M_LEFT)) || (GetMenuKey(M_RIGHT)))
	{
		selectData->formatConfirm ^= 1 ;
		PlayNote(MENU_MOVE, 1, 20);
	}

	if (selectData->formatConfirm)
	{
		//if ((toggle % 2) == 0)
			AddText ("YES", text_data (80, 120, 16), RedText) ; 
		//else
			//AddText ("YES", text_data (80, 120, 16), WhiteText) ;
		AddText ("NO", text_data (210, 120, 16), WhiteText) ; 
	}
	else
	{
		AddText ("YES", text_data (80, 120, 16), WhiteText) ; 
		//if ((toggle % 2) == 0)
			AddText ("NO", text_data (210, 120, 16), RedText) ; 
		//else
			//AddText ("NO", text_data (210, 120, 16), WhiteText) ;
	}	
	toggle ++ ;
}

void InitChampionship ()
{
	short	i ;

	for (i=0; i < NO_OF_SHIPS; i++)
	{
		winTable[i].points = 0 ;
		winTable[i].racePoints = 0 ;
		winTable[i].pilot[0] = '\0' ;
		winTable[i].xPos = 0 ;
		winTable[i].position = 0 ;
		winTable[i].racePosition = 0 ;
	}
}


void CheckSaveType (SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	LoadCardFiles (selectData, &(loadGames[0]), selectData->cardNum)  ;

	if (((selectData->slotRow*2)+(selectData->slotColumn+1)) == (selectData->slotsFilled+1))
	{
  		if (selectData->nameEntered[0] != 0)
  		{
  			selectData->saveType = NORM ;
		}
	}
	else
	{
		if (selectData->nameEntered[0] != 0)
		{
			if ( strcmp (selectData->nameEntered, loadGames[(selectData->slotRow*2)+selectData->slotColumn]) == 0)
			{
				selectData->saveType = OVERWRITE ;
			}
			else
			{
 	
				selectData->saveType = RENAME ;
			}
		}
	}
}


void InitButtons (char *fileName)
{
	TIMlist  		*timPtr;
	short				tablePos ;
	char 				modelFile[256];
	int				i ;

	strcpy( modelFile, "wipeout\\textures\\" );
	strcat( modelFile, fileName) ;
	strcat( modelFile, ".cmp");

	tablePos = TextureTableCount;

	timPtr = LoadCompressedTextureSequence( modelFile );
	LoadCmpFiles( timPtr ); 

	for (i=0; i < 18; i++)
	{
		buttonTex[i] = TextureTable [tablePos] ;
		tablePos ++ ;
	}
}



void DisplayButtons (int x, int y, int buttType) 
{
		RECT	tw;


		x *= upres;
		y *= upres;

		SetDrawMode(&(drmode[buttType][CurrentScreen]), 0, 0, FONT_TPAGE, &tw);
		AddPrim(OT[CurrentScreen], (ulong *)&(drmode[buttType][CurrentScreen]));

		SetSprt(&(buttons[buttType][CurrentScreen]));			

		buttons[buttType][CurrentScreen].r0=230 ; // PCwipeout
		buttons[buttType][CurrentScreen].g0=0 ;
		buttons[buttType][CurrentScreen].b0=BLACK_NOT_DRAWN ;

		buttons[buttType][CurrentScreen].x0 = x;
		buttons[buttType][CurrentScreen].y0 = y;

		buttons[buttType][CurrentScreen].w = (buttonTex[buttType]->textureW * 4) - 1;
		buttons[buttType][CurrentScreen].h = buttonTex[buttType]->textureH;

		buttons[buttType][CurrentScreen].u0 = buttonTex[buttType]->u0;
		buttons[buttType][CurrentScreen].v0 = buttonTex[buttType]->v0;

  		AddPrim( OT[ CurrentScreen ] , ( ulong* ) &(buttons[buttType][CurrentScreen]) );

		SetDrawMode(&drmode2[buttType][CurrentScreen], 0, 0, 27, &tw);
		AddPrim(OT[CurrentScreen], (ulong *)&drmode2[buttType][CurrentScreen]);
}



void DrawLine (int x, int y, int x1, int y1, POLY_F4 *line, int lineNum) 
{
		SetPolyF4(&(line[lineNum]));			
		line[lineNum].r0=230 ;
		line[lineNum].g0=0 ;
		line[lineNum].b0=BLACK_NOT_DRAWN ;

		setXY4(&(line[lineNum]),x, y, x+2, y,
				x1, y1, x1+2,y1+2 ) ;
	
  	AddPrim( OT[ CurrentScreen ] , ( ulong* ) &(line[lineNum]) );
}


#if !USE_ATI


// THIS FUCKER BETTER WORK NOW

TimData *AJSLoad16BitTexture(char *filename, char location)
{
	TimData	*timSize;
	bm			pcx;

	
	timSize = ( TimData* )DAlloc( heap, sizeof(TimData) );

	pcx.width=320;
	pcx.height=200;
	pcx.bitmap=(char*)DAlloc(heap,(320*200));
	if (pcx.bitmap==NULL)
	{
		printf("Out of memory\n");
		exit(0);
	}

	pcx.palette=shield_palette;

// now load PCX

	ReadPCX(filename,&pcx,PICTURE|PALETTE);

	timSize->textureX = 0 ;
	timSize->textureY = 0 ;
	timSize->textureW = pcx.width ;
	timSize->textureH = pcx.height ;

	while (LockClearBuffer());
	PitchMemCpy320X200(backscreen,&(pcx.bitmap[0]));
	UnlockClearBuffer();
	free(pcx.bitmap);

	return (timSize) ;
}
#endif //USE_ATI

void JJSReadTIM(char *filename , char *location, TimData *tim)
{
	FILE *fp;
	unsigned short pix0,pix1,*data;
	int i;

	fp=fopen(filename,"rb");
	fread(tim,12,1,fp);		// id and flags & bnum
	fread(&tim->textureX,2,1,fp);
	fread(&tim->textureY,2,1,fp);
	fread(&tim->textureH,2,1,fp);
	fread(&tim->textureW,2,1,fp);
	data=(unsigned short *)location;
	for(i=0;i<320*240;++i)
	{
		fread(&pix0,2,1,fp);
		pix1=((pix0&0x1f)<<10)|(pix0&0x3e0)|(pix0>>10);
		*data++=pix1;
	}
	fclose(fp);
}

void QuitYesNo(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int toggle = 0;
	static char flag = 0;

	if (andybodge)
	{
		selectData->confirm = 1;
		andybodge=0;
	}

	DisplayButtons ( 30, 210, PSELECT) ;
	DisplayButtons ( 240, 210, PBACK) ;

	CentreText ("ARE YOU SURE YOU", text_data (77, 95, 8), RedText) ; 
	CentreText ("WANT TO QUIT", text_data (105, 105, 8), RedText) ; 

	if ((GetMenuKey(M_LEFT)) || (GetMenuKey(M_RIGHT)))
	{
		selectData->confirm ^= 1 ;
	}

	if (selectData->confirm)
	{
		AddText ("YES", text_data (80, 120, 16), WhiteText) ; 
		//if ((toggle % 2) == 0)
			AddText ("NO", text_data (210, 120, 16), RedText) ; 
		//else
			//AddText ("NO", text_data (210, 120, 16), WhiteText) ;
	}
	else
	{
		//if ((toggle % 2) == 0)
			AddText ("YES", text_data (80, 120, 16), RedText) ;
		//else
			//AddText ("YES", text_data (80, 120, 16), WhiteText) ;
		AddText ("NO", text_data (210, 120, 16), WhiteText) ; 
	}	

	toggle ++ ;
}


void GraphicOptions(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int toggle = 0;
	static char listpos = 0;
	char	x=50, y=140, l=12;
	char	DrawStr[8];
	char	ResStr[8];
	char	ScreenStr[8];
	char	SkyStr[8];
	char	TexStr[8];
	char	PCStr[8];


	if(andybodge)
	{
		my_ScreenSize = ScreenSize;
		my_Sky = MipMap;
		my_Resolution = Resolution;
		my_DrawDistance = DrawDistance;
		my_TextureTrack = ft;
		my_SkyTrackFlags = SkyTrackFlags;

		andybodge=0;
		inmenu = 0;
		Reallyinmenu = 1;
	}

 	CentreText ("GRAPHIC DETAIL OPTIONS", text_data (1,21, 8), RedText) ;

	DisplayButtons ( 30, 210, PSELECT) ;
	DisplayButtons ( 240, 210, PBACK) ;
	
	if (GetMenuKey(M_DOWN))
	{
		if (listpos==3) 
			listpos = 0;
		else 
			listpos++;
	}

	if (GetMenuKey(M_UP))
	{
		if (listpos==0) 
			listpos = 3;
		else
			listpos--;
	}

	if (GetMenuKey(M_LEFT))
	{
		PlayNote(MENU_SELECT, 1, 20);
		switch(listpos)
		{
			case 0:	if (DrawDistance==HIGH_DISTANCE) 
						DrawDistance=LOW_DISTANCE;
					else if (DrawDistance==MEDIUM_DISTANCE) 
						DrawDistance=HIGH_DISTANCE;
					else if (DrawDistance==LOW_DISTANCE) 
						DrawDistance=MEDIUM_DISTANCE;
					break;
			case 1:
				switch(Resolution)
				{
					case HIGH:
						Resolution=MEDIUM;
						break;
					case MEDIUM:
						Resolution=LOW;
						break;
					case LOW:
						Resolution=SMALL;
						break;
					case SMALL:
						if(memorySize<1)
							Resolution=MEDIUM;
						else
							Resolution=HIGH;
						break;
				}
				break;
			case 2:
				ft--;
				if(ft<0)
					ft=1;
				TextureTrack = 1;
				break;
			case 3:
				Sky ^= 1;
				break;
		}
	}

	if (GetMenuKey(M_RIGHT))
	{
		PlayNote(MENU_SELECT, 1, 20);
		switch(listpos)
		{
			case 0:	if (DrawDistance==HIGH_DISTANCE) DrawDistance=MEDIUM_DISTANCE;
						else
							if (DrawDistance==MEDIUM_DISTANCE) DrawDistance=LOW_DISTANCE;
							else
								if (DrawDistance==LOW_DISTANCE) DrawDistance=HIGH_DISTANCE;
						break;
			case 1:
				switch(Resolution)
				{
					case HIGH:
						Resolution=SMALL;
						break;
					case MEDIUM:
						if(memorySize<1)
							Resolution=SMALL;
						else
							Resolution=HIGH;
						break;
					case LOW:
						Resolution=MEDIUM;
						break;
					case SMALL:
						Resolution=LOW;
						break;
				}
				break;
			case 2:
				ft++;
				if(ft>1)
					ft=0;
				TextureTrack = 1;
				break;
			case 3:
				Sky ^= 1;
		}
	}

	switch(listpos)
	{
		case 0:	AddText ("DRAW DISTANCE", text_data (x, y, 8), RedText);
 			AddText ("RESOLUTION", text_data (x, y+l, 8), WhiteText) ;
 			//AddText ("SCREEN SIZE", text_data (x, y+(l*2), 8), WhiteText) ;
 			//AddText ("SKY", text_data (x, y+(l*2), 8), WhiteText) ;
 			AddText ("TEXTURE FILTER", text_data (x, y+(l*2), 8), WhiteText) ;
 			//AddText ("MIP MAPPING", text_data (x, y+(l*2), 8), WhiteText) ;
 			AddText ("SKY", text_data (x, y+(l*3), 8), WhiteText) ;
			break;
		case 1:	AddText ("RESOLUTION", text_data (x, y+l, 8), RedText) ;
			AddText ("DRAW DISTANCE", text_data (x, y, 8), WhiteText);
 			//AddText ("SCREEN SIZE", text_data (x, y+(l*2), 8), WhiteText) ;
// 			AddText ("SKY", text_data (x, y+(l*2), 8), WhiteText) ;
 			AddText ("TEXTURE FILTER", text_data (x, y+(l*2), 8), WhiteText) ;
// 			AddText ("MIP MAPPING", text_data (x, y+(l*2), 8), WhiteText) ;
 			AddText ("SKY", text_data (x, y+(l*3), 8), WhiteText) ;
			break;
#if 0
		case 3:	//AddText ("MIP MAPPING", text_data (x, y+(l*2), 8), RedText) ;
			AddText ("DRAW DISTANCE", text_data (x, y, 8), WhiteText);
 			AddText ("RESOLUTION", text_data (x, y+l, 8), WhiteText) ;
 			AddText ("TEXTURE FILTER", text_data (x, y+(l*3), 8), WhiteText) ;
 			//AddText ("PERS CORR", text_data (x, y+(l*4), 8), WhiteText) ;
			break;
#endif
		case 2:	AddText ("TEXTURE FILTER", text_data (x, y+(l*2), 8), RedText) ;
			AddText ("DRAW DISTANCE", text_data (x, y, 8), WhiteText);
 			AddText ("RESOLUTION", text_data (x, y+l, 8), WhiteText) ;
 			//AddText ("SCREEN SIZE", text_data (x, y+(l*2), 8), WhiteText) ;
 			//AddText ("SKY", text_data (x, y+(l*2), 8), WhiteText) ;
 			//AddText ("MIP MAPPING", text_data (x, y+(l*2), 8), WhiteText) ;
 			AddText ("SKY", text_data (x, y+(l*3), 8), WhiteText) ;
			break;
		case 3: AddText ("SKY", text_data (x, y+(l*3), 8), RedText) ;
//			AddText ("MIP MAPPING", text_data (x, y+(l*2), 8), WhiteText) ;
			AddText ("DRAW DISTANCE", text_data (x, y, 8), WhiteText);
 			AddText ("RESOLUTION", text_data (x, y+l, 8), WhiteText) ;
 			//AddText ("SCREEN SIZE", text_data (x, y+(l*2), 8), WhiteText) ;
 			AddText ("TEXTURE FILTER", text_data (x, y+(l*2), 8), WhiteText) ;
			break;
	}


	if (DrawDistance==HIGH_DISTANCE) strcpy(DrawStr,"FAR");
	if (DrawDistance==MEDIUM_DISTANCE) strcpy(DrawStr,"MEDIUM");
	if (DrawDistance==LOW_DISTANCE) strcpy(DrawStr,"NEAR");

	switch(Resolution)
	{
		case HIGH:
			strcpy(ResStr,"640X480");
			break;
		case MEDIUM:
			strcpy(ResStr,"512X384");
			break;
		case LOW:
			strcpy(ResStr,"400X300");
			break;
		case SMALL:
			strcpy(ResStr,"320X240");
			break;
	}

#if 0
	if (ScreenSize==FULL) strcpy(ScreenStr,"FULL");
	if (ScreenSize==MEDIUM) strcpy(ScreenStr,"MEDIUM");
	if (ScreenSize==SMALL) strcpy(ScreenStr,"SMALL");


#endif
	if (Sky) strcpy(SkyStr,"ON");
	if (!Sky) strcpy(SkyStr,"OFF");

	switch(ft)
	{
		case 0:
			strcpy(TexStr,"OFF");
			break;
		case 1:
			strcpy(TexStr,"ON");
			break;
	}
	
	AddText (DrawStr, text_data (x+180, y, 8), RedText); 
 	AddText (ResStr, text_data (x+180, y+l, 8), RedText) ; 
 	//AddText (ScreenStr, text_data (x+190, y+(l*2), 8), RedText) ;
 	AddText (SkyStr, text_data (x+180, y+(l*3), 8), RedText) ;
 	AddText (TexStr, text_data (x+180, y+(l*2), 8), RedText) ; 
// 	AddText (PCStr, text_data (x+180, y+(l*3), 8), RedText) ;

	toggle ++ ;
	if(Resolution!=screenres)
	{
		screenres=Resolution;
		changeScreen();
		my_Resolution = screenres;
	}
}



void ControllerOptions(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int toggle = 0;
	char	controllerstring[20];
	char	configstring[20];
	char colour1;
	char colour2;

	
	davebodge = 1;
	andybodge2=1;

	if (andybodge)
	{
		changecontroller=1;
		andybodge=0;
		orig_controller = controller;
	}

 	CentreText ("CONTROLLER OPTIONS", text_data (1,21, 8), RedText) ;

	DisplayButtons ( 30, 210, PSELECT);
	DisplayButtons ( 240, 210, PBACK);

	if (GetMenuKey(M_UP)||GetMenuKey(M_DOWN))
	{
		if (orig_controller != _JOYSTICK1)
		{
			toggle ++ ;
			if (changecontroller) 
				changecontroller=0;
			else 
				changecontroller=1;
		}
	}

	if (changecontroller)
	{
		colour1=RedText;
		colour2=WhiteText;
		if (GetMenuKey(M_LEFT))
		{
			PlayNote(MENU_MOVE, 1, 20);
			if (orig_controller==_KEYBOARD) orig_controller=_JOYSTICK1;
			else
				if (orig_controller==_MOUSE1) orig_controller=_KEYBOARD;
				else
					if (orig_controller==_JOYSTICK1) orig_controller=_MOUSE1;
		}
	
		if (GetMenuKey(M_RIGHT))
		{
			PlayNote(MENU_MOVE, 1, 20);
			if (orig_controller==_KEYBOARD) orig_controller=_MOUSE1;
			else
				if (orig_controller==_MOUSE1) orig_controller=_JOYSTICK1;
				else
					if (orig_controller==_JOYSTICK1) orig_controller=_KEYBOARD;
		}

	}
	else
	{
		if (orig_controller != _JOYSTICK1)
		{
			colour1=WhiteText;
			colour2=RedText;
		}
	}

	switch (orig_controller)
	{
		case _KEYBOARD: strcpy(configstring, "CONFIGURE KEYBOARD");
					   strcpy(controllerstring, "KEYBOARD");
  						CentreText (configstring, text_data (0, 170, 8), colour2) ;
			break;
		case _MOUSE1: strcpy(configstring, "CONFIGURE MOUSE");
					strcpy(controllerstring, "MOUSE");
  						CentreText (configstring, text_data (0, 170, 8), colour2) ;
			break;
		case _JOYSTICK1:
				if (!JJSJoyConfig())
				{
					colour1=WhiteText;
					colour2=RedText;
					strcpy(configstring, "NO JOYSTICK DETECTED");
 					CentreText (configstring, text_data (0, 170, 8), colour2) ;
				}
			   strcpy(controllerstring, "JOYSTICK");

			break;
	}

  CentreText (controllerstring, text_data (0, 50, 8), colour1) ; 

}



void ControllerEdit(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int toggle = 0;
	static char flag = 0;

	DisplayButtons ( 30, 210, PSELECT) ;
	DisplayButtons ( 240, 210, PBACK) ;

	switch(controller)
	{
		case _KEYBOARD:	
			CentreText ("CONFIGURE KEYBOARD", text_data (1,21, 8), RedText) ;
			if(davebodge == 1)
			{
				KeyConfig0();
				davebodge = 0;
			}
			else
			{
				KeyConfig();
			}
			break;

		case _MOUSE1:	
			CentreText ("MOUSE SENSITIVITY", text_data (1,21, 8), RedText) ;
			MouseConfig();
			break;

		case _JOYSTICK1:	
			//CentreText ("CONFIGURE JOYSTICK", text_data (1,21, 8), RedText) ;
			//JoystickConfig();
			break;
	}

	toggle ++ ;
}



void DrawControllerMenu (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0 ;


	InitMenuDisplay (110);

	switch(orig_controller)
	{
	// jed
		case _KEYBOARD:
			etc_Joystick1Switch = _DEVICE_OFF; //jjs quick and dirty
			SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[1]->skeleton),0,	0,	3500) ;	 /*1800*/
			SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[1]->skeleton), hdg,0,	0);
		  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[1], NULL);
			hdg += 20;
			break;

	// jed
		case _MOUSE1:
			SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[0]->skeleton),0,	0,	4500) ;	 /*1800*/
			SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[0]->skeleton),	hdg,75,25);
		  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[0], NULL);
			hdg += 40;
			break;

	// jed
		case _JOYSTICK1:
			SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[2]->skeleton),0,	0,	3500) ;	 /*1800*/
			SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[2]->skeleton),	hdg,-50,0);
		  	TransformStaticObject(menuSystem[(*currMenu)].menuModel[2], NULL);
			hdg += 40;
			break;
	}
}



void KeyConfig0(void)
{
	char	keyc;


	// Keep a copy of the keys
	Keycons[0] = OriginalKeyCodes[15];
	Keycons[1] = OriginalKeyCodes[13];
	Keycons[2] = OriginalKeyCodes[12];
	Keycons[3] = OriginalKeyCodes[14];
	Keycons[4] = OriginalKeyCodes[0];
	Keycons[5] = OriginalKeyCodes[1];
	Keycons[6] = OriginalKeyCodes[6];
	Keycons[7] = OriginalKeyCodes[5];
	Keycons[8] = OriginalKeyCodes[4];
	Keycons[9] = OriginalKeyCodes[11];

	// Clear the main keys
	//
	// Left
	etc_KeyboardMap[etc_KeyboardConfig[15]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[15] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[15]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[15] + 128] = 0xFFFFFFFF;

	// Right
	etc_KeyboardMap[etc_KeyboardConfig[13]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[13] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[13]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[13] + 128] = 0xFFFFFFFF;

	// Up
	etc_KeyboardMap[etc_KeyboardConfig[12]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[12] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[12]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[12] + 128] = 0xFFFFFFFF;

	// Down
	etc_KeyboardMap[etc_KeyboardConfig[14]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[14] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[14]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[14] + 128] = 0xFFFFFFFF;

	// Brake Left
	etc_KeyboardMap[etc_KeyboardConfig[0]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[0] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[0]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[0] + 128] = 0xFFFFFFFF;

	// Brake Right
	etc_KeyboardMap[etc_KeyboardConfig[1]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[1] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[1]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[1] + 128] = 0xFFFFFFFF;

	// Accel
	etc_KeyboardMap[etc_KeyboardConfig[6]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[6] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[6]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[6] + 128] = 0xFFFFFFFF;

	// Fire
	etc_KeyboardMap[etc_KeyboardConfig[5]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[5] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[5]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[5] + 128] = 0xFFFFFFFF;

	// View [triangle]
	etc_KeyboardMap[etc_KeyboardConfig[4]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[4] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[4]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[4] + 128] = 0xFFFFFFFF;

	// Pause [Start]
	etc_KeyboardMap[etc_KeyboardConfig[11]] = 0;
	//etc_KeyboardMap[etc_KeyboardConfig[11] + 128] = 0xFFFFFFFF;
	etc_ExtendedKeyboardMap[etc_KeyboardConfig[11]] = 0;
	//etc_ExtendedKeyboardMap[etc_KeyboardConfig[11] + 128] = 0xFFFFFFFF;

	etc_KeyboardConfig[15] = 0;		// Left
	etc_KeyboardConfig[13] = 0;		// Right
	etc_KeyboardConfig[12] = 0;		// Up
	etc_KeyboardConfig[14] = 0;		// Down
	etc_KeyboardConfig[0] = 0;		// Brake Left
	etc_KeyboardConfig[1] = 0;		// Brake Right
	etc_KeyboardConfig[6] = 0;		// Accel
	etc_KeyboardConfig[5] = 0;		// Fire
	etc_KeyboardConfig[4] = 0;		// Change View
	etc_KeyboardConfig[11] = 0;		// Pause (start)

	for(keyc = 0; keyc < 16; keyc++)
		NewOriginalKeyCodes[keyc] = 0;

	etc_KeyPressed = 0;
	etc_ExtendedFlag = 0;
	etc_Key = 0;
}



void KeyConfig(void)
{
	char			exists;
	unsigned short	KeyCode = 0;


	CentreText ("PRESS KEY FOR", text_data (1, 6*12, 8), RedText) ;

	switch(KeyToChange)
	{
		case NONE:
				if(DelayCounter >= 15)
				{
					etc_KeyPressed = 0;
					etc_Key = 0;
					KeyToChange = LEFT;
					DelayCounter = 0;
				}
				else
					DelayCounter++;

				break;

		case LEFT:
				CentreText ("LEFT", text_data (1, 7*12, 8), WhiteText) ;
				
				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[15] = KeyCode;
							NewOriginalKeyCodes[15] = KeyCode;
							SetKey(15, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case RIGHT:
				CentreText ("RIGHT", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[13] = KeyCode;
							NewOriginalKeyCodes[13] = KeyCode;
							SetKey(13, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case UP:
				CentreText ("UP", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[12] = KeyCode;
							NewOriginalKeyCodes[12] = KeyCode;
							SetKey(12, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case DOWN:
				CentreText ("DOWN", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[14] = KeyCode;
							NewOriginalKeyCodes[14] = KeyCode;
							SetKey(14, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case BRAKEL:
				CentreText ("LEFT AIRBRAKE", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[0] = KeyCode;
							NewOriginalKeyCodes[0] = KeyCode;
							SetKey(0, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case BRAKER:
				CentreText ("RIGHT AIRBRAKE", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[1] = KeyCode;
							NewOriginalKeyCodes[1] = KeyCode;
							SetKey(1, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case ACCEL:
				CentreText ("ACCELERATE", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[6] = KeyCode;
							NewOriginalKeyCodes[6] = KeyCode;
							SetKey(6, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case FIRE:
				CentreText ("FIRE", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[5] = KeyCode;
							NewOriginalKeyCodes[5] = KeyCode;
							SetKey(5, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case VIEW_KEY:
				CentreText ("CHANGE VIEW", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[4] = KeyCode;
							NewOriginalKeyCodes[4] = KeyCode;
							SetKey(4, KeyCode);
							KeyToChange++;
						}
					}
				}

				break;

		case PAUSE_KEY:
				CentreText ("PAUSE", text_data (1, 7*12, 8), WhiteText) ;

				if(etc_KeyPressed)
				{
					KeyCode = GetKeyNoBlock();

					if(KeyCode > 0)
					{
						exists = CheckKey(KeyCode);

						if(exists)
						{
							PlayNote(MENU_SELECT, 1, 20);
							OriginalKeyCodes[11] = KeyCode;
							NewOriginalKeyCodes[11] = KeyCode;
							SetKey(11, KeyCode);
							KeyToChange = NONE;
							davebodge = 2;
						}
					}
				}

				break;
	}
}

void	MouseConfig(void)
{
	static int toggle = 0;
	char	x=50, y=140, l=12;

	if (andybodge2)
	{
		cur_mouse_setting=mouse_sensitivity;
		andybodge2=0;
	}


	if (GetMenuKey(M_UP))
	{
		if (cur_mouse_setting==HIGH_SENS) cur_mouse_setting=LOW_SENS;
		else
			if (cur_mouse_setting==MEDIUM_SENS) cur_mouse_setting=HIGH_SENS;
			else
				if (cur_mouse_setting==LOW_SENS) cur_mouse_setting=MEDIUM_SENS;
	}

	if (GetMenuKey(M_DOWN))
	{
		if (cur_mouse_setting==HIGH_SENS) cur_mouse_setting=MEDIUM_SENS;
		else
			if (cur_mouse_setting==MEDIUM_SENS) cur_mouse_setting=LOW_SENS;
			else
				if (cur_mouse_setting==LOW_SENS) cur_mouse_setting=HIGH_SENS;
	}

#ifdef PORT_MOUSE
	if (GetMenuKey(M_SELECT))
	{
		mouse_sensitivity=cur_mouse_setting;

		etc_regs.w.ax = 0x000F;				// Sensitivity
		etc_regs.w.cx = mouse_sensitivity;	// Default 8
		etc_regs.w.dx = 16;					// Default 16
		int386(_MOUSE_INT, &etc_regs, &etc_regs);

		davebodge=2;
	}

#else
	if (GetMenuKey(M_SELECT))
	{
		mouse_sensitivity=cur_mouse_setting;
		davebodge=2;
	}
#endif

	switch(cur_mouse_setting)
	{
		case HIGH_SENS:
			CentreText ("HIGH", text_data (x, y, 8), RedText); 
		 	CentreText ("MEDIUM", text_data (x, y+(l), 8), WhiteText) ; 
		 	CentreText ("LOW", text_data (x, y+(l*2), 8), WhiteText) ; 
			break;
		case MEDIUM_SENS:
			CentreText ("HIGH", text_data (x, y, 8), WhiteText) ; 
			CentreText ("MEDIUM", text_data (x, y+l, 8), RedText) ; 
 			CentreText ("LOW", text_data (x, y+(l*2), 8), WhiteText) ; 
			break;
		case LOW_SENS:
			CentreText ("HIGH", text_data (x, y, 8), WhiteText) ; 
 			CentreText ("MEDIUM", text_data (x, y+(l), 8), WhiteText) ; 
			CentreText ("LOW", text_data (x, y+(l*2), 8), RedText) ; 
			break;
	}

	toggle ++ ;
}



void	JoystickConfig(void)
{
	char	old_controller;

#if 0
	if(!etc_JoystickHandler)
	{
		old_controller = etc_InputDeviceSelect;
		etc_InputDeviceSelect = _JOYSTICK1;
		StartPAD();
		etc_InputDeviceSelect = old_controller;
	}

	pmjoy();

	switch(JoystickCalibrationMode)
	{
		case TOPLEFT:
				CentreText ("MOVE JOYSTICK TO TOP LEFT", text_data (1, 6*12, 8), RedText);
				CentreText ("AND PRESS FIRE", text_data (1, 7*12, 8), RedText);

				if((xpad & PadCross0) || (xpad & PadCircle0))
				{
					JoyMinX = etc_Joy1.jX - JOY_BODGE;
					JoyMinY = etc_Joy1.jY - JOY_BODGE;

					if(JoyMinX < 0)
						JoyMinX = 0;
					
					if(JoyMinY < 0)
						JoyMinY = 0;
					
					JoystickCalibrationMode = BOTTOMRIGHT;
					PlayNote(MENU_SELECT, 1, 20);
				}

				break;

		case BOTTOMRIGHT:
				CentreText ("MOVE JOYSTICK TO BOTTOM RIGHT", text_data (1, 6*12, 8), RedText);
				CentreText ("AND PRESS FIRE", text_data (1, 7*12, 8), RedText);

				if((xpad & PadCross0) || (xpad & PadCircle0))
				{
					JoyMaxX = etc_Joy1.jX + JOY_BODGE;
					JoyMaxY = etc_Joy1.jY + JOY_BODGE;
					
					JoystickCalibrationMode = CENTRE;
					PlayNote(MENU_SELECT, 1, 20);
				}

				break;

		case CENTRE:
				CentreText ("CENTRE JOYSTICK AND PRESS FIRE", text_data (1, 6*12, 8), RedText);

				if((xpad & PadCross0) || (xpad & PadCircle0))
				{
					JoyCentreX = etc_Joy1.jX;
					JoyCentreY = etc_Joy1.jY;
					
					left_scale = JoyCentreX - JoyMinX;		// 0 - left_scale;
					right_scale = JoyMaxX - JoyCentreX;		// 0 - right_scale;
					left_scale_factor = 128.0 / (float)left_scale;
					right_scale_factor = 128.0 / (float)right_scale;

					JoyCentreFudge = (left_scale + right_scale) / 32;

/*
					ResetVMode();
					printf("Min X    = %d\n", JoyMinX);
					printf("Max X    = %d\n", JoyMaxX);
					printf("Min Y    = %d\n", JoyMinY);
					printf("Max Y    = %d\n", JoyMaxY);
					printf("Centre X = %d\n", JoyCentreX);
					printf("Centre Y = %d\n", JoyCentreY);

					printf("left_scale = %d\n", left_scale);
					printf("right_scale = %d\n", right_scale);
					printf("left_scale_factor = %f\n", left_scale_factor);
					printf("right_scale_factor = %f\n", right_scale_factor);
					getch();
*/
					JoystickCalibrationMode = TOPLEFT;
					PlayNote(MENU_SELECT, 1, 20);
					davebodge = 2;
				}

				break;
	}
#endif
	JJSJoyConfig();
	JoyMinX = 0;
	PlayNote(MENU_SELECT, 1, 20);
	davebodge = 2;
	changecontroller = 0;
}



void DrawControllerEdit (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static hdg = 0 ;

	InitMenuDisplay (80);

	switch(controller)
	{
		case _MOUSE1:
			SetSkeletonPosition((menuSystem[(*currMenu)].menuModel[0]->skeleton),0,	0,	6000) ;	 /*1800*/
			SetSkeletonDirectionHPR((menuSystem[(*currMenu)].menuModel[0]->skeleton),	hdg,0,0);
			TransformStaticObject(menuSystem[(*currMenu)].menuModel[0], NULL);
			switch(cur_mouse_setting)
			{
				case HIGH_SENS:		hdg += 120;	break;	  
				case MEDIUM_SENS:	hdg += 40;	break;
				case LOW_SENS:		hdg += 10;	break;
			}

			break;
	}

}

void DrawGraphicDetail (Menus *menuSystem, int *currMenu, int shapeNo, int *swapDelay)
{
	static int heading = 0, hdg = 0 ;


	DrawTeamShips (0, 0, MODELSIZE, &heading, menuSystem[(*currMenu)].menuModel[shipShapeOrder[0]]) ;
}


void MostRecentSaveFile(char *filename)
{
	DIR *dirp;
	struct dirent *direntp;
	unsigned short int d_time=0;
	unsigned short int d_date=0;
	char tempname[_MAX_PATH2];
	char *ext, *search;
	char fred[_MAX_PATH2];

	filename[0]=0; // initialise it to NULL
	
	dirp=opendir("wipeout");
	if (dirp==NULL)	return;
							
	for (; ; )
	{
		direntp=readdir(dirp);
		if (direntp==NULL) break;

// look for .wsf files only
		_splitpath2(direntp->d_name,tempname,NULL,NULL,NULL,&ext);

		if (!(strcmpi(ext,".wsf")))
		{
			if (direntp->d_date>d_date)
			{
				d_date=direntp->d_date;
				d_time=direntp->d_time;
				strcpy(filename,direntp->d_name);
			}
			else
			{
				if (direntp->d_date==d_date)
				{
					if (direntp->d_time>d_time)
					{
				  		 d_time=direntp->d_time;
						 strcpy(filename,direntp->d_name);
					}
				}
			}
		}
	}

	closedir(dirp);
	if (filename[0]!=0)
	{
		search = (char *)strrchr(filename,'.');
		if (search != NULL)
			search[0]= 0;
	}
}



char CheckKey(unsigned short code)
{
	if(NewOriginalKeyCodes[15] == code)
		return(0);
	else if(NewOriginalKeyCodes[13] == code)
		return(0);
	else if(NewOriginalKeyCodes[12] == code)
		return(0);
	else if(NewOriginalKeyCodes[14] == code)
		return(0);
	else if(NewOriginalKeyCodes[0] == code)
		return(0);
	else if(NewOriginalKeyCodes[1] == code)
		return(0);
	else if(NewOriginalKeyCodes[6] == code)
		return(0);
	else if(NewOriginalKeyCodes[5] == code)
		return(0);
	else if(NewOriginalKeyCodes[4] == code)
		return(0);
	else if(NewOriginalKeyCodes[11] == code)
		return(0);

	if(code == 0x1c)
		return(0);

	return(1);
}



void ResetKeys(void)
{
	OriginalKeyCodes[15] = Keycons[0];
	OriginalKeyCodes[13] = Keycons[1];
	OriginalKeyCodes[12] = Keycons[2];
	OriginalKeyCodes[14] = Keycons[3];
	OriginalKeyCodes[0] = Keycons[4];
	OriginalKeyCodes[1] = Keycons[5];
	OriginalKeyCodes[6] = Keycons[6];
	OriginalKeyCodes[5] = Keycons[7];
	OriginalKeyCodes[4] = Keycons[8];
	OriginalKeyCodes[11] = Keycons[9];

	SetKey(15, OriginalKeyCodes[15]);
	SetKey(13, OriginalKeyCodes[13]);
	SetKey(12, OriginalKeyCodes[12]);
	SetKey(14, OriginalKeyCodes[14]);
	SetKey(0,  OriginalKeyCodes[0]);
	SetKey(1,  OriginalKeyCodes[1]);
	SetKey(6,  OriginalKeyCodes[6]);
	SetKey(5,  OriginalKeyCodes[5]);
	SetKey(4,  OriginalKeyCodes[4]);
	SetKey(11, OriginalKeyCodes[11]);
}
#if 0
void NetworkOptions(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int toggle = 0;
	static option = 1;
	char colour1;
	char colour2;
	char players_colour[10] = 	{ 1,1,1,1,1,1,1,1,1,1 };
	char socket_colour[10] = 	{ 1,1,1,1,1,1,1,1,1,1 };
	static char player_number;
	static char socket_number;

 	CentreText ("CONFIGURE NETWORK", text_data (1,21, 8), RedText) ;

	DisplayButtons ( 30, 210, PSELECT);
	DisplayButtons ( 240, 210, PBACK);

	if (andybodge)
	{
		andybodge=0;
		player_number=CommNetPlayers;
		socket_number=CommNetSocket;
	}
	players_colour[player_number]=RedText;
	socket_colour[socket_number]=RedText;

	if (GetMenuKey(M_UP)||GetMenuKey(M_DOWN))
	{
		option ^=1;
	}



	if (option)	// number of players
	{
		colour1=toggle%2;
		colour2=WhiteText;

		if (GetMenuKey(M_RIGHT))
		{
			players_colour[player_number]=WhiteText;
			player_number++;
			if (player_number>8) player_number=2;
			players_colour[player_number]=RedText;
		}
		if (GetMenuKey(M_LEFT))
		{
			players_colour[player_number]=WhiteText;
			player_number--;
			if (player_number<2) player_number=8;
			players_colour[player_number]=RedText;
		}
	}
	else // socket number
	{
		colour1=WhiteText;
		colour2=toggle%2;

		if (GetMenuKey(M_RIGHT))
		{
			socket_colour[socket_number]=WhiteText;
			socket_number++;
			if (socket_number>9) socket_number=0;
			socket_colour[socket_number]=RedText;
		}
		if (GetMenuKey(M_LEFT))
		{
			socket_colour[socket_number]=WhiteText;
			if (socket_number==0) socket_number=10;
			socket_number--;
			socket_colour[socket_number]=RedText;
		}
	}

  CentreText ("NUMBER OF PLAYERS", text_data (0, 80, 8), colour1) ; 
  CentreText ("SELECT SOCKET", text_data (0, 140, 8), colour2) ; 

	AddNumber(2, text_data(65,96,8), players_colour[2]);
	AddNumber(3, text_data(95,96,8), players_colour[3]);
	AddNumber(4, text_data(125,96,8), players_colour[4]);
	AddNumber(5, text_data(155,96,8), players_colour[5]);
	AddNumber(6, text_data(185,96,8), players_colour[6]);
	AddNumber(7, text_data(215,96,8), players_colour[7]);
	AddNumber(8, text_data(245,96,8), players_colour[8]);

	AddNumber(0, text_data(20,156,8),  socket_colour[0]);
	AddNumber(1, text_data(50,156,8),  socket_colour[1]);
	AddNumber(2, text_data(80,156,8),  socket_colour[2]);
	AddNumber(3, text_data(110,156,8),  socket_colour[3]);
	AddNumber(4, text_data(140,156,8), socket_colour[4]);
	AddNumber(5, text_data(170,156,8), socket_colour[5]);
	AddNumber(6, text_data(200,156,8), socket_colour[6]);
	AddNumber(7, text_data(230,156,8), socket_colour[7]);
	AddNumber(8, text_data(260,156,8), socket_colour[8]);
	AddNumber(9, text_data(290,156,8), socket_colour[9]);

	if (GetMenuKey(M_SELECT))
	{
		CommNetPlayers=player_number;
		CommNetSocket=socket_number;
		CommMultiPlaySelect=_TCP_NETWORK;
		davebodge=2;
	}

	toggle++;
}



void SerialOptions(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	static int toggle = 0;
	static option = 1;
	char colour1;
	char colour2;
	char comm_colour[5] = 	{1,1,1,1,1};
	char baud_colour[3] = 	{1,1,1};
	static char comm_port;
	static char baud_rate;

 	CentreText ("CONFIGURE SERIAL", text_data (1,21, 8), RedText) ;

	DisplayButtons ( 30, 210, PSELECT);
	DisplayButtons ( 240, 210, PBACK);

	if (andybodge)
	{
		andybodge=0;
		comm_port = CommSerialDevice;
		baud_rate=CommSerialBaudRate;
	}

	comm_colour[comm_port]=RedText;
	baud_colour[baud_rate]=RedText;

	if (GetMenuKey(M_UP)||GetMenuKey(M_DOWN))
	{
		option ^=1;
	}



	if (option)	// comm port
	{
		colour1=toggle%2;
		colour2=WhiteText;

		if (GetMenuKey(M_RIGHT))
		{
			comm_colour[comm_port]=WhiteText;
			comm_port++;
			if (comm_port > 4) comm_port = 1;
			comm_colour[comm_port]=RedText;
		}
		if (GetMenuKey(M_LEFT))
		{
			comm_colour[comm_port]=WhiteText;
			comm_port--;
			if (comm_port == 0) comm_port = 4;
			comm_colour[comm_port]=RedText;
		}
	}
	else // socket number
	{
		colour1=WhiteText;
		colour2=toggle%2;

		if (GetMenuKey(M_RIGHT))
		{
			baud_colour[baud_rate]=WhiteText;
			baud_rate++;
			if (baud_rate > 2) baud_rate=1;
			baud_colour[baud_rate]=RedText;
		}
		if (GetMenuKey(M_LEFT))
		{
			baud_colour[baud_rate]=WhiteText;
			if (baud_rate==0) baud_rate=2;
			baud_rate--;
			baud_colour[baud_rate]=RedText;
		}
	}

  CentreText ("COMM PORT", text_data (0, 80, 8), colour1) ; 
  CentreText ("BAUD RATE", text_data (0, 140, 8), colour2) ; 

	AddText("COM", text_data(46, 96, 8), comm_colour[1]);
	AddNumber(1, text_data(86,96,8), comm_colour[1]);

	AddText("COM", text_data(106, 96, 8), comm_colour[2]);
	AddNumber(2, text_data(146,96,8), comm_colour[2]);

	AddText("COM", text_data(166, 96, 8), comm_colour[3]);
	AddNumber(3, text_data(206,96,8), comm_colour[3]);

	AddText("COM", text_data(226, 96, 8), comm_colour[4]);
	AddNumber(4, text_data(266,96,8), comm_colour[4]);


	AddNumber(57600, text_data(80,156,8),  baud_colour[1]);
	AddNumber(115200, text_data(180,156,8),  baud_colour[2]);

	if (GetMenuKey(M_SELECT))
	{
		CommSerialDevice=comm_port;
		CommSerialBaudRate = baud_rate;
		CommMultiPlaySelect=_HEAD_2_HEAD;
		davebodge=2;
	}

	toggle++;
}



void	SerialNetworkHandling(MusicPrims *musicPrims, SlideTextures *musicTex, SelectionData *selectData, char loadGames[][9], ConfigData *gameData)
{
	global_mark_error = 0;
	gameType = SINGLE;

	if(davebodge != 4)
		MasterSlaveSIO(musicPrims,musicTex,selectData,loadGames,gameData);

	if(global_mark_error)
		davebodge = 3;
}


#endif