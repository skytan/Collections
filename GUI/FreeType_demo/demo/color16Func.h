
#ifndef __COLOR16FUNC_H__
#define __COLOR16FUNC_H__

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

#define   GUI_GetAAColor(value, max, fullColor, backColor)                         \
                                                                                   \
	GUI_RGB((unsigned char)(((max) - (value)) * GUI_GetRColor(backColor)/(max)) +  \
	        (unsigned char)((value) * GUI_GetRColor(fullColor)/(max)),             \
	        (unsigned char)(((max) - (value)) * GUI_GetGColor(backColor)/(max)) +  \
	        (unsigned char)((value) * GUI_GetGColor(fullColor)/(max)),             \
	        (unsigned char)(((max) - (value)) * GUI_GetBColor(backColor)/(max)) +  \
	        (unsigned char)((value) * GUI_GetBColor(fullColor)/(max))              \
	        )



#endif   /*__COLOR16FUNC_H__*/