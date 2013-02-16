#CC=g++
#CPPFLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg -I/usr/include/SDL -D_GNU_SOURCE -D_REENTRANT -L/usr/lib/arm-linux-gnueabi -lSDL

#libshapes.o: libshapes.c
#	gcc -c libshapes.c -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg 

#oglinit.o: oglinit.c
#	gcc -c oglinit.c -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg 

#all: main.o libshapes.o oglinit.o
#	$(CC) -o out main.cpp main.o libshapes.o oglinit.o -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg -I/usr/include/SDL -D_GNU_SOURCE -D_REENTRANT -L/usr/lib/arm-linux-gnueabi -lSDL
	






CC = g++ 
#CPPFLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg -I/usr/include/SDL -D_GNU_SOURCE -D_REENTRANT -L/usr/lib/arm-linux-gnueabi -lSDL
LIBS = -L/opt/vc/lib -lGLESv2 -ljpeg -L/usr/lib/arm-linux-gnueabi -lSDL
INCL = -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/usr/include/SDL 
MAIN = main.cpp
OGL = oglinit.c
SHAPESH = shapes.h

all: main

main: libshapes.o oglinit.o spi.o guielements.o main.o fontinfo.o $(SHAPESH) 
	$(CC) -o $@ libshapes.o oglinit.o spi.o guielements.o main.o fontinfo.o $(LIBS) $(INCL) 

main.o: $(SHAPESH) $(MAIN) 
	$(CC) -o $@ -c $(MAIN) $(LIBS) $(INCL) 
	
#    $(CC) -o $@ libshapes.o oglinit.o main.o $(LIBS)

#main.o: $(SDLFH) $(MAIN) $(CLASSH)
#    $(CC) -o $@ -c $(MAIN)

#shapes.o: shapes.h
#	gcc -c shapes.h -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg 

libshapes.o: libshapes.c $(SHAPESH) 
	gcc -c libshapes.c -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg 

oglinit.o: oglinit.c $(SHAPESH) 
	gcc -c oglinit.c -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg 

spi.o: spi.cpp spi.h 
	g++ -c spi.cpp 

fontinfo.o: fontinfo.cpp fontinfo.h
	g++ -c fontinfo.cpp -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -L/opt/vc/lib -lGLESv2 -ljpeg 
	
#guielements.o: guielements.cpp guielements.h libshapes.o 
#	g++ -o guielements.cpp libshapes.o $(LIBS) $(INCL) 

guielements.o: guielements.cpp guielements.h fontinfo.h
	g++ -o $@ -c guielements.cpp $(LIBS) $(INCL) 