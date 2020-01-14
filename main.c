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

#define ANGLE_TURN 12
#define Y_CENTER -0.13

#define ROAD_WIDTH 2
#define ROAD_THICK 0.15
#define PI 3.1415927

#define DAY 1
#define NIGHT 0

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

int earth = DAY; //Day or Night
int at0=100;      //  Constant  attenuation %
int at1=20;        //  Linear    attenuation %
int at2=20;        //  Quadratic attenuation %

//First person camera location
double fpX = 0;
double fpY = 0.8;
double fpZ = 0;

//x, y, z for reference point in glLookAt() for FP mode
double refX = 0;
double refY = 0;
double refZ = 0;

//User car
float carRotateX = 0.5;
float carRotateZ = 0;
float carXIncrement = 0;
float carZIncrement = 0;
float carRotate = 0;

//Enemy car
float centerXIncrement = 0;
float centerZIncrement = 0;
float xRotate = 0.5;
float zRotate = 0;
float carRotate2 = 0;

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
		
GLuint	_textureSand, _textureYellowBrick, _textureStartLine;

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
	_textureStartLine = LoadTexBMP("texture/start-line.bmp");
	
	_textureGarageDoor = LoadTexBMP("texture/garage-door.bmp");
	_textureSidewalk = LoadTexBMP("texture/sidewalk.bmp");	
	_textureBasicMetal = LoadTexBMP("texture/basic-metal.bmp");
	_textureGlass = LoadTexBMP("texture/glass.bmp");
    _textureWheel = LoadTexBMP("texture/car-wheel.bmp");
    _textureTire = LoadTexBMP("texture/tire-tread.bmp");
	_textureCarGrill = LoadTexBMP("texture/car-grill.bmp");
	_textureHeadLamp = LoadTexBMP("texture/headlamp.bmp");
	_textureGlass = LoadTexBMP("texture/glass.bmp");
	_textureCarbonFiber = LoadTexBMP("texture/carbon-fiber.bmp");
	_textureGreyBrick = LoadTexBMP("texture/grey-brick.bmp");
	_textureWoodBeam = LoadTexBMP("texture/wood-beam.bmp");
	_textureMetalRoof = LoadTexBMP("texture/metal-roof.bmp");
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

static void quadsNoRepeateTexture(double x,double z) 
{
   glPushMatrix();
   		glScaled(0.5,1,0.5);
   		glTranslated(x,-0.8,z);
	   glBegin(GL_QUADS);
		   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
		   glTexCoord2f(1,0.0); glVertex3f(+1,+1,+1);
		   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
		   glTexCoord2f(0.0,1); glVertex3f(-1,+1,-1);
	   glEnd();
   glPopMatrix();                   	
}

static void startLine(){
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureStartLine);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   		
   		   glDisable(GL_POLYGON_OFFSET_FILL);

		   //  Set specular color to white
		   float white[] = {1,1,1,1};
		   float black[] = {0,0,0,1};
		   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
		   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
		   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
		   
		   glColor3f(0.7, 0.7, 0.7);
		   
   			quadsNoRepeateTexture(-20,29.22);
   			quadsNoRepeateTexture(-20,31.22);
   			quadsNoRepeateTexture(-20,33.22);
   			
   			glPushMatrix();
   				glTranslated(-27,0,7.12);
		   		glRotated(90,0,1,0);
   				quadsNoRepeateTexture(-20,34.22);
   			glPopMatrix(); 
		   
		      
    glDisable(GL_TEXTURE_2D);
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


