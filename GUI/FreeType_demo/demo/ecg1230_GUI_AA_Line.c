
#include "math.h"

extern int BackColor;


/******************************************************************
* 函数: unsigned char GUI_GetRColor()
* 功能: 获取指定颜色中红色的数值.
* 参数: int color ---- 指定颜色
* 说明: 该组函数只对ECG1230系统作用, 即颜色配置为(16位: 5R5G5B-, 555-, 高位->低位)
******************************************************************/
#define GUI_GetRColor(color)  (((color)>>11) & 0x001f)


/******************************************************************
* 函数: unsigned char GUI_GetGColor()
* 功能: 获取指定颜色中绿色的数值.
* 参数: int color ---- 指定颜色
******************************************************************/
#define GUI_GetGColor(color)    (((color)>>6) & 0x001f)


/******************************************************************
* 函数: unsigned char GUI_GetRColor()
* 功能: 获取指定颜色中蓝色的数值.
* 参数: int color ---- 指定颜色
******************************************************************/
#define GUI_GetBColor(color)  (((color)>>1) & 0x001f)


/******************************************************************
* 函数: GUI_RGB()
* 功能: 将指定的red,green,blue色元合成16位颜色.
******************************************************************/
#define  GUI_RGB(r, g, b)  (((r) << 11) & 0xf800) | (((g) << 6)  & 0x07c0) | (((b) << 1) & 0x003e) 


/******************************************************************
* 函数: GUI_GetAAColor()
* 功能: 计算反走样直线上某个点所分配的颜色值.
* 参数: int value     ---- 实际点到理论点的距离在总距离中所占的比例;
*       int max       ---- 像素对的总距离;
*       int fullColor ---- 设定的直线颜色值;
*       int backColor ---- 实际点处于的背景颜色;
* 返回: int ---- 点所应分配的颜色值
* 说明: 理论上像素对总距离max为1, value值大于0小于1. 为了避免浮点运算,
*       将数据放大处理,总距离放大为max(int), value也放大相同倍数,所占
*       比例还是不变.通过所占比例计算分配颜色的深浅.
******************************************************************/
/*
int GUI_GetAAColor(int value, int max, int fullColor, int backColor)
{
	unsigned char red, green, blue;
	
	//value = value + (max/5);         //根据实际需要, 适当加深前景色
	//value = (value > max)?max:value;
	
	red = (unsigned char)((max - value)*GUI_GetRColor(backColor)/max) + 
	      (unsigned char)(value*GUI_GetRColor(fullColor)/max);
	      
	green = (unsigned char)((max - value)*GUI_GetGColor(backColor)/max) + 
	        (unsigned char)(value*GUI_GetGColor(fullColor)/max);
	        
	blue = (unsigned char)((max - value)*GUI_GetBColor(backColor)/max) + 
	       (unsigned char)(value*GUI_GetBColor(fullColor)/max);
	
	return GUI_RGB(red, green, blue);
}*/

#define   GUI_GetAAColor(value, max, fullColor, backColor)                         \
                                                                                   \
	GUI_RGB((unsigned char)(((max) - (value)) * GUI_GetRColor(backColor)/(max)) +  \
	        (unsigned char)((value) * GUI_GetRColor(fullColor)/(max)),             \
	        (unsigned char)(((max) - (value)) * GUI_GetGColor(backColor)/(max)) +  \
	        (unsigned char)((value) * GUI_GetGColor(fullColor)/(max)),             \
	        (unsigned char)(((max) - (value)) * GUI_GetBColor(backColor)/(max)) +  \
	        (unsigned char)((value) * GUI_GetBColor(fullColor)/(max))              \
	        )                                                                     


void GUI_HLine(int x0, int y0, int x1, int color, int value, int max)
{
    int  bak, bgColor;

    if(x0>x1)
    {
        bak = x1;
        x1  = x0;
        x0  = bak;
    }

    do{
        bgColor = LCD_GetPixel(x0,y0);
        LCD_PutPixel(x0, y0, GUI_GetAAColor(value, max, color, bgColor));
        x0++;
    } while(x1>=x0);
}

void GUI_VLine(int x0, int y0, int y1, int color, int value, int max)
{
    int  bak, bgColor;

    if(y0>y1)
    {
        bak = y1;
        y1  = y0;
        y0  = bak;
    }

    do{
        bgColor = LCD_GetPixel(x0,y0);
        LCD_PutPixel(x0, y0, GUI_GetAAColor(value, max, color, bgColor));
        y0++;
    } while(y1>=y0);
}



