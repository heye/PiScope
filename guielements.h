#ifndef GUIELEMENTS_H
#define GUIELEMENTS_H

//openVG shit
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

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
		int len;		
		int mWidth;
		int mHeight;
		VGfloat* mX;
		VGfloat* mY;
	public:
		Graph(int, int);
		~Graph();
		void setData(uint8_t*, int);
		void draw();
		
		//void setOffset(int);
};


#endif