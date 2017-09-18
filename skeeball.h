/*
Programmer: Michael Wang and Justin Peek
Class: cse20311
Assignment: Final Project
Purpose: Creation of skeeball class that helps keep track of scores, number of balls, and points
*/

using namespace std;

class Skeeball{
  	public:
		Skeeball();
		Skeeball(int, int, int);
		~Skeeball();
		int getballs();
		int getscore();
		int gethighscore();
		void setballs(int);
		void setscore(int);
		void sethighscore(int);
		void useballs(); //subtract 1 ball
		void addscore(int); //add points to current score
		bool comparehighscore(int); //compares the final score with the highscore
	private:
		int balls;
		int score;
		int highscore;
};