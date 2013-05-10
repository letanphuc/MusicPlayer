
/* File name	: music.h
 * Project 		: MUSIC PLAYER
 * Version 		: 1.0
 * Creation Date: 23/03/2013
 * Tabsize		: 4
 * Author  		: Group PHM                  
 * Copyright	: (c) 2013
 * License		: Free.
 * Comments		: 
 * 
 */

#ifndef	__MUSIC_H__
#define	__MUSIC_H__

/*====================================*/
/* Prototype declaration      		  */
/*====================================*/
#include "fatfs/src/ff.h"
#include "fatfs/src/diskio.h"

extern unsigned int minute , second ,micro_second;


#define CHOOSE_SONG     0
#define PLAY_SONG_LYRIC 1
#define WAITING         2

int music(void);


#endif


/****************END FILE**************/

