#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction startDir, int depth, StudentWorld* world);
	virtual void doSomething() = 0;	
	StudentWorld* getWorld() const { return m_world; }

	void setAliveStatus(bool status) { m_alive = status; }

	bool getAliveStatus() const	{ return m_alive; }

	virtual bool isSurvivor() const { return false; }
	virtual bool isZombie() const { return false; }									// functions to determine type of actor
	virtual bool isPlayer() const { return false; }

	virtual bool determineOverlap(double x, double y) const;							// determines if this actor overlaps with another actor
//	virtual double determineDistance(Actor* a) const;
	double determineDistance(double x, double y) const;
	

	virtual bool isDamageable() const { return false; }								// actors can be damaged unless otherwise noted		   (only characters and goodies can be damaged and landmines)
	virtual bool blocksMovement() const { return false; }							// actors do not block movement unless otherwise noted (only walls and characters)
	virtual bool blocksWeapons() const { return false; }							// actors except walls and exits dont block deadly weapons (flames/vomit)

	virtual void remove(Actor* a);
	void sound(int soundNum) const;									// when an actor dies, use this to play their dying sound

private:
	StudentWorld* m_world;
	bool m_alive;
};

class ActivatingObject : public Actor
{
public:
	ActivatingObject(int id, int startX, int StartY, int dir,int depth, StudentWorld* world);
	virtual void doSomething();
	virtual void remove(Actor* a);
	

private:

};


class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	bool blocksMovement() const { return true; }
	bool blocksWeapons() const { return true; }
};

class Exit : public ActivatingObject
{
public:
	Exit(int startX, int startY, StudentWorld* world);


	void remove(Actor* a);
	bool blocksWeapons() const { return true; }
};

class Pit : public ActivatingObject
{
public:
	Pit(int startX, int startY, StudentWorld* world);

};

class Flame : public ActivatingObject
{
public:
	Flame(int startX, int startY, int dir, StudentWorld* world);
	void doSomething();

private:
	int m_lifetime;
};


class LandMine : public ActivatingObject
{
public:
	LandMine(int startX, int startY, StudentWorld* world);
	void remove(Actor* a);
	void doSomething();
	bool isDamageable() const { return true; }
	void explode();

private:
	int m_safety;
	bool m_active;
};



class Character : public Actor
{
public:
	Character(int id, int startX, int startY, StudentWorld* world);
	bool isDamageable() const { return true; }
	virtual void doSomething() = 0;
	virtual bool blocksMovement() const { return true; }
	virtual void move(int pix);
	virtual bool paralyze();
private:
	int m_callNum;

};

class Zombie : public Character
{
public:
	Zombie(int startX, int startY, StudentWorld* world);
	virtual void doSomething() = 0;
	virtual bool isZombie() const { return true; }
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
};

class Survivor : public Character
{
public:
	Survivor(int id, int startX, int startY, StudentWorld* world);
	virtual void doSomething() = 0;
	virtual bool isSurvivor() const { return true; }

	bool checkInfection();										// checks if the survivor is infected, if so, do infected things
	
	bool isInfected() const { return m_infected; }
	int getInfectionCount() const { return m_infectionCount; }

private:
	bool m_infected;
	int m_infectionCount;
};

class Citizen : public Survivor
{
public:
	Citizen(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
private:
	
};


class Penelope : public Survivor
{
public:
	Penelope(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	virtual bool isPlayer() const { return true; }

	void incFlameThrowerCount() { m_flameThrowerCount += 5; }
	void incLandMinesCount() { m_landMinesCount += 2;}
	void incVaccineCount() { m_vaccineCount++; }

private:
	int m_flameThrowerCount;
	int m_landMinesCount;
	int m_vaccineCount;
	
	

	
};


class Goodie : public ActivatingObject
{
public:
	Goodie(int id, int startX, int startY, StudentWorld* world);
	bool isDamageable() const { return true; }
	virtual void giveGoodie(Penelope *p) = 0;
	void remove(Actor *a);
	void pickUp(Goodie* g);
};

class GasCanGoodie : public Goodie
{
public:
	GasCanGoodie(int startX, int startY, StudentWorld* world);
	void giveGoodie(Penelope *p);

};

class LandMineGoodie : public Goodie
{
public:
	LandMineGoodie(int startX, int startY, StudentWorld* world);
	void giveGoodie(Penelope *p);
};

class VaccineGoodie : public Goodie
{
public:
	VaccineGoodie(int startX, int startY, StudentWorld* world);
	void giveGoodie(Penelope *p);
};

#endif // ACTOR_H_