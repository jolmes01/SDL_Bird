// 	Game.cpp
/*
	Implementación de la clase Game. 
	En este archivo se incluye toda la lógica de nuestro programa. 
*/

#include "Game.h"
#include "data.h"
#include "SocketDatagrama.h"

/* Global functions */


Game::Game(){
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
	if (ventana == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	render = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
	if (m_pRenderer == nullptr){
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	//se carga la imagen que contiene todos los graficos
	bmp = SDL_LoadBMP("atlas.bmp");
	//se define el blanco como color transparente
	SDL_SetColorKey(bmp, 1, SDL_MapRGB(bmp->format, 255, 255, 255));
	textura = SDL_CreateTextureFromSurface(render, bmp);
	SDL_FreeSurface(bmp);
	if (textura == NULL) {
        std::cout << "FAILED TO FIND THE IMAGE" << std::endl;
		m_bRunning = 0;
	}

	SDL_RenderClear(render);
	renderFondo();

	//Inicializa la posición de los pajaros dentro de la pantalla, pasandolos del atlas.bmp al recuadro del juego
	initPajarosOrigen();
	SDL_RenderPresent(render);
}

void Game::handleEvents(){
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
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			m_bRunning = false;
			break;
			default:
			break;
		}
	}
}

void Game::update(){
}

void Game::render(){
	
}

void Game::clean(){
	/* Free all objects */
	SDL_DestroyTexture(textura);
	SDL_DestroyRenderer(render);
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

	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
	rectangulo_destino.x += 288;
	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
	rectangulo_destino.x += 288;
	SDL_RenderCopy(render, textura, &rectangulo_origen, &rectangulo_destino);
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
		SDL_RenderCopy(render, textura, &rectangulo_origen[i], &rectangulo_destino[i]);
	}
}

void Game::iniciarConexion(){
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
	}
}

