// OpenGL_Game.cpp
// Bare bones version of "Happy Vikings"
// C.Markham Jan 2015  

/* Credits:
 *	Ninja star sprite	- http://vignette1.wikia.nocookie.net/finalfantasy/images/6/68/MQNinjaStar.PNG/revision/latest?cb=20121019051146
 *  Running man sprite	- Can't find the link.
 *  Background			- Leonardo Da Vinci.
 */

#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>    // Required for sound only (consider OpenAL), 
#include <mmsystem.h>   // Again only required for sound only

#include <freeglut.h>   // GLUT OpenGL Utility Toolkit
#include "Sprite.h"     // Prototype Sprite class

// 3 Sprites are used in the "game"
Sprite player;
Sprite star;
bool star_falling = false;
Sprite background;

// Score data and method
int score=0;
char score_text[20];


// Callback function called each time screen is to be redrawn (called by OS not you directly)
static void display(void)                                 
{															// Keep game logic out of display()...
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f) ;
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the Depth Buffer

		background.drawSprite();							// Draw background at X,Y in a given direction
		player.drawSprite();								// Draw player	
		star.drawSprite();								    // Draw ball 
	    
		sprintf(score_text,"Score: %d",score);              // Display text 
		Sprite::text(30,610,score_text);

		glutSwapBuffers();  // Display buffer just drawn and then switch to draw on the other buffer
}


// Callback function to allow dynamic resizing of runtime display
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
	Sprite::spriteModeOn(0,0);  // (0,0) Causes orginal screen size to be reused
}

// Callback to manage standard keys on the keyboard
// In this case 'q' or 'Q' causes program to terminate
static void qwerty_keys(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'Q':
    case 'q': glutLeaveMainLoop () ; break;  // (Stop!)
    default: break;
    }
}

// Callback for cursor keys on the keyboard
// <- and -> cursors keys cause player to move left and right
static void function_keys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
			if (player.sprite_x_position >= 0) {
				player.advance_frame(0,7);
				player.sprite_x_position-=10;
				player.direction=false;
			}
		break;

    case GLUT_KEY_RIGHT: 
			if (player.sprite_x_position < 400) {
				player.advance_frame(0,7);
				player.sprite_x_position+=10;
				player.direction=true;
			}
		break;

    default: break;
    }
}

// Game loop
// static void idle()    // if using  glutIdleFunc(idle); this line should replace the one below
static void idle(int v)  // Runs between frame updates in doublebuffer mode, game logic goes here
{
	star.advance_projectile();
	if(star.sprite_y_position<20)   // if it has landed
	{ 	   
		score++;
		star.sprite_y_position=640;
		star.sprite_y_velocity=1;
		star.sprite_x_position= rand() % 432;
	}

	if(player.collision(star)) score=0; // If player collides with ball score is reset to zero

    glutPostRedisplay();      // Send message to ask for screen to be redrawn
	glutTimerFunc(10,idle,0); // Restart the timer (remove line if using the more common glutIdleFunc(idle) approach)
}

/* Program entry point - starts here */
int main(int argc, char *argv[])
{
	Sprite::spriteModeOn(432,640);		// Create a screen 640x480 to contain game 

	// load sprite files (24bit BMP), Frames (X,Y), position (X,Y), size (Width,Height), direction
	// Use sprites with Width and Height divisible by 4, ideally powers of two e.g. 256x128

	player.load("guy.bmp",8,1,320,10,64,88,true);  
	star.load("star.bmp",1,1,100,640,36,36,true);         
	background.load("background.bmp",1,1,0,0,432,640,true); 

    glutReshapeFunc(resize);			// Called when window is resized (not required)
	glutDisplayFunc(display);			// Callback function to redraw, an event created by glutPostRedisplay()
    glutKeyboardFunc(qwerty_keys);		// QWERTY Keys
    glutSpecialFunc(function_keys);		// Cursor keys etc 

	//glutIdleFunc(idle);			    // Called during idle time 
	glutTimerFunc(10,idle,0);           // Use timer callback for more consistent timing
	
	glutMainLoop();						// Start main loop running (go!)
    
	return EXIT_SUCCESS;
}