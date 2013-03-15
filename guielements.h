#ifndef GUIELEMENTS_H
#define GUIELEMENTS_H

//openVG shit
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"
#include <math.h>

//SDL shit 
#include "SDL.h"

//anderer shit
#include "trigger.h"
#include <string.h>
#include <string>

#ifndef BUFFLEN
#define BUFFLEN 4096
#endif


using namespace std;


class Mouse{
	private:
		int mWidth;
		int mHeight;
		int mX;
		int mY;
		int mDown;
	public:
		Mouse(int, int);
		void draw();
		void update();
		int getX();
		int getY();
		int getDown();
};


/*****
*
* Button(width, height, posx, posy, sizex, sizey, text)
*	width: 	picture width
*	height: picture height
*	posx: 	lower left x
*	posy: 	lower left y
*	sizex: 	button width
*	sizey: 	button height
*	text: 	button text
*	
*
****/
class Button{
	private:
		int mWidth;
		int mHeight;
		int mSizeX;
		int mSizeY;
		int mPosX1;	//lower left
		int mPosY1;
		int mPosX2;	//upper right
		int mPosY2;
		int mActive;
		int mPressed;
		const char* mText;
	public:
		Button(int width, int height, int posX, int posY, int sizeX, int sizeY, const char* text);
		void draw(void);
		void update(Mouse&);
		int getPressed();
};


class Graph{
	private:
		//uint8_t* data;
		//int mLen;		
		Trigger mTrigger;
		
		int mWidth;
		int mHeight;
		
		int mOffsetY;
		int mOffsetX;
		int mMidpoint;
		
		float mScale;
		
		uint16_t* mData;
		
		VGfloat* mX;
		VGfloat* mY;
		
		int mRed;
		int mGreen;
		int mBlue;
		
		void moveY();
		void drawMark(int x, int y);
	public:
		Graph(int width, int height, int offsetY, int midpoint, int red, int green, int blue);
		~Graph();
		void setData(uint16_t*, int);
		void setOffsetY(int offset);
		void setOffsetX(int offset);
		void setTriggerValue(int pos);
		void setScale(float scale);
		//void setYOffset(float offset);
		void draw();
		
		//void setOffset(int);
};


class Poti{
	private:
		float mPhi;
		int mValue;
		float mFactor;
		int mWidth;
		int mHeight;
		int mX;
		int mY;
		int mDotx;
		int mDoty;
		int mWrap;
		//char mText[64];
		char* mText;
		char* mUnit;
	public:
		//Poti(int width, int height, int posX, int posY);
		Poti(int width, int height, int posX, int posY,  string text);
		Poti(int width, int height, int posX, int posY,  string text,  string unit);
		//Poti(int width, int height, int posX, int posY, const char* text, int wrap);
		void setFactor(float factor);
		void setValue(int value);
		void draw();
		void update(Mouse&);
		float getValue();
};


class Grid{
	private:
		int mWidth;
		int mHeight;
		int mDiv;
		VGfloat* mX;
		VGfloat* mY;
		int mLen;
	public:
		Grid(int width, int height, int div);
		void draw();
};


class Menu{
	private:	
		//intern
		int mWidth;
		int mHeight;
		
		int mPosX;
		int mPosY;
		int mSizeX;
		int mSizeY;
		
		//Buttons & stuff
		Mouse* mCursor;
		
		Poti* mCHAVertShiftPt;
		Poti* mCHBVertShiftPt;
		Poti* mCHAVertDivPt;
		Poti* mCHBVertDivPt;
		
		Poti* mHorzShiftPt;
		Poti* mHorzDivPt;
		
		Poti* mTriggerValuePt;
		
		//Poti* mTrigger
		
		//data
		int mVpp;
		
	public:
		Menu(int width, int height, int posX, int posY, Mouse* cursor);
		
		int getCHAVertShift();
		int getCHBVertShift();
		int getTriggerValue();
		float getCHAVertScale();
		float getCHBVertScale();
		
		int getHorzShift();
		int getHorzDiv();
		
		void update();
		
		void draw();
		
};


/*class Mark{
	private:
	public:
}*/


#endif