#include "cstdlib"
#include "ncurses.h"
#include "iostream"

struct Display{
    int width;
    int height;
    Display(int w,int h){
        width = w;
        height = h;
    }
    Display(){
        width = 20;
        width = 20;
    }
};

enum Direction{
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};


class Game{
    public:
    Game(int width, int height){
        display = Display(width,height);     
        gameOver = false;
        dir = STOP;
        score = 0;   
    };
    void startGame(){
        Setup();
        while(!gameOver){
            updateGameScreen();
            input();
        }
        getch();
        endwin();
    }

    private:
        Display display;
        Direction dir;
        bool gameOver;
        int score;

        void Setup(){
            initscr();
            clear();
            noecho();
            cbreak();
            curs_set(0);
        }

        void updateGameScreen(){
            clear();
            for(int i = 0; i < display.width + 2; i++)
                mvprintw(0,i,"*");
            for(int i = 0; i < display.height + 2; i++){
                for(int j = 0; j < display.width + 2; j++){
                    if( i == 0 | i == display.height + 1)
                        mvprintw(i,j,"*");
                    else if( j == 0 | j == display.height + 1 )
                        mvprintw(i,j,"*");
                }
            }
            mvprintw(display.height + 3, 0, "Score %d",score);
            refresh();
        }
        void input(){
            keypad(stdscr,TRUE);
            halfdelay(1);
            int c = getch();
            switch(c){
                case KEY_LEFT:
                    dir = LEFT;
                    break;
                case KEY_UP:
                    dir = UP;
                    break;
                case KEY_DOWN:
                    dir = DOWN;
                    break;
                case KEY_RIGHT:
                    dir = RIGHT;
                    break;
                case 113: // q
                    gameOver = true;
                    break;
            }
        }
};


int main(){
    Game game(20,20);
    game.startGame();
    return 0;
}