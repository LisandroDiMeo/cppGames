#include "cstdlib"
#include "ncurses.h"
#include "iostream"

enum Direction{
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Tail{
    int tailX[100];
    int tailY[100];
};

struct Head{
    int x;
    int y;
};

class Snake{
    public:
        Snake(int posX, int posY){
            head.x = posX;
            head.y = posY;
        }
        Snake(){
            head.x = 0;
            head.y = 0;
        }
        void changeDirection(Direction dir){

            int prevX = tail.tailX[0];
            int prevY = tail.tailY[0];
            int prev2x,prev2y;
            tail.tailX[0] = head.x;
            tail.tailY[0] = head.y;
            for(int i = 1; i < tailLength; i++){
                prev2x = tail.tailX[i];
                prev2y = tail.tailY[i];
                tail.tailX[i] = prevX;
                tail.tailY[i] = prevY;
                prevX = prev2x;
                prevY = prev2y;
            }

            switch(dir){
                case LEFT:
                    head.x--;
                    break;
                case RIGHT:
                    head.x++;
                    break;
                case UP:
                    head.y--;
                    break;
                case DOWN:
                    head.y++;
                    break;
                default:
                    break;
            }
        }
        int getTailLength() const{
            return tailLength;
        }
        int increaseTailLength(){
            tailLength++;
            return tailLength;    
        }
        int getHeadx() const{
            return head.x;
        }
        int getHeady() const{
            return head.y;
        }
        int getTailAtx(int at) const{
            return tail.tailX[at];
        }
        int getTailAty(int at) const{
            return tail.tailY[at];
        }
    private:
        Tail tail;
        Head head;
        int tailLength;
};

struct Display{
    int width;
    int height;
};

struct Fruit{
    int fruitX;
    int fruitY;
    void generateNewFruit(int w, int h){
        fruitX = rand()%w + 1;
        fruitY = rand()%h + 1;
    }
};

class Game{
    public:
    Snake snake;
    Display display;
    Fruit fruit;
    int score;
    bool gameOver;
    Direction eDir;

    Game(int width, int height){
        gameOver = false;
        eDir = STOP;
        display.height = height;
        display.width = width;
        snake = Snake(display.width / 2, display.height / 2);
        fruit.fruitX = rand()%display.width + 1;
        fruit.fruitY = rand()%display.height + 1;
        score = 0;
    }

    void Setup(){
        initscr();
        clear();
        noecho();
        cbreak();
        curs_set(0);
    }

    void checkCollitions(){
        if(snake.getHeadx() > display.width || 
        snake.getHeadx() < 1 || 
        snake.getHeady() > display.height ||
        snake.getHeady() < 1)
            gameOver = true;
        if(snake.getHeadx() == fruit.fruitX && snake.getHeady() == fruit.fruitY){
            score+=1;
            fruit.generateNewFruit(display.width,display.height);
            snake.increaseTailLength();
        }
        for(int i = 0; i < snake.getTailLength(); i++)
            if(snake.getHeadx() == snake.getTailAtx(i) && snake.getHeady() == snake.getTailAty(i))
                gameOver = true;
    }

    void startGame(){
        Setup();
        while(!gameOver){
            updateGameScren();
            input();
            snake.changeDirection(eDir);
            checkCollitions();
        }
        getch();
        endwin();
    }

    void updateGameScren(){
        clear();
        for(int i = 0; i < display.width + 2; i++)
            mvprintw(0,i,"*");
        for(int i = 0; i < display.height + 2; i++){
            for(int j = 0; j < display.width + 2; j++){
                if( i == 0 | i == display.height + 1)
                    mvprintw(i,j,"*");
                else if( j == 0 | j == display.height + 1 )
                    mvprintw(i,j,"*");
                else if( i == snake.getHeady() && j == snake.getHeadx())
                    mvprintw(i,j,"O");
                else if(fruit.fruitX == j && fruit.fruitY == i)
                    mvprintw(i,j,"@");
                else
                    for(int k = 0; k < snake.getTailLength(); k++){
                        if(snake.getTailAtx(k) == j && snake.getTailAty(k) == i)
                            mvprintw(i,j,"o");
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
                eDir = LEFT;
                break;
            case KEY_UP:
                eDir = UP;
                break;
            case KEY_DOWN:
                eDir = DOWN;
                break;
            case KEY_RIGHT:
                eDir = RIGHT;
                break;
            case 113: // q
                gameOver = true;
                break;
        }
    }
};

using namespace std;

int main(){
    Game juego(20,20);
    juego.startGame();
    return 0;
}