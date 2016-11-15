#ifndef COMMON_H
#define COMMON_H

#include "sui.h"
#include "perr.h"
#include "config.h"



int  wcstoi(const wchar_t *s);
unsigned hash(const char * s);

int pretval(const char *file, const char *fun, int line, int ret);

#define pret(x)   pretval(__FILE__, __FUNCTION__, __LINE__, (x))

#define INT_ENTRY_LEN 8


#define POS_LEFT   	0xff51	//NO62
#define POS_RIGHT  	0xff53	//NO64
#define POS_UP		0xff52	//NO53
#define POS_DOWN	0xff54	//NO63
#define POS_ENTER 	0xff0d	//NO58
#define POS_DEL		0xffff	//NO21
#define POS_PLU		0xfc0f	//NO38
#define POS_ESC		0xfc10	//NO11

#define POS_PLU1	0xfc18	//NO12
#define POS_PLU2	0xfc12	//NO13
#define POS_CATEGORY	0xfc13	//NO14
#define POS_TEMP_HANG	0xfc14	//NO15,暂挂键
#define POS_CLEAR	0xfc15	//NO16,总清键
#define POS_PAPER	0xfc16	//NO17,走纸键

#define POS_PLU3	0xfc17	//NO22
#define POS_PLU4	0xfc01	//NO23
#define POS_PRICE	0xfc02	//NO24
#define POS_DISCOUNT	0xfc03	//NO25,打折键

#if DEVICE_SED_E318||DEVICE_SED_E318_2
	#define POS_DIRECT_SALE	POS_CLEAR	//NO26,直销键	
#else
	#define POS_DIRECT_SALE	0xfc04	//NO26,直销键	
#endif

#define POS_CHANGE      0xfc05	//NO27,辅币键
#define POS_QUANTITY	0xfc06	//NO28,数量键

#define POS_PLU5	0xfc07	//NO32
#define POS_PLU6	0xfc08	//NO33

#if !DEVICE_SED_E330	
	#define POS_PLU7	0xfc0a	//NO42
	#define POS_PLU8	0xfc0b	//NO43
#endif

#define POS_NO52	0xfc0d	//NO52
#define POS_NO54	0xfc0e	//NO54
#define POS_TMP_KEY		0xfc18	//临时键

#if DEVICE_SED_E330
	#define POS_BANK_SALE		0xfc09	//
	#define POS_BANK_DEAL		0xfc0d	//
	#define POS_BANK_REPRINT	0xfc0c	//
#endif

#if DEVICE_SED_E318||DEVICE_SED_E318_2
	#define POS_BANK_RETURN		POS_CHANGE	//
#else
	#define POS_BANK_RETURN		0xfc0e	//
#endif

#if DEVICE_SED_E330
	#define POS_BANK_LOGOFF	0xfc0a	//NO42
#endif

#endif
