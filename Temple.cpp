//
//  Temple.cpp
//  Project 3
//
//  Created by Ellen Chen on 5/13/24.
//

#include "globals.h"
#include "utilities.h"
#include "Temple.h"
#include "Actor.h"
#include "Objects.h"

#include <iostream>
using namespace std;


Temple::Temple()
: m_level(0), m_player(nullptr), nMonster(0)
{
    // TEMPLE CONSTRUCTOR
    
    for(int i = 0; i < MAXMONSTERS; i++)
    {
        m_monster[i] = nullptr;
    }
    
    generateTemple(); // makes Temple with rooms
    buildTempleCorridors();
    
    // store a template of temple
    for (int r = 0; r < nROWS; r++)
    {
        for (int c = 0; c < nCOLS; c++)
        {
            gridTemplate[r][c] = m_grid[r][c];
        }
    }
    
    // add player
    addPlayer();
    
    // add monsters
    int numMonstersToAdd = randInt(2, 5*(level()+1)+1);
    for (int i = 0; i < numMonstersToAdd; i++)
    {
        addMonster();
    }
    
    
    // add objects
    int numObjectsToAdd = randInt(2, 3);
    for (int n = 0; n < numObjectsToAdd; n++)
    {
        addObject();
    }
    
    
    // add stair
    addStair();
}


Temple::~Temple()
{
    // TEMPLE DESTRUCTOR
    delete m_player;
    
    for (int i = 0; i < nMonster; i++)
    {
        if (m_monster[i] != nullptr)
        {
            delete m_monster[i];
            m_monster[i] = nullptr;
        }
    }
    
    
    for (int i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i] != nullptr)
        {
            delete m_objects[i];
            m_objects[i] = nullptr;
        }
    }
    m_objects.clear();
    
}

bool Temple::isActorAt(int r, int c) const
{
    if (m_player != nullptr)
    {
        if(m_player->r() == r && m_player->c() == c)
            return true;
    }
    
    if (monsterAt(r, c) != nullptr)
        return true;
    
    return false;
}

bool Temple::hasWall(int r, int c) const
{
    return m_grid[r][c] == '#';
}

bool Temple::goldenIdolAt(int r, int c) const
{
    return (m_idol.r == r && m_idol.c == c);
}

bool Temple::stairAt(int r, int c) const
{
    return (m_stair.r == r && m_stair.c == c);
}



Player* Temple::player() const
{
    return m_player;
}


Monster* Temple::monster(int i) const
{
    return m_monster[i];
}


Objects* Temple::object(int i) const
{
    return m_objects[i];
}





Monster* Temple::monsterAt(int r, int c) const
{
    for(int i = 0; i < nMonster; i++)
    {
        if (m_monster[i] != nullptr)
        {
            if (m_monster[i]->r() == r && m_monster[i]->c() == c)
            {
                return m_monster[i];
            }
        }
    }
    
    return nullptr;
}





int Temple::nMonsters() const
{
    return nMonster;
}

int Temple::level() const
{
    return m_level;
}


int Temple::numObjects() const
{
    return m_objects.size();
}

vector<pair<int,int>> Temple::getValidPair() const
{
    return validPair;
}


int Temple::stairR() const
{
    return m_stair.r;
}

int Temple::stairC() const
{
    return m_stair.c;
}



int Temple::idolR() const
{
    return m_idol.r;
}

int Temple::idolC() const
{
    return m_idol.c;
}



void Temple::setMonster(int i, Monster* mp)
{
    m_monster[i] = mp;
}

void Temple::setNMonsters(int num)
{
    nMonster = num;
}


void Temple::dropObjects(Objects *op)
{
    m_objects.push_back(op);
    // nObjects++;
}

void Temple::eraseObject(Objects *op)
{
    for (auto i = m_objects.begin(); i != m_objects.end();)
    {
        Objects* curObject = *i;
        
        if(curObject == op)
            m_objects.erase(i);
        else
            i++;
    }
}



void Temple::addPlayer()
{
    // pick a random point in valid spot -> to generate actors and objects
    int pairChosen = randInt(validPair.size());
    pair point = validPair[pairChosen];
    
    int r = point.first;
    int c = point.second;
    
    m_player = new Player(this, r, c);
}

