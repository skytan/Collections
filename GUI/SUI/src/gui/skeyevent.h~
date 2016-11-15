#ifndef SKEYEVENT_H
#define SKEYEVENT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sevent.h"
#include "sed200_key.h"
/*
#define KEY_UP		(0x41+0x80)
#define KEY_DOWN	(0x42+0x80)
#define KEY_RIGHT	(0x43+0x80)
#define KEY_LEFT	(0x44+0x80)

#define KEY_BACKSPACE	0x08
#define KEY_TAB		0x9
#define KEY_ADD		L'+'
#define KEY_SUB		L'_'
#define KEY_QUIT	0x1b
#define KEY_ENTER	0xd
#define KEY_DIT		L'.'
#define KEY_DELETE	0x7f
#define KEY_SALE	0x4
#define KEY_PRINT	0xf4
*/

class sKeyEvent: public sEvent
{
public:
	sKeyEvent(char *tty = 0, sKeyEvent *e = NULL);
	virtual ~sKeyEvent();

	virtual class sEvent* select();

private:
	struct termios termnew, termold;
	int fd;
};


#endif
