/*
 *  EAS Kiki Pratiwi 171511046
 *
 *  p             Toggles first person/perspective projection
 *  arrow keys    Change view angle
 *  PgDn/PgUp     Zoom in and out (in perspective view)
 *  ESC           Exit
 *
 */
#include "lib/JTKPOLBAN.h"
#include "lib/loadtexbmp.c"
#include "lib/fatal.c"
#include "lib/Errcheck.c"
#include "lib/print.c"
#include "lib/project.c"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

int mode	=	0;      //  Projection mode
int th		=	110;    //  Azimuth of view angle
int ph		=	0;      //  Elevation of view angle
int fov		=	55;     //  Field of view (for perspective)
double asp	=	1.333;  //  Aspect ratio
double dim	=	8;   	//  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   25; // Light distance
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int zh        =  90;  // Light azimuth

//First person camera location
double fpX = 0;
double fpY = 0.7;
double fpZ = -0.3;

//x, y, z for reference point in glLookAt() for FP mode
double refX = 10;
double refY = 0;
double refZ = 0;

//Texture Variables
int tMode = 0;
float texScale = 0.5;

//Light Vecotrs
float Ambient[]   = {0.8 ,0.8 ,0.8 ,1.0};
float Diffuse[]   = {1.0,1.0,1.0,1.0};
float Specular[]  = {0,0,0,1.0};

GLuint	_textureBasicMetal, _textureGlass, _textureWheel, _textureTire,
		_textureWoodFence, _textureGrass, _textureCinderBlock, _textureCarGrill, 
		_textureHeadLamp, _textureCarbonFiber, _textureSidewalk, 
		_textureGarageDoor, _textureWalkway, _textureHedge, _textureGreyBrick, 
		_textureWoodBeam, _textureFrontDoor2, _textureWindow1, _textureSkyboxFront, 
		_textureSkyboxBack, _textureSkyboxRight, _textureSkyboxLeft, _textureSkyboxTop, 
		_textureAsphalt, _textureBrownBrick, _textureWhiteBrick, _textureMetalRoof, 
		_textureWarehouseWindow, _textureSupport;

GLuint	_textureBrick, _textureFence, _textureStone, _textureConcrete, 
		_textureOrangeConcrete, _textureDirt, _texturePebble, _textureCeiling,
		_textureDiffuse, _textureDoor, _textureGrass, _textureRoof, _textureWindow;   
		
GLuint	_textureSand, _textureYellowBrick;

int refreshMills = 15;
unsigned int ID;
float _angle = 0.0;

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
	
	_textureSand = LoadTexBMP("texture/sand.bmp");   
	_textureYellowBrick = LoadTexBMP("texture/yellow-brick.bmp");   
}



static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}



static void skybox(float dim) {
   // front
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxFront);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
	   glNormal3f(0, 0, -1);
	   glTexCoord2f(1.0,0.0); glVertex3f(+dim,-dim, dim);
	   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, dim);
	   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, dim);
	   glTexCoord2f(1.0,1.0); glVertex3f(+dim,+dim, dim);
   glEnd();

   // back
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxBack);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
	   glNormal3f(0, 0, 1);
	   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, -dim);
	   glTexCoord2f(0.0,0.0); glVertex3f(+dim,-dim, -dim);
	   glTexCoord2f(0.0,1.0); glVertex3f(+dim,+dim, -dim);
	   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, -dim);
   glEnd();

   // right
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxRight);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
	   glNormal3f(-1, 0, 0);
	   glTexCoord2f(1.0,0.0); glVertex3f(dim,-dim, -dim);
	   glTexCoord2f(0.0,0.0); glVertex3f(dim,-dim, +dim);
	   glTexCoord2f(0.0,1.0); glVertex3f(dim,+dim, +dim);
	   glTexCoord2f(1.0,1.0); glVertex3f(dim,+dim, -dim);
   glEnd();

   // left
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxLeft);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
	   glNormal3f(1, 0, 0);
	   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, +dim);
	   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, -dim);
	   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, -dim);
	   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, +dim);
   glEnd();

   // top
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxTop);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
	   glNormal3f(0, -1, 0);
	   glTexCoord2f(1.0,0.0); glVertex3f(+dim,dim, +dim);
	   glTexCoord2f(0.0,0.0); glVertex3f(-dim,dim, +dim);
	   glTexCoord2f(0.0,1.0); glVertex3f(-dim,dim, -dim);
	   glTexCoord2f(1.0,1.0); glVertex3f(+dim,dim, -dim);
   glEnd();
}


