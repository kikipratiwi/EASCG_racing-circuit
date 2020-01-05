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

#include <math.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#define X_CENTER 0
#define Y_CENTER -1.5
#define Z_CENTER 0
#define ROAD_WIDTH 5
#define ROAD_LONG 1
#define ROAD_THICK 0.15
#define NUM_ROAD_PARAMS 6
#define COUNT_ARRAY_ELEMENT(x)  (sizeof(x) / sizeof((x)[0]))
#define CUBE_TOP_LEFT 1
#define CUBE_TOP_RIGHT 2
#define CUBE_BOTTOM_LEFT 3
#define CUBE_BOTTOM_RIGHT 4
#define ROAD 1
#define SIDEWALK 2

#define ANGLE_TURN 6
#define DAY 1
#define NIGHT 0

//--original
//int mode=0;       //  Projection mode
//int th=110;         //  Azimuth of view angle
//int ph=0;         //  Elevation of view angle
//int fov=55;       //  Field of view (for perspective)

//int mode=0;       //  Projection mode
//int th=105;         //  Azimuth of view angle
//int ph=0;         //  Elevation of view angle
//int fov=55;       //  Field of view (for perspective)

//double fpX = 0;
//double fpY = 0.7;
//double fpZ = 0;
//heli view---------------------------------
//int mode=1;       //  Projection mode
//int th=0;         //  Azimuth of view angle
//int fov=117;       //  Field of view (for perspective)
//int ph=90;         //  Elevation of view angle

//int mode=1;       //  Projection mode
//int th=100;         //  Azimuth of view angle
//int fov=75;       //  Field of view (for perspective)
//int ph=45;         //  Elevation of view angle

int mode=1;       //  Projection mode
int th=105;         //  Azimuth of view angle
int fov=100;       //  Field of view (for perspective)
int ph=0;         //  Elevation of view angle
//------------------------------------------
//int mode=0;       //  Projection mode
//int th=-10;         //  Azimuth of view angle
//int fov=117;       //  Field of view (for perspective)
//int ph=0;         //  Elevation of view angle

int move=1;       //  Move light
int axes=0;       //  Display axes
int light=1;      //  Lighting
double asp=1.333;  //  Aspect ratio
double dim=12.0;   //  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   25;  // Light distance
// int inc       =  10;  // Ball increment
// int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 13;  // Elevation of light
int earth = DAY; //Day or Night

int at0=100;      //  Constant  attenuation %
int at1=20;        //  Linear    attenuation %
int at2=20;        //  Quadratic attenuation %

double fpMoveInc = 0.02; //Multiplier for how much to move each keystroke in FP mode

//First person camera location
double fpX = 0;
double fpY = 0;
double fpZ = -0.3;

//x, y, z for refrence point in glLookAt() for FP mode
double refX = 5;
double refY = 0;
double refZ = 0;

//Texture Variables
int tMode = 0;
float texScale = 0.5;

//Light Vecotrs
float Ambient[]   = {0.8 ,0.8 ,0.8 ,1.0};
float Diffuse[]   = {1.0,1.0,1.0,1.0};
float Specular[]  = {0,0,0,1.0};

//====================

float xPos = -6;
int carRotate = 0;

double xRotateTurn = 0;
double ZRotateTurn = 0;
double turnXInc = 0;
double turnZInc = 0;

//Camera
float centerXIncrement = 0;
float centerZIncrement = 0;
float xRotate = 0.5;
float zRotate = 0;

//Camera First
float camRotateX = 0.5;
float camRotateZ = 0;
float cameraXIncrement = 0;
float cameraZIncrement = 0;

float carRotate2 = 0;
float temp;

//====================

double circuit[] = { 
//     tx,  ty, tz,  roadLong, degree, rotate pivot
    20,        Y_CENTER, (9.9),   21,   0, 0,                //1   
    (22.6),  Y_CENTER, (-32.5), 50,  45, CUBE_BOTTOM_RIGHT, //2
    (-5.5),   Y_CENTER, (-34),  17.4,  90, CUBE_BOTTOM_RIGHT, //3
     (-45.5),  Y_CENTER, (-20.86),      17.64,  45, CUBE_TOP_LEFT,     //4
     (-30.5),    Y_CENTER, (-11.4),      19.5,  30, CUBE_TOP_LEFT,     //5
     -11.39,     Y_CENTER, (3.0),    15.45,   0, 0,                 //6
     -19.55,    Y_CENTER, (17.65),    14.45,  30, CUBE_TOP_LEFT,     //7
     -38,    Y_CENTER, (55),      29,  90, CUBE_TOP_LEFT      //8
};

