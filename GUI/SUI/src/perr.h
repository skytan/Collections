/******************************************************************
** �ļ���: goods.h
** Copyright (c) 2010 ɣ������ ����������
** ������: �½���
** �ա���: 2011��3��21��
** �޸���:
** �ա���:
** ��  ��: ��ϲ����һ���ṹ��Ա�ĳ��������� �����ֶ���һ�����ƣ�#defin MAN_LEN 64�������ĺ꣬Ȼ���������������һ�����ݵĳ��ȣ�Ȼ���ڴ�������ط�����MAX_LEN
** ���������ǰ�ߣ����ﶨ��һ��(M_SIZE)�������ʹ��,�Ҿ�������������ֱ��һ��        
**         
**         
**         
** ��  ;: 
**
** �桡��: 0.1
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
