#include<bits/stdc++.h>
#include <unistd.h>
#include<stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

bool gameOver;
const int width = 70,height = 20;
int snakeX,snakeY,fruitX,fruitY,score;

enum edir {STOP = 0,LEFT,RIGHT,UP,DOWN};
edir dir;

int tailX[200],tailY[200];
int tail_length;

static struct termios initial_settings, new_settings;

static int peek_character = -1;

//For Linux
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

// For Linux
void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

//For Linux
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

//For Linux
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
		cout<<"ꟷ";

	cout<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(j == 0 || j == width-1)
				cout<<"ⅼ";

			if(i == snakeY && j == snakeX)
				cout<<"Ȫ";

			else if(i == fruitY && j == fruitX)
				cout<<"ṑ";

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
		cout<<"ꟷ";

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
		fruitX = rand()%width;
		fruitY = rand()%height;
		tail_length++;
	}

	if(snakeX >= width-1 || snakeX < 0 || snakeY >= height || snakeY < 0)
	{
		gameOver = true;
	}
	
	

}

void input()
{

	//Windows User must Delete the _kbhit() and _getch() functions.
	//And add conio.h header file.

	if(_kbhit())
	{
		switch(_getch())
		{
			case 'a' : 
				dir = LEFT;
				break;

			case 'w' : 
				dir = UP;
				break;
			
			case 's' : 
				dir = DOWN;
				break;
			
			case 'd' : 
				dir = RIGHT;
				break;
			
			case 'x' : 
				gameOver = true;
				break;						
		}
	}
}

int main()
{
	//For Linux
	init_keyboard();
	setup();

	while(!gameOver)
	{
		
		draw();
		input();
		logic();
		usleep(100000);
	}

	// For Linux
	close_keyboard();
	return 0;
}