#include "SDL.h"
#include <iostream>
#include "data.h"
#include "SocketDatagrama.h"

/* Global variables */
int posX, posY, nJugador = 0;
SDL_Rect rectangulo_origen[3]; //Variables para los pajaros en el atlas.bmp
SDL_Rect rectangulo_destino[3]; //Variables para la posicion en pantalla de los pajaros

/* Global functions */
void renderFondo(SDL_Renderer *, SDL_Texture *);
void initPajarosOrigen(SDL_Renderer *, SDL_Texture *);

int main(int argc, char **argv)
{
	struct birdPackage infoToSend, infoReceived;
	int	port;
	char serverIp  [16];
	if (argc != 3) {
        std::cout << "USO flappy <serverIp> <port>" << std::endl;
		exit(-1);
	}

	strcpy(serverIp, argv[1]);
	port = atoi(argv[2]);

	SocketDatagrama	socket;

	std::cout << "Enviando al servidor: " << serverIp << " : " << port << " datos iniciados" << std::endl;

	bzero(&infoToSend,sizeof(birdPackage));
	bzero(&infoReceived,sizeof(birdPackage));

	/* CONEXION POR PRIMERA VEZ */
	infoToSend.opcode = NEW;

	std::cout << "OPCODE: " << infoToSend.opcode << std::endl;

	PaqueteDatagrama paq((char *)&infoToSend, sizeof(birdPackage), serverIp, port);
	socket.envia(paq);

    PaqueteDatagrama receive(sizeof(birdPackage));
    socket.recibe(receive);

    memcpy(&infoReceived, receive.obtieneDatos(), sizeof(birdPackage));

	if(infoReceived.opcode == DENY) //Se rechaza al jugador, por lo que se tiene que salir
	{
		std::cout << "Partida llena intente más tarde..." << std::endl;
		exit(-1);
	}else{
		nJugador = infoReceived.jugadorNum;
		infoToSend.opcode = JUMP;
		std::cout << "Partida iniciada, jugador: " << nJugador << std::endl;
		for (int i = 0; i < 3; ++i)
		{
			cout << "X: " << infoReceived.posicionJUMP_X[i] << "Y: " << infoReceived.posicionJUMP_Y[i] << "\n";
		}
	}

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

	//Inicializa la posición de los pajaros dentro de la pantalla, pasandolos del atlas.bmp al recuadro del juego
	initPajarosOrigen(render, textura);
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
				done = 0;
			}
		}
		//enviar Paquete con las coordenadas
		infoToSend.posicionJUMP_X[nJugador] = rectangulo_destino[nJugador].x;
		infoToSend.posicionJUMP_Y[nJugador] = rectangulo_destino[nJugador].y;
		infoToSend.angulo = angulo;
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
			rectangulo_destino[i].x = infoReceived.posicionJUMP_X[nJugador];
			rectangulo_destino[i].y = infoReceived.posicionJUMP_Y[nJugador];
		}
		for (i = 0; i < infoReceived.jugadoresTotales; ++i)
		{
			//funciona como copy pero rota la imagen con el angulo
			SDL_RenderCopyEx(render, textura, &rectangulo_origen[i], &rectangulo_destino[i], angulo, NULL, SDL_FLIP_NONE);
		}
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

// se encarga de copiar 3 veces el fondo y presentarlo consecutivamente
void renderFondo(SDL_Renderer * render, SDL_Texture * textura)
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

void initPajarosOrigen(SDL_Renderer * render, SDL_Texture * textura)
{
	int xPos[] = {5,175,228};
	int yPos[] = {982,866};
	int w = 35, h = 25, i = 0;
	for (i = 0; i < 3; ++i)
	{
		//Rectangulo con las coordenadas donde se encuentra el pagaro en la textura
		rectangulo_origen[i].x = xPos[i];
		if(i == 2) rectangulo_origen[i].y = yPos[1];
		else rectangulo_origen[i].y = yPos[0];
		rectangulo_origen[i].w = w;
		rectangulo_origen[i].h = h;
		//Rectangulo con las coordenadas donde se encuentra el pagaro en la textura
		rectangulo_destino[i].x = 100 * (i+1);
		rectangulo_destino[i].y = 300;
		rectangulo_destino[i].w = w;
		rectangulo_destino[i].h = h;
		SDL_RenderCopy(render, textura, &rectangulo_origen[i], &rectangulo_destino[i]);
	}
}