double sidewalk[] = {
//     tx,  ty, tz,  roadLong, degree, rotate pivot
    20,        0, (30-20),   21,   0, 0,                //1   
    (20+2.6),  0, (30-62.5), 50,  45, CUBE_BOTTOM_RIGHT, //2
    (-5.5),   0, (-34),  17.4,  90, CUBE_BOTTOM_RIGHT, //3
     (-45),  0, (-20.5),      17.64,  45, CUBE_TOP_LEFT,     //4
     (-30.65),    0, (-11.35),      20.45,  30, CUBE_TOP_LEFT,     //5
     -10.4,     0, (4.0),    15.45,   0, 0,                 //6
     -18.9,    0, (18.1),    14.45,  30, CUBE_TOP_LEFT,     //7
     -37,    0, (54.6),      28,  90, CUBE_TOP_LEFT      //8
};

size_t circuitSize = COUNT_ARRAY_ELEMENT(circuit);


// int refreshMills = 15;
// unsigned int ID;

// //Camera/Eye
// float centerXIncrement = 0;
// float centerZIncrement = 0;
// float xRotate = 0.5;
// float zRotate = 0;

// float carRotate2 = 0;
// float temp;


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
        _textureDiffuse, _textureDoor, _textureGrass, _textureRoof, _textureWindow,
		_textureSand, _textureCement, _textureSquareStone, _textureHexagonStone, _textureGreyStone,
		_textureYellowBrick;      

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
    
    _textureAsphalt = LoadTexBMP("texture/asphalt.bmp");
    _textureSidewalk = LoadTexBMP("texture/sidewalk.bmp");
    
	_textureSand =  LoadTexBMP("texture/sand-1 (3).bmp");
	_textureCement = LoadTexBMP("texture/cement.bmp");
	_textureSquareStone = LoadTexBMP("texture/square-stone.bmp");
	_textureHexagonStone = LoadTexBMP("texture/hexagon-stone.bmp");
	_textureGreyStone = LoadTexBMP("texture/grey-stone.bmp");
	
	
	_textureBasicMetal = LoadTexBMP("texture/basic-metal.bmp");
	_textureGlass = LoadTexBMP("texture/glass.bmp");
    _textureWheel = LoadTexBMP("texture/car-wheel.bmp");
    _textureTire = LoadTexBMP("texture/tire-tread.bmp");
    _textureWoodFence = LoadTexBMP("texture/wood-fence.bmp");
	_textureGrass = LoadTexBMP("texture/grass.bmp");
	_textureCinderBlock = LoadTexBMP("texture/cinder-block.bmp");
	_textureCarGrill = LoadTexBMP("texture/car-grill.bmp");
	_textureHeadLamp = LoadTexBMP("texture/headlamp.bmp");
	_textureGlass = LoadTexBMP("texture/glass.bmp");
	_textureCarbonFiber = LoadTexBMP("texture/carbon-fiber.bmp");
	_textureSidewalk = LoadTexBMP("texture/sidewalk.bmp");
	_textureGarageDoor = LoadTexBMP("texture/garage-door.bmp");
	_textureWalkway = LoadTexBMP("texture/walkway.bmp");
	_textureHedge = LoadTexBMP("texture/hedge.bmp");
	_textureGreyBrick = LoadTexBMP("texture/grey-brick.bmp");
	_textureWoodBeam = LoadTexBMP("texture/wood-beam.bmp");
	_textureFrontDoor2 = LoadTexBMP("texture/front-door-2.bmp");
	_textureWindow1 = LoadTexBMP("texture/window-1.bmp");
	_textureSkyboxFront = LoadTexBMP("texture/skybox-front.bmp");
	_textureSkyboxBack = LoadTexBMP("texture/skybox-back.bmp");
	_textureSkyboxRight = LoadTexBMP("texture/skybox-right.bmp");
	_textureSkyboxLeft = LoadTexBMP("texture/skybox-left.bmp");
	_textureSkyboxTop = LoadTexBMP("texture/skybox-top.bmp");
	_textureAsphalt = LoadTexBMP("texture/asphalt.bmp");
	_textureBrownBrick = LoadTexBMP("texture/brown-brick.bmp");
	_textureWhiteBrick = LoadTexBMP("texture/white-brick.bmp");
	_textureMetalRoof = LoadTexBMP("texture/metal-roof.bmp");
	_textureWarehouseWindow = LoadTexBMP("texture/warehouse-window.bmp");
	
	
	_textureYellowBrick = LoadTexBMP("texture/yellow-brick.bmp");
}

