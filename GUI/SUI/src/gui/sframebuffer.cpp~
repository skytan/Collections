#include "sframebuffer.h"
#include "sperr.h"


sFramebuffer::sFramebuffer()
{
	char *env = getenv("FRAMEBUFFER");
	if( !env ) env = (char*)"/dev/fb0";

	m_fd = open(env, O_RDWR);

	if( m_fd < 0)
        {
                perr("%s", "");
                exit(-1);
        }

        if(ioctl(m_fd, FBIOGET_FSCREENINFO, &m_fix) == -1)
        {
                close(m_fd);
		perr("%s", "ioctl FBIOGET_FSCREENINFO");  
                exit(-1);
        }

        if(ioctl(m_fd, FBIOGET_VSCREENINFO, &m_var) == -1)
        {
                close(m_fd);
		perr("%s", "ioctl FBIOGET_VSCREENINFO");
                exit(-1);
        }

        m_size = m_var.yres * m_fix.line_length;
        m_addr = (unsigned char *)mmap(NULL, m_size, PROT_READ|PROT_WRITE,MAP_SHARED, m_fd, 0);
        if( !m_addr )
        {
              perr("%s", "");
              exit(-102);
        }
        
//        perr("xres:%d\tyres:%d", m_var.xres, m_var.yres);

}

sFramebuffer::~sFramebuffer()
{
	close(m_fd);
}

/*
static inline void set1Bit(unsigned char *p, int bit, unsigned val)
{
	struct b1{
		unsigned char b0:1;
		unsigned char b1:1;
		unsigned char b2:1;
		unsigned char b3:1;
		unsigned char b4:1;
		unsigned char b5:1;
		unsigned char b6:1;
		unsigned char b7:1;
	} *pb1 = (struct b1*)p;
	
	switch(bit)
	{
		case 0:
			pb1->b0 = val;
			break;
		case 1:
			pb1->b1 = val;
			break;
		case 2:
			pb1->b2 = val;
			break;
		case 3:
			pb1->b3 = val;
			break;
		case 4:
			pb1->b4 = val;
			break;
		case 5:
			pb1->b5 = val;
			break;
		case 6:
			pb1->b6 = val;
			break;
		case 7:
			pb1->b7 = val;
			break;
		default:
			break;
	}
}

static inline void set2Bit(unsigned char *p, int bit, unsigned val)
{
	struct b2{
		unsigned char b0:2;
		unsigned char b1:2;
		unsigned char b2:2;
		unsigned char b3:2;
	}*pb1 = (struct b2*)p;;

	switch(bit)
	{
		case 0:
			pb1->b0 = val;
			break;
		case 1:
			pb1->b1 = val;
			break;
		case 2:
			pb1->b2 = val;
			break;
		case 3:
			pb1->b3 = val;
			break;
		default:
			break;
	}
}

static inline void set4Bit(unsigned char *p, int bit, unsigned val)
{
	struct b4{
		unsigned char b0:4;
		unsigned char b1:4;
	}*pb1 = (struct b4*)p;;

	switch(bit)
	{
		case 0:
			pb1->b0 = val;
			break;
		case 1:
			pb1->b1 = val;
			break;
		default:
			break;
	}
}
*/

void sFramebuffer::setPoint(int x, int y, unsigned val)
{
	int pos;
	if( x >= m_var.xres || y >= m_var.yres || x < 0 || y < 0 ) return;

	pos = y*m_var.xres + x;
	

	switch(m_var.bits_per_pixel)
	{
		case 1:
			if(val & 1)
			{
				m_addr[pos>>3] |= (1<<(pos&0x7));
			}
			else
			{
				m_addr[pos>>3] &= ( ~(1<<(pos&0x7)) );
			}
			break;
		case 2:
			pos <<= 1;
			m_addr[pos>>3] &= (3<<(pos&7));
			m_addr[pos>>3] |= ((val&3)<<(pos&7));
			break;
		case 4:
			if( pos & 0x1 )
			{
				m_addr[pos>>1] &= 0x0f;
				m_addr[pos>>1] |= (0xf0&val);
			}
			else
			{
				m_addr[pos>>1] &= 0xf0;
				m_addr[pos>>1] |= (0x0f&val);
			}
			break;
		case 8:
			*(unsigned char*)(m_addr+pos) = (unsigned char)val;
			break;
		case 16:
			*(unsigned short*)(m_addr+(pos<<1)) = (unsigned short)val;
			break;
		case 32:
			*(unsigned int*)(m_addr+(pos<<2)) = val;
			break;
		default:
			break;
	}	
}

void sFramebuffer::reversePoint(int x, int y)
{
	int pos;
	if( x >= m_var.xres || y >= m_var.yres || x < 0 || y < 0 ) return;

	pos = y*m_var.xres + x;
	

	switch(m_var.bits_per_pixel)
	{
		case 1:
			m_addr[pos>>3] ^= (0x01<< (pos & 0x7) );
			break;
		case 2:
			m_addr[ pos>>2 ] ^= (0x03<< ((pos<<1) & 0x7) );
			break;
		case 4:
			m_addr[pos>>1] ^= (0x0f<< ((pos<<2) & 0x7) );
			break;
		case 8:
			*(unsigned char*)(m_addr+pos) = ~*(unsigned char*)(m_addr+pos);
			break;
		case 16:
			*(unsigned short*)(m_addr+(pos<<1)) = ~*(unsigned short*)(m_addr+(pos<<1));
			break;
		case 32:
			*(unsigned int*)(m_addr+(pos<<2)) = ~*(unsigned int*)(m_addr+(pos<<2));
			break;
		default:
			break;
	}	
}

