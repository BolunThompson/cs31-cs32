#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "Level.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
// TODO: Remove asserts when I turn this in

class StudentWorld : public GameWorld {
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  inline virtual ~StudentWorld() { cleanUp(); };
  inline void addActorNextTick(Actor *actor) { actorsToAdd.push_back(actor); }

  void winLevel();
  void addNextActors();
  // singleton, so I'm not going to write a copy constructor/assignment

  // includes player
  // could be on the Actor class but its consistent with the other
  // actorExists methods
  bool actorAtPlayer(const Actor *actor) const;

  bool validMoveLoc(int x, int y) const;
  bool validFallLoc(int x, int y) const;
  bool existsStandable(int x, int y) const;
  bool existsClimbable(int x, int y) const;
  bool existsSolid(int x, int y) const;

  void attackFlammable(Bonfire *fire) const;
  void attackBlastable(Burp *fire) const;
  inline Player *getPlayer() const { return mPlayer; }

private:
  // NOTE: Includes player!
  std::vector<Actor *> actors;
  std::vector<Actor *> actorsToAdd;
  Player *mPlayer;
  bool levelWon = false;
  void generateActors(const Level &level);
  std::string getStatusLine() const;

  template <typename F> auto getAnActor(int x, int y, F func) const;

  template <typename F> bool actorExists(int x, int y, F func) const;

  template <typename F> bool actorExists(const Actor *actor, F func) const;

  inline void addActor(Actor *actor) { actors.push_back(actor); }

  static inline bool rightLoc(int x, int y, const Actor *v) {
    return ((v->getX() == x) && (v->getY() == y));
  }
  static inline bool actorsTogether(const Actor *a1, const Actor *a2) {
    return rightLoc(a1->getX(), a1->getY(), a2);
  }

  template <typename F> void attackPred(const Actor *source, F func) const;
};

#endif // STUDENTWORLD_H_
