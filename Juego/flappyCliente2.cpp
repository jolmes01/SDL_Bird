#include "SDL.h"
#include <iostream>
#include "data.h"
#include "SocketDatagrama.h"

const int FPS = 1390; 
const int DELAY_TIME = 1000.0f / FPS;  
Game* g_game = 0;

 SDL_Point window_position = {//    Posición de la ventana
 	SDL_WINDOWPOS_CENTERED,
 	SDL_WINDOWPOS_CENTERED
 };

int main(int argc, char **argv)
{
	/*Invocacion a la clase Game*/
	g_game = new Game();
	g_game->init("---> Flappy <---", , 200, 200, 864, 510, SDL_WINDOW_SHOWN);

	
		***g_game->handleEvents();
		***g_game->update();
		***g_game->render();
		

    Uint32 frameStart, frameTime;
	while(g_game->running()) {
		frameStart = SDL_GetTicks();
		//validacion del piso
		if (rectangulo_destino[nJugador].y < 500) {
			rectangulo_destino[nJugador].y += 10;
		}
		//validacion del angulo maximo
		if (angulo < 450) {
			angulo += 15;
		}
		//mientras exista un evento en el pila de eventos
			while (SDL_PollEvent(&event)) {
			//salto
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
				rectangulo_destino[nJugador].y -= 100;
				angulo = 300;
				infoToSend.opcode = 1;
			} else if (event.type == SDL_QUIT) {
				infoToSend.opcode = CLOSE;
				done = 0;
			}
		}
		//enviar Paquete con las coordenadas
		infoToSend.jugadorNum = nJugador;
		infoToSend.posicionJUMP_X[nJugador] = rectangulo_destino[nJugador].x;
		infoToSend.posicionJUMP_Y[nJugador] = rectangulo_destino[nJugador].y;
		infoToSend.angulo[nJugador] = angulo;
		PaqueteDatagrama paq((char *)&infoToSend, sizeof(birdPackage), serverIp, port);
		socket.envia(paq);
		//Recibir paquete con las coordenadas actualizadas
		PaqueteDatagrama receive(sizeof(birdPackage));
    	socket.recibe(receive);
    	memcpy(&infoReceived, receive.obtieneDatos(), sizeof(birdPackage));
		//se pinta actualiza el render
		SDL_RenderClear(render);
		renderFondo(render, textura);

		int i = 0;
		//Actualiza la posicion de los pajaros
		for (int i = 0; i < 3; ++i)
		{
			rectangulo_destino[i].x = infoReceived.posicionJUMP_X[i];
			rectangulo_destino[i].y = infoReceived.posicionJUMP_Y[i];
			angulos[i] = infoReceived.angulo[i];

		}
		for (i = 0; i < infoReceived.jugadoresTotales; ++i)
		{
			//funciona como copy pero rota la imagen con el angulo
			SDL_RenderCopyEx(render, textura, &rectangulo_origen[i], &rectangulo_destino[i], infoReceived.angulo[i], NULL, SDL_FLIP_NONE);
		}
		SDL_RenderPresent(render);

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
