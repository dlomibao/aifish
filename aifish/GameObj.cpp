#include "GameObj.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "NeuralNet.h"
#define PI 3.14159265

extern std::vector<GameObj> rockList;
extern std::vector<Fish> fishList;
extern std::vector<Food> foodList;
extern std::vector<Shark> sharkList;
extern bool randommoves;

 const int SCREEN_WIDTH=1024;
 const int SCREEN_HEIGHT=768;
/*
int layersizes2[]={18,30*4};
NeuralNet nn2=NeuralNet(2,layersizes2,NULL);
*/
unsigned int GameObj::idcount=0;
GameObj::GameObj(){
	id=++idcount;
	texture=NULL;
	this->turnRate=5;
	this->acceleration=.25;
	this->vmax=2;
	this->brakerate=.75;
	this->friction=.0005;
	x=0;
	y=0;
	v=0;
	rot=0;
	w=0;
	h=0;
	cw=w/2;
	ch=h/2;
}
GameObj::GameObj(Texture* tex,int xposition,int yposition, double rotation,double turnRate,double acceleration,double vmax,double brakerate,double friction){
	id=++idcount;
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
	w=tex->getWidth();
	h=tex->getHeight();
	cw=w/2;
	ch=h/2;
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
	texture->render(x-cw,y-ch,NULL,rot,NULL);
}
bool GameObj::checkCollision(GameObj *o){
	int dx=x-o->x;
	int dy=y-o->y;
	int rads=cw+o->cw;//assume circle so cw==radius
	if((dx*dx)+(dy*dy)<(rads*rads)){
		return true;
	}else{
		return false;
	}
}

double GameObj::getDistanceSquared(GameObj &o){
	double dx=x-o.getX();
	double dy=y-o.getY();
	return dx*dx+dy*dy;
}
int GameObj::getClosest(std::vector<GameObj> &list){
	int closest=0;
	double ds,d;
	for(int i=0;i<list.size();++i){
		d=getDistanceSquared(list[i]);
		if(d<ds){
			closest=i;
			ds=d;
		}
	}
	return closest;
}
template<typename T>
double getDistanceSquared(double myx,double myy,T &o){
	double dx=myx-o.getX();
	double dy=myy-o.getY();
	return dx*dx+dy*dy;
}
template<typename T>
int getClosest(double myx,double myy, std::vector<T> &list){
	int closest=0;
	double ds,d;
	for(int i=0;i<list.size();++i){
		d=getDistanceSquared(myx,myy,list[i]);
		if(d<ds){
			closest=i;
			ds=d;
		}
	}
	return closest;
}
template<typename T>
int getClosest(GameObj &o, std::vector<T> &list){
	int closest=0;
	double ds,d;
	for(int i=0;i<list.size();++i){
		if(o.getId()!=list[i].getId()){
			d=o.getDistanceSquared(list[i]);
			if(d<ds){
				closest=i;
				ds=d;
			}
		}
	}
	return closest;
}

double GameObj::getDirToObj(GameObj &o){
	double dx=x-o.x;
	double dy=y-o.y;
	double ret=90;
	if(dx>0){ret*=-1;}
	return ret+atan(dy/dx)*180/PI;

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


	/*
	if(checkCollisionList(rockList)){
		//roll back move
		setPos(tmpx,tmpy);
		v=0;
	}*/
	//temp stuff to handle wrap around for current screen size.
		if(y<0){y=SCREEN_HEIGHT-1;}
		if(x<0){x=SCREEN_WIDTH-1;}
		if(y>SCREEN_HEIGHT){y=0;}
		if(x>SCREEN_WIDTH){x=0;}



}
bool GameObj::checkCollisionList(std::vector<GameObj> &list){
	for(std::vector<GameObj>::iterator it=list.begin();it!=list.end();++it){
		if(this->checkCollision(&*it)){
			return true;
		}
	}
	return false;
}

double GameObj::getX(){return x;}
double GameObj::getY(){return y;}
double GameObj::getRot(){return rot;}
double GameObj::getVel(){return v;}
double GameObj::getVmax(){return vmax;}
unsigned int GameObj::getId(){return id;}


