// 	Game.h
/*
	Estructura de la clase Game. 
	Clase que nos permite crear y manejar los gráficos de nuestro programa.
*/

#ifndef __Game__
#define __Game__

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include "data.h"
#include "SocketDatagrama.h"
#include "PaqueteDatagrama.h"

class Game
{
public:
	Game(int, char **);
	~Game();
	void init(const char* , int , int, int, int);
	void render();
	void update();
	void handleEvents();
	void clean();
	bool running();
	void renderFondo();
	void initPajarosOrigen();
	void iniciarConexion();
	void renderTuberias(struct birdPackage *);
	bool colision(SDL_Rect *,SDL_Rect *);
private:
	bool m_bRunning;
	SDL_Window* ventana;
	SDL_Renderer* _render;
	SDL_Texture* textura;
	SDL_Event event;
	int posX, posY, nJugador;
	SDL_Rect rectangulo_origen[3]; //Variables para los pajaros en el atlas.bmp
	SDL_Rect rectangulo_destino[3]; //Variables para la posicion en pantalla de los pajaros
	double angulos[3]; //Variables de los angulos para cada jugador
	SDL_Surface * bmp;
	double	angulo;
	struct birdPackage infoToSend, infoReceived;
	int	port;
	char serverIp  [16];
	SocketDatagrama	socket;
};
#endif
