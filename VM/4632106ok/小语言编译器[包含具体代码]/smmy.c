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
    struct treeNode * child[MAXCHILDREN];  /* ���3��Ҷ��                  */
    struct treeNode * sibling;             /* ��һ���ӽṹ                 */
    struct treeNode * father;              /* ��һ�����ṹ                 */

    int lineno;                            /* ��������ı���Դ�����е��к� */
    int bcliang;                           /* ��Ϊ��/��������¼�˱�/�������ڴ��е�λ�� */

    struct treeNode2                       /* ���Խṹ                     */
    {
        TokenType   op;                    /* ��Ϊ�ؼ��֣����Ϊ���ֹؼ��� */
        int         qx;                    /* ��Ϊ����������Ȩ��ֵ��ʵ���Ⱥ���� */
        char        libian;                /* if, repeat�������Ҫ��������ʱ�ı��, ���ڴ�ӡ�﷨�� */
        char        csdm;                  /* if, repeat�������Ҫ��������ʱ�ı��, ����ת��Ϊ������ */
        char        name[20];              /* ��Ϊ��������¼��������       */
    } attr;
}TreeNode;


char *KeyGJZ[] = {"if", "then", "else", "end", "repeat", "until", "read", "write",
                 ":=", ";", "+", "-", "*", "/", "=", ">", "<", "(", ")"};

