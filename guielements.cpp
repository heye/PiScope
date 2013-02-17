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
	Line(mX-10, mY, mX+10, mY);
	Line(mX, mY-10, mX, mY+10);
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
	StrokeWidth(1);
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





Graph::Graph(int width, int height, int offsetY, int midpoint){
	mWidth = width;
	mHeight = height;
	mOffsetY = offsetY;
	mMidpoint = midpoint;
	
	mX = new VGfloat[mWidth];
	mY = new VGfloat[mWidth];
	
	for(int i = 0; i<mWidth; i++){
		mX[i] = i;
		mY[i] = height/2;
	}
	
	mRed = 255;
	mGreen = 0;
	mBlue = 255;
}
Graph::~Graph(){
	delete mX;
	delete mY;
}

void Graph::setData(uint16_t* newData, int mLen){
	if(mLen > mWidth)
		mLen = mWidth;
	for(int i = 0; i < mLen; i++){
		mY[i] = (float)newData[i] - (mMidpoint - mOffsetY);
	}
}

void Graph::draw(){
	//moveY();

	Stroke(mRed,mGreen,mBlue,1); // red green blue alpha
	StrokeWidth(1);
	
	int div = mWidth/256;
	int r = mWidth%256;
	
	for(int i = 0; i<div; i++){
		Polyline(mX+i*256,mY+i*256, 256);	
	}
	if(r > 0)
		Polyline(mX+(mWidth-r), mY + (mWidth-r), r);
	
	drawMark(mWidth, mOffsetY);
	
}

/*void Graph::moveY(){
	for(int i = 0; i < mLen; i++){
		mY[i] += mOffsetY;
		mY[i] -= mMidpoint;
	}
}*/

void Graph::drawMark(int x, int y){
	Stroke(mRed,mGreen,mBlue,1); // red green blue alpha
	StrokeWidth(1);
	
	VGfloat markX[] = {x, x-12, x};
	VGfloat markY[] = {y+8, y, y-8};
	
	Polyline(markX, markY, 3);	
	
}








Poti::Poti(int width, int height, int posX, int posY){
	mWidth = width;
	mHeight = height;
	mX = posX;
	mY = posY;
	mWrap = 0;
	mPhi = 0;
	mDotx = mX + cos(mPhi)*25;
	mDoty = mY + sin(mPhi)*25;
}
Poti::Poti(int width, int height, int posX, int posY, int wrap){
	mWidth = width;
	mHeight = height;
	mX = posX;
	mY = posY;
	mWrap = 1;
	mPhi = 0;
	mDotx = mX + cos(mPhi)*25;
	mDoty = mY + sin(mPhi)*25;
}
void Poti::draw(){
	Stroke(255,255,255,1); // red green blue alpha
	StrokeWidth(1);
	Fill(255, 255, 255, 0.1);	
	
	Circle(mX, mY, 50);
	Line(mX-10, mY, mX+10, mY);
	
	StrokeWidth(0);
	Fill(0, 255, 0, 1);
	Circle(mDotx, mDoty, 10);
}
void Poti::update(mouse& cursor){
	if(cursor.getDown() && cursor.getX() < mX+20 && cursor.getX() > mX-20 &&
		cursor.getY() < mY && cursor.getY() > mY -20){
		mPhi += 0.08;
	}
	else if(cursor.getDown() && cursor.getX() < mX+20 && cursor.getX() > mX-20 &&
		cursor.getY() < mY + 20 && cursor.getY() > mY){
		mPhi -= 0.08;
	}
	
	//make wrap around
	if(mWrap){
		if(mPhi > 6.28)
			mPhi -= 6.28;
		if(mPhi < 0)
			mPhi += 6.28;
	}
	mDotx = mX + cos(mPhi)*25; //dont calculate dot position in every fram for better performance
	mDoty = mY + sin(mPhi)*25;
}
float Poti::getValue(){
	return mPhi;
}



