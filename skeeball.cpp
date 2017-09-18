/*
Programmer: Michael Wang and Justin Peek
Class: cse20311
Assignment: Final Project
Purpose: Class implementation of the Skeeball class
*/

#include "skeeball.h"

Skeeball::Skeeball()
{
	balls = 0;
	score = 0;
	highscore = 0;
}
Skeeball::Skeeball(int newballs, int newscore, int newhighscore)
{ balls = newballs, score = newscore, highscore = newhighscore; }

Skeeball::~Skeeball() { }

int Skeeball::getballs()
{ return balls; }

int Skeeball::getscore()
{ return score; }

int Skeeball::gethighscore()
{ return highscore; }

void Skeeball::setballs(int newballs)
{ balls = newballs; }

void Skeeball::setscore(int newscore)
{ score = newscore; }

void Skeeball::sethighscore(int newhighscore)
{ highscore = newhighscore; }

void Skeeball::useballs() //used for when the user releases their mouse
{
	balls = balls - 1;
}
void Skeeball::addscore(int newscore) //used for adding points to current score when the ball rolls into a hole
{
	score = score + newscore;
}
bool Skeeball::comparehighscore(int newhighscore) //compares current score with high score
{
	return (newhighscore > highscore);
}
