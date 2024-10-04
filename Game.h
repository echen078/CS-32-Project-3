// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Actor.h"

class Temple;

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    
    void wonGame();
    void play();
    
private:
    Temple* m_temple;
    
    bool gameWon;
};

#endif // GAME_INCLUDED