int     BCLiangCount = 0;                   /* ��¼Ŀǰ�ж��ٱ���           */
int     strCLJLCount = 0;                   /* ��¼Ŀǰ�ж��ٱ���������     */
char    strCLJL[1000][20];                  /* ���ڼ�¼��ǰ���Ա���������   */


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


        /* ������������ */
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


        /* ˵���ҵ�Ȩֵ���������ڵ� */
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


            /* ����ƹ�ֵ */
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

            /* ��ǰ�ƹ�ֵ */
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
    char    Modekey = -1;       /* ��ǵ�ǰӦ�������������ַ�   */
    char    ZSkey = 0;          /* ����Ƿ�ע�Ϳ�ʼ             */
    char    ModeGJZ;            /* �����ؼ��֣�����ͺ�         */

    char    strCL[20];
    char    strCLKey = 0;       /* ���֮ǰ�Ƿ��¼Ϊһ����������� */
    char    ZKHKey = 0;         /* ��������� */


    fp2 = fopen("the1.txt", "w+");


    /* ��һ��ɨ�裬���ؼ��֣����������������ʽ���г��� */
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


                if (str == '}') return 0;     /* ˵��{��ǰ���֣�û��{}���    */
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


                    if (str == ':')           /* ˵��һ����ֵ�ſ�ʼ           */
                    {
                        buffer[i-1] = '\0';
                        fprintf(fp2, "%06d:  %s\n", lineNo, buffer);
                        i = 0;

                        str = fgetc(fp);
                        if (str == '=')       /* ˵����ֵ����ȷ               */
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
                if (str == '{') return 0;     /* ˵��������2��{��û��{}���   */
                if (str == '}') ZSkey = 0;    /* ˵��ע�ͽ���                 */
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




    /* �����﷨�� */
    fp2 = fopen("the1.txt", "r");
    while (1)
    {
        fgets(buffer, LINE_SIZE, fp2);

        lenstr = strlen(buffer);
        if (lenstr > 10)    /* ȥ��������Ϊ�յ��� */
        {
            memcpy(chuli, buffer, 6);
            chuli[6] = '\0';
            lineNo = atoi(chuli);

            memcpy(chuli, &buffer[9], lenstr-10);
            chuli[lenstr-10] = '\0';

            
            for (i = 0; i < 19; i++)
            {
                /* ˵���Ǹ��ؼ��� */
                if (!strcmp(chuli, KeyGJZ[i]))
                {
                    switch (i)
                    {
                        case ZKUOHAO:           /* ˵����ǰ�ؼ���Ϊ(        */
                            ZKHKey = 1;
                            qxzy += 4;
                            break;

                        case YKUOHAO:           /* ˵����ǰ�ؼ���Ϊ)        */
                            if (strCLKey == 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }

                            qxzy -= 4;
                            break;

                        case XIAOYU:            /* ˵����ǰ�ؼ���Ϊ<        */
                        case DAYU:              /* ˵����ǰ�ؼ���Ϊ>        */
                        case DENGYU:            /* ˵����ǰ�ؼ���Ϊ=        */
                            if (ZKHKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */   
                            }

                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
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
                                    return 0;       /* �﷨����               */  


                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case UNTIL:
                                case IF:
                                    /* ˵��+֮ǰû��һ������ */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */
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


                        case CHU:               /* ˵����ǰ�ؼ���Ϊ/        */
                        case CHENG:             /* ˵����ǰ�ؼ���Ϊ*        */
                        case JIAN:              /* ˵����ǰ�ؼ���Ϊ-        */
                        case JIA:               /* ˵����ǰ�ؼ���Ϊ+        */
                            if (ZKHKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */   
                            }


                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }

                            switch (Modekey)
                            {
                                case READ:
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */

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
                                    /* ˵��+֮ǰû��һ������ */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */
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

                        case FUZHI:             /* ˵����ǰ�ؼ���Ϊ:=       */
                            if (ZKHKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */   
                            }


                            switch (Modekey)
                            {
                                case WRITE:     /* ˵��֮ǰ��write�ؼ���    */
                                case UNTIL:     /* ˵��֮ǰ��until�ؼ���    */
                                case XIAOYU:
                                case DAYU:
                                case DENGYU:
                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case FUZHI:
                                case READ:
                                case IF:        /* ˵��֮ǰҲ��if�ؼ���   */
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */

                                case REPEAT:    /* ˵��֮ǰ��repeat�ؼ���    */
                                case END:       /* ˵��֮ǰ��end�ؼ���    */
                                case ELSE:      /* ˵��֮ǰ��else�ؼ���   */
                                case THEN:      /* ˵��֮ǰ��then�ؼ���   */
                                case FENGHAO:
                                case -1:
                                    /* ˵��:=֮ǰû��һ������ */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */
                                    }
                                    else
                                    {
                                        /* �ж��Ƿ�Ϊһ���� */
                                        int strl = strlen(strCL);

                                        for (j = 0; j < strl; j++)
                                            if (strCL[j] >= '0' && strCL[j] <= '9') break;

                                        /* Ϊ�Ϸ����� */
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
                                                /* ����:=�ؼ��ֽڵ� */
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
                                            return 0;       /* �﷨����               */
                                        }
                                    }
                                    break;
                            }
                            break;


                        case UNTIL:             /* ˵����ǰ�ؼ���Ϊuntil        */
                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            } 
                            else
                            {
                                /* �����Ų��Գ� */
                                if (qxzy != 0)
                                {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */
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
                                        return 0;       /* �﷨����               */

                                    
                                    case FENGHAO:
                                    case END:
                                        /* ������repeat�ڵ� */
                                        Temp = NULL;
                                        while (!(Head->attr.op == REPEAT && Temp == Head->child[0]))
                                        {
                                            /* ˵���Ѿ��������� */
                                            if (Head->father == NULL)
                                            {
                                                lineNoError = lineNo;
                                                fclose(fp2);
                                                return 0;       /* �﷨����              */    
                                            }
                                            Temp = Head;
                                            Head = Head->father;
                                        }

                                        /* ˵���б������������0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                        break;
                                }
                            }
                            break;

                        case FENGHAO:           /* ˵����ǰ�ؼ���Ϊ;        */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }


                            switch (Modekey)
                            {
                                case FENGHAO:
                                case UNTIL:         /* ˵��֮ǰ��until�ؼ���  */
                                case REPEAT:        /* ˵��֮ǰ��repeat�ؼ��� */
                                case END:           /* ˵��֮ǰ��end�ؼ���    */
                                case ELSE:          /* ˵��֮ǰ��else�ؼ���   */
                                case THEN:          /* ˵��֮ǰ��then�ؼ���   */
                                case IF:            /* ˵��֮ǰҲ��if�ؼ���   */
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */

                                case XIAOYU:
                                case DAYU:
                                case DENGYU:
                                case CHU:
                                case CHENG:
                                case JIAN:
                                case JIA:
                                case FUZHI:
                                case WRITE:
                                case READ:          /* ˵��֮ǰҲ�Ǹ�read��� */
                                    /* ˵��read֮��û�б��� */
                                    if (strCLKey == 0)
                                    {
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */    
                                    }
                                    else
                                    {
                                        /* ����Ѱ�ұ��ʽ�ڵ� */
                                        if (Modekey != READ)
                                        {
                                            while (Head->attr.op != FUZHI && Head->attr.op != IF && Head->attr.op != REPEAT
                                                    && Head->attr.op != READ && Head->attr.op != WRITE)
                                            {
                                                if (Head->father == NULL)
                                                {
                                                    lineNoError = lineNo;
                                                    fclose(fp2);
                                                    return 0;       /* �﷨����               */        
                                                }

                                                Head = Head->father;
                                            }
                                        }


                                        /* ˵���б������������0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                    }
                                    break;
                            }
                            break;

                        case REPEAT:            /* ˵����ǰ�ؼ���Ϊrepeat        */
                        case IF:                /* ˵����ǰ�ؼ���Ϊif       */
                            /* �����Ų��Գ� */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }


                            if (strCLKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
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
                                    case UNTIL:         /* ˵��֮ǰ��until�ؼ���  */
                                    case WRITE:         /* ˵��֮ǰ��write�ؼ���  */
                                    case READ:          /* ˵��֮ǰҲ�Ǹ�read��� */
                                    case XIAOYU:        /* ˵��֮ǰ��<�ؼ���      */
                                    case DAYU:          /* ˵��֮ǰ��>�ؼ���      */
                                    case DENGYU:        /* ˵��֮ǰ��=�ؼ���      */
                                    case CHU:           /* ˵��֮ǰ��/�ؼ���      */
                                    case CHENG:         /* ˵��֮ǰ��*�ؼ���      */
                                    case JIAN:          /* ˵��֮ǰ��-�ؼ���      */
                                    case JIA:           /* ˵��֮ǰ��+�ؼ���      */
                                    case FUZHI:         /* ˵��֮ǰ��:=�ؼ���     */
                                    case IF:            /* ˵��֮ǰҲ��if�ؼ���   */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */

                                    case THEN:          /* ˵��֮ǰ��then�ؼ���   */
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

                                    case ELSE:          /* ˵��֮ǰ��else�ؼ���   */
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
                                    case END:           /* ˵��֮ǰ��end�ؼ���    */
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

                                    case REPEAT:        /* ˵��֮ǰ��repeat�ؼ��� */
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

                        case THEN:              /* ˵����ǰ�ؼ���Ϊthen     */
                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            } 
                            else
                            {
                                /* �����Ų��Գ� */
                                if (qxzy != 0)
                                {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */
                                }



                                /* ��˵��֮ǰû�м�¼һ��������then֮ǰ�����Ǹ��������� */
                                if (strCLKey == 0)
                                {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */
                                }

                                switch (Modekey)
                                {   
                                    /*case DENGYU: ˵��֮ǰ��=�ؼ���      */
                                    case FENGHAO:
                                    case FUZHI: /* ˵��֮ǰ��:=�ؼ���     */
                                    case WRITE: /* ˵��֮ǰ��write�ؼ���  */
                                    case READ:  /* ˵��֮ǰҲ�Ǹ�read��� */
                                    case UNTIL: /* ˵��֮ǰ��until�ؼ���  */
                                    case REPEAT:/* ˵��֮ǰ��repeat�ؼ��� */    
                                    case END:   /* ˵��֮ǰ��end�ؼ���    */
                                    case ELSE:  /* ˵��֮ǰ��else�ؼ���   */
                                    case THEN:  /* ˵��֮ǰ��then�ؼ���   */
                                    case IF:    /* ˵��֮ǰ��if�ؼ���     */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */

                                    case XIAOYU:
                                    case DAYU:
                                    case DENGYU:
                                    case CHU:
                                    case CHENG:
                                    case JIAN:
                                    case JIA:
                                        /* ����Ѱ�� if �ڵ� */
                                        while (Head->attr.op != FUZHI && Head->attr.op != IF)
                                        {
                                            /* ˵���Ѿ��������� */
                                            if (Head->father == NULL)
                                            {
                                                lineNoError = lineNo;
                                                fclose(fp2);
                                                return 0;       /* �﷨����              */    
                                            }
                                            Head = Head->father;
                                        }

                                        if (Head->attr.op == FUZHI)
                                        {
                                            lineNoError = lineNo;
                                            fclose(fp2);
                                            return 0;       /* �﷨����              */
                                        }

                                        /* ˵���б������������0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                        break;
                                }
                            }
                            break;

                        case END:
                        case ELSE:              /* ˵����ǰ�ؼ���Ϊelse     */
                            /* �����Ų��Գ� */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }


                            if (strCLKey == 1)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }


                            if (Head == NULL)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }
                            else
                            {
                                switch (Modekey)
                                {
                                    case UNTIL: /* ˵��֮ǰ��end�ؼ���    */
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
                                    case IF:    /* ˵��֮ǰ��if�ؼ���     */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */

                                    case FENGHAO:
                                    case END:   /* ˵��֮ǰ��end�ؼ���    */
                                        /* �ҵ����Լ���Ӧ��IF�ڵ� */
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

                                        /* ˵���б������������0 */
                                        strCLKey = 0;
                                        Modekey = i;
                                        break;
                                }
                            }
                            break;

                        case WRITE:             /* ˵����ǰ�ؼ���Ϊwrite    */
                        case READ:              /* ˵����ǰ�ؼ���Ϊread     */
                            /* �����Ų��Գ� */
                            if (qxzy != 0)
                            {
                                lineNoError = lineNo;
                                fclose(fp2);
                                return 0;       /* �﷨����               */
                            }


                            if (strCLKey == 1)
                            {
                                    lineNoError = lineNo;
                                    fclose(fp2);
                                    return 0;       /* �﷨����               */
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
                                    case FENGHAO:       /* ˵��֮ǰ��;�ؼ���      */
                                    case END:           /* ˵��֮ǰ��end�ؼ���    */
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

                                    case UNTIL:         /* ˵��֮ǰ��until�ؼ���  */
                                    case WRITE:         /* ˵��֮ǰ��write�ؼ���  */
                                    case READ:          /* ˵��֮ǰҲ�Ǹ�read��� */
                                    case XIAOYU:        /* ˵��֮ǰ��<�ؼ���      */
                                    case DAYU:          /* ˵��֮ǰ��>�ؼ���      */
                                    case DENGYU:        /* ˵��֮ǰ��=�ؼ���      */
                                    case CHU:           /* ˵��֮ǰ��/�ؼ���      */
                                    case CHENG:         /* ˵��֮ǰ��*�ؼ���      */
                                    case JIAN:          /* ˵��֮ǰ��-�ؼ���      */
                                    case JIA:           /* ˵��֮ǰ��+�ؼ���      */
                                    case FUZHI:         /* ˵��֮ǰ��:=�ؼ���     */
                                    case IF:            /* ˵��֮ǰ��if�ؼ���     */
                                        lineNoError = lineNo;
                                        fclose(fp2);
                                        return 0;       /* �﷨����               */

                                    case THEN:          /* ˵��֮ǰ��then�ؼ���   */
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

                                    case ELSE:          /* ˵��֮ǰ��else�ؼ���   */
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

                                    case REPEAT:        /* ˵��֮ǰ��repeat�ؼ��� */
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

            /* ˵��û���ҵ�ƥ��ؼ��֣�˵������ʱ�������� */
            if (i == 19)
            {
                /* ���������ű�� */
                ZKHKey = 0;

                /* ˵��֮ǰ�����и��������� */
                if (strCLKey == 1)
                {
                    lineNoError = lineNo;
                    fclose(fp2);
                    return 0;       /* �﷨����               */
                }
                else
                {
                    /* �ж��ַ��Ƿ�Ϊ�Ϸ��������� */
                    for (i = 0; i < lenstr-10; i++)
                        if (chuli[i] >= '0' && chuli[i] <= '9') break;

                    /* ˵���ǶԵ� */
                    if (i == 0 || i == (lenstr-10))
                    {
                        for (i = 0; i < strCLJLCount; i++)
                            if (!strcmp(chuli, strCLJL[i])) break;

                        /* ˵��ԭʼ��¼û�м�¼���ַ��� */
                        if (i == strCLJLCount)
                        {
                            strcpy(strCLJL[strCLJLCount], chuli);
                            strCLJLCount++;
                        }
                    }
                    else
                    {

                        /* ˵���Ǹ����ֻ��Ǹ�����ȷ�ı������� */
                        lineNoError = lineNo;
                        fclose(fp2);
                        return 0;       /* �﷨����               */
                    }

                    strcpy(strCL, chuli);
                    strCLKey = 1;
                }

                switch (Modekey)
                {
                    case READ:          /* ˵���˱�������֮ǰΪread��� */
                        for (i = 0; i < lenstr-10; i++)
                            if (chuli[i] >= '0' && chuli[i] <= '9') break;
                        
                        if (i == (lenstr-10))   /* ˵���Ǹ����� */
                        {
                            /* ��¼read������ı����� */
                            strcpy(Head->attr.name, chuli);
                        }
                        else
                        {
                            /* ˵���Ǹ����ֻ��Ǹ�����ȷ�ı������� */
                            lineNoError = lineNo;
                            fclose(fp2);
                            return 0;       /* �﷨����               */
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
                    case FUZHI:       /* ˵���˱�������֮ǰΪ:=��� */
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
                        if (i == (lenstr-10))   /* ˵���Ǹ����� */
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
                return 0;       /* �﷨����               */
            }


            if (strCLKey == 1)
            {
                lineNoError = lineNo;
                fclose(fp2);
                return 0;       /* �﷨����               */
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
                    return 0;       /* �﷨����               */
            }


            /* ���ӽ����ڵ� */
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




    /* ��ӡ�﷨�� */
    fp2 = fopen("�﷨��.txt", "w+");
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




    /* ���ɻ����� */
    fp2 = fopen("������.txt", "w+");

    lineNo = 0;
    /*fprintf(fp2, "% 3d: % 6s  6,0(7)\n", lineNo++, "LDC");*/

    /* ��ʼ��MTMY��������л��� */
    for (i = 0; i < strCLJLCount; i++) /* �������ͳ���д���ڴ� */
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

                                    /* ���� else */
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
                                    /* ���� else */
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

                                    /* ����end ��ı�ʶ */
                                    Head->lineno = lineNo++;
                                    Head = Head->child[2];
                                    break;       
                                }
                            }

                            if (Head->attr.csdm == 3)
                            {
                                /* ����end ��ı�ʶ */
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


                /* ��¼ if ��ת���Ӧ��д�ڵڼ��� */
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

                                    /* ���� else */
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
                                    /* ���� else */
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

                                    /* ����end ��ı�ʶ */
                                    Head->lineno = lineNo++;
                                    Head = Head->child[2];
                                    break;       
                                }
                            }

                            if (Head->attr.csdm == 3)
                            {
                                /* ����end ��ı�ʶ */
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
