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

#define ANGLE_TURN 6
#define Y_CENTER -0.13

#define ROAD_WIDTH 2
#define ROAD_THICK 0.15
#define PI 3.1415927
#endif

int mode	=	0;      //  Projection mode
float thf   =   105;    //  Azimuth of view angle for first person
//int th		=	110;    //  Azimuth of view angle
int th		=	105;    //  Azimuth of view angle
int ph		=	0;      //  Elevation of view angle
int fov		=	55;     //  Field of view (for perspective)
double asp	=	1.333;  //  Aspect ratio
double dim	=	25;   	//  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   25; // Light distance
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int zh        =  90;  // Light azimuth

float shiny   =   1;  // Shininess (value)

//First person camera location
double fpX = 0;
double fpY = 0.8;
double fpZ = 0;

//x, y, z for reference point in glLookAt() for FP mode
double refX = 0;
double refY = 0;
double refZ = 0;

//Person Control
float carRotateX = 0.5;
float carRotateZ = 0;
float carXIncrement = 0;
float carZIncrement = 0;
float carRotate = 0;

float temp;

//Race
int start = 0;

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
	
	_textureBasicMetal = LoadTexBMP("texture/basic-metal.bmp");
	_textureAsphalt = LoadTexBMP("texture/asphalt.bmp");
	
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
   glDisable(GL_POLYGON_OFFSET_FILL);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glColor3f(0.7, 0.7, 0.7);
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
		cube(x, y, z, width, 0.17, width, 0);
	glPopMatrix();
}

static void support(double x, double y, double z)
{
   glBindTexture(GL_TEXTURE_2D,_textureSupport );
   glPushMatrix();
      glTranslated(x,y,z);
      cube(0,-0.05,-0.5, 2,0.15,0.2, 0);
   glPopMatrix();
}


	
static void quads(double x,double y,double z,
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

	   glBegin(GL_QUADS);
		   texRepX = dx/texScale;
		   texRepY = dz/texScale;
		   glNormal3f( 0,+1, 0);
		   
		   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
		   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
		   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
		   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
	   glEnd();
	   
	//  Undo transofrmations
   glPopMatrix();
}

static void road(double x, double y, double z)
{
   glPushMatrix();
      glTranslated(x,y,z);
	  glScaled(2,1,2);
      
      	quads(-8,0,6.6,1,Y_CENTER+0.2,4.4,0);
		quads(-5,0,10,2,Y_CENTER+0.2,1,0);
		quads(2.1,0,4.7,8,Y_CENTER+0.2,1,45);
		quads(7.5,0,-3.3,1,Y_CENTER+0.2,3,0);
		quads(5.75,0,-4.7,1,Y_CENTER+0.2,3,-45);
		quads(0.75,0,-1.,1,Y_CENTER+0.2,4,-60);
		quads(-3.4,0,0.8,1.5,Y_CENTER+0.2,1,0);
		quads(-6.4,0,1.9,2.5,Y_CENTER+0.2,1,30);
      
   glPopMatrix();
}

static void pitstop(double x, double y, double z)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
           
   glBindTexture(GL_TEXTURE_2D,_textureYellowBrick);
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
       //Building
       cube(0,1.6,0, 2,0.4,1, 0); //Top   
       glBindTexture(GL_TEXTURE_2D,_textureBrownBrick);
       cube(-1.75,0.65,0, 0.25,0.55,1, 0); //Left
       cube(1.75,0.65,0, 0.25,0.55,1, 0); //Right
       cube(0,0.65,0, 0.2,0.55,1, 0); //Middle
   glPopMatrix();   
   
   float texRepX = 1.0;
   float texRepY = 1.0;   
   //Door left
   glBindTexture(GL_TEXTURE_2D,_textureYellowBrick);
   glPushMatrix();
      glTranslated(x,y,z);
       glBegin(GL_QUADS);
           //Left
           glTexCoord2f(0.0,0.0); glVertex3f(-1.5, 0.1, 0.9);
           glTexCoord2f(texRepX,0.0); glVertex3f(-0.2, 0.1, 0.9);
           glTexCoord2f(texRepX,texRepY); glVertex3f(-0.2, 1.2, 0.9);
           glTexCoord2f(0.0,texRepY); glVertex3f(-1.5, 1.2, 0.9);
        glEnd();
   glPopMatrix();  
   //Door Right     
   glPushMatrix();
      glTranslated(x+1.7,y,z);
       glBegin(GL_QUADS);         
           //Left
           glTexCoord2f(0.0,0.0); glVertex3f(-1.5, 0.1, 0.9);
           glTexCoord2f(texRepX,0.0); glVertex3f(-0.2, 0.1, 0.9);
           glTexCoord2f(texRepX,texRepY); glVertex3f(-0.2, 1.2, 0.9);
           glTexCoord2f(0.0,texRepY); glVertex3f(-1.5, 1.2, 0.9);
        glEnd();
   glPopMatrix();
   //Floor
   glBindTexture(GL_TEXTURE_2D,_textureSidewalk );
   glPushMatrix();
      glTranslated(x,y,z);
      cube(0,-0.05,0.9, 2,0.15,0.1, 0);
   glPopMatrix();
   
   //Sidewalk
   glPushMatrix();
       glTranslated(x,0, 0);
       cube(0,-0.05,z+1.5, 2,0.15,0.5, 0); // Along Street
       glTranslated(0,0, 1);
       cube(0,-0.05,z+1.5, 2,0.15,0.5, 0); // Along Street
   glPopMatrix();
   //
   support(x,y, z+3.7);
   //Road
   road(x, y,z+4.91);
   
