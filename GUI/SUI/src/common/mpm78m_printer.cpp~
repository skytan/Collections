/******************************************************************
** 文件名: mpm78m_printer.c
** Copyright (c) 2010 桑达商用 技术开发部
** 创建人: 雒孟锬
** 日　期: 2011年5月6日
** 修改人:
** 日　期:
** 功  能: MPM78M打印机程序
**         
**         
**         
**         
** 用  途: 
**
** 版　本: 0.1
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <asm/msr.h> 

#include "../common.h"
#include "sui.h"
#include "mpm78m_printer.h"

struct termios  oldtio;

/******************************************************************
功  能: 打开打印机串口


返回值说明： -1：错误 正数：打印机句柄
******************************************************************/
int open_printer_com(void)
{
	int iSerial;
	char arrchSerialName[20] = "/dev/ttyS3";
	struct termios newtio;
	
	iSerial = open(arrchSerialName, O_RDWR | O_NOCTTY);
	if (iSerial < 0) 
		return -1;

	memset(&oldtio, 0, sizeof(oldtio));
	tcgetattr(iSerial, &oldtio);
	bzero(&newtio, sizeof(newtio));
	newtio = oldtio;
	
	newtio.c_cflag &= ~CSIZE;
	//newtio.c_cflag = B57600 | CS8 | CLOCAL | CREAD ;	//57600,e,8,1
	newtio.c_cflag = CS8 | CLOCAL | CREAD ;	//n,8,1
	newtio.c_iflag = IGNPAR;	// Ignore bytes with parity errors and make terminal raw and dumb.} 
	newtio.c_cflag &= ~PARENB;
	newtio.c_cflag &= ~CSTOPB;
	newtio.c_iflag |= IGNPAR;
	
	newtio.c_oflag = 0;
	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;	/* inter-character timer unused */
	newtio.c_cc[VMIN] = 0;	/* return anyway  */

	cfsetospeed(&newtio,B57600);	//for POSIX
	cfsetispeed(&newtio,B57600);
	tcflush(iSerial, TCIOFLUSH);
	tcsetattr(iSerial, TCSANOW, &newtio);
	
	return iSerial;
}

int close_printer_com(int iSerial)
{
	tcflush(iSerial, TCIOFLUSH);
	tcsetattr(iSerial, TCSANOW, &oldtio);
	
	return 0;
}



/******************************************************************
功  能: 错误返回

char ch IN 打印机返回的错误码
char *pszMessage IN/OUT 错误

返回值说明： 无
******************************************************************/
void mpm78m_error_return(char ch, char *pszMessage)
{
	strcpy(pszMessage, "");
	if(ch & 0x80 == 0x80)
	{
		strcat(pszMessage, "打印机错 ");
	}
	
	if(ch & 0x40 == 0x40)
	{
		strcat(pszMessage, "打印头过热 ");
	}

	if(ch & 0x20 == 0x20)
	{
		strcat(pszMessage, "黑标检测错误 ");
	}

	if(ch & 0x10 == 0x10)
	{
		strcat(pszMessage, "上电续打请求 ");
	}

	if(ch & 0x08 == 0x08)
	{
		strcat(pszMessage, "接收数据错误 ");
	}

	if(ch & 0x04 == 0x04)
	{
		strcat(pszMessage, "打印机缺纸 ");
	}

	if(ch & 0x02 == 0x02)
	{
		strcat(pszMessage, "打印机复位异常!");
	}
}



