#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "Texture.h"
class GameObj{
public:
	GameObj();
	GameObj(Texture* tex,int xposition,int yposition, double rotation=0.0,double turnRate=5.0,double acceleration=.3,double vmax=2.0,double brakerate=.5,double friction=.0005);
	//~GameObj();//use default destructor for now

	void setTexture(Texture* tex);
	double turnRate;
	double acceleration;
	double vmax;
	double brakerate;
	double friction;

	void setPos(int x,int y);
	void setVel(double velocity);


	void update();
	void handleEvents();//todo
	void render();

	void turnLeft();
	void turnRight();
	void accelerate();
	void brake();
private:
	Texture *texture;
	double x,y;
	double rot;
	double v;

};






#endif
