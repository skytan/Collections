#include "printer.h"
#include "../perr.h"
#include "../config.h"

int printer_open(struct printer *ptrPri)
{
	struct termios newtio;
	int ttySfp;
   
	ttySfp = open("/dev/ttyS3", O_RDWR | O_NOCTTY);
	if (ttySfp < 0) {
		return -1;
	}

	tcgetattr(ttySfp, &ptrPri->oldtio);	
	newtio = ptrPri->oldtio;

#ifdef PRINTER_MPM78M
	newtio.c_cflag = B57600 | CS8 | CLOCAL | CREAD;	//57600,n,8,1
#else
	newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;	//9600,n,8,1
#endif
	newtio.c_iflag = IGNPAR;	// Ignore bytes with parity errors and make terminal raw and dumb.} 
	newtio.c_oflag = 0;
    /* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;	/* inter-character timer unused */
	newtio.c_cc[VMIN] = 0;	/* return anyway  */
	
	tcflush(ttySfp, TCIOFLUSH);
	tcsetattr(ttySfp, TCSANOW, &newtio);
	 
	return ttySfp;
}

int printer_close(struct printer *ptrPri)
{
	tcflush(ptrPri->fd, TCIOFLUSH);
	tcsetattr(ptrPri->fd, TCSANOW, &ptrPri->oldtio);

	return 0;
}	
