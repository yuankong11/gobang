#include "head.h"

//linux下编译请加参数-Dsys_linux

int currentUser;//0：黑棋，1：白棋
int mode;//1：人人对战，2：人机对战（玩家执黑），3：人机对战（玩家执白），4：机机对战

int main()
{
    char a[MAXLENGTH];//代码中全部的char a[MAXLENGTH]都是用来储存输入的

    //初始化
    #ifndef sys_linux//in WIN version
    system("color 70");
    #endif
    initial();
    //进行预处理，加快运算速度
    initialPatternTable();
    initialScoreTable();

    //选择模式
    printf("Welcome to gobang game(Work of ZengHongbin).(*^__^*)\n");
    printf("Please choose game mode:\n");
    printf("1: pvp\n2: pve(player go first)\n3: pve(AI go first)\n4: AI vs AI\n");
    while(getLine(a) != 2 || a[0] < '1' || a[0] > '4')//要求输入正确
        printf("Wrong input.\n");
    mode = a[0] - '0';

    re: displayBoard();//展示棋盘

    while(1)
    {
        displayWord();//展示一些文字
        operate();//玩家或者AI进行操作
        displayBoard();
        if((mode == 2 && currentUser == 1) || (mode == 3 && currentUser == 0) || mode == 4)//AI完成时输出其操作
        {
            char x;
            int y;
            translateIntToChar(&x, &y, recordPoint[pIndex].x, recordPoint[pIndex].y);
            printf("%c%d\n", x, y);

            if(mode == 4)//机机对战下暂停
            {
                #ifdef sys_linux
                int l;
                printf("press ENTER to continue or quit to quit.\n");
                while(1)
                {
                    l = getLine(a);
                    if(l == 1 && a[0] == '\n')
                        break;
                    else if(isEqual_string(a, "quit\n"))
                        exit(0);
                    printf("press ENTER to continue or quit to quit.\n");
                }
                #else
                system("pause");
                #endif // sys_linux
            }
        }

        if(pIndex == SIZE*SIZE)//如果棋盘落满
        {
            printf("All places have been placed, the game ended in a draw.\n");
            restart();
            goto re;
        }
        if(isVictorious(currentUser))
        {
            printf("Player%d win!!!\n", currentUser+1);
            restart();
            goto re;
        }

        if(currentUser == 0 && (mode == 1 || mode == 2 ) && pIndex >= 11 && isForbiddenMove(recordPoint[pIndex].x, recordPoint[pIndex].y, currentUser))//判断黑棋是否产生禁手
        {
            printf("Player1, a forbidden move will be made, do you want to change your move or get lost(y/n)?\n");
            setRecordBoard(recordPoint[pIndex].x, recordPoint[pIndex].y, 0);
            --pIndex;
            while(getLine(a) != 2 || (a[0] != 'y' && a[0] != 'n'))//要求输入正确
                printf("Wrong input.\n");
            if(a[0] == 'y')
                goto re;
            else
            {
                printf("Player2 win!!!\n");
                restart();
                goto re;
            }
        }

        changeCurrentUser();
    }
    return 0;
}

void operate()//让玩家或者AI进行操作
{
    if(mode == 1)
        player();
    else if(mode == 2)
    {
        if(currentUser == 0)
            player();
        else
            AI();
    }
    else if(mode == 3)
    {
        if(currentUser == 0)
            AI();
        else
            player();
    }
    else if(mode == 4)
        AI();
}

void changeCurrentUser()
{
    currentUser = !currentUser;
}
