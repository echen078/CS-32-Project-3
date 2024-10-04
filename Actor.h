#ifndef Actor_h
#define Actor_h

#include <string>
#include <vector>

#include "globals.h"

class Objects;
class Weapons;
class Temple;

////////////////////////////////////////////
// ACTOR BASE CLASS - ABC
////////////////////////////////////////////

class Actor
{
public:
    Actor(Temple* tp, int r, int c, int hit, int armor, int str, int dex, int sleep, Weapons* weapon);
    virtual ~Actor();
    
    // Getters
    int r() const;
    int c() const;
    int hit() const;
    int armor() const;
    int str() const;
    int dex() const;
    int sleep() const;
    Temple* temple() const;
    Weapons* weapon() const;
    
    // Setters
    void setr(int r);
    void setc(int c);
    void setHit(int hit);
    void setArmor(int armor);
    void setStr(int str);
    void setDex(int dex);
    void setSleep(int sleep);
    void setWeapon(Weapons* weapon);
    
    // actor actions
    void attack(Actor* actor);
    virtual std::string description() = 0;
    virtual void move(char move) = 0;
    
private:
    Temple* m_temple;
    
    int m_r;
    int m_c;
    
    int m_hit; // health
    int m_armor; // armor
    int m_str; // strength
    int m_dex; // dexterity
    int m_sleep; // sleep time
    
    Weapons* m_weapon;
    
};




/////////////////////////
// PLAYER CLASS
/////////////////////////

class Player : public Actor
{
public:
    Player(Temple* tp, int r, int c);
    virtual ~Player();
    
    virtual std::string description();
    int getMaxHit() const;
    void setMaxHit(int maxHit);
    
    // actions (by reading user input)
    void move(char move);
    void pickupObject();
    void displayInventory();
    void readScroll();
    void eraseFromInventory(Objects *op);
    void wield();
    
private:
    int m_maxHit;
    std::vector<Objects*> inventory;
};




//////////////////////////////////////
// MONSTER CLASS - ABC
//////////////////////////////////////

class Monster : public Actor
{
public:
    Monster(Temple* tp, int r, int c, int hit, int armor, int str, int dex, int sleep, Weapons* weapon, int smellDistance);
    virtual ~Monster();
    
    int smellDistance() const;
    
    void setSmellDistance(int smellDistance);
    int distanceFromPlayer();
    virtual bool canSmell();
    virtual char moveDir() const;
    void move(char move);
    
    virtual void dropWeapon() = 0;
    
private:
    int m_smellDistance;
};


///////////////////////////////////////////////////
// DERIVED CLASSES of monster
///////////////////////////////////////////////////

class Bogeymen : public Monster
{
public:
    Bogeymen(Temple* tp, int r, int c);
    virtual ~Bogeymen();

    virtual std::string description();
    virtual void dropWeapon();
    
private:
    
};

class Snakewomen : public Monster
{
public:
    Snakewomen(Temple* tp, int r, int c);
    virtual ~Snakewomen();
    
    virtual std::string description();
    virtual void dropWeapon();
};



class Dragons : public Monster
{
public:
    Dragons(Temple* tp, int r, int c);
    virtual ~Dragons();
    
    virtual std::string description();
    virtual void dropWeapon();
};



class Goblins : public Monster
{
public:
    Goblins(Temple* tp, int r, int c);
    virtual ~Goblins();
    
    virtual std::string description();
    virtual void dropWeapon();
    
    virtual bool canSmell();
    virtual char moveDir() const;
    virtual void move(char move);
private:
    // helper
    std::vector<std::pair<int, int>> shortestPath;
    
    bool canMove(std::vector<std::pair<int, int>> visitedPath, int r, int c);
    void findShortestPath(int monsterR, int monsterC, int targetR, int targetC, int &min_dist, int dist, std::vector<std::pair<int,int>> &visitedPath, std::vector<std::pair<int, int>> &shortestPath);
};


#endif
