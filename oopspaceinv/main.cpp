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

struct XWing_body{

    char body[18] = {
        '|',' ',' ',' ',' ','|',
        '|','_','/','\\','_','|',
        '|',' ','\\','/',' ','|'
        };
    int dimx = 3;
    int dimy = 6;
};


class XWing{

    public:
        XWing(){
            xPos = 0;
            shipStatus = 100;
        }
        void changeDirection(Direction dir){
            switch(dir){
                case LEFT:
                    xPos--;
                    break;
                case RIGHT:
                    xPos++;
                    break;
                case UP:
                    Shoot();
                default:
                    break;
            }
        }

        void Shoot(){
            
        }

        void draw(int d){
            for(int i = 0; i < xwing.dimx * xwing.dimy; i++){
                if (i % 6 == 0) d++;
                char *a = new char(xwing.body[i]);
                mvprintw(d,1 + (i % 6) + xPos,a);
                free(a);
            }
        }
        int sizeX() const { return xwing.dimx; };
        int sizeY() const { return xwing.dimy; };
        int getPos() const { return xPos; };
    private:
        int xPos;
        int shipStatus;
        XWing_body xwing;

        
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
            xwing.changeDirection(dir);
        }
        getch();
        endwin();
    }

    private:
        Display display;
        Direction dir;
        XWing xwing;
        bool gameOver;
        int score;

        void Setup(){
            initscr();
            start_color();
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            clear();
            noecho();
            cbreak();
            curs_set(0);
        }

        void updateGameScreen(){
            clear();
            for(int i = 0; i < display.width + 2; i++){
                attron(COLOR_PAIR(1));
                mvprintw(0,i,"*");
                refresh();
            }
                
            for(int i = 0; i < display.height + 2; i++){
                for(int j = 0; j < display.width + 2; j++){
                    if( i == 0 | i == display.height + 1){
                        mvprintw(i,j,"*");
                    }
                        
                    else if( j == 0 | j == display.width + 1 ){
                        mvprintw(i,j,"*");
                    }
                        
                    else if(i == display.height - xwing.sizeY() &&  j == 3 ){
                        // Fijarse en donde dibujar la nave
                        xwing.draw(display.height - xwing.sizeY() + 2);
                    }
                        
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
    Game game(70,20);
    game.startGame();
    return 0;
}