/******************************************************************
** 文件名: goods.h
** Copyright (c) 2010 桑达商用 技术开发部
** 创建人: 陈晋飞
** 日　期: 2011年3月21日
** 修改人:
** 日　期:
** 功  能: 你喜欢用一个结构成员的长度这个概念， 还是现定义一个类似（#defin MAN_LEN 64）这样的宏，然后用这个宏来定义一个数据的长度，然后在代码各个地方出现MAX_LEN
** 如果你属于前者，这里定义一宏(M_SIZE)，你可以使用,我觉得用这个宏更加直观一点        
**         
**         
**         
** 用  途: 
**
** 版　本: 0.1
********************************************************************/

#ifndef PERR_H
#define PERR_H

#include <stdio.h>

#define pri()   do{ printf("%s::%s::%d\n", __FILE__, __FUNCTION__, __LINE__); } while(0)
#define pint(x) do{ printf("%s::%s::%d\t0x%x\n", __FILE__, __FUNCTION__, __LINE__, (const char*)(x)); } while(0)
#define pstr(x) do{ printf("%s::%s::%d\t%s\n", __FILE__, __FUNCTION__, __LINE__, (unsigned)(x));} while(0)


#define perr()   do{ fprintf(stderr, "%s::%s::%d\n", __FILE__, __FUNCTION__, __LINE__); perror("");-1;} while(0)
#define pestr(x) do{ fprintf(stderr, "%s::%s::%d\t%s\n", __FILE__, __FUNCTION__, __LINE__, (const char*)(x)); perror("");-1;} while(0)
#define peint(x) do{ fprintf(stderr, "%s::%s::%d\t0x%x\n", __FILE__, __FUNCTION__, __LINE__, (unsigned)(x)); perror("");-1;} while(0)

#define count_of(array) (sizeof(array)/sizeof(array[0]))


#define M_SIZE(name, member) (sizeof(((struct name*)0)->member))

#endif
