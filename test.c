// shapes: minimal program to explore OpenVG

// Anthony Starks (ajstarks@gmail.com)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
 

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "fontinfo.h"
#include "shapes.h"
//#include "libshapes.c"
 

typedef struct
{
	uint32_t screen_width;
	uint32_t screen_height;

	// OpenGL|ES objects
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
} STATE_T;

 

static void exit_func(void);

static STATE_T _state, *state=&_state;

 

// init_ogl sets the display, OpenGL|ES context and screen information

// state holds the OGLES model information

static void init_ogl(STATE_T *state) {
	int32_t success = 0;

	EGLBoolean result;
	EGLint num_config;
 

	static EGL_DISPMANX_WINDOW_T nativewindow;
 

	DISPMANX_ELEMENT_HANDLE_T dispman_element;	
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;

	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

 
	static const EGLint attribute_list[] = {

		EGL_RED_SIZE, 8,

		EGL_GREEN_SIZE, 8,

		EGL_BLUE_SIZE, 8,

		EGL_ALPHA_SIZE, 8,

		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,

		EGL_NONE

	};

 

	EGLConfig config;

 

	// get an EGL display connection
	state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(state->display!=EGL_NO_DISPLAY);

 

	// initialize the EGL display connection
	result = eglInitialize(state->display, NULL, NULL);
	assert(EGL_FALSE != result);

 

	// bind OpenVG API
	eglBindAPI(EGL_OPENVG_API);

 

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);

 

	// create an EGL rendering context
	state->context = eglCreateContext(state->display, config, EGL_NO_CONTEXT, NULL);
	assert(state->context!=EGL_NO_CONTEXT);

 

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */, &state->screen_width, &state->screen_height);
	assert( success >= 0 );

 

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = state->screen_width;
	dst_rect.height = state->screen_height;

 

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = state->screen_width << 16;
	src_rect.height = state->screen_height << 16;

 

	dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
	dispman_update = vc_dispmanx_update_start( 0 );

 

	dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,

	0/*layer*/, &dst_rect, 0/*src*/,

	&src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, 0/*transform*/);

 

	nativewindow.element = dispman_element;
	nativewindow.width = state->screen_width;
	nativewindow.height = state->screen_height;

	vc_dispmanx_update_submit_sync( dispman_update );

 

	state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
	assert(state->surface != EGL_NO_SURFACE);

 

	// connect the context to the surface
	result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);	
	assert(EGL_FALSE != result);

 

	//DAVE - Set up screen ratio
	glViewport(0, 0, (GLsizei)state->screen_width, (GLsizei)state->screen_height);

 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

 

	float ratio = (float)state->screen_width / (float)state->screen_height;
	glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
}

 

 
 

 


 

 

 

 

 

// randcolor returns a fraction of 255

VGfloat randcolor() {
	
	return (VGfloat)(rand() % 256) / 255.0;

}

 

 

// randf returns a floating point number bounded by n

VGfloat randf(n) {

	return (VGfloat)(rand() % n);

}

 

 


 

 

// exit_func cleans up

static void exit_func(void) {
	// clear screen
	glClear( GL_COLOR_BUFFER_BIT );	
	eglSwapBuffers(state->display, state->surface);
 

	// Release OpenGL resources
	eglMakeCurrent( state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroySurface( state->display, state->surface );
	eglDestroyContext( state->display, state->context );
	eglTerminate( state->display );

}

 

 

// main initializes the system and shows the picture,

// exit and clean up when you hit [RETURN].

int main (int argc, char **argv) {
	int w, h, n;
	
	bcm_host_init();

	memset( state, 0, sizeof( *state ) ); // clear application state

	init_ogl(state); // Start OGLES

	w = state->screen_width;

	h = state->screen_height;

	n = 0;

	if (argc > 1) {
		n = atoi(argv[1]);
	}

	//eglSwapBuffers(state->display, state->surface);
		

	Background(0, 0, 0);                                      
	TextMid(10.0, 10.0, "hello, world", SerifTypeface, 2.0);
	End();

	//abwarten
	while (getchar() != '\n') {;}

	exit_func();

	return 0;

}