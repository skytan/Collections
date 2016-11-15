
#include "..\trueType\ft2build.h"
#include "..\trueType\freetype.h"

#include "gb_unicode.h"
#include "TTF_func.h"
#include "color16Func.h"

extern char *ttfAddrBase;
extern unsigned long ttfSize;

/*******************************************************
*����: TTF_Init()
*˵��: ���γ�ʼ��,ָ���������ش�С.
*����: int fontWidth, int fontHeight ---- ���ο������ش�С
*      unsigned char bitmap          ---- ������ʽ,��:��������
*����: �ɹ�0, ʧ��-1
*******************************************************/
int TTF_Init(int fontWidth, int fontHeight, unsigned char bitmap)
{
	FT_UInt       glyph_index;	
    
	/*��ʼ����*/
	ft_error = FT_Init_FreeType( &ft_Lib );
	if ( ft_error )
	{
		printf("Error at init ft_Lib!\n");
		FT_Done_FreeType(ft_Lib);
		return -1; 
	}
	
	/*װ��һ������face, �����ַ�ʽ:1Ϊ���ڴ�װ��,2��Ӳ�̼���*/
	ft_error = FT_New_Memory_Face( ft_Lib,
								ttfAddrBase, /* ����ĵ�һ���ֽ� */
								ttfSize, /* ����Ĵ�С�����ֽڱ�ʾ�� */
								0, /* face ���� */
								&face );
	
	//ft_error = FT_New_Face(ft_Lib,   //Ӳ�̼��ط�ʽ
	//					"/sd/0/SIMSUN.TTC",
	//					0,
	//					&face );
	if ( ft_error )
	{
		printf("New face error!\n");
		FT_Done_FreeType(ft_Lib);
		return -1; 
	}
		
	/*ֱ��ָ���������ش�С  */
	ft_error = FT_Set_Pixel_Sizes(face, //face������
                               fontWidth,    //���ؿ��
                               fontHeight ); //���ظ߶� 	
	if (ft_error)
	{
		printf("Error at set fontSize!\n");
		return -1;
	}
	
	/*��������ͼ��ʽ��*/
	/*�����У�FT_LOAD_DEFAULT   -- Ĭ�Ϸ�ʽ
	          FT_LOAD_RENDER    -- ������
	          FT_LOAD_NO_BITMAP -- �������������ͼ��
	  �����ο��ĵ�
	*/
	TTF_bitmap_type = bitmap;
	
	
	/*�任����,��������б��*/
	matrix.xx = 1 << 16;
	matrix.xy = 0x5800;     //ֵԽ��,б��Խ��
	matrix.yx = 0;
	matrix.yy = 1 << 16;
    
    logMsg("init ok.\n");
	return 0;
}

/*******************************************************
*����: ft_draw_bitmap()
*˵��: ����ʵ��ϵͳ�������ַ����Ƶ�Ŀ����,�û�����Ҫ���ô˺���.
*����: FT_Bitmap *bitmap      ----  ���ε�ͼ���ַ
*      int x, int y           ----  ���λ��Ƶ�Ŀ��λ��
*      U16 color              ----  ���ε���ɫ
*
*ע: ��freetyp�����������ʱ, ͨ��FT_LOAD_NO_BITMAP����,������
*    �����ص������ͣ�Ҳ���Լ��ص������͡�����ڻ�ͼ��ʱ��������
*    ��ʽ,��Ӧ���������case��
*    1.��������ʽ:ÿ����ռ��һ���ֽ�,��ֵΪ�õ�ĻҶ�ֵ.
*    2.����ʽ�� ÿ����ռ��һ��λ,��ֵֻ��Ϊ0 �� 1.
*******************************************************/
static void ft_draw_bitmap(FT_Bitmap *bitmap, int x, int y, U16 color)
{
	int i, j;
	short RGB555;
	unsigned char gray;	
	
	switch (bitmap->pixel_mode)
	{
	case FT_PIXEL_MODE_GRAY://���ط�������ʽʱ
		for ( i = 0 ; i < bitmap->rows; i++)            //bitmap->rows Ϊ����ͼ�������
		{
        	for ( j = 0 ; j < bitmap->width; j++)       //bitmap->widthΪ����ͼ��ÿ�еĵ���
        	{
        		gray = bitmap->buffer[i * bitmap->pitch + j];  //bitmap->pitchΪÿ�е��ֽ���
		
				if (gray > 0)       //ͨ������������޸�, �ɾ����Ƿ����ʹ�÷�����
				{
					LCD_PutPixel(x + j, y + i, GUI_GetAAColor(gray, 255, color, 0xfffe));
				}
			}
		}
		break;
	case FT_PIXEL_MODE_MONO:/*�̶�����ʽʱ */
		for ( i = 0 ; i < bitmap->rows; i++)           //bitmap->rows Ϊ����ͼ�������
		{
			for(j = 0; j < bitmap->width; j++ )        //bitmap->widthΪ����ͼ��ÿ�еĵ���
			{
				LCD_PutPixel(x + j, y + i,             //bitmap->pitchΪÿ�е��ֽ���
							(((bitmap->buffer[i * bitmap->pitch + j / 8]) & (0x80 >> (j & 7))) ? color : 0xfffe)
							);
			}
		}
		break;        	
	}
	//logMsg("draw ok.\n");
}

