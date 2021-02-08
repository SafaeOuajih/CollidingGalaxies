#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include "GL/glew.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
//#include "recup_data.h"
#include "text.h"

static float g_inertia = 0.5f;

static float oldCamPos[] = { 0.0f, 0.0f, -45.0f };
static float oldCamRot[] = { 0.0f, 0.0f, 0.0f };
static float newCamPos[] = { 0.0f, 0.0f, -45.0f };
static float newCamRot[] = { 0.0f, 0.0f, 0.0f };

static bool g_showGrid = true;
static bool g_showAxes = true;

void DrawGridXZ( float ox, float oy, float oz, int w, int h, float sz ) {
	
	int i;

	glLineWidth( 1.0f );

	glBegin( GL_LINES );

	glColor3f( 0.48f, 0.48f, 0.48f );

	for ( i = 0; i <= h; ++i ) {
		glVertex3f( ox, oy, oz + i * sz );
		glVertex3f( ox + w * sz, oy, oz + i * sz );
	}

	for ( i = 0; i <= h; ++i ) {
		glVertex3f( ox + i * sz, oy, oz );
		glVertex3f( ox + i * sz, oy, oz + h * sz );
	}

	glEnd();

}

void ShowAxes() {

	glLineWidth( 2.0f );

	glBegin( GL_LINES );
	
	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 2.0f, 0.0f, 0.0f );

	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 2.0f, 0.0f );

	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 2.0f );
	
	glEnd();

}




