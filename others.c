#include "head.h"

int getLine(char a[])//get a line inputed or a (MAXLENGTH-1) long string, return '\0''s index
{
	int charInput, i;
	for(i = 0; i < MAXLENGTH-1 && (charInput=getchar())!='\n' && charInput!=EOF; ++i)
        a[i] = charInput;
	if(charInput == '\n')
		a[i++] = '\n';
	a[i] = '\0';
	return (i);
}

int isPieceOfUser(int x, int y, int user)
{
    if(user == 0)
    {
        if(recordBoard[x][y] == 1)
            return 1;
        else
            return 0;
    }
    else
    {
        if(recordBoard[x][y] == 2)
            return 1;
        else
            return 0;
    }
}

void initial()
{
    int i, j, d;
    pIndex = 0;
    currentUser = 0;
    for(i = 1; i <= SIZE; ++i)
        for(j = 1; j <= SIZE; ++j)
        {
            for(d = 1; d <= 4; ++d)
                pattern[0][i][j][d] = pattern[1][i][j][d] = 0;
            flag[i][j] = 0;
        }
    setRecordBoard(0, 0, 0);//初始化棋盘
}

int isVictorious(int user)
{
    if(pIndex >= 9)//至少落9子才可能赢
    {
        if(user == 0)
            return(five(recordPoint[pIndex].x, recordPoint[pIndex].y, user) == 0);
        else
            return(five(recordPoint[pIndex].x, recordPoint[pIndex].y, user) >= 0);//此时长连非禁手
    }
    else
        return 0;
}

void restart()//重新开始
{
    #ifdef sys_linux
    printf("Please input \"restart\" to play a new game or \"quit\" to quit.\n");
    while(!subrestart())
        printf("Please input \"restart\" to play a new game or \"quit\" to quit.\n");
    #else
    initial();
    system("pause");
    #endif // sys_linux
}

int subrestart()//要求输入正确
{
    char a[MAXLENGTH];
    int l;

    if(getLine(a) == MAXLENGTH-1)
    {
        printf("Error: wrong input.\n");
        if(a[MAXLENGTH-1] != '\n')//清空缓冲区
            for(l = getLine(a); a[l-1] != '\n'; l = getLine(a))
                ;
        return 0;
    }
    if(isEqual_string(a, "quit\n"))
    {
        exit(0);
    }
    if(isEqual_string(a, "restart\n"))
    {
        initial();
        return 1;
    }

    return 0;
}

int isInBoard(int x, int y)
{
    if(x < 1 || x > SIZE || y < 1 || y > SIZE)
        return 0;
    else
        return 1;
}

int isValidPiece(int x, int y, int user)//是否在棋盘内以及user的棋子
{
    return(isInBoard(x, y) && isPieceOfUser(x, y, user));
}

int isAvailablePlace(int x, int y, int user)//是否是在棋盘内的、空的、非禁手的
{
    return(isInBoard(x, y) && recordBoard[x][y] == 0 && !isForbiddenMove(x, y, user));
}

int isUsablePlace(int x, int y)//是否是在棋盘内的、非禁手的
{
    return(isInBoard(x, y) && recordBoard[x][y] == 0);
}