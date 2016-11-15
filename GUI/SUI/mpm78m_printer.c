/******************************************************************
** �ļ���: mpm78m_printer.c
** Copyright (c) 2010 ɣ������ ����������
** ������: ������
** �ա���: 2011��5��6��
** �޸���:
** �ա���:
** ��  ��: MPM78M��ӡ������
**         
**         
**         
**         
** ��  ;: 
**
** �桡��: 0.1
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


#include "mpm78m_printer.h"



/******************************************************************
��  ��: �򿪴�ӡ������


����ֵ˵���� -1������ ��������ӡ�����
******************************************************************/
int open_printer_com(void)
{
	int iSerial;
	char arrchSerialName[20] = "/dev/ttyS3";
	struct termios newtio, oldtio;
	
	
	
	
	
	
	
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



/******************************************************************
��  ��: ���󷵻�

char ch IN ��ӡ�����صĴ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ��
******************************************************************/
void mpm78m_error_return(char ch, char *pszMessage)
{
	strcpy(pszMessage, "");
	if(ch & 0x80 == 0x80)
	{
		strcat(pszMessage, "��ӡ���� ");
	}
	
	if(ch & 0x40 == 0x40)
	{
		strcat(pszMessage, "��ӡͷ���� ");
	}

	if(ch & 0x20 == 0x20)
	{
		strcat(pszMessage, "�ڱ������ ");
	}

	if(ch & 0x10 == 0x10)
	{
		strcat(pszMessage, "�ϵ��������� ");
	}

	if(ch & 0x08 == 0x08)
	{
		strcat(pszMessage, "�������ݴ��� ");
	}

	if(ch & 0x04 == 0x04)
	{
		strcat(pszMessage, "��ӡ��ȱֽ ");
	}

	if(ch & 0x02 == 0x02)
	{
		strcat(pszMessage, "��ӡ����λ�쳣!");
	}
}



/******************************************************************
��  ��: ����ӡ��״̬

int ifd IN ��ӡ�����

����ֵ˵���� ���������� 0����ȷ
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

		//����2�ֽ�����
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
��  ��: ����ӡ��״̬

int ifd IN ��ӡ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
		strcpy(pszMessage, "��ӡ������Ӧ");
		return -1;
	}
	else
	{
		char ch, ch1;
		
		
		
		
		
		read(ifd, &ch, 1);

		//����2�ֽ�����
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd,&rfds);

		if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
		{
			sprintf(pszMessage, "��ӡ������Ӧ ״̬%02xH", ch);
			return -2;
		}
		else
		{
			read(ifd, &ch1, 1);
			
			if(ch1 != 0)
			{
				sprintf(pszMessage, "��ӡ��ͨѶ�� ״̬%02xH", ch);
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

	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: �ڱ���

int ifd IN ��ӡ�����

����ֵ˵���� ���������� 0����ȷ
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
			printf("%s_____find_black_mark ����״̬:%x\n", __FILE__, ch);

			//�ȴ�����
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			FD_ZERO(&rfds);
			FD_SET(ifd, &rfds);
			
			if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
			{
				printf("%s_____find_black_mark ����Ӧ1\n", __FILE__);

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
��  ��: �ڱ���

int ifd IN ��ӡ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
		strcpy(pszMessage, "��ӡ������Ӧ");
		return -1;
	} 
	else 
	{
		read(ifd, &ch, 1);
    	
		if(ch != 0)
		{
			printf("%s_____find_black_mark ����״̬:%x\n", __FILE__, ch);

			//�ȴ�����
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			FD_ZERO(&rfds);
			FD_SET(ifd, &rfds);
			
			if (select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0) 
			{
				printf("%s_____find_black_mark ����Ӧ1\n", __FILE__);

				sprintf(pszMessage, "��ӡ������Ӧ1 ״̬%02xH", ch);
				return -2;
			} 
			else 
			{
				char ch1;
				
				
				
				
				
				read(ifd, &ch1, 1);
				
				if(ch1 != 0)
				{
					printf("%s_____find_black_mark err:%02x ret:%02x\n", __FILE__, ch, ch1);
					sprintf(pszMessage, "���ش� ״̬%02xH ����%02xH", ch, ch1);
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

	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: ��ӡ�ַ��������в��ܰ��������ַ� ������С��0x20(�ո�) ��
	һ��44���ַ�������ַ������������һ�У����Զ���ӻس����з�����ӡ
	����ַ�������һ�У����Զ����д�ӡ��ÿ�л��Զ���ӻس����з�����ӡ

int ifd IN ��ӡ�����
char *pszStr IN ����Ҫ��ӡ���ַ���

����ֵ˵���� ���������� 0����ȷ
******************************************************************/
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
			//ɨ��һ��
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
					//����
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
			printf("%s____print_mem78m_��ӡʱû�з���_________________1\n", __FILE__);
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



/******************************************************************
��  ��: ��ӡ�ַ���
	һ��44���ַ�������ַ������������һ�У����Զ���ӻس����з�����ӡ
	����ַ�������һ�У����Զ����д�ӡ��ÿ�л��Զ���ӻس����з�����ӡ

int ifd IN ��ӡ�����
char *pszStr IN ����Ҫ��ӡ���ַ���
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
			//ɨ��һ��
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
					//����
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
			printf("%s____print_mem78m_��ӡʱû�з���_________________1\n", __FILE__);
			strcpy(pszMessage, "��ӡ������Ӧ");
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
	
	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: ��ֽ ÿ������Ϊ0.176mm 30��Ϊ2��

int ifd IN ��ӡ�����
unsigned int uiFeedmm IN Ҫ��ֽ�ĵ�����

����ֵ˵���� ���������� 0����ȷ
******************************************************************/
int mpm78m_feed_mm(int ifd, unsigned int uiFeedmm)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x01, 0x00, 0x07, 0x00, 0x33};

	struct timeval tv;
	fd_set rfds;
	char ch;






	//printf("%s_____feed_mm____iFeedmm = %d___________0\n", __FILE__, iFeedmm);
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

			return -1;
		} 
		else 
		{
			read(ifd, &ch, 1);

			//printf("%s_____feed_mm__ch = %x_____________\n", __FILE__, ch);
			if(ch != 0)
			{
				printf("%s_____feed_mm____ch = %x___________2\n", __FILE__, ch);

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
	
	return -255;
}



/******************************************************************
��  ��: ���н�ֽ ÿ������Ϊ0.176mm 30��Ϊ2��

int ifd IN ��ӡ�����
unsigned int uiFeedmm IN Ҫ��ֽ�ĵ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
******************************************************************/
int mpm78m_feed_mm_str(int ifd, unsigned int uiFeedmm, char *pszMessage)
{
	unsigned char szSendBuf[] = {0x1b, 0x10, 0x01, 0x00, 0x07, 0x00, 0x33};

	struct timeval tv;
	fd_set rfds;
	char ch;






	//printf("%s_____feed_mm____iFeedmm = %d___________0\n", __FILE__, iFeedmm);
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
			strcpy(pszMessage, "��ӡ������Ӧ");
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
	
	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: ������ֽ ÿ������Ϊ0.176mm 30��Ϊ2��

int ifd IN ��ӡ�����
unsigned int uiBackmm IN Ҫ��ֽ�ĵ�����

����ֵ˵���� ���������� 0����ȷ
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
��  ��: ������ֽ ÿ������Ϊ0.176mm 30��Ϊ2��

int ifd IN ��ӡ�����
unsigned int uiBackmm IN Ҫ��ֽ�ĵ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
			strcpy(pszMessage, "��ӡ������Ӧ");
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
	
	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: BID��ӡ�������ȴ�ӡ��

int ifd IN ��ӡ�����

����ֵ˵���� ���������� 0����ȷ
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
��  ��: BID��ӡ�������ȴ�ӡ��

int ifd IN ��ӡ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
		strcpy(pszMessage, "��ӡ������Ӧ");
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

	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: BIDֵ���ã����������ã�

int ifd IN ��ӡ�����
unsigned char chAccuracy IN BIDֵ ���ȷ�Χ0 - 20

����ֵ˵���� ���������� 0����ȷ
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
��  ��: BIDֵ���ã����������ã�

int ifd IN ��ӡ�����
unsigned char chAccuracy IN BIDֵ ���ȷ�Χ0 - 20
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
		strcpy(pszMessage, "��ӡ������Ӧ");
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

	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: �Լ��ӡ

int ifd IN ��ӡ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
��  ��: �Լ��ӡ

int ifd IN ��ӡ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
		strcpy(ptrchMessage, "��ӡ������Ӧ");
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

	strcpy(ptrchMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: ���ڱ����ֵ

int ifd IN ��ӡ�����

����ֵ˵���� ���������� �������ڱ����ֵ
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

		//����2�ֽ�����
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
��  ��: ���ڱ����ֵ

int ifd IN ��ӡ�����
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� �������ڱ����ֵ
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
		strcpy(pszMessage, "��ӡ������Ӧ");
		return -1;
	}
	else
	{
		char ch, ch1;
		
		
		
		
		
		read(ifd, &ch, 1);

		//����2�ֽ�����
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ifd, &rfds);

		if(select(ifd + 1, &rfds, NULL, NULL, &tv) <= 0)
		{
			printf("%s_____read_black_mark___no return____ch = %x___________2\n", __FILE__, ch);
			sprintf(pszMessage, "��ӡ������Ӧ �ڱ����ֵ%02xH", ch);
			return -2;
		}
		else
		{
			read(ifd, &ch1, 1);
			
			if(ch1 != 0)
			{
				sprintf(pszMessage, "��ӡ��ͨѶ�� �ڱ����ֵ%02xH", ch);
				return -3;
			}
		}

		return ch;
	}	

	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: �ڱ����ֵ����

int ifd IN ��ӡ�����
unsigned char chBlackMark IN �ڱ����ֵ 17��127

����ֵ˵���� ���������� 0����ȷ
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
��  ��: �ڱ����ֵ����

int ifd IN ��ӡ�����
unsigned char chBlackMark IN �ڱ����ֵ 17��127
char *pszMessage IN/OUT ����

����ֵ˵���� ���������� 0����ȷ
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
		strcpy(pszMessage, "��ӡ������Ӧ");
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

	strcpy(pszMessage, "�����");
	return -255;
}



/******************************************************************
��  ��: ����ӡ�н�ֽ

int ifd IN ��ӡ�����
unsigned char uchLine IN ��ֽ����

����ֵ˵���� -1������ 0����ȷ
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


