#include "guielements.h"

//Fontinfo SansTypeface;


mouse::mouse(int width, int height){
	mWidth = width;
	mHeight = height;
	mX = width/2;
	mY = height/2;
	mDown = 0;
}
int mouse::getX(){
	return mX;
}
int mouse::getY(){
	return mY;
}
int mouse::getDown(){
	return mDown;
}
void mouse::update(){
	SDL_Event event;
	while( SDL_PollEvent( &event ) ){                
		switch( event.type ){
         	case SDL_MOUSEMOTION:              
           		mX += event.motion.xrel;
           		mY -= event.motion.yrel;
                		
           		if(mX < 0) mX = 0;
           		if(mX > mWidth) mX = mWidth;
           		if(mY < 0 ) mY = 0;
           		if(mY > mHeight) mY = mHeight;    
                		
           		break;
          	case SDL_MOUSEBUTTONDOWN:            		
           		mDown = 1;
                break;
          	case SDL_MOUSEBUTTONUP:            		
          		mDown = 0;
                break;
            default:
                break;
        }
    }
}
void mouse::draw(){
	Stroke(255,255,255, 1);
	if(mDown){
		Fill(255, 255, 255, 0.5);	
		Circle(mX, mY, 25);
	}
	else{
		Fill(0, 0, 0, 0);	
		Circle(mX, mY, 25);
	}
}




Button::Button(int width, int height, int posX, int posY, int sizeX, int sizeY, const char* text){
	mText = text;
	mWidth = width;
	mHeight = height;
	mSizeX = sizeX; 
	mSizeY = sizeY;
	mPosX2 = posX + sizeX;
	mPosY2 = posY + sizeY;
	mPosX1 = posX; 
	mPosY1 = posY;
	mActive = 0;
	mPressed = 0;
}
void Button::draw(){
	Stroke(255,255,255, 1);
	
	if(mActive)
		Fill(120, 120, 120, 1);
	else
		Fill(0, 0, 0, 1);
	
	Roundrect(mPosX1,mPosY1,mSizeX, mSizeY, 5, 5);
	Fill(255, 100, 100, 1);
	
	//Fontinfo SansTypeface, SerifTypeface, MonoTypeface;
	TextMid((mPosX1 + mSizeX/2), (mPosY1 + mSizeY/2 - 5), (char*)mText,SansTypeface, 10);
}
void Button::update(mouse& cursor){
	if(mActive == 1 && !cursor.getDown())
		mPressed = 1;
	else
		mPressed = 0;
		
	if(cursor.getDown() && cursor.getX() >= mPosX1 && cursor.getX() <= mPosX2 && cursor.getY() >= mPosY1 && cursor.getY() <= mPosY2)
		mActive = 1;
	else
		mActive = 0;
}
int Button::getPressed(){
	return mPressed;
}





Graph::Graph(int width, int height){
	mWidth = width;
	mHeight = height;
	mX = new VGfloat[mWidth];
	mY = new VGfloat[mWidth];
	
	for(int i = 0; i<mWidth; i++){
		mX[i] = i;
		mY[i] = height/2;
	}
	
	
}
Graph::~Graph(){
	delete mX;
	delete mY;
}

void Graph::setData(uint8_t* newData, int len){
	if(len > mWidth)
		len = mWidth;
	for(int i = 0; i < len; i++){
		mY[i] = (float)newData[i];
	}
}

void Graph::draw(){
	Stroke(255,255,0,1); // red green blue alpha
	StrokeWidth(1);
	
	int div = mWidth/256;
	int r = mWidth%256;
	
	for(int i = 0; i<div; i++){
		Polyline(mX+i*256,mY+i*256, 256);	
	}
	if(r > 0)
		Polyline(mX+(mWidth-r), mY + (mWidth-r), r);
}