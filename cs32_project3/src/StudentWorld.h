#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

// #include "Actor.h"
#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  inline virtual ~StudentWorld() { cleanUp(); };
  // singleton, so I'm not going to write a copy constructor/assignment
private:
  std::vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