/*******************************************************
*����: InternalCode2Unicode()
*˵��: ��������תΪunicode
*����: ���ֵ�16λ����,��һ�ֽ��ڸ�λ,�ڶ��ֽ��ڵ�λ
*����: ���ֵ�unicode
*******************************************************/
static U16 InternalCode2Unicode(U16 iCode)
{
	unsigned char hByte, lByte;
	
	/* ����תΪ��λ�� */
	lByte = (iCode & 0x00ff) - 0xA0;           //λ��
	hByte = ((iCode >> 8) & 0x00ff) - 0xA0;    //����
	
	
	return unicodeTable[hByte - 16][lByte];  /* ���������� */
}

/*******************************************************
*����: TTF_DisplayUnicode()
*˵��: ��ָ��λ����ʾһ���ַ�.
*����: int x, int y                ---- ��ʾ����;
*      const unsigned char *text   ---- �ַ����׵�ַ;
*      int num                     ---- �ַ�����;
*      U16 color                   ---- �ַ�����ɫ;
*      unsigned char style         ---- ��ʾ��ʽ:����(FONT_NORMAL),б��(FONT_ITALIC),����(FONT_BOLD)
*����: �ɹ�0, ʧ��-1
*******************************************************/
int TTF_DisplayUnicode(int x, int y, const unsigned char *text, int num, U16 color, unsigned char style)
{
	int i;
	U16 fontCode;
	FT_GlyphSlot slot = face->glyph;  /* һ��С�ݾ� */;
	
	for (i = 0; i < num; i++)
	{
		if (style & FONT_BOLD)//����
		{
			FT_Pos strength = (1 << 6); //ֵԽ��Խ��
     		FT_Outline_Embolden(&face->glyph->outline, strength);
		}
		
		if (style & FONT_ITALIC)//б��
		{
			FT_Set_Transform( face, &matrix, &pen );
		}
		
		/*����ת��*/
		fontCode = text[i];		
		if (fontCode >> 7) //����
		{
			fontCode = text[i] << 8;     //������ڸ�8λ
			fontCode |= text[++i];       //λ����ڵ�8λ
			fontCode = InternalCode2Unicode(fontCode);
		}
		
		/* װ������ͼ�����βۣ�����Ĩ����ǰ������ͼ�� */
		/* �����ص����ֿ� */
		ft_error = FT_Load_Char( face, fontCode, 
		                         TTF_bitmap_type ); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
		if ( ft_error )
		{
			printf("Error at load char!\n");
			return -1;
		}		
		
		if (style & FONT_BOLD)//����ʱ,���󻭲�,����ģ������޷��Ӵ�
		{
			FT_GlyphSlot_Embolden( slot );
		}
		
		/* ���ڣ����Ƶ�Ŀ�����(surface) */
		ft_draw_bitmap( &slot->bitmap,
						x + slot->bitmap_left,
						y - slot->bitmap_top,     /*ˮƽ����ʱ�������,��������ٷ����εĹ涨*/
						color
						);
		
		/* ���ӱ�λ�� */
		x += slot->advance.x >> 6;
		//y += slot->advance.y >> 6;
		
	}	
	return 0;
}