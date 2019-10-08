#include "head.h"

int isForbiddenMove(int x, int y, int user)
{
    int t = 0, tmp = 0, d;

    if(user == 1)//白棋无禁手
        return 0;

    for(d = 1; d <= 4; ++d)//双三
        tmp += (pattern[user][x][y][d] == L3);
    if(tmp >= 2)
        return 1;

    tmp = 0;
    for(d = 1; d <= 4; ++d)//双四
        tmp += (pattern[user][x][y][d] == L4 || pattern[user][x][y][d] == D4);
    if(tmp >= 2)
        return 1;

    if(recordBoard[x][y] == 0)
    {
        t = 1;
        setRecordBoard(x, y, 1);
    }
    if(five(x, y, user) > 0)//长连
    {
        if(t)
            setRecordBoard(x, y, 0);
        return 1;
    }
    if(t)
        setRecordBoard(x, y, 0);
    return 0;

    //后面是递归判断复杂禁手，前面只涉及了简单禁手，实战中不太可能出现，又十分消耗时间，可选择性开启
    if(doubleFour(x, y, user))
    {
        if(t)
            setRecordBoard(x, y, 0);
        return 1;
    }
    if(doubleThree(x, y, user))
    {
        if(t)
            setRecordBoard(x, y, 0);
        return 1;
    }
    if(t)
        setRecordBoard(x, y, 0);
    return 0;
}

int doubleThree(int x, int y, int user)
{
    if(three(x, y, user) >= 2)
        return 1;
    else
        return 0;
}

int doubleFour(int x, int y, int user)
{
    struct point s;
    s = four(x, y, user);
    if(s.x + s.y >= 2)
        return 1;
    else
        return 0;
}

//以下判断活三、活四等的方法基本是穷举了其类型

