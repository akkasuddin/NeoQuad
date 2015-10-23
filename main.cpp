/******************************************************************
 * Name: main.cpp
 * Purpose: main routines are defined here
 * 
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 09/19/2015
 *******************************************************************/

#include<iostream>
#include<cmath>
#include<GL/glut.h>
#include "QuadTimer.h"
#include "camera.h"

#include "NeoQuad.h"
#include "Button.h"

using namespace std;
using namespace glm;

//Create the Camera
Camera camera;

class Window {
public:
    Window() {
        this->interval = 1000 / 60;             //60 FPS
        this->window_handle = -1;
    }
    int window_handle, interval;
    ivec2 size;
    float window_aspect;
} window;


//Resize the window and properly update the camera viewport
void ReshapeFunc(int w, int h) {
    if (h > 0) {
        window.size = ivec2(w, h);
        window.window_aspect = float(w) / float(h);
    }
    camera.SetViewport(0, 0, window.size.x, window.size.y);
}

//Keyboard input for camera, also handles exit case
void CameraKeyboardFunc(unsigned char c, int x, int y) {
    switch (c) {
        case 'i':
            camera.Move(FORWARD);
            break;
        case 'j':
            camera.Move(LEFT);
            break;
        case 'k':
            camera.Move(BACK);
            break;
        case 'l':
            camera.Move(RIGHT);
            break;
        case 'u':
            camera.Move(DOWN);
            break;
        case 'o':
            camera.Move(UP);
            break;
        case 'x':
        case 27:
            exit(0);
            return;
        default:
            break;
    }
}
bool cameraControltoggle = true;
//Used when person clicks mouse
void CallBackMouseFunc(int mousebutton, int state, int x, int y) {
    if(cameraControltoggle)
        camera.SetPos(mousebutton, state, x, y);

    if(Button::checkClick(state, x,window.size.y-y)!=NULL)
    {
        if(state == GLUT_DOWN)
            cameraControltoggle = false;
            
    }
    if(state == GLUT_UP)
        cameraControltoggle = true;
    
   
}
//Used when person drags mouse around
void CallBackMotionFunc(int x, int y) {
    if(cameraControltoggle)
        camera.Move2D(x, y);
}

//Redraw based on fps set for window
void TimerFunc(int value) {
    if (window.window_handle != -1) {
        glutTimerFunc(window.interval, TimerFunc, value);
        glutPostRedisplay();
    }
}


void initializeRendering()
{
    glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Set the blending function for translucency (note off at init time)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // This Will Clear The Background Color To Black
    glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                       // type of depth test to do.
    // glEnable(GL_DEPTH_TEST);                    // enables depth testing.
    glShadeModel(GL_SMOOTH);                    // Enables Smooth Color Shading
    
    glEnable(GL_LIGHTING); //Enable lighting
    //   glMatrixMode(GL_PROJECTION);
    //  glLoadIdentity();                           // Reset The Projection Matrix
    
    //   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);   // Calculate The Aspect Ratio Of The Window
    
    glMatrixMode(GL_MODELVIEW);
    
    /*****************************************For Background***********************/ 
    
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
   
   
    GLfloat LightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f}; 
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);//*/
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glEnable(GL_LIGHT1);
    
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
    const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glEnable(GL_LIGHT0);
    
 
    
    /*****************************************For Background***********************/
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_LIGHT0); //Enable light #0
    // glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glEnable(GL_CULL_FACE);
    
}



Button *powerButton,*speedUpButton, *speedDownButton;
NeoQuad *neoQuad;
void keypressHandler(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'd':
            neoQuad->yawQuad(2);
            break;
        case 'a':
            neoQuad->yawQuad(-2);
            break;
        case 's':
            neoQuad->rollQuad(2);
            break;
        case 'w':
            neoQuad->rollQuad(-2);
            break;
        case 'f':
            neoQuad->pitchQuad(2);
            break;
        case 'g':
            neoQuad->pitchQuad(-2);
            break;
        case ';':
            neoQuad->changePropSpeed(-0.1);
            break;
        case '\'':
            neoQuad->changePropSpeed(0.1);
            break;
        case 'p':
            neoQuad->powerToggle();
            break;
        case 'm':
            neoQuad->toggleAnimate();
            break;
        
        case 13:
            exit(0);
            
    };
    CameraKeyboardFunc(key,x,y);
    //glutPostRedisplay();
}

void drawHandler()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0);                          // Enables Clearing Of The Depth Buffer
    glPushMatrix();
    // glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, window.size.x, window.size.y);
    
    glm::mat4 model, view, projection;
    camera.Update();
    camera.GetMatricies(projection, view, model);
    
    glm::mat4 mvp = projection* view * model;       //Compute the mvp matrix
    glLoadMatrixf(glm::value_ptr(mvp));
    glColor3f(1.0f,1.0f,1.0f);
    //*
    neoQuad->draw();
    
    glPopMatrix();
    
    
    
    
    
    //glDisable(GL_CULL_FACE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, window.size.x, 0.0f,window.size.y, 0.0f, 100000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.0f,0.0f,0.0f);
    glutSolidTeapot(1);
    
    Button::drawButtons();

    
    
    glutSwapBuffers();
    
}



void powerButtonCallback()
{
    neoQuad->powerToggle();    
}

void speedUpCallback()
{
    neoQuad->changePropSpeed(0.1);
}

void speedDownCallback()
{
    neoQuad->changePropSpeed(-0.1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); //initialize glut 
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //initialize display mode
    
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(0, 0);
    window.window_handle = glutCreateWindow("Age of Quadrones"); //create window
    int btnid;
    powerButton = new Button(100,110,200,90,"PowerUP",powerButtonCallback);
    speedUpButton = new Button(50,90,150,70,"SpeedUP",speedUpCallback);
    speedDownButton = new Button(150,90,250,70,"SpeedDN",speedDownCallback);
    int xxx = Button::buttons.size();
  //  int ids = Button::btnID;
    //*
    glutReshapeFunc(ReshapeFunc);
    glutDisplayFunc(drawHandler);
    glutKeyboardFunc(keypressHandler);
    glutMouseFunc(CallBackMouseFunc);
    glutMotionFunc(CallBackMotionFunc);
    glutTimerFunc(window.interval, TimerFunc, 0);
    
    neoQuad = new NeoQuad();
    neoQuad->moveAbs(10,60,0);
    initializeRendering();//Setup camera
    
    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(0, 60, 100));
    camera.SetLookAt(glm::vec3(0, 60, 0));
    camera.SetClipping(.1, 80000);
    camera.SetFOV(45);
    //Start the glut loop!
    //*/
    glutMainLoop();
    return 0;
    
    
}

