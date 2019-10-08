#include "head.h"

int scoreTable[8][8][8][8];//分值表，根据一个点四个方向的棋型查找分值

void AI()
{
    if(pIndex == 0 || pIndex == 1)//第一子靠近中心落子
    {
        if(recordBoard[7][8] != 0)//如果已经有子
            place(8, 8, currentUser);
        else
            place(7, 8, currentUser);
    }
    else
        maxmin(currentUser);
}

void maxmin(int user)//带alpha-beta剪枝的负值最大搜索
{
    struct node piece[SIZE*SIZE+1];//储存生成的落子点
    int n = generate(user, piece);//生成按分值排序的可落子点
    int i, x, y;
    int alpha = initialAlpha, t;
    int begin = clock();
    int mx = 1, my = 1;
    for(i = 1; i <= n; ++i)
    {
        x = piece[i].x;
        y = piece[i].y;
        setRecordBoard(x, y, (user==0)?1:2);//尝试落子
        updateFlag(x, y, 1);
        updatePattern(x, y);
        t = -alphabeta(!user, DEPTH-1, initialAlpha, -alpha);//负值最大搜索
        setRecordBoard(x, y, 0);
        updatePattern(x, y);
        updateFlag(x, y, -1);
        if(t > alpha)//找到更优的解
        {
            alpha = t;
            mx = x;
            my = y;
        }
        if(clock()-begin >= MAXTIME)//如果已经超时，输出当前最优解
            break;
    }

    place(mx, my, user);
}

double alphabeta(int user, int depth, int alpha, int beta)
{
    if(depth == 0)//叶子节点，返回局面分值
        return score(user);
    struct node piece[SIZE*SIZE+1];
    int n = generate(user, piece);
    int i, x, y;
    int t;

    if(piece[1].value >= 2000)//存在必胜走法
        return WIN;

    for(i = 1; i <= n; ++i)
    {
        x = piece[i].x;
        y = piece[i].y;
        setRecordBoard(x, y, (user==0)?1:2);
        updateFlag(x, y, 1);
        updatePattern(x, y);
        t = -alphabeta(!user, depth-1, -beta, -alpha);
        setRecordBoard(x, y, 0);
        updatePattern(x, y);
        updateFlag(x, y, -1);
        if(t > alpha)
            alpha = t;
        if(alpha >= beta)//alpha-beta剪枝
            break;
    }
    return alpha;
}

int cmp(struct node a, struct node b)//按分值降序，如果相同按离棋盘中心远近升序排序
{
    if(b.value > a.value)
        return 1;
    else if(b.value < a.value)
        return -1;
    else
        return distance(a.x, a.y) - distance(b.x, b.y);
}

void mySwap(struct node p[], int a, int b)
{
    struct node t = p[a];
    p[a] = p[b];
    p[b] = t;
}

void quicksort(struct node p[], int l, int r, int (*cmp)(struct node, struct node))//对可用节点按分值降序排序
{
    int mid = (l+r-1)/2;
    int last = l, i = l+1;
    if(l >= r-1)
        return;
    mySwap(p, l, mid);
    for(; i < r; ++i)
        if((*cmp)(p[i], p[l]) < 0)
            mySwap(p, ++last, i);
    mySwap(p, last, l);
    quicksort(p, l, last, cmp);
    quicksort(p, last+1, r, cmp);
}

int evaluateMove(int x, int y, int user)//对落子点进行评分
{
	int score[2];
	score[user] = scoreTable[pattern[user][x][y][1]] [pattern[user][x][y][2]] [pattern[user][x][y][3]] [pattern[user][x][y][4]];//通过四个方向的棋型查找分值
	score[!user] = scoreTable[pattern[!user][x][y][1]] [pattern[!user][x][y][2]] [pattern[!user][x][y][3]] [pattern[!user][x][y][4]];

	if (score[user] >= 200 || score[!user] >= 200)//存在杀招
		return (score[user]>=score[!user])?score[user]*2:score[!user];//在对方不能成五情况下优先走自己的杀招
	else
		return score[user]*2 + score[!user];//返回分值，优先进攻
}

