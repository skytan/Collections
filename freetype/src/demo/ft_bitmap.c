#include <ft2build.h>
//#include FT_FREETYPE_H
#include "F:\vc++\PROJECTS\freetype\freetype\include\freetype\freetype.h"

#pragma comment(lib, "freetype27.lib")  
#define CHARSIZE 40

int GetCharBitmap(int iCharSize, unsigned int uiCharCode);

int GetCharBitmap(int iCharSize, unsigned int uiCharCode)
{
     FT_Library ftLibrary;
	 FT_Face	face;
	 FT_UInt	uiGlyphIndex;
     FT_Error	ftError;

	 //template variables
	 int iRow;
	 int iCol;

	 ftError = FT_Init_FreeType(&ftLibrary);
     if(ftError)
     {
         printf("Init freetype library fail!\n");
         return -1;
     }

     ftError = FT_New_Face( ftLibrary, 
		 "F:\\vc++\\PROJECTS\\freetype\\freetype\\builds\\windows\\visualc\\Debug\\vrinda.ttf",
		 0, &face );
     if(ftError == FT_Err_Unknown_File_Format)
     {
         printf("Error! Could not support this format!\n");
         return -1;
     }
     else if(ftError)
     {
         printf("Error! Could not open file ukai.ttc!\n");
         return -1;
     }
 
 
	 ftError = FT_Set_Pixel_Sizes(face, iCharSize, 0);
     if(ftError)
     {
         printf("Set pixel sizes to %d*%d error!\n", iCharSize, iCharSize);
         return -1;
     }
     uiGlyphIndex = FT_Get_Char_Index(face, uiCharCode);
 
     FT_Load_Glyph(face,uiGlyphIndex, FT_LOAD_DEFAULT);
 
 
     FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

     for(iRow = 0; iRow < face->glyph->bitmap.rows; iRow++)
     {
         for(iCol = 0; iCol < face->glyph->bitmap.width; iCol++)
         {
              if((face->glyph->bitmap.buffer[iRow * face->glyph->bitmap.pitch + iCol/8] & (0xC0 >> (iCol % 8))) == 0)
              {
                   printf("_");
              }
			  else
			  {  
                   printf("0");
			  }
         }
         printf("\n");
     }
     return 0; 
}

int main(int argc, char** argv)
{
     unsigned int uiCharCode = 0x0077;
     GetCharBitmap(CHARSIZE, uiCharCode);
     return 0;
}
 
