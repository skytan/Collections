#include <stdio.h>
#include "category.h"
#include "../perr.h"

int category_init()
{
	struct category cate;
	int	i;
	FILE	*pf;
	
	if( !(pf = fopen(CATEGORY_FILE_NAME, "wb")) )
	{
		perr();
		return -1;
	}
	
	bzero(&cate, sizeof(cate));
	
	for( i = 0; i <= CATEGORY_MAN_COUNT; i++) fwrite(&cate, 1, sizeof(cate), pf);
	
	fclose(pf);
	
	return 0;
}

int category_read(struct category *pcate)
{
	struct category cate;
	int	number;
	FILE	*pf;
	
	if( pcate->iNumber < 0 || pcate->iNumber > CATEGORY_MAN_COUNT ) return -1;
	
	if( !(pf = fopen(CATEGORY_FILE_NAME, "rb")) )
	{
		perr();
		return -2;
	}
	
	number = pcate->iNumber;
	
	if( fseek(pf, number * sizeof(struct category), SEEK_SET) )
	{
		perr();
		fclose(pf);
		return -3;
	}
	
	if( fread(pcate, sizeof(struct category), 1, pf) != 1 )
	{
		perr();
		fclose(pf);
		return -4;
	}
	
	fclose(pf);
	
	return number == pcate->iNumber ? 0 : -5;
}

int category_write(struct category *pcate)
{
	FILE	*pf;
		
	if( pcate->iNumber < 0 || pcate->iNumber > CATEGORY_MAN_COUNT ) return -1;
	
	if( !(pf = fopen(CATEGORY_FILE_NAME, "rb+")) )
	{
		category_init();
		if( !(pf = fopen(CATEGORY_FILE_NAME, "rb+")) )
		{
			perr();
			return -2;
		}
	}
	
	if( fseek(pf, pcate->iNumber * sizeof(struct category), SEEK_SET) )
	{
		perr();
		fclose(pf);
		return -3;
	}
	
	if( fwrite(pcate, sizeof(struct category), 1, pf) != 1 )
	{
		perr();
		fclose(pf);
		return -4;
	}
	
	fclose(pf);
	
	return 0;
}