static void drawZBaseRectangle(float x1, float y1, float z1, float x2, float y2, float z2) {
    glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glVertex3f(x1, y1, z1); // top left
            glVertex3f(x2, y1, z1); // top right
            glVertex3f(x2, y2, z2); // bottom right
            glVertex3f(x1, y2, z2); // bottom left
        glEnd();
    glDisable(GL_TEXTURE_2D);
}

static void drawXYBaseRectangle(float x1, float y1, float z1, float x2, float y2, float z2) {
    glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glVertex3f(x1, y1, z1); // top left
            glVertex3f(x1, y1, z2); // top right
            glVertex3f(x2, y2, z2); // bottom right
            glVertex3f(x2, y2, z1); // bottom left
        glEnd();
    glDisable(GL_TEXTURE_2D);
}

static void desert() {
    /* =============================================Sand========================================= */
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureSand);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
//            glTexCoord3f(0.0,1.0,1);  glVertex3f(-50,Y_CENTER-0.1,50); //scale 1/70
//            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-50,Y_CENTER-0.1,-50);
//            glTexCoord3f(1.0,0.0,-1);  glVertex3f(50,Y_CENTER-0.1,-50);
//            glTexCoord3f(1.0,1.0,1);  glVertex3f(50,Y_CENTER-0.1,50);
            
            glTexCoord3f(0.0,30.0,1);  glVertex3f(-50,-1.5,50); //scale 1/70
            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-50,-1.5,-50);
            glTexCoord3f(30.0,0.0,-1);  glVertex3f(50,-1.5,-50);
            glTexCoord3f(30.0,30.0,1);  glVertex3f(50,-1.5,50);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
     
}

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
   //  Save transformation ===============================================================================================
   glPushMatrix();
       //  Offset, scale and rotate
//       glTranslated(x,y,z);
       glRotated(th,0,1,0);
