#ifndef PERR_H
#define PERR_H

#include "sconfig.h"


int spretval(const char *file, const char *fun, int line, int ret);
void sp_data(const char *file, const char *fun, int line, const char *str, int len);

#define sret(x)   spretval(__FILE__, __FUNCTION__, __LINE__, (x))

#if __DEGUB
      #define print(format, args...) do{printf("%s::%d\t"format"\n", __FILE__, __LINE__, ##args);}while(0)
      #define perr(format, args...) do{fprintf(stderr, "%s::%d\t"format, __FILE__, __LINE__, ##args);perror("");}while(0)
      #define pdata(data, len)  p_data(__FILE__, __FUNCTION__, __LINE__, (const char*)data, (int)(len))
#else
      #define print(format, args...)   
      #define perr(format, args...)   
      #define pdata(data, len)  
#endif


#define count_of(array) (sizeof(array)/sizeof(array[0]))

#define MAX(x, y) ((x)>(y)?(x):(y))
#define MIN(x, y) ((x)<(y)?(x):(y))

#define member_size(name, member) (sizeof((struct name*)0->member))

#endif
