
#include "..\trueType\ft2build.h"
#include "..\trueType\freetype.h"

#include "gb_unicode.h"
#include "TTF_func.h"
#include "color16Func.h"

extern char *ttfAddrBase;
extern unsigned long ttfSize;

/*******************************************************
*函数: TTF_Init()
*说明: 字形初始化,指定字形像素大小.
*参数: int fontWidth, int fontHeight ---- 字形宽、高像素大小
*      unsigned char bitmap          ---- 字形样式,如:反走样等
*返回: 成功0, 失败-1
*******************************************************/
int TTF_Init(int fontWidth, int fontHeight, unsigned char bitmap)
{
	FT_UInt       glyph_index;	
    
	/*初始化库*/
	ft_error = FT_Init_FreeType( &ft_Lib );
	if ( ft_error )
	{
		printf("Error at init ft_Lib!\n");
		FT_Done_FreeType(ft_Lib);
		return -1; 
	}
	
	/*装载一个字体face, 有两种方式:1为从内存装载,2从硬盘加载*/
	ft_error = FT_New_Memory_Face( ft_Lib,
								ttfAddrBase, /* 缓存的第一个字节 */
								ttfSize, /* 缓存的大小（以字节表示） */
								0, /* face 索引 */
								&face );
	
	//ft_error = FT_New_Face(ft_Lib,   //硬盘加载方式
	//					"/sd/0/SIMSUN.TTC",
	//					0,
	//					&face );
	if ( ft_error )
	{
		printf("New face error!\n");
		FT_Done_FreeType(ft_Lib);
		return -1; 
	}
		
	/*直接指定字体像素大小  */
	ft_error = FT_Set_Pixel_Sizes(face, //face对象句柄
                               fontWidth,    //象素宽度
                               fontHeight ); //象素高度 	
	if (ft_error)
	{
		printf("Error at set fontSize!\n");
		return -1;
	}
	
	/*设置字体图像式样*/
	/*参数有：FT_LOAD_DEFAULT   -- 默认方式
	          FT_LOAD_RENDER    -- 反走样
	          FT_LOAD_NO_BITMAP -- 不加载字体点阵图像
	  其他参考文档
	*/
	TTF_bitmap_type = bitmap;
	
	
	/*变换矩阵,用于设置斜休*/
	matrix.xx = 1 << 16;
	matrix.xy = 0x5800;     //值越大,斜度越大
	matrix.yx = 0;
	matrix.yy = 1 << 16;
    
    logMsg("init ok.\n");
	return 0;
}

/*******************************************************
*函数: ft_draw_bitmap()
*说明: 根据实际系统将生成字符绘制到目标上,用户不需要调用此函数.
*参数: FT_Bitmap *bitmap      ----  字形的图像地址
*      int x, int y           ----  字形绘制的目标位置
*      U16 color              ----  字形的颜色
*
*注: 在freetyp里面加载字形时, 通过FT_LOAD_NO_BITMAP设置,可设置
*    不加载点阵字型，也可以加载点阵字型。因此在画图形时，有两种
*    方式,对应下面的两个case。
*    1.反走样方式:每个点占用一个字节,其值为该点的灰度值.
*    2.点阵方式： 每个点占用一个位,其值只能为0 或 1.
*******************************************************/
static void ft_draw_bitmap(FT_Bitmap *bitmap, int x, int y, U16 color)
{
	int i, j;
	short RGB555;
	unsigned char gray;	
	
	switch (bitmap->pixel_mode)
	{
	case FT_PIXEL_MODE_GRAY://加载反走样方式时
		for ( i = 0 ; i < bitmap->rows; i++)            //bitmap->rows 为字形图像的行数
		{
        	for ( j = 0 ; j < bitmap->width; j++)       //bitmap->width为字形图像每行的点数
        	{
        		gray = bitmap->buffer[i * bitmap->pitch + j];  //bitmap->pitch为每行的字节数
		
				if (gray > 0)       //通过在这里进行修改, 可决定是否采用使用反走样
				{
					LCD_PutPixel(x + j, y + i, GUI_GetAAColor(gray, 255, color, 0xfffe));
				}
			}
		}
		break;
	case FT_PIXEL_MODE_MONO:/*固定点阵方式时 */
		for ( i = 0 ; i < bitmap->rows; i++)           //bitmap->rows 为字形图像的行数
		{
			for(j = 0; j < bitmap->width; j++ )        //bitmap->width为字形图像每行的点数
			{
				LCD_PutPixel(x + j, y + i,             //bitmap->pitch为每行的字节数
							(((bitmap->buffer[i * bitmap->pitch + j / 8]) & (0x80 >> (j & 7))) ? color : 0xfffe)
							);
			}
		}
		break;        	
	}
	//logMsg("draw ok.\n");
}

