// my first program in C++

#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"
#include "SDL.h"
#include "spi.h"

#include "guielements.h"

using namespace std;






//Fontinfo SerifTypeface, MonoTypeface;






 

void drawFPS(int x, int y){
	static int lastTime = 0;
	char buff[50];
	
	int currentTime = SDL_GetTicks();
	sprintf(buff, "FPS: %d \n", 1000/ (currentTime - lastTime));
	
	Fill(255, 255, 255, 1);
	
	//Fontinfo SerifTypeface;
	
	//Fontinfo SansTypeface, SerifTypeface, MonoTypeface;
    TextMid(x, y, buff, SerifTypeface, 10);  // Greetings 
    
    lastTime = currentTime;
}

void drawGrid(int div, int width, int height){
	int i;
	
	Stroke(255,255,255, 0.5);
	StrokeWidth(1);
	
	for(i= 0; i < width; i+=div){
		Line(i, 0, i, height);
	}
	for(i = 0; i < height; i+=div){
		Line(0,i, width,i);
	}
}

int main ()
{
	//variable required for initialising
    int width, height;

	//initialise SDL
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }    
    
	// initialise openVG
    init(&width, &height);                  // Graphics initialization
    Start(width, height);                   // Start the picture
            
    //create buttons, etc...
    Button quit(width, height, width-80, height-30, 80, 30, "QUIT");
 	mouse cursor(width, height);
    
    spi spiConnection(16000000);    
    
        
    Background(0, 0, 0);    
    
    drawGrid(50, width, height);	
	
    End();
    
   	int quitv = 0; 
    while(quitv == 0){
    	//get and process data
    	
    	spiConnection.read();
    	  
    	//draw 
    	Background(0, 0, 0);    
    	
    	drawGrid(50, width, height);
    	
    	quit.update(cursor);
    	quit.draw();
    	if(quit.getPressed()){
    		quitv = 1;
    	}
    	
    	drawFPS(width-100, height-100);
    	
    	Stroke(255,255,0,1); 
		StrokeWidth(1);
    	
    	cursor.update();
    	cursor.draw();    	 
    
    	End();
    }
	
	
	finish();                               // Graphics cleanup
    exit(0);
	
	
  	//cout << "Hello World!";
  	return 0;
}