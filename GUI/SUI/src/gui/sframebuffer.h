#ifndef SFRAMEBUFFER_H
#define SFRAMEBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>


class sFramebuffer
{
public:
	sFramebuffer();
	~sFramebuffer();

	void reversePoint(int x, int y);
	void setXy(int x, int y, int val);

	void clearScreen();

	int getFbXres();
	int getFbYres();
	int getBitlen();
	int getLinelen();
	
	void setXline(int x, int y, int size, bool val);

protected:
	void setPoint(int x, int y, unsigned val);
	unsigned char *getName();
/*	
	void setCharBit(unsigned char *p, int len);
	void setCharBitBig(unsigned char *p, int len);
	void clearCharBit(unsigned char *p, int len);
	void clearCharBitBig(unsigned char *p, int len);
	
	void setBit(void *addr, unsigned offset, unsigned  len, bool val);
*/
private:
	struct fb_fix_screeninfo m_fix;
	struct fb_var_screeninfo m_var;

	unsigned char *m_addr;
	unsigned int m_size;
	
	int m_fd;
};

#endif
