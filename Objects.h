#ifndef Objects_h
#define Objects_h

#include <string>
#include "Temple.h"
#include "Actor.h"

class Objects // should be ABC
{
public:
    Objects(Temple* tp, int r, int c, std::string name);
    virtual ~Objects();
    
    
    // Getters
    int objectR() const;
    int objectC() const;
    std::string name();
    Temple* temple() const;
    
private:
    Temple* m_temple;
    
    int m_r;
    int m_c;
    std::string m_name;
};






class Weapons : public Objects
{
public:
    Weapons(Temple* tp, int r, int c, std::string name, int dexBonus, int damage);
    virtual ~Weapons();
    
    // Getters
    int dexBonus() const;
    int damage() const;
    std::string action() const;
    
    // Setters
    void setAction(std::string action);
    
    
private:
    int m_dexBonus;
    int m_damage;
    
    std::string m_action;
};





class Maces : public Weapons
{
public:
    Maces(Temple* tp, int r, int c);
    virtual ~Maces();
    
private:
    
};


class ShortSwords : public Weapons
{
public:
    ShortSwords(Temple* tp, int r, int c);
    virtual ~ShortSwords();
    
private:
    
};


class LongSwords : public Weapons
{
public:
    LongSwords(Temple* tp, int r, int c);
    virtual ~LongSwords();
    
private:
    
};


class MagicAxes : public Weapons
{
public:
    MagicAxes(Temple* tp, int r, int c);
    virtual ~MagicAxes();
    
private:
    
};

class MagicFangs : public Weapons
{
public:
    MagicFangs(Temple* tp, int r, int c);
    virtual ~MagicFangs();
    
private:
};







// SCROLLS
class Scrolls : public Objects
{
public:
    Scrolls(Temple* tp, int r, int c, std::string name);
    virtual ~Scrolls();
    
    virtual void read() = 0;
    virtual std::string readDescription() = 0;
    
private:
    
};



class scrollTeleportation : public Scrolls
{
public:
    scrollTeleportation(Temple* tp, int r, int c);
    virtual ~scrollTeleportation();
    
    void read();
    std::string readDescription();
    
private:
    
};

class scrollArmor : public Scrolls
{
public:
    scrollArmor(Temple* tp, int r, int c);
    virtual ~scrollArmor();
    
    void read();
    std::string readDescription();
    
private:
    
};

class scrollStr : public Scrolls
{
public:
    scrollStr(Temple* tp, int r, int c);
    virtual ~scrollStr();
    
    void read();
    std::string readDescription();
    
private:
    
};

class scrollHealth : public Scrolls
{
public:
    scrollHealth(Temple* tp, int r, int c);
    virtual ~scrollHealth();
    
    void read();
    std::string readDescription();
    
private:
    
};

class scrollDex : public Scrolls
{
public:
    scrollDex(Temple* tp, int r, int c);
    virtual ~scrollDex();
    
    void read();
    std::string readDescription();
    
private:
    
};

#endif /* Objects_h */
