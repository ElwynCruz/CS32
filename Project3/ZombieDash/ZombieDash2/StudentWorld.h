#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;
class Goodie;
class Level;


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	
	bool canMoveInto(Actor* a, double x, double y) const;		// takes in an actor and returns if that actor can move into space x, y
	//bool isOverlapping(Actor* curActor) const;		// takes an x and y coordinate and determines if location is occupied by another actor

	
	void removeAllRemovable(Actor* a);							// removes all the actors that a can remove
	
	
	int getNumCitizens() const { return m_numCitizens; }
	void decNumCitizens() { m_numCitizens--; }
	void setLevelComplete() { m_levelComplete = true; }

	bool burn(int startX, int startY, int dir);
	void leaveHole(int startX, int startY);
	void placeLandMine(int startX, int startY);
	Penelope* getPlayer() const { return m_Penelope; }
	void giveGoodie(Goodie* g); 


private:
	Penelope* m_Penelope;
	std::vector<Actor*> m_Actors;
	int m_numCitizens;
	bool m_levelComplete;

	bool isOpen(Actor* a, double x, double y) const;


	void attackCoordinates(int &x, int &y, int dir);					// private helper function for burn and puke : sets x and y to the location where we will create flames/vomit
};

#endif // STUDENTWORLD_H_
