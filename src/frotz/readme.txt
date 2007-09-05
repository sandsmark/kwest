Modifications from original frotz code:

-remove getopt.c

-renamed main.c to frotz_main.c and changed start of main function to

int frotz_main(const char* filename)
{
    strcpy(story_name, filename);

    os_process_arguments(0, (char *)0);

    init_buffer (); 

-add include <string.h> to frotz_main.c

-instantiate f_setup in frotz_main.c:
f_setup_t f_setup;

-escape the definition of bool in frotz.h

#ifndef __cplusplus
typedef int bool;
#endif

-decorate interface headers in frotz.h with

#ifdef __cplusplus
extern "C"
{
#endif

..

#include "setup.h"

#ifdef __cplusplus
}
#endif

-add prototypes of os_read_mouse, os_path_open to interface functions in 
frotz.h, 

-try to improve erro handling somewhat: extern int frotz_fatal_error, checks in
 frotz_main, fast_mem.c

-frotz.h and frotz_main.cpp: char story_name[MAX_FILE_NAME]

-frotz.h, frotz_main.cpp, k_init.cpp: added char short_name[MAX_FILE_NAME]
 to give a good name to the saved game files.
