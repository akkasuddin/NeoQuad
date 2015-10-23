
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include<vector>

using namespace std;

class Button
{
private:
    int left,top,right,bottom;
    int state;

    char label[120];
 
public:
    
    Button(int left,int top,int right,int bottom,char *label, void(* mouseClickCallback)(void));
    bool bounds(int x, int y);

    void draw(); 
    
    
    static int btnID;
    static vector<Button*> buttons;
    static Button* checkClick(int buttonState,int x,int y);
    static void drawButtons();
    void (*buttonClickEvent)(void);

};


#endif