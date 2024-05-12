#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

struct BALL{
   float x,y,xvel=0,yvel=0;
};

const int frame_rate = 30;
const float paddle_size=0.25;
const float ai_speed=15;

float enemyPaddlePos = 10;
int gameWidth, gameHeight;
int playerPos;
int newPaddleSize;

BALL ball;
POINT pt;
RECT wd;
int wdWidth, wdHeight;
int fontX, fontY;
HWND hwnd = GetConsoleWindow();

void getFontSize(){
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    if(GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo)){
      fontX=fontInfo.dwFontSize.X;
      fontY=fontInfo.dwFontSize.Y;
    }
}

void draw(){
    system("CLS");
    string out;
    string gmWidth;
    int y;
    y = playerPos;
    for(int i = 1; i<gameWidth-6; i++){
        gmWidth+=' ';
    }
    for(int i = 1; i<=wdHeight/fontY; i++){
        if(i>y-newPaddleSize/2 && i<y+newPaddleSize/2) out +='X';
        else out += ' ';
        if(i==(int)ball.y){
            for(int j = 1; j<=ball.x; j++) out+=' ';
            out += 'O';
            for(int j = ball.x+1; j<=gameWidth-8; j++) out+=' ';
        }
        else out +=gmWidth;
        if(i>enemyPaddlePos-newPaddleSize/2 && i<enemyPaddlePos+newPaddleSize/2) out +='X';
        out += '\n';
    }
    cout << out;
    cout.flush();
}

int main() {
    ball.x=100;
    ball.y=60;
    ball.xvel=-60;
    ball.yvel=ball.xvel;
    do
    {
        getFontSize();
        GetCursorPos(&pt);
        if(GetWindowRect(hwnd, &wd)) {
            wdWidth = wd.right - wd.left;
            wdHeight = wd.bottom - wd.top;
        }
        newPaddleSize = paddle_size*wdHeight/(fontY*1.0f);
  
        gameWidth=wdWidth/fontX;
        gameHeight=wdHeight/fontY;
      
        playerPos=(pt.y-wd.top)/fontY;
      
        ball.yvel = abs(ball.xvel)*wdHeight/wdWidth * ball.yvel/abs(ball.yvel)/3;
        ball.x+=ball.xvel/frame_rate;
        ball.y+=ball.yvel/frame_rate;
      
        if(ball.xvel>0){
            if(enemyPaddlePos<ball.y-newPaddleSize/4) enemyPaddlePos+=ai_speed/frame_rate;
            else if(enemyPaddlePos>ball.y+newPaddleSize/4)enemyPaddlePos-=ai_speed/frame_rate;
        }
        if(ball.y<=4){
            ball.y = 5;
            ball.yvel*=-1;
        }
        if(ball.y>=gameHeight+1){
            ball.y = gameHeight;
            ball.yvel*=-1;
        }
        if(ball.x<=0){
            if(ball.y>=(playerPos-newPaddleSize/2) && ball.y<=(playerPos+newPaddleSize/2)){
                ball.xvel*=-1;
                ball.x=1;
            }
            else{
                ball.x = gameWidth/2;
                ball.y= gameHeight/2;
                ball.xvel*=-1;
            }
        }
        if(ball.x>=gameWidth-7){
            if(ball.y>enemyPaddlePos-newPaddleSize/2 && ball.y<enemyPaddlePos+newPaddleSize/2){
                ball.xvel*=-1;
                ball.x=gameWidth-8;
            }
            else{
                ball.x=gameWidth/2;
                ball.y=gameHeight/2;
                ball.xvel*=-1;
            }
        }
        draw();
        Sleep(1000/frame_rate);
    }while(pt.x!=0 || pt.y!=0);
    system("pause");
    return 0;
}
