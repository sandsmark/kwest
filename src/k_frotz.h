/*
 * k_frotz.h
 *
 * KDE interface, declarations, definitions, and defaults
 *
 */

#ifndef K_FROTZ_H
#define K_FROTZ_H

#define MASTER_CONFIG		"frotz.conf"
#define USER_CONFIG		".frotzrc"

#define ASCII_DEF		1
#define ATTRIB_ASSIG_DEF	0
#define ATTRIB_TEST_DEF		0
#define COLOR_DEF		1
#define ERROR_HALT_DEF		0
#define EXPAND_DEF		0
#define PIRACY_DEF		0
#define TANDY_DEF		0
#define OBJ_MOVE_DEF		0
#define OBJ_LOC_DEF		0
#define BACKGROUND_DEF		BLUE_COLOUR
#define FOREGROUND_DEF		WHITE_COLOUR
#define HEIGHT_DEF		-1	/* let curses figure it out */
#define CONTEXTLINES_DEF	0
#define WIDTH_DEF		80
#define TWIDTH_DEF		80
#define SEED_DEF		-1
#define SLOTS_DEF		MAX_UNDO_SLOTS
#define LMARGIN_DEF		0
#define RMARGIN_DEF		0
#define ERR_REPORT_DEF		ERR_REPORT_ONCE
#define	QUETZAL_DEF		1
#define SAVEDIR_DEF		"if-saves"
#define ZCODEPATH_DEF		"/usr/games/zcode:/usr/local/games/zcode"

#define LINELEN		256	/* for getconfig()	*/
#define COMMENT		'#'	/* for config files	*/
#define PATHSEP		':'	/* for pathopen()	*/
#define DIRSEP		'/'	/* for pathopen()	*/

#define EDITMODE_EMACS	0
#define EDITMODE_VI	1

#define PIC_NUMBER	0
#define PIC_WIDTH	2
#define PIC_HEIGHT	4
#define PIC_FLAGS	6
#define PIC_DATA	8
#define PIC_COLOUR	11

#define DEL_CHAR  155
#define EOL_CHAR  156
#define HOME_CHAR 157
#define END_CHAR  158
#define INS_CHAR  159
#define PGUP_CHAR 160
#define PGDN_CHAR 161

/* Paths where z-files may be found */
#define	PATH1		"ZCODE_PATH"
#define PATH2		"INFOCOM_PATH"

#define NO_SOUND
#ifdef OSS_SOUND
# undef NO_SOUND
#endif

#include "kwestview.h"
#include <stdio.h>

extern bool unix_init_pictures(); /* ux_pic */
extern char stripped_story_name[FILENAME_MAX+1];
extern char semi_stripped_story_name[FILENAME_MAX+1];
extern char *progname;
extern char *gamepath;	/* use to find sound files */

struct frotz_config
{
  bool enable_color;
  bool force_color;
  int fg_color;
  int bg_color;
  int width;
  int height;
  int seed;
  bool tandy;
  bool piracy;
};

extern KwestView* global_kwestview;


#endif
