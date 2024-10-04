// Game.cpp

#include "Game.h"
#include "utilities.h"
#include "Temple.h"
#include "Actor.h"

#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
{
    // set game status
    gameWon = false;
    
    // create new temple
    m_temple = new Temple();
    
    // set goblin smell distance
    for (int i = 0; i < m_temple->nMonsters(); i++)
    {
        if (m_temple->monster(i)->description() == "the Goblins")
        {
            m_temple->monster(i)->setSmellDistance(goblinSmellDistance);
        }
        
    }
}

Game::~Game()
{
    delete m_temple;
    m_temple = nullptr;
}

void Game::wonGame()
{
    m_temple->addMessage("You pick up the golden idol");
    m_temple->addMessage("Congratulations, you won!");
    
    gameWon = true;
}

void Game::play()
{
    m_temple->display();
    
    char input = ' ';
    input = getCharacter();
    
    while (input != 'q')
    {
        Player* player = m_temple->player();
        
        
        if (player->hit() < player->getMaxHit())
        {
            if(trueWithProbability(0.10))
            {
                player->setHit(player->hit()+1);
            }
        }
        
        if (player->sleep() == 0) // if not asleep, can have actions
        {
            switch (input) {
                case ARROW_UP:
                case ARROW_DOWN:
                case ARROW_LEFT:
                case ARROW_RIGHT:
                    player->move(input);
                    break;
                case 'g':
                    if (m_temple->goldenIdolAt(player->r(), player->c()))
                    {
                        wonGame();
                        break;
                    }
                    else
                        player->pickupObject();
                    break;
                case 'i':
                    player->displayInventory();
                    getCharacter();
                    break;
                case 'w':
                    player->displayInventory();
                    player->wield();
                    break;
                case 'r':
                    player->displayInventory();
                    player->readScroll();
                    break;
                case 'c':
                    if (player->sleep() == 0)
                    {
                        player->setStr(9);
                        player->setHit(50);
                    }
                    break;
                case '>':
                    if (m_temple->level() < 4)
                    {
                        if (m_temple->stairAt(player->r(), player->c()))
                        {
                            m_temple->nextLevel();
                        }
                    }
                    break;
            }
            
        }
        else
        {
            player->setSleep(player->sleep() - 1);
        }
        
            // MONSTERS MOVE

            for(int i = 0; i < m_temple->nMonsters(); i++)
            {
                Monster* monster = m_temple->monster(i);
                
                if(monster->canSmell())
                {
                    if(monster->sleep() == 0) { // if not asleep
                        if (monster->distanceFromPlayer() <= 1)
                            monster->attack(m_temple->player());
                        else
                            monster->move(monster->moveDir());
                    }
                    else
                        monster->setSleep(monster->sleep() - 1);
                }
                
            }
        
        m_temple->display();
        
        if (gameWon || player->hit() <= 0)
        {
            cout << "Press q to exit game." << endl;
            
            while (input != 'q')
            { input = getCharacter(); }
            return;
        }
        
        input = getCharacter();
        
    }
    

    
}

// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons, 
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
