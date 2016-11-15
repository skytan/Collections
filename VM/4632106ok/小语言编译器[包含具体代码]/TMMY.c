/* ʹ���κα�׼C���������뼴�ɣ�ʵ��������TC����ģ��� */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/******* const *******/
#define   MEM_SIZE  2048 /* increase for large programs */
#define   NO_REGS   8

#define   LINE_SIZE 512


/*#define   PUTCODE*/


typedef enum {
   /* RR instructions */
   opHALT,    /* RR     halt, operands are ignored */
   opIN,      /* RR     read into reg(r); s and t are ignored */
   opOUT,     /* RR     write from reg(r), s and t are ignored */
   opADD,    /* RR     reg(r) = reg(s)+reg(t) */
   opSUB,    /* RR     reg(r) = reg(s)-reg(t) */
   opMUL,    /* RR     reg(r) = reg(s)*reg(t) */
   opDIV,    /* RR     reg(r) = reg(s)/reg(t) */
   opRRLim,   /* limit of RR opcodes */

   /* RM instructions */
   opLD,      /* RM     reg(r) = mem(d+reg(s)) */
   opST,      /* RM     mem(d+reg(s)) = reg(r) */
   opRMLim,   /* Limit of RM opcodes */

   /* RA instructions */
   opLDA,     /* RA     reg(r) = d+reg(s) */
   opLDC,     /* RA     reg(r) = d ; reg(s) is ignored */
   opJLT,     /* RA     if reg(r)<0 then reg(7) = d+reg(s) */
   opJLE,     /* RA     if reg(r)<=0 then reg(7) = d+reg(s) */
   opJGT,     /* RA     if reg(r)>0 then reg(7) = d+reg(s) */
   opJGE,     /* RA     if reg(r)>=0 then reg(7) = d+reg(s) */
   opJEQ,     /* RA     if reg(r)==0 then reg(7) = d+reg(s) */
   opJNE,     /* RA     if reg(r)!=0 then reg(7) = d+reg(s) */
   opRALim    /* Limit of RA opcodes */
   } OPCODE;


typedef struct {
      int iop;
      int iarg[3];
   } INSTRUCTION;

INSTRUCTION Ord;



char * opCodeTab[]
        = {"HALT","IN","OUT","ADD","SUB","MUL","DIV","????",
            /* RR opcodes */
           "LD","ST","????",
           /* RM opcodes */
           "LDA","LDC","JLT","JLE","JGT","JGE","JEQ","JNE","????"
           /* RA opcodes */
          };


int     Mem[MEM_SIZE];      /* �ڴ�                 */

int     reg[NO_REGS];       /* �Ĵ���               */
FILE    *fp;                /* �ļ�ָ��              */




/* ��ȡÿһ�е������Ϣ */
char GetInfLine(int No)
{
    int     i, j, size, ord, NoF;
    char    buffer[LINE_SIZE];  /* ���ڶ�ȡһ���ļ����� */
    char    Num[LINE_SIZE];


    /* ���ļ�ָ���ƶ�����ͷ */
    fseek(fp, 0, SEEK_SET);

ZAI:
    /* ��ȡһ����Ϣ         */
    for (i = 0; i < LINE_SIZE; i++)
    {
        if (feof(fp)) return 0;
        buffer[i] = fgetc(fp);
        if (buffer[i] == '\n') break;
    }
    buffer[i] = '\0';
    size = i+1;


    /* ��ȡ���             */
    j = 0;
    for (i = 0; i < size; i++)
    {
        if ((buffer[i]>='0' && buffer[i]<='9') || (buffer[i] == ':'))
        {
            Num[j] = buffer[i];
            if (Num[j] == ':')
            {
                Num[j] = '\0';
                NoF = atoi(Num);
                ord = i+1;
                break;
            }
            j++;
        }
    }

    
    /* ˵���ҵ���Ӧ�� */
    if (NoF == No)      
    {

#ifdef PUTCODE
        printf("%06d", NoF);
#endif

        /* ��ʼ����ָ������ */
        j = 0;
        for (i = ord; i < size; i++)
        {
            if ((buffer[i]>='A' && buffer[i]<='Z') || (buffer[i]>='0' && buffer[i]<='9'))
            {
                Num[j] = buffer[i];
                if (Num[j]>='0' && Num[j]<='9')
                {
                    Num[j] = '\0';

#ifdef PUTCODE
                    printf("  % 4s", Num);
#endif

                    ord = i;

                    for (j = 0; j < 20; j++)
                    {
                        if (!strcmp(Num, opCodeTab[j]))
                        {
                            Ord.iop = j;

#ifdef PUTCODE
                            printf("   %02d        ", Ord.iop);
#endif

                            break;
                        }
                    }
                    break;
                }
                j++;
            }
        }



        /* ��ʼ����ָ����� */
        j = NoF = 0;
        for (i = ord; i < size; i++)
        {
            if ((buffer[i]>='0' && buffer[i]<='9') || buffer[i]=='(' || buffer[i]==')'
                || buffer[i]=='+' || buffer[i]=='-' || buffer[i]==',' || buffer[i]=='\0')
            {
                Num[j] = buffer[i];
                switch (Num[j])
                {
                    case ',':           /* ˵��������ǰΪ���� */
                        Num[j] = '\0';
                        Ord.iarg[NoF] = atoi(Num);

#ifdef PUTCODE
                        printf("  %04d", Ord.iarg[NoF]);
#endif

                        NoF++;
                        j = -1;
                        break;

                    case '(':           /* ˵��������ǰΪ���� */
                        Num[j] = '\0';
                        Ord.iarg[NoF] = atoi(Num);

#ifdef PUTCODE
                        printf("  %04d", Ord.iarg[NoF]);
#endif

                        NoF++;
                        j = -1;
                        break;

                    case ')':
                    case '\0':          /* ˵����������\0��ǰΪ��3������ */
                        Num[j] = '\0';
                        Ord.iarg[NoF] = atoi(Num);

#ifdef PUTCODE
                        printf("  %04d", Ord.iarg[NoF]);
#endif

                        NoF++;
                        j = -1;
                        goto OUT;

                }
                j++;
            }
        }

OUT:

#ifdef PUTCODE
        printf("  %04d\n", NoF);
#endif
        return 1;
    }
    else
    {
        goto ZAI;
    }
}


