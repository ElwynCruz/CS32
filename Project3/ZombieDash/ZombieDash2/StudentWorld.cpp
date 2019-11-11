#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	m_Penelope = nullptr;
	m_numCitizens = 0;
	m_levelComplete = false;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	m_levelComplete = false;
	Level cur(assetPath());
	ostringstream oss;

	oss << "level";
	oss.fill('0');
	oss	<< setw(2) << getLevel() << ".txt";
	// getLevel()
	
	string levelFile = oss.str();
	Level::LoadResult loadResult = cur.loadLevel("level03.txt");
	if (loadResult == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (loadResult == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (loadResult == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int i = 0; i < LEVEL_WIDTH; i++)
		{
			for (int j = 0; j < LEVEL_HEIGHT; j++)
			{
				Actor *tempActor;
				Level::MazeEntry ge = cur.getContentsOf(i, j);
				switch (ge)
				{
				
				case Level::empty:
					break;
				case Level::smart_zombie:
					tempActor = new SmartZombie((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::dumb_zombie:
					tempActor = new DumbZombie((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::citizen:
					tempActor = new Citizen((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_numCitizens++;
					m_Actors.push_back(tempActor);
					break;
				case Level::player:
					m_Penelope = new Penelope((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					break;
				case Level::exit:
					tempActor = new Exit((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::wall:
					tempActor = new Wall((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::pit:
					tempActor = new Pit((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::gas_can_goodie:
					tempActor = new GasCanGoodie((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::landmine_goodie:
					tempActor = new LandMineGoodie((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				case Level::vaccine_goodie:
					tempActor = new VaccineGoodie((SPRITE_WIDTH*i), (SPRITE_HEIGHT*j), this);
					m_Actors.push_back(tempActor);
					break;
				}
			}
		}
	}
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	// Give each actor a chance to do something, including Penelope
	
	m_Penelope->doSomething();

	
	//for (it = m_Actors.begin(); it != m_Actors.end(); it++)
	for (int it = 0; it < m_Actors.size(); it++)
	{
		
		if (m_Actors[it]->getAliveStatus())
		{
			m_Actors[it]->doSomething();
			if (!m_Penelope->getAliveStatus())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (m_levelComplete)
			{
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
		
	}
	vector<Actor*>::iterator it;
	for (it = m_Actors.begin(); it != m_Actors.end();)
	{
		if (!(*it)->getAliveStatus())
		{
			Actor* temp = *it;
			it = m_Actors.erase(it);
			delete temp;
			
		}
		else
		{
			it++;
		}
	}

    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	int numRemaining = m_Actors.size();
	for (int i = 0; i < numRemaining; i++)
	{
		delete m_Actors[i];
	}

	m_Actors.clear();
	delete m_Penelope;
	m_numCitizens = 0;
}

bool StudentWorld::canMoveInto(Actor* a, double x, double y) const										
{

	if (a != m_Penelope)
	{
		if (!isOpen(m_Penelope, x, y))
			return false;
	}

	for (int i = 0; i < m_Actors.size(); i++)
	{
		if (a == m_Actors[i])
			continue;
		if (!isOpen(m_Actors[i], x, y))
			return false;
	}
	return true;

}

bool StudentWorld::isOpen(Actor* a, double x, double y) const
{
	double curX, curY, curBoundX, curBoundY;

	double newBoundX = x + SPRITE_WIDTH - 1;
	double newBoundY = y + SPRITE_HEIGHT - 1;

	if (a->blocksMovement())
	{
		curX = a->getX();
		curY = a->getY();
		curBoundX = curX + SPRITE_WIDTH - 1;
		curBoundY = curY + SPRITE_HEIGHT - 1;
		if (curX <= newBoundX && curX >= x)
		{
			if ((curY <= newBoundY && curY >= y) || (curBoundY <= newBoundY && curBoundY >= y))
				return false;
		}

		if (curBoundX <= newBoundX && curBoundX >= x)
		{
			if ((curY <= newBoundY && curY >= y) || (curBoundY <= newBoundY && curBoundY >= y))
				return false;
		}
	}
	return true;
}


void StudentWorld::removeAllRemovable(Actor *a)
{
	if (a != m_Penelope && a->determineOverlap(m_Penelope->getX(), m_Penelope->getY()))
		a->remove(m_Penelope);
	for (int i = 0; i < m_Actors.size(); i++)
	{

		if (a->determineOverlap(m_Actors[i]->getX(), m_Actors[i]->getY()))
			a->remove(m_Actors[i]);
	}
}



bool StudentWorld::burn(int startX, int startY, int dir)						// takes in penelope's X and Y, then tries to make 3 flames in her direction + SPRITE_HEIGHT/WIDTH
{

	for (int i = 0; i < m_Actors.size(); i++)
	{
		if (m_Actors[i]->blocksWeapons() && m_Actors[i]->determineOverlap(startX, startY))
			return false;
	}

	m_Actors.push_back(new Flame(startX, startY, dir, this));
	return true;

}

void StudentWorld::leaveHole(int startX, int startY)													// take the startingX and startingY coordinates of the landMine
{																									
	m_Actors.push_back(new Pit(startX, startY, this));													//make pit from landMine coordinates
																		
	
}

void StudentWorld::placeLandMine(int startX, int startY)
{
	Actor* temp = new LandMine(startX, startY, this);
	m_Actors.push_back(temp);
}


void StudentWorld::giveGoodie(Goodie* g)
{
	 g->giveGoodie(m_Penelope); 
}


void StudentWorld::attackCoordinates(int &x, int &y, int dir)
{
	switch (dir)
	{
	case GraphObject::right:
		x += SPRITE_WIDTH;
		break;
	case GraphObject::left:
		x -= SPRITE_WIDTH;
		break;
	case GraphObject::up:
		y += SPRITE_HEIGHT;
		break;
	case GraphObject::down:
		y -= SPRITE_HEIGHT;
		break;
	}
}