void Temple::addMonster()
{
    // pick a random point in valid spot -> to generate actors and objects
    int pairChosen = randInt(validPair.size());
    pair point = validPair[pairChosen];
    
    int r = point.first;
    int c = point.second;
    
    while (isActorAt(r, c))
    {
        pairChosen = randInt(validPair.size());
        pair newPoint = validPair[pairChosen];
        
        r = newPoint.first;
        c = newPoint.second;
    }
    
    switch (m_level) {
        case 4:
        case 3:
        {
            int monsterToAdd = randInt(1, 4); // 1 = snakewomen, 2 = goblins, 3 = bogeymen, 4 = dragons
            switch (monsterToAdd) {
                case 1:
                    m_monster[nMonster] = new Snakewomen(this, r, c);
                    nMonster++;
                    break;
                case 2:
                    m_monster[nMonster] = new Goblins(this, r, c);
                    nMonster++;
                    break;
                case 3:
                    m_monster[nMonster] = new Bogeymen(this, r, c);
                    nMonster++;
                    break;
                case 4:
                    m_monster[nMonster] = new Dragons(this, r, c);
                    nMonster++;
                    break;
                default:
                    break;
            }
        }
            break;
        case 2:
        {
            int monsterToAdd = randInt(1, 3); // 1 = snakewomen, 2 = goblins, 3 = bogeymen, 4 = dragons -> no dragons
            switch (monsterToAdd) {
                case 1:
                    m_monster[nMonster] = new Snakewomen(this, r, c);
                    nMonster++;
                    break;
                case 2:
                    m_monster[nMonster] = new Goblins(this, r, c);
                    nMonster++;
                    break;
                case 3:
                    m_monster[nMonster] = new Bogeymen(this, r, c);
                    nMonster++;
                    break;
                default:
                    break;
            }
        }
            break;
        case 1:
        case 0:
        {
            int monsterToAdd = randInt(1, 2); // 1 = snakewomen, 2 = goblins, 3 = bogeymen, 4 = dragons -> no bogeymen or dragons
            switch (monsterToAdd) {
                case 1:
                    m_monster[nMonster] = new Snakewomen(this, r, c);
                    nMonster++;
                    break;
                case 2:
                    m_monster[nMonster] = new Goblins(this, r, c);
                    nMonster++;
                    break;
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
}


void Temple::addObject()
{
    // pick a random point in valid spot -> to generate actors and objects
    int pairChosen = randInt(validPair.size());
    pair point = validPair[pairChosen];
    
    int r = point.first;
    int c = point.second;
    
    int objectToAdd = randInt(1, 7); // 1 = Maces, 2 = short swords, 3 = long swords, 4 = scroll of armor, 5 = scroll of str, 6 = scroll of health, 7 = scroll of dex
    switch (objectToAdd) {
        case 1:
            m_objects.push_back(new Maces(this, r, c));
            break;
        case 2:
            m_objects.push_back(new ShortSwords(this, r, c));
            break;
        case 3:
            m_objects.push_back(new LongSwords(this, r, c));
            break;
        case 4:
            m_objects.push_back(new scrollArmor(this, r, c));
            break;
        case 5:
            m_objects.push_back(new scrollStr(this, r, c));
            break;
        case 6:
            m_objects.push_back(new scrollHealth(this, r, c));
            break;
        case 7:
            m_objects.push_back(new scrollDex(this, r, c));
            break;
        default:
            break;
    }
}


void Temple::addStair()
{
    int pairChosen = randInt(validPair.size());
    pair point = validPair[pairChosen];
    
    int stairR = point.first;
    int stairC = point.second;
    
    m_stair = {stairR, stairC};
}

void Temple::addIdol()
{
    int pairChosen = randInt(validPair.size());
    pair point = validPair[pairChosen];
    
    int idolR = point.first;
    int idolC = point.second;
    
    m_idol = {idolR, idolC};
}


void Temple::nextLevel()
{
    // clear monsters and objects from previous level
    
    for(int i = 0; i < nMonster; i++)
    {
        if (m_monster[i] != nullptr)
        {
            delete m_monster[i];
            m_monster[i] = nullptr;
        }
    }
    
    setNMonsters(0);
    
    for (int i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i] != nullptr)
        {
            delete m_objects[i];
            m_objects[i] = nullptr;
        }
    }
    
    m_objects.clear();
    
    // also clear valid pairs and rooms
    roomsInTemple.clear();
    validPair.clear();
    
    
    
    // increment level
    m_level++;
    
    
    // generate new temple with rooms and corridors
    generateTemple();
    buildTempleCorridors();
    
    // store a template of the new temple
    for (int r = 0; r < nROWS; r++)
    {
        for (int c = 0; c < nCOLS; c++)
        {
            gridTemplate[r][c] = m_grid[r][c];
        }
    }
    
    // respawn player
    int pairChosen = randInt(validPair.size());
    pair point = validPair[pairChosen];
    
    int r = point.first;
    int c = point.second;
    
    player()->setr(r);
    player()->setc(c);
    
    
    
    // add monsters
    int numMonstersToAdd = randInt(2, 5*(level()+1)+1);
    for (int i = 0; i < numMonstersToAdd; i++)
    {
        addMonster();
    }
    
    
    // add objects
    int numObjectsToAdd = randInt(2, 3);
    for (int n = 0; n < numObjectsToAdd; n++)
    {
        addObject();
    }
    
    
    // add stair/golden idol
    if (m_level < 4)
    {
        addStair();
    }
    else
    {
        addIdol();
    }
}



void Temple::addMessage(string message)
{
    messages.push(message);
}



void Temple::display()
{
// Displaying room
    
    for (int r = 0; r < nROWS; r++)
    {
        for (int c = 0; c < nCOLS; c++)
        {
                m_grid[r][c] = gridTemplate[r][c];
        }
    }
    
    
    
    // Indicate player location
    if(m_player != nullptr)
    {
        char& gridChar = m_grid[m_player->r()][m_player->c()];
        if (gridChar == ' ')
            gridChar = '@';
    }
    
    // Indicate monster position
    for (int i = 0; i < nMonster; i++)
    {
        Monster* mp = m_monster[i];
        string monster = mp->description();
        
        char& gridChar = m_grid[mp->r()][mp->c()];
        if (gridChar == ' ')
        {
            if (monster == "the Snakewomen")
                gridChar = 'S';
            if (monster == "the Goblin")
                gridChar = 'G';
            if (monster == "the Bogeymen")
                gridChar = 'B';
            if (monster == "the Dragon")
                gridChar = 'D';
        }
    }
    
    
    
    // Indicate object location
    for (int i = 0; i < m_objects.size(); i++)
    {
        
        Objects* op = m_objects[i];
        
        if (op != nullptr) {
            // check if it's weapon or scroll
            Weapons* wp = dynamic_cast<Weapons*> (op); // convert object ptr to weapon ptr
            
            
            if (wp != nullptr) // is a weapon
            {
                
                char& gridChar = m_grid[op->objectR()][op->objectC()];
                if (gridChar == ' ')
                {
                    gridChar = '(';
                }
            }
            else // a scroll
            {
                char& gridChar = m_grid[op->objectR()][op->objectC()];
                if (gridChar == ' ')
                {
                    gridChar = '?';
                }
            }
        }
    }
    
    // Indicate stair/golden idol location
    if (m_level < 4)
    {
        char& gridChar = m_grid[m_stair.r][m_stair.c];
        if (gridChar == ' ')
        {
            gridChar = '>';
        }
    }
    else
    {
        char& gridChar = m_grid[m_idol.r][m_idol.c];
        if (gridChar == ' ')
        {
            gridChar = '&';
        }
    }
    
    
// PRINT DISPLAY
    clearScreen();
    
    // displaying board
    for (int r = 0; r < 18; r++)
    {
        for (int c = 0; c < 70; c++)
        {
                cout << m_grid[r][c];
        }
        cout << endl;
    }
    
    cout << "Level: " << m_level << ", Hit points: " << m_player->hit() << ", Armor: " << m_player->armor() << ", Strength: " << m_player->str() << ", Dexterity: " << m_player->dex() << "\n" << endl;
    
    
    // displaying messages
    size_t ogSize = messages.size();
    
    for (size_t i = 0; i < ogSize; i++)
    {
        cout << messages.front() << endl;
        messages.pop();
    }
    
    
    
}


bool Temple::inBounds(int r, int c) const
{
    return (r > 0 && r < nROWS-1 && c > 0 && c < nCOLS-1);
}


bool Temple::noOverlap(int startR, int startC, int height, int width)
{
    for (int r = startR-1; r <= startR + height; r++)
    {
        for (int c = startC-1; c <= startC + width; c++)
        {
            if (m_grid[r][c] != '#') // don't have to worry about actors/object yet, this is before other things are build
            {
                return false;
            }
        }
    }
    
    
    return true;
}


void Temple::generateTemple()
{
    for (int r = 0; r < nROWS; r++)
    {
        for (int c = 0; c < nCOLS; c++)
        {
            m_grid[r][c] = '#';
        }
    }
    
    int numRooms = randInt(4, 6);
    
    for (int i = 0; i < numRooms; i++)
    {
        drawRoom(20);
    }
}



void Temple::drawRoom(int timesRan)
{
    if (timesRan <= 0)
        return;
    
    int roomHeight = randInt(4, 7);
    int roomWidth = randInt(6, 18);
    
    int roomStartR = randInt(1, 16-roomHeight);
    int roomStartC = randInt(1, 68-roomWidth);
    
    // if drawable then draw, if not make new starting pt and height
    if (noOverlap(roomStartR, roomStartC, roomHeight, roomWidth))
    {
        for (int row = roomStartR; row < roomStartR+roomHeight; row++)
        {
            for (int col = roomStartC; col < roomStartC+roomWidth; col++)
            {
                m_grid[row][col] = ' ';
                validPair.push_back(make_pair(row, col));
            }
        }
        
        // store room (for later to build corridor)
        Room room = {roomStartR, roomStartC, roomHeight, roomWidth};
        roomsInTemple.push_back(room);
        
        return;
    }
    else
        drawRoom(timesRan--);
}


void Temple::buildTempleCorridors()
{
    // access each room (for each room, connect to the next room)
    for (int i = 0; i < roomsInTemple.size()-1; i++)
    {
        int r1 = roomsInTemple[i].startR + roomsInTemple[i].height/2;
        int c1 = roomsInTemple[i].startC + roomsInTemple[i].width/2;
        
        int r2 = roomsInTemple[i+1].startR + roomsInTemple[i+1].height/2;
        int c2 = roomsInTemple[i+1].startC + roomsInTemple[i+1].width/2;
        
        while (r1 < r2) m_grid[r1++][c1] = ' ';
        while (r1 > r2) m_grid[r1--][c1] = ' ';
        while (c1 < c2) m_grid[r1][c1++] = ' ';
        while (c1 > c2) m_grid[r1][c1--] = ' ';
    }
    
}