/* ִ����������Ĵ��� */
char SetpTMMY()
{
    int r, s, t, m;


    /*printf("pc=%d   iarg1=%d    iarg2=%d      iarg3=%d\n", i, Ord.iarg[0], Ord.iarg[1], Ord.iarg[2]);
    getch();
    */


    if (Ord.iop < opRRLim)
    {
        r = Ord.iarg[0];
        s = Ord.iarg[1];
        t = Ord.iarg[2];
    }


    if (Ord.iop>opRRLim && Ord.iop<opRMLim)
    {
        r = Ord.iarg[0];
        s = Ord.iarg[2];
        m = Ord.iarg[1] + reg[s];

        if (m<0 || m>MEM_SIZE) return 0;
    }


    if (Ord.iop>opRMLim)
    {
        r = Ord.iarg[0];
        s = Ord.iarg[2];
        m = Ord.iarg[1] + reg[s];    
    }



    /* ����������ִ�� */
    switch (Ord.iop)
    {
        case opHALT:                /* �������� */
            printf("HALT %d,%d,%d\n", r, s, t);
            return 0;

        case opIN:                  /* ����������r��s,t���� */
            printf("Input Value: ");
            scanf("%d", &reg[r]);
            return 1;

        case opOUT:                 /* ��r�����s,t����     */
            printf("Ouput Value: %d\n", reg[r]);
            return 1;
            
        case opADD:                 /* r = s+t              */
            reg[r] = reg[s] + reg[t];
            return 1;

        case opSUB:                 /* r = s-t              */
            reg[r] = reg[s] - reg[t];
            return 1;

        case opMUL:                 /* r = s*t              */
            reg[r] = reg[s] * reg[t];
            return 1;

        case opDIV:                 /* r = s/t              */
            if (reg[r] != 0)
                reg[r] = reg[s] / reg[t];
            else
                return 0;
            return 1;



        case opLD:                  /* r = mem(d+s)��       */
            reg[r] = Mem[m];
            return 1;

        case opST:                  /* mem(d+s) = r         */
            Mem[m] = reg[r];
            return 1;
            


        case opLDA:                 /* r = d+s              */
            reg[r] = m;
            return 1;

        case opLDC:                 /* r = d                */
            reg[r] = Ord.iarg[1];
            return 1;

        case opJLT:                 /* if r<0 then reg(7) = d+s     */
            if (reg[r] < 0) reg[7] = m;
            return 1;

        case opJLE:                 /* if r<=0 then reg(7) = d+s    */
            if (reg[r] <= 0) reg[7] = m;
            return 1;

        case opJGT:                 /* if r>0 then reg(7) = d+s     */
            if (reg[r] > 0) reg[7] = m;
            return 1;

        case opJGE:                 /* if r>=0 then reg(7) = d+s    */
            if (reg[r] >= 0) reg[7] = m;
            return 1;

        case opJEQ:                 /* if r==0 then reg(7) = d+s    */
            if (reg[r] == 0) reg[7] = m;
            return 1;

        case opJNE:                 /* if r!=0 then reg(7) = d+s    */
            if (reg[r] != 0) reg[7] = m;
            return 1;

    }
}


void main(int argc, char * argv[])
{
    int     i;


    if (argc != 2)
    {
        printf("usage: %s <filename>\n", argv[1]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("file '%s' not found\n", argv[1]);
        exit(1);
    }

    /* ��ʼ���ڴ�ͻ����� */
    Mem[0] = MEM_SIZE-1;
    for (i = 1; i < MEM_SIZE; i++) Mem[i] = 0;
    for (i = 0; i < NO_REGS; i++) reg[i] = 0;

    i = 0;
    while (1)
    {
        i = reg[7];         /* �൱��IP�Ĵ�������ȡ��һ��ִ��ָ���ַ                       */
        if (!GetInfLine(i))
        {
            printf("\n ERROR ");
            break;
        }
        reg[7] = i+1;


        if (!SetpTMMY())   /* ִ��ָ�IPֻ���п����ڴ˺����ڱ��޸ģ�ģ����CPU��ִ������  */
        {
            printf("\n END ");
            break;
        }
    }

    fclose(fp);
    getch();
}
