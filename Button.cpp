#include "Button.h"



#include<GL/glut.h>
#include <string.h>

vector<Button*> Button::buttons;

Button::Button(int left, int top, int right, int bottom,char*label, void(* mouseClickCallback)(void))
{
    
    strcpy(this->label,label);
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
    state = GLUT_UP; 
    Button:: buttons.push_back(this);
    this->buttonClickEvent = mouseClickCallback;
    

}

bool Button::bounds(int x, int y)
{
    if((left<x && x<right) && (bottom<y && y<top))
        return true;
    else 
        return false;

}





void Button::draw()
{
    glDisable(GL_LIGHTING);
    //glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    //glOrtho(left,top,right, bottom,);
    glTranslatef(0,0,-.1);
    

    
    glLineWidth(2);
    //glTranslatef(0, 0, 0);
    if(state == GLUT_UP)
        glColor3f(.85f,.85f,.85f);
    else
        glColor3f(.5f,.5f,.5f);
    
    glBegin(GL_LINE_STRIP);
        glVertex2f(right-1,top-1);
        glVertex2f(left+1,top-1);
        glVertex2f(left+1,bottom+1);
        //glVertex2f(right,bottom);
    glEnd();
    
    if(state == GLUT_UP)
        glColor3f(.5f,.5f,.5f);
    else
        glColor3f(.85f,.85f,.85f);
    
    glBegin(GL_LINE_STRIP);
    //glVertex2f(right,top);
        glVertex2f(left+1,bottom+1);
        glVertex2f(right-1,bottom+1);
        glVertex2f(right-1,top-1);
    glEnd(); 
   
    
    glColor3f(.75f,.75f,.75f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(left+1,top-1);
        glVertex2f(left+1,bottom+1);
        glVertex2f(right-1,bottom+1);
        glVertex2f(right-1,top-1);
    glEnd();
    
    glLineWidth(1);
    
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(left+3, bottom+3, .1);
    //glTranslatef(left,top,0);
    float scale =  (top-bottom-6)/100.0; 
    glScalef( scale,scale,scale);
    for( char* p = label; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
}



Button* Button::checkClick(int buttonState, int x, int y)
{
    int xx = buttons.size();
    for(vector<Button*>::iterator it = buttons.begin(); it!=buttons.end();it++)
    {
        if((*it)->bounds(x,y))
        {
            (*it)->state = buttonState;
            if(buttonState==GLUT_DOWN)
                (*it)->buttonClickEvent();
            return (*it);
            
        }
    }
    return NULL;
    
}

void Button::drawButtons()
{
    for(vector<Button*>::iterator it = buttons.begin(); it!=buttons.end();it++)
    {
        (*it)->draw();
    }
    
}

