#include <iostream>
#include <string>
using namespace std;

#include "globals.h"
#include "Actor.h"
#include "utilities.h"
#include "Temple.h"
#include "Objects.h"

//////////////////////////
// ACTOR CLASS
//////////////////////////

// constructor
Actor::Actor(Temple* tp, int r, int c, int hit, int armor, int str, int dex, int sleep, Weapons* weapon)
: m_temple(tp), m_r(r), m_c(c), m_hit(hit), m_armor(armor), m_str(str), m_dex(dex), m_sleep(sleep), m_weapon(weapon)
{}

// destructor
Actor::~Actor() {}


// getters
int Actor::r() const { return m_r; }
int Actor::c() const { return m_c; }
int Actor::hit() const { return m_hit; }
int Actor::armor() const { return m_armor; }
int Actor::str() const { return m_str; }
int Actor::dex() const { return m_dex; }
int Actor::sleep() const { return m_sleep; }
Temple* Actor::temple() const { return m_temple; }
Weapons* Actor::weapon() const { return m_weapon; }


// setters
void Actor::setr(int r) { if (r > 0 && r < 18) m_r = r; }
void Actor::setc(int c) { if (c > 0 && c < 70) m_c = c; }


void Actor::setHit(int hit)
{
    if (hit <= 0)
        m_hit = 0;
    else if (hit <= 99)
        m_hit = hit;
    else
        m_hit = 99;
}

void Actor::setArmor(int armor)
{
    if (armor <= 0)
        m_armor = 0;
    else if (armor <= 99)
        m_armor = armor;
    else
        m_armor = 99;
}

void Actor::setStr(int str)
{
    if (str <= 0)
        m_str = 0;
    else if (str <= 99)
        m_str = str;
    else
        m_str = 99;
}

void Actor::setDex(int dex)
{
    if (dex <= 0)
        m_dex = 0;
    else if (dex <= 99)
        m_dex = dex;
    else
        m_dex = 99;
}

void Actor::setSleep(int sleep)
{
    if (sleep <= 0)
        m_sleep = 0;
    else if (sleep <= 9)
        m_sleep = sleep;
    else
        m_sleep = 9;
}

void Actor::setWeapon(Weapons* weapon) { m_weapon = weapon; }




void Actor::attack(Actor* defender)
{
    int attackerPoints = this->dex() + this->weapon()->dexBonus();
    int defenderPoints = defender->dex() + defender->armor();
    int damagePoints = randInt(0, this->str() + this->weapon()->damage() - 1);
    
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) 
    {
        defender->setHit((defender->hit()) - damagePoints);

        // add message to queue (to be added to display)
        if (defender->hit() <= 0)
        {
            if (this->weapon()->name() == "magic fangs of sleep")
            {
                temple()->addMessage(this->description() + " " + this->weapon()->action() + " magic fangs at " + defender->description() + " dealing a final blow.");
            }
            else
            {
                temple()->addMessage(this->description() + " " + this->weapon()->action() + " " + this->weapon()->name() + " at " + defender->description() + " dealing a final blow.");
            }
            
            // if the defender is a monster and it died, then delete
            Monster* mp = dynamic_cast<Monster*>(defender);
            
            if (mp != nullptr) // if is a monster
            {
                // check where in the list the monster is
                for (int i = 0; i < temple()->nMonsters(); i++)
                {
                    if (mp == temple()->monster(i))
                    {
                        // drop weapon
                        temple()->monster(i)->dropWeapon();
                        
                        
                        // remove monster
                        delete temple()->monster(i);
                        temple()->setMonster(i, nullptr);
                        temple()->setNMonsters(temple()->nMonsters()-1);
                    }
                }
                
                
                // re-adjust array
                for (int i = 0; i < temple()->nMonsters(); i++)
                {
                    if (temple()->monster(i) == nullptr)
                    {
                        temple()->setMonster(i, temple()->monster(i+1));
                        temple()->setMonster(i+1, nullptr);
                    }
                }
            }
            
            return ;
        }
        
        if (this->weapon()->name() == "magic fangs of sleep")
        {
            if (trueWithProbability(0.20))
            {
                int sleepTime = max(defender->m_sleep, randInt(2, 6)); // takes the max of defender's current and new sleep time
                defender->setSleep(sleepTime);
                
                temple()->addMessage(this->description() + " " + this->weapon()->action() + " magic fangs at " + defender->description() + " and hits, putting " + defender->description() + " to sleep.");
                
                return;
            }
            else
            {
                temple()->addMessage(this->description() + " " + this->weapon()->action() + " magic fangs at " + defender->description() + " and hits.");
                return;
            }
            
        }
        
        temple()->addMessage(this->description() + " " + this->weapon()->action() + " " + this->weapon()->name() + " at " + defender->description() + " and hits.");
    }
    else
    {
        temple()->addMessage(this->description() + " " + this->weapon()->action() + " " + this->weapon()->name() + " at " + defender->description() + " and misses.");
    }
}



