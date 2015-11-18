/******************************************************************
 * Name   : Button.h
 * Purpose: Button class for creating buttons in opengl
 *          Have not used any glut functions at all :)
 *          Have implemented a system of callbacks for easy handling
 *          of mouse clicks
 *         
 * Author: Akkas Uddin Haque
 * Email : akkasuddin@live.com
 *
 * Creation Date: 10/22/2015
 *******************************************************************/
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include<vector>
//#include<string>
using namespace std;

class Button
{
private:
    int left,top,right,bottom;
    int state;
    char label[120];
    //string label;
    //checks if boundary conditions are met 
    inline bool bounds(int x, int y);
    //draws the button
    void draw(); 
    //button click event callee
    void (*buttonClickEvent)(void);
    
 
public:
    //Constructor
    Button(int left,int top,int right,int bottom,char *label /*string label*/, void(* mouseClickCallback)(void));
    
    static vector<Button*> buttons;
    static Button* checkClick(int buttonState,int x,int y);
    static void drawButtons();
    
};


#endif