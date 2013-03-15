#include "guielements.h"

//Fontinfo SansTypeface;
using namespace std;

Mouse::Mouse(int width, int height){
	mWidth = width;
	mHeight = height;
	mX = width/2;
	mY = height/2;
	mDown = 0;
}
int Mouse::getX(){
	return mX;
}
int Mouse::getY(){
	return mY;
}
int Mouse::getDown(){
	return mDown;
}
void Mouse::update(){
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
void Mouse::draw(){
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
void Button::update(Mouse& cursor){
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





Graph::Graph(int width, int height, int offsetY, int midpoint, int red, int green, int blue){
	mWidth = width;
	mHeight = height;
	mOffsetY = offsetY;
	mMidpoint = midpoint;
	mTrigger.setMid(mMidpoint);
	
	mScale = 1;
	
	mOffsetX = 512;
	
	//mData = new uint16_t[BUFFLEN];
	
	mX = new VGfloat[mWidth];
	mY = new VGfloat[mWidth];
	
	for(int i = 0; i<mWidth; i++){
		mX[i] = i;
		mY[i] = height/2;
	}
	
	mRed = red;
	mGreen = green;
	mBlue = blue;
}
Graph::~Graph(){
	delete mX;
	delete mY;
}

void Graph::setData(uint16_t* newData, int len){
	mTrigger.setData(newData, len);

	if(len > BUFFLEN)
		len = BUFFLEN;
	mData = newData;
	/*for(int i = 0; i < len; i++){
		mData[i] = newData[i];
	}*/
}

void Graph::setOffsetY(int offset){
	mOffsetY = offset;
}

void Graph::setOffsetX(int offset){
	mOffsetX = offset;
}

void Graph::setTriggerValue(int value){
	mTrigger.setValue(value);
}

void Graph::draw(){
	//shift data (trigger / Y offset)
	
	int drawOffset=0;
	
	if(mTrigger.getPos() == -1){
		for(int i = 0 ; i<mWidth; i++){
			mY[i] = (mData[i] - mMidpoint)*mScale + mOffsetY;
		}
	}	
	else if(mTrigger.getPos()<mOffsetX){
		drawOffset = mOffsetX - mTrigger.getPos();
		for(int i = drawOffset, j=0; j<BUFFLEN, i<mWidth; j++, i++){
			mY[i] = (mData[j] - mMidpoint)*mScale + mOffsetY;
		}
	}
	else if(mTrigger.getPos()>mOffsetX){
		for(int i = 0, j=mTrigger.getPos()-mOffsetX; j<BUFFLEN, i<mWidth; j++, i++){
			mY[i] = (mData[j] - mMidpoint)*mScale + mOffsetY;
		}
	}
	
	
	//trigger bubble
	Stroke(mRed,mGreen,mBlue,1); // red green blue alpha
	Fill(0, 0, 0, 0);	
	Circle(mOffsetX, mTrigger.getValue()*mScale+mOffsetY, 25);

	//draw graph
	Stroke(mRed,mGreen,mBlue,1); // red green blue alpha
	StrokeWidth(1);
	
	int div = (mWidth-drawOffset)/256;
	int r = (mWidth-drawOffset)%256 ;
	
	for(int i = 0; (i*256+drawOffset+256) <= mWidth; i++){
		Polyline(mX+i*256+drawOffset,mY+i*256+drawOffset, 256);	
	}
	if(r > 0)
		Polyline(mX+(mWidth-r), mY + (mWidth-r), r);
	
	drawMark(mWidth, mOffsetY);
	
}

void Graph::setScale(float scale){
	mScale = scale;
}

void Graph::drawMark(int x, int y){
	Stroke(mRed,mGreen,mBlue,1); // red green blue alpha
	StrokeWidth(1);
	
	VGfloat markX[] = {x, x-12, x};
	VGfloat markY[] = {y+8, y, y-8};
	
	Polyline(markX, markY, 3);	
	
}









/*Poti::Poti(int width, int height, int posX, int posY){
	mWidth = width;
	mHeight = height;
	mX = posX;
	mY = posY;
	mWrap = 0;
	mPhi = 0;
	mDotx = mX + cos(mPhi)*25;
	mDoty = mY + sin(mPhi)*25;
}*/
Poti::Poti(int width, int height, int posX, int posY, string text){
	mWidth = width;
	mHeight = height;
	mX = posX;
	mY = posY;
	mWrap = 0;
	mPhi = 0;
	mDotx = mX + cos(mPhi)*25;
	mDoty = mY + sin(mPhi)*25;
	
	mText = new char[text.size()+1];
	mText[text.size()]=0;
	memcpy(mText,text.c_str(),text.size());

	mUnit = new char[1];
	mUnit[0] = 0;
	
	mFactor = 1;
	mValue = 0;
}
Poti::Poti(int width, int height, int posX, int posY,  string text, string unit){
	mWidth = width;
	mHeight = height;
	mX = posX;
	mY = posY;
	mWrap = 0;
	mPhi = 0;
	mDotx = mX + cos(mPhi)*25;
	mDoty = mY + sin(mPhi)*25;
	
	mText = new char[text.size()+1];
	mText[text.size()]=0;
	memcpy(mText,text.c_str(),text.size());
	
	mUnit = new char[unit.size()+1];
	mUnit[unit.size()]=0;
	memcpy(mUnit,unit.c_str(),unit.size());

	
	mFactor = 1;
	mValue = 0;
}
/*Poti::Poti(int width, int height, int posX, int posY, const char* text, int wrap){
	mWidth = width;
	mHeight = height;
	mX = posX;
	mY = posY;
	mWrap = 1;
	mPhi = 0;
	mDotx = mX + cos(mPhi)*25;
	mDoty = mY + sin(mPhi)*25;
	mText = text;
	//mFactor = 0;
	mValue = 0;
}*/
void Poti::draw(){
	Stroke(255,255,255,1); // red green blue alpha
	StrokeWidth(1);
	Fill(255, 255, 255, 0.1);	
	
	Circle(mX, mY, 50);
	Line(mX-10, mY, mX+10, mY);
	
	StrokeWidth(0);
	Fill(0, 255, 0, 1);
	Circle(mDotx, mDoty, 10);
	
	
	Fill(255, 255, 255, 1);
	TextMid(mX, mY+30, mText, SansTypeface, 10);
	
	
	char buff[64];	
	sprintf(buff, "%d %s", mValue, mUnit);
	TextMid(mX, mY-40, buff, SansTypeface, 10);
	
}
void Poti::setFactor(float factor){
	mFactor = factor;
}
void Poti::setValue(int value){
	mValue = value;
	mPhi = mValue/mFactor;
}
void Poti::update(Mouse& cursor){
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
	
	mValue = mPhi*mFactor;
	
	mDotx = mX + cos(mPhi)*25; //dont calculate dot position in every fram for better performance
	mDoty = mY + sin(mPhi)*25;
}
float Poti::getValue(){
	//return mPhi;
	return mValue;
}



Menu::Menu(int width, int height, int posX, int posY, Mouse* cursor){
	mWidth = width;
	mHeight = height;
	mPosX = posX;
	mPosY = posY;
	mSizeX = 500;
	mSizeY = 200;
	mVpp = 2;
	mCursor = cursor;
	
	mCHAVertShiftPt = new Poti(mWidth, mHeight, mPosX+50, mPosY+50, "CHA-vert-shift", "px");
	mCHAVertShiftPt->setFactor(-256/3.141);
	mCHAVertShiftPt->setValue(mHeight/2+50);
	
	mCHBVertShiftPt = new Poti(mWidth, mHeight, mPosX+50, mPosY+150, "CHB-vert-shift", "px");
	mCHBVertShiftPt->setFactor(-256/3.141);
	mCHBVertShiftPt->setValue(mHeight/2-50);
	
	mCHAVertDivPt = new Poti(mWidth, mHeight, mPosX+150, mPosY+50, "CHA-vert-div", "mV/div");
	mCHAVertDivPt->setFactor(-64/3.14159);
	mCHAVertDivPt->setValue(100);	//default 100mV/Div
	
	mCHBVertDivPt = new Poti(mWidth, mHeight, mPosX+150, mPosY+150, "CHB-vert-div", "mV/div");
	mCHBVertDivPt->setFactor(-64/3.141);
	mCHBVertDivPt->setValue(100);	//default 100mV/Div
	
	mHorzShiftPt = new Poti(mWidth, mHeight, mPosX+250, mPosY+50, "Horizontal Shift", "px");
	mHorzShiftPt->setFactor(-256/3.141);
	
	
	
}
void Menu::draw(){
	//frame
	Stroke(255,255,255, 1);
	StrokeWidth(1);
	
	//black background
	Fill(0, 0, 0, 1);
	
	Roundrect(mPosX,mPosY,mSizeX, mSizeY, 10, 10);
	
	//Buttons & stuff
	mCHAVertShiftPt->draw();	
	mCHBVertShiftPt->draw();	
	mCHAVertDivPt->draw();	
	mCHBVertDivPt->draw();	
	mHorzShiftPt->draw();

}

void Menu::update(){
	mCHAVertShiftPt->update(*mCursor);
	mCHBVertShiftPt->update(*mCursor);
	mCHAVertDivPt->update(*mCursor);
	mCHBVertDivPt->update(*mCursor);
	mHorzShiftPt->update(*mCursor);
}

int Menu::getCHAVertShift(){
	return mCHAVertShiftPt->getValue();
}
int Menu::getCHBVertShift(){
	return mCHBVertShiftPt->getValue();
}
float Menu::getCHAVertScale(){
	return 100000/(1024*(mCHAVertDivPt->getValue()));
}
float Menu::getCHBVertScale(){
	return 100000/(1024*(mCHBVertDivPt->getValue()));
}
int Menu::getHorzShift(){
	return mHorzShiftPt->getValue();
}
int Menu::getHorzDiv(){
	return mHorzDivPt->getValue();
}




Grid::Grid(int width, int height, int div){
	mWidth = width;
	mHeight = height;
	mDiv = div;
	
	mLen = (mWidth/mDiv)*2 + 1 + (mHeight/mDiv)*2 +1 +2;
	//mLen = (mHeight/mDiv)*2 +1;
	
	mX = new VGfloat[mLen];
	mY = new VGfloat[mLen];
	
	
	for(int i = 0, j=0 ; i <= mWidth/mDiv; i++, j+=mDiv){
		if(i%2 == 0){
			mX[2*i] = j;
			mX[2*i+1] = j;
			mY[2*i] = 0;
			mY[2*i+1] = mHeight;
		}
		else{
			mX[2*i] = j;
			mX[2*i+1] = j;
			mY[2*i] = mHeight;
			mY[2*i+1] = 0;
		}
	}
	
	for(int i = (mWidth/mDiv)+1, j=0 ; i <= (mWidth/mDiv + mHeight/mDiv)+1; i++, j+=mDiv){
		if(i%2 == 0){
			mX[2*i] = 0;
			mX[2*i+1] = mWidth;
			mY[2*i] = j;
			mY[2*i+1] = j;
		}
		else{
			mX[2*i] = mWidth;
			mX[2*i+1] = 0;
			mY[2*i] = j;
			mY[2*i+1] = j;
		}
	}	
	
	
	
	
}
void Grid::draw(){
	Stroke(100,100,100,1); // red green blue alpha
	StrokeWidth(1);
	
	
	Polyline(mX, mY, mLen);	
}



