/* 使用任何标准C编译器编译即可，实例中是用TC编译的，简单 */

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


int     Mem[MEM_SIZE];      /* 内存                 */

int     reg[NO_REGS];       /* 寄存器               */
FILE    *fp;                /* 文件指针              */




/* 获取每一行的相关信息 */
char GetInfLine(int No)
{
    int     i, j, size, ord, NoF;
    char    buffer[LINE_SIZE];  /* 用于读取一行文件内容 */
    char    Num[LINE_SIZE];


    /* 将文件指针移动到开头 */
    fseek(fp, 0, SEEK_SET);

ZAI:
    /* 获取一行信息         */
    for (i = 0; i < LINE_SIZE; i++)
    {
        if (feof(fp)) return 0;
        buffer[i] = fgetc(fp);
        if (buffer[i] == '\n') break;
    }
    buffer[i] = '\0';
    size = i+1;


    /* 获取编号             */
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

    
    /* 说明找到对应行 */
    if (NoF == No)      
    {

#ifdef PUTCODE
        printf("%06d", NoF);
#endif

        /* 开始查找指令命令 */
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



        /* 开始查找指令参数 */
        j = NoF = 0;
        for (i = ord; i < size; i++)
        {
            if ((buffer[i]>='0' && buffer[i]<='9') || buffer[i]=='(' || buffer[i]==')'
                || buffer[i]=='+' || buffer[i]=='-' || buffer[i]==',' || buffer[i]=='\0')
            {
                Num[j] = buffer[i];
                switch (Num[j])
                {
                    case ',':           /* 说明‘，’前为数字 */
                        Num[j] = '\0';
                        Ord.iarg[NoF] = atoi(Num);

#ifdef PUTCODE
                        printf("  %04d", Ord.iarg[NoF]);
#endif

                        NoF++;
                        j = -1;
                        break;

                    case '(':           /* 说明‘（’前为数字 */
                        Num[j] = '\0';
                        Ord.iarg[NoF] = atoi(Num);

#ifdef PUTCODE
                        printf("  %04d", Ord.iarg[NoF]);
#endif

                        NoF++;
                        j = -1;
                        break;

                    case ')':
                    case '\0':          /* 说明‘）’‘\0’前为第3个数字 */
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


/* 执行命令，操作寄存器 */
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



    /* 根据命令编号执行 */
    switch (Ord.iop)
    {
        case opHALT:                /* 结束命令 */
            printf("HALT %d,%d,%d\n", r, s, t);
            return 0;

        case opIN:                  /* 输入整数到r；s,t忽略 */
            printf("Input Value: ");
            scanf("%d", &reg[r]);
            return 1;

        case opOUT:                 /* 将r输出；s,t忽略     */
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



        case opLD:                  /* r = mem(d+s)；       */
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

    /* 初始化内存和积存器 */
    Mem[0] = MEM_SIZE-1;
    for (i = 1; i < MEM_SIZE; i++) Mem[i] = 0;
    for (i = 0; i < NO_REGS; i++) reg[i] = 0;

    i = 0;
    while (1)
    {
        i = reg[7];         /* 相当于IP寄存器，提取下一条执行指令地址                       */
        if (!GetInfLine(i))
        {
            printf("\n ERROR ");
            break;
        }
        reg[7] = i+1;


        if (!SetpTMMY())   /* 执行指令，IP只向有可能在此函数内被修改，模拟了CPU的执行流程  */
        {
            printf("\n END ");
            break;
        }
    }

    fclose(fp);
    getch();
}
