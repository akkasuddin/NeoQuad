/******************************************************************
 * Name   : NeoQuad.cpp
 * Purpose: NeoQuad member functions defined here
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/

#include "NeoQuad.h"
#include <GL/glut.h>
#include <cmath>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include "ImageLoader/pgmIO.h"
#include "ImageLoader/bmpIO.h"
using namespace std;

#define SOLID_RENDERING 

NeoQuad::NeoQuad()
{
    propAngle = 0.0;
    propSpeed = 2;
    animate = true;
    currentState = NEUTRAL;
    timeline = new Timeline("NEO");
    //timeline->displayLines();
    LoadGLTextures();
}
void NeoQuad::drawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
    glPushMatrix();
    glScalef(fA,fB,fC);
   // glMatrixMode(GL_TEXTURE);
    
   // glScalef(fA,1/fB,1/fC);
  //  glMatrixMode(GL_MODELVIEW);
    gluSphere(quadricObj,1.0f,uiSlices,uiStacks);
    glPopMatrix();
    
}
void DrawTorus( double innerRadius, double outerRadius, GLint sides, GLint rings ) {

   // glScalef(5,5,5);
    double TWOPI = 2 * Pi;
    for (int i = 0; i < sides; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= rings; j++) {
            for (int k = 1; k >= 0; k--) {
                
                double s = (i + k) % sides + outerRadius/2;
                double t = j;// % rings;
                double theta = s * TWOPI / sides;
                double rho = t * TWOPI / rings;

                double x = (outerRadius + innerRadius * cos(theta)) * cos(rho);
                double y = (outerRadius + innerRadius * cos(theta)) * sin(rho);
                double z = innerRadius * sin(theta);
                float u = (i + k) / (float)sides;
                float v = j;// (float)rings;
                float nx = cos(rho)*cos(theta);
                float ny = sin(rho)*cos(theta);
                float nz = sin(theta);   
                glNormal3f(nx,ny,nz);
                glTexCoord2f(u,v);
                
                glVertex3d(x, y, z);
            }
        }
        glEnd();
    }
  //  glScalef(.2,.2,.2);
    
}

void NeoQuad::drawBlade()
{
    glDisable(GL_CULL_FACE);
    //Draw blade
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glVertex3f(0.5f, 10.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
    //Draw blade
    
    glEnable(GL_CULL_FACE);
    
}

void NeoQuad::drawPropellers(int rotorDirection)
{
    glPushMatrix();
    static float propScale = 1;
    
    if(currentState == POWERINGUP)
    {
        propScale +=0.015*.25;
    }
    
    if(currentState == POWERINGDOWN)
    {
        propScale -=0.015*.25;
    }
    glScalef(propScale,propScale,propScale);
    gluCylinder(quadricObj, 0.25f, 0.25f, 3.0f, 10.0f, 10.0f);
    glTranslatef(0.0f, 0.0f, 3.0f);
    
    DrawTorus(.5f, 10.0f, 10.0f, 30.0f);
    if (rotorDirection)
        glRotatef(propAngle, 0.0f, 0.0f, 1.0f);
    else
        glRotatef(-propAngle, 0.0f, 0.0f, 1.0f);
    
    drawBlade();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    drawBlade();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    drawBlade();
    
    glPopMatrix();
    
}

void NeoQuad::drawGun(float gunScale)
{
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    //drawAxes();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quadricObj,0.0f,2.0f,10,3);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    
    gluCylinder(quadricObj, 2.0f, 2.0f, 15.0f, 10.0f, 10.0f);
    glTranslatef(0.0f,0.0f,15.0f);
    
    glScalef(1.0f,1.0f,gunScale);
    gluDisk(quadricObj,1.0f,2.0f,10,3);
    gluCylinder(quadricObj, 1.0f, 1.0f, 10.0f, 10.0f, 10.0f);
    glTranslatef(0.0f,0.0f,10.0f);
    gluDisk(quadricObj,.5f,1.0f,10,3);
    gluCylinder(quadricObj, .5f, .5f, 10.0f, 10.0f, 10.0f);
    
    
}


void NeoQuad::drawGuns()
{
    static float gunScale = 0;
    
    if(currentState == POWERINGUP)
    {
        gunScale +=0.025;
        if((int)gunScale == 1)
            currentState = POWEREDUP;
    }
    
    if(currentState == POWERINGDOWN)
    {
        gunScale -=0.025;
        if((int)(gunScale*10) == 0)
            currentState = NEUTRAL;
    }
    
    
    //drawGuns
    glPushMatrix();
    glTranslatef(-7.0f,0.0f,6.0f);
    drawGun(gunScale);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-7.0f,0.0f,-6.0f);
    drawGun(gunScale);
    glPopMatrix();
    
}

void NeoQuad::LoadGLTextures()
{
    int w, h, c;
       glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    
    // Load texture map for sphere.
    data = PGM_FILE_READ("Data/image.ppm", &w, &h, &c); 
    /*BITMAPINFO *BitmapInfo; 
    data = LoadDIBitmap("Data/perf_white.bmp",&BitmapInfo);
    w = BitmapInfo->bmiHeader.biWidth;
    h = BitmapInfo->bmiHeader.biHeight;
    */
    glGenTextures(1, &EarthMap);
    glBindTexture(GL_TEXTURE_2D, EarthMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );   
    
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data );
    
    
    free(data);
    //free(BitmapInfo);
    // Load texture map for sphere.
    data = PGM_FILE_READ("Data/perf_white.ppm", &w, &h, &c); 
    /*BITMAPINFO *BitmapInfo; 
     d ata = Load*DIBitmap("Data/perf_white.bmp",&BitmapInfo);
     w = BitmapInfo->bmiHeader.biWidth;
     h = BitmapInfo->bmiHeader.biHeight;
     */
    glGenTextures(1, &propTex);
    glBindTexture(GL_TEXTURE_2D, propTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );   
    
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data );
    
    
    free(data);
    
    gluQuadricDrawStyle(quadricObj, GLU_FILL);
    gluQuadricTexture(quadricObj, GL_TRUE);
    gluQuadricNormals(quadricObj, GLU_SMOOTH);
 }

