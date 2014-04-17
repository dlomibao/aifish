#include<iostream>
#include<stdio.h>
#include<SDL.h>
#include<SDL_image.h>
#include "Texture.h"
#include "GameObj.h"

const int SCREEN_WIDTH=1024;
const int SCREEN_HEIGHT=768;
using namespace std;

//resources
SDL_Renderer* renderer=NULL;
SDL_Window* window=NULL;
//SDL_Surface* screen=NULL;//don't need this anymore since we are using renderer
Texture sharktex;
Texture foodtex;
Texture fishtex;

bool init(){
	bool success=true;
	//initialize
		if(SDL_Init(SDL_INIT_VIDEO)<0){
			cout<<"SDL failed to init "<<SDL_GetError()<<endl;
			success= false;
		}else{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");//linear texture filtering
			window=SDL_CreateWindow("AI fish",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
			if(window==NULL){
				cout<<"failed to create window "<<SDL_GetError()<<endl;
				success= false;
			}else{
				renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
				if(renderer==NULL){
					cout<<"renderer failed to be created "<<SDL_GetError()<<endl;
					success=false;
				}else{
					SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
					//init sdl_img
					int imgFlags=IMG_INIT_PNG;
					if(!(IMG_Init(imgFlags) & imgFlags)){cout<<"SDL_image failed"<<IMG_GetError()<<endl;success=false;}
				}
			}
		}
	return success;
}
SDL_Texture* loadTexture(string filename){
	SDL_Texture* newtex=NULL;
	SDL_Surface* surf=IMG_Load(filename.c_str());
	if(surf==NULL){cout<<"failed loading "<<filename<<IMG_GetError()<<endl;}
	else{
		newtex=SDL_CreateTextureFromSurface(renderer,surf);
		if(newtex==NULL){cout<<"failed creating texture "<<filename<<SDL_GetError()<<endl;}
		SDL_FreeSurface(surf);
	}
	return newtex;
}
bool load(){
	bool success=true;
	sharktex.loadFromFile("./images/shark.png");
	foodtex.loadFromFile("./images/food.png");
	fishtex.loadFromFile("./images/fish.png");
	return success;
}
void close(){
	sharktex.free();
	foodtex.free();
	fishtex.free();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc,char** argv){

	if(!init()){cout<<"init failed"<<endl; exit(1);}
	if(!load()){cout<<"load failed"<<endl; exit(2);}

	///////////////test object
	//GameObj(Texture* tex,double turnRate,double acceleration,double vmax,double brakerate,double friction){

	GameObj shark(&sharktex,400,400,0,5.0,.3,2,.5,.0005);



	bool quit=false;
	SDL_Event e;
	while(!quit){
		//event handling
		while(SDL_PollEvent(&e)!=0){
			if(e.type==SDL_QUIT){quit=true;}
			else if(e.type==SDL_KEYDOWN){
				switch(e.key.keysym.sym){
				case SDLK_LEFT:
					shark.turnLeft();
					break;
				case SDLK_RIGHT:
					shark.turnRight();
					break;
				case SDLK_UP:
					shark.accelerate();
					break;
				case SDLK_DOWN:
					shark.brake();
					break;
				default: break;
				}
			}
		}
		shark.update();

		SDL_RenderClear(renderer);//clearScreen
		shark.render();
		sharktex.render(100,100,NULL,90,NULL);
		foodtex.render(200,200,NULL,0,NULL);
		fishtex.render(300,300,NULL,0,NULL);

		SDL_RenderPresent(renderer);
	}


	close();
	return 0;
}