static void pyramid (double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
   glDisable(GL_POLYGON_OFFSET_FILL);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
      // Enable color material mode:
   // The ambient and diffuse color of the front faces will track the color set by glColor().
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   
   glColor3f(0.7, 0.7, 0.7);


   //  Save transformation ===============================================================================================
   glPushMatrix();
      //  Offset, scale and rotate
      glTranslated(x,y+0.4,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glTexCoord3f(0,1,1); glVertex3f( 0.0f, 1.0f, 0.0f); //A
      glTexCoord3f(-1,-1,1); glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord3f(1,-1,1); glVertex3f(1.0f, -1.0f, 1.0f);
 
      // Right
      glTexCoord3f(0,1,1);   glVertex3f(0.0f, 1.0f, 0.0f);
      glTexCoord3f(-1,-1,1); glVertex3f(1.0f, -1.0f, 1.0f);
      glTexCoord3f(1,-1,1);  glVertex3f(1.0f, -1.0f, -1.0f);
 
      // Back
      glTexCoord3f(0,1,1);   glVertex3f(0.0f, 1.0f, 0.0f);
      glTexCoord3f(-1,-1,1); glVertex3f(1.0f, -1.0f, -1.0f);
      glTexCoord3f(1,-1,1);  glVertex3f(-1.0f, -1.0f, -1.0f);
 
      // Left
      glTexCoord3f(0,1,1);   glVertex3f( 0.0f, 1.0f, 0.0f);
      glTexCoord3f(-1,-1,1); glVertex3f(-1.0f,-1.0f,-1.0f);
      glTexCoord3f(1,-1,1);  glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid
   
   glPopMatrix();
}


void setLighting(){
     if(earth == DAY){
        Ambient[0] = 0.8;           Ambient[1] = 0.8;          Ambient[2] = 0.8;
        Diffuse[0] = 1;          Diffuse[1] = 1;          Diffuse[2] = 1;
        //Disable Lighting
        glDisable(GL_LIGHT1);     
//        glDisable(GL_LIGHT2);     
//        glDisable(GL_LIGHT3);     
//        glDisable(GL_LIGHT4);     
//        glDisable(GL_LIGHT5);     
//        glDisable(GL_LIGHT6);     
     }     
     else{
		//Setting For Night
		Ambient[0] = (20 / 255) * 0.8;          Ambient[1] = (60 / 255) * 0.8;          Ambient[2] = (180 / 255) * 0.8;
		Diffuse[0] = 0;          				Diffuse[1] = 0;          				Diffuse[2] = 0;
		
		glEnable(GL_LIGHT1);
		
			float amb[4] = {0,0,0,0};
			float dif[4] = {1,1,1,1}; //White
			float spec[4] = {1,1,1,1};
			
			//White Light
			glLightfv(GL_LIGHT1,GL_AMBIENT ,amb);              
			glLightfv(GL_LIGHT1,GL_DIFFUSE ,dif);
			glLightfv(GL_LIGHT1,GL_SPECULAR,spec);              
			
			
			float ligthStand1[4] = {-8,8,7.5, 1.0};
			float direction[3] = {0.0,-1.0,0.0};
			
			glLightfv(GL_LIGHT1,GL_POSITION,ligthStand1);
			glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,40);
			glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,direction);
			
			//glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,at0/100.0);              
			//glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,at1/100.0);
			//glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,at2/100.0);

	}
}


//Car Start
/*
 *  Draw a section of the car body
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     w (1 to color windows for car body, 0 otherwise)
 */

static void body(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int w)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   texScale = 0.1;

   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);

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

   //  End
   glEnd();

   glDisable(GL_POLYGON_OFFSET_FILL);
   //Color and texture to add windows
   if(w == 1) {
      glColor3f(0.8, 0.8, 1);
      glBindTexture(GL_TEXTURE_2D,_textureGlass);
      texRepX = 1.0;
      texRepY = 1.0;
      glBegin(GL_QUADS);
      //  Front
      glNormal3f(0, 0, 1);
      glTexCoord2f(0.0,0.0); glVertex3f(-0.8,-1, 1);
      glTexCoord2f(texRepX,0.0); glVertex3f(+0.8,-1, 1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(+0.8,+1, 1);
      glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,+1, 1);
      //  Back
      glNormal3f(0, 0,-1);
      glTexCoord2f(0.0,0.0); glVertex3f(+0.8,-1,-1);
      glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,-1,-1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,+1,-1);
      glTexCoord2f(0.0,texRepY); glVertex3f(+0.8,+1,-1);
      glEnd();
   }
   glEnable(GL_POLYGON_OFFSET_FILL);
   
   //  Undo transformations
   glPopMatrix();
}

static void wheel(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int d,
                 int s)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   //Turn off shininess for the rubber tire
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glBindTexture(GL_TEXTURE_2D,_textureWheel);

   glColor3f(0.8,0.8,0.8);
   //Tire
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, 0.05);
   for (th=360;th>=0;th-=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D, _textureTire);

   glColor3f(0.5,0.5,0.55);
   glBegin(GL_QUAD_STRIP);
   
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glNormal3f(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0);
      glTexCoord2f(0,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
      glTexCoord2f(1,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0.05);
   }
   // glNormal3f(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), -0.05);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0.05);
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void bumper(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int m)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   texScale = 0.1;

   //Offset the bumper so that the lights and grill are drawn directly on the surface
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Bumper

   //Base
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_POLYGON);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0,0,0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(0,0,-0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1,0,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1,0,0.35);
   glEnd();

   //Front Panels
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, 0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0, 0.4);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, -0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0, 0, -0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, -0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0.2, -0.35);
   
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0.894427, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0.25, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, 0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.25, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, -0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.25, -0.35);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.2, -0.4);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   if (m == 1) {
      glColor3f(0.2,0.2,0.2);
      glBindTexture(GL_TEXTURE_2D,_textureCarGrill);

      //Grill
      glBegin(GL_QUADS);
      glNormal3f(1, 0, 0);
      glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.15, 0.18);
      glTexCoord2f(0.5,0.0); glVertex3f(0.1, 0.03, 0.18);
      glTexCoord2f(0.5,1.0); glVertex3f(0.1, 0.03, -0.18);
      glTexCoord2f(0.0,1.0); glVertex3f(0.1, 0.15, -0.18);
      glEnd();
   }

   //Lights (taillights or headlights)
   float emColor[4];
   if(m == 1) {
      emColor[0] = 1.0 * emission;
      emColor[1] = 1.0 * emission;
      emColor[2] = 0.8 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(1, 1, 0.8);
   } else {
      emColor[0] = 0.8 * emission;
      emColor[1] = 0.0 * emission;
      emColor[2] = 0.0 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(.8, 0, 0);
   }

   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,emColor);
   glMaterialfv(GL_FRONT,GL_EMISSION,emColor);

   glBindTexture(GL_TEXTURE_2D,_textureHeadLamp);

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glEnable(GL_POLYGON_OFFSET_FILL);

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();  
}

