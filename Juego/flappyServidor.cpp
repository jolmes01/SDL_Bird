/* compile with `gcc -lSDL2 -o main main.c` */

#include "SDL.h"
#include <iostream> //I included it since I used cout
#include "SocketDatagrama.h"
#include "data.h"

void renderFondo(SDL_Renderer * r,SDL_Texture *s)
{
    SDL_Rect s_rect; // CREATES THE IMAGE'S SPECS
    s_rect.x = 0;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
    s_rect.y = 0;
    s_rect.w = 288; //width of the texture
    s_rect.h = 510; // height of the texture

    SDL_Rect d_rect; // CREATES THE IMAGE'S SPECS
    d_rect.x = 0;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
    d_rect.y = 0;
    d_rect.w = 288; //width of the texture
    d_rect.h = 510; // height of the texture

    SDL_RenderCopy(r, s, &s_rect, &d_rect); // THE NULL IS THE AREA YOU COULD USE TO CROP THE IMAGE
    d_rect.x += 288;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
    SDL_RenderCopy(r, s, &s_rect, &d_rect);
    d_rect.x += 288;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
    SDL_RenderCopy(r, s, &s_rect, &d_rect);
}

int main(int argc, char ** argv)
{
    struct birdPackage infoReceived;
    int port;
    char serverIp[16];
    if(argc != 3)
    {
        std::cout << "USO flappy <serverIp> <port>" << std::endl;
        exit(-1);
    }
    strcpy(serverIp, argv[1]);
    port = atoi(argv[2]);
    SocketDatagrama socket(port);
    
    

SDL_Init(SDL_INIT_VIDEO);
SDL_Window * w = SDL_CreateWindow("Hi", 200, 200, 864, 510, SDL_WINDOW_SHOWN);
SDL_Renderer * r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
SDL_Surface *bmp = SDL_LoadBMP("atlas.bmp");
//SDL_Surface *bmp = IMG_Load("atlas.png");
SDL_SetColorKey(bmp, 1,SDL_MapRGB(bmp->format, 255, 255, 255));
SDL_Texture *s = SDL_CreateTextureFromSurface(r, bmp);
SDL_FreeSurface(bmp);
SDL_Event event;
if (s == NULL)
{
std::cout << "FAILED TO FIND THE IMAGE" << std::endl; //we did this to check if IMG_LoadTexture found the image, if it showed this message in the cmd window (the black system-like one) then it means that the image can't be found
}
SDL_Rect s_rect; // CREATES THE IMAGE'S SPECS
s_rect.x = 0;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
s_rect.y = 0;
s_rect.w = 288; //width of the texture
s_rect.h = 510; // height of the texture

SDL_Rect d_rect; // CREATES THE IMAGE'S SPECS
d_rect.x = 0;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
d_rect.y = 0;
d_rect.w = 288; //width of the texture
d_rect.h = 510; // height of the texture

SDL_RenderClear(r);
SDL_RenderCopy(r, s, &s_rect, &d_rect); // THE NULL IS THE AREA YOU COULD USE TO CROP THE IMAGE
d_rect.x += 288;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
SDL_RenderCopy(r, s, &s_rect, &d_rect);
d_rect.x += 288;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
SDL_RenderCopy(r, s, &s_rect, &d_rect);
s_rect.x = 5;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
s_rect.y = 982;
s_rect.w = 35; //width of the texture
s_rect.h = 25;
d_rect.x = 100;    // just like the window, the x and y values determine it's displacement from the origin which is the top left of your window
d_rect.y = 300;
d_rect.w = 35; //width of the texture
d_rect.h = 25;
SDL_RenderCopy(r, s, &s_rect, &d_rect);
SDL_RenderPresent(r);

//SDL_Delay(10000);
int done = 1;
double angulo = 330;
const int FPS = 24; //Cuantos frames por segundo queremos, 60 es el que utilizan los televisores
  const int DELAY_TIME = 1000.0f / FPS;  //1000 ms entre los fps da el numero de milisegundos entre cada frame
  Uint32 frameStart, frameTime;
while(done){
    frameStart = SDL_GetTicks();
    bzero(&infoReceived,sizeof(birdPackage));
    PaqueteDatagrama receive(sizeof(birdPackage));
    socket.recibe(receive);
    memcpy(&infoReceived, receive.obtieneDatos(), sizeof(birdPackage));
    d_rect.x = infoReceived.posicionJUMP_X;
    d_rect.y = infoReceived.posicionJUMP_Y;
    angulo = infoReceived.angulo;

    //esperar paquete con las coordenadas nuevas
    while ( SDL_PollEvent(&event) ) 
    {
        //std::cout << std::endl << std::endl<< "event"  << std::endl << std::endl;
        if(event.type == SDL_QUIT){
            done = 0;
        }
    }
    SDL_RenderClear(r);
    renderFondo(r,s);
    //std::cout << "d     "<< d_rect.x<<"       "<<d_rect.y<<"      "<<angulo<< std::endl;
    SDL_RenderCopyEx(r, s, &s_rect, &d_rect,angulo,NULL,SDL_FLIP_NONE);
    //SDL_RenderCopy(r, s, &s_rect, &d_rect);
    SDL_RenderPresent(r);
    frameTime = SDL_GetTicks() - frameStart;  //Tiempo que ha tardado en presentarse el frame
      if(frameTime< DELAY_TIME)  //Si es menor al tiempo que debería ser
      {
        //std::cout << "DELAY_TIME" << std::endl;
         SDL_Delay((int)(DELAY_TIME - frameTime)); //Espera un tiempo
      }
}

/* Free all objects*/
SDL_DestroyTexture(s);
SDL_DestroyRenderer(r);
SDL_DestroyWindow(w);

/* Quit program */
SDL_Quit();
return 0;
}