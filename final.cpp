/*
Programmers: Michael Wang and Justin Peek
Class: cse20311
Assignment: Final Project
Purpose: To simulate the game of skee-ball 
*/

#include <iostream>
#include <cmath>
#include "gfxnew.h"
#include "skeeball.h"
#include "unistd.h"
#include <string>

using namespace std;

void drawBoard();
int rollBall(float ballSpeed, float ballAngle, float xf, float yf, int balls, int ballRadius); //animation
float speedBall(int xi, int yi, float xf, float yf); //waits for user to click and release the mouse, returns the speed of the ball based on how far the mouse moves before its click is released
float angleBall(int xi, int yi, float xf, float yf); //calculates the angle using the click and release points of the mouse
void displayBalls(int balls, int ballRadius);

int main()
{
	int screenWidth = 500, screenHeight = 1000;

	bool loop = true;
	bool isclicked = false;
	char c = ' ';
	int event = 0;
	//ball attributes
	int ballRadius = 15;
	float ballxi = 0, ballyi = 0;
	float ballxf = 0, ballyf = 0;
	float ballSpeed = 0.0;
	float ballAngle = 0.0;
	int points;
	string score;
	bool newhighscore = false;
	string highscore;

	Skeeball data(9,0,0);

	gfx_open(screenWidth, screenHeight, "Skee-Ball");
	drawBoard();
	displayBalls(data.getballs(), ballRadius); 

	cout << "Welcome to SkeeBall! You have 9 balls to get as many points as possible!" << endl;
	cout << "Click, drag, and release your mouse in order to throw a ball." << endl;
	cout << "You may only use the bottom half of the red ramp (can't release past line)." << endl;
	cout << "Press p to play again and q to quit. Good luck!" << endl;

	while (loop)
	{
		score = to_string(data.getscore()); //convert the score to a string
		gfx_text(125, 60, score.c_str()); //convert the string into a char using cstrings
		highscore = to_string(data.gethighscore());
		gfx_text(360, 60, highscore.c_str());

		if ((event = gfx_event_waiting()) != 0)
		{
			c = gfx_wait();

			if ((c == 1 || c == 2 || c == 3) && (gfx_xpos() >= 100 && gfx_xpos() <= 400) && (gfx_ypos() >= 500 && gfx_ypos() <= 1000) && data.getballs() > 0) 
			{
				if (event == 3) //mouse down event
				{
					isclicked = true;	
					ballxi = gfx_xpos();	
					ballyi = gfx_ypos();
				}
				else if (event == 4) //mouse release event
				{
					isclicked = false;
					ballxf = gfx_xpos();
					ballyf = gfx_ypos();
					data.useballs(); //subtract a ball

					ballSpeed = speedBall(ballxi, ballyi, ballxf, ballyf); //find speed of ball
					ballAngle = angleBall(ballxi, ballyi, ballxf, ballyf); //find angle of click to click release
					//update board
					drawBoard();
					displayBalls(data.getballs(), ballRadius); //display the number of balls left

					//calculate and update the scores and display
					points = rollBall(ballSpeed, ballAngle, ballxf, ballyf, data.getballs(), ballRadius); //animate the ball and return the points
					data.addscore(points); //add the points to the total points
					score = to_string(data.getscore()); //convert the score to a string
					gfx_text(125, 60, score.c_str()); //convert the string into a char using cstrings
				}
			}
			if (c == 'q')
				return 1;
		}
		if (data.getballs() == 0)
			cout << "Game Over! Press q to quit or p to play again!" << endl;

		while (data.getballs() == 0) //when the user runs out of balls
		{
			if ((event = gfx_event_waiting()) != 0)
			{
				c = gfx_wait();
				
				newhighscore = data.comparehighscore(data.getscore()); //compare the current score with the high score
			
				if (newhighscore) //if the newhighscore is greater than the highscore
				{
					highscore = to_string(data.getscore()); //set highscore on the board to the current score
					gfx_text(360, 60, highscore.c_str());
					data.sethighscore(data.getscore());
				}
				else 
				{
					highscore = to_string(data.gethighscore()); //dont change the current high schoore
					gfx_text(360, 60, highscore.c_str());
				}

				if (c == 'q')
					return 1;

				if (c == 'p')
				{
					data.setballs(9);
					data.setscore(0);
					drawBoard();
					displayBalls(data.getballs(), ballRadius);
					break;
				}
			}
		}
	}
	return 0;
}
void drawBoard()
{
	gfx_clear_color(195,196,181);  // Changes the background color to gray
  	gfx_clear(); // Allows the background to change colors
// Makes the "ramp"
	gfx_color(201, 36, 36); // Changes the applying color to red
    gfx_fill_rectangle(101, 361, 299, 639);
// Makes the bounds of the game
    gfx_color(0,0,0); // Changes the applying color to black
    gfx_arc(360, 360, 100, 100, 0, 90);
    gfx_line(400, 1000, 400, 360);
    gfx_line(100, 1000, 100, 360);
    gfx_line(460, 1000, 460, 411);
    gfx_line(410, 360, 100, 360);
    gfx_arc(50, 360, 100, 100, 90, 90);
    gfx_line(50, 411, 50, 1000);
    gfx_rectangle(60, 700, 30, 5);
    gfx_text(58, 670, "INSERT");
    gfx_text(61, 690, "COINS");
    gfx_rectangle(75, 0, 350, 100);
    gfx_line(192, 0, 192, 100);
    gfx_line(309, 0, 309, 100);
    gfx_text(240, 30, "SKEE");
    gfx_text(240, 60, "BALL");
    gfx_text(120, 20, "SCORE");
    gfx_rectangle(98, 30, 70, 50);
    gfx_text(345, 20, "HIGHSCORE");
    gfx_rectangle(330, 30, 70, 50);
    gfx_line(100, 500, 400, 500); //line which user can't throw beyond
    //makes holes and their point values
    gfx_arc(100, 190, 300, 160,  0, -180); // Creates the bottom arc, 10 points
    gfx_line(100, 270, 100, 100); // Closes the arc, making a ring
    gfx_line(400, 270, 400, 100); // Makes the ring on the other side
    gfx_ellipse(250, 240, 110, 80);  // Creates the biggest inner circle, 20 points
    gfx_ellipse(250, 250, 35, 25); // Creates the smallest center circle, 30 points
    gfx_ellipse(250, 190, 35, 25); // Create the circle above center, 40 points
    gfx_ellipse(250, 135, 30, 25); // Create the circle above center rings, 50 points
    gfx_ellipse(140, 135, 30, 25); // Creates the circle worth 100 points on left
    gfx_ellipse(360, 135, 30, 25); // Creates the circle worth 100 points on right
// Changes colors and adds the text
    gfx_color(17, 63, 189); //changes color to blue
    gfx_text(245, 320, "10");
    gfx_text(245, 290, "20");
    gfx_text(245, 240, "30");
    gfx_text(245, 175, "40");
    gfx_text(245, 120, "50");
    gfx_text(130, 120, "100");
    gfx_text(355, 120, "100");
}
float speedBall(int xi, int yi, float xf, float yf)
{
	float distance = 0.0, speed = 0.0;

	distance = sqrt(pow(xf - xi,2) + pow(yf - yi,2)); //distance formula

	if (distance <= 40)
		speed = 4.00;
	else if (distance > 40 && distance <= 80)
		speed = 6.00;
	else if (distance > 80 && distance <= 120)
		speed = 8.00;
	else if (distance > 120 && distance <= 160)
		speed = 10.00;
	else if (distance > 160)
		speed = 12.00;

	return speed;
}
float angleBall(int xi, int yi, float xf, float yf)
{
	float angle = 0.0;

	angle = atan2((yf - yi), (xf - xi)); //calculate angle using trig

	return angle;
}
void displayBalls(int balls, int ballRadius)
{
	int yposBalls[] = {755, 710, 665, 620, 575, 530, 485, 440, 395}; 

	gfx_color(0,0,0); //change color to black

	for (int i = 0; i < balls; i++)
	{	
		gfx_circle(430, yposBalls[i], ballRadius); //create 9 black circles at 9 different positions
		gfx_fill_circle(430, yposBalls[i], ballRadius);
	}
}
int rollBall(float ballSpeed, float ballAngle, float xf, float yf, int balls, int ballRadius)
{
	int addscore = 0;
	float xold = 0.0, yold = 0.0; 

	while ((xf - ballRadius >= 100 && xf + ballRadius <= 400) && (yf - ballRadius >= 360)) //ball animation on the red ramp
	{
		gfx_color(0,0,0); //draw a black ball moving up the ramp
		gfx_circle(xf, yf, ballRadius);
		gfx_fill_circle(xf, yf, ballRadius);
		gfx_flush();
		usleep(50000);

		xold = xf;
		yold = yf;

		xf = xf + ballSpeed * cos(ballAngle); // update the ball based on speed and angle
		yf = yf + ballSpeed * sin(ballAngle);

		gfx_color(201, 36, 36); //replace the old ball with a red ball so it blends into the background
		gfx_circle(xold, yold, ballRadius);
		gfx_fill_circle(xold, yold, ballRadius);
		gfx_color(0,0,0); //return back to black
	}
	//ball animation after the ramp
	while((xf - ballRadius >= 100 && xf + ballRadius <= 400) && addscore == 0)
	{
		gfx_color(0,0,0); //draw a black ball moving up the ramp
		gfx_circle(xf, yf, ballRadius);
		gfx_fill_circle(xf, yf, ballRadius);
		gfx_flush();
		usleep(50000);

		xold = xf;
		yold = yf;

		xf = xf + ballSpeed * cos(ballAngle); // update the ball based on speed and angle
		yf = yf + ballSpeed * sin(ballAngle);

		drawBoard(); //update the board
		displayBalls(balls, ballRadius);

		//point detection
		//ball rolls out of permeters will give 0 points
		if (yf + ballRadius >= 1000 || yf - ballRadius <= 0)
		{
			addscore = 0;
			break;
		}
		//lowest speed
		if (ballSpeed == 4 && yf - ballRadius <= 320) //Gives a maxiumum of 10 points
			addscore = 10;

		//second lowest speed
		if (ballSpeed == 6 && xf - ballRadius >= 140 && xf + ballRadius <= 360 && yf - ballRadius >= 160 && yf + ballRadius <= 330) //20 points if the ball stays within the ellipse
			addscore = 20;
		else if (ballSpeed == 6 && (xf - ballRadius < 140 || xf + ballRadius > 360) && yf - ballRadius >= 160 && yf + ballRadius <= 320) //10 points if ball is outside of ellipse
			addscore = 10;

		//medium speed
		if (ballSpeed == 8 && xf - ballRadius >= 215 && xf + ballRadius <= 285 && yf + ballRadius <= 275 && yf - ballRadius >= 225) //30 points inside 30 point ellipse
			addscore = 30;
		else if (ballSpeed == 8 && ((xf - ballRadius < 215 && xf - ballRadius >= 140) || (xf + ballRadius <= 360 && xf + ballRadius > 285)) && yf + ballRadius <= 280) //ball inside 20 pt ellipse but not 30 pt ellipse
			addscore = 20;
		else if (ballSpeed == 8 && (xf - ballRadius < 140 || xf + ballRadius > 360) && yf - ballRadius >= 160 && yf + ballRadius <= 280) //ball neither inside 30 pt or 20 pt ellipse
			addscore = 10;

		//second to fastest speed
		if (ballSpeed == 10 && xf - ballRadius >= 215 && xf + ballRadius <= 285 && yf + ballRadius <= 215 && yf - ballRadius >= 165) //40 points inside 40 point ellipse
			addscore = 40;
		else if (ballSpeed == 10 && ((xf - ballRadius < 215 && xf - ballRadius >= 140) || (xf + ballRadius <= 360 && xf + ballRadius > 285)) && yf + ballRadius <= 220) //ball inside 20 pt ellipse but not 40 pt ellipse
			addscore = 20;
		else if (ballSpeed == 10 && (xf - ballRadius < 140 || xf + ballRadius > 360) && yf - ballRadius >= 160 && yf + ballRadius <= 220) //ball neither inside 40 pt or 20 pt ellipse
			addscore = 10;

		//fastest speed, if it misses 50 or 100 points, then only 10 points are given
		if (ballSpeed == 12 && xf - ballRadius >= 110 && xf + ballRadius <= 170 && yf - ballRadius >= 100 && yf + ballRadius <= 160) //top left 100 pt ellipse
			addscore = 100;
		else if (ballSpeed == 12 && xf - ballRadius >= 330 && xf + ballRadius <= 390 && yf - ballRadius >= 100 && yf + ballRadius <= 160) //top right 100 pt ellipse
			addscore = 100;
		else if (ballSpeed == 12 && xf - ballRadius >= 220 && xf + ballRadius <= 280 && yf - ballRadius >= 100 && yf + ballRadius <= 160) //top 50pt ellipse
			addscore = 50;
		else if (ballSpeed == 12 && ((xf - ballRadius < 110) || (xf + ballRadius > 170 && xf - ballRadius < 220) || (xf + ballRadius > 280 && xf - ballRadius < 330) || (xf + ballRadius > 390)) && yf - ballRadius >= 100 && yf + ballRadius <= 160)
			addscore = 10; 

	}
	return addscore;
}