int main( int argc, char ** argv ) {

	
	omp_set_num_threads( 4 );


/////////////////////////////////////////////////

	FILE* fp = fopen( "dubinski.tab", "r" );
	if ( !fp ) return false;
        int size = 81920*7; 
	float data[size];
 
	int i = 0;
	int size2 = 1024;
	float accelx [size2];
	float accely [size2];
	float accelz [size2];
	float masse[size2] ; 
	float posx[size2] ;
	float posy[size2] ;
	float posz[size2] ;
	float velx[size2] ;
	float vely[size2] ;
	float velz[size2] ;
	int j =0;
	int f = 0;
	int r = 7*80;
	int k = 0;
	int h = 0;
	int step = 0;
        float M =10;

/////////////////////////////////////////

	#pragma omp parallel for
	for(i=0; i < size;  i = i+7){
		fscanf(fp,"%f %f %f %f %f %f %f", &data[i], &data[i+1], &data[i+2], &data[i+3], &data[i+4], &data[i+5], &data[i+6]);

	} 
	fclose(fp);
	fp = 0;


//////////////////////////////////////////

	for(i=0; i < size;i = i+r){
		
			masse[f]=data[i];
			posx[f]=data[i+1] ;
			posy[f]=data[i+2] ;
			posz[f]=data[i+3] ;
			velx[f]=data[i+4] ;
			vely[f]=data[i+5] ;
			velz[f]=data[i+6] ;
			f=f+1;				
		
		
	}
/////////////////////////////////////////


	SDL_Event event;
	SDL_Window * window;
	SDL_DisplayMode current;
  	
	int width = 640;
	int height = 480;

	bool done = false;

	float mouseOriginX = 0.0f;
	float mouseOriginY = 0.0f;

	float mouseMoveX = 0.0f;
	float mouseMoveY = 0.0f;

	float mouseDeltaX = 0.0f;
	float mouseDeltaY = 0.0f;

	struct timeval begin, end;
	float fps = 0.0;
	char sfps[40] = "FPS: ";

	if ( SDL_Init ( SDL_INIT_EVERYTHING ) < 0 ) {
		printf( "error: unable to init sdl\n" );
		return -1;
	}

	if ( SDL_GetDesktopDisplayMode( 0, &current ) ) {
		printf( "error: unable to get current display mode\n" );
		return -1;
	}

	window = SDL_CreateWindow( "SDL", 	SDL_WINDOWPOS_CENTERED, 
										SDL_WINDOWPOS_CENTERED, 
										width, height, 
										SDL_WINDOW_OPENGL );
  
	SDL_GLContext glWindow = SDL_GL_CreateContext( window );

	GLenum status = glewInit();

	if ( status != GLEW_OK ) {
		printf( "error: unable to init glew\n" );
		return -1;
	}

	if ( ! InitTextRes( "./bin/DroidSans.ttf" ) ) {
		printf( "error: unable to init text resources\n" );
		return -1;
	}

	SDL_GL_SetSwapInterval( 1 );

	while ( !done ) {
  		
		int i;

		while ( SDL_PollEvent( &event ) ) {
      
			unsigned int e = event.type;
			
			if ( e == SDL_MOUSEMOTION ) {
				mouseMoveX = event.motion.x;
				mouseMoveY = height - event.motion.y - 1;
			} else if ( e == SDL_KEYDOWN ) {
				if ( event.key.keysym.sym == SDLK_F1 ) {
					g_showGrid = !g_showGrid;
				} else if ( event.key.keysym.sym == SDLK_F2 ) {
					g_showAxes = !g_showAxes;
				} else if ( event.key.keysym.sym == SDLK_ESCAPE ) {
 					done = true;
				}
			}

			if ( e == SDL_QUIT ) {
				printf( "quit\n" );
				done = true;
			}

		}

		mouseDeltaX = mouseMoveX - mouseOriginX;
		mouseDeltaY = mouseMoveY - mouseOriginY;

		if ( SDL_GetMouseState( 0, 0 ) & SDL_BUTTON_LMASK ) {
			oldCamRot[ 0 ] += -mouseDeltaY / 5.0f;
			oldCamRot[ 1 ] += mouseDeltaX / 5.0f;
		}else if ( SDL_GetMouseState( 0, 0 ) & SDL_BUTTON_RMASK ) {
			oldCamPos[ 2 ] += ( mouseDeltaY / 100.0f ) * 0.5 * fabs( oldCamPos[ 2 ] );
			oldCamPos[ 2 ]  = oldCamPos[ 2 ] > -5.0f ? -5.0f : oldCamPos[ 2 ];
		}

		mouseOriginX = mouseMoveX;
		mouseOriginY = mouseMoveY;

		glViewport( 0, 0, width, height );
		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable( GL_BLEND );
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glDisable( GL_TEXTURE_2D );
		glEnable( GL_DEPTH_TEST );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 50.0f, (float)width / (float)height, 0.1f, 100000.0f );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		for ( i = 0; i < 3; ++i ) {
			newCamPos[ i ] += ( oldCamPos[ i ] - newCamPos[ i ] ) * g_inertia;
			newCamRot[ i ] += ( oldCamRot[ i ] - newCamRot[ i ] ) * g_inertia;
		}

		glTranslatef( newCamPos[0], newCamPos[1], newCamPos[2] );
		glRotatef( newCamRot[0], 1.0f, 0.0f, 0.0f );
		glRotatef( newCamRot[1], 0.0f, 1.0f, 0.0f );
		
		if ( g_showGrid ) {
			DrawGridXZ( -100.0f, 0.0f, -100.0f, 20, 20, 10.0 );
		}


		if ( g_showAxes ) {

		}

		gettimeofday( &begin, NULL );




//////////////////////////////////////////





		glBegin(GL_POINTS);
		for ( j = 0; j < size2; j++) {
		        accelx [j]  =0;
 			accely [j]  =0;	
                        accelz [j]  =0;

			glColor3f( 0.0f, 1.0f, 0.0f );
			glVertex3f( posx[j], posy[j], posz[j] );
	
		}
		glEnd();
               

		
///////////////////////////////////////////

		#pragma omp parallel for
		for(k=0; k<size2; k++){
		    #pragma omp parallel for 
                    for(h=0;h<size2 ; h++){
			if ( h != k ) {
                                float mulPosX = posx[h]-posx[k];
        			float mulPosY = posy[h]-posy[k];			
				float mulPosZ = posz[h]-posz[k];
		                float d = sqrtf(mulPosX*mulPosX + mulPosY*mulPosY + mulPosZ*mulPosZ);
				if ( d < 1.0 ) d = 1.0;
                                float l = masse[h]* M * (1/(d*d*d));
		                accelx [k] += mulPosX*l;
	 			accely [k] += mulPosY*l;	
		                accelz [k] += mulPosZ*l;
			}
			
               	    }
           
		}
  
	        #pragma omp parallel for 
		for(k=0; k<size2; k=k+2){

			velx[k]+=accelx [k];
			vely[k]+=accely [k];
			velz[k]+=accelz [k];
		        posx[k]+=velx[k]*0.1;
		        posy[k]+=vely[k]*0.1;
		        posz[k]+=velz[k]*0.1;

			velx[k+1]+=accelx [k+1];
			vely[k+1]+=accely [k+1];
			velz[k+1]+=accelz [k+1];
		        posx[k+1]+=velx[k+1]*0.1;
		        posy[k+1]+=vely[k+1]*0.1;
		        posz[k+1]+=velz[k+1]*0.1;

		}


	


/////////////////////////////////////////////
		gettimeofday( &end, NULL );

		fps = (float)1.0f / ( ( end.tv_sec - begin.tv_sec ) * 1000000.0f + end.tv_usec - begin.tv_usec) * 1000000.0f;
		sprintf( sfps, "FPS : %.4f", fps );

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		DrawText( 10, height - 20, sfps, TEXT_ALIGN_LEFT, RGBA(255, 255, 255, 255) );
		DrawText( 10, 30, "'F1' : show/hide grid", TEXT_ALIGN_LEFT, RGBA(255, 255, 255, 255) );
		DrawText( 10, 10, "'F2' : show/hide axes", TEXT_ALIGN_LEFT, RGBA(255, 255, 255, 255) );

		SDL_GL_SwapWindow( window );
		SDL_UpdateWindowSurface( window );
	}

	SDL_GL_DeleteContext( glWindow );
	DestroyTextRes();
	SDL_DestroyWindow( window );
	SDL_Quit();
 
	return 1;
}

