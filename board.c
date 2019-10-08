#include "head.h"

int recordBoard[SIZE+1][SIZE+1];//棋盘记录，0：空，1：黑棋，2：白棋
struct point recordPoint[SIZE*SIZE+1];//下的第i个点的坐标
int pIndex;////用来记录下的点数，从1开始
int patternTable[2][65536];//棋型表，根据key的值查找棋型
int pattern[2][SIZE+1][SIZE+1][5];//棋型记录表，例如pattern[0][1][2][3]的意思是黑棋落在(1, 2)位置时在3方向形成的棋型
int flag[SIZE+1][SIZE+1];//周围以2为半径延伸的5*5矩形中的其他棋子数

const int dx[5] = {0, 1, 0,  1, 1};//方向向量，用于遍历
const int dy[5] = {0, 0, 1, -1, 1};

void initialPatternTable()
{
    int key;
    for(key = 0; key <= 65535; ++key)
    {
        patternTable[0][key] = keyToPattern(0, key);
        patternTable[1][key] = keyToPattern(1, key);
    }
}

void updateFlag(int x, int y, int state)//更新(x, y)周围以2为半径延伸的5*5矩形的flag，不包括(x, y)
{
    int i, j;
    for(i = -2; i <= 2; ++i)
        for(j = -2; j <= 2; ++j)
        {
            if(!i & !j)
                continue;
            if(isInBoard(x+i, y+j))
                flag[x+i][y+j] += state;
        }
}

void updatePattern(int x, int y)//更新(x, y)周围四个方向32个点的棋型，不包括(x, y)
{
    int d, i, j, k, key;

    for(d = 1; d <= 4; ++d)
    {
        for(k = -4; k <= 4; ++k)
        {
            if(k == 0)
                continue;
            i = x + k*dx[d];
            j = y + k*dy[d];
            if(!isInBoard(i, j))
                continue;
            if(!flag[i][j])//如果两格内没有棋子，则不可能形成规定的棋型
                continue;
            key = getKey(i, j, d);
            pattern[0][i][j][d] = patternTable[0][key];
            pattern[1][i][j][d] = patternTable[1][key];
        }
    }
}

int getKey(int x, int y, int d)//获取点在方向d上的key
{
    int key = 0, i, j, k, t, count = 0;
    for(k = -4; k <= 4; ++k)
    {
        if(k == 0)
            continue;
        i = x + k*dx[d];
        j = y + k*dy[d];
        if(isInBoard(i, j))
            t = (recordBoard[i][j] << (2*(count++)));
        else
            t = OUTSIDE << 2*(count++);
        key |= t;
    }
    return key;
}

