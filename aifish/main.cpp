#include<iostream>
#include<stdio.h>
#include<SDL.h>

#include<SDL_image.h>
#include "Texture.h"
#include "GameObj.h"

#include <time.h>
#include <stdlib.h>


const int SCREEN_WIDTH=1024;
const int SCREEN_HEIGHT=768;

using namespace std;

time_t startt,endt;
#define FRAMERATE 30.0
bool randommoves=true;

inline void startwait(){startt=clock();}
inline void waitforframe(){while (( clock() - startt ) / (double) CLOCKS_PER_SEC < 1/FRAMERATE);}

//resources
SDL_Renderer* renderer=NULL;
SDL_Window* window=NULL;
//SDL_Surface* screen=NULL;//don't need this anymore since we are using renderer
Texture sharktex;
Texture foodtex;
Texture fishtex;
Texture rocktex;
Texture deadfishtex;
std::vector<GameObj> rockList;
std::vector<Fish> fishList;
std::vector<Food> foodList;
std::vector<Shark> sharkList;


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
					SDL_SetRenderDrawColor(renderer,0xAA,0xAA,0xFF,0xFF);
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
	sharktex.loadFromFile("./images/sharkcirc.png");
	foodtex.loadFromFile("./images/food.png");
	fishtex.loadFromFile("./images/fishcirc.png");
	rocktex.loadFromFile("./images/rock.png");
	deadfishtex.loadFromFile("./images/fishcircdead.png");
	return success;
}
void close(){
	sharktex.free();
	foodtex.free();
	fishtex.free();
	deadfishtex.free();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc,char** argv){

	srand(time(NULL));//generate rand seed;

	if(!init()){cout<<"init failed"<<endl; exit(1);}
	if(!load()){cout<<"load failed"<<endl; exit(2);}


	int i=0;
	int rockcount=15;
	int fishcount=1;
	if(argc>1){fishcount=atoi(argv[1]);}
	int foodcount=20;
	int sharkcount=2;

	for(i=0;i<rockcount;++i){
		GameObj trock(&rocktex,rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,0,5.0,.3,2,.5,.0005);
		rockList.push_back(trock);
	}

	//int movecount=1;//I want to be able to predict the likelihood of living the next 30 moves
	////int layersizes[]={50,movecount};
	//std::vector<double>input=std::vector<double>(18+4*movecount,0);
	int stepsback=10;
	std::vector<double>inputve=std::vector<double>(18+4*stepsback,0);
	//NeuralNet *nn=new NeuralNet(2,layersizes,input);

	int layersizes[]={20,20,4};
	NeuralNet *nn=new NeuralNet(3,layersizes,inputve);
	for(i=0;i<fishcount;++i){
		//Fish(Texture* tex,int xposition,int yposition, double rotation,
		//     double turnRate,double acceleration,double vmax,double brakerate,double friction)
		//Fish tfish=Fish(&fishtex,rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%360,6.0,.4,6,.5,.0005);

		fishList.push_back(Fish(&fishtex,rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%360,8.0,.8,12,1.,.001,stepsback,nn));
		fishList[i].setDeadTex(&deadfishtex);
		//fishList[i].nn=nn;
	}

	for(i=0;i<foodcount;++i){
		Food tfood(&foodtex,rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,0,5.0,.3,2,.5,.0005);
		foodList.push_back(tfood);
	}
	for(i=0;i<sharkcount;++i){
		Shark tshark(&sharktex,rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,0,2.7,.5,10,.5,.00005);
		sharkList.push_back(tshark);
	}




	bool kleft=false,kright=false,kup=false,kdown=false,wait=true,nonn=false;

	bool quit=false;
	SDL_Event e;
	while(!quit){
		startwait();
		//event handling

		while(SDL_PollEvent(&e)!=0){
			if(e.type==SDL_QUIT){quit=true;}
			else if(e.type==SDL_KEYDOWN){
				switch(e.key.keysym.sym){
				case SDLK_z:
					nonn=!nonn;
					break;
				case SDLK_f://fish user control training
					Fish::trainingMode=!Fish::trainingMode;
					break;
				case SDLK_u:
					sharkList[0].toggleUserControll();
					break;
				case SDLK_w:
					wait=false;
					break;
				case SDLK_r:
					randommoves=!randommoves;
					break;
				case SDLK_LEFT:
					kleft=true;
					break;
				case SDLK_RIGHT:
					kright=true;
					break;
				case SDLK_UP:
					kup=true;
					break;
				case SDLK_DOWN:
					kdown=true;
					break;
				default: break;
				}
			}else if(e.type==SDL_KEYUP){
				switch(e.key.keysym.sym){
				case SDLK_LEFT:
					kleft=false;
					break;
				case SDLK_RIGHT:
					kright=false;
					break;
				case SDLK_UP:
					kup=false;
					break;
				case SDLK_DOWN:
					kdown=false;
					break;
				default: break;
				}
			}
		}
		/*if(kup){sharkList[0].accelerate();}
		if(kdown){sharkList[0].brake();}
		if(kleft){sharkList[0].turnLeft();}
		if(kright){sharkList[0].turnRight();}*/
		if(kup){fishList[0].trainaccelerate();}
		if(kdown){fishList[0].trainbrake();}
		if(kleft){fishList[0].trainturnLeft();}
		if(kright){fishList[0].trainturnRight();}


		for(i=0;i<fishcount;++i){
			if(nonn){fishList[i].update_nonn();}else{
			fishList[i].update();}
		}

		for(i=0;i<sharkcount;++i){sharkList[i].update();}
		for(i=0;i<foodcount;++i){foodList[i].update();}
		//if(shark.checkCollision(&fish)){cout<<"collide"<<endl;}
		SDL_RenderClear(renderer);//clearScreen
		for( i=0;i<rockcount;++i){rockList[i].render();	}
		for( i=0;i<fishcount;++i){fishList[i].render();	}
		for( i=0;i<foodcount;++i){foodList[i].render();	}
		for( i=0;i<sharkcount;++i){sharkList[i].render();}

		SDL_RenderPresent(renderer);
		if(wait){waitforframe();}//limits framerate
		/*double fishavg=0;
		for(i=0;i<fishcount;++i){
			fishavg+=fishList[i].lifespan;
		}
		cout<<"average fish life "<< fishavg/(double)fishcount<< endl;
		*/

	}


	close();
	return 0;
}