int generate(int user, struct node piece[])//生成按分值排序的可落子点，存入piece数组，返回数组大小
{
    int i, j, t;
    int n = 0;
    for(i = 1; i <= SIZE; ++i)
    {
        for(j = 1; j <= SIZE; ++j)
        {
            if(flag[i][j] && isAvailablePlace(i, j, user))//只选择两格内有子的非禁手点
            {
                if( (t = evaluateMove(i, j, user)) > 0)//至少存在死二，第一子落下后不可能连死二都没有
                {
                    ++n;
                    piece[n].x = i;
                    piece[n].y = j;
                    piece[n].value = t;
                }
            }
        }
    }
    quicksort(piece, 1, n+1, cmp);
    if(piece[1].value >= 2000)//存在必胜走法
        return 1;
    if (piece[1].value == 1200)//对方存在活三
	{
		for (i = 2; i <= n; ++i)//寻找活三点
			if (piece[i].value != 1200)
                break;
		for (; i <= n; ++i)//寻找自己和对方的死四点
		{
			if(isPatternExisting(user, piece[i].x, piece[i].y, D4) | isPatternExisting(!user, piece[i].x, piece[i].y, D4))
                ;
            else
                break;
		}
		return (i-1 > MAXN)?MAXN:n;//对方的杀招，直接返回，减少搜索量
	}
    return (n > MAXN)?MAXN:n;//最大只搜索MAXN个点
}

int isPatternExisting(int user, int x, int y, int type)//某个点是否存在某种棋型
{
    return (pattern[user][x][y][1] == type) | (pattern[user][x][y][2] == type) | (pattern[user][x][y][3] == type) | (pattern[user][x][y][4] == type);
}

int patternsToScore(int d1, int d2, int d3, int d4)//将四个方向的棋型进行评分
{
	int t[8] = {0};//L1,D2,L2,D3,L3,D4,L4,L5对应的棋型数
	int s = 0, i;
	const int weight[6] = {0, 2, 5, 5, 12, 12};//L1,D2,L2,D3,L3,D4
	++t[d1];
	++t[d2];
	++t[d3];
	++t[d4];

	if (t[L5] > 0)//冲四或者活四
		return 5000;
	if (t[L4] > 0 || t[D4] > 1)//活三
		return 1200;
	if (t[D4] > 0 && t[L3] > 0)//死三或者活二
		return 1000;
	if (t[L3] > 1)//双活二，落子后形成双三
		return 200;

	for (i = D2; i <= D4; i++)//否则按权值计分
		s += weight[i] * t[i];
	return s;
}

void initialScoreTable()
{
    int d1, d2, d3, d4;
    for(d1 = 0; d1 <= L5; ++d1)
        for(d2 = 0; d2 <= L5; ++d2)
            for(d3 = 0; d3 <= L5; ++d3)
                for(d4 = 0; d4 <= L5; ++d4)
                    scoreTable[d1][d2][d3][d4] = patternsToScore(d1, d2, d3, d4);
}

double score(int user)//返回对AI的局面评分
{
    #if(DEPTH%2 != 0)//为了返回对AI的局面评分，若搜索深度为奇数要改变user
    user = !user;
    #endif
	int userType[8] = {0};//L1,D2,L2,D3,L3,D4,L4,L5对应的棋型数
	int enemyType[8] = {0};
	int t = 0, i, j;
	int userScore = 0, enemyScore = 0;
    const int weight[8] = {0, 2, 12, 18, 96, 144, 800, 1200};//L1,D2,L2,D3,L3,D4,L4,L5的权值

	for (i = 1; i < SIZE; ++i)
	{
		for (j = 1; j < SIZE; ++j)
		{
			if (flag[i][j] && (!recordBoard[i][j]))
			{
				t = userType[D4];
				++userType[pattern[user][i][j][1]];
				++userType[pattern[user][i][j][2]];
				++userType[pattern[user][i][j][3]];
				++userType[pattern[user][i][j][4]];
				++enemyType[pattern[!user][i][j][1]];
				++enemyType[pattern[!user][i][j][2]];
				++enemyType[pattern[!user][i][j][3]];
				++enemyType[pattern[!user][i][j][4]];

				if (userType[D4] - t >= 2)//如果有两个死四，那么就是一个活四
				{
					userType[D4] -= 2;
					userType[L4]++;
				}
			}
		}
	}

	for (i = 1; i < 8; ++i)
	{
		userScore += userType[i] * weight[i];
		enemyScore += enemyType[i] * weight[i];
	}

	return userScore*1.2 - enemyScore;//攻击比防守重要
}

int distance(int x, int y)//返回离(7,8)的距离
{
    int xd, yd;
    xd = x - 7;
    yd = y - 8;
    if(xd < 0)
        xd = -xd;
    if(yd < 0)
        yd = -yd;
    return xd + yd;
}