//       glScaled(dx,dy,dz);
    
       //Texture repitition values
       float texRepX = 1.0;
       float texRepY = 1.0;
    
       //  Cube ==============================================================================================================
       glBegin(GL_QUADS);
           //  Front
           texRepX = dx/texScale;
           texRepY = dy/texScale;
           glNormal3f( 0, 0, 1);
           glTexCoord2f(0.0,0.0);            glVertex3f(x,     y,        z); //A
           glTexCoord2f(texRepX,0.0);        glVertex3f(x+dx, y,        z); //B
           glTexCoord2f(texRepX,texRepY);    glVertex3f(x+dx, y+dy,    z); //C
           glTexCoord2f(0.0,texRepY);        glVertex3f(x,     y+dy,    z); //D
   
           //  Back
           texRepX = dx/texScale;
           texRepY = dy/texScale;
           glNormal3f( 0, 0,-1);
           glTexCoord2f(texRepX,0.0);        glVertex3f(x,     y,        z+dz); //E
           glTexCoord2f(0.0,0.0);            glVertex3f(x+dx, y,        z+dz); //F
           glTexCoord2f(0.0,texRepY);        glVertex3f(x+dx, y+dy,    z+dz); //G
           glTexCoord2f(texRepX,texRepY);    glVertex3f(x,     y+dy,    z+dz); //H

           //  Right
           texRepX = dz/texScale;
           texRepY = dy/texScale;
           glNormal3f(+1, 0, 0);
           glTexCoord2f(0.0,0.0);            glVertex3f(x+dx, y,         z); //B
           glTexCoord2f(texRepX,0.0);        glVertex3f(x+dx, y,        z+dz); //F
           glTexCoord2f(texRepX,texRepY);    glVertex3f(x+dx, y+dy,    z+dz); //G
           glTexCoord2f(0.0,texRepY);        glVertex3f(x+dx, y+dy,     z); //C
           
           //  Left
           texRepX = dz/texScale;
           texRepY = dy/texScale;
           glNormal3f(-1, 0, 0);
           glTexCoord2f(texRepX,0.0);        glVertex3f(x,     y,         z); //A
           glTexCoord2f(0.0,0.0);            glVertex3f(x,     y,        z+dz); //E
           glTexCoord2f(0.0,texRepY);        glVertex3f(x,     y+dy,    z+dz); //H
           glTexCoord2f(texRepX,texRepY);    glVertex3f(x,     y+dy,     z); //D
   
           //  Top
           texRepX = dx/texScale;
           texRepY = dz/texScale;
           glNormal3f( 0,+1, 0);
           glTexCoord2f(0.0,0.0);            glVertex3f(x,     y+dy,     z); //D
           glTexCoord2f(texRepX,0.0);        glVertex3f(x+dx, y+dy,     z); //C
           glTexCoord2f(0.0,texRepY);        glVertex3f(x+dx, y+dy,    z+dz); //G
           glTexCoord2f(texRepX,texRepY);    glVertex3f(x,     y+dy,    z+dz); //H
   
           //  Bottom
           texRepX = dx/texScale;
           texRepY = dz/texScale;
           glNormal3f( 0,-one, 0);
           glTexCoord2f(0.0,texRepY);        glVertex3f(x,     y,         z); //A
           glTexCoord2f(texRepX,texRepY);    glVertex3f(x+dx, y,         z); //B
           glTexCoord2f(texRepX,0.0);        glVertex3f(x+dx, y,        z+dz); //F
           glTexCoord2f(0.0,0.0);            glVertex3f(x,     y,        z+dz); //E
           
       //  End of Cube ====================================================================================================
       glEnd();
       
   //  Undo transofrmations ===============================================================================================
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

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

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


static void pitstop(double x, double y, double z)
{
   glPushMatrix();
	   //  Offset
	   glTranslated(x,y,z);
	//   1, 0 ,-2
	   //Building
	//   texScale = 0.5;
	   glEnable(GL_TEXTURE_2D);
	   glBindTexture(GL_TEXTURE_2D,_textureSupport);
	   		cube(0,1.6,0, 2,0.4,1, 0); //Top
	   glDisable(GL_TEXTURE_2D);
	   
	   glPushMatrix();
		   glEnable(GL_TEXTURE_2D);
		   glBindTexture(GL_TEXTURE_2D,_textureBrownBrick);
			   cube(-1.75,0.65,0, 0.25,0.55,1, 0); //Left
			   cube(1.75,0.65,0, 0.25,0.55,1, 0); //Right
			   cube(0,0.65,0, 0.2,0.55,1, 0); //Middle
	   		glDisable(GL_TEXTURE_2D);
	   glPopMatrix();  
   glPopMatrix();   
}

static void straightRoad(double tx, double ty, double tz, double roadLong, double degree, int edge)
{
    int xt, yt, zt;
    
    switch (edge) {
        case CUBE_TOP_LEFT :
            xt = tx + ROAD_WIDTH;
            yt = ty + ROAD_THICK;
            zt = tz - roadLong;
        break;
        
        case CUBE_TOP_RIGHT :
            xt = tx + ROAD_WIDTH;
            yt = ty + ROAD_THICK;
            zt = tz + roadLong;   
        break;
        
        case CUBE_BOTTOM_LEFT :
            xt = tx - ROAD_WIDTH;
            yt = ty + ROAD_THICK;
            zt = tz - roadLong; 
        break;
        
        case CUBE_BOTTOM_RIGHT :
            xt = tx - ROAD_WIDTH;
            yt = ty + ROAD_THICK;
            zt = tz + roadLong; 
        break;
        
        default:
            xt = 0;
            yt = 0;
            zt = 0; 
        break;
    }
    
    glPushMatrix();
        glTranslated(xt, yt, zt);
        glRotated(degree,0,1,0);
        glTranslated(-xt, -yt, -zt);
        cube(tx, ty, tz, ROAD_WIDTH, ROAD_THICK, roadLong, 0);
    glPopMatrix();
}

