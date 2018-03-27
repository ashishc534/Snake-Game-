#include<bits/stdc++.h>
#include <unistd.h>
#include<stdio.h>
#include <sys/select.h>
// #include <curses.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

#define Two_dcin(arr,n) for(int i=0;i<n;i++){for(int j=0;j<n;j++)cin>>arr[i][j];}
#define Two_dcout(arr,n) for(int i=0;i<n;i++){for(int j=0;j<n;j++)cout<<arr[i][j]<<" "; cout<<endl;}
using namespace std;

bool gameOver;
const int width = 70,height = 20;
int snakeX,snakeY,fruitX,fruitY,score;

enum edir{STOP = 0,LEFT,RIGHT,UP,DOWN};
edir dir;

int tailX[2000],tailY[2000];
int tail_length;

static struct termios initial_settings, new_settings;

static int peek_character = -1;

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int _getch()
{
    char ch;

    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}


int _kbhit()
{
    unsigned char ch;
    int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}

void setup()
{
	gameOver = false;
	snakeX = width/2;
	snakeY = height/2;
	
	fruitX = rand()%width;
	fruitY = rand()%height;
}

void draw()
{
	system("clear");
	for(int i=0;i<width;i++)
		cout<<"#";

	cout<<endl;

	for(int i=0;i<tail_length;i++)
	{
		if(fruitX == tailX[i] && fruitY == tailY[i])
		{
			if((fruitX + 1) < (width - 1))
				fruitX += 1;

			else if((fruitY + 1) < (height - 1))
				fruitY += 1;
		}
	}

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(j == 0 || j == width-1)
				cout<<"#";

			if(i == snakeY && j == snakeX)
				cout<<"O";

			else if(i == fruitY && j == fruitX)
				cout<<"F";

			else
			{
				bool print = false;
				for(int k = 0;k < tail_length; k++)
				{
					if(tailX[k] == j && tailY[k] == i)
					{
						cout<<"o";
						print = true;
					}
				}
				if(!print)
				cout<<" ";
			}
		}
		cout<<endl;
	}
	for(int i=0;i<width;i++)
		cout<<"#";

	cout<<endl;
	cout<<"SCORE : "<<score<<endl;
}

void logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X,prev2Y;


	tailX[0] = snakeX;
	tailY[0] = snakeY;

	for(int i=1;i<tail_length;i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];

		tailX[i] = prevX;
		tailY[i] = prevY;

		prevX = prev2X;
		prevY = prev2Y;
	}
	switch(dir)
	{
		case LEFT:
				snakeX--;
				break;

		case RIGHT:
				snakeX++;
				break;

		case UP:
				snakeY--;
				break;

		case DOWN:
				snakeY++;
				break;

		default:
				break;
	}

	if(snakeX > width || snakeX < 0 || snakeY > height || snakeY < 0)
		gameOver = true;
	
	for(int i=0;i<tail_length;i++)
	{
		if(snakeX == tailX[i] && snakeY == tailY[i])
		{
			gameOver = true;
			break;
		}
	}

	if(snakeX == fruitX && snakeY == fruitY)
	{
		score += 10;

		srand(time(NULL));
		fruitX = rand()%width;
		fruitY = rand()%height;

		tail_length++;
	}

}


void MoveToFruit()
{

	if(fruitX < snakeX)
		dir = LEFT;
	

	else if(fruitX > snakeX)
		dir = RIGHT;

	else if(fruitX == snakeX)
	{
		if(fruitY < snakeY)
			dir = UP;

		else dir = DOWN;
	}


	// for(int i=0;i<tail_length;i++)
	// {
	// 	if(snakeX + 1 == tailX[i] || snakeX - 1 == tailX[i])
	// 	{
	// 		if(snakeY > height - snakeY)
	// 			dir = UP;

	// 		else
	// 			dir = DOWN;
	// 	}	

	// 	// if(snakeY + 1 == tail[i] || snakeY - 1 == tail[i])
	// 	// {

	// 	// }
	// }
}

int main()
{
	init_keyboard();
	setup();

	
	
	while(!gameOver)
	{
		
		draw();
		MoveToFruit();
		//input();
		logic();
		usleep(100000);
		// delay(20);
	}

	return 0;
}