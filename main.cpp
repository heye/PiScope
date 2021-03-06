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

#define BUFFLEN 4096
#define GRID_WIDTH 50
#define SAMPLERATE 50 //in MSPS

using namespace std;






//Fontinfo SerifTypeface, MonoTypeface;



 

void drawFPS(int x, int y){
	static int lastTime = 0;
	char buff[50];
	
	int currentTime = SDL_GetTicks();
	sprintf(buff, "FPS: %d \n", 1000/ (currentTime - lastTime));
	
	Fill(255, 255, 255, 1);
    TextMid(x, y, buff, SerifTypeface, 10);  // Greetings 
    
    lastTime = currentTime;
}






int main ()
{
	//variable required for initialising
    int width, height, pause;
    pause = 0;

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
 	Button pauseButton(width, height-30, width-80, height-60, 80, 30, "PAUSE");
 	
 	Mouse cursor(width, height);
 	
 	Graph g(width, height, 512, 605, 250, 255, 0);
 	g.setTriggerValue(0);
 	Graph g2(width, height, 700, 605, 0, 255, 250);
 	g2.setTriggerValue(0);
 	

    spi spiConnection(16000000);    
    Grid grid(width, height, GRID_WIDTH);
    
    Menu menu(width, height, (width/2)-250, 0, &cursor);
        
    Background(0, 0, 0);    
    
    	
    End();
    
   	int quitv = 0; 
    while(quitv == 0){
    	//#######################get and process data#########################  
    	
    	if(pauseButton.getPressed()){
    		pause = (pause+1)%2;
    	}   	
    	if(pause == 0){
    		spiConnection.read();
    		g.setData(spiConnection.getCHB(), spiConnection.getBuffLen());
    		g2.setData(spiConnection.getCHA(), spiConnection.getBuffLen());
    	}
    	
    	//channel A 
    	g.setTriggerValue(menu.getTriggerValue());    	
    	g.setOffsetY(menu.getCHAVertShift());
    	g.setOffsetX(menu.getHorzShift());
    	g.setScale(menu.getCHAVertScale());
    	
    	//channel B    	
    	g2.setTriggerValue(menu.getTriggerValue());    
    	g2.setOffsetX(menu.getHorzShift());    	
		g2.setOffsetY(menu.getCHBVertShift());
    	g2.setScale(menu.getCHBVertScale());
		
    	//other buttons + cursor
    	cursor.update();
    	pauseButton.update(cursor);
    	quit.update(cursor);
    	spiConnection.setSampleDelay(menu.getHorzDiv()*(SAMPLERATE/GRID_WIDTH)-1);
    	
    	if(quit.getPressed()){
    		quitv = 1;
    	}
    	
    	//menu
    	menu.update();
    	 
    	//################################draw #############################
    	Background(0, 0, 0);
    	
    	grid.draw();
    	
    	g.draw();
    	g2.draw();
    	
    	quit.draw();
    	pauseButton.draw();
    	
    	drawFPS(width-100, height-100);    	
    	
    	menu.draw();
    	
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