/******************************************************************
* 函数: void ECG_DrawAALine()
* 功能: 绘制一条反走样的直线.
* 参数: int x1,y1 ---- 起点坐标;
*       int x2,y2 ---- 终点坐标;
*       int color ---- 直线颜色;
* 返回: 无
* 说明: 针对心电图波形数据在画波形时, 进行了基线加粗及消抖动处理.
******************************************************************/
void ECG_DrawAALine(int x1, int y1, int x2, int y2, int color)
{
	int dx, dy;
	int temp;
	int k;
	unsigned char red, green, blue;
	int backColor;
	
	/*  在心电图系统不可能存在这种情况
	if (x1 == x2)
	{
		return;
	}*/
	
	/* 处理水平线, 加粗*/
	if (y1 == y2)
	{
		//GUI_HLine(x1, y1-1, x2, color, 2, 10);
		GUI_HLine(x1, y1,   x2, color, 8, 10);  //表达式中的数字决定点颜色,
		GUI_HLine(x1, y1+1, x2, color, 2, 10);  //可根据实际情况修改.
		return;
	}
	
	dx = x2 - x1;
	dy = y2 - y1;
	
	
	if (abs(dx) == abs(dy))  //处理k=+-1时,处理;同时抑制基线抖动
	{
		int xs;
		int yt;
		
		if (dx < 0)
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		dx = x2 - x1;
		dy = y2 - y1;
			
		yt = y1;
		
		
		k = dy;
		
		//起点
		backColor = LCD_GetPixel(x1, y1);
		LCD_PutPixel(x1, y1,   GUI_GetAAColor(6, 10, color, backColor));    //表达式中的数字决定点颜色,
		backColor = LCD_GetPixel(x1, y1+k);
		LCD_PutPixel(x1, y1+k, GUI_GetAAColor(4, 10, color, backColor));    //可根据实际情况修改.
		
		for (xs=x1+1; xs<x2; xs++)
		{
			backColor = LCD_GetPixel(xs, yt);
			LCD_PutPixel(xs, yt,   GUI_GetAAColor(6, 10, color, backColor));
			backColor = LCD_GetPixel(xs, yt+1);
			LCD_PutPixel(xs, yt+1, GUI_GetAAColor(4, 10, color, backColor));
			yt += k;
		}
		
		//终点
		backColor = LCD_GetPixel(x2, y2);
		LCD_PutPixel(x2, y2,   GUI_GetAAColor(6, 10, color, backColor));
		backColor = LCD_GetPixel(x2, y2-k);
		LCD_PutPixel(x2, y2-k, GUI_GetAAColor(4, 10, color, backColor));
		
	}
	else if (abs(dx) > abs(dy))  /*以x为步进*/
	{
		int xs;
		int yt;
		int distance;
		
		if (dx < 0)
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		dx = x2 - x1;
		dy = y2 - y1;
		
		k = (dy * 1000) / dx;         //放大1000倍,避免浮点运算.注意:防止溢出!
		
		yt = y1 * 1000;
		
		for (xs=x1; xs<=x2; xs++)
		{
			distance = yt%1000;
			
			backColor = LCD_GetPixel(xs, (yt/1000));
			LCD_PutPixel(xs, (yt/1000),   GUI_GetAAColor(1000-distance, 1000, color, backColor));
			backColor = LCD_GetPixel(xs, (yt/1000)+1);
			LCD_PutPixel(xs, (yt/1000)+1, GUI_GetAAColor(distance, 1000, color, backColor));
			
			yt += k;
		}
	}
	else        /*以y为步进*/
	{
		int ys;
		int xt;
		int distance;
		
		if (dy < 0)
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		dx = x2 - x1;
		dy = y2 - y1;
		
		k = (dx * 1000) / dy;           //放大1000倍,避免浮点运算
		
		xt = x1 * 1000;
		
		for (ys=y1; ys<y2; ys++)
		{
			distance = xt%1000;
			
			backColor = LCD_GetPixel((xt/1000), ys);
			LCD_PutPixel((xt/1000),   ys, GUI_GetAAColor(1000-distance, 1000, color, backColor));
			backColor = LCD_GetPixel((xt/1000)+1, ys);
			LCD_PutPixel((xt/1000)+1, ys, GUI_GetAAColor(distance, 1000, color, backColor));
			
			xt += k;
		}
	}

}