#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "Texture.h"
#include <vector>
#include "NeuralNet.h"
class GameObj{
public:
	static unsigned int idcount;
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
	unsigned int getId();
	double getX();
	double getY();
	double getRot();
	double getVel();
	double getVmax();
	//double getRadius();
	double getDistanceSquared(GameObj &o);
	int getClosest(std::vector<GameObj> &list);
	double getDirToObj(GameObj &o);

	void update();
	bool checkCollision(GameObj *o);
	bool checkCollisionList(std::vector<GameObj> &list);

	void render();

	void turnLeft();
	void turnRight();
	void accelerate();
	void brake();


protected:
	unsigned int id;
	Texture *texture;
	double x,y;
	int w,h,cw,ch;
	double rot;
	double v;

};


class Fish: public GameObj{

public:

 	Fish(Texture* tex,int xposition,int yposition, double rotation=0.0,double turnRate=5.0,double acceleration=.3,double vmax=2.0,double brakerate=.5,double friction=.0005,int stepsback=10,NeuralNet * neural=NULL);
 	~Fish();
 	void update();
	void sense();
	void setDeadTex(Texture*tex);
	void setLiveTex(Texture*tex);
	double getHealth();
	NeuralNet *nn;
	std::vector<double> input;
	static std::vector<double> actual;//put training results in here
	std::vector<double> prediction;
	void die();
	int feedFood();

	void trainturnLeft();//fish training
	void trainturnRight();
	void trainaccelerate();
	void trainbrake();
	void update_nonn();
	void generateGoodMoves();

	int lifespan;
	int shortestlife;
	int longestlife;
	double avglife;

	static bool trainingMode;

private:

	double health;//0-1
	Texture* deadtex;
	Texture* livetex;

	int stepsback;//number of predictions made by network
	int stepscount;//current number of moves gone through
	void generateRandomMoves();
	//std::vector<double> sc[3];//senses and choices

	//nninput
	/*
	 *  health
	 *  x,y,v,rot(normalized)
	 *  nearest fish (x,y,rot,health,v)
     *  nearest rock (x,y)
     *  shark (x,y,rot,v)
     *  nearest food (x,y)
     *
     *
	 */

};


class Shark: public GameObj{
public:
 	Shark(Texture* tex,int xposition,int yposition, double rotation=0.0,double turnRate=5.0,double acceleration=.3,double vmax=2.0,double brakerate=.5,double friction=.0005);
	void update();
	void toggleUserControll();
protected:
	bool usercontrolled;
};

class Food: public GameObj{
public:
 	Food(Texture* tex,int xposition,int yposition, double rotation=0.0,double turnRate=5.0,double acceleration=.3,double vmax=2.0,double brakerate=.5,double friction=.0005);
	void update();
};





#endif
