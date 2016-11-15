/******************************************************************
** �ļ���: invoice.h
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



#ifndef INVOICE_H
#define INVOICE_H

#include "goods.h"

struct invoice{
	int  		iGoodsCategoryNumber;	//��Ʒ��������
	int		iTotal;
	int   		iCash;
	int  		iTotalCashDiscount;	//���ۿ۽��
	int  		iTotalDiscountRate;	//���ۿ���
	int 		iType; 			//��Ʊ����(�������ˣ��죩Ʊ,��Ʊ)

	struct goods	*ptrGoods;
	int		iGoodsNumber;

	int 		iCode;			//���������
	int  		iOneAmount;		//����1�Ľ��
	int  		iTwoAmount;		//����2�Ľ��
	char 		szOneMenuItem[24];	//����1�Ŀ�ѡ��,�������ÿ�Ϊ19λ
	char 		szTwoMenuItem[24];	//����2�Ŀ�ѡ��

	char 		szBaseCode[24];		//��Ʊ����
	char  		szNumber[12];		//��Ʊ���루��6λ��Ϊ����СƱ����ˮ�ţ�

	char 		szCheckerCode[8];	//�տ�Ա����
	char  		szTime[12];		//ʱ�䣨�ꡢ�¡��ա�ʱ���֣�


	char 		szFiscalCode[40];		//��α��
	char   		szPayerDepartmentName[204];	//���λ����40
 
	char 		uchTaxpayerDepartmentName[204];		//��˰�˵�λ����41
	char 		uchTaxNo[24];				//��˰��
	char 		uchFiscalCashRegister[20];		//˰���տ���������8	
}__attribute__ ((__packed__));


typedef struct invoice struInvoice;

int invoice_read(struct invoice *ptrInc);
int invoice_write(struct invoice *ptrInc);
int invoice_print(struct invoice *prtInc);

/*
#define INVOICE_FILE "/flash2/data/invoice.cat"
#define INVOICE_GOODS_FILE "/flash2/data/invoice_goods.cat"
*/

#define INVOICE_FILE "invoice.cat"
#define INVOICE_GOODS_FILE "invoice_goods.cat"


#endif
