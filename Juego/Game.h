// 	Game.h
/*
	Estructura de la clase Game. 
	Clase que nos permite crear y manejar los gráficos de nuestro programa.
*/

#ifndef __Game__
#define __Game__

#include "data.h"
#include "SocketDatagrama.h"
#include <iostream>
#include "SDL.h"	
#include <cmath>

class Game
{
public:
	Game();
	~Game();
	void init(const char* , int , int, int, int);
	void render();
	void update();
	void handleEvents();
	void clean();
	bool running();
	void renderFondo();
	void initPajarosOrigen();
private:
	bool m_bRunning;
	SDL_Window* ventana;
	SDL_Renderer* render;
	SDL_Texture* textura;
	int posX, posY, nJugador = 0;
	SDL_Rect rectangulo_origen[3]; //Variables para los pajaros en el atlas.bmp
	SDL_Rect rectangulo_destino[3]; //Variables para la posicion en pantalla de los pajaros
	double angulos[3]; //Variables de los angulos para cada jugador
	SDL_Surface * bmp;
	double	angulo = 330;
};
#endif
