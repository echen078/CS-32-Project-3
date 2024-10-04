#ifndef Temple_h
#define Temple_h

#include "Actor.h"
#include "globals.h"

#include <queue>
#include <vector>

class Objects;

class Temple
{
public:
    Temple();
    ~Temple();
    

    bool isActorAt(int r, int c) const;
    bool hasWall(int r, int c) const;
    bool goldenIdolAt(int r, int c) const;
    bool stairAt(int r, int c) const;
    
// getters
    Player* player() const; // access player in Temple
    Monster* monster(int i) const; // access monster at index i
    Monster* monsterAt(int r, int c) const; //helper function: returns monster at r, c (easier to call than to loop thru monster(int i)
    int nMonsters() const;
    Objects* object(int i) const; // access object at index i
    int numObjects() const;
    int level() const;
    std::vector<std::pair<int,int>> getValidPair() const;
    
    int stairR() const;
    int stairC() const;
    
    int idolR() const;
    int idolC() const;
    
// setters
    void setMonster(int i, Monster* mp); // set monster[i] to the monster pointer mp
    void setNMonsters(int num);
    void dropObjects(Objects* op); // helper function for dropWeapon: pushes back op to m_objects, adds to nObjects
    void eraseObject(Objects* op); // helper function for pickupObject: erases op from temple and auto-adjusts vector
    
    
    void addPlayer();
    void addMonster();
    void addObject();
    void addStair();
    void nextLevel();
    void addIdol();
    
    void addMessage(std::string message);
    
    void display();
    
    
private:
    char m_grid[nROWS][nCOLS];
    char gridTemplate[nROWS][nCOLS];
    int m_level;  // specifies level
    
    Player* m_player;
    
    Monster* m_monster[MAXMONSTERS];
    int nMonster;
    
    std::vector<Objects*> m_objects;
    
    std::queue<std::string> messages;
    
    
    // Helper function

    bool inBounds(int r, int c) const;
    
    void generateTemple();
    void drawRoom(int timesRan);
    void buildTempleCorridors();
    
    struct Room
    {
        int startR;
        int startC;
        int height;
        int width;
    };

    
    bool noOverlap(int startR, int startC, int height, int width);
    
    
    std::vector<Room> roomsInTemple;
    std::vector<std::pair<int,int>> validPair;
    
    struct Stair
    {
        int r;
        int c;
    };
    
    Stair m_stair;
    
    
    struct Idol
    {
        int r;
        int c;
    };
    
    Idol m_idol;
};

#endif /* Temple_h */