Shark::Shark(Texture* tex,int xposition,int yposition, double rotation,double turnRate,double acceleration,double vmax,double brakerate,double friction){
	this->texture=tex;
	this->turnRate=turnRate;
	this->acceleration=acceleration;
	this->vmax=vmax;
	this->brakerate=brakerate;
	this->friction=friction;
	this->x=xposition;
	this->y=yposition;
	this->v=0;
	this->rot=rotation;
	this->w=tex->getWidth();
	this->h=tex->getHeight();
	this->cw=w/2;
	this->ch=h/2;
	int checkCount=0;
	this->usercontrolled=false;
	while(checkCollisionList(rockList) && checkCount<SCREEN_WIDTH*SCREEN_HEIGHT){
		x=fmod(x+1,SCREEN_WIDTH);
		if(x<1){y=fmod(y+1,SCREEN_HEIGHT);}
		checkCount++;
	}
	//int cl=this->getClosest(rockList);
	//rot=this->getDirToObj(rockList[cl]);
	//std::cout<<checkCount<<std::endl;
}
void Shark::toggleUserControll(){
	usercontrolled=!usercontrolled;
}
void Shark::update(){
	if(!usercontrolled){
		//automatically chase nearest
			int closest=0;
			double ds=getDistanceSquared(fishList[0]);

			for(int i=1;i<fishList.size();++i){
				double d=getDistanceSquared(fishList[i]);
				//std::cout<<d<<std::endl;
				if(d<ds){
					closest=i;
					ds=d;
				}
			}
			double rotnear=this->getDirToObj(fishList[closest]);
			if(rotnear<rot){turnLeft();}else{turnRight();}
			accelerate();
	}




	double tmpx=getX();
	double tmpy=getY();
	GameObj::update();
	if(checkCollisionList(rockList)){
		//roll back move
		//if(v==0)setPos(tmpx+rand()%16-8,tmpy+rand()%16-8);
		v/=1.85;
		//accelerate();
	}
	//accelerate();
}

//Fish::avglife_all=0;
//Fish::totallives=0;
bool Fish::trainingMode=true;

std::vector<double> Fish::actual=std::vector<double>(4,0);
Fish::Fish(Texture* tex,int xposition,int yposition, double rotation,double turnRate,double acceleration,double vmax,double brakerate,double friction,int stepsback,NeuralNet *neural){
	this->texture=tex;
	this->livetex=tex;
	this->deadtex=tex;
	this->turnRate=turnRate;
	this->acceleration=acceleration;
	this->vmax=vmax;
	this->brakerate=brakerate;
	this->friction=friction;
	this->x=xposition;
	this->y=yposition;
	this->v=0;
	this->rot=rotation;
	this->w=tex->getWidth();
	this->h=tex->getHeight();
	this->cw=w/2;
	this->ch=h/2;
	int checkCount=0;
	while(checkCollisionList(rockList) && checkCount<SCREEN_WIDTH*SCREEN_HEIGHT){

		x=fmod(x+1,SCREEN_WIDTH);
		if(x<1){y=fmod(y+1,SCREEN_HEIGHT);}
		checkCount++;
	}
	health=1.0;
	//stepsback=10;//I want to be able to predict the likelihood of living the next 30 moves
//	int layersizes[]={50,50,movecount};
	stepscount=10;
	this->stepsback=stepsback;
	this->input=std::vector<double>(18+4*this->stepsback,0);

	this->prediction=std::vector<double>(4,0);

	//nn=NeuralNet(3,layersizes,input);*/
	//std::cout<<checkCount<<std::endl;
	 lifespan=0;
	 shortestlife=0;
	 longestlife=0;
	 avglife=0;
	 nn=neural;

}
Fish::~Fish(){
	//delete(nn);
}
void Fish::setDeadTex(Texture *tex){
	this->deadtex=tex;
}
void Fish::setLiveTex(Texture *tex){
	this->livetex=tex;
}
double Fish::getHealth(){return health;}
void Fish::sense(){
	/*
	 *  health
	 *  x,y,v,rot(normalized)
	 *
	 *  nearest rock (x,y)
	 *
	 *  nearest fish (x,y,rot,health,v)
     *
     *  shark (x,y,rot,v)
     *  nearest food (x,y)
     *
     *
	 */
	//std::cout<<"sense"<<std::endl;
	input[0]=health;
	input[1]=x/SCREEN_WIDTH;//normalizes
	input[2]=y/SCREEN_HEIGHT;
	input[3]=v/vmax;
	input[4]=rot/360;

	//nearest rock
	int near= this->getClosest(rockList);
	GameObj igo=rockList[near];
	input[5]=igo.getX()/SCREEN_WIDTH;
	input[6]=igo.getY()/SCREEN_HEIGHT;

	//nearest fish
	//near= ::getClosest(x,y,fishList);
	near= ::getClosest(*(GameObj *)this,fishList);
	Fish ifish=fishList[near];
	//input[7]=ifish.getHealth();
	input[8]=ifish.getX()/SCREEN_WIDTH;//normalizes
	input[9]=ifish.getY()/SCREEN_HEIGHT;
	input[10]=ifish.getVel()/ifish.getVmax();
	//input[11]=ifish.getRot()/360.0;
	input[11]=getDirToObj(igo);

	//nearest shark;
	near= ::getClosest(x,y,sharkList);
	Shark ishark=sharkList[near];
	input[12]=this->getDistanceSquared(ishark)/(SCREEN_HEIGHT*SCREEN_HEIGHT);//normalizes
	input[13]=this->getDirToObj(ishark)/360.0;
	input[14]=ishark.getVel()/ifish.getVmax();
	input[15]=ishark.getRot()/360.0;

	near= ::getClosest(x,y,foodList);
	Food ifd=foodList[near];
	input[16]=ifd.getX()/SCREEN_WIDTH;
	input[17]=ifd.getY()/SCREEN_HEIGHT;

	input[7]=this->getDirToObj(ifd)/360.0;


	for(int i=0;i<18;i++){
		if(input[i]<0){input[i]=1+input[i];}
	//std::cout<<"sense"<<i<<" "<<input[i]<<std::endl;
		}
}
void Fish::generateRandomMoves(){
	//int i=18;
	for(int i=18;i<this->stepsback*4+18;i+=4){

		input[i]    =((double)rand()/(double)RAND_MAX >.5)?.999:001;//0 nothing left 1;
		input[i+1]  =((double)rand()/(double)RAND_MAX >.5)?.999:001;;//0 nothing right 1;
		input[i+2]  =((double)rand()/(double)RAND_MAX >.5)?.999:001;;//accel 1, nothing 0
		input[i+4]  =((double)rand()/(double)RAND_MAX >.5)?.999:001;; //brake -1;


	}
}
void Fish::generateGoodMoves(){
	int maxtry=15;
	std::vector<double> curr;
	for(int i=0;i<stepsback;i++){
		bool good=false;
		int tries=0;
		double best[4]={0,0,0,0};
		int offset=18+i*4;
		while(good==false && tries<maxtry){

			input[offset]    =(double)rand()/(double)RAND_MAX;//0 nothing left 1;
			input[offset+1]  =(double)rand()/(double)RAND_MAX;//0 nothing right 1;
			input[offset+2]  =(double)rand()/(double)RAND_MAX;//accel 1, nothing 0
			input[offset+4]  =(double)rand()/(double)RAND_MAX; //brake -1;
			nn->feed(input);
			curr=nn->getOutput();

			if(curr[i] > .6){good=true;}
			tries++;
		}


	}
}
void Fish::die(){
	std::cout<<"fish#"<<id<<" life:"<<this->lifespan<<std::endl;
	this->lifespan=0;
	this->texture=this->deadtex;
	this->render();
	texture=livetex;
    health=1.;//make it so that food is more valuable
	x=rand()%SCREEN_WIDTH;
	y=rand()%SCREEN_HEIGHT;
	rot=rand()%360;
	int checkCount=0;
	while(checkCollisionList(rockList) && checkCount<SCREEN_WIDTH*SCREEN_HEIGHT){
		x=fmod(x+1,SCREEN_WIDTH);
		if(x<1){y=fmod(y+1,SCREEN_HEIGHT);}
		checkCount++;
	}
}
int Fish::feedFood(){
	health=1.0;
	return 1;
}
void Fish::trainturnLeft(){actual[0]=1.0;turnLeft();}
void Fish::trainturnRight(){actual[1]=1.0;turnRight();}
void Fish::trainaccelerate(){actual[2]=1.0;accelerate();}
void Fish::trainbrake(){actual[3]=1.0;brake();}