//////////////////////////
// PLAYER CLASS - derived of ACTOR
//////////////////////////

Player::Player(Temple* tp, int r, int c)
: Actor(tp, r, c, 20, 2, 2, 2, 0, new ShortSwords(tp, 0, 0)), m_maxHit(20)
{
    inventory.push_back(weapon());
    
    if (tp == nullptr)
    {
        cout << "***** The player must be created in some Temple!" << endl;
        exit(1);
    }
}

Player::~Player() 
{
    for (int i = 0; i < inventory.size(); i++)
    {
        delete inventory[i];
        inventory[i] = nullptr;
    }
    inventory.clear();
}


string Player::description() { return "Player"; }
int Player::getMaxHit() const { return m_maxHit; }
void Player::setMaxHit(int maxHit) { m_maxHit = maxHit; }


void Player::move(char dir)
{ // doesn't move if new position has another actor or is a wall (but attacks if new position has another actor)
    int r = this->r();
    int c = this->c();
    
    switch (dir)
    {
        case ARROW_UP:
            if (r-1 > 0)
                r -= 1;
            else // out of bound, won't move
                return;
            break;
        case ARROW_DOWN:
            if (r+1 < nROWS-1)
                r += 1;
            else
                return;
            break;
        case ARROW_LEFT:
            if (c-1 > 0)
                c -= 1;
            else
                return;
            break;
        case ARROW_RIGHT:
            if (c+1 < nCOLS-1)
                c += 1;
            else
                return;
            break;
    }
    
    if(temple()->isActorAt(r, c))
    {
        (temple()->player())->attack(temple()->monsterAt(r, c));  // attacks player at new location if has another actor
    }
    else // no actor at no location
    {
        if (!temple()->hasWall(r, c))
        {
            setr(r);
            setc(c);
        }
    }
}


void Player::pickupObject()
{
    if (inventory.size() <= 25)
    {
        int r = this->r();
        int c = this->c();
        
        for (int i = 0; i < temple()->numObjects(); i++)
        {
            Objects* op = temple()->object(i);
            if (op->objectR() == r && op->objectC() == c)
            {
                // add to inventory
                inventory.push_back(op);
                
                Scrolls* sp = dynamic_cast<Scrolls*>(op);
                
                if (sp != nullptr) // if object is scroll
                {
                    temple()->addMessage("You pick up a scroll called " + op->name());
                }
                else // if object is weapon
                {
                    temple()->addMessage("You pick up " + op->name());
                }
                
                // delete from map
                temple()->eraseObject(op);
                // delete op; CANT DELETE HERE DO IT AT THE END OF GAME (?)
            }
        }
    }
    else
    {
        temple()->addMessage("Your knapsack is full; you can't pick that up.");
    }
}


void Player::displayInventory()
{
    clearScreen();
    
    
    cout << "Inventory:" << endl;
    char line = 'a';
    
    for (int i = 0; i < inventory.size(); i++)
    {
        Objects* op = inventory[i];
        Scrolls* sp = dynamic_cast<Scrolls*>(op);
        if (sp != nullptr)   // it is a scroll: A scroll called _____
        {
            cout << line << ". A scroll called " << op->name() << endl;
        }
        else // weapons
        {
            cout << line << ". " << op->name() << endl;
        }
        line++;
    }
}


void Player::readScroll()
{
    char scrollToRead = getCharacter();
    int inventoryIndex = scrollToRead-49-'0';
    
    
    if(inventoryIndex <= inventory.size()-1)
    {
        Objects* readThis = inventory[inventoryIndex];
        
        Scrolls* scroll = dynamic_cast<Scrolls*>(readThis);
        
        if (scroll != nullptr) // can be read
        {
            scroll->read();
            temple()->addMessage("You read the scroll called " + scroll->name());
            temple()->addMessage(scroll->readDescription());
            
            // erase item from inventory
            eraseFromInventory(readThis);
            
        }
        else
            temple()->addMessage("You can't read a " + readThis->name());
    }
}

