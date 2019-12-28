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
    Tail tail;
    Head head;
    int tailLength;
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
    // int getTailLength() const{
    //     return tailLength;
    // }
    // int increaseTailLength(){
    //     tailLength++;
    //     return tailLength;    
    // }
    // int getHeadx(){
    //     return head.x;
    // }
    // int getHeady(){
    //     return head.y;
    // }
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
        this->gameOver = false;
        this->eDir = STOP;
        this->display.height = height;
        this->display.width = width;
        this->snake.head.x = this->display.width / 2;
        this->snake.head.y = this->display.height / 2;
        fruit.fruitX = rand()%this->display.width + 1;
        fruit.fruitY = rand()%this->display.height + 1;
        this->score = 0;
    }

    void Setup(){
        initscr();
        clear();
        noecho();
        cbreak();
        curs_set(0);
    }

    void checkCollitions(){
        if(snake.head.x > display.width || 
        snake.head.x < 1 || 
        snake.head.y > display.height ||
        snake.head.y < 1)
            gameOver = true;
        if(snake.head.x == fruit.fruitX && snake.head.y == fruit.fruitY){
            score+=1;
            fruit.generateNewFruit(display.width,display.height);
            snake.tailLength++;
        }
        for(int i = 0; i < snake.tailLength; i++)
            if(snake.head.x == snake.tail.tailX[i] && snake.head.y == snake.tail.tailY[i])
                gameOver = true;
    }

    void startGame(){
        this->Setup();
        while(!this->gameOver){
            this->updateGameScren();
            this->input();
            this->snake.changeDirection(this->eDir);
            this->checkCollitions();
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
                else if( i == snake.head.y && j == snake.head.x)
                    mvprintw(i,j,"O");
                else if(fruit.fruitX == j && fruit.fruitY == i)
                    mvprintw(i,j,"@");
                else
                    for(int k = 0; k < snake.tailLength; k++){
                        if(snake.tail.tailX[k] == j && snake.tail.tailY[k] == i)
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
                this->eDir = LEFT;
                break;
            case KEY_UP:
                this->eDir = UP;
                break;
            case KEY_DOWN:
                this->eDir = DOWN;
                break;
            case KEY_RIGHT:
                this->eDir = RIGHT;
                break;
            case 113: // q
                this->gameOver = true;
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