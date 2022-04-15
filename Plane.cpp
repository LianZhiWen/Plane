#include<stdio.h>
#include<graphics.h> //ͼ��� 
#include<mmsystem.h>  //win32 ��ý���豸�ӿ�
#include<time.h>

#pragma comment(lib,"winmm.lib")   //��̬��

#define WIN_WIDTH 591  //�����
#define WIN_HEIGHT 864
#define BULLET_NUM 15 //���һ���Է���15
#define ENEMY_NUM 10 //���л�
/*
int a ;
char b
*/
struct Image
{
	IMAGE backGround; //����ͼƬ
	IMAGE Player[2];
	IMAGE Bullet[2];
	IMAGE Enemy[4];


}image;
//�ṹ�����ֵ�  �ṹ�� ö�� ����
enum TYPE
{
	SAMLL,
	BIG,
};

struct  Plane //�� �л� �ӵ�
{
	int x;
	int y;
	bool flag; //�Ƿ���
	int width;
	int height;
	union  //����һ���ڴ� 
	{
		int type; //�л���������
		int score; //��Ҷ�������
	}un;
	int hp; //Ѫ��


}player,bullet[BULLET_NUM],enemy[ENEMY_NUM];

DWORD t1,t2,t3,t4;
DWORD WINAPI PlayMusic(LPVOID lp);

//��ģ�鴦�� ר�ŵ�����ר�ŵĺ���
void EnemyHp(int i)
{
	int flag = rand() % 10;
	if (flag>= 0&&flag<=2) //ʮ��֮���ʲ���һ����ɻ�
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

//����ͼƬ


void GameInit()  //��ʼ������
{
	//�������������
	srand(GetTickCount());

	t1 = t2 = t3 = t4 = GetTickCount();//��ȡϵͳ��������ǰ�������ĺ�����
	//��������
	//mciSendString(TEXT("open D:\\images\\game_music.mp3 alias BGM"),0,0,0); //���ý���豸�ӿڷ����ַ��� media device interface
	//mciSendString(TEXT("play BGM repeat"),0,0,0);
	
	
	
	
	//���ر���
	loadimage(&image.backGround,_T("D:\\images\\background.jpg")); //��ͼƬ���ز�����
	loadimage(&image.Player[0], _T("D:\\images\\planeNormal_1.jpg"));
	loadimage(&image.Player[1], _T("D:\\images\\planeNormal_2.jpg"));
	loadimage(&image.Bullet[0], _T("D:\\images\\bullet1.jpg"));
	loadimage(&image.Bullet[1], _T("D:\\images\\bullet2.jpg"));
	loadimage(&image.Enemy[0], _T("D:\\images\\enemy_1.jpg"));
	loadimage(&image.Enemy[1], _T("D:\\images\\enemy_2.jpg"));
	loadimage(&image.Enemy[2], _T("D:\\images\\enemyPlane1.jpg"));
	loadimage(&image.Enemy[3], _T("D:\\images\\enemyPlane2.jpg"));
	//��ʼ���������

	player.x = WIN_WIDTH/2;
	player.y = WIN_HEIGHT - 220;
	player.flag = true;
	player.hp = 999;
	player.un.score = 0;

	//��ʼ���ӵ�����
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;

	}
	
	//��ʼ���л�
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
	putimage(0, 0, &image.backGround);//���Ʊ���
	//���Ʒɻ�
	putimage(player.x, player.y, &image.Player[0], NOTSRCERASE); //�ɻ�
	putimage(player.x, player.y, &image.Player[1], SRCINVERT);  //͸��λͼ  �������ɻ����д���
	//�����ӵ�
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
		if (bullet[i].flag==false) //�ж��ӵ��Ƿ���
		{
			bullet[i].flag = true;
			bullet[i].x = player.x+45;//�м䷢��
			bullet[i].y = player.y;
			break;

		}
	}

}

void BulletMove(int speed) //�ӵ��ƶ�
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

void CreateEnemy()  //�����л�
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag==0)
		{
			enemy[i].flag = true;
			enemy[i].x = rand() % (WIN_WIDTH-60);
			enemy[i].y = 0;        //�л���ʼ״̬
			EnemyHp(i);
			break;
		}
	}
}
void EnemyMove(int speed) //�л��ƶ�
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
	//�����ƶ� ����
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
	//�����ӵ�
	if (GetAsyncKeyState(VK_SPACE)&&Timer(300,1))
	{
	
		//HANDLE threadID = CreateThread(NULL, 0, PlayMusic , NULL , 0, 0);//����һ���߳�
		//CloseHandle(threadID);

		CreateBullet();
	
	}

	

}

void PlayerEnemy()
{ 
	//1.�����л����� �ж��Ƿ���
	//2.�����ӵ����� 
	//3.�ж��ӵ��Ƿ���ел�
	//4.�ﵽ�ˣ����Ըı� �� Ѫ��-- ���״̬ ��ҷ��� ++
	//5.���Ѫ��Ϊ0 �� �л�����
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
	initgraph(WIN_WIDTH,WIN_HEIGHT/*,SHOWCONSOLE*/);  //����һ��ͼ�δ���
	GameInit();
	BeginBatchDraw(); //����˫�����ͼ�������ڴ��ﻭ�ã�����ʵ���㿴


	while (1)
	{
		Gamedraw();
		FlushBatchDraw();//���뿴��
		GameControl(1); //speed�ƶ�����
		BulletMove(2);//�ӵ��ƶ������ٶ�

		if (Timer(500,0))
		{
			CreateEnemy();
		}
		if (Timer(10,2))
		{
			EnemyMove(1);// �л��ƶ�
		}
	
		
		PlayerEnemy();

	}
	EndBatchDraw(); //����
	return 0;
}


//ͨ�����̲߳�������

//DWORD WINAPI PlayMusic(LPVOID lp)
//{
//
//	mciSendString(TEXT("close gun "), 0, 0, 0);
//	mciSendString(TEXT("open D:\\images\\f_gun.mp3 alias gun"), 0, 0, 0); //���ý���豸�ӿڷ����ַ��� media device interface
//	mciSendString(TEXT("play gun "), 0, 0, 0);
//	return 0;
//
//}