void Player::eraseFromInventory(Objects *op)
{
    // erase item from inventory
    for (auto i = inventory.begin(); i != inventory.end();)
    {
        Objects* curObject = *i;
        
        if(curObject == op)
            inventory.erase(i);
        else
            i++;
    }
}


void Player::wield()
{
    char weaponToWield = getCharacter();
    int inventoryIndex = weaponToWield-49-'0';
    
    if(inventoryIndex <= inventory.size()-1)
    {
        Objects* wieldThis = inventory[inventoryIndex];
        
        Weapons* weapon = dynamic_cast<Weapons*>(wieldThis);
        
        if (weapon != nullptr) // a weapon, can be wield
        {
            // switch weapon
            temple()->player()->setWeapon(weapon);
            
            // display message
            temple()->addMessage("You are wielding " + weapon->name());
            
        }
        else
            temple()->addMessage("You can't wield " + wieldThis->name());
    }
    
}





// MONSTER CLASS - derived of ACTOR
Monster::Monster(Temple* tp, int r, int c, int hit, int armor, int str, int dex, int sleep, Weapons* weapon, int smellDistance)
: Actor(tp, r, c, hit, armor, str, dex, sleep, weapon), m_smellDistance(smellDistance) {}

Monster::~Monster() { delete weapon(); }

int Monster::smellDistance() const
{
    return m_smellDistance;
}


void Monster::setSmellDistance(int smellDistance) { m_smellDistance = smellDistance; }

int Monster::distanceFromPlayer()
{
    int playerR = temple()->player()->r();
    int playerC = temple()->player()->c();
    int monsterR = this->r();
    int monsterC = this->c();
    
    int rDist = abs(playerR - monsterR);
    int cDist = abs(playerC - monsterC);
    int totDist = rDist + cDist;
    
    return totDist;
}

bool Monster::canSmell()
{
    return distanceFromPlayer() <= m_smellDistance;
}


char Monster::moveDir() const
{
    char cDir;
    char rDir;
    char dir = ' ';
    bool blocked = true;
    
    if ((temple()->player())->c() > this->c())
        cDir = ARROW_RIGHT;
    else if ((temple()->player())->c() < this->c())
        cDir = ARROW_LEFT;
    else // if on the same col
        cDir = ' ';
    
    if ((temple()->player())->r() > this->r())
        rDir = ARROW_DOWN;
    else if ((temple()->player())->r() < this->r())
        rDir = ARROW_UP;
    else // if on the same row
        rDir = ' ';
     
    
    switch (cDir) {
        case ARROW_RIGHT:
            if(!(temple()->isActorAt(this->r(),this->c()+1)) && !temple()->hasWall(r(), c()+1))
            {
                blocked = false;
                dir = ARROW_RIGHT;
            }
            break;
        case ARROW_LEFT:
            if(!(temple()->isActorAt(this->r(),this->c()-1)) && !temple()->hasWall(r(), c()-1))
            {
                blocked = false;
                dir = ARROW_LEFT;
            }
            break;
        default:
            break;
    }
    
    if (blocked){
        switch (rDir) {
            case ARROW_DOWN:
                if (!(temple()->isActorAt(this->r()+1,this->c())) && !temple()->hasWall(r()+1, c()))
                {
                    blocked = false;
                    dir = ARROW_DOWN;
                }
                break;
            case ARROW_UP:
                if (!(temple()->isActorAt(this->r()-1,this->c())) && !temple()->hasWall(r()-1, c()))
                {
                    blocked = false;
                    dir = ARROW_UP;
                }
                break;
            default:
                break;
        }
    }
    
    
    return dir;
}



void Monster::move(char move)
{   // move direction is checked (no actor + no wall) before passed in as parameter
    switch (move)
    {
        case ARROW_UP:
            if (r()-1 > 0) {
                setr(r()-1);
            }
            break;
        case ARROW_DOWN:
            if (r()+1 < nROWS-1) {
                setr(r()+1);
            }
            break;
        case ARROW_LEFT:
            if (c()-1 > 0) {
                setc(c()-1);
            }
            break;
        case ARROW_RIGHT:
            if (c()+1 < nCOLS-1) {
                setc(c()+1);
            }
            break;
        default:
            break;
    }
}