int keyToPattern(int user, int key)//将key转换成棋型
{
    int line[9];
    int i, j, p, q;
    int empty, l, count;

    //将key转换成数组
    for(i = 0; i <= 3; ++i)
    {
        line[i] = key & 3;//3:0b11，屏蔽高位
        key >>= 2;
    }
    for(i = 5; i <= 8; ++i)//留出4来比较一致
    {
        line[i] = key & 3;
        key >>= 2;
    }
    //连在一起的
    for(i = 1; i <= 4 && line[4-i] == (user+1); ++i)//五连
        if(i == 4)
            return L5;
    --i;
    for(j = 1; j <= 4 && line[4+j] == (user+1); ++j)//五连
        if(j+i == 4)
            return L5;
    --j;
    if(i == 3)//四连
    {
        if(line[0] == EMPTY)
        {
            if(line[5] == EMPTY)
                return L4;
            else
                return D4;
        }
        else if(line[5] == EMPTY)
            return D4;
        else
            return EMPTY;
    }
    if(j+i == 3)//四连
    {
        if(line[3-i] == EMPTY)
        {
            if(line[5+j] == EMPTY)
                return L4;
            else
                return D4;
        }
        else if(line[5+j] == EMPTY)
            return D4;
        else
            return EMPTY;
    }

    //缺1死4
    //count是己方的棋子数，l是以己方棋子为边界的包含空位的棋子数
    for(p = 1, l = count = 1, empty = 0; p <= 4; ++p)
    {
        if(line[4-p] == (user+1))
        {
            ++count;
            l = count + empty;//只有找到了己方棋子才更新l
        }
        else if(line[4-p] == EMPTY)
        {
            ++empty;
        }
        else
            break;
    }
    empty = l - count;//只算中间的空格
    p = l-1;
    for(q = 1; q <= 4; ++q)
    {
        if(line[4+q] == (user+1))
        {
            ++count;
            l = count + empty;
        }
        else if(line[4+q] == EMPTY)
        {
            ++empty;
        }
        else
            break;
    }
    q = l-p-1;

    if(l == 5 && count == 4)
        return D4;

    if(i == 2)//三连
    {
        if(line[1] == EMPTY)
        {
            if(line[5] == EMPTY)
                return L3;
            else
                return D3;
        }
        else if(line[5] == EMPTY)
            return D3;
        else
            return EMPTY;
    }
    if(j+i == 2)//三连
    {
        if(line[3-i] == EMPTY)
        {
            if(line[5+j] == EMPTY)
                return L3;
            else
                return D3;
        }
        else if(line[5+j] == EMPTY)
            return D3;
        else
            return EMPTY;
    }
    //缺1活3死3，之前的结果可以直接用
    if(l == 4 && count == 3)
    {
        if(line[3-p] == EMPTY)
        {
            if(line[5+q] == EMPTY)
                return L3;
            else
                return D3;
        }
        else if(line[5+q] == EMPTY)
            return D3;
        else
            return EMPTY;
    }
    //活2死2
    if(line[3] == (user+1))
    {
        if(line[2] == EMPTY)
        {
            if(line[5] == EMPTY)
                return L2;
            else
                return D2;
        }
        else if(line[5] == EMPTY)
            return D2;
        else
            return EMPTY;
    }
    if(line[5] == (user+1))
    {
        if(line[3] == EMPTY)
        {
            if(line[6] == EMPTY)
                return L2;
            else
                return D2;
        }
        else if(line[6] == EMPTY)
            return D2;
        else
            return EMPTY;
    }

    return EMPTY;
}

int place(int x, int y, int user)//落子，失败返回0
{
    if(recordBoard[x][y] != 0)
    {
        printf("The place is placed. Please choose another one.\n");
        return 0;
    }
    setRecordBoard(x, y, (user == 0)?1:2);
    ++pIndex;
    recordPoint[pIndex].x = x;
    recordPoint[pIndex].y = y;
    updateFlag(x, y, 1);
    updatePattern(x, y);
    return 1;
}

void setRecordBoard(int x, int y, int state)//(0,0)：初始化，(-1, -1)黑白交换
{
    if(x == 0 && y == 0)
    {
        for(x = 1; x <= SIZE; ++x)
            for(y = 1; y <= SIZE; ++y)
                recordBoard[x][y] = 0;
    }
    else if(x == -1 && y == -1)
    {
        for(x = 1; x <= SIZE; ++x)
            for(y = 1; y <= SIZE; ++y)
                recordBoard[x][y] = -recordBoard[x][y];
    }
    else
        recordBoard[x][y] = state;
}

void displayWord()
{
    if(mode == 1)
        printf("Player%d, please input your operation:", (currentUser==0)?1:2);
    else if(mode == 2)
    {
        if(currentUser == 0)
            printf("Player1, please input your operation:");
        else
            printf("AI is thinking.\n");
    }
    else if(mode == 3)
    {
        if(currentUser == 1)
            printf("Player2, please input your operation:");
        else
            printf("AI is thinking.\n");
    }
    else
        printf("AI is thinking.\n");
}

