#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : GraphObject
{
public:
	Actor(int id, int x, int y, int dir, int depth, StudentWorld* world);
	bool isDead() const;
	void die()  { m_alive = false; }
	virtual void activate(Actor* a);												// does nothing for non activateable objects

	virtual bool blocksMovement() { return false; }
	virtual bool blocksFlames() { return false; }
private:
	bool m_alive;

};



#endif // ACTOR_H_