/*******************************************************
*函数: InternalCode2Unicode()
*说明: 中文内码转为unicode
*参数: 汉字的16位内码,第一字节在高位,第二字节在低位
*返回: 汉字的unicode
*******************************************************/
static U16 InternalCode2Unicode(U16 iCode)
{
	unsigned char hByte, lByte;
	
	/* 内码转为区位码 */
	lByte = (iCode & 0x00ff) - 0xA0;           //位码
	hByte = ((iCode >> 8) & 0x00ff) - 0xA0;    //区码
	
	
	return unicodeTable[hByte - 16][lByte];  /* 根据码表调整 */
}

/*******************************************************
*函数: TTF_DisplayUnicode()
*说明: 在指定位置显示一串字符.
*参数: int x, int y                ---- 显示坐标;
*      const unsigned char *text   ---- 字符串首地址;
*      int num                     ---- 字符个数;
*      U16 color                   ---- 字符串颜色;
*      unsigned char style         ---- 显示样式:正常(FONT_NORMAL),斜体(FONT_ITALIC),粗体(FONT_BOLD)
*返回: 成功0, 失败-1
*******************************************************/
int TTF_DisplayUnicode(int x, int y, const unsigned char *text, int num, U16 color, unsigned char style)
{
	int i;
	U16 fontCode;
	FT_GlyphSlot slot = face->glyph;  /* 一个小捷径 */;
	
	for (i = 0; i < num; i++)
	{
		if (style & FONT_BOLD)//粗体
		{
			FT_Pos strength = (1 << 6); //值越大，越粗
     		FT_Outline_Embolden(&face->glyph->outline, strength);
		}
		
		if (style & FONT_ITALIC)//斜体
		{
			FT_Set_Transform( face, &matrix, &pen );
		}
		
		/*编码转换*/
		fontCode = text[i];		
		if (fontCode >> 7) //中文
		{
			fontCode = text[i] << 8;     //区码放在高8位
			fontCode |= text[++i];       //位码放在低8位
			fontCode = InternalCode2Unicode(fontCode);
		}
		
		/* 装载字形图像到字形槽（将会抹掉先前的字形图像） */
		/* 不加载点阵字库 */
		ft_error = FT_Load_Char( face, fontCode, 
		                         TTF_bitmap_type ); /*  FT_LOAD_NO_BITMAP | FT_LOAD_RENDER */
		if ( ft_error )
		{
			printf("Error at load char!\n");
			return -1;
		}		
		
		if (style & FONT_BOLD)//粗体时,增大画槽,必需的，否则无法加粗
		{
			FT_GlyphSlot_Embolden( slot );
		}
		
		/* 现在，绘制到目标表面(surface) */
		ft_draw_bitmap( &slot->bitmap,
						x + slot->bitmap_left,
						y - slot->bitmap_top,     /*水平对齐时必需如此,具体参数官方字形的规定*/
						color
						);
		
		/* 增加笔位置 */
		x += slot->advance.x >> 6;
		//y += slot->advance.y >> 6;
		
	}	
	return 0;
}