#include "GameObj.h"
#include <math.h>

#include <math.h>
#define PI 3.14159265

GameObj::GameObj(Texture* tex,int xposition,int yposition, double rotation,double turnRate,double acceleration,double vmax,double brakerate,double friction){
	texture=tex;
	this->turnRate=turnRate;
	this->acceleration=acceleration;
	this->vmax=vmax;
	this->brakerate=brakerate;
	this->friction=friction;
	x=xposition;
	y=yposition;
	v=0;
	rot=rotation;
}


void GameObj::accelerate(){
	v+=acceleration;
	if(v>vmax){v=vmax;}
}
void GameObj::brake(){
	v-=brakerate;
	if(v<0){v=0;}
}
void GameObj::turnLeft(){
	this->rot=fmod(rot-turnRate,360.0);
}
void GameObj::turnRight(){
	this->rot=fmod(rot+turnRate,360.0);
}
void GameObj::setPos(int x,int y){
	this->x=x;this->y=y;
}
void GameObj::setVel(double velocity){
	if(velocity<0){//negative velocity is just rotated by 180degrees
		velocity*=-1;
		rot=fmod(rot+180.0,360.0);
	}
	v=velocity;
}
void GameObj::render(){
	texture->render(x,y,NULL,rot,NULL);
}
void GameObj::update(){
	v-=friction;
	if(v<0){v=0;}
	double pirot=-(2.0*PI*rot/360.0);
	x+=v*-sin(pirot);
	y+=v*-cos(pirot);
	//set up rotation/position adjustment like this because
	//0 degrees is facing up and top left is (0,0)
	//basically this is what works. I can refactor it later to use less calculation

	//temp stuff to handle wrap around for current screen size.
	if(y<0){y=767;}
	if(x<0){x=1023;}
	if(y>767){y=0;}
	if(x>1023){x=0;}
}
