
#ifndef  __TTF_FUNC_H__
#define  __TTF_FUNC_H__


#define   FONT_NORMAL    0
#define   FONT_ITALIC    1
#define   FONT_BOLD      2

typedef  unsigned short  U16;


/*--------------------------------------------
* 用户配置变量
可赋值为：
FT_LOAD_DEFAULT                      (0x0)默认
FT_LOAD_NO_SCALE                     (0x1)固定大小
FT_LOAD_NO_HINTING                   (0x2)不自动hinting
FT_LOAD_RENDER                       (0x4)反走样
FT_LOAD_NO_BITMAP                    (0x8)点阵
FT_LOAD_VERTICAL_LAYOUT              (0x10)垂直方式
FT_LOAD_FORCE_AUTOHINT               (0x20)强制hinting
...
支持如 FT_LOAD_DEFAULT | FT_LOAD_RENDER的方式;
其它说明见官方文档
*--------------------------------------------*/
unsigned char  TTF_bitmap_type;


/*--------------------------------------------
* 字形变量, 一旦设置好各对应字形变量后,用户不需要考虑
*--------------------------------------------*/
FT_Library ft_Lib;   /* 库的句柄 */
FT_Face face;         /* face 对象的句柄 */
FT_UInt glyph_index;
FT_Vector pen;        
FT_Matrix     matrix;              /* 变换矩阵 */
FT_Error ft_error;





/*******************************************************
*函数: TTF_Init()
*说明: 字形初始化,指定字形像素大小
*参数: int fontWidth, int fontHeight ---- 字形宽、高像素大小
*      unsigned char bitmap          ---- 字形样式,如:反走样等.参数见配置变量TTF_bitmap_type.
*返回: 成功0, 失败-1
*******************************************************/
int TTF_Init(int fontWidth, int fontHeight, unsigned char bitmap);


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
int TTF_DisplayUnicode(int x, int y, const unsigned char *text, int num, U16 color, unsigned char style);



#endif    /* __TTF_FUNC_H__ */