void Fish::update(){
	this->sense();//fill 0-17 input with sensory data
	if(trainingMode){//user controled training
		nn->feed(input);
		nn->backProp(input,actual);//actual set by user

		for(int i=18+4*this->stepsback-4-1;i>17;i--){//shift older data
			double temp=input[i];
			input[i+4]=temp;
			//this->input[i+4]=input[i];
			//this->input[i+5]=input[i+1];
			//this->input[i+6]=this->input[i+2];
			//this->input[i+7]=this->input[i+3];
		}

		this->input[18]=actual[0];
		this->input[19]=actual[1];
		this->input[20]=actual[2];
		this->input[21]=actual[3];
		/*for(int i=0;i<18+4*stepsback;i++){
			std::cout<<input[i]<<std::endl;
		}*/

		Fish::actual[0]=0.0;//reset back to 0 for next update
		Fish::actual[1]=0.0;
		Fish::actual[2]=0.0;
		Fish::actual[3]=0.0;
	}else{//use nn for decisions
		nn->feed(input);
		std::vector<double> outp=nn->getOutput();
		if((double)rand()/(double)RAND_MAX < outp[0]){turnLeft();outp[0]=1.0;}else{outp[0]=0.0;}
		if((double)rand()/(double)RAND_MAX < outp[1]){turnRight();outp[1]=1.0;}else{outp[1]=0.0;}
		if((double)rand()/(double)RAND_MAX < outp[2]){accelerate();outp[2]=1.0;}else{outp[2]=0.0;}
		if((double)rand()/(double)RAND_MAX < outp[3]){brake();outp[3]=1.0;}else{outp[3]=0.0;}
		for(int i=18+4*this->stepsback-4-1;i>17;i--){//shift older data
			double temp=input[i];
			input[i+4]=temp;
		}
		this->input[18]=outp[0];
		this->input[19]=outp[1];
		this->input[20]=outp[2];
		this->input[21]=outp[3];
	}
	double tmpx=getX();
	double tmpy=getY();
	GameObj::update();
	if(checkCollisionList(rockList)){
		//roll back move
		setPos(tmpx,tmpy);
		v=0;
		health-=.05;//rocks hurt
	}
	for(std::vector<Shark>::iterator it=sharkList.begin();it!=sharkList.end();++it){
		if(this->checkCollision(&*it)){
			health-=.25;
			texture=this->deadtex;
		}
	}
	health-=.001;
	//if(health>.5){texture=livetex;}else
	//if(health<.5){texture=deadtex;}
	if(health<0){
		die();
	}else{
		lifespan++;
	}
}

