#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stdbool.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "recup_data.h"
#include <stdio.h>
#include <stdlib.h>
/*
void get_data(float* data){
FILE* fp = fopen( "dubinski.tab", "rb" );
if ( !fp ) return false;
 data = (float*)malloc( size ); 
	if ( !data ) {
		fclose( fp );
		return false;
	}
for(int i = 0 ; i <81920*7; i++){
	fscanf(fp,"%f", data++);
} 


	fclose(fp);
	fp = 0;
}



void ShowPoint() {

	//glLineWidth( 2.0f );

	glBegin(GL_POINTS);
	
	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	
	glEnd();

}
*/
