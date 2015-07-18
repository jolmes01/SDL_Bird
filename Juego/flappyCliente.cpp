#include "SDL.h"
#include <iostream>
#include "data.h"
#include "SocketDatagrama.h"

// se encarga de copiar 3 veces el fondo y presentarlo consecutivamente
void 
renderFondo(SDL_Renderer * render, SDL_Texture * textura)
{
	SDL_Rect	rectangulo_origen;
	rectangulo_origen.x = 0;
	rectangulo_origen.y = 0;
	rectangulo_origen.w = 288;
	rectangulo_origen.h = 510;

	SDL_Rect	rectangulo_destino;
	rectangulo_destino.x = 0;
	rectangulo_destino.y = 0;
	rectangulo_destino.w = 288;
	rectangulo_destino.h = 510;

	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
	rectangulo_destino.x += 288;
	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
	rectangulo_destino.x += 288;
	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
}

int main(int argc, char **argv)
{
	int		port;
	char	serverIp  [16];
	if (argc != 3) {
        std::cout << "USO flappy <serverIp> <port>" << std::endl;
		exit(-1);
	}

	birdPackage infoToSend;
	bzero(&infoToSend,sizeof(birdPackage));
	SocketDatagrama	socket;

	strcpy(serverIp, argv[1]);
	port = atoi(argv[2]);

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window     *ventana = SDL_CreateWindow("Hi", 200, 200, 864, 510, SDL_WINDOW_SHOWN);
	SDL_Renderer   *render = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
	//se carga la imagen que contiene todos los graficos
	SDL_Surface * bmp = SDL_LoadBMP("atlas.bmp");
	//se define el blanco como color transparente
	SDL_SetColorKey(bmp, 1, SDL_MapRGB(bmp->format, 255, 255, 255));
	SDL_Texture    *textura = SDL_CreateTextureFromSurface(render, bmp);
	SDL_FreeSurface(bmp);
	SDL_Event	event;
	if (textura == NULL) {
        std::cout << "FAILED TO FIND THE IMAGE" << std::endl;
	}
	SDL_RenderClear(render);
	renderFondo(render, textura);
	//Rectangulo con las coordenadas donde se encuentra el pagaro en la textura
	SDL_Rect rectangulo_origen;
	rectangulo_origen.x = 5;
	rectangulo_origen.y = 982;
	rectangulo_origen.w = 35;
	rectangulo_origen.h = 25;
	//Rectangulo con las coordenadas donde se pintara el pagaro en el render
	SDL_Rect rectangulo_destino;
	rectangulo_destino.x = 100;
	rectangulo_destino.y = 300;
	rectangulo_destino.w = 35;
	rectangulo_destino.h = 25;
	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
	SDL_RenderPresent(render);


	int		done = 1;
	double		angulo = 330;
	const int	FPS = 24;
	//Cuantos frames por segundo queremos, 60 es el que utilizan los televisores
    const int	DELAY_TIME = 1000.0f / FPS;
	//1000 ms entre los fps da el numero de milisegundos entre cada frame
    Uint32 frameStart, frameTime;
	while (done) {
		frameStart = SDL_GetTicks();

		//validacion del piso
			if (rectangulo_destino.y < 500) {
			rectangulo_destino.y += 10;
		}
		//validacion del angulo maximo
			if (angulo < 450) {
			angulo += 15;
		}
		//mientras exista un evento en el pila de eventos
			while (SDL_PollEvent(&event)) {
			//salto
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
				rectangulo_destino.y -= 100;
				angulo = 300;
				infoToSend.opcode = 1;
			} else if (event.type == SDL_QUIT) {
				done = 0;
			}
		}
		//enviar Paquete con las coordenadas
		infoToSend.posicionJUMP_X = rectangulo_destino.x;
		infoToSend.posicionJUMP_Y = rectangulo_destino.y;
		infoToSend.angulo = angulo;
		PaqueteDatagrama paq((char *)&infoToSend, sizeof(birdPackage), serverIp, port);
		socket.envia(paq);
		//se pinta actualiza el render
		SDL_RenderClear(render);
		renderFondo(render, textura);
		//funciona como copy pero rota la imagen con el angulo
		SDL_RenderCopyEx(render, textura, &rectangulo_origen, &rectangulo_destino, angulo, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(render);

		frameTime = SDL_GetTicks() - frameStart;
		//Tiempo que ha tardado en presentarse el frame
		if (frameTime < DELAY_TIME)//Si es menor al tiempo que deber Ã ­a ser
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));//Espera un tiempo
		}
	}

	/* Free all objects */
	SDL_DestroyTexture(textura);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(ventana);

	/* Quit program */
	SDL_Quit();
	return 0;
}