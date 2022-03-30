/* (C) Psygnosis 1994. By Jason Carl Denton & Rob Smith
*/


#include "standard.h"
#include <stdio.h>

#include "malloc.h"
#include "tim.h"
#include "object.h"
#include "file.h"
#include "ttf.h"
#include "track.h"
#include "ships.h"
#include "weapon.h"
#include "control.h"
#include "droid.h"
#include "camera.h"
#include "sound.h"
#include "sparks2.h"
#include "global.h"
#include "error.h"


TexTemplate* LoadTtfFile( char *file, short *count, short libraryTextures )
{
   long        	length;
   char*       	ttfFile;
	short*			tex;
	short				i;

   length = FileLength( file );
   if ( length <= 0 )
   {
      sprintf( errorString, "Ttf.c:LoadTtfFile: Ttf file %s not found\n", file );
      Error( errorString, Fatal );
   }

   ttfFile = DAlloc( heap, length + 32 );

   if ( !ttfFile )
   {
   /* Memory Allocation Failure! */

      sprintf( "Ttf.c:LoadTtfFile: Failed to allocate memory for file %s\n", file );
      Error( errorString, Fatal );
   }

   if ( LoadFile( file, ttfFile ) != length )
   {
   /* File Error! */

      sprintf( errorString, "Ttf.c:LoadTtfFile: Failed to load file %s\n", file );
      DFree( heap, ttfFile );
      Error( errorString, Fatal );
   }

	tex = ( short* ) ttfFile;
	while ( tex < ( ( short* )( ttfFile + length ) ) )
	{
#if LoadMessages
//		printf( "TTF %2d: ", *count );
#endif
	 	for ( i=0; i<21; i++ )
		{
			*tex = ( ( *tex & 0x00ff ) << 8 ) + 
				 	( ( *tex & 0xff00 ) >> 8 );
#if 0
         *tex += libraryTextures;
#endif

#if LoadMessages
//			printf( "%3d ", *tex );
#endif
			tex += 1;
		}
      (*count)++;               /* count of textures */
#if LoadMessages
//		printf( "\n" );
#endif
	}

   return( ( TexTemplate* ) ttfFile );
}