/******************************************************************
功  能: 读打印机状态

int ifd IN 打印机句柄

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_read_state(int ifd)
{
	struct timeval tv;
	fd_set rfds;

	unsigned char szSendBuf[] = {0x1b, 0x10, 0x06, 0x00, 0x07, 0x00, 0x38};
	
	
	
	


	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, szSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		return -1;
	}
	else
	{
		char ch, ch1;
		
		
		
		
		
		read(ifd, &ch, 1);

		//读第2字节数据
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd,&rfds);

		if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
		{
			return -2;
		}
		else
		{
			read(ifd, &ch1, 1);
			
			if(ch1 != 0)
			{
				return -3;
			}
		}

		if(ch != 0)
		{
			return -4;
		}
		else
			return 0;
	}	

	return -255;
}



/******************************************************************
功  能: 读打印机状态

int ifd IN 打印机句柄
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_read_state_str(int ifd, char *pszMessage)
{
	struct timeval tv;
	fd_set rfds;

	unsigned char szSendBuf[] = {0x1b, 0x10, 0x06, 0x00, 0x07, 0x00, 0x38};
	
	
	
	


	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, szSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		strcpy(pszMessage, "打印机无响应");
		return -1;
	}
	else
	{
		char ch, ch1;
		
		
		
		
		
		read(ifd, &ch, 1);

		//读第2字节数据
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd,&rfds);

		if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
		{
			sprintf(pszMessage, "打印机无响应 状态%02xH", ch);
			return -2;
		}
		else
		{
			read(ifd, &ch1, 1);
			
			if(ch1 != 0)
			{
				sprintf(pszMessage, "打印机通讯错 状态%02xH", ch);
				return -3;
			}
		}

		if(ch != 0)
		{
			mpm78m_error_return(ch, pszMessage);
			return -4;
		}
		else
			return 0;
	}	

	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 黑标检测

int ifd IN 打印机句柄

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_find_black_mark(int ifd)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x05, 0x00, 0x07, 0x00, 0x37};

	struct timeval tv;
	fd_set rfds;
	char ch;






	
		
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, szSendBuf, 7);

	if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
	{
		return -1;
	} 
	else 
	{
		read(ifd, &ch, 1);
    	
		if(ch != 0)
		{
			printf("%s_____find_black_mark 错误状态:%x\n", __FILE__, ch);

			//等待返回
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			FD_ZERO(&rfds);
			FD_SET(ifd, &rfds);
			
			if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
			{
				printf("%s_____find_black_mark 无响应1\n", __FILE__);

				return -2;
			} 
			else 
			{
				char ch1;
				
				
				
				
				
				read(ifd, &ch1, 1);
				
				if(ch1 != 0)
				{
					printf("%s_____find_black_mark err:%02x ret:%02x\n", __FILE__, ch, ch1);
					return -3;
				}
				else
				{
					printf("%s_____find_black_mark err:%02x\n", __FILE__, ch);
					return -4;
				}
			}
		}
		else
		{
			return 0;
		}		
	}

	return -255;
}



/******************************************************************
功  能: 黑标检测

int ifd IN 打印机句柄
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_find_black_mark_str(int ifd, char *pszMessage)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x05, 0x00, 0x07, 0x00, 0x37};

	struct timeval tv;
	fd_set rfds;
	char ch;






	
		
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, szSendBuf, 7);

	if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
	{
		strcpy(pszMessage, "打印机无响应");
		return -1;
	} 
	else 
	{
		read(ifd, &ch, 1);
    	
		if(ch != 0)
		{
			printf("%s_____find_black_mark 错误状态:%x\n", __FILE__, ch);

			//等待返回
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			FD_ZERO(&rfds);
			FD_SET(ifd, &rfds);
			
			if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
			{
				printf("%s_____find_black_mark 无响应1\n", __FILE__);

				sprintf(pszMessage, "打印机无响应1 状态%02xH", ch);
				return -2;
			} 
			else 
			{
				char ch1;
				
				
				
				
				
				read(ifd, &ch1, 1);
				
				if(ch1 != 0)
				{
					printf("%s_____find_black_mark err:%02x ret:%02x\n", __FILE__, ch, ch1);
					sprintf(pszMessage, "返回错 状态%02xH 返回%02xH", ch, ch1);
					return -3;
				}
				else
				{
					printf("%s_____find_black_mark err:%02x\n", __FILE__, ch);
					mpm78m_error_return(ch, pszMessage);
					return -4;
				}
			}
		}
		else
		{
			return 0;
		}		
	}

	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 打印字符串（其中不能包含控制字符 即不能小于0x20(空格) ）
	一行44个字符，如果字符串不够或等于一行，会自动添加回车换行符，打印
	如果字符串超出一行，会自动分行打印，每行会自动添加回车换行符，打印

int ifd IN 打印机句柄
char *pszStr IN 输入要打印的字符串

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_print_utf8(int ifd, unsigned char *pszStr)
{
	char gbk[512];
	
	utf8_to_gbk2312( (const char *)pszStr, gbk, sizeof(gbk));
	
	return  mpm78m_print_str(ifd, (unsigned char *)gbk);
}


int mpm78m_print_str(int ifd, unsigned char *pszStr)
{
	unsigned char uchLen, uch1, uch2, uchTemp;
	unsigned char uchPrintLen;
	unsigned char uchCharNum;
	
	unsigned char szPrintBuf[800];
	int iLen, i1;
	char *ptrTemp;
	char *ptrScan;
	
	struct timeval tv;
	fd_set rfds;
	char ch;







	iLen = strlen( (const char*)pszStr);
	//if(iLen == 0)
	//	return 0;
		
	//printf("print_mem78m__________________%s\n", pszStr);

	//for(i1 = 0; i1 < iLen; i1++)
	//{
	//	printf("%02x ", pszStr[i1]);
	//}
	//printf("\n");
	
	ptrTemp = (char*)pszStr;

	while(1)
	{
		if(iLen > 44)
		{
			//扫描一行
			ptrScan = ptrTemp;
			uchLen = 0;
			uchPrintLen = 0;
			do
			{		
				uchTemp = *ptrScan;
		
				if('\0' == uchTemp)
					break;

				if(uchTemp <  0x80)
				{
					uchCharNum = 1;
				}
				else if(((uchTemp == 0x81) || (uchTemp == 0x82)) && (*(ptrScan + 1) < 0x40))
				{
					uchCharNum = 4;
				}
				else
					uchCharNum = 2;
				
				if(uchCharNum == 1)
				{
					if(uchPrintLen + 1 > 44)
						break;
						
					uchPrintLen++;	
				}
				else
				{
					//汉字
					if(uchPrintLen + 2 > 44)
						break;

					uchPrintLen += 2;	
				}
					
				ptrScan += uchCharNum;	
				uchLen += uchCharNum;
			}while(1);
		}	
		else
			uchLen = iLen;

		szPrintBuf[0] = 0x1b;
		szPrintBuf[1] = 0x10;
		szPrintBuf[2] = 0x07;
		szPrintBuf[3] = 0x00;
		szPrintBuf[4] = uchLen + 6 + 2;//<90

		uch2 = 0;
		for(uch1 = 0; uch1 < 5; uch1++)
			uch2 += szPrintBuf[uch1];

		for(uch1 = 0; uch1 < uchLen; uch1++)
		{
			szPrintBuf[uch1 + 5] = ptrTemp[uch1];
			uch2 += ptrTemp[uch1];
		}
	
		szPrintBuf[uchLen + 5] = 0x0d;
		szPrintBuf[uchLen + 6] = 0x0a;
		szPrintBuf[uchLen + 7] = uch2 + szPrintBuf[uchLen + 5] + szPrintBuf[uchLen + 6];

	#if 0	
		for(uch1 = 0; uch1 < uchLen + 8; uch1++)
		{
			printf("%02x ", szPrintBuf[uch1]);
		}
		printf("\n");
	#endif

		write(ifd, szPrintBuf, uchLen + 8);
		
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);
		
		if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
		{
			printf("%s____print_mem78m_打印时没有返回_________________1\n", __FILE__);
			return -1;
		} 
		else 
		{
			read(ifd, &ch, 1);
    	
			if(ch != 0)
			{
				printf("%s____print_mem78m_____%x_____________3\n", __FILE__, ch);
				return -2;
			}
			else
			{
				//return 0;
			}		
		}

		ptrTemp += uchLen;
		if(iLen > 44)
		{
			iLen -= uchLen;
			if(iLen == 0)
				return 0;
		}
		else
			return 0;
	}
	
	return -255;
}


int mpm78m_print_string(int ifd, unsigned char *pszStr)
{
	unsigned char uchLen, uch1, uch2, uchTemp;
	unsigned char uchPrintLen;
	unsigned char uchCharNum;
	
	unsigned char szPrintBuf[512];
	int iLen, i1;
	char *ptrTemp;
	char *ptrScan;
	
	struct timeval tv;
	fd_set rfds;
	char ch;

	iLen = strlen( (const char*)pszStr);	
	ptrTemp = (char*)pszStr;

	uchLen = MIN(iLen, 500);

	szPrintBuf[0] = 0x1b;
	szPrintBuf[1] = 0x10;
	szPrintBuf[2] = 0x07;
	szPrintBuf[3] = 0x00;
	szPrintBuf[4] = uchLen + 6;//<90

	uch2 = 0;
	for(uch1 = 0; uch1 < 5; uch1++)
		uch2 += szPrintBuf[uch1];

	for(uch1 = 0; uch1 < uchLen; uch1++)
	{
		szPrintBuf[uch1 + 5] = ptrTemp[uch1];
		uch2 += ptrTemp[uch1];
	}

	szPrintBuf[uchLen + 5] = uch2;

	write(ifd, szPrintBuf, uchLen + 6);
		
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);
		
	if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
	{
		return pret(-1);
	} 
	else 
	{
		read(ifd, &ch, 1);
    	
		if(ch != 0)
		{
			return pret(-2);
		}		
	}
	
	return 0;
}

/******************************************************************
功  能: 打印字符串
	一行44个字符，如果字符串不够或等于一行，会自动添加回车换行符，打印
	如果字符串超出一行，会自动分行打印，每行会自动添加回车换行符，打印

int ifd IN 打印机句柄
char *pszStr IN 输入要打印的字符串
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_print_str_message(int ifd, char *pszStr, char *pszMessage)
{
	unsigned char uchLen, uch1, uch2, uchTemp;;
	unsigned char uchPrintLen;
	unsigned char uchCharNum;
	
	unsigned char szPrintBuf[800];
	int iLen, i1;
	char *ptrTemp;
	char *ptrScan;
	
	struct timeval tv;
	fd_set rfds;
	char ch;







	iLen = strlen(pszStr);
	//if(iLen == 0)
	//	return 0;
		
	//printf("print_mem78m__________________%s\n", pszStr);

	//for(i1 = 0; i1 < iLen; i1++)
	//{
	//	printf("%02x ", pszStr[i1]);
	//}
	//printf("\n");
	
	ptrTemp = pszStr;

	while(1)
	{
		if(iLen > 44)
		{
			//扫描一行
			ptrScan = ptrTemp;
			uchLen = 0;
			uchPrintLen = 0;
			do
			{
				uchTemp = *ptrScan;
		
				if('\0' == uchTemp)
					break;

				if(uchTemp <  0x80)
				{
					uchCharNum = 1;
				}
				else if(((uchTemp == 0x81) || (uchTemp == 0x82)) && (*(ptrScan + 1) < 0x40))
				{
					uchCharNum = 4;
				}
				else
					uchCharNum = 2;
				
				if(uchCharNum == 1)
				{
					if(uchPrintLen + 1 > 44)
						break;
						
					uchPrintLen++;	
				}
				else
				{
					//汉字
					if(uchPrintLen + 2 > 44)
						break;

					uchPrintLen += 2;	
				}
					
				ptrScan += uchCharNum;	
				uchLen += uchCharNum;
			}while(1);
		}	
		else
			uchLen = iLen;

		szPrintBuf[0] = 0x1b;
		szPrintBuf[1] = 0x10;
		szPrintBuf[2] = 0x07;
		szPrintBuf[3] = 0x00;
		szPrintBuf[4] = uchLen + 6 + 2;//<90

		uch2 = 0;
		for(uch1 = 0; uch1 < 5; uch1++)
			uch2 += szPrintBuf[uch1];

		for(uch1 = 0; uch1 < uchLen; uch1++)
		{
			szPrintBuf[uch1 + 5] = ptrTemp[uch1];
			uch2 += ptrTemp[uch1];
		}
	
		szPrintBuf[uchLen + 5] = 0x0d;
		szPrintBuf[uchLen + 6] = 0x0a;
		szPrintBuf[uchLen + 7] = uch2 + szPrintBuf[uchLen + 5] + szPrintBuf[uchLen + 6];

	#if 0	
		for(uch1 = 0; uch1 < uchLen + 8; uch1++)
		{
			printf("%02x ", szPrintBuf[uch1]);
		}
		printf("\n");
	#endif

		write(ifd, szPrintBuf, uchLen + 8);
		
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);
		
		if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
		{
			printf("%s____print_mem78m_打印时没有返回_________________1\n", __FILE__);
			strcpy(pszMessage, "打印机无响应");
			return -1;
		} 
		else 
		{
			read(ifd, &ch, 1);
    	
			if(ch != 0)
			{
				printf("%s____print_mem78m_____%x_____________3\n", __FILE__, ch);
				mpm78m_error_return(ch, pszMessage);
				return -2;
			}
			else
			{
				//return 0;
			}		
		}

		ptrTemp += uchLen;
		if(iLen > 44)
		{
			iLen -= uchLen;
			if(iLen == 0)
				return 0;
		}
		else
			return 0;
	}
	
	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 进纸 每步长度为0.176mm 30步为2行

int ifd IN 打印机句柄
unsigned int uiFeedmm IN 要进纸的点行数

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_feed_mm(int ifd, unsigned int uiFeedmm)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x01, 0x00, 0x07, 0x00, 0x33};

	struct timeval tv;
	fd_set rfds;
	char ch;

	while(1)
	{
		if(uiFeedmm == 0)
			return 0;
		
		if(uiFeedmm > 240)
			ch = 240;
		else
			ch = uiFeedmm;
				
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		szSendBuf[5] = ch;
		szSendBuf[6] = 0x33 + ch;
		
		write(ifd, szSendBuf, 7);
		if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
		{
			return pret(-1);
		} 
		else 
		{
			read(ifd, &ch, 1);

			//printf("%s_____feed_mm__ch = %x_____________\n", __FILE__, ch);
			if(ch != 0)
			{
				return pret(-2);
			}
			else
			{
				//return 0;
			}
		}

		if(uiFeedmm > 240)
		{
			uiFeedmm -= 240;
		}
		else
			return 0;
	}
	
	return pret(-255);
}



/******************************************************************
功  能: 点行进纸 每步长度为0.176mm 30步为2行

int ifd IN 打印机句柄
unsigned int uiFeedmm IN 要进纸的点行数
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_feed_mm_str(int ifd, unsigned int uiFeedmm, char *pszMessage)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x01, 0x00, 0x07, 0x00, 0x33};

	struct timeval tv;
	fd_set rfds;
	char ch;


	while(1)
	{
		if(uiFeedmm == 0)
			return 0;
		
		if(uiFeedmm > 240)
			ch = 240;
		else
			ch = uiFeedmm;
				
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		szSendBuf[5] = ch;
		szSendBuf[6] = 0x33 + ch;
		
		write(ifd, szSendBuf, 7);
		if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
		{
			printf("%s_____feed_mm___no return____________1\n", __FILE__);
			strcpy(pszMessage, "打印机无响应");
			return -1;
		} 
		else 
		{
			read(ifd, &ch, 1);

			//printf("%s_____feed_mm__ch = %x_____________\n", __FILE__, ch);
			if(ch != 0)
			{
				printf("%s_____feed_mm____ch = %x___________2\n", __FILE__, ch);
				mpm78m_error_return(ch, pszMessage);
				return -2;
			}
			else
			{
				//return 0;
			}
		}

		if(uiFeedmm > 240)
		{
			uiFeedmm -= 240;
		}
		else
			return 0;
	}
	
	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 点行退纸 每步长度为0.176mm 30步为2行

int ifd IN 打印机句柄
unsigned int uiBackmm IN 要退纸的点行数

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_back_mm(int ifd, unsigned int uiBackmm)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x00, 0x00, 0x07, 0x00, 0x32};
	unsigned char uch1;

	struct timeval tv;
	fd_set rfds;
	char ch;






	
	while(1)
	{
		if(uiBackmm == 0)
			return 0;
		
		if(uiBackmm > 48)
		{
			uch1 = 48;
		}
		else
		{
			uch1 = uiBackmm;
		}	
		
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		szSendBuf[5] = uch1;
		szSendBuf[6] = 0x32 + uch1;

		write(ifd, szSendBuf, 7);
		if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
		{
			printf("%s_____back_paper_on_mm___no return____________1\n", __FILE__);

			return -1;
		} 
		else 
		{
			read(ifd, &ch, 1);
    	
			if(ch != 0)
			{
				printf("%s_____back_paper_on_mm____ch = %x___________2\n", __FILE__, ch);

				return -2;
			}
			else
			{
				//return 0;
			}		
		}

		if(uiBackmm > 48)
		{
			uiBackmm -= 48;
		}
		else
		{
			return 0;
		}	
	}
	

	return -255;
}



/******************************************************************
功  能: 点行退纸 每步长度为0.176mm 30步为2行

int ifd IN 打印机句柄
unsigned int uiBackmm IN 要退纸的点行数
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_back_mm_str(int ifd, unsigned int uiBackmm, char *pszMessage)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x00, 0x00, 0x07, 0x00, 0x32};
	unsigned char uch1;

	struct timeval tv;
	fd_set rfds;
	char ch;






	
	while(1)
	{
		if(uiBackmm == 0)
			return 0;
		
		if(uiBackmm > 48)
		{
			uch1 = 48;
		}
		else
		{
			uch1 = uiBackmm;
		}	
		
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		szSendBuf[5] = uch1;
		szSendBuf[6] = 0x32 + uch1;

		write(ifd, szSendBuf, 7);
		if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
		{
			printf("%s_____back_paper_on_mm___no return____________1\n", __FILE__);
			strcpy(pszMessage, "打印机无响应");
			return -1;
		} 
		else 
		{
			read(ifd, &ch, 1);
    	
			if(ch != 0)
			{
				printf("%s_____back_paper_on_mm____ch = %x___________2\n", __FILE__, ch);
				mpm78m_error_return(ch, pszMessage);
				return -2;
			}
			else
			{
				//return 0;
			}		
		}

		if(uiBackmm > 48)
		{
			uiBackmm -= 48;
		}
		else
		{
			return 0;
		}	
	}
	
	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: BID打印（清晰度打印）

int ifd IN 打印机句柄

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_BID_print(int ifd)
{
	unsigned char arruchSendBuf[] = {0x1b, 0x10, 0x02, 0x00, 0x07, 0x00, 0x34};
	unsigned char uchFlag = 0;
	
	struct timeval tv;
	fd_set rfds;
	char ch;




	tv.tv_sec = 20;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, arruchSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____BID_print___no return____________1\n", __FILE__);

		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		
		if(ch != 0)
		{
			printf("%s_____BID_print____ch = %x___________2\n", __FILE__, ch);

			return -2;
		}
		else
		{
			return 0;
		}
	}			


	return -255;
}


/******************************************************************
功  能: BID打印（清晰度打印）

int ifd IN 打印机句柄
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_BID_print_str(int ifd, char *pszMessage)
{
	unsigned char arruchSendBuf[] = {0x1b, 0x10, 0x02, 0x00, 0x07, 0x00, 0x34};
	unsigned char uchFlag = 0;
	
	struct timeval tv;
	fd_set rfds;
	char ch;




	tv.tv_sec = 20;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, arruchSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____BID_print___no return____________1\n", __FILE__);
		strcpy(pszMessage, "打印机无响应");
		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		
		if(ch != 0)
		{
			printf("%s_____BID_print____ch = %x___________2\n", __FILE__, ch);
			mpm78m_error_return(ch, pszMessage);
			return -2;
		}
		else
		{
			return 0;
		}
	}			

	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: BID值设置（清晰度设置）

int ifd IN 打印机句柄
unsigned char chAccuracy IN BID值 精度范围0 - 20

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_BID_choose(int ifd, unsigned char chAccuracy)
{
	unsigned char arruchSendBuf[] = {0x1b, 0x10, 0x03, 0x00, 0x07, 0x00, 0x35};

	struct timeval tv;
	fd_set rfds;
	char ch;
	
	



	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);
	
	arruchSendBuf[5] = chAccuracy;
	arruchSendBuf[6] += chAccuracy;
	
	write(ifd, arruchSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____BID_choose___no return____________1\n", __FILE__);

		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		if(ch != 0)
		{
			printf("%s_____BID_choose____ch = %x___________2\n", __FILE__, ch);

			return -2;
		}
		else
		{
			return 0;
		}
	}			


	return -255;
}



/******************************************************************
功  能: BID值设置（清晰度设置）

int ifd IN 打印机句柄
unsigned char chAccuracy IN BID值 精度范围0 - 20
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_BID_choose_str(int ifd, unsigned char chAccuracy, char *pszMessage)
{
	unsigned char arruchSendBuf[] = {0x1b, 0x10, 0x03, 0x00, 0x07, 0x00, 0x35};

	struct timeval tv;
	fd_set rfds;
	char ch;
	
	



	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);
	
	arruchSendBuf[5] = chAccuracy;
	arruchSendBuf[6] += chAccuracy;
	
	write(ifd, arruchSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____BID_choose___no return____________1\n", __FILE__);
		strcpy(pszMessage, "打印机无响应");
		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		if(ch != 0)
		{
			printf("%s_____BID_choose____ch = %x___________2\n", __FILE__, ch);
			mpm78m_error_return(ch, pszMessage);
			return -2;
		}
		else
		{
			return 0;
		}
	}			

	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 自检打印

int ifd IN 打印机句柄
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int MPM78M_self_test_print(int ifd)
{
	unsigned char arruchSendBuf[] = {0x1b, 0x10, 0x04, 0x00, 0x07, 0x00, 0x36};

	struct timeval tv;
	fd_set rfds;
	char ch;

	
	
	


	tv.tv_sec = 17;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, arruchSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____MPM78M_self_test_print___no return____________1\n", __FILE__);

		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		if(ch != 0)
		{
			printf("%s_____MPM78M_self_test_print____ch = %x___________2\n", __FILE__, ch);

			return -2;
		}
		else
		{
			return 0;
		}
	}			


	return -255;
}



/******************************************************************
功  能: 自检打印

int ifd IN 打印机句柄
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int MPM78M_self_test_print_str(int ifd, char *ptrchMessage)
{
	unsigned char arruchSendBuf[] = {0x1b, 0x10, 0x04, 0x00, 0x07, 0x00, 0x36};

	struct timeval tv;
	fd_set rfds;
	char ch;

	
	
	


	tv.tv_sec = 17;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, arruchSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____MPM78M_self_test_print___no return____________1\n", __FILE__);
		strcpy(ptrchMessage, "打印机无响应");
		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		if(ch != 0)
		{
			printf("%s_____MPM78M_self_test_print____ch = %x___________2\n", __FILE__, ch);
			mpm78m_error_return(ch, ptrchMessage);
			return -2;
		}
		else
		{
			return 0;
		}
	}			

	strcpy(ptrchMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 读黑标调整值

int ifd IN 打印机句柄

返回值说明： 负数：错误 正数：黑标调整值
******************************************************************/
int mpm78m_read_black_mark(int ifd)
{
	struct timeval tv;
	fd_set rfds;

	unsigned char szSendBuf[] = {0x1b, 0x10, 0x09, 0x00, 0x07, 0x00, 0x3B};
	unsigned char uchFlag = 0;
	
	
	


	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, szSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____read_black_mark___no return____________1\n", __FILE__);

		return -1;
	}
	else
	{
		char ch, ch1;
		
		
		
		
		
		read(ifd, &ch, 1);

		//读第2字节数据
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
		{
			printf("%s_____read_black_mark___no return____ch = %x___________2\n", __FILE__, ch);

			return -2;
		}
		else
		{
			read(ifd, &ch1, 1);
			
			if(ch1 != 0)
			{

				return -3;
			}
		}

		return ch;
	}	


	return -255;
}



