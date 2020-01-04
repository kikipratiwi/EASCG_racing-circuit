/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

#include "lib/JTKPOLBAN.h"
#include "lib/loadtexbmp.c"
#include "lib/fatal.c"
#include "lib/Errcheck.c"
#include "lib/print.c"
#include "lib/project.c"

#include <math.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

GLuint    _textureBasicMetal, _textureGlass, _textureWheel, _textureTire,
        _textureWoodFence, _textureGrass, _textureCinderBlock, _textureCarGrill, 
        _textureHeadLamp, _textureCarbonFiber, _textureSidewalk, 
        _textureGarageDoor, _textureWalkway, _textureHedge, _textureGreyBrick, 
        _textureWoodBeam, _textureFrontDoor2, _textureWindow1, _textureSkyboxFront, 
        _textureSkyboxBack, _textureSkyboxRight, _textureSkyboxLeft, _textureSkyboxTop, 
        _textureAsphalt, _textureBrownBrick, _textureWhiteBrick, _textureMetalRoof, 
        _textureWarehouseWindow, _textureSupport;

GLuint    _textureBrick, _textureFence, _textureStone, _textureConcrete, 
        _textureOrangeConcrete, _textureDirt, _texturePebble, _textureCeiling,
        _textureDiffuse, _textureDoor, _textureRoof, _textureWindow,
		_textureSand, _textureCement, _textureSquareStone, _textureHexagonStone, _textureGreyStone;      

/*
 *  GLUT calls this routine at the first time to load the texture
 */
void initTexture() {
    _textureSkyboxFront = LoadTexBMP("texture/skybox-front.bmp");
    _textureSkyboxBack = LoadTexBMP("texture/skybox-back.bmp");
    _textureSkyboxRight = LoadTexBMP("texture/skybox-right.bmp");
    _textureSkyboxLeft = LoadTexBMP("texture/skybox-left.bmp");
    _textureSkyboxTop = LoadTexBMP("texture/skybox-top.bmp");
    _textureBrownBrick = LoadTexBMP("texture/brown-brick.bmp");


    _textureGrass = LoadTexBMP("texture/grass.bmp");
    _textureBrick = LoadTexBMP("texture/bricks.bmp");
    _textureRoof = LoadTexBMP("texture/roof.bmp");
    _textureDoor = LoadTexBMP("texture/front-door-2.bmp");
    _textureWindow = LoadTexBMP("texture/window-1.bmp");
    _textureFence = LoadTexBMP("texture/fence.bmp");
    _textureStone = LoadTexBMP("texture/stone.bmp");
    _textureConcrete = LoadTexBMP("texture/concrete.bmp");
    _textureOrangeConcrete = LoadTexBMP("texture/concrete-orange.bmp");
    _textureDirt = LoadTexBMP("texture/sidewalk.bmp");
    _texturePebble = LoadTexBMP("texture/pebble.bmp");
    _textureCeiling = LoadTexBMP("texture/ceiling.bmp");
    _textureDiffuse = LoadTexBMP("texture/diffuse.bmp");  
    
    _textureBrownBrick = LoadTexBMP("texture/brown-brick.bmp");
    _textureSupport = LoadTexBMP("texture/support.bmp");
    
    _textureAsphalt = LoadTexBMP("texture/road_road_0005_01_tiled_s.bmp");
    _textureSidewalk = LoadTexBMP("texture/sidewalk.bmp");
    
//	_textureSand =  LoadTexBMP("texture/sand.bmp");
//	_textureCement = LoadTexBMP("texture/cement.bmp");
//	_textureSquareStone = LoadTexBMP("texture/square-stone.bmp");
//	_textureHexagonStone = LoadTexBMP("texture/hexagon-stone.bmp");
//	_textureGreyStone = LoadTexBMP("texture/grey-stone.bmp");
}
 
/* Global variables */
char title[] = "3D Shapes";
 
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
   
   // load the texture
   initTexture();
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a pyramid consists of 4 triangles
   glLoadIdentity();                  // Reset the model-view matrix
   glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left and into the screen
 
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      
	
glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureDirt);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  // Front
      glTexCoord2f(0,1);      glVertex3f( 0.0f, 1.0f, 0.0f);
      glTexCoord2f(1,1); glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1,1); glVertex3f(1.0f, -1.0f, 1.0f);
      glDisable(GL_TEXTURE_2D);
 
      // Right
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
 
      // Back
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f, -1.0f, -1.0f);
 
      // Left
      glColor3f(1.0f,0.0f,0.0f);       // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f,0.0f,1.0f);       // Blue
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glColor3f(0.0f,1.0f,0.0f);       // Green
      glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid
 
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}