static void curveRoad(double x, double y, double z,
                 double dx, double dy, double dz,
                 double th, double radius)
{
//    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
        
           //  Save transformation
           glPushMatrix();
           //  Offset, scale and rotate
           glTranslated(x,y,z);
           glRotated(th,0,1,0);
           glScaled(dx,dy,dz);
        
           //Texture repitition values
           float texRepX = 1.0;
           float texRepY = 1.0;
        
           texRepX = dx/texScale;
           texRepY = dz/texScale;
            int i;
            float x_now; 
                    float z_now;
            const unsigned int triangles = 20; // number of triangles
            const float twoPi = 2.0f * 3.14159f;
            float delta = twoPi / triangles;
            // Top
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0,+1.0,0.0); // origin
            for(i = 0; i <= triangles/4; i++){
                x_now =  (radius * cos((i) *  delta));
                z_now =  (radius * sin((i) * delta));
                glTexCoord2f(x_now,z_now); glVertex3f(x_now,+1.0,z_now);
            }
            glEnd();
            
            // Bottom
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0,-1.0,0.0); // origin
            for(i = 0; i <= triangles/4; i++){
                x_now =  (radius * cos((i) *  delta));
                z_now =  (radius * sin((i) * delta));
                glTexCoord2f(x_now,z_now); glVertex3f(x_now,-1.0,z_now);
            }
            glEnd();
            
            // Side Circular
            glBegin(GL_TRIANGLE_STRIP);
                for(i = 0; i <= triangles/4;i++) { 
                    x_now =  (radius * cos((i) *  delta));
                    z_now =  (radius * sin((i) * delta));
                    glTexCoord2f(x_now,z_now); glVertex3f(x_now,+1.0,z_now);
                    glTexCoord2f(x_now,z_now); glVertex3f(x_now,-1.0,z_now);
                }
            glEnd();
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