/******************************************************************
功  能: 读黑标调整值

int ifd IN 打印机句柄
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 正数：黑标调整值
******************************************************************/
int mpm78m_read_black_mark_str(int ifd, char *pszMessage)
{
	struct timeval tv;
	fd_set rfds;

	unsigned char szSendBuf[] = {0x1b, 0x10, 0x09, 0x00, 0x07, 0x00, 0x3B};
	unsigned char uchFlag = 0;
	
	
	


	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);

	write(ifd, szSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____read_black_mark___no return____________1\n", __FILE__);
		strcpy(pszMessage, "打印机无响应");
		return -1;
	}
	else
	{
		char ch, ch1;
		
		
		
		
		
		read(ifd, &ch, 1);

		//读第2字节数据
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
		{
			printf("%s_____read_black_mark___no return____ch = %x___________2\n", __FILE__, ch);
			sprintf(pszMessage, "打印机无响应 黑标调整值%02xH", ch);
			return -2;
		}
		else
		{
			read(ifd, &ch1, 1);
			
			if(ch1 != 0)
			{
				sprintf(pszMessage, "打印机通讯错 黑标调整值%02xH", ch);
				return -3;
			}
		}

		return ch;
	}	

	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 黑标调整值设置

int ifd IN 打印机句柄
unsigned char chBlackMark IN 黑标调整值 17—127

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_black_mark_choose(int ifd, unsigned char chBlackMark)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x0A, 0x00, 0x07, 0x00, 0x3C};

	struct timeval tv;
	fd_set rfds;
	char ch;
	
	



	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);
	
	szSendBuf[5] = chBlackMark;
	szSendBuf[6] += chBlackMark;
	
	write(ifd, szSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____BID_choose___no return____________1\n", __FILE__);

		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		if(ch != 0)
		{
			printf("%s_____BID_choose____ch = %x___________2\n", __FILE__, ch);

			return -2;
		}
		else
		{
			return 0;
		}
	}			


	return -255;
}