void NeoQuad::drawQuad()
{
    glEnable(GL_TEXTURE_2D);  
    rotateProps();
    
    glMultMatrixf(glm::value_ptr(Model)); //load Model matrix
    glBindTexture(GL_TEXTURE_2D, EarthMap);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    
    //Draw Axes
    //drawAxes();
    
    glColor4f(.75f, .75f, .75f, 1.0f);
    //DrawTorus();
    //return;
    //draw body
    glRotatef(-90.0f,0.0f,1.0f,0.0f);
    drawEllipsoid(20, 20, 5, 5, 15);
    glRotatef(90.0f,0.0f,1.0f,0.0f);
    
    glTranslatef(15.0f, 0.0f, 0.0f);
    

    //GLUquadricObj *quadricObj = gluNewQuadric();
    
    gluSphere(quadricObj, 5.0f, 20, 20); //draw head
    //gluDeleteQuadric(quadricObj);
    
    //glDisable(GL_TEXTURE_2D);
    glTranslatef(-15.0f, 0.0f, 0.0f);
    
    glBindTexture(GL_TEXTURE_2D, propTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //draw Arms
    glPushMatrix();    
    glTranslatef(20.0f, 5.0f, 20.0f);
    glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);
    
    gluSphere(quadricObj, .5, 15, 15); 
    gluCylinder(quadricObj, 0.25f, 0.25f, 40.0f*sqrt2, 10.0f, 10.0f);
    
    glTranslatef(0.0f,0.0f,40.0f*sqrt2);
    gluSphere(quadricObj, .5, 15, 15);
    glTranslatef(0.0f,0.0f,-40.0f*sqrt2);
    
    glTranslatef(20.0f*sqrt2,0.0, 20.0f*sqrt2);
    glRotatef(90.0f, 0.0, -1.0f, 0.0f);
    //drawAxes();
    gluSphere(quadricObj, .5, 15, 15); 
    gluCylinder(quadricObj, 0.25f, 0.25f, 40.0f*sqrt2, 10.0f, 10.0f);
    
    glTranslatef(0.0f,0.0f,40.0f*sqrt2);
    gluSphere(quadricObj, .5, 15, 15);
    glTranslatef(0.0f,0.0f,-40.0f*sqrt2);
    
    //Draw 
    
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    //drawAxes();
    for(int i=0;i<4;i++)
    {
        drawPropellers(i%2);
        glTranslatef(20.0f*sqrt2, -20.0f*sqrt2, 0.0f);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    }
    
    glPopMatrix();
    
    if(currentState != NEUTRAL)
    {
        drawGuns();
        
    }
    
    glDisable(GL_TEXTURE_2D);
    
}


void NeoQuad::rotateProps()
{
    if (animate)
    {
        
        propAngle += propSpeed*propTime.getTimeDiffSec()*1000;
    }
    propTime.getTimeDiffSec();
    
}

void NeoQuad::toggleAnimate()
{
    animate = !animate;
}

void NeoQuad::changePropSpeed(float increment)
{
    propSpeed+=increment;
}


void NeoQuad::powerToggle()
{
    if(currentState == NEUTRAL)
        currentState = POWERINGUP;
    else if(currentState == POWEREDUP)
        currentState = POWERINGDOWN;
}

