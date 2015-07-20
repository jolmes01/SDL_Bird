#include "SDL.h"
#include <iostream>
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

bool colision(SDL_Rect *rec1,SDL_Rect *rec2){
	int w1,h1,w2,h2,x1,y1,x2,y2;

	w1=rec1->w;
	h1=rec1->h;
	w2=rec2->w;
	h2=rec2->h;

	x1=rec1->x;
	y1=rec1->y;
	x2=rec2->x;
	y2=rec2->y;

	if (((x1+w1)>x2) && ((y1+h1)>y2) && ((x2+w2)>x1) && ((y2+h2)>y1)) {
		return true;
	} else {
	    return false;
	}

}

int 
main(int argc, char **argv)
{
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
	rectangulo_origen.x = 174;//174 azul 6 amarillo
	rectangulo_origen.y = 982;
	rectangulo_origen.w = 34;
	rectangulo_origen.h = 24;
	//Rectangulo con las coordenadas donde se pintara el pagaro en el render
	SDL_Rect rectangulo_destino;
	rectangulo_destino.x = 100;
	rectangulo_destino.y = 300;
	rectangulo_destino.w = 34;
	rectangulo_destino.h = 24;
	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);

	SDL_Rect rectangulo_origen_tubo;
	rectangulo_origen_tubo.x = 112;
	rectangulo_origen_tubo.y = 646;
	rectangulo_origen_tubo.w = 52;
	rectangulo_origen_tubo.h = 320;
	SDL_Rect rectangulo_destino_tubo;
	rectangulo_destino_tubo.x = 800;
	rectangulo_destino_tubo.y = -150;
	rectangulo_destino_tubo.w = 52;
	rectangulo_destino_tubo.h = 320;
	SDL_RenderCopy(render, textura, &rectangulo_origen_tubo, &rectangulo_destino_tubo);
	SDL_Rect rectangulo_origen_tubo1;
	rectangulo_origen_tubo1.x = 168;
	rectangulo_origen_tubo1.y = 646;
	rectangulo_origen_tubo1.w = 52;
	rectangulo_origen_tubo1.h = 320;
	SDL_Rect rectangulo_destino_tubo1;
	rectangulo_destino_tubo1.x = 800;
	rectangulo_destino_tubo1.y = 320;
	rectangulo_destino_tubo1.w = 52;
	rectangulo_destino_tubo1.h = 320;
	SDL_RenderCopy(render, textura, &rectangulo_origen_tubo1, &rectangulo_destino_tubo1);



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
		if (rectangulo_destino.y < 400) {
			rectangulo_destino.y += 10;
		}
		//validacion del angulo maximo
		if (angulo < 450) {
			angulo += 15;
		}

		if(rectangulo_destino_tubo.x > 0){
			rectangulo_destino_tubo.x -= 2;
			rectangulo_destino_tubo1.x -= 2;
		}else{
			rectangulo_destino_tubo.x = 864;
			rectangulo_destino_tubo1.x = 864;
		}

		//mientras exista un evento en el pila de eventos
		while (SDL_PollEvent(&event)) {
			//salto
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
				rectangulo_destino.y -= 100;
				angulo = 300;
			} else if (event.type == SDL_QUIT) {
				done = 0;
			}
		}
		//se pinta actualiza el render
		SDL_RenderClear(render);
		renderFondo(render, textura);
		//funciona como copy pero rota la imagen con el angulo
		SDL_RenderCopy(render, textura, &rectangulo_origen_tubo, &rectangulo_destino_tubo);
		SDL_RenderCopy(render, textura, &rectangulo_origen_tubo1, &rectangulo_destino_tubo1);
		SDL_RenderCopyEx(render, textura, &rectangulo_origen, &rectangulo_destino, angulo, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(render);

		if(colision(&rectangulo_destino,&rectangulo_destino_tubo)
			||colision(&rectangulo_destino,&rectangulo_destino_tubo1))
		{
			done = 0;
		}

		frameTime = SDL_GetTicks() - frameStart;
		//Tiempo que ha tardado en presentarse el frame
		if (frameTime < DELAY_TIME)//Si es menor al tiempo que deber Ã ­a ser
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));//Espera un tiempo
		}
	}
	angulo = 450;
	while(rectangulo_destino.y < 400) {
		frameStart = SDL_GetTicks();
		rectangulo_destino.y += 10;
		//se pinta actualiza el render
		SDL_RenderClear(render);
		renderFondo(render, textura);
		//funciona como copy pero rota la imagen con el angulo
		SDL_RenderCopy(render, textura, &rectangulo_origen_tubo, &rectangulo_destino_tubo);
		SDL_RenderCopy(render, textura, &rectangulo_origen_tubo1, &rectangulo_destino_tubo1);
		SDL_RenderCopyEx(render, textura, &rectangulo_origen, &rectangulo_destino, angulo, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(render);

		frameTime = SDL_GetTicks() - frameStart;
		//Tiempo que ha tardado en presentarse el frame
		if (frameTime < DELAY_TIME)//Si es menor al tiempo que deber Ã ­a ser
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));//Espera un tiempo
		}
	}
	SDL_Delay(5000);

	/* Free all objects */
	SDL_DestroyTexture(textura);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(ventana);

	/* Quit program */
	SDL_Quit();
	return 0;
}