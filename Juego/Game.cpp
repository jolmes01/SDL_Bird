// 	Game.cpp
/*
	Implementación de la clase Game. 
	En este archivo se incluye toda la lógica de nuestro programa. 
*/
#include "SDL.h"
#include "Game.h"
#include "data.h"
#include "SocketDatagrama.h"
#include "PaqueteDatagrama.h"
#include <iostream>
#include <cmath>

/* Global functions */

Game::Game(int argc, char **argv){
	if (argc != 3) {
		std::cout << "USO flappy <serverIp> <port>" << std::endl;
		exit(-1);
	}
	strcpy(serverIp, argv[1]);
	port = atoi(argv[2]);
	nJugador = 0;
	angulo = 330;
	iniciarConexion();
}

Game::~Game(){
	Game::clean();
}

bool Game::running(){
	return m_bRunning;
}

void Game::init(const char* titulo, int xpos, int ypos, int ancho, int alto){
	m_bRunning = 1;
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	ventana = SDL_CreateWindow(titulo, xpos, ypos, ancho, alto, SDL_WINDOW_SHOWN);
<<<<<<< HEAD
	if (ventana == nullptr){
=======
	_render = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
	/*if (ventana == nullptr){
>>>>>>> origin/master
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	if (_render == nullptr){
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	//se carga la imagen que contiene todos los graficos
	bmp = SDL_LoadBMP("atlas.bmp");
	//se define el blanco como color transparente
	SDL_SetColorKey(bmp, 1, SDL_MapRGB(bmp->format, 255, 255, 255));
	textura = SDL_CreateTextureFromSurface(_render, bmp);
	SDL_FreeSurface(bmp);
	if (textura == NULL) {
		std::cout << "FAILED TO FIND THE IMAGE " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	
	SDL_RenderClear(_render);
	renderFondo();
	
	//Inicializa la posición de los pajaros dentro de la pantalla, pasandolos del atlas.bmp al recuadro del juego
	initPajarosOrigen();
	SDL_RenderPresent(_render);
}

void Game::handleEvents(){
	while(SDL_PollEvent(&event))
	{
		//salto
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
			rectangulo_destino[nJugador].y -= 100;
			angulo = 300;
			infoToSend.opcode = 1;
		} else if (event.type == SDL_QUIT) {
			infoToSend.opcode = CLOSE;
			m_bRunning = false;
		}
	}
}

void Game::update(){
	//validacion del piso
	if (rectangulo_destino[nJugador].y < 500) {
		rectangulo_destino[nJugador].y += 10;
	}
	//validacion del angulo maximo
	if (angulo < 450) {
		angulo += 15;
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
	SDL_RenderClear(_render);
	renderFondo();
	renderTuberias(&infoReceived);
	//int i = 0;
	//Actualiza la posicion de los pajaros
	for (int i = 0; i < 3; ++i)	{
		rectangulo_destino[i].x = infoReceived.posicionJUMP_X[i];
		rectangulo_destino[i].y = infoReceived.posicionJUMP_Y[i];
		angulos[i] = infoReceived.angulo[i];
	}
	
}

void Game::render(){
	for (int i = 0; i < infoReceived.jugadoresTotales; ++i)
	{
		//funciona como copy pero rota la imagen con el angulo
		SDL_RenderCopyEx(_render, textura, &rectangulo_origen[i], &rectangulo_destino[i], infoReceived.angulo[i], NULL, SDL_FLIP_NONE);
	}
	SDL_RenderPresent(_render);
}

void Game::clean(){
	/* Free all objects */
	SDL_DestroyTexture(textura);
	SDL_DestroyRenderer(_render);
	SDL_DestroyWindow(ventana);
	
	/* Quit program */
	SDL_Quit();
}

// se encarga de copiar 3 veces el fondo y presentarlo consecutivamente
void Game::renderFondo()
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
	
	SDL_RenderCopy(_render, textura, &rectangulo_origen, &rectangulo_destino);
	rectangulo_destino.x += 288;
	SDL_RenderCopy(_render, textura, &rectangulo_origen, &rectangulo_destino);
	rectangulo_destino.x += 288;
	SDL_RenderCopy(_render, textura, &rectangulo_origen, &rectangulo_destino);
}

void Game::initPajarosOrigen()
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
		SDL_RenderCopy(_render, textura, &rectangulo_origen[i], &rectangulo_destino[i]);
	}
}

void Game::renderTuberias(struct birdPackage *p){
	//cout <<"Tuberia";
	SDL_Rect tuberia;
	tuberia.x = 110;
	tuberia.y = 645;
	tuberia.w = 55 ;
	tuberia.h = 325;
	
	SDL_Rect tuberia2;
	tuberia2.x = 165;
	tuberia2.y = 645;
	tuberia2.w = 55 ;
	tuberia2.h = 325;
	
	SDL_Rect pantalla;
	//pantalla.x = 10;
	//pantalla.y = -20;
	pantalla.w = 55;
	pantalla.h = 325;
	
	for(int i = 0 ; i < TUBE_LIST_SIZE; i++){
		pantalla.x = p->posicionTUBES_X[i];
		pantalla.y = p->posicionTUBES_Y[i] -250 ;
		SDL_RenderCopy(_render, textura, &tuberia, &pantalla);
		if(colision(&pantalla, &rectangulo_destino[nJugador])){
			cout <<"Dead\n";
			infoToSend.opcode=DEAD;
		}
		pantalla.y = p->posicionTUBES_Y[i] + 200;
		
		SDL_RenderCopy(_render, textura, &tuberia2, &pantalla);
		if(colision(&pantalla, &rectangulo_destino[nJugador])){
			cout <<"Dead\n";
			infoToSend.opcode=DEAD;
		}
	}
	
}

bool Game::colision(SDL_Rect *rec1,SDL_Rect *rec2){
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

void Game::iniciarConexion(){
	
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
	}
}
