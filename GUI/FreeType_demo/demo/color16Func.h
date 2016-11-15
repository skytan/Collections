
#ifndef __COLOR16FUNC_H__
#define __COLOR16FUNC_H__

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