// Derived classes of MONSTER
Bogeymen::Bogeymen(Temple* tp, int r, int c)
: Monster(tp, r, c, randInt(5, 10), 2, randInt(2, 3), randInt(2, 3), 0, new ShortSwords(tp, 0, 0), 5)
{
    if (tp == nullptr)
    {
        cout << "***** The monster must be created in some Temple!" << endl;
        exit(1);
    }
}

Bogeymen::~Bogeymen() {}

string Bogeymen::description()
{
    return "the Bogeymen";
}

void Bogeymen::dropWeapon()
{
    // CAN BE CONDENSED
    
    int r = this->r();
    int c = this->c();
    
    // check if other object at the location
    bool hasObject = false;
    for (int i = 0; i < temple()->numObjects(); i++)
    {
        if (temple()->object(i)->objectR() == r && temple()->object(i)->objectC() == c)
        {
            hasObject = true;
        }
    }
    
    // weapons can't be drop on stairs or golden idol
    if (temple()->goldenIdolAt(r, c) || temple()->stairAt(r, c))
    {
        hasObject = true;
    }
    
    
    if (!hasObject)
    {
        if(trueWithProbability(.10))
        {
            temple()->dropObjects(new MagicAxes(temple(), r, c));
        }
    }
}




Snakewomen::Snakewomen(Temple* tp, int r, int c)
: Monster(tp, r, c, randInt(3, 6), 3, 2, 3, 0, new MagicFangs(tp, 0, 0), 3)
{
    if (tp == nullptr)
    {
        cout << "***** The monster must be created in some Temple!" << endl;
        exit(1);
    }
}

Snakewomen::~Snakewomen() {}

string Snakewomen::description()
{
    return "the Snakewomen";
}

void Snakewomen::dropWeapon()
{
    int r = this->r();
    int c = this->c();
    
    // check if other object at the location
    bool hasObject = false;
    for (int i = 0; i < temple()->numObjects(); i++)
    {
        if (temple()->object(i)->objectR() == r && temple()->object(i)->objectC() == c)
        {
            hasObject = true;
        }
    }
    
    // weapons can't be drop on stairs or golden idol
    if (temple()->goldenIdolAt(r, c) || temple()->stairAt(r, c))
    {
        hasObject = true;
    }
    
    
    if (!hasObject)
    {
        if(trueWithProbability(0.333))
        {
            temple()->dropObjects(new MagicFangs(temple(), r, c));
        }
    }
}



Dragons::Dragons(Temple* tp, int r, int c)
: Monster(tp, r, c, randInt(20, 25), 4, 4, 4, 0, new LongSwords(tp, 0, 0), 1)
{
    if (tp == nullptr)
    {
        cout << "***** The monster must be created in some Temple!" << endl;
        exit(1);
    }
}

Dragons::~Dragons() {}

string Dragons::description()
{
    return "the Dragon";
}


void Dragons::dropWeapon()
{
    int r = this->r();
    int c = this->c();
    
    // check if other object at the location
    bool hasObject = false;
    for (int i = 0; i < temple()->numObjects(); i++)
    {
        if (temple()->object(i)->objectR() == r && temple()->object(i)->objectC() == c)
        {
            hasObject = true;
        }
    }
    
    // weapons can't be drop on stairs or golden idol
    if (temple()->goldenIdolAt(r, c) || temple()->stairAt(r, c))
    {
        hasObject = true;
    }
    
    
    if (!hasObject)
    {
        // 100% drop scroll of some kind
        int scrollToAdd = randInt(1, 4);
        switch (scrollToAdd) {
            case 1:
                temple()->dropObjects(new scrollArmor(temple(), r, c));
                break;
            case 2:
                temple()->dropObjects(new scrollStr(temple(), r, c));
                break;
            case 3:
                temple()->dropObjects(new scrollHealth(temple(), r, c));
                break;
            case 4:
                temple()->dropObjects(new scrollDex(temple(), r, c));
                break;
            default:
                break;
        }
    }
}



Goblins::Goblins(Temple* tp, int r, int c)
: Monster(tp, r, c, randInt(15, 20), 1, 3, 1, 0, new ShortSwords(tp, 0, 0), 15)
{
    if (tp == nullptr)
    {
        cout << "***** The monster must be created in some Temple!" << endl;
        exit(1);
    }
}

Goblins::~Goblins() {}

string Goblins::description()
{
    return "the Goblin";
}

