#include "cstdlib"
#include "ncurses.h"
#include "iostream"
#include "vector"
#include "list"

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

struct TIE_fighter_body{
    char body[5] = {
        '|','-','0','-','|'
    };
    int dimx = 5;
    int dimy = 1;
};

class Bullet{
    public:
        Bullet(){
            x = 0;
            y = 0;
        }
        Bullet(int _x, int _y){
            x = _x;
            y = _y;
        }
        int getPosx() const {return x;};
        int getPosy() const {return y;};
        void changePos() {y--;};
    private:
        int x;
        int y;
};

class XWing{

    public:
        XWing(){
            xPos = 0;
            shipStatus = 100;
        }
        int changeDirection(Direction dir){
            switch(dir){
                case LEFT:
                    return xPos-=2;
                    break;
                case RIGHT:
                    return xPos+=2;
                    break;
                case UP:
                    return -1;
                default:
                    return 0;
                    break;
            }
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

class TIE_fighter{
    public:
        int getPosx() const { return xPos; };
        int getPosy() const { return yPos; };
        int changePosx() { 
            if(direction < 0){
                return xPos--;
            }
            else{
                return xPos++;
            }
        };
        int changePosy() { return ++yPos ; };
        TIE_fighter(){
            xPos = 0;
            yPos = 0;
            direction = 1;
            shipStatus = rand() % 50;
            if(shipStatus >= 40)
                velocity = 1;
            else if(shipStatus < 40 && shipStatus >= 20)
                velocity = 2;
            else
                velocity = 3;
        }
        TIE_fighter(int _x, int _y){
            xPos = _x;
            yPos = _y;
            direction = 1;
            shipStatus = rand() % 50;
            if(shipStatus >= 40)
                velocity = 1;
            else if(shipStatus < 40 && shipStatus >= 20)
                velocity = 2;
            else
                velocity = 3;
        }
        void draw(int _x, int _y){
            for(int i = 0; i < 5; i++){
                char *a = new char(body.body[i]);
                mvprintw(_x,_y+i,a);
                free(a);
            }
        }
        void changeDirection(){
            direction = direction * (-1);
        };
    private:
        int xPos;
        int yPos;
        int shipStatus;
        int velocity;
        int direction;
        TIE_fighter_body body;
};

class Game{
    public:
        Game(int width, int height){
            display = Display(width,height);     
            gameOver = false;
            dir = STOP;
            score = 0;
            enemies.push_back(TIE_fighter(display.width / 2,2));   
        };
        void startGame(){
            Setup();
            while(!gameOver){
                updateGameScreen();
                input();
                int shoot = xwing.changeDirection(dir);
                if (shoot == -1){
                    // Crear bala en esta pos
                    bullets.push_back(Bullet(xwing.getPos(),display.height - xwing.sizeY()));
                }
                changeEnemiesPosition();
                checkCollitions();
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
        std::vector<Bullet> bullets;
        std::list<TIE_fighter> enemies;

        void Setup(){
            initscr();
            start_color();
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            clear();
            noecho();
            cbreak();
            curs_set(0);
        }

        void checkCollitions(){
            // colisiones entre enemigos-balas
            for(int i = 0; i < bullets.size(); i++){
                // for(int j = 0; j < enemies.size(); j++){
                    // if(bullets[i].getPosx() >= enemies[j].getPosx() && 
                    // bullets[i].getPosx() <= enemies[j].getPosx() + 5 &&
                    // bullets[i].getPosy() == enemies[j].getPosy()){
                    //     // enemies.pop_back();    
                    // }
                // }
                std::list<TIE_fighter>::iterator j = enemies.begin();
                for(TIE_fighter &tie : enemies){
                    if(bullets[i].getPosx() >= tie.getPosx() && 
                    bullets[i].getPosx() <= tie.getPosx() + 5 &&
                    bullets[i].getPosy() == tie.getPosy()){
                        enemies.erase(j);
                    }
                    std::advance(j,1);
                }
            }
        }

        void changeEnemiesPosition(){
            for(TIE_fighter &tie : enemies){
                if(tie.getPosx() > 60){
                    tie.changeDirection();
                    tie.changePosy();
                }else if(tie.getPosx() < 3){
                    tie.changeDirection();
                    tie.changePosy();
                }
                tie.changePosx();
            }
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
                        xwing.draw(display.height - xwing.sizeY() + 2);
                    }

                    if(enemies.size() > 0){
                        for(TIE_fighter &tie : enemies){
                            if(j == tie.getPosx() && i == tie.getPosy()){
                                tie.draw(i,j);
                            }   
                        }
                    }

                    if(bullets.size() > 0){
                        for(int k = 0; k < bullets.size(); k ++){
                            if(i == bullets[k].getPosy() && j == bullets[k].getPosx()){
                                mvprintw(i,j,"^");
                                bullets[k].changePos();
                                if(bullets[k].getPosy() == 0){
                                    bullets.pop_back();
                                }
                            }
                        }
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
                default:
                    dir = STOP;
            }
        }
};


int main(){
    Game game(70,20);
    game.startGame();
    return 0;
}