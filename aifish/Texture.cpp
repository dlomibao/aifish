#include "Texture.h"
Texture::Texture(){mTexture=NULL;mWidth=0;mHeight=0;}
Texture::Texture(std::string path){Texture();loadFromFile(path);}
Texture::~Texture(){free();}
bool Texture::loadFromFile(std::string path){
	free();//free any existing texture
	SDL_Texture* newtex=NULL;
	SDL_Surface* surf=IMG_Load(path.c_str());
	if(surf==NULL){std::cout<<"failed to load img"<<path<<IMG_GetError()<<std::endl;}
	else{
		SDL_SetColorKey(surf,SDL_TRUE,SDL_MapRGB(surf->format,0,0xFF,0xFF));
		newtex=SDL_CreateTextureFromSurface(renderer,surf);
		if(newtex==NULL){std::cout<<"failed to make texture "<<path<<SDL_GetError()<<std::endl;}
		else{
			mWidth=surf->w;
			mHeight=surf->h;
		}
		SDL_FreeSurface(surf);
	}
	mTexture=newtex;
	return mTexture!=NULL;
}
void Texture::free(){
	if(mTexture!=NULL){
		SDL_DestroyTexture(mTexture);
		mTexture=NULL;
		mWidth=0;
		mHeight=0;
	}
}
void Texture::setColor(Uint8 r,Uint8 g,Uint8 b){
	SDL_SetTextureColorMod(mTexture,r,g,b);
}
void Texture::setBlendMode(SDL_BlendMode blending){SDL_SetTextureBlendMode(mTexture,blending);}
void Texture::setAlpha(Uint8 alpha){
	SDL_SetTextureAlphaMod(mTexture,alpha);
}
void Texture::render(int x,int y,SDL_Rect*clip,double angle, SDL_Point* center, SDL_RendererFlip flip){
	SDL_Rect renderQuad={x,y,mWidth,mHeight};
	if(clip!=NULL){
		renderQuad.w=clip->w;
		renderQuad.h=clip->h;
	}
	SDL_RenderCopyEx(renderer,mTexture,clip,&renderQuad,angle,center,flip);
}
int Texture::getWidth(){return mWidth;}
int Texture::getHeight(){return mHeight;}
