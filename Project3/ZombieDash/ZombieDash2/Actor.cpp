#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//																//
//					ACTOR IMPLEMENTATION						//
//																//
Actor::Actor(int imageID, int startX, int startY, Direction startDir, int depth, StudentWorld* world)
	:GraphObject(imageID, startX, startY, startDir, depth)
{
	m_world = world;
	m_alive = true;
}



bool Actor::determineOverlap(double x, double y) const
{
	double distance = determineDistance(x, y);
	return (distance <= 10);
}

double Actor::determineDistance(double x, double y) const
{
	return sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
}

void Actor::remove(Actor* a)
{
	if (a->getAliveStatus())
	{
		a->m_alive = false;
		if (a->isSurvivor() && !a->isPlayer())
		{
			getWorld()->decNumCitizens();
			getWorld()->playSound(SOUND_CITIZEN_DIE);
			std::cerr << getWorld()->getNumCitizens();
		}
	}
}

void Actor::sound(int soundNum) const							// 0 = die, 1 = save
{	
	if (isPlayer())
	{
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	else if (isSurvivor())
	{
		getWorld()->playSound(SOUND_CITIZEN_DIE);
	}
	else if (isZombie())
		getWorld()->playSound(SOUND_ZOMBIE_DIE);
}

//																//																
//						WALL IMPLEMENTAION						//
//																//
Wall::Wall(int startX, int startY, StudentWorld* world)
	:Actor(IID_WALL, startX, startY, right, 0, world)
{

}

void Wall::doSomething()
{
	return;
}


//																//																
//					ACTIVATINGOBJECT IMPLEMENTAION				//
//																//
ActivatingObject::ActivatingObject(int id, int startX, int startY, int dir, int depth, StudentWorld* world)
	:Actor(id, startX, startY, dir, depth, world)
{

}

void ActivatingObject::doSomething()
{
	if (!getAliveStatus())
		return;
	getWorld()->removeAllRemovable(this);
}

void ActivatingObject::remove(Actor *a)
{
	if (a->isDamageable())
	{
		
		Actor::remove(a);
	}
}
//																//																
//						EXIT IMPLEMENTAION						//
//																//

Exit::Exit(int startX, int startY, StudentWorld* world)
	:ActivatingObject(IID_EXIT, startX, startY, right , 1, world)
{

}

void Exit::remove(Actor* a)
{
	if (a->isPlayer())
	{
		if (getWorld()->getNumCitizens() == 0)
		{
			getWorld()->setLevelComplete();
			getWorld()->playSound(SOUND_LEVEL_FINISHED);
		}
	}
	else if (a->isSurvivor())
	{
		ActivatingObject::remove(a);

	}
}



//																//																
//						PIT IMPLEMENTAION						//
//																//

Pit::Pit(int startX, int startY, StudentWorld* world)
	:ActivatingObject(IID_PIT, startX, startY, right, 0, world)
{

}


//																//																
//						FLAME IMPLEMENTAION						//
//																//

Flame::Flame(int startX, int startY, int dir, StudentWorld* world)
	:ActivatingObject(IID_FLAME, startX, startY, dir, 0,world)
{
	m_lifetime = 0;
}


void Flame::doSomething()
{	
	if (m_lifetime >= 2)
		setAliveStatus(false);
	m_lifetime++;
	ActivatingObject::doSomething();
}

//																//																
//						LANDMINE IMPLEMENTAION					//
//																//

LandMine::LandMine(int startX, int startY, StudentWorld* world)
	:ActivatingObject(IID_LANDMINE, startX, startY, right, 1, world)
{
	m_safety = 30;
	m_active = false;
}


void LandMine::doSomething()
{
	if (m_active)
		ActivatingObject::doSomething();
	if (m_safety == 0)
	{
		m_active = true;
		return;
	}
	else
	{
		m_safety--;
	}
	
}

void LandMine::remove(Actor *a)
{

	if (getAliveStatus())
	{
		if ((a->isSurvivor() || a->isZombie()))
		{
			explode();
			

		}
	}
}

void LandMine::explode()
{
	int x = getX() - SPRITE_WIDTH;
	int y = getY() - SPRITE_HEIGHT;
	for (int i = 0; i < 3; i++, x += SPRITE_WIDTH)
	{
		for (int j = 0; j < 3; j++, y += SPRITE_HEIGHT)
		{
			getWorld()->burn(x, y, up);
		}
		y = getY() - SPRITE_HEIGHT;
	}

	getWorld()->leaveHole(getX(), getY());

}

//																//																
//						GOODIE IMPLEMENTAION					//
//																//

Goodie::Goodie(int id, int startX, int startY, StudentWorld* world)
	:ActivatingObject(id, startX, startY, right, 1, world)
{

}

void Goodie::remove(Actor *a)
{
	if (a->isPlayer())
	{
		pickUp(this);
	}
}

void Goodie::pickUp(Goodie* g)
{
	Actor::remove(this);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	getWorld()->giveGoodie(g);
}

//																//																
//						GAS CAN GOODIE IMPLEMENTAION			//
//																//

GasCanGoodie::GasCanGoodie(int startX, int startY, StudentWorld* world)
	:Goodie(IID_GAS_CAN_GOODIE, startX, startY, world)
{

}

void GasCanGoodie::giveGoodie(Penelope *p)
{
	p->incFlameThrowerCount();
}


//																//																
//						LANDMINE GOODIE IMPLEMENTAION			//
//																//

LandMineGoodie::LandMineGoodie(int startX, int startY, StudentWorld* world)
	:Goodie(IID_LANDMINE_GOODIE, startX, startY, world)
{

}

void LandMineGoodie::giveGoodie(Penelope *p)
{
	p->incLandMinesCount();
}

//																//																
//						VACCINE GOODIE IMPLEMENTAION			//
//																//

VaccineGoodie::VaccineGoodie(int startX, int startY, StudentWorld* world)
	:Goodie(IID_VACCINE_GOODIE, startX, startY, world)
{

}

void VaccineGoodie::giveGoodie(Penelope *p)
{
	p->incVaccineCount();
}



//																//																
//					CHARACTER IMPLEMENTAION						//
//																//

Character::Character(int id, int startX, int startY, StudentWorld* world)
	:Actor(id, startX, startY, right, 0, world)
{
	m_callNum = 0;
}

bool Character::paralyze()
{
	m_callNum++;
	if (m_callNum % 2 == 1)
		return false;
	return true;
}

void Character::move(int pix)
{
	double dest_x = getX();
	double dest_y = getY();

	switch (getDirection())
	{
	case right:
		dest_x += pix;
		break;
	case up:
		dest_y += pix;
		break;
	case left:
		dest_x -= pix;
		break;
	case down:
		dest_y -= pix;
		break;
	}
	if (getWorld()->canMoveInto(this, dest_x, dest_y))
		moveTo(dest_x, dest_y);
}



//																//																
//					ZOMBIE IMPLEMENTAION						//
//																//

Zombie::Zombie(int startX, int startY, StudentWorld* world)
	:Character(IID_ZOMBIE, startX, startY, world)
{

}

//																//																
//				SMART ZOMBIE IMPLEMENTAION						//
//																//

SmartZombie::SmartZombie(int startX, int startY, StudentWorld* world)
	:Zombie(startX,startY, world)
{

}

void SmartZombie::doSomething()
{

}

//																//																
//				DUMB ZOMBIE IMPLEMENTAION						//
//																//

DumbZombie::DumbZombie(int startX, int startY, StudentWorld* world)
	:Zombie(startX, startY, world)
{

}

void DumbZombie::doSomething()
{

}

//																//																
//					SURVIVOR IMPLEMENTAION						//
//																//

Survivor::Survivor(int id, int startX, int startY, StudentWorld* world)
	:Character(id, startX, startY, world)
{
	m_infectionCount = 0;
	m_infected = false;
}



bool Survivor::checkInfection()
{
	if (m_infected)
	{
		m_infectionCount++;
		if (m_infectionCount >= 500)
		{
			setAliveStatus(false);
			getWorld()->playSound(SOUND_ZOMBIE_BORN);
			return true;
		}
	}
	return false;
}

//																//																
//					CITIZEN IMPLEMENTAION						//
//																//

Citizen::Citizen(int startX, int startY, StudentWorld* world)
	:Survivor(IID_CITIZEN, startX, startY, world)
{

}

void Citizen::doSomething()
{
	if (checkInfection())
		return;
	if (paralyze())
		return;
	double dist_p = determineDistance(getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY());


	if (dist_p < 80)
	{
		
		if (getWorld()->getPlayer()->getY() > getY())
		{
			setDirection(up);
			
		}
		else if (getWorld()->getPlayer()->getY() < getY())
		{
			setDirection(down);
			
		}
		else if (getWorld()->getPlayer()->getX() > getX())
		{
			setDirection(right);

		}
		else if (getWorld()->getPlayer()->getX() < getX())
		{
			setDirection(left);

		}
		move(2);
	}
}

//																//																
//					PENELOPE IMPLEMENTAION						//
//																//
Penelope::Penelope(int startX, int startY, StudentWorld* world)
	:Survivor(IID_PLAYER, startX, startY, world)
{
	m_flameThrowerCount = 0;
	m_landMinesCount = 999;
	m_vaccineCount = 0;

}

void Penelope::doSomething()
{
	int ch;
	if (checkInfection())
		return;

	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
			
		case KEY_PRESS_LEFT:
			setDirection(left);
			move(4);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			move(4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			move(4);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			move(4);
			break;
		case KEY_PRESS_SPACE:
			if (m_flameThrowerCount > 0)
			{
				int x = getX(), y = getY();
				m_flameThrowerCount--;
				for (int i = 0; i < 3; i++)
				{
					switch (getDirection())
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
					if (!getWorld()->burn(x, y, getDirection()))
					{
						break;
					}
					//getWorld()->burn(x, y, getDirection());
				}
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				
			}
			break;
		case KEY_PRESS_TAB:
			if (m_landMinesCount > 0)
			{
				m_landMinesCount--;
				getWorld()->placeLandMine(getX(), getY());
			}
			break;
		}
		
	}
}