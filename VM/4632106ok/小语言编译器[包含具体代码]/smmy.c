#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define   LINE_SIZE 512


typedef enum
   {
    IF, THEN, ELSE ,END ,REPEAT, UNTIL, READ, WRITE,
    FUZHI, FENGHAO, JIA, JIAN, CHENG, CHU, DENGYU, DAYU, XIAOYU, ZKUOHAO, YKUOHAO,
    BLIANG, CLIANG, LAST
   } TokenType;



#define MAXCHILDREN 3

typedef struct treeNode
{
    struct treeNode * child[MAXCHILDREN];  /* 最多3个叶子                  */
    struct treeNode * sibling;             /* 下一个子结构                 */
    struct treeNode * father;              /* 上一个父结构                 */

    int lineno;                            /* 此语句在文本中源代码中的行号 */
    int bcliang;                           /* 若为变/常量，记录此变/常量在内存中的位置 */

    struct treeNode2                       /* 属性结构                     */
    {
        TokenType   op;                    /* 若为关键字，标记为何种关键字 */
        int         qx;                    /* 若为计算符，标记权限值，实现先后计算 */
        char        libian;                /* if, repeat等语句需要返回历遍时的标记, 用在打印语法树 */
        char        csdm;                  /* if, repeat等语句需要返回历遍时的标记, 用在转化为汇编代码 */
        char        name[20];              /* 若为变量，记录变量名称       */
    } attr;
}TreeNode;


char *KeyGJZ[] = {"if", "then", "else", "end", "repeat", "until", "read", "write",
                 ":=", ";", "+", "-", "*", "/", "=", ">", "<", "(", ")"};

int     BCLiangCount = 0;                   /* 记录目前有多少变量           */
int     strCLJLCount = 0;                   /* 记录目前有多少变量的名称     */
char    strCLJL[1000][20];                  /* 用于记录当前所以变量的名称   */


FILE    *fp, *fp2;
int     lineNoError = 0;
int     lineNo = 1;

TreeNode    *Head = NULL, *Head2 = NULL, *Biao = NULL, *Temp = NULL, *Last = NULL;



char MakeTest(TreeNode *Start)
{
    int i, j, qxzy = 0;

    if (Temp == Last)
    {
        /*fprintf(fp2, "% 3d: % 6s  6,0(7)\n", lineNo++, "LDC");*/

        for (i = 0; i < strCLJLCount; i++)
            if (!strcmp(strCLJL[i], Temp->attr.name)) break;

        fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "LD", i);
        return 1;
    }
    else
    {
        /*fprintf(fp2, "% 3d: % 6s  6,0(7)\n", lineNo++, "LDC");*/


        /* 进行算术计算 */
        j = 0;
        while (Temp != Last->child[2])
        {
            if (Temp->attr.op == CLIANG || Temp->attr.op == BLIANG)
            {
                for (i = 0; i < strCLJLCount; i++)
                    if (!strcmp(strCLJL[i], Temp->attr.name)) break;

                /*Mem[j+strCLJLCount] = Mem[i];*/
                fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "LD", i);
                fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", j+strCLJLCount+BCLiangCount);
                j++;
            }

            Temp = Temp->child[2];
        }
                    
BACK:
        qxzy = 0;
        Head2 = NULL;
        Temp = Start;
        while (Temp->child[2] != Last->child[2])
        {
            if (Temp->attr.op != CLIANG && Temp->attr.op != BLIANG)
            {
                if (Temp->attr.qx > qxzy)
                {
                    Head2 = Temp;
                    qxzy = Temp->attr.qx;
                }
            }
            Temp = Temp->child[2];
        }

        j = 0;
        Temp = Start;
        while (Temp->child[2] != Last->child[2])
        {
            if (Temp->attr.op != CLIANG && Temp->attr.op != BLIANG)
            {
                j++;
                if (Temp == Head2) break;
            }
            Temp = Temp->child[2];
        }


        /* 说明找到权值最大的算术节点 */
        if (Head2 != NULL)
        {
            fprintf(fp2, "% 3d: % 6s  1,%d(6)\n", lineNo++, "LD", j+strCLJLCount+BCLiangCount-1);
            fprintf(fp2, "% 3d: % 6s  2,%d(6)\n", lineNo++, "LD", j+strCLJLCount+BCLiangCount);
                         

            Head2->attr.qx = 0;
            switch (Head2->attr.op)
            {
                case JIA:
                    fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "ADD");
                    /*qxzy = Mem[j+strCLJLCount-1] + Mem[j+strCLJLCount];*/ break;

                case JIAN:
                    fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "SUB");
                    /*qxzy = Mem[j+strCLJLCount-1] - Mem[j+strCLJLCount];*/ break;

                case CHENG:
                    fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "MUL");
                    /*qxzy = Mem[j+strCLJLCount-1] * Mem[j+strCLJLCount];*/ break;

                case CHU:
                    fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "DIV");
                    /*qxzy = Mem[j+strCLJLCount-1] / Mem[j+strCLJLCount];*/ break;
            }
            /*Mem[j+strCLJLCount] = Mem[j+strCLJLCount-1] = qxzy;*/


            /* 向后推广值 */
            Temp = Head2;
            i = j;
            while (Temp->child[2] != Last->child[2])
            {
                if (Temp->attr.op != CLIANG && Temp->attr.op != BLIANG)
                {
                    if (Temp->attr.qx == 0)
                    {
                        fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i+strCLJLCount+BCLiangCount-1);
                        fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i+strCLJLCount+BCLiangCount);
                        /*Mem[i+strCLJLCount] = Mem[i+strCLJLCount-1] = qxzy;*/
                        i++;
                    }
                    else
                        break;
                }
                Temp = Temp->child[2];
            }

            /* 向前推广值 */
            Temp = Head2->father;
            i = j;
            while (Temp->father != Head)
            {
                if (Temp->attr.op != CLIANG && Temp->attr.op != BLIANG)
                {
                    if (Temp->attr.qx == 0)
                    {
                        i--;
                        fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i+strCLJLCount+BCLiangCount-1);
                        fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i+strCLJLCount+BCLiangCount);
                        /*Mem[i+strCLJLCount] = Mem[i+strCLJLCount-1] = qxzy;*/
                    }
                    else
                        break;
                }
                Temp = Temp->father;
            }

goto BACK;
        }
        else
        {
            return 2;
        }
    }
}

