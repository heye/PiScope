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

class mouse{
	private:
		int mWidth;
		int mHeight;
		int mX;
		int mY;
		int mDown;
	public:
		mouse(int, int);
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
		Button(int, int, int, int, int, int, const char*);
		void draw(void);
		void update(mouse&);
		int getPressed();
};


class Graph{
	private:
		uint8_t* data;
		int mLen;		
		int mWidth;
		int mHeight;
		
		int mOffsetY;
		int mMidpoint;
		
		VGfloat* mX;
		VGfloat* mY;
		
		int mRed;
		int mGreen;
		int mBlue;
		
		void moveY();
		void drawMark(int x, int y);
	public:
		Graph(int width, int height, int offsetY, int midpoint);
		~Graph();
		void setData(uint16_t*, int);
		//void setYOffset(float offset);
		void draw();
		
		//void setOffset(int);
};



class Poti{
	private:
		float mPhi;
		int mWidth;
		int mHeight;
		int mX;
		int mY;
		int mDotx;
		int mDoty;
		int mWrap;
	public:
		Poti(int width, int height, int posX, int posY);
		Poti(int width, int height, int posX, int posY, int wrap);
		void draw();
		void update(mouse&);
		float getValue();
};

/*class Mark{
	private:
	public:
}*/


#endif