/******************************************************************
功  能: 黑标调整值设置

int ifd IN 打印机句柄
unsigned char chBlackMark IN 黑标调整值 17—127
char *pszMessage IN/OUT 错误

返回值说明： 负数：错误 0：正确
******************************************************************/
int mpm78m_black_mark_choose_str(int ifd, unsigned char chBlackMark, char *pszMessage)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x0A, 0x00, 0x07, 0x00, 0x3C};

	struct timeval tv;
	fd_set rfds;
	char ch;
	
	



	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ifd, &rfds);
	
	szSendBuf[5] = chBlackMark;
	szSendBuf[6] += chBlackMark;
	
	write(ifd, szSendBuf, 7);
	if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
	{
		printf("%s_____BID_choose___no return____________1\n", __FILE__);
		strcpy(pszMessage, "打印机无响应");
		return -1;
	}
	else
	{
		read(ifd, &ch, 1);
		if(ch != 0)
		{
			printf("%s_____BID_choose____ch = %x___________2\n", __FILE__, ch);
			mpm78m_error_return(ch, pszMessage);
			return -2;
		}
		else
		{
			return 0;
		}
	}			

	strcpy(pszMessage, "程序错");
	return -255;
}



/******************************************************************
功  能: 按打印行进纸

int ifd IN 打印机句柄
unsigned char uchLine IN 进纸行数

返回值说明： -1：错误 0：正确
******************************************************************/
int mpm78m_feed_on_line(int ifd, unsigned char uchLine)
{
	unsigned char szBuf[] = { '\0' };

	while(uchLine != 0)
	{	
		if(mpm78m_print_str(ifd, szBuf) != 0)
		{
			return -1;
		}
		
		uchLine--;		
	}
	
	return 0;
}


