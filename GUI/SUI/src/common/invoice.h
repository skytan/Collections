/******************************************************************
** 文件名: invoice.h
** Copyright (c) 2010 桑达商用 技术开发部
** 创建人: 陈晋飞
** 日　期: 2011年3月21日
** 修改人:
** 日　期:
** 功  能: 发票的定义
**         
**         
**         
**         
** 用  途: 
**
** 版　本: 0.1
********************************************************************/



#ifndef INVOICE_H
#define INVOICE_H

#include "goods.h"

struct invoice{
	int  		iGoodsCategoryNumber;	//商品种类数量
	int		iTotal;
	int   		iCash;
	int  		iTotalCashDiscount;	//总折扣金额
	int  		iTotalDiscountRate;	//总折扣率
	int 		iType; 			//发票类型(正常、退（红）票,废票)

	struct goods	*ptrGoods;
	int		iGoodsNumber;

	int 		iCode;			//币种组合码
	int  		iOneAmount;		//币种1的金额
	int  		iTwoAmount;		//币种2的金额
	char 		szOneMenuItem[24];	//币种1的可选项,其中信用卡为19位
	char 		szTwoMenuItem[24];	//币种2的可选项

	char 		szBaseCode[24];		//发票代码
	char  		szNumber[12];		//发票号码（后6位作为销售小票的流水号）

	char 		szCheckerCode[8];	//收款员代号
	char  		szTime[12];		//时间（年、月、日、时、分）


	char 		szFiscalCode[40];		//防伪码
	char   		szPayerDepartmentName[204];	//付款单位名称40
 
	char 		uchTaxpayerDepartmentName[204];		//纳税人单位名称41
	char 		uchTaxNo[24];				//纳税号
	char 		uchFiscalCashRegister[20];		//税控收款机机器编号8	
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