int three(int x, int y, int user)//return number of living three
{
    int t = 0, t1 = 0, t2 = 0, tmp, s= 0 ;

    if(recordBoard[x][y] == 0)
    {
        t = 1;
        setRecordBoard(x, y, (user==0)?1:2);
    }

{//三连
    //纵向
    if(isValidPiece(x-1, y, user) && isValidPiece(x-2, y, user))
        if(isUsablePlace(x-3, y) && isUsablePlace(x+1, y))
        {
            if(isUsablePlace(x-4, y) && !isForbiddenMove(x-3, y, user))
            {
                setRecordBoard(x-3, y, (user==0)?1:2);
                t1 = (!isForbiddenMove(x-4, y, user) + !isForbiddenMove(x+1, y, user));
                setRecordBoard(x-3, y, 0);
            }
            if(isUsablePlace(x+2, y) && !isForbiddenMove(x+1, y, user))
            {
                setRecordBoard(x+1, y, (user==0)?1:2);
                t2 = (!isForbiddenMove(x-3, y, user) + !isForbiddenMove(x+2, y, user));
                setRecordBoard(x+1, y, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x-1, y, user) && isValidPiece(x+1, y, user))
        if(isUsablePlace(x-2, y) && isUsablePlace(x+2, y))
        {
            if(isUsablePlace(x-3, y) && !isForbiddenMove(x-2, y, user))
            {
                setRecordBoard(x-2, y, (user==0)?1:2);
                t1 = (!isForbiddenMove(x-3, y, user) + !isForbiddenMove(x+2, y, user));
                setRecordBoard(x-2, y, 0);
            }
            if(isUsablePlace(x+3, y) && !isForbiddenMove(x+2, y, user))
            {
                setRecordBoard(x+2, y, (user==0)?1:2);
                t2 = (!isForbiddenMove(x+3, y, user) + !isForbiddenMove(x-2, y, user));
                setRecordBoard(x+2, y, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x+1, y, user) && isValidPiece(x+2, y, user))
        if(isUsablePlace(x+3, y) && isUsablePlace(x-1, y))
        {
            if(isUsablePlace(x+4, y) && !isForbiddenMove(x+3, y, user))
            {
                setRecordBoard(x+3, y, (user==0)?1:2);
                t1 = (!isForbiddenMove(x+4, y, user) + !isForbiddenMove(x-1, y, user));
                setRecordBoard(x+3, y, 0);
            }
            if(isUsablePlace(x-2, y) && !isForbiddenMove(x-1, y, user))
            {
                setRecordBoard(x-1, y, (user==0)?1:2);
                t2 = (!isForbiddenMove(x+3, y, user) + !isForbiddenMove(x-2, y, user));
                setRecordBoard(x-1, y, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    //横向
    if(isValidPiece(x, y-1, user) && isValidPiece(x, y-2, user))
        if(isUsablePlace(x, y-3) && isUsablePlace(x, y+1))
        {
            if(isUsablePlace(x, y-4) && !isForbiddenMove(x, y-3, user))
            {
                setRecordBoard(x, y-3, (user==0)?1:2);
                t1 = (!isForbiddenMove(x, y-4, user) + !isForbiddenMove(x, y+1, user));
                setRecordBoard(x, y-3, 0);
            }
            if(isUsablePlace(x, y+2) && !isForbiddenMove(x, y+1, user))
            {
                setRecordBoard(x, y+1, (user==0)?1:2);
                t2 = (!isForbiddenMove(x, y-3, user) + !isForbiddenMove(x, y+2, user));
                setRecordBoard(x, y+1, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x, y-1, user) && isValidPiece(x, y+1, user))
        if(isUsablePlace(x, y-2) && isUsablePlace(x, y+2))
        {
            if(isUsablePlace(x, y-3) && !isForbiddenMove(x, y-2, user))
            {
                setRecordBoard(x, y-2, (user==0)?1:2);
                t1 = (!isForbiddenMove(x, y-3, user) + !isForbiddenMove(x, y+2, user));
                setRecordBoard(x, y-2, 0);
            }
            if(isUsablePlace(x, y+3) && !isForbiddenMove(x, y+2, user))
            {
                setRecordBoard(x, y+2, (user==0)?1:2);
                t2 = (!isForbiddenMove(x, y+3, user) + !isForbiddenMove(x, y-2, user));
                setRecordBoard(x, y+2, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x, y+1, user) && isValidPiece(x, y+2, user))
        if(isUsablePlace(x, y+3) && isUsablePlace(x, y-1))
        {
            if(isUsablePlace(x, y+4) && !isForbiddenMove(x, y+3, user))
            {
                setRecordBoard(x, y+3, (user==0)?1:2);
                t1 = (!isForbiddenMove(x, y+4, user) + !isForbiddenMove(x, y-1, user));
                setRecordBoard(x, y+3, 0);
            }
            if(isUsablePlace(x, y-2) && !isForbiddenMove(x, y-1, user))
            {
                setRecordBoard(x, y-1, (user==0)?1:2);
                t2 = (!isForbiddenMove(x, y+3, user) + !isForbiddenMove(x, y-2, user));
                setRecordBoard(x, y-1, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    //左斜
    if(isValidPiece(x-1, y+1, user) && isValidPiece(x-2, y+2, user))
        if(isUsablePlace(x-3, y+3) && isUsablePlace(x+1, y-1))
        {
            if(isUsablePlace(x-4, y+4) && !isForbiddenMove(x-3, y+3, user))
            {
                setRecordBoard(x-3, y+3, (user==0)?1:2);
                t1 = (!isForbiddenMove(x-4, y+4, user) + !isForbiddenMove(x+1, y-1, user));
                setRecordBoard(x-3, y+3, 0);
            }
            if(isUsablePlace(x+2, y-2) && !isForbiddenMove(x+1, y-1, user))
            {
                setRecordBoard(x+1, y-1, (user==0)?1:2);
                t2 = (!isForbiddenMove(x-3, y+3, user) + !isForbiddenMove(x+2, y-2, user));
                setRecordBoard(x+1, y-1, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x-1, y+1, user) && isValidPiece(x+1, y-1, user))
        if(isUsablePlace(x-2, y+2) && isUsablePlace(x+2, y-2))
        {
            if(isUsablePlace(x-3, y+3) && !isForbiddenMove(x-2, y+2, user))
            {
                setRecordBoard(x-2, y+2, (user==0)?1:2);
                t1 = (!isForbiddenMove(x-3, y+3, user) + !isForbiddenMove(x+2, y-2, user));
                setRecordBoard(x-2, y+2, 0);
            }
            if(isUsablePlace(x+3, y-3) && !isForbiddenMove(x+2, y-2, user))
            {
                setRecordBoard(x+2, y-2, (user==0)?1:2);
                t2 = (!isForbiddenMove(x+3, y-3, user) + !isForbiddenMove(x-2, y+2, user));
                setRecordBoard(x+2, y-2, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x+1, y-1, user) && isValidPiece(x+2, y-2, user))
        if(isUsablePlace(x+3, y-3) && isUsablePlace(x-1, y+1))
        {
            if(isUsablePlace(x+4, y-4) && !isForbiddenMove(x+3, y-3, user))
            {
                setRecordBoard(x+3, y-3, (user==0)?1:2);
                t1 = (!isForbiddenMove(x+4, y-4, user) + !isForbiddenMove(x-1, y+1, user));
                setRecordBoard(x+3, y-3, 0);
            }
            if(isUsablePlace(x-2, y+2) && !isForbiddenMove(x-1, y+1, user))
            {
                setRecordBoard(x-1, y+1, (user==0)?1:2);
                t2 = (!isForbiddenMove(x+3, y-3, user) + !isForbiddenMove(x-2, y+2, user));
                setRecordBoard(x-1, y+1, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    //右斜
    if(isValidPiece(x-1, y-1, user) && isValidPiece(x-2, y-2, user))
        if(isUsablePlace(x-3, y-3) && isUsablePlace(x+1, y+1))
        {
            if(isUsablePlace(x-4, y-4) && !isForbiddenMove(x-3, y-3, user))
            {
                setRecordBoard(x-3, y-3, (user==0)?1:2);
                t1 = (!isForbiddenMove(x-4, y-4, user) + !isForbiddenMove(x+1, y+1, user));
                setRecordBoard(x-3, y-3, 0);
            }
            if(isUsablePlace(x+2, y+2) && !isForbiddenMove(x+1, y+1, user))
            {
                setRecordBoard(x+1, y+1, (user==0)?1:2);
                t2 = (!isForbiddenMove(x-3, y-3, user) + !isForbiddenMove(x+2, y+2, user));
                setRecordBoard(x+1, y+1, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x-1, y-1, user) && isValidPiece(x+1, y+1, user))
        if(isUsablePlace(x-2, y-2) && isUsablePlace(x+2, y+2))
        {
            if(isUsablePlace(x-3, y-3) && !isForbiddenMove(x-2, y-2, user))
            {
                setRecordBoard(x-2, y-2, (user==0)?1:2);
                t1 = (!isForbiddenMove(x-3, y-3, user) + !isForbiddenMove(x+2, y+2, user));
                setRecordBoard(x-2, y-2, 0);
            }
            if(isUsablePlace(x+3, y+3) && !isForbiddenMove(x+2, y+2, user))
            {
                setRecordBoard(x+2, y+2, (user==0)?1:2);
                t2 = (!isForbiddenMove(x+3, y+3, user) + !isForbiddenMove(x-2, y-2, user));
                setRecordBoard(x+2, y+2, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
    if(isValidPiece(x+1, y+1, user) && isValidPiece(x+2, y+2, user))
        if(isUsablePlace(x+3, y+3) && isUsablePlace(x-1, y-1))
        {
            if(isUsablePlace(x+4, y+4) && !isForbiddenMove(x+3, y+3, user))
            {
                setRecordBoard(x+3, y+3, (user==0)?1:2);
                t1 = (!isForbiddenMove(x+4, y+4, user) + !isForbiddenMove(x-1, y-1, user));
                setRecordBoard(x+3, y+3, 0);
            }
            if(isUsablePlace(x-2, y-2) && !isForbiddenMove(x-1, y-1, user))
            {
                setRecordBoard(x-1, y-1, (user==0)?1:2);
                t2 = (!isForbiddenMove(x+3, y+3, user) + !isForbiddenMove(x-2, y-2, user));
                setRecordBoard(x-1, y-1, 0);
            }
            s += ((t1 == 2) || (t2 == 2));
        }
    t1 = t2 = 0;
}

{//缺1
    //纵向
    //.| .
    if(isValidPiece(x-1, y, user) && isValidPiece(x+2, y, user) && isAvailablePlace(x+1, y, user))
    {
        setRecordBoard(x+1, y, (user==0)?1:2);
        tmp = (isAvailablePlace(x-2, y, user) + isAvailablePlace(x+3, y, user));
        s += (tmp == 2);
        setRecordBoard(x+1, y, 0);
    }
    //. |.
    if(isValidPiece(x-2, y, user) && isValidPiece(x+1, y, user) && isAvailablePlace(x-1, y, user))
    {
        setRecordBoard(x-1, y, (user==0)?1:2);
        tmp = (isAvailablePlace(x-3, y, user) + isAvailablePlace(x+2, y, user));
        s += (tmp == 2);
        setRecordBoard(x-1, y, 0);
    }
    //. .|
    if(isValidPiece(x-3, y, user) && isValidPiece(x-1, y, user) && isAvailablePlace(x-2, y, user))
    {
        setRecordBoard(x-2, y, (user==0)?1:2);
        tmp = (isAvailablePlace(x-4, y, user) + isAvailablePlace(x+1, y, user));
        s += (tmp == 2);
        setRecordBoard(x-2, y, 0);
    }
    //|. .
    if(isValidPiece(x+1, y, user) && isValidPiece(x+3, y, user) && isAvailablePlace(x+2, y, user))
    {
        setRecordBoard(x+2, y, (user==0)?1:2);
        tmp = (isAvailablePlace(x-1, y, user) + isAvailablePlace(x+4, y, user));
        s += (tmp == 2);
        setRecordBoard(x+2, y, 0);
    }
    //.. |
    if(isValidPiece(x-3, y, user) && isValidPiece(x-2, y, user) && isAvailablePlace(x-1, y, user))
    {
        setRecordBoard(x-1, y, (user==0)?1:2);
        tmp = (isAvailablePlace(x-4, y, user) + isAvailablePlace(x+1, y, user));
        s += (tmp == 2);
        setRecordBoard(x-1, y, 0);
    }
    //| ..
    if(isValidPiece(x+3, y, user) && isValidPiece(x+2, y, user) && isAvailablePlace(x+1, y, user))
    {
        setRecordBoard(x+1, y, (user==0)?1:2);
        tmp = (isAvailablePlace(x+4, y, user) + isAvailablePlace(x-1, y, user));
        s += (tmp == 2);
        setRecordBoard(x+1, y, 0);
    }
    //横向
    //.| .
    if(isValidPiece(x, y-1, user) && isValidPiece(x, y+2, user) && isAvailablePlace(x, y+1, user))
    {
        setRecordBoard(x, y+1, (user==0)?1:2);
        tmp = (isAvailablePlace(x, y-2, user) + isAvailablePlace(x, y+3, user));
        s += (tmp == 2);
        setRecordBoard(x, y+1, 0);
    }
    //. |.
    if(isValidPiece(x, y-2, user) && isValidPiece(x, y+1, user) && isAvailablePlace(x, y-1, user))
    {
        setRecordBoard(x, y-1, (user==0)?1:2);
        tmp = (isAvailablePlace(x, y-3, user) + isAvailablePlace(x, y+2, user));
        s += (tmp == 2);
        setRecordBoard(x, y-1, 0);
    }
    //. .|
    if(isValidPiece(x, y-3, user) && isValidPiece(x, y-1, user) && isAvailablePlace(x, y-2, user))
    {
        setRecordBoard(x, y-2, (user==0)?1:2);
        tmp = (isAvailablePlace(x, y-4, user) + isAvailablePlace(x, y+1, user));
        s += (tmp == 2);
        setRecordBoard(x, y-2, 0);
    }
    //|. .
    if(isValidPiece(x, y+1, user) && isValidPiece(x, y+3, user) && isAvailablePlace(x, y+2, user))
    {
        setRecordBoard(x, y+2, (user==0)?1:2);
        tmp = (isAvailablePlace(x, y-1, user) + isAvailablePlace(x, y+4, user));
        s += (tmp == 2);
        setRecordBoard(x, y+2, 0);
    }
    //.. |
    if(isValidPiece(x, y-3, user) && isValidPiece(x, y-2, user) && isAvailablePlace(x, y-1, user))
    {
        setRecordBoard(x, y-1, (user==0)?1:2);
        tmp = (isAvailablePlace(x, y-4, user) + isAvailablePlace(x, y+1, user));
        s += (tmp == 2);
        setRecordBoard(x, y-1, 0);
    }
    //| ..
    if(isValidPiece(x, y+3, user) && isValidPiece(x, y+2, user) && isAvailablePlace(x, y+1, user))
    {
        setRecordBoard(x, y+1, (user==0)?1:2);
        tmp = (isAvailablePlace(x, y+4, user) + isAvailablePlace(x, y-1, user));
        s += (tmp == 2);
        setRecordBoard(x, y+1, 0);
    }
    //左斜
    //.| .
    if(isValidPiece(x-1, y+1, user) && isValidPiece(x+2, y-2, user) && isAvailablePlace(x+1, y-1, user))
    {
        setRecordBoard(x+1, y-1, (user==0)?1:2);
        tmp = (isAvailablePlace(x-2, y+2, user) + isAvailablePlace(x+3, y-3, user));
        s += (tmp == 2);
        setRecordBoard(x+1, y-1, 0);
    }
    //. |.
    if(isValidPiece(x-2, y+2, user) && isValidPiece(x+1, y-1, user) && isAvailablePlace(x-1, y+1, user))
    {
        setRecordBoard(x-1, y+1, (user==0)?1:2);
        tmp = (isAvailablePlace(x-3, y+3, user) + isAvailablePlace(x+2, y-2, user));
        s += (tmp == 2);
        setRecordBoard(x-1, y+1, 0);
    }
    //. .|
    if(isValidPiece(x-3, y+3, user) && isValidPiece(x-1, y+1, user) && isAvailablePlace(x-2, y+2, user))
    {
        setRecordBoard(x-2, y+2, (user==0)?1:2);
        tmp = (isAvailablePlace(x-4, y+4, user) + isAvailablePlace(x+1, y-1, user));
        s += (tmp == 2);
        setRecordBoard(x-2, y+2, 0);
    }
    //|. .
    if(isValidPiece(x+1, y-1, user) && isValidPiece(x+3, y-3, user) && isAvailablePlace(x+2, y-2, user))
    {
        setRecordBoard(x+2, y-2, (user==0)?1:2);
        tmp = (isAvailablePlace(x-1, y+1, user) + isAvailablePlace(x+4, y-4, user));
        s += (tmp == 2);
        setRecordBoard(x+2, y-2, 0);
    }
    //.. |
    if(isValidPiece(x-3, y+3, user) && isValidPiece(x-2, y+2, user) && isAvailablePlace(x-1, y+1, user))
    {
        setRecordBoard(x-1, y+1, (user==0)?1:2);
        tmp = (isAvailablePlace(x-4, y+4, user) + isAvailablePlace(x+1, y-1, user));
        s += (tmp == 2);
        setRecordBoard(x-1, y+1, 0);
    }
    //| ..
    if(isValidPiece(x+3, y-3, user) && isValidPiece(x+2, y-2, user) && isAvailablePlace(x+1, y-1, user))
    {
        setRecordBoard(x+1, y-1, (user==0)?1:2);
        tmp = (isAvailablePlace(x+4, y-4, user) + isAvailablePlace(x-1, y+1, user));
        s += (tmp == 2);
        setRecordBoard(x+1, y-1, 0);
    }
    //右斜
    //.| .
    if(isValidPiece(x-1, y-1, user) && isValidPiece(x+2, y+2, user) && isAvailablePlace(x+1, y+1, user))
    {
        setRecordBoard(x+1, y+1, (user==0)?1:2);
        tmp = (isAvailablePlace(x-2, y-2, user) + isAvailablePlace(x+3, y+3, user));
        s += (tmp == 2);
        setRecordBoard(x+1, y+1, 0);
    }
    //. |.
    if(isValidPiece(x-2, y-2, user) && isValidPiece(x+1, y+1, user) && isAvailablePlace(x-1, y-1, user))
    {
        setRecordBoard(x-1, y-1, (user==0)?1:2);
        tmp = (isAvailablePlace(x-3, y-3, user) + isAvailablePlace(x+2, y+2, user));
        s += (tmp == 2);
        setRecordBoard(x-1, y-1, 0);
    }
    //. .|
    if(isValidPiece(x-3, y-3, user) && isValidPiece(x-1, y-1, user) && isAvailablePlace(x-2, y-2, user))
    {
        setRecordBoard(x-2, y-2, (user==0)?1:2);
        tmp = (isAvailablePlace(x-4, y-4, user) + isAvailablePlace(x+1, y+1, user));
        s += (tmp == 2);
        setRecordBoard(x-2, y-2, 0);
    }
    //|. .
    if(isValidPiece(x+1, y+1, user) && isValidPiece(x+3, y+3, user) && isAvailablePlace(x+2, y+2, user))
    {
        setRecordBoard(x+2, y+2, (user==0)?1:2);
        tmp = (isAvailablePlace(x-1, y-1, user) + isAvailablePlace(x+4, y+4, user));
        s += (tmp == 2);
        setRecordBoard(x+2, y+2, 0);
    }
    //.. |
    if(isValidPiece(x-3, y-3, user) && isValidPiece(x-2, y-2, user) && isAvailablePlace(x-1, y-1, user))
    {
        setRecordBoard(x-1, y-1, (user==0)?1:2);
        tmp = (isAvailablePlace(x-4, y-4, user) + isAvailablePlace(x+1, y+1, user));
        s += (tmp == 2);
        setRecordBoard(x-1, y-1, 0);
    }
    //| ..
    if(isValidPiece(x+3, y+3, user) && isValidPiece(x+2, y+2, user) && isAvailablePlace(x+1, y+1, user))
    {
        setRecordBoard(x+1, y+1, (user==0)?1:2);
        tmp = (isAvailablePlace(x+4, y+4, user) + isAvailablePlace(x-1, y-1, user));
        s += (tmp == 2);
        setRecordBoard(x+1, y+1, 0);
    }
}

    if(t)
        setRecordBoard(x, y, 0);
    return s;
}

struct point four(int x, int y, int user)//s.x: number of living four, s.y: number of die four
{
    int i, j, t, tmp = 0;
    struct point s = {0, 0};

    if(recordBoard[x][y] == 0)
    {
        tmp = 1;
        setRecordBoard(x, y, (user==0)?1:2);
    }

{//四连
    //纵向
    for(i = 1; isValidPiece(x-i, y, user); ++i)
        if(i == 3)
        {
            t = ( isAvailablePlace(x-4, y, user) + isAvailablePlace(x+1, y, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    for(j = 1, --i; isValidPiece(x+j, y, user); ++j)
        if(j+i == 3)
        {
            t = ( isAvailablePlace(x-i-1, y, user) + isAvailablePlace(x+j+1, y, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    //横向
    for(i = 1; isValidPiece(x, y-i, user); ++i)
        if(i == 3)
        {
            t = ( isAvailablePlace(x, y-4, user) + isAvailablePlace(x, y+1, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    for(j = 1, --i; isValidPiece(x, y+j, user); ++j)
        if(j+i == 3)
        {
            t = ( isAvailablePlace(x, y-i-1, user) + isAvailablePlace(x, y+j+1, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    //左斜
    for(i = 1; isValidPiece(x-i, y+i, user); ++i)
        if(i == 3)
        {
            t = ( isAvailablePlace(x-4, y+4, user) + isAvailablePlace(x+1, y-1, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    for(j = 1, --i; isValidPiece(x+j, y-j, user); ++j)
        if(j+i == 3)
        {
            t = ( isAvailablePlace(x-i-1, y+i+1, user) + isAvailablePlace(x+j+1, y-j-1, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    //右斜
    for(i = 1; isValidPiece(x-i, y-i, user); ++i)
        if(i == 3)
        {
            t = ( isAvailablePlace(x-4, y-4, user) + isAvailablePlace(x+1, y+1, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
    for(j = 1, --i; isValidPiece(x+j, y+j, user); ++j)
        if(j+i == 3)
        {
            t = ( isAvailablePlace(x-i-1, y-i-1, user) + isAvailablePlace(x+j+1, y+j+1, user) );
            s.x += (t == 2);
            s.y += (t == 1);
            break;
        }
}

{//缺1
    //纵向
    //. ...
    if(isValidPiece(x+2, y, user) && isValidPiece(x+3, y, user) && isValidPiece(x+4, y, user) && isAvailablePlace(x+1, y, user))
        ++s.y;
    if(isValidPiece(x-2, y, user) && isValidPiece(x+1, y, user) && isValidPiece(x+2, y, user) && isAvailablePlace(x-1, y, user))
        ++s.y;
    if(isValidPiece(x-3, y, user) && isValidPiece(x-1, y, user) && isValidPiece(x+1, y, user) && isAvailablePlace(x-2, y, user))
        ++s.y;
    if(isValidPiece(x-4, y, user) && isValidPiece(x-2, y, user) && isValidPiece(x-1, y, user) && isAvailablePlace(x-3, y, user))
        ++s.y;
    //.. ..
    if(isValidPiece(x+1, y, user) && isValidPiece(x+3, y, user) && isValidPiece(x+4, y, user) && isAvailablePlace(x+2, y, user))
        ++s.y;
    if(isValidPiece(x-1, y, user) && isValidPiece(x+2, y, user) && isValidPiece(x+3, y, user) && isAvailablePlace(x+1, y, user))
        ++s.y;
    if(isValidPiece(x-3, y, user) && isValidPiece(x-2, y, user) && isValidPiece(x+1, y, user) && isAvailablePlace(x-1, y, user))
        ++s.y;
    if(isValidPiece(x-4, y, user) && isValidPiece(x-3, y, user) && isValidPiece(x-1, y, user) && isAvailablePlace(x-2, y, user))
        ++s.y;
    //... .
    if(isValidPiece(x+1, y, user) && isValidPiece(x+2, y, user) && isValidPiece(x+4, y, user) && isAvailablePlace(x+3, y, user))
        ++s.y;
    if(isValidPiece(x-1, y, user) && isValidPiece(x+1, y, user) && isValidPiece(x+3, y, user) && isAvailablePlace(x+2, y, user))
        ++s.y;
    if(isValidPiece(x-1, y, user) && isValidPiece(x-2, y, user) && isValidPiece(x+2, y, user) && isAvailablePlace(x+1, y, user))
        ++s.y;
    if(isValidPiece(x-4, y, user) && isValidPiece(x-3, y, user) && isValidPiece(x-2, y, user) && isAvailablePlace(x-1, y, user))
        ++s.y;
    //横向
    //. ...
    if(isValidPiece(x, y+2, user) && isValidPiece(x, y+3, user) && isValidPiece(x, y+4, user) && isAvailablePlace(x, y+1, user))
        ++s.y;
    if(isValidPiece(x, y-2, user) && isValidPiece(x, y+1, user) && isValidPiece(x, y+2, user) && isAvailablePlace(x, y-1, user))
        ++s.y;
    if(isValidPiece(x, y-3, user) && isValidPiece(x, y-1, user) && isValidPiece(x, y+1, user) && isAvailablePlace(x, y-2, user))
        ++s.y;
    if(isValidPiece(x, y-4, user) && isValidPiece(x, y-2, user) && isValidPiece(x, y-1, user) && isAvailablePlace(x, y-3, user))
        ++s.y;
    //.. ..
    if(isValidPiece(x, y+1, user) && isValidPiece(x, y+3, user) && isValidPiece(x, y+4, user) && isAvailablePlace(x, y+2, user))
        ++s.y;
    if(isValidPiece(x, y-1, user) && isValidPiece(x, y+2, user) && isValidPiece(x, y+3, user) && isAvailablePlace(x, y+1, user))
        ++s.y;
    if(isValidPiece(x, y-3, user) && isValidPiece(x, y-2, user) && isValidPiece(x, y+1, user) && isAvailablePlace(x, y-1, user))
        ++s.y;
    if(isValidPiece(x, y-4, user) && isValidPiece(x, y-3, user) && isValidPiece(x, y-1, user) && isAvailablePlace(x, y-2, user))
        ++s.y;
    //... .
    if(isValidPiece(x, y+1, user) && isValidPiece(x, y+2, user) && isValidPiece(x, y+4, user) && isAvailablePlace(x, y+3, user))
        ++s.y;
    if(isValidPiece(x, y-1, user) && isValidPiece(x, y+1, user) && isValidPiece(x, y+3, user) && isAvailablePlace(x, y+2, user))
        ++s.y;
    if(isValidPiece(x, y-1, user) && isValidPiece(x, y-2, user) && isValidPiece(x, y+2, user) && isAvailablePlace(x, y+1, user))
        ++s.y;
    if(isValidPiece(x, y-4, user) && isValidPiece(x, y-3, user) && isValidPiece(x, y-2, user) && isAvailablePlace(x, y-1, user))
        ++s.y;
    //左斜
    //. ...
    if(isValidPiece(x+2, y-2, user) && isValidPiece(x+3, y-3, user) && isValidPiece(x+4, y-4, user) && isAvailablePlace(x+1, y-1, user))
        ++s.y;
    if(isValidPiece(x-2, y+2, user) && isValidPiece(x+1, y-1, user) && isValidPiece(x+2, y-2, user) && isAvailablePlace(x-1, y+1, user))
        ++s.y;
    if(isValidPiece(x-3, y+3, user) && isValidPiece(x-1, y+1, user) && isValidPiece(x+1, y-1, user) && isAvailablePlace(x-2, y+2, user))
        ++s.y;
    if(isValidPiece(x-4, y+4, user) && isValidPiece(x-2, y+2, user) && isValidPiece(x-1, y+1, user) && isAvailablePlace(x-3, y+3, user))
        ++s.y;
    //.. ..
    if(isValidPiece(x+1, y-1, user) && isValidPiece(x+3, y-3, user) && isValidPiece(x+4, y-4, user) && isAvailablePlace(x+2, y-2, user))
        ++s.y;
    if(isValidPiece(x-1, y+1, user) && isValidPiece(x+2, y-2, user) && isValidPiece(x+3, y-3, user) && isAvailablePlace(x+1, y-1, user))
        ++s.y;
    if(isValidPiece(x-3, y+3, user) && isValidPiece(x-2, y+2, user) && isValidPiece(x+1, y-1, user) && isAvailablePlace(x-1, y+1, user))
        ++s.y;
    if(isValidPiece(x-4, y+4, user) && isValidPiece(x-3, y+3, user) && isValidPiece(x-1, y+1, user) && isAvailablePlace(x-2, y+2, user))
        ++s.y;
    //... .
    if(isValidPiece(x+1, y-1, user) && isValidPiece(x+2, y-2, user) && isValidPiece(x+4, y-4, user) && isAvailablePlace(x+3, y-3, user))
        ++s.y;
    if(isValidPiece(x-1, y+1, user) && isValidPiece(x+1, y-1, user) && isValidPiece(x+3, y-3, user) && isAvailablePlace(x+2, y-2, user))
        ++s.y;
    if(isValidPiece(x-1, y+1, user) && isValidPiece(x-2, y+2, user) && isValidPiece(x+2, y-2, user) && isAvailablePlace(x+1, y-1, user))
        ++s.y;
    if(isValidPiece(x-4, y+4, user) && isValidPiece(x-3, y+3, user) && isValidPiece(x-2, y+2, user) && isAvailablePlace(x-1, y+1, user))
        ++s.y;
    //右斜
    //. ...
    if(isValidPiece(x+2, y+2, user) && isValidPiece(x+3, y+3, user) && isValidPiece(x+4, y+4, user) && isAvailablePlace(x+1, y+1, user))
        ++s.y;
    if(isValidPiece(x-2, y-2, user) && isValidPiece(x+1, y+1, user) && isValidPiece(x+2, y+2, user) && isAvailablePlace(x-1, y-1, user))
        ++s.y;
    if(isValidPiece(x-3, y-3, user) && isValidPiece(x-1, y-1, user) && isValidPiece(x+1, y+1, user) && isAvailablePlace(x-2, y-2, user))
        ++s.y;
    if(isValidPiece(x-4, y-4, user) && isValidPiece(x-2, y-2, user) && isValidPiece(x-1, y-1, user) && isAvailablePlace(x-3, y-3, user))
        ++s.y;
    //.. ..
    if(isValidPiece(x+1, y+1, user) && isValidPiece(x+3, y+3, user) && isValidPiece(x+4, y+4, user) && isAvailablePlace(x+2, y+2, user))
        ++s.y;
    if(isValidPiece(x-1, y-1, user) && isValidPiece(x+2, y+2, user) && isValidPiece(x+3, y+3, user) && isAvailablePlace(x+1, y+1, user))
        ++s.y;
    if(isValidPiece(x-3, y-3, user) && isValidPiece(x-2, y-2, user) && isValidPiece(x+1, y+1, user) && isAvailablePlace(x-1, y-1, user))
        ++s.y;
    if(isValidPiece(x-4, y-4, user) && isValidPiece(x-3, y-3, user) && isValidPiece(x-1, y-1, user) && isAvailablePlace(x-2, y-2, user))
        ++s.y;
    //... .
    if(isValidPiece(x+1, y+1, user) && isValidPiece(x+2, y+2, user) && isValidPiece(x+4, y+4, user) && isAvailablePlace(x+3, y+3, user))
        ++s.y;
    if(isValidPiece(x-1, y-1, user) && isValidPiece(x+1, y+1, user) && isValidPiece(x+3, y+3, user) && isAvailablePlace(x+2, y+2, user))
        ++s.y;
    if(isValidPiece(x-1, y-1, user) && isValidPiece(x-2, y-2, user) && isValidPiece(x+2, y+2, user) && isAvailablePlace(x+1, y+1, user))
        ++s.y;
    if(isValidPiece(x-4, y-4, user) && isValidPiece(x-3, y-3, user) && isValidPiece(x-2, y-2, user) && isAvailablePlace(x-1, y-1, user))
        ++s.y;
}

    if(tmp)
        setRecordBoard(x, y, 0);
    return s;
}

int five(int x, int y, int user)//-1 for less than five, 0 for five, 1 for more than five
{
    int i, j, t = 0;

    if(recordBoard[x][y] == 0)
    {
        t = 1;
        setRecordBoard(x, y, (user==0)?1:2);
    }
    //纵向
    for(i = 1; isValidPiece(x-i, y, user); ++i)
        if(i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x-5, y, user) || isValidPiece(x+1, y, user))
                return 1;
            else
                return 0;
        }
    for(j = 1, --i; isValidPiece(x+j, y, user); ++j)
        if(j+i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x+j+1, y, user))
                return 1;
            else
                return 0;
        }
    //横向
    for(i = 1; isValidPiece(x, y-i, user); ++i)
        if(i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x, y-5, user) || isValidPiece(x, y+1, user))
                return 1;
            else
                return 0;
        }
    for(j = 1, --i; isValidPiece(x, y+j, user); ++j)
        if(j+i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x, y+j+1, user))
                return 1;
            else
                return 0;
        }
    //左斜
    for(i = 1; isValidPiece(x-i, y+i, user); ++i)
        if(i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x-5, y+5, user) || isValidPiece(x+1, y-1, user))
                return 1;
            else
                return 0;
        }
    for(j = 1, --i; isValidPiece(x+j, y-j, user); ++j)
        if(j+i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x+j+1, y-j-1, user))
                return 1;
            else
                return 0;
        }
    //右斜
    for(i = 1; isValidPiece(x-i, y-i, user); ++i)
        if(i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x-5, y-5, user) || isValidPiece(x+1, y+1, user))
                return 1;
            else
                return 0;
        }
    for(j = 1, --i; isValidPiece(x+j, y+j, user); ++j)
        if(j+i == 4)
        {
            if(t)
                setRecordBoard(x, y, 0);
            if(isValidPiece(x+j+1, y+j+1, user))
                return 1;
            else
                return 0;
        }

    if(t)
        setRecordBoard(x, y, 0);
    return -1;
}