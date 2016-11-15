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
**__attribute__ ((aligned(4)));
** �桡��: 0.1
********************************************************************/

#ifndef GOODS_H
#define GOODS_H

/*
struct goods
{
	int 	uchState;		//��Ʒ��״̬
	int 	uchCategory;		//��Ʒ�����Ĳ���
	int	uchTaxIndex;    	// 
	int	uchDiscount;		//��Ʒ�����ĵ�Ʒ�ۿ���
	int 	uiPrice;		//��Ʒ����,�洢ʱǰ�油0
	int 	iQuantity;		//��Ʒ�Ŀ������,�洢ʱǰ�油0
	char 	szNumber[13];		//��Ʒ����,���13���ַ�	
	char 	szName[64];		//��Ʒ����,���12������
	char 	szUnit[4];		//��Ʒ��λ,���1������	
	char 	szTaxItems[4];
}__attribute__ ((__packed__));
*/

struct goods
{
	char	szEntry[12];
	char	szName[20];
	int	iPrice;
	int 	iQuantity;
	char	szModel[12];
	char	szType[16];
}__attribute__ ((__packed__));;

typedef struct goods struGOODS;

int goods_init();
int goods_read(struct goods *ptrGoods);
int goods_write(struct goods *ptrGoods);
int goods_delete(struct goods *pgood);
void	goods_show(struct goods *ptrGoods);

int goods_export(char *file);
int goods_import(char *file);

/*
#define GOODS_EXNAME "/flash2/data/exgoods.dat"
#define GOODS_NAME "/flash2/data/goods.dat"
*/


#define GOODS_EXNAME "exgoods.dat"
#define GOODS_NAME "goods.dat"

#define GOODS_MAX_COUNT 100
#define GOODS_HASH_COUNT 10

#endif