
#include "math.h"

extern int BackColor;


/******************************************************************
* ����: unsigned char GUI_GetRColor()
* ����: ��ȡָ����ɫ�к�ɫ����ֵ.
* ����: int color ---- ָ����ɫ
* ˵��: ���麯��ֻ��ECG1230ϵͳ����, ����ɫ����Ϊ(16λ: 5R5G5B-, 555-, ��λ->��λ)
******************************************************************/
#define GUI_GetRColor(color)  (((color)>>11) & 0x001f)


/******************************************************************
* ����: unsigned char GUI_GetGColor()
* ����: ��ȡָ����ɫ����ɫ����ֵ.
* ����: int color ---- ָ����ɫ
******************************************************************/
#define GUI_GetGColor(color)    (((color)>>6) & 0x001f)


/******************************************************************
* ����: unsigned char GUI_GetRColor()
* ����: ��ȡָ����ɫ����ɫ����ֵ.
* ����: int color ---- ָ����ɫ
******************************************************************/
#define GUI_GetBColor(color)  (((color)>>1) & 0x001f)


/******************************************************************
* ����: GUI_RGB()
* ����: ��ָ����red,green,blueɫԪ�ϳ�16λ��ɫ.
******************************************************************/
#define  GUI_RGB(r, g, b)  (((r) << 11) & 0xf800) | (((g) << 6)  & 0x07c0) | (((b) << 1) & 0x003e) 


/******************************************************************
* ����: GUI_GetAAColor()
* ����: ���㷴����ֱ����ĳ�������������ɫֵ.
* ����: int value     ---- ʵ�ʵ㵽���۵�ľ������ܾ�������ռ�ı���;
*       int max       ---- ���ضԵ��ܾ���;
*       int fullColor ---- �趨��ֱ����ɫֵ;
*       int backColor ---- ʵ�ʵ㴦�ڵı�����ɫ;
* ����: int ---- ����Ӧ�������ɫֵ
* ˵��: ���������ض��ܾ���maxΪ1, valueֵ����0С��1. Ϊ�˱��⸡������,
*       �����ݷŴ���,�ܾ���Ŵ�Ϊmax(int), valueҲ�Ŵ���ͬ����,��ռ
*       �������ǲ���.ͨ����ռ�������������ɫ����ǳ.
******************************************************************/
/*
int GUI_GetAAColor(int value, int max, int fullColor, int backColor)
{
	unsigned char red, green, blue;
	
	//value = value + (max/5);         //����ʵ����Ҫ, �ʵ�����ǰ��ɫ
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
* ����: void ECG_DrawAALine()
* ����: ����һ����������ֱ��.
* ����: int x1,y1 ---- �������;
*       int x2,y2 ---- �յ�����;
*       int color ---- ֱ����ɫ;
* ����: ��
* ˵��: ����ĵ�ͼ���������ڻ�����ʱ, �����˻��߼Ӵּ�����������.
******************************************************************/
void ECG_DrawAALine(int x1, int y1, int x2, int y2, int color)
{
	int dx, dy;
	int temp;
	int k;
	unsigned char red, green, blue;
	int backColor;
	
	/*  ���ĵ�ͼϵͳ�����ܴ����������
	if (x1 == x2)
	{
		return;
	}*/
	
	/* ����ˮƽ��, �Ӵ�*/
	if (y1 == y2)
	{
		//GUI_HLine(x1, y1-1, x2, color, 2, 10);
		GUI_HLine(x1, y1,   x2, color, 8, 10);  //���ʽ�е����־�������ɫ,
		GUI_HLine(x1, y1+1, x2, color, 2, 10);  //�ɸ���ʵ������޸�.
		return;
	}
	
	dx = x2 - x1;
	dy = y2 - y1;
	
	
	if (abs(dx) == abs(dy))  //����k=+-1ʱ,����;ͬʱ���ƻ��߶���
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
		
		//���
		backColor = LCD_GetPixel(x1, y1);
		LCD_PutPixel(x1, y1,   GUI_GetAAColor(6, 10, color, backColor));    //���ʽ�е����־�������ɫ,
		backColor = LCD_GetPixel(x1, y1+k);
		LCD_PutPixel(x1, y1+k, GUI_GetAAColor(4, 10, color, backColor));    //�ɸ���ʵ������޸�.
		
		for (xs=x1+1; xs<x2; xs++)
		{
			backColor = LCD_GetPixel(xs, yt);
			LCD_PutPixel(xs, yt,   GUI_GetAAColor(6, 10, color, backColor));
			backColor = LCD_GetPixel(xs, yt+1);
			LCD_PutPixel(xs, yt+1, GUI_GetAAColor(4, 10, color, backColor));
			yt += k;
		}
		
		//�յ�
		backColor = LCD_GetPixel(x2, y2);
		LCD_PutPixel(x2, y2,   GUI_GetAAColor(6, 10, color, backColor));
		backColor = LCD_GetPixel(x2, y2-k);
		LCD_PutPixel(x2, y2-k, GUI_GetAAColor(4, 10, color, backColor));
		
	}
	else if (abs(dx) > abs(dy))  /*��xΪ����*/
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
		
		k = (dy * 1000) / dx;         //�Ŵ�1000��,���⸡������.ע��:��ֹ���!
		
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
	else        /*��yΪ����*/
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
		
		k = (dx * 1000) / dy;           //�Ŵ�1000��,���⸡������
		
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