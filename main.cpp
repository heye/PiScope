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
 	Graph g(width, height, 512, 512);
    Poti pot(width, height, 100, 100);
    Poti pot2(width, height, 200, 100);
    Poti pot3(width, height, 300, 100);
    Poti pot4(width, height, 400, 100);
    Poti pot5(width, height, 500, 100);
    spi spiConnection(16000000);    
    
        
    Background(0, 0, 0);    
    
    drawGrid(50, width, height);	
	
    End();
    
   	int quitv = 0; 
    while(quitv == 0){
    	//#######################get and process data#########################
    	
    	spiConnection.read();
    	//spiConnection.print();
    	g.setData(spiConnection.getCHA(), spiConnection.getBuffLen());
    	
    	cursor.update();
    	pot.update(cursor);
    	pot2.update(cursor);
    	pot3.update(cursor);
    	pot4.update(cursor);
    	pot5.update(cursor);
    	quit.update(cursor);
    	
    	if(quit.getPressed()){
    		quitv = 1;
    	}
    	 
    	//################################draw #############################
    	Background(0, 0, 0);
    	drawGrid(50, width, height);
    	
    	g.draw();
    	pot.draw();
    	pot2.draw();
    	pot3.draw();
    	pot4.draw();
    	pot5.draw();
    	quit.draw();
    	
    	drawFPS(width-100, height-100);
    	
    	Stroke(255,255,0,1); 
		StrokeWidth(1);
    	
    	
    	cursor.draw();    	 
    
    	End();
    }
	
	
	finish();                               // Graphics cleanup
    exit(0);
	
	
  	//cout << "Hello World!";
  	return 0;
}