void Goblins::dropWeapon()
{
    int r = this->r();
    int c = this->c();
    
    // check if other object at the location
    bool hasObject = false;
    for (int i = 0; i < temple()->numObjects(); i++)
    {
        if (temple()->object(i)->objectR() == r && temple()->object(i)->objectC() == c)
        {
            hasObject = true;
        }
    }
    
    // weapons can't be drop on stairs or golden idol
    if (temple()->goldenIdolAt(r, c) || temple()->stairAt(r, c))
    {
        hasObject = true;
    }
    
    
    if (!hasObject)
    {
        if(trueWithProbability(0.333))
        {
            int weaponToDrop = randInt(1, 2);
            switch (weaponToDrop) {
                case 1:
                    // m_objects.push_back(new scrollArmor(this, r, c));
                    temple()->dropObjects(new MagicAxes(temple(), r, c));
                    break;
                case 2:
                    // m_objects.push_back(new scrollStr(this, r, c));
                    // nObjects++;
                    temple()->dropObjects(new MagicFangs(temple(), r, c));
                    break;
                default:
                    break;
            }
        }
        
    }
}



bool Goblins::canMove(vector<pair<int, int>> visitedPath, int r, int c)
{
    bool visited = false;
    //loop thru visitedPath, if x y is in their, set visited = true;
    for (int i = 0; i < visitedPath.size(); i++)
    {
        pair coord = visitedPath[i];
        int visitedR = coord.first;
        int visitedC = coord.second;
        
        if (visitedR == r && visitedC == c)
        {
            visited = true;
        }
    }
    
    return (r > 0 && r < nROWS - 1 &&
            c > 0 && c < nCOLS -1 &&
            !visited && !(temple()->hasWall(r, c)) &&
            (temple()->monsterAt(r, c) == nullptr));
}


void Goblins::findShortestPath(int monsterR, int monsterC, int targetR, int targetC, int &min_dist, int dist, vector<pair<int,int>> &visitedPath, vector<pair<int, int>> &shortestPath)
{
    if (dist > smellDistance() || dist > min_dist)
    {
        return;
    }
    
    // add current coordinate to visitedPath
    visitedPath.push_back(make_pair(monsterR, monsterC));
    
    // base case
    
    if (targetR == monsterR && targetC == monsterC)
    {
        if (dist < min_dist)
        {
            shortestPath.clear();
            for (int i = 0; i < visitedPath.size(); i++)
            {
                shortestPath.push_back(visitedPath[i]);
            }
            min_dist = dist;
        }
        
        visitedPath.pop_back();
        return;
    }

    
    // check all four directions
    if(canMove(visitedPath, monsterR+1, monsterC)) {
        findShortestPath(monsterR+1, monsterC, targetR, targetC, min_dist, dist + 1, visitedPath, shortestPath);
    }
    if(canMove(visitedPath, monsterR-1, monsterC)) {
        findShortestPath(monsterR-1, monsterC, targetR, targetC, min_dist, dist + 1, visitedPath, shortestPath);
    }
    if(canMove(visitedPath, monsterR, monsterC+1)) {
        findShortestPath(monsterR, monsterC+1, targetR, targetC, min_dist, dist + 1, visitedPath, shortestPath);
    }
    if(canMove(visitedPath, monsterR, monsterC-1)) {
        findShortestPath(monsterR, monsterC-1, targetR, targetC, min_dist, dist + 1, visitedPath, shortestPath);
    }
    
    
    // backtrack: remove current coordinate from visitedPath
    visitedPath.pop_back();
    
    
}


// Wrapper function
bool Goblins::canSmell()
{
    int playerR = temple()->player()->r();
    int playerC = temple()->player()->c();
    
    vector<pair<int, int>> visitedPath;
    
    int min_dist = smellDistance() + 1;
    findShortestPath(this->r(), this->c(), playerR, playerC, min_dist, 0, visitedPath, shortestPath);
    
    if (min_dist <= smellDistance())
            return true;
    return false;
}


char Goblins::moveDir() const
{
    return ' ';
}


void Goblins::move(char move) // only called if can smell (meaning there will always be valid coordinates inside shortestPath when called)
{
    // shortestPath: first coord is monster location, last coord is player location
    // so first move is shortestPath[1];
    int moveR = shortestPath[1].first;
    int moveC = shortestPath[1].second;
    
    this->setr(moveR);
    this->setc(moveC);
}
