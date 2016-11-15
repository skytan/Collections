#include <stdlib.h>
#include "skeyevent.h"
#include "sperr.h"

sKeyEvent::sKeyEvent(char *tty, sKeyEvent *e):sEvent(e)
{
	char *ttyname;
	if( !e ) 
	{
		type = EVENT_TYPE_KEY;
		value = 0;
		mask = 0;
	}


	if( tty )
	{
		ttyname = tty;
	}
	else if( !(ttyname =  getenv("TTY")) )
	{
		ttyname = (char*)"/dev/tty";
	}
	
	if( (fd = open(ttyname, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0 )
	{
		perr("%s", "");
	}

	tcgetattr(fd, &termold);

	bzero(&termnew, sizeof(termnew));
	termnew.c_cflag = termold.c_cflag;
	termnew.c_iflag = IGNPAR;
	termnew.c_oflag = termold.c_oflag;
	termnew.c_cc[VTIME] = 0;
	termnew.c_cc[VMIN] = 0;

	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &termnew);

}

sKeyEvent::~sKeyEvent()
{
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &termold);
}

class  sEvent* sKeyEvent::select()
{
	int ret;
	struct timeval tv;
	fd_set set;
	
	FD_ZERO(&set);
	FD_SET(fd, &set);
	
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	
	type = 0;
	
	ret = ::select(fd+1, &set, 0, 0, &tv);
	if( ret == -1 )
	{
		perr("%s", "");
		return 0;
	}
	else if( ret )
	{
		type = EVENT_TYPE_KEY;		
	}
	else
	{
		type = EVENT_TYPE_TIMER;
		print("%d", type);
		return this;
	}
	
	value = 0;
	read(fd, &value, 4);

	if( value > 0xffff )
	{
		value >>= 16;
		value += 0x80;
	}
	print("0x%x", value);
	
	return this;
}
