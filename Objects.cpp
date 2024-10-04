#include <string>
using namespace std;

#include "Objects.h"
#include "utilities.h"

Objects::Objects(Temple* tp, int r, int c, string name)
: m_temple(tp), m_r(r), m_c(c), m_name(name)
{}

Objects::~Objects() {}

int Objects::objectR() const
{
    return m_r;
}

int Objects::objectC() const
{
    return m_c;
}

string Objects::name()
{
    return m_name;
}

Temple* Objects::temple() const
{
    return m_temple;
}




Weapons::Weapons(Temple* tp, int r, int c, string name, int dexBonus, int damage)
: Objects(tp, r, c, name), m_dexBonus(dexBonus), m_damage(damage)
{}

Weapons::~Weapons() {}

int Weapons::dexBonus() const
{
    return m_dexBonus;
}

int Weapons::damage() const
{
    return m_damage;
}


string Weapons::action() const
{
    return m_action;
}


void Weapons::setAction(string action)
{
    m_action = action;
}





Maces::Maces(Temple* tp, int r, int c)
: Weapons(tp, r, c, "mace", 0, 2)
{setAction("swings");}

Maces::~Maces() {}


ShortSwords::ShortSwords(Temple* tp, int r, int c)
: Weapons(tp, r, c, "short sword", 0, 2)
{setAction("slashes");}

ShortSwords::~ShortSwords() {}


LongSwords::LongSwords(Temple* tp, int r, int c)
: Weapons(tp, r, c, "long sword", 2, 4)
{setAction("swings");}

LongSwords::~LongSwords() {}


MagicAxes::MagicAxes(Temple* tp, int r, int c)
: Weapons(tp, r, c, "magic axe", 5, 5)
{setAction("chops");}

MagicAxes::~MagicAxes() {}


MagicFangs::MagicFangs(Temple* tp, int r, int c)
: Weapons(tp, r, c, "magic fangs of sleep", 3, 2)
{setAction("strikes");}

MagicFangs::~MagicFangs() {}





Scrolls::Scrolls(Temple* tp, int r, int c, string name)
: Objects(tp, r, c, name)
{}

Scrolls::~Scrolls() {}





scrollTeleportation::scrollTeleportation(Temple* tp, int r, int c)
: Scrolls(tp, r, c, "scroll of teleportation")
{}

scrollTeleportation::~scrollTeleportation() {}

void scrollTeleportation::read()
{
    int pairChosen = randInt(temple()->getValidPair().size());
    pair point = temple()->getValidPair()[pairChosen];
    
    int r = point.first;
    int c = point.second;
    
    temple()->player()->setr(r);
    temple()->player()->setc(c);
    // player randomly moved to another place in the level not occupied by wall or monster
}

string scrollTeleportation::readDescription()
{
    return "You feel your body wrenched in space and time.";
}





scrollArmor::scrollArmor(Temple* tp, int r, int c)
: Scrolls(tp, r, c, "scroll of enhance armor")
{}

scrollArmor::~scrollArmor() {}

void scrollArmor::read()
{
    // armor points incr by randint 1-3
    int armorIncr = randInt(1, 3);
    temple()->player()->setArmor(temple()->player()->armor()+armorIncr);
}

string scrollArmor::readDescription()
{
    return "Your armor glows blue.";
}



scrollStr::scrollStr(Temple* tp, int r, int c)
: Scrolls(tp, r, c, "scroll of strength")
{}

scrollStr::~scrollStr() {}

void scrollStr::read()
{
    // strength incr by rand int 1-3
    int strIncr = randInt(1, 3);
    temple()->player()->setStr(temple()->player()->str()+strIncr);
}

string scrollStr::readDescription()
{
    return "Your muscles bulge.";
}



scrollHealth::scrollHealth(Temple* tp, int r, int c)
: Scrolls(tp, r, c, "scroll of enhance health")
{}

scrollHealth::~scrollHealth() {}

void scrollHealth::read()
{
    // max hit incr by rand int 3-8 (doesn't affect current hit)
    int healthIncr = randInt(3, 8);
    temple()->player()->setMaxHit(temple()->player()->getMaxHit()+healthIncr);
}

string scrollHealth::readDescription()
{
    return "You feel your heart beating stronger.";
}



scrollDex::scrollDex(Temple* tp, int r, int c)
: Scrolls(tp, r, c, "scroll of enhance dexterity")
{}

scrollDex::~scrollDex() {}

void scrollDex::read()
{
    // player dex incr by 1
    temple()->player()->setDex(temple()->player()->dex()+1);
}

string scrollDex::readDescription()
{
    return "You feel like less of a klutz.";
}
