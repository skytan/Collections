#ifndef CATEGORY_H
#define CATEGORY_H

struct category
{
	int	iNumber;
	int	iTaxIndex;
	int	iTaxItems[4];
	char	szName[64];
};//__attribute__ ((__packed__));
	
	
	
	

int category_init();

int category_read(struct category *ptrt);
int category_write(struct category *ptr);
int category_export();
int category_ixport();

#define CATEGORY_MAN_COUNT	200
#define CATEGORY_FILE_NAME	"/flash2/data/category1.dat"

#endif