#if 0

#include "../common/invoice.h"

struct invoice invoceTest = {
	1, 1, 1, 1, 1,
	0,0,
	0, 0, 0, "", "",
	"121000921201", "00000000",
	"01", "2010-6-1",
	"1455 4910 2024 3978 6005",
	"国家税务局",
	"纳税人单位名称",
	"纳税号",
	"机器编号"
};

int invoice_print(struct invoice *ptrInc)
{
	unsigned char buf[512];
	int printfd = open_printer_com();
	if( printfd < 0 ) return -1;
	
	mpm78m_read_black_mark(printfd);
	
	sprintf((char*)buf, "%s%s", "发票代码:", ptrInc->szBaseCode);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "发票号码:", ptrInc->szNumber);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "机打号码:", "00000002");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "机器编号:", ptrInc->uchFiscalCashRegister);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "收款单位:", ptrInc->uchTaxpayerDepartmentName);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "税务登记号:", ptrInc->uchTaxNo);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%-12s%s%s", "开票日期:", ptrInc->szTime, "收款员:", ptrInc->szCheckerCode);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "付款单位:", "个人");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	mpm78m_feed_on_line(printfd, 8);
	
	sprintf((char*)buf, "%s%s", "小写合计:", "1.00");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "大写合计:", "壹圆整");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "税控码:", ptrInc->szFiscalCode);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	close_printer_com(printfd);
}

int main()
{
	invoice_print(&invoceTest);
}

#endif


