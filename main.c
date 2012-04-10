/*
 *  UniMotion - Unified Motion detection for Apple portables.
 *
 *  Copyright (c) 2007 Lincoln Ramsay. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "unimotion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

static int loop = 0;
int x, y, z;
int type;

// Drawing routine.
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		
		// Pass coordinates to glTranslatef
		// see also moveSphere.cpp in Desktop/OPENGL/experiments
		glTranslatef(x, y, z);
		
		glScalef(1.0, 1.0, 1.0);
		glutWireSphere(2.0, 10, 8);

	glPopMatrix();
	
    glFlush();
}

// Initialization routine.
void setup(void) {
   glClearColor(1.0, 1.0, 1.0, 0.0);  
}

// OpenGL window reshape routine.
void resize(int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-100.0, 100.0, -100.0, 100.0, 10.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
   switch(key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Processing the animation. 
void animate(void) {
	int ok = 0;
   	ok = read_sms_raw(type, &x, &y, &z);
    if ( ok );
        // printf("%d %d %d\n", x, y, z);
	
	// set it back to zero.
	z = -15;
	fflush(stdout);
        if ( !ok )
            fprintf(stderr, "no co-ordinates!\n");
        if ( loop ) usleep(loop*1000);
    
    glutPostRedisplay();
}


int main( int argc, char **argv ) {

	int i = 1;
    
    if ( argc >= i+1 )
        loop = atoi(argv[i]);

    type = detect_sms();
    if ( type == unknown ) {
        fprintf(stderr, "Could not detect an SMS\n");
        return 1;
    }
    
    // Graphics 
	printf("OPENGL\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   	glutInitWindowSize(1000, 750);
   	glutInitWindowPosition(0, 0); 
   	glutCreateWindow("Mac Motion Sensor"); 
   	glutDisplayFunc(drawScene); 
   	glutReshapeFunc(resize);  
   	glutKeyboardFunc(keyInput);
   
   	// This is the special thing for this lesson
	// We tell Glut to call our function every time he can
   	glutIdleFunc(animate);
   	glutMainLoop(); 
   
    return 0;
}
