
#ifndef  __TTF_FUNC_H__
#define  __TTF_FUNC_H__


#define   FONT_NORMAL    0
#define   FONT_ITALIC    1
#define   FONT_BOLD      2

typedef  unsigned short  U16;


/*--------------------------------------------
* �û����ñ���
�ɸ�ֵΪ��
FT_LOAD_DEFAULT                      (0x0)Ĭ��
FT_LOAD_NO_SCALE                     (0x1)�̶���С
FT_LOAD_NO_HINTING                   (0x2)���Զ�hinting
FT_LOAD_RENDER                       (0x4)������
FT_LOAD_NO_BITMAP                    (0x8)����
FT_LOAD_VERTICAL_LAYOUT              (0x10)��ֱ��ʽ
FT_LOAD_FORCE_AUTOHINT               (0x20)ǿ��hinting
...
֧���� FT_LOAD_DEFAULT | FT_LOAD_RENDER�ķ�ʽ;
����˵�����ٷ��ĵ�
*--------------------------------------------*/
unsigned char  TTF_bitmap_type;


/*--------------------------------------------
* ���α���, һ�����úø���Ӧ���α�����,�û�����Ҫ����
*--------------------------------------------*/
FT_Library ft_Lib;   /* ��ľ�� */
FT_Face face;         /* face ����ľ�� */
FT_UInt glyph_index;
FT_Vector pen;        
FT_Matrix     matrix;              /* �任���� */
FT_Error ft_error;





/*******************************************************
*����: TTF_Init()
*˵��: ���γ�ʼ��,ָ���������ش�С
*����: int fontWidth, int fontHeight ---- ���ο������ش�С
*      unsigned char bitmap          ---- ������ʽ,��:��������.���������ñ���TTF_bitmap_type.
*����: �ɹ�0, ʧ��-1
*******************************************************/
int TTF_Init(int fontWidth, int fontHeight, unsigned char bitmap);


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
int TTF_DisplayUnicode(int x, int y, const unsigned char *text, int num, U16 color, unsigned char style);



#endif    /* __TTF_FUNC_H__ */