void displayBoard()
{
    int i, j;
    #ifdef sys_linux
    system("clear");
    for(i = 1; i <= SIZE; ++i)
    {
        for(j = 0; j <= SIZE; ++j)
        {
            if(j == 0)
                printf("%2d", 16-i);
            else
            {
                switch(recordBoard[i][j])
                {
                    case 0:
                        if(i == 1)
                        {
                            if(j == 1)
                                printf("┌");
                            else if(j == SIZE)
                                printf("─┐");
                            else
                                printf("─┬");
                        }
                        else if(i == SIZE)
                        {
                            if(j == 1)
                                printf("└");
                            else if(j == SIZE)
                                printf("─┘");
                            else
                                printf("─┴");
                        }
                        else
                        {
                            if(j == 1)
                                printf("├");
                            else if(j == SIZE)
                                printf("─┤");
                            else
                                printf("─┼");
                        }
                        break;
                    case 1:
                        if( (i == recordPoint[pIndex].x && j == recordPoint[pIndex].y) || (pIndex >= 2 && i == recordPoint[pIndex-1].x && j == recordPoint[pIndex-1].y) )//如果是上一回合的落子
                        {
                            if(j != 1)
                                printf("─▲");
                            else
                                printf("▲");
                            break;
                        }
                        else
                        {
                            if(j != 1)
                                printf("─●");
                            else
                                printf("●");
                            break;
                        }
                    case 2:
                        if( (i == recordPoint[pIndex].x && j == recordPoint[pIndex].y) || (pIndex >= 2 && i == recordPoint[pIndex-1].x && j == recordPoint[pIndex-1].y) )
                        {
                            if(j != 1)
                                printf("─△");
                            else
                                printf("△");
                            break;
                        }
                        else
                        {
                            if(j != 1)
                                printf("─◎");
                            else
                                printf("◎");

                            break;
                        }
                }
            }
        }
        printf("\n");
    }
    #else
    system("cls");
    for(i = 1; i <= SIZE; ++i)
    {
        for(j = 0; j <= SIZE; ++j)
        {
            if(j == 0)
                //printf("%2d", i);
                printf("%2d", 16-i);
            else
            {
                switch(recordBoard[i][j])
                {
                    case 0:
                        if(i == 1)
                        {
                            if(j == 1)
                                printf("┌");
                            else if(j == SIZE)
                            {
                                if(recordBoard[i][j-1] == 0)
                                    printf("─┐");
                                else
                                    printf("┐");
                            }
                            else if(recordBoard[i][j-1] == 0)
                                printf("─┬");
                            else
                                printf("┬");
                        }
                        else if(i == SIZE)
                        {
                            if(j == 1)
                                printf("└");
                            else if(j == SIZE)
                            {
                                if(recordBoard[i][j-1] == 0)
                                    printf("─┘");
                                else
                                    printf("┘");
                            }
                            else if(recordBoard[i][j-1] == 0)
                                printf("─┴");
                            else
                                printf("┴");
                        }
                        else
                        {
                            if(j == 1)
                                printf("├");
                            else if(j == SIZE)
                            {
                                if(recordBoard[i][j-1] == 0)
                                    printf("─┤");
                                else
                                    printf("┤");
                            }
                            else if(recordBoard[i][j-1] == 0)
                                printf("─┼");
                            else
                                printf("┼");
                        }
                        break;
                    case 1:
                        if( (i == recordPoint[pIndex].x && j == recordPoint[pIndex].y) || (pIndex >= 2 && i == recordPoint[pIndex-1].x && j == recordPoint[pIndex-1].y) )
                        {
                            if(j != 1 && recordBoard[i][j-1] == 0)
                                printf("─▲");
                            else
                                printf("▲");
                            break;
                        }
                        else
                        {
                            if(j != 1 && recordBoard[i][j-1] == 0)
                                printf("─●");
                            else
                                printf("●");
                            break;
                        }
                    case 2:
                        if( (i == recordPoint[pIndex].x && j == recordPoint[pIndex].y) || (pIndex >= 2 && i == recordPoint[pIndex-1].x && j == recordPoint[pIndex-1].y) )
                        {
                            if(j != 1 && recordBoard[i][j-1] == 0)
                                printf("─△");
                            else
                                printf("△");
                            break;
                        }
                        else
                        {
                            if(j != 1 && recordBoard[i][j-1] == 0)
                                printf("─◎");
                            else
                                printf("◎");
                            break;
                        }
                }
            }
        }
        printf("\n");
    }
    #endif
    //printf("  1 2 3 4 5 6 7 8 9 0 1 2 3 4 5\n");
    printf("  A B C D E F G H I J K L M N O\n");
}

void translateIntToChar(char *ansx, int *ansy, int x, int y)//例如(1,1)->A15
{
    *ansx = 'A' + y - 1;
    *ansy = 16 - x;
}