static void workshop(double x, double z, double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,0,z);
   glRotated(th,0,1,0);

   //Building - Brown Workshop
   texScale = 0.5;
   cube(0,1.6,0, 2,0.4,1, 0); //Top
   cube(-1.75,0.65,0, 0.25,0.55,1, 0); //Left
   cube(1.75,0.65,0, 0.25,0.55,1, 0); //Right
   cube(0,0.65,0, 0.2,0.55,1, 0); //Middle

   //"Floor" for brown workshop
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureSidewalk );
   texScale = 1;
   cube(0,-0.05,0.9, 2,0.15,0.1, 0);
   
   //Garage Doors
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,_textureGarageDoor);
   glBegin(GL_QUADS);
   float texRepX = 1.0;
   float texRepY = 1.0;
   //Left
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1.5, 0.1, 0.9);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.2, 0.1, 0.9);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.2, 1.2, 0.9);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1.5, 1.2, 0.9);
   //Right
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2, 0.1, 0.9);
   glTexCoord2f(texRepX,0.0); glVertex3f(1.5, 0.1, 0.9);
   glTexCoord2f(texRepX,texRepY); glVertex3f(1.5, 1.2, 0.9);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2, 1.2, 0.9);
   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
   //  Save transformation ===============================================================================================
   glPushMatrix();
    
      //  Offset, scale and rotate
      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

		//front
			// center
		        glBegin(GL_QUADS);  // Wall
		            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,Y_CENTER,1.5); //A
		            glTexCoord3f(2.0,2.0,0.1);  glVertex3f(1,Y_CENTER,1.5); //B
		            glTexCoord3f(2.0,0.0,0.1);  glVertex3f(1,Y_CENTER+2,1); //C
		            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,Y_CENTER+2,1); //D
		        glEnd();
		    
			// right
		        glBegin(GL_TRIANGLES);  // top-back
		            glTexCoord3f(0,1,1);  glVertex3f(1,Y_CENTER,1.5);
		            glTexCoord3f(0.5,1,1);  glVertex3f(1.5,Y_CENTER,1.5);
		            glTexCoord3f(0,3,1);  glVertex3f(1,Y_CENTER+2,1);
		        glEnd();
	
			// left
		        glBegin(GL_TRIANGLES);  // top-back
		            glTexCoord3f(0,1,1);  glVertex3f(-1,Y_CENTER,1.5);
		            glTexCoord3f(0.5,1,1);  glVertex3f(-1.5,Y_CENTER,1.5);
		            glTexCoord3f(0,3,1);  glVertex3f(-1,Y_CENTER+2,1);
		        glEnd();
	    
	    // back
	    glPushMatrix();
	        glTranslatef(0,0,-2.5);
	        
	        // center
		        glBegin(GL_QUADS);  // Wall
		            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,Y_CENTER,1); //A
		            glTexCoord3f(2.0,2.0,0.1);  glVertex3f(1,Y_CENTER,1); //B
		            glTexCoord3f(2.0,0.0,0.1);  glVertex3f(1,Y_CENTER+2,1.5); //C
		            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,Y_CENTER+2,1.5); //D
		        glEnd();
		    
			// right
		        glBegin(GL_TRIANGLES);  // top-back
		            glTexCoord3f(0,1,1);  glVertex3f(1,Y_CENTER,1);
		            glTexCoord3f(0.5,1,1);  glVertex3f(1.5,Y_CENTER,1);
		            glTexCoord3f(0,3,1);  glVertex3f(1,Y_CENTER+2,1.5);
		        glEnd();
	
			// left
		        glBegin(GL_TRIANGLES);  // top-back
		            glTexCoord3f(0,1,1);  glVertex3f(-1,Y_CENTER,1);
		            glTexCoord3f(0.5,1,1);  glVertex3f(-1.5,Y_CENTER,1);
		            glTexCoord3f(0,3,1);  glVertex3f(-1,Y_CENTER+2,1.5);
		        glEnd();
	
	    glPopMatrix();
	    
	    // right
	    glPushMatrix();
	        glTranslatef(2.5,0,0);
	        
	        	// front
			        glBegin(GL_QUADS);  // Wall
			            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,Y_CENTER,1); //A
			            glTexCoord3f(2.0,2.0,0.1);  glVertex3f(-1,Y_CENTER,-1); //B
			            glTexCoord3f(2.0,0.0,0.1);  glVertex3f(-1.5,Y_CENTER+2,-1); //C
			            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1.5,Y_CENTER+2,1); //D
			        glEnd();
			    
				// right
			        glBegin(GL_TRIANGLES);  // top-back
			            glTexCoord3f(0,1,1);  glVertex3f(-1,Y_CENTER,-1);
			            glTexCoord3f(0.5,1,1);  glVertex3f(-1,Y_CENTER,-1.5);
			            glTexCoord3f(0,3,1);  glVertex3f(-1.5,Y_CENTER+2,-1);
			        glEnd();
		
				// left
			        glBegin(GL_TRIANGLES);  // top-back
			            glTexCoord3f(0,1,1);  glVertex3f(-1,Y_CENTER,1);
			            glTexCoord3f(0.5,1,1);  glVertex3f(-1,Y_CENTER,1.5);
			            glTexCoord3f(0,3,1);  glVertex3f(-1.5,Y_CENTER+2,1);
			        glEnd();
	
	    glPopMatrix();
	    
	    // left
	        	// front
			        glBegin(GL_QUADS);  // Wall
			            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1.5,Y_CENTER,1); //A
			            glTexCoord3f(2.0,2.0,0.1);  glVertex3f(-1.5,Y_CENTER,-1); //B
			            glTexCoord3f(2.0,0.0,0.1);  glVertex3f(-1,Y_CENTER+2,-1); //C
			            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,Y_CENTER+2,1); //D
			        glEnd();
			    
				// right
			        glBegin(GL_TRIANGLES);  // top-back
			            glTexCoord3f(0,1,1);  glVertex3f(-1.5,Y_CENTER,-1);
			            glTexCoord3f(0.5,1,1);  glVertex3f(-1.5,Y_CENTER,-1.5);
			            glTexCoord3f(0,3,1);  glVertex3f(-1,Y_CENTER+2,-1);
			        glEnd();
		
				// left
			        glBegin(GL_TRIANGLES);  // top-back
			            glTexCoord3f(0,1,1);  glVertex3f(-1.5,Y_CENTER,1);
			            glTexCoord3f(0.5,1,1);  glVertex3f(-1.5,Y_CENTER,1.5);
			            glTexCoord3f(0,3,1);  glVertex3f(-1,Y_CENTER+2,1);
			        glEnd();
	
	    
	    // top
	    glPushMatrix();
	        glTranslatef(0,2,0);
	        
	        	// front
			        glBegin(GL_QUADS);  // Wall
			            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,Y_CENTER,1); //A
			            glTexCoord3f(2.0,2.0,0.1);  glVertex3f(1,Y_CENTER,1); //B
			            glTexCoord3f(2.0,0.0,0.1);  glVertex3f(1,Y_CENTER,-1); //C
			            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,Y_CENTER,-1); //D
			        glEnd();
			
		glPopMatrix();
	        
	        
	glPopMatrix();
}

