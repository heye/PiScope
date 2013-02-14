// first OpenVG program
// Anthony Starks (ajstarks@gmail.com)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"
#include "SDL.h"

#include <iostream>
using namespace std;


void drawGrid(int div, int width, int height){
	int i;
	
	Stroke(255,255,255,0.5);
	StrokeWidth(1);
	
	for(i= 0; i < width; i+=div){
		Line(i, 0, i, height);
	}
	for(i = 0; i < height; i+=div){
		Line(0,i, width,i);
	}
}

void drawFPS(int x, int y){
	static int lastTime = 0;
	char buff[50];
	
	int currentTime = SDL_GetTicks();
	sprintf(buff, "FPS: %d \n", 1000/ (currentTime - lastTime));
	
	Fill(255, 255, 255, 1); 
    TextMid(x, y, buff, SerifTypeface, 10);  // Greetings 
    
    lastTime = currentTime;
}

void drawMouse(int x, int y){
	Circle(x, y, 50);
}

void mouse(int width, int height){

	static int x = 0;
	static int y = 0;
	
	static int mouseDown = 0;
		
	SDL_Event event;
	int quit = 0;
	char buff[50];	
	
            while( SDL_PollEvent( &event ) ){                
                switch( event.type ){
                    /*case SDL_KEYDOWN:
                    	sprintf(buff, "KEY down");
                		TextMid(1000, 100, buff, SerifTypeface, 10);  // Greetings 
                		break;*/
            		case SDL_MOUSEMOTION:                		
                		sprintf(buff, "Mouse moved by %d,%d \n", event.motion.xrel, event.motion.yrel);
                		TextMid(1000, 100, buff, SerifTypeface, 10);  // Greetings 
                		
                		x += event.motion.xrel;
                		y -= event.motion.yrel;
                		
                		if(x < 0) x = 0;
                		if(x > width) x = width;
                		if(y <0) y = 0;
                		if(y > height) y = height;    
                		
                		break;
            		case SDL_MOUSEBUTTONDOWN:            		
            			mouseDown = 1;
                        break;
            		case SDL_MOUSEBUTTONUP:            		
            			mouseDown = 0;
                        break;
                    default:
                        break;
                }
            }
           	
    
	if(mouseDown)
		Fill(0, 255, 0, 1); 
	else
		Fill(255, 255, 255, 1); 
    drawMouse(x, y);	
}

int main() {
    int width, height;
    char s[3];

    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    init(&width, &height);                  // Graphics initialization
    Start(width, height);                   // Start the picture
        
    Background(0, 0, 0);    
    
    drawGrid(50, width, height);	
	
    End();
    
    
    
	fgets(s, 2, stdin);  
	
	VGfloat v[width];
	VGfloat y[width];
	char buff[50];
	int x = 0;
	int i;
	
	for(i = 0; i< (width); i++){
		v[i] = i+1;
	}
	
	
	while(x < 300){
		x++;
		Background(0, 0, 0); 
    	
    	drawGrid(50, width, height);
    
    	mouse(width, height);
    
    	drawFPS(width - 100, height - 100);
    	
    	/*
    	for(i = 0; i < 100; i++){
    		Fill(rand()%255, rand()%255, rand()%255, 0.5);                   // Big blue marble
    		Circle(rand()%width, rand()%height, rand()%150);            // The "world"
    	}*/
    	
    	for(i = 0; i < (width); i++){
    		y[i] = height/2 + rand()%(height/10) - height/20;
    		//y[i] = 100;
    	}
    	
		Stroke(255,255,0,1); // red green blue alpha
		StrokeWidth(1);
		
		
		
		Polyline(v,y, 256);
		Polyline(v+256,y+256, 256);
		Polyline(v+512,y+512, 256);
		Polyline(v+768,y+768, 256);
		Polyline(v+1024,y+1024, 256);

    	
    	Fill(255, 255, 255, 0.5); 
    	
    	sprintf(buff, "x = %d",x);
    	
		TextMid(x, x, buff, SerifTypeface, 10);  // Greetings 
		End();  
	}

    fgets(s, 2, stdin);                     // look at the pic, end with [RETURN]
    finish();                               // Graphics cleanup
    exit(0);
}