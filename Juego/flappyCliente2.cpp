#include "SDL.h"
#include "Game.h"
#include "SocketDatagrama.h"
#include "PaqueteDatagrama.h"

const int FPS = 20;
const int DELAY_TIME = 1000.0f / FPS;
Game* g_game = 0;

 SDL_Point window_position = {//    Posición de la ventana
 	SDL_WINDOWPOS_CENTERED,
 	SDL_WINDOWPOS_CENTERED
 };

int main(int argc, char **argv)
{
	/*Invocacion a la clase Game*/
	g_game = new Game(argc, argv);
	g_game->init("---> Flappy <---", 200, 200, 864, 510);		

    Uint32 frameStart, frameTime;
	while(g_game->running()) {
		frameStart = SDL_GetTicks();
		
		g_game->handleEvents();
		g_game->update();
		g_game->render();

		frameTime = SDL_GetTicks() - frameStart;
		//Tiempo que ha tardado en presentarse el frame
		if (frameTime < DELAY_TIME)//Si es menor al tiempo que deber Ã ­a ser
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));//Espera un tiempo
		}
	}
	g_game->clean();
	return 0;
}