static void pyramid (double x,double y,double z,
                 double dx,double dy,double dz,
                 double th) {
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
      
//      printf("x:%d y:%d z:%d\n", Ex, Ey, Ez);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  First Person
   else
   {
       
//      refX = (dim * Sin(th)) + fpX;
   	  refX = (dim * Sin(th));
      refY = (dim * Sin(ph))+fpY;
      //refZ = (dim * -Cos(th)) + fpZ;
      refZ = (dim * -Cos(th));
      
//      gluLookAt(-3.5,10,2.3, refX,refY,refZ, 0,1,0);
       
//      centerX = (dim * Sin(th)) + fpX;
//      centerY = (dim * Sin(ph));
//      centerZ = (dim * -Cos(th)) + fpZ;
//      gluLookAt(fpX,fpY,fpZ, centerX, centerY, centerZ, 0,1,0);
//        gluLookAt(eyeX,eyeY,eyeZ, centerX, centerY, centerZ, 0,1,0);


//      gluLookAt(fpX,fpY,fpZ, refX,refY,refZ, 0,1,0);
        glRotated(-carRotate2,0,1,0);
//	glRotated(90,0,1,0);
        printf("%f\n", refY);
//    	glRotated(90,0,1,0);
//      gluLookAt(25.5+centerZIncrement,1,22-centerXIncrement, 22+centerZIncrement,refY,5-centerXIncrement, 0,1,0);
        gluLookAt(16+centerXIncrement, 0 , 26.3+centerZIncrement, 24.510370+centerXIncrement,refY,25.941143+centerZIncrement, 0,1,0);
   }

   //  Draw scene =========================================================================================================
   //  Skybox
   skybox(100);
   
//   glPushMatrix();
//      glEnable(GL_LIGHT1);
//      float amb[4] = {0,0,0,0};
//      float dif[4] = {0.8,0,0,1};
//      float spec[4] = {0,0,0,1};
//      float pos[4] = {-0.2,5,-3,1.0};
//      //Red Light
//      glLightfv(GL_LIGHT1,GL_AMBIENT ,amb);
//      glLightfv(GL_LIGHT1,GL_DIFFUSE ,dif);
//      glLightfv(GL_LIGHT1,GL_SPECULAR,spec);
//      glLightfv(GL_LIGHT1,GL_POSITION,pos);
//
//      glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,at0/100.0);
//      glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,at1/100.0);
//      glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,at2/100.0);
//
//      //Red Light
//      float redEm[4] = {0.8, 0, 0, 1.0};
//      glMaterialf(GL_FRONT,GL_SHININESS,0);
//      glMaterialfv(GL_FRONT,GL_SPECULAR,redEm);
//      glMaterialfv(GL_FRONT,GL_EMISSION,redEm);
//      glColor3f(0.5, 0, 0);
//      cube(-0.2,5,-3, 0.07,0.02,0.1, 0);
//   glPopMatrix();
   
   //  Desert
   desert();
   
//   drawXYBaseRectangle(3, 0, -2, 9, 0, 4);
   
//  pitstop(1, 0, -2);   
//   pitstop(3, 0, -2);
//   pitstop(-1, 0, -2);
//          tx, ty, tz, long, rotate

int index=0;
int roadNum;

glEnable(GL_TEXTURE_2D);
	
    glBindTexture(GL_TEXTURE_2D, _textureAsphalt);
	glPushMatrix();
//	    glScaled(-1,1,-1);
		for(roadNum=0; roadNum < circuitSize/NUM_ROAD_PARAMS; roadNum++) {
		    straightRoad( circuit[index+0], //tx
		                  circuit[index+1], //ty
		                  circuit[index+2], //tz
		                  circuit[index+3], //road long
		                  circuit[index+4], //degree
		                  circuit[index+5] //rotate pivot
		                );  
		    index += NUM_ROAD_PARAMS;
		}
	glPopMatrix();
glDisable(GL_TEXTURE_2D);
	
	
//index=0;
//for(roadNum=0; roadNum < circuitSize/NUM_ROAD_PARAMS; roadNum++) {
//    straightRoad( sidewalk[index+0], //tx
//                  sidewalk[index+1], //ty
//                  sidewalk[index+2], //tz
//                  sidewalk[index+3], //road long
//                  sidewalk[index+4], //degree
//                  sidewalk[index+5], //rotate pivot
//                  sidewalk[index+6]  //texture type
//                );
//                 
//    index += NUM_ROAD_PARAMS;
//}
//curveRoad(0,0,0,2,0,2,0,5);
   //Brown house - player side
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureBrick);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		house(circuit[0]+10,0,20,1,1,1,0);

    glDisable(GL_TEXTURE_2D);
    
    //Pyramid
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureYellowBrick);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		pyramid(-5,1,-5,3,3,3,40);
		
		pyramid(15,0,15,2,2,2,0);
		pyramid(10,-1,15,1,1,1,0);
    //Car
    glDisable(GL_TEXTURE_2D);
    
        glPushMatrix();
    	glTranslated(18,0,24);
    	//glRotated(90,0,1,0);
	   //Red Car
