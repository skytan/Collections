
#include "..\trueType\ft2build.h"
#include "..\trueType\freetype.h"
#include "TTF_func.h"
#include "stat.h"


char *ttfAddrBase;
unsigned long ttfSize;



void main(void)
{
	int i;
	char text[] = "��ǰ����ɽɽ�������������и����к��ж���һ��������ʢ��ˮˮ������������ð����������Ź�";
	FILE *fp;
	struct stat pStat;
	
	LCD_FillColor(0xfffe);
	sdmmcDevOpen();
	//cd("/sd/0/");
	
	stat("/sd/0/SIMSUN.TTC", &pStat);
	ttfSize = pStat.st_size;
	
	ttfAddrBase = (char *)malloc(ttfSize);
	
	printf("size: %d\n", ttfSize);
	
	if ((fp = fopen("/sd/0/SIMSUN.TTC", "r")) == NULL)
		printf("open file error.\n");
	
	fread(ttfAddrBase, ttfSize, 1, fp);
	
	fclose(fp);
	
	TTF_Init(0, 16, FT_LOAD_NO_BITMAP | FT_LOAD_RENDER);
	
	for (i = 20; i < 590; i+=20)	
	{
		TTF_DisplayUnicode(5, i, text, sizeof(text)-1, 0, FONT_NORMAL);
	}
	
	FT_Done_Face(face);
	FT_Done_FreeType( ft_Lib );
	
	free(ttfAddrBase);
	ttfAddrBase = NULL;
}