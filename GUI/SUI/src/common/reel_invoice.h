/******************************************************************
** �ļ���: goods.h
** Copyright (c) 2010 ɣ������ ����������
** ������: �½���
** �ա���: 2011��3��21��
** �޸���:
** �ա���:
** ��  ��: ��Ʊ�Ķ���
**         
**         
**         
**         
** ��  ;: 
**
** �桡��: 0.1
********************************************************************/

#ifndef  REEL_INVOICE_H
#define  REEL_INVOICE_H

//����Ʊ����
struct  reel_invoice
{
	int  	uiValidInvoiceNum;    				//������Ʊ����
	int  	uiReturnInvoiceNum;   				//��Ʊ����
	int  	uiInvalidInvoiceNum;  				//��Ʊ��
	int	ulTotalMoney; 			//������Ʊ�ܽ��
	int	ulReturnTotalMoney;			//��Ʊ�ܽ��

	char 	szName[204];		//˰�˵�λ����40
	char 	szTaxNo[24];						//��˰�˱���8
	char 	chFiscalCashRegister[16];			//˰���տ���������8	
	char 	szInvoiceBaseCode[20];				//��Ʊ����
	char 	szInvoiceStartNo[12];  				//��Ʊ��ʼ��
	char 	szInvoiceEndNo[12];    				//��Ʊ��ֹ��

	char 	szStartDate[12];						//��ʼʹ������ 2002-01-01
	char 	szEndDate[12];						//����ʹ������
}__attribute__ ((__packed__));;

typedef struct reel_invoice struReelInvoice;


#endif
