#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 15//棋盘大小
#define MAXTIME 15000//最长搜索时间
#define MAXLENGTH 50//getLine函数的最长字符串
#define DEPTH 6//搜索深度
#define initialAlpha -100000000//alpha-beta搜索的初始alpha，尽可能小
#define initialBeta   100000000//alpha-beta搜索的初始beta，尽可能大
#define MAXN 40//每层最大搜索点数，由于进行了排序，认为太后面的点是比较差的

#define WHITE 0
#define BLACK 1

#define EMPTY 0
#define WHITEPIECE 1//白子
#define BLACKPIECE 2//黑子
#define OUTSIDE 3//超出棋盘
#define WIN 10000

#define L1 0//活一
#define D2 1//死二
#define L2 2//活二
#define D3 3//死三
#define L3 4//活三
#define D4 5//死四
#define L4 6//活四
#define L5 7//五连

struct point//储存坐标
{
    int x;
    int y;
};

struct node{//储存坐标及其评分
    int x;
    int y;
    int value;
};

//全局变量
extern int currentUser;
extern int mode;//模式

extern const int dx[5];//方向向量，用于遍历
extern const int dy[5];

extern int recordBoard[SIZE+1][SIZE+1];//棋盘记录，0：空，1：黑棋，2：白棋
extern struct point recordPoint[SIZE*SIZE+1];//储存下的第i个点的坐标
extern int pIndex;////用来记录下的点数，从1开始
extern int patternTable[2][65536];//棋型表，根据key的值查找棋型
extern int pattern[2][SIZE+1][SIZE+1][5];//棋型记录表
extern int flag[SIZE+1][SIZE+1];//周围以2为半径延伸的5*5矩形中的其他棋子数

extern int scoreTable[8][8][8][8];//分值表，根据一个点四个方向的棋型查找分值

//函数

//gobang.c
int main();
void operate();//让玩家或者AI进行操作
void changeCurrentUser();

//AI.c
void AI();
void maxmin(int user);//带alpha-beta剪枝的负值最大搜索
double alphabeta(int user, int depth, int alpha, int beta);//alpha-beta搜索
int cmp(struct node a, struct node b);//按分值降序，如果相同按离棋盘中心远近升序排序
void mySwap(struct node p[], int a, int b);
void quicksort(struct node p[], int l, int r, int (*cmp)(struct node, struct node));//对可用节点按分值降序排序
int evaluateMove(int x, int y, int user);//对落子点进行评分
int isPatternExisting(int user, int x, int y, int type);//某个点是否存在某种棋型
int generate(int user, struct node piece[]);//生成按分值排序的可落子点，存入piece数组，返回数组大小
int patternsToScore(int d1, int d2, int d3, int d4);//将四个方向的棋型进行评分
void initialScoreTable();
double score(int user);//返回对AI的局面评分
int distance(int x, int y);//返回离(7,8)的距离

//player.c
void player();
int operate_player();//玩家进行操作
int recall();//返回撤回的步数，0表示失败
int isEqual_string(char a[], char b[]);//判断字符串a、b是否相等
int translateCharToInt(char a[3], int *x, int *y);//将输入的坐标转换成程序内坐标，例如A15->(1,1)，返回输入是否合法

//others.c
int getLine(char a[]);//get a line inputed or a (MAXLENGTH-1) long string, return '\0''s index
int isPieceOfUser(int x, int y, int user);
void initial();
int isVictorious(int user);
void restart();//重新开始
int subrestart();//要求输入正确
int isInBoard(int x, int y);
int isValidPiece(int x, int y, int user);//是否在棋盘内以及user的棋子
int isAvailablePlace(int x, int y, int user);//是否是在棋盘内的、空的、非禁手的
int isUsablePlace(int x, int y);//是否是在棋盘内的、非禁手的

//board.c
void initialPatternTable();
void updateFlag(int x, int y, int state);//更新(x, y)周围以2为半径延伸的5*5矩形的flag，不包括(x, y)
void updatePattern(int x, int y);//更新(x, y)周围四个方向32个点的棋型，不包括(x, y)
int getKey(int x, int y, int d);//获取点在方向d上的key
int keyToPattern(int user, int key);//将key转换成棋型
int place(int x, int y, int user);//落子，失败返回0
void setRecordBoard(int x, int y, int state);//(0,0)：初始化，(-1, -1)黑白交换
void displayWord();
void displayBoard();
void translateIntToChar(char *ansx, int *ansy, int x, int y);//例如(1,1)->A15

//forbiddenMove.c
int isForbiddenMove(int x, int y, int user);
int doubleThree(int x, int y, int user);
int doubleFour(int x, int y, int user);
int three(int x, int y, int user);//return number of living three
struct point four(int x, int y, int user);//s.x: number of living four, s.y: number of die four
int five(int x, int y, int user);//-1 for less than five, 0 for five, 1 for more than five

#endif // HEAD_H_INCLUDED