//      road(x, y,z+8);
}


static void pillar(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glRotated(th,0,1,0);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   float radScale = 0.03;
   float baseRadScale = 0.05;
   float capRadScale = 0.01;
   float outerRadScale = 0.08;

   //Pole
   glEnable(GL_TEXTURE_2D);
	   glColor3f(0.4, 0.4, 0.4);
	   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);   
	   glBegin(GL_QUADS);
		   int i;
		   for(i=45; i <= 360; i += 45) {
		      
		      //Pole Lower
		      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
		      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i), 0.1, radScale*Sin(i));
		      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i-45), 0.1, radScale*Sin(i-45));
		      glTexCoord2f(Cos(i), 2); glVertex3f(radScale*Cos(i-45), 0.5, radScale*Sin(i-45));
		      glTexCoord2f(Cos(i), 2); glVertex3f(radScale*Cos(i), 0.5, radScale*Sin(i));
		      
		   }
	   glEnd();
   glDisable(GL_TEXTURE_2D);


   //Light
   float em[4] = {0.8, 0.8, 0.1, 1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,em);
   glMaterialfv(GL_FRONT,GL_EMISSION,em);

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);			
	

   //Undo transformations
   glPopMatrix();
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
      refX = ((dim * Sin(thf)) + fpX ) + carXIncrement;     
      refY = (dim * Sin(ph))+ fpY;
      refZ = (dim * -Cos(thf)) + fpZ + carZIncrement;
      printf("%f\n", refY);
   
      glRotated(-carRotate,0,1,0);      
         gluLookAt(-1+carXIncrement, 0.9 ,-2.7+carZIncrement , 8.210370+carXIncrement,refY,-3.058857+carZIncrement, 0,1,0);  
   }

   //  Draw scene =========================================================================================================
   //  Skybox
   skybox(70);
   
   //  Sand
   dessert(0,Y_CENTER-0.1,0,30);
   
   //PitStop
//   pitstop(1, 0, -10);   
//   pitstop(5, 0, -10);

	glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, _textureConcrete);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		quads(0,0,0,30,Y_CENTER+0.1,1,0);
	
    glDisable(GL_TEXTURE_2D);
			
	glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, _textureOrangeConcrete);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		quads(0,0,0,1,Y_CENTER+0.1,30,0);
	
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, _textureAsphalt);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		road(0,0,-4);
	
    glDisable(GL_TEXTURE_2D);

   float i, gap=2.1, nPillar=6.0;
   for(i=0; i<(gap*nPillar); i=i+gap) {
		pillar(-13.5,-1,3+i,15,8,15,0);
   }
//    glEnable(GL_TEXTURE_2D);
//	    glBindTexture(GL_TEXTURE_2D, _textureYellowBrick);
//	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	    
//   		cube(-13.5,3.3,3.8,0.5,0.3,1.25,0);
//   		cube(-13.5,3.3,6.5,0.5,0.3,1.,0);
//   		cube(-13.5,3.3,9,0.5,0.3,1.25,0);
//   		
//    glDisable(GL_TEXTURE_2D);
   
   nPillar=9.0;
   for(i=0; i<(gap*nPillar); i=i+gap) {
		pillar(-18.5,-1,0.5+i,15,8,15,0);
   }
   
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

void personControl(int direction){
     int degree ,rotate, turn;
     
     switch(direction){
        case 1:
             degree = 0;             rotate = 1;             turn = 0;                  
             break;                  
             
        case 2:
             degree = 0;             rotate = -1;             turn = 0;             
             break;
             
        case 3:
             degree = -ANGLE_TURN;    rotate = 1;              turn = -ANGLE_TURN;
             break;
             
        default :
             degree = ANGLE_TURN;    rotate = 1;              turn = ANGLE_TURN;
             break;   
     }  
        temp = carRotateX;
        carRotateX = carRotateX*Cos(degree) + carRotateZ * Sin(degree);
        carRotateZ = -temp*Sin(degree)+carRotateZ*Cos(degree);
        carXIncrement += (carRotateX * rotate);
        carZIncrement += (carRotateZ * rotate);
        
        carRotate += turn;  
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
   // Controll Red car
   else if(ch == 'w' || ch == 'W') {      
       if(!start)   
           start = 1;
             
       personControl(1);
   } else if(ch == 's' || ch == 'S') {      
       personControl(2);  
   } else if(ch == 'd' || ch == 'D') {
       personControl(3);          
   } else if(ch == 'a' || ch == 'A') {
       personControl(4);
   }   
   
   // Reset rotate after 360   
   th %= 360;     
      
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
