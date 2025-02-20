#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

int getKey();

class Actor : public GraphObject {
  public:
    Actor(int imageID, int startX, int startY, int dir = 0, double size = 1.0);
    virtual void doSomething() {}  // Default behavior (does nothing)
    bool isAlive() { return alive; }
    static int getKeyDir();
  
  private:
    bool alive = true;
};

class Floor : public Actor {
  public:
    Floor(int x, int y);
};

class Player : public Actor {
  public:
    Player(int x, int y);
    void doSomething() override;
};

// New actor types (stubs)
class LeftKong : public Actor {
  public:
    LeftKong(int x, int y);
    void doSomething() override;
};

class RightKong : public Actor {
  public:
    RightKong(int x, int y);
    void doSomething() override;
};

class Ladder : public Actor {
  public:
    Ladder(int x, int y);
    void doSomething() override;
};

class Bonfire : public Actor {
  public:
    Bonfire(int x, int y);
    void doSomething() override;
};

class Fireball : public Actor {
  public:
    Fireball(int x, int y);
    void doSomething() override;
};

class Koopa : public Actor {
  public:
    Koopa(int x, int y);
    void doSomething() override;
};

class ExtraLife : public Actor {
  public:
    ExtraLife(int x, int y);
    void doSomething() override;
};

class Garlic : public Actor {
  public:
    Garlic(int x, int y);
    void doSomething() override;
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