void sFramebuffer::setXy(int x, int y, int val)
{
#if SCONFIG_REVERSE_COLOUR
 	if(!val)
	{
		setPoint(x, y, 0xffffffff);
	}
	else
	{
		setPoint(x, y, 0);
	}
#else
	if(val)
	{
		setPoint(x, y, 0xffffffff);
	}
	else
	{
		setPoint(x, y, 0);
	}
#endif
}

static inline void setCharBit(unsigned char *p, int len)
{
	switch( len )
	{
		case 1:
			*p |= 0x1;
			break;
		case 2:
			*p |= 0x3;
			break;
		case 3:
			*p |= 0x7;
			break;
		case 4:
			*p |= 0xf;
			break;
		case 5:
			*p |= 0x1f;
			break;
		case 6:
			*p |= 0x3f;
			break;
		case 7:
			*p |= 0x7f;
			break;
		default:
			break;
	}
}

static inline void setCharBitBig(unsigned char *p, int len)
{
	switch( len )
	{
		case 1:
			*p |= 0x80;
			break;
		case 2:
			*p |= 0xc0;
			break;
		case 3:
			*p |= 0xe0;
			break;
		case 4:
			*p |= 0xf0;
			break;
		case 5:
			*p |= 0xf8;
			break;
		case 6:
			*p |= 0xfc;
			break;
		case 7:
			*p |= 0xfe;
			break;
		default:
			break;
	}
}

static inline void clearCharBit(unsigned char *p, int len)
{
	switch( len )
	{
		case 1:
			*p &= 0xfe;
			break;
		case 2:
			*p &= 0xfc;
			break;
		case 3:
			*p &= 0xf8;
			break;
		case 4:
			*p &= 0xf0;
			break;
		case 5:
			*p &= 0xe0;
			break;
		case 6:
			*p &= 0xc0;
			break;
		case 7:
			*p &= 0x80;
			break;
		default:
			break;
	}
}

static inline void clearCharBitBig(unsigned char *p, int len)
{
	switch( len )
	{
		case 1:
			*p &= 0x7f;
			break;
		case 2:
			*p &= 0x3f;
			break;
		case 3:
			*p &= 0x1f;
			break;
		case 4:
			*p &= 0xf;
			break;
		case 5:
			*p &= 0x7;
			break;
		case 6:
			*p &= 0x3;
			break;
		case 7:
			*p &= 0x1;
			break;
		default:
			break;
	}
}

	
static inline void setBit(unsigned char *p, unsigned offset, unsigned  len, bool val)
{
	const unsigned char bit_table[8][8] = {
		{0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff},
		{0x00, 0x02, 0x06, 0x0e, 0x1e, 0x3e, 0x7e, 0xfe},
		{0x00, 0x00, 0x04, 0x0c, 0x1c, 0x3c, 0x7c, 0xfc},
		{0x00, 0x00, 0x00, 0x08, 0x18, 0x38, 0x78, 0xf8},
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 0x70, 0xf0},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0xe0},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80}
	};

	if( !len ) return;
	
	if( val )
	{
		if( ((offset + len - 1) >> 3) > (offset >> 3) )
		{
			p[ offset >> 3 ] |= bit_table[ offset & 0x7 ][ 7 ];
			memset(p + (offset >> 3) + 1, 0xff, ((offset + len) >> 3) - ((offset >> 3) + 1));
			p[ (offset + len - 1) >> 3 ] |= bit_table[ 0 ][ (offset + len - 1) & 0x7 ];
		}else
		{
			p[ offset >> 3 ] |= bit_table[ offset & 0x7 ][ (offset + len - 1) & 0x7 ];
		}

	}else
	{
		if( ((offset + len - 1) >> 3) > (offset >> 3) )
		{
			p[ offset >> 3 ] &= ~bit_table[ offset & 0x7 ][ 7 ];
			memset(p + (offset >> 3) + 1, 0x0, ((offset + len) >> 3) - ((offset >> 3) + 1));
			p[ (offset + len - 1) >> 3 ] &= ~bit_table[ 0 ][ (offset + len - 1) & 0x7 ];
		}else
		{
			p[ offset >> 3 ] &= ~bit_table[ offset & 0x7 ][ (offset + len - 1) & 0x7 ];
		}
	}
}	

void sFramebuffer::setXline(int x, int y, int size, bool val)
{
	if( x >= m_var.xres || y >= m_var.yres || x < 0 || y < 0 ) return;
	size = MIN(m_var.xres - x, size);
/*	
	for(int i = x; i < x + size; i++)
	{
		setXy(i, y, val);
	}
	
	return;*/
#if SCONFIG_REVERSE_COLOUR
 	setBit(m_addr, (y*m_var.xres + x) * m_var.bits_per_pixel, size * m_var.bits_per_pixel, !val);
#else
	setBit(m_addr, (y*m_var.xres + x) * m_var.bits_per_pixel, size * m_var.bits_per_pixel, val);
#endif
}

int sFramebuffer::getFbXres()
{
	return m_var.xres;
}

int sFramebuffer::getFbYres()
{
	return m_var.yres;
}

int sFramebuffer::getBitlen()
{
	return m_var.bits_per_pixel;
}

int sFramebuffer::getLinelen()
{
	return m_fix.line_length;
}

void sFramebuffer::clearScreen()
{
#if SCONFIG_REVERSE_COLOUR
	memset(m_addr, 0xff, m_size);
#else
	memset(m_addr, 0x0, m_size);
#endif
}