//	   car(0,0.13,1.8, 1,1,1, 90, 0.8,0,0);
		car(-2+centerXIncrement, -1 ,2.3+centerZIncrement, 1,1,1, carRotate2, 0,0,0.8);
    glPopMatrix();
        
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
   th %= 360;
  // ph %= 360;
   
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
   printf("th:%d fov:%d ph:%d, refX:%f, refZ:%f, \n", th, fov, ph, refX, refZ);
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
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
    else if(ch == 'w' || ch == 'W')
   {      
          temp = xRotate;
          xRotate = xRotate*Cos(0) + zRotate * Sin(0);
          zRotate = -temp*Sin(0)+zRotate*Cos(0);
          centerXIncrement += xRotate;
          centerZIncrement += zRotate;
          carRotate2 += 0;
   }
   
   else if(ch == 's' || ch == 'S')
   {      
          temp = xRotate;
          xRotate = xRotate*Cos(0) + zRotate * Sin(0);
          zRotate = -temp*Sin(0)+zRotate*Cos(0);
          centerXIncrement -= xRotate;
          centerZIncrement -= zRotate;
          carRotate2 += 0;
   }
     
   else if(ch == 'd' || ch == 'D')
      {
          temp = xRotate;
          xRotate = xRotate*Cos(-15) + zRotate * Sin(-15);
          zRotate = -temp*Sin(-15)+zRotate*Cos(-15);       
          centerXIncrement += xRotate;
          centerZIncrement += zRotate;
         
          carRotate2 -= 15;
         
      }
   else if(ch == 'a' || ch == 'A')
   {
          temp = xRotate;          
          xRotate = xRotate*Cos(15) + zRotate * Sin(15);
          zRotate = -temp*Sin(15)+zRotate*Cos(15);
          centerXIncrement += xRotate;
          centerZIncrement += zRotate;
          carRotate2 += 15;    
   }  
      
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