void dessert(double x, double y, double z, double width){
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
	glColor3f(1, 1, 1);       
	glBindTexture(GL_TEXTURE_2D, _textureSand);    
	glPushMatrix();
		cube(x, y, z, width, 1, width, 0);
	glPopMatrix();
}

static void home() {
   
    /* =============================================Grass========================================= */
    glPushMatrix();
    	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureGrass);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,70.0,1);  glVertex3f(-50,-1.5,50); //scale 1/70
            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-50,-1.5,-50);
            glTexCoord3f(70.0,0.0,-1);  glVertex3f(50,-1.5,-50);
            glTexCoord3f(70.0,70.0,1);  glVertex3f(50,-1.5,50);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /* =============================================Wall========================================= */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureBrick);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Depan
	    glPushMatrix();
	        glBegin(GL_QUADS);  // Wall
	            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-2,0,1);
	            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(2,0,1);
	            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(2,-1.5,1);
	            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-2,-1.5,1);
	        glEnd();
	    glPopMatrix();
	
	    // Belakang
	    glPushMatrix();
	        glBegin(GL_QUADS);  // Wall
	            glTexCoord3f(0.0,2.0,-1);  glVertex3f(-2,0,-1);
	            glTexCoord3f(4.0,2.0,-1);  glVertex3f(2,0,-1);
	            glTexCoord3f(4.0,0.0,-1);  glVertex3f(2,-1.5,-1);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2,-1.5,-1);
	        glEnd();
	    glPopMatrix();
	    
	    // Kanan
	    glPushMatrix();
	        glBegin(GL_QUADS);  // Wall
	            glTexCoord3f(0.0,2.0,1); glVertex3f(2,0,1);
	            glTexCoord3f(2.0,2.0,-1); glVertex3f(2,0,-1);
	            glTexCoord3f(2.0,0.0,-1); glVertex3f(2,-1.5,-1);
	            glTexCoord3f(0.0,0.0,1); glVertex3f(2,-1.5,1);
	        glEnd();
	    glPopMatrix();
	
	    // Kiri
	    glPushMatrix();
	        glBegin(GL_QUADS);  // Wall
	            glTexCoord3f(0.0,2.0,1);    glVertex3f(-2,0,1);
	            glTexCoord3f(2.0,2.0,-1);    glVertex3f(-2,0,-1);
	            glTexCoord3f(2.0,0.0,-1);    glVertex3f(-2,-1.5,-1);
	            glTexCoord3f(0.0,0.0,1);    glVertex3f(-2,-1.5,1);
	        glEnd();
	    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    /* =============================================Roof========================================= */
    glPushMatrix();
		// load roof texture
		glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureRoof);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        
        
        
	        
			glBegin(GL_QUADS);	// Atap Depan
				glTexCoord3f(0.0, 2.0, 0);		glVertex3f(-2.2, 0.5, 0);
				glTexCoord3f(4.0, 2.0, 0);		glVertex3f(2.2, 0.5, 0);
				glTexCoord3f(4.0, 0.0, 1.25);		glVertex3f(2.2, -0.1, 1.25);
				glTexCoord3f(0.0, 0.0, 1.25);		glVertex3f(-2.2, -0.1, 1.25);
		 	glEnd();
		        
			glBegin(GL_QUADS);	// Atap Belakang
				glTexCoord3f(0.0, 2.0, 0);		glVertex3f(-2.2, 0.5, 0);
				glTexCoord3f(4.0, 2.0, 0);		glVertex3f(2.2, 0.5, 0);
				glTexCoord3f(4.0, 0.0, -1.25);		glVertex3f(2.2, -0.1, -1.25);
				glTexCoord3f(0.0, 0.0, -1.25);		glVertex3f(-2.2, -0.1, -1.25);
		 	glEnd(); 	
        glDisable(GL_TEXTURE_2D);
        
		// load roof-brick texture
		glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureBrick);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        
			glBegin(GL_TRIANGLES);	// Atap Kanan
				glTexCoord3f(0.0, 1.0, 0);		glVertex3f(2.0, 0.5, 0);
				glTexCoord3f(1.0, 0.0, 1);		glVertex3f(2, 0, 1);
				glTexCoord3f(-1.0, 0.0, -1);	glVertex3f(2, 0, -1);
		 	glEnd();
		        
			glBegin(GL_TRIANGLES);	// Atap Kiri
				glTexCoord3f(0.0, 1.0, 0);		glVertex3f(-2.0, 0.5, 0);
				glTexCoord3f(1.0, 0.0, 1);		glVertex3f(-2, 0, 1);
				glTexCoord3f(-1.0, 0.0, -1);	glVertex3f(-2, 0, -1);
		 	glEnd();	 
        glDisable(GL_TEXTURE_2D);
	glPopMatrix();	 
	
	/* =============================================Door========================================= */
	glPushMatrix();
		// load door texture
		glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureDoor);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        
        
        
        
        glBegin(GL_QUADS);
	        glTexCoord3f(0.0, 1.0, 1.0001);		glVertex3f(-0.3, -0.4, 1.0001);	
	        glTexCoord3f(1.0, 1.0, 1.0001);		glVertex3f(0.3, -0.4, 1.0001);	
	        glTexCoord3f(1.0, 0.0, 1.0001);		glVertex3f(0.3, -1.5, 1.0001);	
	        glTexCoord3f(0.0, 0.0, 1.0001);		glVertex3f(-0.3, -1.5, 1.0001);	
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
	 
	/* ============================================Window======================================== */
	glPushMatrix();
		// load window texture
		glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureWindow);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        
        
        
        
        glBegin(GL_QUADS); //depan kiri
	        glTexCoord3f(0.0, 1.0, 1.0001);		glVertex3f(-1.4, -0.4, 1.0001);	
	        glTexCoord3f(1.0, 1.0, 1.0001);		glVertex3f(-0.8, -0.4, 1.0001);	
	        glTexCoord3f(1.0, 0.0, 1.0001);		glVertex3f(-0.8, -1.1, 1.0001);	
	        glTexCoord3f(0.0, 0.0, 1.0001);		glVertex3f(-1.4, -1.1, 1.0001);	
        glEnd();
        
                
        glBegin(GL_QUADS); //depan kanan
	        glTexCoord3f(0.0, 1.0, 1.0001);		glVertex3f(1.4, -0.4, 1.0001);	
	        glTexCoord3f(1.0, 1.0, 1.0001);		glVertex3f(0.8, -0.4, 1.0001);	
	        glTexCoord3f(1.0, 0.0, 1.0001);		glVertex3f(0.8, -1.1, 1.0001);	
	        glTexCoord3f(0.0, 0.0, 1.0001);		glVertex3f(1.4, -1.1, 1.0001);	
        glEnd();
        
        glBegin(GL_QUADS); //belakang kiri
	        glTexCoord3f(0.0, 1.0, 1.0001);		glVertex3f(-1.4, -0.4, -1.0001);	
	        glTexCoord3f(1.0, 1.0, 1.0001);		glVertex3f(-0.8, -0.4, -1.0001);	
	        glTexCoord3f(1.0, 0.0, 1.0001);		glVertex3f(-0.8, -1.1, -1.0001);	
	        glTexCoord3f(0.0, 0.0, 1.0001);		glVertex3f(-1.4, -1.1, -1.0001);	
        glEnd();    
                
        glBegin(GL_QUADS); //belakang kanan
	        glTexCoord3f(0.0, 1.0, 1.0001);		glVertex3f(1.4, -0.4, -1.0001);	
	        glTexCoord3f(1.0, 1.0, 1.0001);		glVertex3f(0.8, -0.4, -1.0001);	
	        glTexCoord3f(1.0, 0.0, 1.0001);		glVertex3f(0.8, -1.1, -1.0001);	
	        glTexCoord3f(0.0, 0.0, 1.0001);		glVertex3f(1.4, -1.1, -1.0001);	
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();	
      
    /* =============================================Fence========================================= */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureFence);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	    glPushMatrix();
	    
			/* ============================================BACK======================================== */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,1.0,-1);  glVertex3f(-2.7,-1,-2.5);
	            glTexCoord3f(8.0,1.0,-1);  glVertex3f(2.7,-1,-2.5);
	            glTexCoord3f(8.0,0.0,-1);  glVertex3f(2.7,-1.5,-2.5);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1.5,-2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,1.0,-1);  glVertex3f(-2.7,-1,-2.3);
	            glTexCoord3f(8.0,1.0,-1);  glVertex3f(2.7,-1,-2.3);
	            glTexCoord3f(8.0,0.0,-1);  glVertex3f(2.7,-1.5,-2.3);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1.5,-2.3);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.5,-1);  glVertex3f(-2.7,-1,-2.3);
	            glTexCoord3f(8.0,0.5,-1);  glVertex3f(2.7,-1,-2.3);
	            glTexCoord3f(8.0,0.0,-1);  glVertex3f(2.7,-1,-2.5);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1,-2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,1.0,-1);   glVertex3f(2.7,-1,-2.3);
	            glTexCoord3f(0.5,1.0,-1);  glVertex3f(2.7,-1,-2.5);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(2.7,-1.5,-2.5);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(2.7,-1.5,-2.3);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,1.0,1);   glVertex3f(-2.7,-1,-2.3);
	            glTexCoord3f(0.5,1.0,-1);  glVertex3f(-2.7,-1,-2.5);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(-2.7,-1.5,-2.5);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1.5,-2.3);
	        glEnd();
	        
	        /* ============================================FRONT-RIGHT======================================== */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,1.0,-1);  glVertex3f(1.6,-1,2.6);
	            glTexCoord3f(5.0,1.0,-1);  glVertex3f(2.7,-1,2.6);
	            glTexCoord3f(5.0,0.0,-1);  glVertex3f(2.7,-1.5,2.6);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(1.6,-1.5,2.6);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,1.0,-1);  glVertex3f(1.6,-1,2.8);
	            glTexCoord3f(3.0,1.0,-1);  glVertex3f(2.7,-1,2.8);
	            glTexCoord3f(3.0,0.0,-1);  glVertex3f(2.7,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(1.6,-1.5,2.8);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.5,-1);  glVertex3f(1.6,-1,2.8);
	            glTexCoord3f(3.0,0.5,-1);  glVertex3f(2.7,-1,2.8);
	            glTexCoord3f(3.0,0.0,-1);  glVertex3f(2.7,-1,2.6);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(1.6,-1,2.6);
	        glEnd();
	        
	        /* ============================================FRONT-LEFT======================================== */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,1.0,-1);  glVertex3f(-1.6,-1,2.6);
	            glTexCoord3f(5.0,1.0,-1);  glVertex3f(-2.7,-1,2.6);
	            glTexCoord3f(5.0,0.0,-1);  glVertex3f(-2.7,-1.5,2.6);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-1.6,-1.5,2.6);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,1.0,-1);  glVertex3f(-1.6,-1,2.8);
	            glTexCoord3f(3.0,1.0,-1);  glVertex3f(-2.7,-1,2.8);
	            glTexCoord3f(3.0,0.0,-1);  glVertex3f(-2.7,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-1.6,-1.5,2.8);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.5,-1);  glVertex3f(-1.6,-1,2.8);
	            glTexCoord3f(3.0,0.5,-1);  glVertex3f(-2.7,-1,2.8);
	            glTexCoord3f(3.0,0.0,-1);  glVertex3f(-2.7,-1,2.6);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-1.6,-1,2.6);
	        glEnd();	        
	        
			/* ============================================RIGHT======================================== */
	        glBegin(GL_QUADS);  // right
	            glTexCoord3f(0.0,1,-1);    glVertex3f(2.9,-1,-2.5);
	            glTexCoord3f(8,1,-1);    glVertex3f(2.9,-1,2.8);
	            glTexCoord3f(8,0.0,-1);  glVertex3f(2.9,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(2.9,-1.5,-2.5);
	        glEnd();
				        
	        glBegin(GL_QUADS);  // left
	            glTexCoord3f(0.0,1,-1);    glVertex3f(2.7,-1,-2.5);
	            glTexCoord3f(8,1,-1);    glVertex3f(2.7,-1,2.8);
	            glTexCoord3f(8,0.0,-1);  glVertex3f(2.7,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(2.7,-1.5,-2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,8,-1);  glVertex3f(2.7,-1,-2.5);
	            glTexCoord3f(0.5,8,-1);  glVertex3f(2.9,-1,-2.5);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(2.9,-1,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(2.7,-1,2.8);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // back
	            glTexCoord3f(0.0,1.0,-1);   glVertex3f(2.7,-1,-2.5);
	            glTexCoord3f(0.5,1.0,-1);  glVertex3f(2.9,-1,-2.5);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(2.9,-1.5,-2.5);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(2.7,-1.5,-2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // front
	            glTexCoord3f(0.0,1.0,-1);   glVertex3f(2.7,-1,2.8);
	            glTexCoord3f(0.5,1.0,-1);  glVertex3f(2.9,-1,2.8);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(2.9,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(2.7,-1.5,2.8);
	        glEnd();	        


			/* ============================================LEFT======================================== */
	        glBegin(GL_QUADS);  // right
	            glTexCoord3f(0.0,1,-1);    glVertex3f(-2.9,-1,-2.5);
	            glTexCoord3f(8,1,-1);    glVertex3f(-2.9,-1,2.8);
	            glTexCoord3f(8,0.0,-1);  glVertex3f(-2.9,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.9,-1.5,-2.5);
	        glEnd();
				        
	        glBegin(GL_QUADS);  // left
	            glTexCoord3f(0.0,1,-1);    glVertex3f(-2.7,-1,-2.5);
	            glTexCoord3f(8,1,-1);    glVertex3f(-2.7,-1,2.8);
	            glTexCoord3f(8,0.0,-1);  glVertex3f(-2.7,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1.5,-2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,8,-1);  glVertex3f(-2.7,-1,-2.5);
	            glTexCoord3f(0.5,8,-1);  glVertex3f(-2.9,-1,-2.5);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(-2.9,-1,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1,2.8);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // back
	            glTexCoord3f(0.0,1.0,-1);   glVertex3f(-2.7,-1,-2.5);
	            glTexCoord3f(0.5,1.0,-1);  glVertex3f(-2.9,-1,-2.5);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(-2.9,-1.5,-2.5);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1.5,-2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // front
	            glTexCoord3f(0.0,1.0,-1);   glVertex3f(-2.7,-1,2.8);
	            glTexCoord3f(0.5,1.0,-1);  glVertex3f(-2.9,-1,2.8);
	            glTexCoord3f(0.5,0.0,-1);  glVertex3f(-2.9,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2.7,-1.5,2.8);
	        glEnd();
	    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    /* =============================================Gate========================================= */
	
	/* ============================================STONE======================================== */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureStone);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glPushMatrix();

	        
	    	/* =============================================RIGHT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,0.8,0);  glVertex3f(0.8,-0.8,2.5);
	            glTexCoord3f(0.8,0.8,0);  glVertex3f(1.2,-0.8,2.5);
	            glTexCoord3f(0.8,0.0,0);  glVertex3f(1.2,-1.5,2.5);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0.8,-1.5,2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.8,-0.8,2.9);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(1.2,-0.8,2.9);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(1.2,-1.5,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0.8,-1.5,2.9);
	        glEnd();

	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(1.2,-0.8,2.5);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(1.2,-0.8,2.9);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(1.2,-1.5,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.2,-1.5,2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.8,-0.8,2.5);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(0.8,-0.8,2.9);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(0.8,-1.5,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0.8,-1.5,2.5);
	        glEnd();

	        
	        /* =============================================LEFT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,0.8,0);  glVertex3f(-0.8,-0.8,2.5);
	            glTexCoord3f(0.8,0.8,0);  glVertex3f(-1.2,-0.8,2.5);
	            glTexCoord3f(0.8,0.0,0);  glVertex3f(-1.2,-1.5,2.5);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-0.8,-1.5,2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.8,-0.8,2.9);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(-1.2,-0.8,2.9);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(-1.2,-1.5,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0.8,-1.5,2.9);
	        glEnd();

	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-1.2,-0.8,2.5);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(-1.2,-0.8,2.9);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(-1.2,-1.5,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.2,-1.5,2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.8,-0.8,2.5);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(-0.8,-0.8,2.9);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(-0.8,-1.5,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0.8,-1.5,2.5);
	        glEnd();
	    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    /* ===========================================CANTILEVER-CONCRETE======================================== */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureConcrete);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glPushMatrix();

	        
	    	/* =============================================RIGHT========================================= */
	        glBegin(GL_QUADS);  // back                            
	            glTexCoord3f(0.0,0.8,0);  glVertex3f(0.7,-0.7,2.4);
	            glTexCoord3f(0.8,0.8,0);  glVertex3f(1.3,-0.7,2.4);
	            glTexCoord3f(0.8,0.0,0);  glVertex3f(1.3,-0.8,2.4);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0.7,-0.8,2.4);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front 
	            glTexCoord3f(0.0,0.8,0);  glVertex3f(0.7,-0.7,3);
	            glTexCoord3f(0.8,0.8,0);  glVertex3f(1.3,-0.7,3);
	            glTexCoord3f(0.8,0.0,0);  glVertex3f(1.3,-0.8,3);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0.7,-0.8,3);
	        glEnd();

	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(1.3,-0.7,2.4);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(1.3,-0.7,3);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(1.3,-0.8,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.3,-0.8,2.4);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.7,-0.7,2.4);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(0.7,-0.7,3);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(0.7,-0.8,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0.7,-0.8,2.4);
	        glEnd();	     
			   
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.7,-0.8,2.4);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.3,-0.8,2.4);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.3,-0.8,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0.7,-0.8,3);
	        glEnd();
	        
	        /* =============================================LEFT========================================= */
	        glBegin(GL_QUADS);  // back                            
	            glTexCoord3f(0.0,0.8,0);  glVertex3f(-0.7,-0.7,2.4);
	            glTexCoord3f(0.8,0.8,0);  glVertex3f(-1.3,-0.7,2.4);
	            glTexCoord3f(0.8,0.0,0);  glVertex3f(-1.3,-0.8,2.4);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-0.7,-0.8,2.4);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front 
	            glTexCoord3f(0.0,0.8,0);  glVertex3f(-0.7,-0.7,3);
	            glTexCoord3f(0.8,0.8,0);  glVertex3f(-1.3,-0.7,3);
	            glTexCoord3f(0.8,0.0,0);  glVertex3f(-1.3,-0.8,3);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-0.7,-0.8,3);
	        glEnd();

	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-1.3,-0.7,2.4);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(-1.3,-0.7,3);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(-1.3,-0.8,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.3,-0.8,2.4);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.7,-0.7,2.4);
	            glTexCoord3f(0.8,0.8,1);  glVertex3f(-0.7,-0.7,3);
	            glTexCoord3f(0.8,0.0,1);  glVertex3f(-0.7,-0.8,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0.7,-0.8,2.4);
	        glEnd();	     
			   
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.7,-0.8,2.4);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.3,-0.8,2.4);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.3,-0.8,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0.7,-0.8,3);
	        glEnd();
	    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
	/* =================================================DIFFUSE-PILLAR=================================== */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureDiffuse);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glPushMatrix();

	        
	    	/* =============================================RIGHT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(0.8,0.8,2.5);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(1.2,0.8,2.5);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(1.2,-0.7,2.5);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0.8,-0.7,2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(0.8,0.8,2.9);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(1.2,0.8,2.9);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(1.2,-0.7,2.9);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0.8,-0.7,2.9);
	        glEnd();

	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(1.2,0.8,2.5);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(1.2,0.8,2.9);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(1.2,-0.7,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.2,-0.7,2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(0.8,1,2.5);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(0.8,1,2.9);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(0.8,-0.7,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0.8,-0.7,2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.8,1,2.5);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.2,0.8,2.5);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.2,0.8,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0.8,1,2.9);
	        glEnd();
	        
	        glBegin(GL_TRIANGLES);  // top-back
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.8,0.8,2.5);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.2,0.8,2.5);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(0.8,1,2.5);
	        glEnd();
				        
	        glBegin(GL_TRIANGLES);  // top-front
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0.8,0.8,2.9);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.2,0.8,2.9);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(0.8,1,2.9);
	        glEnd();
	        
	        /* =============================================LEFT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(-0.8,0.8,2.5);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(-1.2,0.8,2.5);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(-1.2,-0.7,2.5);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-0.8,-0.7,2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(-0.8,0.8,2.9);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(-1.2,0.8,2.9);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(-1.2,-0.7,2.9);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-0.8,-0.7,2.9);
	        glEnd();

	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(-1.2,0.8,2.5);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(-1.2,0.8,2.9);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(-1.2,-0.7,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.2,-0.7,2.5);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(-0.8,1,2.5);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(-0.8,1,2.9);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(-0.8,-0.7,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0.8,-0.7,2.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.8,1,2.5);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.2,0.8,2.5);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.2,0.8,2.9);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0.8,1,2.9);
	        glEnd();
	        
			glBegin(GL_TRIANGLES);  // top-back
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.8,0.8,2.5);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.2,0.8,2.5);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-0.8,1,2.5);
	        glEnd();
				        
	        glBegin(GL_TRIANGLES);  // top-front
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0.8,0.8,2.9);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.2,0.8,2.9);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-0.8,1,2.9);
	        glEnd();
	        
	    glPopMatrix();
    glDisable(GL_TEXTURE_2D);    
	 	

	/* =================================================CONCRETE-PILLAR=================================== */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureConcrete);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glPushMatrix();

	        
	    	/* =============================================RIGHT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(1.4,0.3,2.6);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(1.6,0.3,2.6);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(1.6,-1.5,2.6);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(1.4,-1.5,2.6);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(1.4,0.3,2.8);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(1.6,0.3,2.8);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(1.6,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(1.4,-1.5,2.8);
	        glEnd();

	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(1.6,0.3,2.6);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(1.6,0.3,2.8);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(1.6,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.6,-1.5,2.6);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(1.4,0.4,2.6);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(1.4,0.4,2.8);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(1.4,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.4,-1.5,2.6);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(1.4,0.4,2.6);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.6,0.3,2.6);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.6,0.3,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.4,0.4,2.8);
	        glEnd();
	        
	        glBegin(GL_TRIANGLES);  // top-back
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(1.4,0.3,2.6);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.6,0.3,2.6);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.4,0.4,2.6);
	        glEnd();
				        
	        glBegin(GL_TRIANGLES);  // top-front
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(1.4,0.3,2.8);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.6,0.3,2.8);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.4,0.4,2.8);
	        glEnd();
	        
	        /* =============================================LEFT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(-1.4,0.3,2.6);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(-1.6,0.3,2.6);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(-1.6,-1.5,2.6);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-1.4,-1.5,2.6);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,3.0,0);  glVertex3f(-1.4,0.3,2.8);
	            glTexCoord3f(0.5,3.0,0);  glVertex3f(-1.6,0.3,2.8);
	            glTexCoord3f(0.5,0.0,0);  glVertex3f(-1.6,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-1.4,-1.5,2.8);
	        glEnd();

	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(-1.6,0.3,2.6);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(-1.6,0.3,2.8);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(-1.6,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.6,-1.5,2.6);
	        glEnd();
	        
	    	glBegin(GL_QUADS); // left
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(-1.4,0.4,2.6);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(-1.4,0.4,2.8);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(-1.4,-1.5,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.4,-1.5,2.6);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-1.4,0.4,2.6);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.6,0.3,2.6);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.6,0.3,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.4,0.4,2.8);
	        glEnd();
	        
			glBegin(GL_TRIANGLES);  // top-back
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-1.4,0.3,2.6);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.6,0.3,2.6);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.4,0.4,2.6);
	        glEnd();
				        
	        glBegin(GL_TRIANGLES);  // top-front
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-1.4,0.3,2.8);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.6,0.3,2.8);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.4,0.4,2.8);
	        glEnd();
	        
	    glPopMatrix();
    glDisable(GL_TEXTURE_2D);  	 
	

	/* =================================================TOP=================================== */
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureRoof);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glPushMatrix();

	        
	    	/* =============================================RIGHT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,0.5,0);  glVertex3f(0  ,1.1,2.4);
	            glTexCoord3f(8,0.5,0);  glVertex3f(1.8,0.3,2.4);
	            glTexCoord3f(8,0.0,0);  glVertex3f(1.8,0.2,2.4);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0  ,1,2.4);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,0.5,0);  glVertex3f(0  ,1.1,3);
	            glTexCoord3f(8,0.5,0);  glVertex3f(1.8,0.3,3);
	            glTexCoord3f(8,0.0,0);  glVertex3f(1.8,0.2,3);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0  ,1,3);
	        glEnd();

	    	glBegin(GL_QUADS); // right
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(1.8,0.3,2.4);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(1.8,0.3,3);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(1.8,0.2,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(1.8,0.2,2.4);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // bottom
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0  ,1,2.4);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.8,0.2,2.4);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.8,0.2,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0  ,1,3);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(0  ,1.1,2.4);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(1.8,0.3,2.4);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(1.8,0.3,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(0  ,1.1,3);
	        glEnd();	        
	        
			/* =============================================LEFT========================================= */
	        glBegin(GL_QUADS);  // back
	            glTexCoord3f(0.0,0.5,0);  glVertex3f(0  ,1.1,2.4);
	            glTexCoord3f(8,0.5,0);  glVertex3f(-1.8,0.3,2.4);
	            glTexCoord3f(8,0.0,0);  glVertex3f(-1.8,0.2,2.4);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0  ,1,2.4);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // front
	            glTexCoord3f(0.0,0.5,0);  glVertex3f(0  ,1.1,3);
	            glTexCoord3f(8,0.5,0);  glVertex3f(-1.8,0.3,3);
	            glTexCoord3f(8,0.0,0);  glVertex3f(-1.8,0.2,3);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(0  ,1,3);
	        glEnd();

	    	glBegin(GL_QUADS); // front
	            glTexCoord3f(0.0,3.0,1);  glVertex3f(-1.8,0.3,2.4);
	            glTexCoord3f(0.5,3.0,1);  glVertex3f(-1.8,0.3,3);
	            glTexCoord3f(0.5,0.0,1);  glVertex3f(-1.8,0.2,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-1.8,0.2,2.4);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // bottom
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0  ,1,2.4);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.8,0.2,2.4);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.8,0.2,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0  ,1,3);
	        glEnd();
	        
	        glBegin(GL_QUADS);  // top
	            glTexCoord3f(0.0,0.8,1);  glVertex3f(-0  ,1.1,2.4);
	            glTexCoord3f(8.0,0.8,1);  glVertex3f(-1.8,0.3,2.4);
	            glTexCoord3f(8.0,0.0,1);  glVertex3f(-1.8,0.3,3);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-0  ,1.1,3);
	        glEnd();
	        
	    glPopMatrix();
    glDisable(GL_TEXTURE_2D);  	 
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Enable Textures
   glEnable(GL_TEXTURE_2D);

   //  Undo previous transformations
   glLoadIdentity();
   
   //  Light Position (Sun)
   float Position[]  = {0 ,25 ,0,1.0};
   
   //  Enable lighting
   glEnable(GL_LIGHTING);
   
   //  Location of viewer for specular calculations
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);

   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

    //  Enable light 0 - Sun
   glEnable(GL_LIGHT0);
   
   //  Set ambient, diffuse, specular components and position of light 0 (Sun)
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);

   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  First Person
   else
   {
      refX = (dim * Sin(th)) + fpX;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + fpZ;
      gluLookAt(0,1,1, refX,refY,refZ, 0,1,0);
   }

   //  Draw scene =========================================================================================================
   //  Skybox
   skybox(64);
   
   //  Sand
   dessert(1.5,-0.13,4,20);
   
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{  
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;

   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Reproject
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(800,600);
   glutCreateWindow("EAS 171511046");
   
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   
   // load the texture
   initTexture();
   
   glutIdleFunc(idle);
   
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
