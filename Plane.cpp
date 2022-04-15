#include<stdio.h>
#include<graphics.h> //图像库 
#include<mmsystem.h>  //win32 多媒体设备接口
#include<time.h>

#pragma comment(lib,"winmm.lib")   //静态库

#define WIN_WIDTH 591  //定义宏
#define WIN_HEIGHT 864
#define BULLET_NUM 15 //最大一次性发射15
#define ENEMY_NUM 10 //最大敌机
/*
int a ;
char b
*/
struct Image
{
	IMAGE backGround; //背景图片
	IMAGE Player[2];
	IMAGE Bullet[2];
	IMAGE Enemy[4];


}image;
//结构体三兄弟  结构体 枚举 联合
enum TYPE
{
	SAMLL,
	BIG,
};

struct  Plane //我 敌机 子弹
{
	int x;
	int y;
	bool flag; //是否存活
	int width;
	int height;
	union  //共享一块内存 
	{
		int type; //敌机独有属性
		int score; //玩家独有属性
	}un;
	int hp; //血量


}player,bullet[BULLET_NUM],enemy[ENEMY_NUM];

DWORD t1,t2,t3,t4;
DWORD WINAPI PlayMusic(LPVOID lp);

//分模块处理 专门的事情专门的函数
void EnemyHp(int i)
{
	int flag = rand() % 10;
	if (flag>= 0&&flag<=2) //十分之概率产生一个大飞机
	{
		enemy[i].un.type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 104;
		enemy[i].height = 148;
	}
	else
	{
		enemy[i].un.type = SAMLL;
		enemy[i].hp = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
}

//加载图片


void GameInit()  //初始化函数
{
	//设置随机数种子
	srand(GetTickCount());

	t1 = t2 = t3 = t4 = GetTickCount();//获取系统开机到当前所经过的毫秒数
	//播放音乐
	//mciSendString(TEXT("open D:\\images\\game_music.mp3 alias BGM"),0,0,0); //像多媒体设备接口发送字符串 media device interface
	//mciSendString(TEXT("play BGM repeat"),0,0,0);
	
	
	
	
	//加载背景
	loadimage(&image.backGround,_T("D:\\images\\background.jpg")); //把图片加载并保存
	loadimage(&image.Player[0], _T("D:\\images\\planeNormal_1.jpg"));
	loadimage(&image.Player[1], _T("D:\\images\\planeNormal_2.jpg"));
	loadimage(&image.Bullet[0], _T("D:\\images\\bullet1.jpg"));
	loadimage(&image.Bullet[1], _T("D:\\images\\bullet2.jpg"));
	loadimage(&image.Enemy[0], _T("D:\\images\\enemy_1.jpg"));
	loadimage(&image.Enemy[1], _T("D:\\images\\enemy_2.jpg"));
	loadimage(&image.Enemy[2], _T("D:\\images\\enemyPlane1.jpg"));
	loadimage(&image.Enemy[3], _T("D:\\images\\enemyPlane2.jpg"));
	//初始化玩家数据

	player.x = WIN_WIDTH/2;
	player.y = WIN_HEIGHT - 220;
	player.flag = true;
	player.hp = 999;
	player.un.score = 0;

	//初始化子弹数据
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;

	}
	
	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].flag = false;
		EnemyHp(i);
	}


}



bool Timer(int ms, int id)
{
	static DWORD t[10];

	if (clock() - t[id] > ms)
	{

		t[id] = clock();
		return true;
	}

	return false;
}


void Gamedraw()
{
	putimage(0, 0, &image.backGround);//绘制背景
	//绘制飞机
	putimage(player.x, player.y, &image.Player[0], NOTSRCERASE); //飞机
	putimage(player.x, player.y, &image.Player[1], SRCINVERT);  //透明位图  这两个飞机进行处理
	//绘制子弹
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag==true)
		{
			putimage(bullet[i].x, bullet[i].y, &image.Bullet[0], NOTSRCERASE);
			putimage(bullet[i].x, bullet[i].y, &image.Bullet[1], SRCINVERT);
		}
		
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag)
		{
			if (enemy[i].un.type==TYPE::SAMLL)
			{
				putimage(enemy[i].x, enemy[i].y, &image.Enemy[0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.Enemy[1], SRCINVERT);
			}
			else 
			{
				putimage(enemy[i].x, enemy[i].y, &image.Enemy[2], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.Enemy[3], SRCINVERT);
			}
			
		}
	}





}




void CreateBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag==false) //判断子弹是否存活
		{
			bullet[i].flag = true;
			bullet[i].x = player.x+45;//中间发射
			bullet[i].y = player.y;
			break;

		}
	}

}

void BulletMove(int speed) //子弹移动
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag==true)
		{
			bullet[i].y -= speed;
			if (bullet[i].y <= 0)
			{
				bullet[i].flag = false;
			}
		}
	}
}

void CreateEnemy()  //产生敌机
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag==0)
		{
			enemy[i].flag = true;
			enemy[i].x = rand() % (WIN_WIDTH-60);
			enemy[i].y = 0;        //敌机起始状态
			EnemyHp(i);
			break;
		}
	}
}
void EnemyMove(int speed) //敌机移动
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag)
		{
	
			enemy[i].y+=speed ;
			if (enemy[i].y>=WIN_HEIGHT)
			{
				enemy[i].flag = false;
			}
		}
	}

}

void GameControl(int speed)
{
	//控制移动 攻击
	//_getch(); GetAsyncKeyState();
	if (GetAsyncKeyState(VK_UP)&&player.y>=0)
	{
		player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN)&&player.y+120<=WIN_HEIGHT)
	{
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && player.x+50>=0)
	{
		player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && player.x+55<=WIN_WIDTH)
	{
		player.x += speed;
	}
	//发射子弹
	if (GetAsyncKeyState(VK_SPACE)&&Timer(300,1))
	{
	
		//HANDLE threadID = CreateThread(NULL, 0, PlayMusic , NULL , 0, 0);//创建一个线程
		//CloseHandle(threadID);

		CreateBullet();
	
	}

	

}

void PlayerEnemy()
{ 
	//1.遍历敌机数组 判断是否存活
	//2.遍历子弹数组 
	//3.判断子弹是否击中敌机
	//4.达到了，属性改变 ， 血量-- 存货状态 玩家分数 ++
	//5.如果血量为0 ， 敌机死亡
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].flag)
			continue;

		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bullet[k].flag)
				continue;
			if (bullet[k].x>enemy[i].x&&bullet[k].x<enemy[i].x+enemy[i].width
			&& bullet[k].y>enemy[i].y&&bullet[k].y<enemy[i].y+enemy[i].height)
			{
				bullet[k].flag = false;
				enemy[i].hp--;


			}

				
			
		}
		if (enemy[i].hp<=0)
		{
			enemy[i].flag = false;
			//CreateEnemy();
		}
	}




}

int main()
{
	initgraph(WIN_WIDTH,WIN_HEIGHT/*,SHOWCONSOLE*/);  //创建一个图形窗口
	GameInit();
	BeginBatchDraw(); //开启双缓冲绘图，现在内存里画好，在现实给你看


	while (1)
	{
		Gamedraw();
		FlushBatchDraw();//我想看了
		GameControl(1); //speed移动像素
		BulletMove(2);//子弹移动函数速度

		if (Timer(500,0))
		{
			CreateEnemy();
		}
		if (Timer(10,2))
		{
			EnemyMove(1);// 敌机移动
		}
	
		
		PlayerEnemy();

	}
	EndBatchDraw(); //结束
	return 0;
}


//通过多线程播放音乐

//DWORD WINAPI PlayMusic(LPVOID lp)
//{
//
//	mciSendString(TEXT("close gun "), 0, 0, 0);
//	mciSendString(TEXT("open D:\\images\\f_gun.mp3 alias gun"), 0, 0, 0); //像多媒体设备接口发送字符串 media device interface
//	mciSendString(TEXT("play gun "), 0, 0, 0);
//	return 0;
//
//}