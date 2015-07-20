// 	Game.h
/*
	Esqueleto de la clase Game. 
	Clase que nos permite crear y manejar los gráficos de nuestro programa.
*/

#ifndef __Game__
#define __Game__

#include "Asteroide.h"	
#include "Vector2D.h"
#include <iostream>
#include <SDL2/SDL.h>	
#include <cmath>

class Game
{
public:
	Game(int, int, int);
	~Game();
	void init(const char* , int , int, int);
	void render();
	void update();
	void handleEvents();
	void clean();
	bool running();
	void agrega();
private:
	bool m_bRunning;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	int _ancho;
	int _alto;
	vector <Asteroide> asteroides;
};
#endif