char GetInfLine()
{

    int     qxzy = 0, lenstr, i = 0, j;
    char    str, buffer[LINE_SIZE], chuli[LINE_SIZE];
    char    Modekey = -1;       /* 标记当前应该在搜索何种字符   */
    char    ZSkey = 0;          /* 标记是否注释开始             */
    char    ModeGJZ;            /* 搜索关键字，标记型号         */

    char    strCL[20];
    char    strCLKey = 0;       /* 标记之前是否记录为一个常量或变量 */
    char    ZKHKey = 0;         /* 标记左括号 */


    fp2 = fopen("the1.txt", "w+");


    /* 第一遍扫描，将关键字，变量，常量，表达式罗列出来 */
    while (1)
    {
        str = fgetc(fp);
        if (feof(fp))
        {
            buffer[i] = '\0';
            fprintf(fp2, "%06d:  %s", lineNo, buffer);
            fclose(fp2);
            break;
        }


        if ((str != ' ') && (str != '\0') && (str != '\n'))
        {
            if (ZSkey == 0)
            {
                if (!((str >= 'a' && str <= 'z') || (str >= 'A' && str <= 'Z')
                    || (str == '{') || (str == '}') || (str == '(') || (str == ')')
                        || (str == ':') || (str == '=') || (str == '*') || (str == '/')
                            ||(str == '+') || (str == '-') || (str == ';') || (str == ' ')
                                || (str >= '0' && str <= '9') || (str == '\0') || (str == '\n')
                                    || (str == '>') || (str == '<')))
                {
                    fclose(fp2);
                    return 0;
                }


                if (str == '}') return 0;     /* 说明{提前出现，没有{}配对    */
                if (str == '{')
                {
                    ZSkey = 1;
                }
                else
                {
                    buffer[i++] = str;


                    if ((str == '=') || (str == '+') || (str == '-')
                        || (str == '*') || (str == '/') || (str == ';')
                            || (str == '<') || (str == '>') || (str == '(') || (str == ')'))
                    {
                        buffer[i-1] = '\0';
                        fprintf(fp2, "%06d:  %s\n%06d:  %c\n", lineNo, buffer, lineNo, str);
                        i = 0;   
                    }


                    if (str == ':')           /* 说明一个赋值号开始           */
                    {
                        buffer[i-1] = '\0';
                        fprintf(fp2, "%06d:  %s\n", lineNo, buffer);
                        i = 0;

                        str = fgetc(fp);
                        if (str == '=')       /* 说明赋值号正确               */
                        {
                            fprintf(fp2, "%06d:  :=\n", lineNo);
                            /*str = fgetc(fp);*/
                        }
                        else
                        {
                            fclose(fp2); 
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if (str == '{') return 0;     /* 说明连续有2个{，没有{}配对   */
                if (str == '}') ZSkey = 0;    /* 说明注释结束                 */
            }
        }
        else
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                fprintf(fp2, "%06d:  %s\n", lineNo, buffer);
                i = 0;
            }

            if (str == '\n' || str == '\0') lineNo++;
        }
    }




    /* 构造语法树 */
    fp2 = fopen("the1.txt", "r");
    while (1)
    {
        fgets(buffer, LINE_SIZE, fp2);

        lenstr = strlen(buffer);
        if (lenstr > 10)    /* 去除了内容为空的行 */
        {
            memcpy(chuli, buffer, 6);
            chuli[6] = '\0';
            lineNo = atoi(chuli);

            memcpy(chuli, &buffer[9], lenstr-10);
            chuli[lenstr-10] = '\0';

            
            for (i = 0; i < 19; i++)
            {
                /* 说明是个关键字 */
                if (!strcmp(chuli, KeyGJZ[i]))
                {
                    switch (i)
                    {
                        case ZKUOHAO:           /* 说明当前关键字为(        */
                            ZKHKey = 1;
                            qxzy += 4;
                            break;

                        case YKUOHAO:           /* 说明当前关键字为)        */
                            if (strCLKey == 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }

                            qxzy -= 4;
                            break;

                        case XIAOYU:            /* 说明当前关键字为<        */
                        case DAYU:              /* 说明当前关键字为>        */
                        case DENGYU:            /* 说明当前关键字为=        */
                            if (ZKHKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */   
                            }

                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }

                            switch (Modekey)
                            {
                                case XIAOYU:
                                case DAYU:
                                case DENGYU:
                                case FENGHAO:
                                case FUZHI:
                                case WRITE:
                                case READ:
                                case REPEAT:
                                case END:
                                case ELSE:
                                case THEN:
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */  


                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case UNTIL:
                                case IF:
                                    /* 说明+之前没有一个变量 */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */
                                    }
                                    else
                                    {
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        Temp->father = Head;
                                        Temp->lineno = lineNo;

                                        if ((i == DENGYU) || (i == DAYU) || (i == XIAOYU))
                                        {
                                            Temp->attr.qx = qxzy+1;
                                        }
                                        else
                                        {
                                            if (i == JIA || i == JIAN)
                                                Temp->attr.qx = qxzy+2;
                                            else
                                                Temp->attr.qx = qxzy+3;
                                        }


                                        Temp->attr.op = i;
                                        strcpy(Temp->attr.name, strCL);
                                        Modekey = i;


                                        Head->child[2] = Temp;
                                        Head = Temp;

                                        strCLKey = 0;   
                                    }
                                    break;
                            }
                            break;


                        case CHU:               /* 说明当前关键字为/        */
                        case CHENG:             /* 说明当前关键字为*        */
                        case JIAN:              /* 说明当前关键字为-        */
                        case JIA:               /* 说明当前关键字为+        */
                            if (ZKHKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */   
                            }


                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }

                            switch (Modekey)
                            {
                                case READ:
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */

                                case XIAOYU:
                                case DAYU:
                                case DENGYU:
                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case FENGHAO:
                                case FUZHI:
                                case WRITE:
                                case UNTIL:
                                case REPEAT:
                                case END:
                                case ELSE:
                                case THEN:
                                case IF:
                                    /* 说明+之前没有一个变量 */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */
                                    }
                                    else
                                    {
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        Temp->father = Head;
                                        Temp->lineno = lineNo;

                                        if ((i == DENGYU) || (i == DAYU) || (i == XIAOYU))
                                        {
                                            Temp->attr.qx = qxzy+1;
                                        }
                                        else
                                        {
                                            if (i == JIA || i == JIAN)
                                                Temp->attr.qx = qxzy+2;
                                            else
                                                Temp->attr.qx = qxzy+3;
                                        }


                                        Temp->attr.op = i;
                                        strcpy(Temp->attr.name, strCL);
                                        Modekey = i;


                                        Head->child[2] = Temp;
                                        Head = Temp;

                                        strCLKey = 0;   
                                    }
                                    break;
                            }
                            break;

                        case FUZHI:             /* 说明当前关键字为:=       */
                            if (ZKHKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */   
                            }


                            switch (Modekey)
                            {
                                case WRITE:     /* 说明之前是write关键字    */
                                case UNTIL:     /* 说明之前是until关键字    */
                                case XIAOYU:
                                case DAYU:
                                case DENGYU:
                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case FUZHI:
                                case READ:
                                case IF:        /* 说明之前也是if关键字   */
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */

                                case REPEAT:    /* 说明之前是repeat关键字    */
                                case END:       /* 说明之前是end关键字    */
                                case ELSE:      /* 说明之前是else关键字   */
                                case THEN:      /* 说明之前是then关键字   */
                                case FENGHAO:
                                case -1:
                                    /* 说明:=之前没有一个变量 */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */
                                    }
                                    else
                                    {
                                        /* 判断是否为一变量 */
                                        int strl = strlen(strCL);

                                        for (j = 0; j < strl; j++)
                                            if (strCL[j] >= '0' && strCL[j] <= '9') break;

                                        /* 为合法变量 */
                                        if (j == strl)
                                        {
                                            if (Head == NULL)
                                            {
                                                Head = (TreeNode *)malloc(sizeof(TreeNode));
                                                Head->child[0] = NULL;
                                                Head->child[1] = NULL;
                                                Head->child[2] = NULL;
                                                Head->sibling = NULL;
                                                Head->father = NULL;


                                                Head->sibling = NULL;
                                                Head->father = NULL;
                                                Head->lineno = lineNo;
                                                Head->attr.op = i;
                                                strcpy(Head->attr.name, strCL);


                                                Biao = Head;
                                            }  
                                            else
                                            {
                                                /* 开辟:=关键字节点 */
                                                Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                                Temp->child[0] = NULL;
                                                Temp->child[1] = NULL;
                                                Temp->child[2] = NULL;
                                                Temp->sibling = NULL;
                                                Temp->father = NULL;

                                                Temp->father = Head;
                                                Temp->lineno = lineNo;
                                                Temp->attr.op = i;
                                                strcpy(Temp->attr.name, strCL);

                                                switch (Modekey)
                                                {
                                                    case REPEAT:
                                                        Head->child[0] = Temp;
                                                        break; 

                                                    case THEN:
                                                        Head->child[1] = Temp;
                                                        break;

                                                    case ELSE:
                                                        Head->child[2] = Temp;
                                                        break; 

                                                    default:
                                                        Head->sibling = Temp;      
                                                }


                                                Head = Temp;
                                            }

                                            Modekey = i;
                                            strCLKey = 0;
                                        }
                                        else
                                        {
                                            lineNoError = lineNo;
                                            fclose(fp2);
                                            return 0;       /* 语法错误               */
                                        }
                                    }
                                    break;
                            }
                            break;


                        case UNTIL:             /* 说明当前关键字为until        */
                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            } 
                            else
                            {
                                /* 若括号不对称 */
                                if (qxzy != 0)
                                {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */
                                }


                                switch (Modekey)
                                {
                                    case XIAOYU:
                                    case DAYU:
                                    case DENGYU:
                                    case CHU:
                                    case CHENG:
                                    case JIAN:
                                    case JIA:
                                    case FUZHI:
                                    case WRITE:
                                    case READ:
                                    case UNTIL:
                                    case REPEAT:
                                    case ELSE:
                                    case THEN:
                                    case IF:
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */

                                    
                                    case FENGHAO:
                                    case END:
                                        /* 回述到repeat节点 */
                                        Temp = NULL;
                                        while (!(Head->attr.op == REPEAT && Temp == Head->child[0]))
                                        {
                                            /* 说明已经到了树顶 */
                                            if (Head->father == NULL)
                                            {
                                                lineNoError = lineNo;
                                                fclose(fp2);
                                                return 0;       /* 语法错误              */    
                                            }
                                            Temp = Head;
                                            Head = Head->father;
                                        }

                                        /* 说明有变量，将标记清0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                        break;
                                }
                            }
                            break;

                        case FENGHAO:           /* 说明当前关键字为;        */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }


                            switch (Modekey)
                            {
                                case FENGHAO:
                                case UNTIL:         /* 说明之前是until关键字  */
                                case REPEAT:        /* 说明之前是repeat关键字 */
                                case END:           /* 说明之前是end关键字    */
                                case ELSE:          /* 说明之前是else关键字   */
                                case THEN:          /* 说明之前是then关键字   */
                                case IF:            /* 说明之前也是if关键字   */
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */

                                case XIAOYU:
                                case DAYU:
                                case DENGYU:
                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case FUZHI:
                                case WRITE:
                                case READ:          /* 说明之前也是个read语句 */
                                    /* 说明read之后没有变量 */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */    
                                    }
                                    else
                                    {
                                        /* 返回寻找表达式节点 */
                                        if (Modekey != READ)
                                        {
                                            while (Head->attr.op != FUZHI && Head->attr.op != IF && Head->attr.op != REPEAT
                                                    && Head->attr.op != READ && Head->attr.op != WRITE)
                                            {
                                                if (Head->father == NULL)
                                                {
                                                    lineNoError = lineNo;
                                                    fclose(fp2);
                                                    return 0;       /* 语法错误               */        
                                                }

                                                Head = Head->father;
                                            }
                                        }


                                        /* 说明有变量，将标记清0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                    }
                                    break;
                            }
                            break;

                        case REPEAT:            /* 说明当前关键字为repeat        */
                        case IF:                /* 说明当前关键字为if       */
                            /* 若括号不对称 */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }


                            if (strCLKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }


                            if (Head == NULL)
                            {
                                Head = (TreeNode *)malloc(sizeof(TreeNode));
                                Head->child[0] = NULL;
                                Head->child[1] = NULL;
                                Head->child[2] = NULL;
                                Head->sibling = NULL;
                                Head->father = NULL;

                                if (i == IF)
                                    Head->bcliang = BCLiangCount++;

                                Head->sibling = NULL;
                                Head->father = NULL;
                                Head->lineno = lineNo;
                                Head->attr.op = i;
                                Modekey = i;

                                Biao = Head;
                            }
                            else
                            {
                                switch (Modekey)
                                {
                                    case UNTIL:         /* 说明之前是until关键字  */
                                    case WRITE:         /* 说明之前是write关键字  */
                                    case READ:          /* 说明之前也是个read语句 */
                                    case XIAOYU:        /* 说明之前是<关键字      */
                                    case DAYU:          /* 说明之前是>关键字      */
                                    case DENGYU:        /* 说明之前是=关键字      */
                                    case CHU:           /* 说明之前是/关键字      */
                                    case CHENG:         /* 说明之前是*关键字      */
                                    case JIAN:          /* 说明之前是-关键字      */
                                    case JIA:           /* 说明之前是+关键字      */
                                    case FUZHI:         /* 说明之前是:=关键字     */
                                    case IF:            /* 说明之前也是if关键字   */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */

                                    case THEN:          /* 说明之前是then关键字   */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        if (i == IF)
                                            Temp->bcliang = BCLiangCount++;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->child[1] = Temp;
                                        Head = Temp;
                                        break;

                                    case ELSE:          /* 说明之前是else关键字   */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        if (i == IF)
                                            Temp->bcliang = BCLiangCount++;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->child[2] = Temp;
                                        Head = Temp;
                                        break;

                                    case FENGHAO:
                                    case END:           /* 说明之前是end关键字    */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        if (i == IF)
                                            Temp->bcliang = BCLiangCount++;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->sibling = Temp;
                                        Head = Temp;
                                        break;

                                    case REPEAT:        /* 说明之前是repeat关键字 */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        if (i == IF)
                                            Temp->bcliang = BCLiangCount++;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->child[0] = Temp;
                                        Head = Temp;
                                        break;
                                }
                            }
                            break;

                        case THEN:              /* 说明当前关键字为then     */
                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            } 
                            else
                            {
                                /* 若括号不对称 */
                                if (qxzy != 0)
                                {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */
                                }



                                /* 这说明之前没有记录一个变量，then之前至少是个变量或常量 */
                                if (strCLKey == 0)
                                {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */
                                }

                                switch (Modekey)
                                {   
                                    /*case DENGYU: 说明之前是=关键字      */
                                    case FENGHAO:
                                    case FUZHI: /* 说明之前是:=关键字     */
                                    case WRITE: /* 说明之前是write关键字  */
                                    case READ:  /* 说明之前也是个read语句 */
                                    case UNTIL: /* 说明之前是until关键字  */
                                    case REPEAT:/* 说明之前是repeat关键字 */    
                                    case END:   /* 说明之前是end关键字    */
                                    case ELSE:  /* 说明之前是else关键字   */
                                    case THEN:  /* 说明之前是then关键字   */
                                    case IF:    /* 说明之前是if关键字     */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */

                                    case XIAOYU:
                                    case DAYU:
                                    case DENGYU:
                                    case CHU:
                                    case CHENG:
                                    case JIAN:
                                    case JIA:
                                        /* 返回寻找 if 节点 */
                                        while (Head->attr.op != FUZHI && Head->attr.op != IF)
                                        {
                                            /* 说明已经到了树顶 */
                                            if (Head->father == NULL)
                                            {
                                                lineNoError = lineNo;
                                                fclose(fp2);
                                                return 0;       /* 语法错误              */    
                                            }
                                            Head = Head->father;
                                        }

                                        if (Head->attr.op == FUZHI)
                                        {
                                            lineNoError = lineNo;
                                            fclose(fp2);
                                            return 0;       /* 语法错误              */
                                        }

                                        /* 说明有变量，将标记清0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                        break;
                                }
                            }
                            break;

                        case END:
                        case ELSE:              /* 说明当前关键字为else     */
                            /* 若括号不对称 */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }


                            if (strCLKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }


                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }
                            else
                            {
                                switch (Modekey)
                                {
                                    case UNTIL: /* 说明之前是end关键字    */
                                    case XIAOYU:
                                    case DAYU:
                                    case DENGYU:
                                    case CHU:
                                    case CHENG:
                                    case JIAN:
                                    case JIA:
                                    case FUZHI:
                                    case WRITE:
                                    case READ:
                                    case REPEAT:
                                    case ELSE:
                                    case THEN:
                                    case IF:    /* 说明之前是if关键字     */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */

                                    case FENGHAO:
                                    case END:   /* 说明之前是end关键字    */
                                        /* 找到于自己对应的IF节点 */
                                        Temp = Biao;
                                        if (i == ELSE)
                                        {
                                            while (!(Head->attr.op == IF && Head->child[1] == Temp))
                                            {
                                                Temp = Head;
                                                Head = Head->father;
                                            }
                                        }
                                        else
                                        {
                                            while (!(Head->attr.op == IF && (Head->child[1] == Temp || Head->child[2] == Temp)))
                                            {
                                                Temp = Head;
                                                Head = Head->father;
                                            }


                                        }

                                        /* 说明有变量，将标记清0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                        break;
                                }
                            }
                            break;

                        case WRITE:             /* 说明当前关键字为write    */
                        case READ:              /* 说明当前关键字为read     */
                            /* 若括号不对称 */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* 语法错误               */
                            }


                            if (strCLKey == 1)
                            {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* 语法错误               */
                            }


                            if (Head == NULL)
                            {
                                Head = (TreeNode *)malloc(sizeof(TreeNode));
                                Head->child[0] = NULL;
                                Head->child[1] = NULL;
                                Head->child[2] = NULL;
                                Head->sibling = NULL;
                                Head->father = NULL;

                                Head->sibling = NULL;
                                Head->father = NULL;
                                Head->lineno = lineNo;
                                Head->attr.op = i;
                                Modekey = i;

                                Biao = Head;
                            }
                            else
                            {
                                switch (Modekey)
                                {
                                    case FENGHAO:       /* 说明之前是;关键字      */
                                    case END:           /* 说明之前是end关键字    */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->sibling = Temp;
                                        Head = Temp;
                                        break;

                                    case UNTIL:         /* 说明之前是until关键字  */
                                    case WRITE:         /* 说明之前是write关键字  */
                                    case READ:          /* 说明之前也是个read语句 */
                                    case XIAOYU:        /* 说明之前是<关键字      */
                                    case DAYU:          /* 说明之前是>关键字      */
                                    case DENGYU:        /* 说明之前是=关键字      */
                                    case CHU:           /* 说明之前是/关键字      */
                                    case CHENG:         /* 说明之前是*关键字      */
                                    case JIAN:          /* 说明之前是-关键字      */
                                    case JIA:           /* 说明之前是+关键字      */
                                    case FUZHI:         /* 说明之前是:=关键字     */
                                    case IF:            /* 说明之前是if关键字     */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* 语法错误               */

                                    case THEN:          /* 说明之前是then关键字   */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->child[1] = Temp;
                                        Head = Temp;
                                        break;

                                    case ELSE:          /* 说明之前是else关键字   */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->child[2] = Temp;
                                        Head = Temp;
                                        break;

                                    case REPEAT:        /* 说明之前是repeat关键字 */
                                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                                        Temp->child[0] = NULL;
                                        Temp->child[1] = NULL;
                                        Temp->child[2] = NULL;
                                        Temp->sibling = NULL;
                                        Temp->father = NULL;

                                        Temp->sibling = NULL;
                                        Temp->father = Head;
                                        Temp->lineno = lineNo;
                                        Temp->attr.op = i;
                                        Modekey = i;

                                        Head->child[0] = Temp;
                                        Head = Temp;
                                        break;
                                }
                            }
                            break;
                    }

                    break;
                }
            }

            /*printf("\n% 9d  % 9d   % 9s", i, Modekey, chuli);
            getch();   */

            /* 说明没有找到匹配关键字，说明是临时变量或常量 */
            if (i == 19)
            {
                /* 消除左括号标记 */
                ZKHKey = 0;

                /* 说明之前，已有个变量或常量 */
                if (strCLKey == 1)
                {
                    lineNoError = lineNo;
                    fclose(fp2);
                    return 0;       /* 语法错误               */
                }
                else
                {
                    /* 判断字符是否为合法变量或常量 */
                    for (i = 0; i < lenstr-10; i++)
                        if (chuli[i] >= '0' && chuli[i] <= '9') break;

                    /* 说明是对的 */
                    if (i == 0 || i == (lenstr-10))
                    {
                        for (i = 0; i < strCLJLCount; i++)
                            if (!strcmp(chuli, strCLJL[i])) break;

                        /* 说明原始记录没有记录此字符串 */
                        if (i == strCLJLCount)
                        {
                            strcpy(strCLJL[strCLJLCount], chuli);
                            strCLJLCount++;
                        }
                    }
                    else
                    {

                        /* 说明是个数字或是个不正确的变量命名 */
                        lineNoError = lineNo;
                        fclose(fp2);
                        return 0;       /* 语法错误               */
                    }

                    strcpy(strCL, chuli);
                    strCLKey = 1;
                }

                switch (Modekey)
                {
                    case READ:          /* 说明此变量或常量之前为read语句 */
                        for (i = 0; i < lenstr-10; i++)
                            if (chuli[i] >= '0' && chuli[i] <= '9') break;
                        
                        if (i == (lenstr-10))   /* 说明是个变量 */
                        {
                            /* 记录read将读入的变量名 */
                            strcpy(Head->attr.name, chuli);
                        }
                        else
                        {
                            /* 说明是个数字或是个不正确的变量命名 */
                            lineNoError = lineNo;
                            fclose(fp2);
                            return 0;       /* 语法错误               */
                        }
                        break;


                    case UNTIL:
                    case IF:
                    case WRITE:
                    case XIAOYU:
                    case DAYU:
                    case DENGYU:
                    case CHU:
                    case CHENG:
                    case JIAN:
                    case JIA:
                    case FUZHI:       /* 说明此变量或常量之前为:=语句 */
                        Temp = (TreeNode *)malloc(sizeof(TreeNode));
                        Temp->child[0] = NULL;
                        Temp->child[1] = NULL;
                        Temp->child[2] = NULL;
                        Temp->sibling = NULL;
                        Temp->father = NULL;

                        Temp->sibling = NULL;
                        Temp->father = Head;
                        Temp->lineno = lineNo;

                        for (i = 0; i < lenstr-10; i++)
                            if (chuli[i] >= '0' && chuli[i] <= '9') break;
                        if (i == (lenstr-10))   /* 说明是个变量 */
                        {
                            Temp->attr.op = BLIANG;
                        }
                        else
                        {
                            Temp->attr.op = CLIANG;
                        }
                        strcpy(Temp->attr.name, chuli);


                        if (Modekey == IF)
                            Head->child[0] = Temp;
                        else
                        {
                            if (Modekey == UNTIL)
                            {
                                Head->child[1] = Temp;
                            }
                            else
                                Head->child[2] = Temp;
                        }


                        Head = Temp;
                        break;
                }
            }


            /*printf("%06d %s    %s", lineNo, chuli, buffer);*/
        }


        if (feof(fp2))
        {
            if (qxzy != 0)
            {
                lineNoError = lineNo;
                fclose(fp2);
                return 0;       /* 语法错误               */
            }


            if (strCLKey == 1)
            {
                lineNoError = lineNo;
                fclose(fp2);
                return 0;       /* 语法错误               */
            }

            switch (Modekey)
            {
                case XIAOYU:
                case DAYU:
                case DENGYU:
                case CHU:
                case CHENG:
                case JIAN:
                case JIA:
                case FUZHI:
                case WRITE:
                case READ:
                case UNTIL:
                case REPEAT:
                case ELSE:
                case THEN:
                case IF:
                    lineNoError = lineNo;
                    fclose(fp2);
                    return 0;       /* 语法错误               */
            }


            /* 连接结束节点 */
            Temp = (TreeNode *)malloc(sizeof(TreeNode));
            Temp->child[0] = NULL;
            Temp->child[1] = NULL;
            Temp->child[2] = NULL;
            Temp->sibling = NULL;
            Temp->father = Head;
            Temp->attr.op = LAST;
            Head->sibling = Temp;
            break;
        }
    }
    fclose(fp2);




    /* 打印语法树 */
    fp2 = fopen("语法树.txt", "w+");
    Head = Biao;
    lenstr = 0;
    while (Head->attr.op != LAST)
    {
        buffer[0] = '\0';
        for (i = 0; i < lenstr; i++) strcat(buffer, "    ");

        switch (Head->attr.op)
        {
            case READ:
                fprintf(fp2, "%sread %s;\n", buffer, Head->attr.name);


                if (Head->sibling == NULL)
                {
                    while (1)
                    {
                        Head = Head->father;


                        if (Head->attr.op == REPEAT)
                        {
                            Head->attr.libian++;
                            if (Head->attr.libian == 1)
                            {
                                if (lenstr > 0)
                                {
                                    lenstr--;
                                    buffer[0] = '\0';
                                    for (i = 0; i < lenstr; i++) strcat(buffer, "    ");
                                }


                                fprintf(fp2, "%suntil ", buffer);
                                Head = Head->child[1];
                                break;
                            }

                            if (Head->attr.libian == 2)
                            {
                                fprintf(fp2, ";\n");
                                Head = Head->sibling;
                                break;
                            }
                        }


                        if (Head->attr.op == IF)
                        {
                            Head->attr.libian++;

                            if (Head->attr.libian == 1)
                            {
                                fprintf(fp2, "then\n");
                                lenstr++;
                                Head = Head->child[1];
                                break;
                            }


                            if (lenstr > 0)
                            {
                                lenstr--;
                                buffer[0] = '\0';
                                for (i = 0; i < lenstr; i++) strcat(buffer, "    ");
                            }

                            if (Head->attr.libian == 2)
                            {
                                if (Head->child[2] == NULL)
                                {
                                    fprintf(fp2, "%send\n", buffer);
                                    Head->attr.libian = 3;
                                    Head = Head->sibling;
                                    break;
                                }
                                else
                                {
                                    fprintf(fp2, "%selse\n", buffer);
                                    Head = Head->child[2];
                                    lenstr++;
                                    break;       
                                }
                            }

                            if (Head->attr.libian == 3)
                            {
                                fprintf(fp2, "%send\n", buffer);
                                Head = Head->sibling;
                                break;
                            }
                        }
                    }
                }
                else
                    Head = Head->sibling;

                break;

            case WRITE:
                fprintf(fp2, "%swrite ", buffer);
                /*Head->attr.libian = 1;*/

                Head = Head->child[2];
                break;

            case FUZHI:
                fprintf(fp2, "%s%s := ", buffer, Head->attr.name);

                /*Head->attr.libian = 1;*/
                Head = Head->child[2];
                break;


            case IF:
                if (Head->attr.libian == 0)
                {
                    fprintf(fp2, "%sif ", buffer);
                    Head = Head->child[0];
                }
                break;


            case REPEAT:
                lenstr++;
                fprintf(fp2, "%srepeat\n", buffer);
                Head = Head->child[0];
                break;


            case XIAOYU:
            case DAYU:
            case DENGYU:
            case CHU:
            case CHENG:
            case JIAN:
            case JIA:
                fprintf(fp2, "%s ", KeyGJZ[Head->attr.op]);
                Head = Head->child[2];
                break;

            case CLIANG:
            case BLIANG:
                if (Head->child[2] == NULL)
                {
                    fprintf(fp2, "%s ", Head->attr.name);
                    while (1)
                    {
ZAI:
                        Head = Head->father;
                        if ((Head->attr.op == WRITE || Head->attr.op == FUZHI) && Head->attr.libian == 0)
                        {
                            Head->attr.libian = 1;
                            fprintf(fp2, ";\n");
                            if (Head->sibling == NULL)
                            {
                                goto ZAI;
                            }
                            else
                            {
                                Head = Head->sibling;
                            }
                            break;
                        }


                        if (Head->attr.op == REPEAT)
                        {

                            Head->attr.libian++;
                            if (Head->attr.libian == 1)
                            {
                                if (lenstr > 0)
                                {
                                    lenstr--;
                                    buffer[0] = '\0';
                                    for (i = 0; i < lenstr; i++) strcat(buffer, "    ");
                                }


                                fprintf(fp2, "%suntil ", buffer);
                                Head = Head->child[1];
                                break;
                            }

                            if (Head->attr.libian == 2)
                            {
                                fprintf(fp2, ";\n");
                                if (Head->sibling == NULL)
                                {
                                    goto ZAI;
                                }
                                else
                                    Head = Head->sibling;
                                break;
                            }
                        }


                        if (Head->attr.op == IF)
                        {
                            Head->attr.libian++;

                           /* printf("       %d   %d", Head->attr.libian, Head->child[1]->child[2]->attr.op); */

                            if (Head->attr.libian == 1)
                            {
                                lenstr++;
                                fprintf(fp2, "then\n");
                                Head = Head->child[1];
                                break;
                            }




                            if (lenstr > 0)
                            {
                                lenstr--;
                                buffer[0] = '\0';
                                for (i = 0; i < lenstr; i++) strcat(buffer, "    ");
                            }


                            if (Head->attr.libian == 2)
                            {


                                if (Head->child[2] == NULL)
                                {
                                    fprintf(fp2, "%send\n", buffer);
                                    Head->attr.libian = 3;


                                    if (Head->sibling == NULL)
                                    {
                                        goto ZAI;
                                    }
                                    else
                                    {
                                        Head = Head->sibling;
                                    }
                                    break;
                                }
                                else
                                {
                                    fprintf(fp2, "%selse\n", buffer);
                                    Head = Head->child[2];
                                    lenstr++;
                                    break;       
                                }
                            }

                            if (Head->attr.libian == 3)
                            {

                                fprintf(fp2, "%send\n", buffer);
                                if (Head->sibling == NULL)
                                {
                                    goto ZAI;
                                }
                                else
                                    Head = Head->sibling;
                                break;
                            }
                        }
                     }
                }
                else
                {
                    fprintf(fp2, "%s ", Head->attr.name);
                    Head = Head->child[2];    
                }
                break;
        }
    }
    fclose(fp2);




    /* 生成汇编代码 */
    fp2 = fopen("汇编代码.txt", "w+");

    lineNo = 0;
    /*fprintf(fp2, "% 3d: % 6s  6,0(7)\n", lineNo++, "LDC");*/

    /* 初始化MTMY虚拟机运行环境 */
    for (i = 0; i < strCLJLCount; i++) /* 将变量和常量写入内存 */
    {
        if (strCLJL[i][0] >= '0' && strCLJL[i][0] <= '9')
        {
            fprintf(fp2, "% 3d: % 6s  0,%d(7)\n", lineNo++, "LDC", atoi(strCLJL[i]));
            fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i); 
            /*Mem[i] = atoi(strCLJL[i]);*/
        }
    }


    Head = Biao;
    while (Head->attr.op != LAST)
    {
        switch (Head->attr.op)
        {
            case READ:
                fprintf(fp2, "% 3d: % 6s  0,0,0\n", lineNo++, "IN");
                /*fprintf(fp2, "% 3d: % 6s  6,0(7)\n", lineNo++, "LDC");*/

                for (i = 0; i < strCLJLCount; i++)
                {
                    if (!strcmp(strCLJL[i], Head->attr.name))
                    {
                        break;
                    }
                }
                fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i);


                if (Head->sibling == NULL)
                {
                    while (1)
                    {
                        Head = Head->father;


                        if (Head->attr.op == REPEAT)
                        {

                            Head->attr.csdm++;
                            if (Head->attr.csdm == 1)
                            {
                                Temp = Head->child[1];
                                while (Temp->child[2] != NULL)
                                {
                                    if (Temp->child[2]->attr.op == DENGYU
                                        || Temp->child[2]->attr.op == DAYU
                                            || Temp->child[2]->attr.op == XIAOYU)
                                    {
                                        break;
                                    }
                                    Temp = Temp->child[2];
                                }
                                Last = Temp;
                                Temp = Head->child[1];
                                MakeTest(Head->child[1]);
                                fprintf(fp2, "% 3d: % 6s  1,0(0)\n", lineNo++, "LDA");


                                Head2 = Last->child[2]->child[2];
                                while (Head2->child[2] != NULL)
                                {
                                    Head2 = Head2->child[2];
                                }
                                Temp = Last->child[2]->child[2];
                                Last = Head2;

                                MakeTest(Temp);
                                fprintf(fp2, "% 3d: % 6s  2,0(0)\n", lineNo++, "LDA");



                                fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "SUB");
                                Temp = Head->child[1];
                                while (Temp->child[2] != NULL)
                                {
                                    if (Temp->attr.op == DENGYU
                                        || Temp->attr.op == DAYU
                                            || Temp->attr.op == XIAOYU)
                                    {
                                        break;
                                    }
                                    Temp = Temp->child[2];
                                }
                                switch (Temp->attr.op)
                                {
                                    case DENGYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "JNE", Head->lineno); break;
                                    case DAYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "JLE", Head->lineno); break;
                                    case XIAOYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "JGE", Head->lineno); break;
                                }

                                Head = Head->child[1];
                                while (Head->child[2] != NULL) Head = Head->child[2];
                                break;
                            }

                            if (Head->attr.csdm == 2)
                            {
                                Head = Head->sibling;
                                break;
                            }
                        }



                        if (Head->attr.op == IF)
                        {
                            Head->attr.csdm++;

                            if (Head->attr.csdm == 1)
                            {
                                Head = Head->child[1];
                                break;
                            }

                            if (Head->attr.csdm == 2)
                            {
                                if (Head->child[2] == NULL)
                                {
                                    Head->attr.csdm = 3;

                                    /* 倒悬 else */
                                    Temp = Head->child[0];
                                    while (Temp != NULL)
                                    {
                                        if (Temp->attr.op == DENGYU
                                            || Temp->attr.op == DAYU
                                                || Temp->attr.op == XIAOYU)
                                        {
                                            break;
                                        }
                                        Temp = Temp->child[2];
                                    }

                                    fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno, "LD", strCLJLCount+Head->bcliang);
                                    switch (Temp->attr.op)
                                    {
                                        case DENGYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JNE", lineNo); break;
                                        case DAYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JLE", lineNo); break;
                                        case XIAOYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JGE", lineNo); break;
                                    }


                                    Head = Head->sibling;
                                    break;
                                }
                                else
                                {
                                    /* 倒悬 else */
                                    Temp = Head->child[0];
                                    while (Temp != NULL)
                                    {
                                        if (Temp->attr.op == DENGYU
                                            || Temp->attr.op == DAYU
                                                || Temp->attr.op == XIAOYU)
                                        {
                                            break;
                                        }
                                        Temp = Temp->child[2];
                                    }

                                    fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno, "LD", strCLJLCount+Head->bcliang);
                                    switch (Temp->attr.op)
                                    {
                                        case DENGYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JNE", lineNo+1); break;
                                        case DAYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JLE", lineNo+1); break;
                                        case XIAOYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JGE", lineNo+1); break;
                                    }

                                    /* 跳到end 后的表识 */
                                    Head->lineno = lineNo++;
                                    Head = Head->child[2];
                                    break;       
                                }
                            }

                            if (Head->attr.csdm == 3)
                            {
                                /* 跳到end 后的表识 */
                                fprintf(fp2, "% 3d: % 6s  6,%d(6)\n", Head->lineno, "JEQ", lineNo);
                                Head = Head->sibling;
                                break;
                            }
                        }
                    }
                }
                else
                    Head = Head->sibling;
                break;

            case REPEAT:
                Head->lineno = lineNo;
                Head = Head->child[0];
                break;

            case IF:
                Temp = Head->child[0];
                while (Temp->child[2] != NULL)
                {
                    if (Temp->child[2]->attr.op == DENGYU
                        || Temp->child[2]->attr.op == DAYU
                            || Temp->child[2]->attr.op == XIAOYU)
                    {
                        break;
                    }
                    Temp = Temp->child[2];
                }
                Last = Temp;
                Temp = Head->child[0];
                MakeTest(Head->child[0]);
                fprintf(fp2, "% 3d: % 6s  1,0(0)\n", lineNo++, "LDA");


                Head2 = Last->child[2]->child[2];
                while (Head2->child[2] != NULL)
                {
                    Head2 = Head2->child[2];
                }
                Temp = Last->child[2]->child[2];
                Last = Head2;

                MakeTest(Temp);
                fprintf(fp2, "% 3d: % 6s  2,0(0)\n", lineNo++, "LDA");


                fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "SUB");
                fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", strCLJLCount+Head->bcliang);


                /* 记录 if 跳转语句应该写在第几行 */
                Head->lineno = lineNo;
                lineNo += 2;

                Head = Last;
                break;


            case FUZHI:
            case WRITE:
                Temp = Head->child[2];
                while (Temp->child[2] != NULL)
                {
                    Temp = Temp->child[2];
                }
                Last = Temp;
                Temp = Head->child[2];

                if (MakeTest(Head->child[2]) == 1)
                {
                    switch (Head->attr.op)
                    {
                        case WRITE:
                            fprintf(fp2, "% 3d: % 6s  0,0,0\n", lineNo++, "OUT");
                            break;
                        
                        case FUZHI:
                            for (i = 0; i < strCLJLCount; i++)
                                if (!strcmp(strCLJL[i], Head->attr.name)) break;

                            fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i);   
                            break;
                    }
                }
                else
                {
                    switch (Head->attr.op)
                    {
                        case WRITE:
                            fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "LD", strCLJLCount);
                            fprintf(fp2, "% 3d: % 6s  0,0,0\n", lineNo++, "OUT");
                            break;
                            
                        case FUZHI:
                            for (i = 0; i < strCLJLCount; i++)
                                if (!strcmp(strCLJL[i], Head->attr.name)) break;

                            fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "ST", i);
                            break;
                    }   
                }
                while (Head->child[2] != NULL) Head = Head->child[2];

                break;



            case CLIANG:
            case BLIANG:
                if (Head->child[2] == NULL)
                {
                    while (1)
                    {
ZAI2:
                        Head = Head->father;
                        if ((Head->attr.op == WRITE || Head->attr.op == FUZHI) && Head->attr.csdm == 0)
                        {
                            Head->attr.csdm = 1;
                            if (Head->sibling == NULL)
                            {
                                goto ZAI2;
                            }
                            else
                            {
                                Head = Head->sibling;
                            }
                            break;
                        }


                        if (Head->attr.op == REPEAT)
                        {

                            Head->attr.csdm++;
                            if (Head->attr.csdm == 1)
                            {
                                Temp = Head->child[1];
                                while (Temp->child[2] != NULL)
                                {
                                    if (Temp->child[2]->attr.op == DENGYU
                                        || Temp->child[2]->attr.op == DAYU
                                            || Temp->child[2]->attr.op == XIAOYU)
                                    {
                                        break;
                                    }
                                    Temp = Temp->child[2];
                                }
                                Last = Temp;
                                Temp = Head->child[1];
                                MakeTest(Head->child[1]);
                                fprintf(fp2, "% 3d: % 6s  1,0(0)\n", lineNo++, "LDA");


                                Head2 = Last->child[2]->child[2];
                                while (Head2->child[2] != NULL)
                                {
                                    Head2 = Head2->child[2];
                                }
                                Temp = Last->child[2]->child[2];
                                Last = Head2;

                                MakeTest(Temp);
                                fprintf(fp2, "% 3d: % 6s  2,0(0)\n", lineNo++, "LDA");



                                fprintf(fp2, "% 3d: % 6s  0,1,2\n", lineNo++, "SUB");
                                Temp = Head->child[1];
                                while (Temp->child[2] != NULL)
                                {
                                    if (Temp->attr.op == DENGYU
                                        || Temp->attr.op == DAYU
                                            || Temp->attr.op == XIAOYU)
                                    {
                                        break;
                                    }
                                    Temp = Temp->child[2];
                                }
                                switch (Temp->attr.op)
                                {
                                    case DENGYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "JNE", Head->lineno); break;
                                    case DAYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "JLE", Head->lineno); break;
                                    case XIAOYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", lineNo++, "JGE", Head->lineno); break;
                                }

                                Head = Head->child[1];
                                while (Head->child[2] != NULL) Head = Head->child[2];
                                break;
                            }

                            if (Head->attr.csdm == 2)
                            {
                                if (Head->sibling == NULL)
                                {
                                    goto ZAI2;
                                }
                                else
                                    Head = Head->sibling;
                                break;
                            }
                        }


                        if (Head->attr.op == IF)
                        {
                            Head->attr.csdm++;

                            if (Head->attr.csdm == 1)
                            {
                                Head = Head->child[1];
                                break;
                            }

                            if (Head->attr.csdm == 2)
                            {
                                if (Head->child[2] == NULL)
                                {
                                    Head->attr.csdm = 3;

                                    /* 倒悬 else */
                                    Temp = Head->child[0];
                                    while (Temp != NULL)
                                    {
                                        if (Temp->attr.op == DENGYU
                                            || Temp->attr.op == DAYU
                                                || Temp->attr.op == XIAOYU)
                                        {
                                            break;
                                        }
                                        Temp = Temp->child[2];
                                    }

                                    fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno, "LD", strCLJLCount+Head->bcliang);
                                    switch (Temp->attr.op)
                                    {
                                        case DENGYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JNE", lineNo); break;
                                        case DAYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JLE", lineNo); break;
                                        case XIAOYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JGE", lineNo); break;
                                    }


                                    if (Head->sibling == NULL)
                                    {
                                        goto ZAI2;
                                    }
                                    else
                                    {
                                        Head = Head->sibling;
                                    }
                                    break;
                                }
                                else
                                {
                                    /* 倒悬 else */
                                    Temp = Head->child[0];
                                    while (Temp != NULL)
                                    {
                                        if (Temp->attr.op == DENGYU
                                            || Temp->attr.op == DAYU
                                                || Temp->attr.op == XIAOYU)
                                        {
                                            break;
                                        }
                                        Temp = Temp->child[2];
                                    }

                                    fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno, "LD", strCLJLCount+Head->bcliang);
                                    switch (Temp->attr.op)
                                    {
                                        case DENGYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JNE", lineNo+1); break;
                                        case DAYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JLE", lineNo+1); break;
                                        case XIAOYU: fprintf(fp2, "% 3d: % 6s  0,%d(6)\n", Head->lineno+1, "JGE", lineNo+1); break;
                                    }

                                    /* 跳到end 后的表识 */
                                    Head->lineno = lineNo++;
                                    Head = Head->child[2];
                                    break;       
                                }
                            }

                            if (Head->attr.csdm == 3)
                            {
                                /* 跳到end 后的表识 */
                                fprintf(fp2, "% 3d: % 6s  6,%d(6)\n", Head->lineno, "JEQ", lineNo);


                                if (Head->sibling == NULL)
                                {
                                    goto ZAI2;
                                }
                                else
                                    Head = Head->sibling;
                                break;
                            }
                        }
                     }
                }
                else
                {
                    Head = Head->child[2];    
                }
                break;
        }
    }

    fprintf(fp2, "% 3d: % 6s  0,0,0\n", lineNo++, "HALT");

    fclose(fp2);
}



void main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("usage: %s <filename>\n", argv[1]);
        exit(1);
    }

    fp = fopen(argv[1], "a");
    if (fp == NULL)
    {
        printf("file '%s' not found\n", argv[1]);
        exit(1);
    }
    fseek(fp, SEEK_END, 0);
    fprintf(fp, "\n");
    fclose(fp);

    fp = fopen(argv[1], "r");


    GetInfLine();
    fclose(fp);


    if (lineNoError)
    {
        printf("\n\nerror: line %d   %d", lineNoError, Biao->attr.op);
    }
    else
    {
        printf("\nOK  MEM-BC %d     MEM-IF %d", strCLJLCount, BCLiangCount);
    }


    getch();
}