static void car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 float cr, float cb, float cg)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glPolygonOffset(1,1);

   wheel(0.6,0,0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,0.4, 1,1,1, 0, 8, 10);

   glColor3f(cr, cb, cg);

   //Lower Body
   body(0,0.1,0, 0.8,0.1,0.4, 0, 0);
   //Cabin
   body(-0.1,0.3,0, 0.3,0.1,0.35, 0, 1);

   texScale = 1.0;

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(cr, cb, cg);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //Hood and upper side pannels
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.2, 0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.2, -0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.2, -0.4);
   glEnd();

   //Trunk
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.6, 0.25, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.6, 0.25, -0.35);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureGlass);

   glColor3f(0.8, 0.8, 1);

   //Windshield
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.2,0.4,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2,0.4,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.2,0.4,-0.35);
   glEnd();

   //Rear Window
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.4,0.4,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.4,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.4,0.4,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.6,0.25,-0.35);
   glEnd();

   //glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   //glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   //glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,_textureCarbonFiber);

   //Spoiler
   glColor3f(0.3,0.3,0.3);
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0);

   texRepX = 5.0;
   texRepY = 1.0;

   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.7,0.31,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,-0.35);
   
   glNormal3f(0.196116, 0.980581, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.7,0.31,-0.35);

   texRepX = 5.0;
   texRepY = 0.2;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,0.35);
   
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   glColor3f(cr, cb, cg);

   //Spoiler Fins
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   //Duplicate to draw both sides (with inverted normals) when face culling is on
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   glEnd();

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
         gluLookAt(-14+carXIncrement, 1.3 ,15+carZIncrement , 12.210370+carXIncrement,refY,15.058857+carZIncrement, 0,1,0);  
//         cube(-13+carXIncrement,2,15+carZIncrement,0.2,0.2,0.2, 0);
         cube(12.210370+carXIncrement,refY,15.058857+carZIncrement,0.2,0.2,0.2, 0);
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
    glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, _textureYellowBrick);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    
   		cube(-13.5,3.3,3.8,0.5,0.3,1.25,0);
   		cube(-13.5,3.3,6.5,0.5,0.3,1.,0);
   		cube(-13.5,3.3,9,0.5,0.3,1.25,0);
   		
    glDisable(GL_TEXTURE_2D);
   
//   nPillar=9.0;
//   for(i=0; i<(gap*nPillar); i=i+gap) {
//		pillar(-18.5,-1,0.5+i,15,8,15,0);
//   }
   
    //Pyramid
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureYellowBrick);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		pyramid(-8,Y_CENTER+2.5,7.5,3,3,3,0);
//		cube(-8,6,7.5,0.2,0.2,0.2, 0);
		
		pyramid(15,Y_CENTER+1.5,15,2,2,2,0);
		pyramid(11,Y_CENTER+1,18,1,1,1,0);
    glDisable(GL_TEXTURE_2D);  
   
    		
//============================================================================================day
   
   //Light
   float redEm[4] = {1,1,1,1};
   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,redEm);
   glMaterialfv(GL_FRONT,GL_EMISSION,redEm);
   glColor3f(0.5, 0, 0);
   
//============================================================================================end of day
    /* Opponent's Car*/
    glPushMatrix();
    	glTranslated(0,1.2,0);                               
		car(-11.5+centerXIncrement, -1 ,16.7+centerZIncrement, 1,1,1, carRotate2, 0,0,0);
    glPopMatrix();
    
    /* Controlled Car */
    glPushMatrix();
    	glTranslated(0,1.2,0);
		car(-13+carXIncrement, -1 ,15.2+carZIncrement, 1,1,1, carRotate, 1,0,0);
    glPopMatrix();
    
    /* Start Line */
	startLine();
    
    setLighting();
   
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
      
   // Switch Day   
   else if (ch == 'c' || ch == 'C')
      earth = 1-earth;
      
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
