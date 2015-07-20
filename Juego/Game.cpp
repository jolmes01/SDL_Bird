// 	Game.cpp
/*
	Implementación de la clase Game. 
	En este archivo se incluye toda la lógica de nuestro programa. 
	Es aquí donde se ubica el código para la creación y trazado de las figuras.
*/

#include "Game.h"

#define aleatorio(min, max) min+rand()%max
#define MAX_OFFSET 300
#define CONT_NUEVO 1
#define MAX_DIA 100

int contador = 0;

int* fueraRandom(int , int);

Game::Game(int ancho, int alto, int inicial){ //Se cambio el ingreso de ancho y alto de init al constructor de la clase ya que se necesita antes
	_ancho = ancho;
	_alto = alto;
	srand(time(NULL));
	for (int i = inicial; i > 0; i--)
		agrega();
}

Game::~Game(){
	Game::clean();
}

bool Game::running(){
	return m_bRunning;
}

void Game::init(const char* titulo, int xpos, int ypos, int banderas){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	m_pWindow = SDL_CreateWindow(titulo, xpos, ypos, _ancho, _alto, banderas);
	if (m_pWindow == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr){
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		m_bRunning = 0;
	}
	m_bRunning = 1;
}

void Game::handleEvents(){
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
	cout<<"Figuras: "<<asteroides.size()<<endl;
	contador++;
	if(contador==CONT_NUEVO){
		agrega();
		contador = 0;      
	}
	if(asteroides.size() > 0){
		for(std::vector<Asteroide>::iterator i=asteroides.begin(); i != asteroides.end(); i++)
			if(! (*i).mueve() ) 
				asteroides.erase(i--);
			else{
				if(i>asteroides.begin()){
					for(std::vector<Asteroide>::iterator j=asteroides.begin(); j < i; j++)
						//if(((*i).obtieneCentro()-(*j).obtieneCentro()).length()<(*i).obtieneDiametro()/2+(*j).obtieneDiametro()/2){
						if(((*i).obtieneCentro()-(*j).obtieneCentro()).length()<MAX_DIA){
							if(((*i).obtieneCentro()-(*j).obtieneCentro()).length()<(*i).obtieneDiametro()/2+(*j).obtieneDiametro()/2){
								if((*i).obtieneVelocidad()*(*i).obtieneMasaAprox() < (*j).obtieneVelocidad()*(*j).obtieneMasaAprox())
									asteroides.erase(i);
								else
									asteroides.erase(j);
								i--;
								j--;
							}
							else{
								vector<Vector2D> puntos =(*i).obtieneVertices();
								bool salir = false;
								int veces = 0;
								Vector2D centro = (*i).obtieneCentro();
								vector<Vector2D> otrosPuntos =(*j).obtieneVertices();
								for(std::vector<Vector2D>::iterator k=puntos.begin(); k!=puntos.end(); k++){
									float d1 = (centro-(*k)).length();
									for(std::vector<Vector2D>::iterator l=otrosPuntos.begin(); l!=otrosPuntos.end(); l++){
										float d2 = (centro-(*l)).length();
										if(d1>d2){
											veces++;
											if(veces>4){
												//cout<<"Centro i: "<<(*i).obtieneCentro().getX()<<","<<(*i).obtieneCentro().getY()<<endl;
												//cout<<"Punto k: "<<(*k).getX()<<","<<(*k).getY()<<endl;
												//cout<<"Punto l: "<<(*l).getX()<<","<<(*l).getY()<<endl;
												//cout<<d1<<","<<d2<<endl<<endl;
												if((*i).obtieneVelocidad()*(*i).obtieneMasaAprox() < (*j).obtieneVelocidad()*(*j).obtieneMasaAprox()){
													(*j).cambiaRumbo((*l),(*i).obtieneRumbo()-((*k)-centro));
													asteroides.erase(i);
												}
												else{
													(*i).cambiaRumbo((*k),(*j).obtieneRumbo()-((*l)-(*j).obtieneCentro()));
													asteroides.erase(j);
												}
												i--;
												j--;
												salir = true;
												break;
											}

										}
									}
									if(salir)
										break;
								}
							}
						}
				}
			}
		/*std::vector<Asteroide>::iterator i=asteroides.begin();
		vector<Vector2D> puntos =(*i).obtieneVertices(); 
			for(std::vector<Vector2D>::iterator k=puntos.begin(); k!=puntos.end(); k++)
				cout<<"Punto k: "<<(*k).getX()<<","<<(*k).getY()<<endl;
		*/
	}

}

void Game::render(){
	SDL_RenderClear(m_pRenderer);
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	if(asteroides.size() > 0)	
		for(std::vector<Asteroide>::iterator i=asteroides.begin(); i != asteroides.end(); i++){
			SDL_SetRenderDrawColor(m_pRenderer, (*i).obtieneColor()[0], (*i).obtieneColor()[1], (*i).obtieneColor()[1], 255);
			SDL_RenderDrawLines(m_pRenderer, (*i).getPoints(), (*i).obtieneN()+1);
		}
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(m_pRenderer);
}

void Game::clean(){
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Game::agrega(){
	int* lugarAleatorio = new int[2];
	lugarAleatorio = fueraRandom(_ancho, _alto);
	Vector2D aux(aleatorio(1, _ancho), aleatorio(1, _alto));
	asteroides.emplace_back(Asteroide(lugarAleatorio[0], lugarAleatorio[1], aux));
}


int* fueraRandom(int ancho, int alto) {
	int* coordenada = new int[2];
	if(rand()%2 == 0)
		coordenada[0] = rand()%(MAX_OFFSET+ancho);
	else
		coordenada[0] = -1*rand()%MAX_OFFSET;
	if(rand()%2 == 0)
		if(coordenada[0]>=0 && coordenada[0]<=ancho)
			coordenada[1] = alto+rand()%MAX_OFFSET;
		else
			coordenada[1] = rand()%MAX_OFFSET+alto;
	else
		coordenada[1] = -1*rand()%MAX_OFFSET;
	coordenada[0] += coordenada[0]<-50&&coordenada[0]>ancho+50?0:coordenada[0]<0?-50:50;
	coordenada[1] += coordenada[1]<-50&&coordenada[1]>alto+50?0:coordenada[1]<0?-50:50;
	return coordenada;
}