void Fish::update_nonn(){
	lifespan++;

	int closest=0;
	double ds=getDistanceSquared(foodList[0]);
	for(int i=1;i<foodList.size();++i){
		double d=getDistanceSquared(foodList[i]);
		//std::cout<<d<<std::endl;
		if(d<ds){
			closest=i;
			ds=d;
		}
	}
	double rotnear=this->getDirToObj(foodList[closest]);
	if(this->checkCollision((GameObj *)&foodList[closest])){
		health=1.0;
		this->texture=this->livetex;
	}




	double tmpx=getX();
	double tmpy=getY();
	GameObj::update();
	if(checkCollisionList(rockList)){
			//roll back move
			setPos(tmpx,tmpy);
			v=0;
			health-=.05;//rocks hurt
			turnLeft();
			turnLeft();
			turnLeft();
			accelerate();
			turnLeft();
	}else{
		if(rotnear<rot){turnLeft();}else{turnRight();}
	}
	//if(rotnear<rot){turnLeft();}else{turnRight();}
	accelerate();
	switch (rand()%8){
	case 0:
		accelerate();
		break;
	case 1:
		accelerate();
	case 2:
		turnLeft();
	case 3:
		turnLeft();
		break;
	case 4:
		accelerate();
	case 5:
		turnRight();
	case 6:
		turnRight();
		break;
	case 7:
		brake();
		break;
	default:
		break;
	}
	if(rotnear<rot){turnLeft();}else{turnRight();}

	double sharkd=sqrt(getDistanceSquared(sharkList[0]));
		if(sharkd<200){
			double sharkrot=getDirToObj(sharkList[0]);
			if(sharkrot<rot+15 && sharkrot>rot-15){
				brake();brake();
				rot=fmod(rot+180,360);

			}else{
				accelerate();accelerate();accelerate();
			}
		}

	//hit shark. die();
	for(std::vector<Shark>::iterator it=sharkList.begin();it!=sharkList.end();++it){
		if(this->checkCollision(&*it)){
			health-=.25;
			texture=this->deadtex;
		}
	}
	health-=.003;
	if(health<0){
		die();
			//
	}


}


Food::Food(Texture* tex,int xposition,int yposition, double rotation,double turnRate,double acceleration,double vmax,double brakerate,double friction){
	this->texture=tex;
	this->turnRate=turnRate;
	this->acceleration=acceleration;
	this->vmax=vmax;
	this->brakerate=brakerate;
	this->friction=friction;
	this->x=xposition;
	this->y=yposition;
	this->v=0;
	this->rot=rotation;
	this->w=tex->getWidth();
	this->h=tex->getHeight();
	this->cw=w/2;
	this->ch=h/2;
	int checkCount=0;
	while(checkCollisionList(rockList) && checkCount<SCREEN_WIDTH*SCREEN_HEIGHT){

		x=fmod(x+1,SCREEN_WIDTH);
		if(x<1){y=fmod(y+1,SCREEN_HEIGHT);}
		checkCount++;
	}
	//std::cout<<checkCount<<std::endl;
}
void Food::update(){
	//std::cout<<"checking"<<std::endl;
	GameObj::update();
	for(std::vector<Fish>::iterator it=fishList.begin();it!=fishList.end();++it){
		if(this->checkCollision(&*it)){
			Fish hi=*it;
			hi.feedFood();

			x=rand()%SCREEN_WIDTH;
			y=rand()%SCREEN_HEIGHT;
			int checkCount=0;
			while(checkCollisionList(rockList) && checkCount<SCREEN_WIDTH*SCREEN_HEIGHT){
				x=fmod(x+1,SCREEN_WIDTH);
				if(x<1){y=fmod(y+1,SCREEN_HEIGHT);}
				checkCount++;
			}
		}
	}

}





