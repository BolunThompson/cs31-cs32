#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <optional>
#include <utility>

class StudentWorld;
class Player;
class Barrel;

class Actor : public GraphObject {
public:
  Actor(StudentWorld *world, int imageID, int startX, int startY, int dir = 0);
  virtual ~Actor() {};
  // generic do something method
  // see implementation for psuedocode
  virtual void doSomething();
  // returns whether to continue doing something
  virtual bool doSomethingWithPlayer() {
    return true;
  } // Default behavior (does nothing)
  virtual void doSomethingAfter() {}  // Default behavior (does nothing)
  virtual void doSomethingBefore() {} // Default behavior (does nothing)
  virtual void doSomethingTimer() {}  // Default behavior (does nothing)
  virtual void doAttacked() {};       // Default behavior (does nothing)
  static int getKeyDir(int key);
  inline void incFrameAlways() { mIncFrame = true; }
  inline virtual bool isStandable() const { return false; }
  inline virtual bool isClimbable() const { return false; }
  inline virtual bool isSolid() const { return false; }
  inline virtual bool isFlammable() const { return false; }
  inline virtual bool isBlastable() const { return false; }
  inline StudentWorld *getWorld() const { return mWorld; }
  inline bool isAlive() const { return mAlive; }
  inline virtual void kill() { mAlive = false; }
  Player *getPlayer();
  std::optional<Actor *> getFloor();
  bool moveIfValid(int x, int y);
  bool fallIfValid();
  bool moveIfValidAndSolid(int x, int y);
  void moveInDirIfSolidElseReverse();
  inline void freeze() { mFrozenTimer = 50; }
  inline bool isFrozen() const { return mFrozenTimer > 0; }
  inline void decFrozen() { --mFrozenTimer; }
  // number of ticks until the doSomethingTimer method is called
  // 0 means never is time
  inline virtual int resetTime() const { return 0; }
  void reverse();
  std::pair<int, int> getDirPos() const;
  std::pair<int, int> getDirPos(int dir) const;
  int distanceBetween(const Actor *a2) const;

private:
  bool mAlive = true;
  bool mIncFrame = false;
  int mFrozenTimer = 0;
  int mTimer = -1;
  StudentWorld *mWorld;
  template <typename F> bool moveIf(int x, int y, F f);
};

class Enemy : public Actor {
public:
  Enemy(StudentWorld *world, int imageID, int x, int y, bool dropGoodie,
        int dir = right);
  void
  doAttacked() override; // Default behavior is to behave like an attacked enemy
  bool doSomethingWithPlayer() override; // Default behavior is to kill player
  inline int resetTime() const override { return 10; }
  inline bool isBlastable() const override { return true; }
  // assumes that a subclass that sets dropGoodie to be true would override this
  // to return the correct goodie
  virtual Actor *newGoodie() { return nullptr; };

private:
  bool mDropGoodie;
};

class Goodie : public Actor {
public:
  Goodie(StudentWorld *world, int imageID, int x, int y, int scoreBoost);
  bool doSomethingWithPlayer() override;
  virtual void givePowerUp() = 0;

private:
  int mScoreBoost;
};

class Floor : public Actor {
public:
  Floor(StudentWorld *world, int x, int y);
  virtual inline ~Floor() {}
  inline bool isStandable() const override { return true; }
  bool isSolid() const override { return true; }
};

class Player : public Actor {
public:
  Player(StudentWorld *world, int x, int y);
  virtual inline ~Player() {}
  bool isFlammable() const override { return true; }
  bool doSomethingWithPlayer() override;
  void doAttacked() override;
  inline void addBurps(int b) { burps += b; }
  inline int getBurps() { return burps; }
  void kill() override;

private:
  int jumpTick = 0;
  int oldDir = 0;
  int burps = 0;
  void incJumpTick();
  void doJumpTick();
  void moveDir(int dir);
};

class Kong : public Actor {
public:
  Kong(StudentWorld *world, int x, int y, int dir);
  virtual inline ~Kong() {}
  void doSomethingBefore() override;
  void doSomethingTimer() override;
  int resetTime() const override { return 5; }

private:
  // not using boolean for extensibility
  enum class KongState { Flee, Normal };
  bool closeToPlayer();
  int barrelTickDefault();
  int barrelTick = barrelTickDefault();
  KongState state = KongState::Normal;
};

class Ladder : public Actor {
public:
  Ladder(StudentWorld *world, int x, int y);
  virtual inline ~Ladder() {}
  inline virtual bool isClimbable() const override { return true; }
  inline virtual bool isStandable() const override { return true; }
};

class Bonfire : public Actor {
public:
  Bonfire(StudentWorld *world, int x, int y);
  virtual inline ~Bonfire() {}
  void doSomethingAfter() override;
};

class ExtraLife : public Goodie {
public:
  ExtraLife(StudentWorld *world, int x, int y);
  virtual inline ~ExtraLife() {}
  void givePowerUp() override;
};

class Garlic : public Goodie {
public:
  Garlic(StudentWorld *world, int x, int y);
  virtual inline ~Garlic() {}
  void givePowerUp() override;
};

class Fireball : public Enemy {
public:
  Fireball(StudentWorld *world, int x, int y);
  virtual inline ~Fireball() {}
  void doSomethingTimer() override;
  inline Actor *newGoodie() override {
    return new Garlic(getWorld(), getX(), getY());
  };

private:
  enum class ClimbingState { Up, Down, None };
  ClimbingState state = ClimbingState::None;
  void moveToClimbingState(ClimbingState state);
  bool rightState(ClimbingState desired) const;
  int getDirFromState() const;
};

class Koopa : public Enemy {
public:
  Koopa(StudentWorld *world, int x, int y);
  virtual inline ~Koopa() {}
  bool doSomethingWithPlayer() override;
  void doSomethingBefore() override;
  inline void doSomethingTimer() override { moveInDirIfSolidElseReverse(); }
  inline Actor *newGoodie() override {
    return new ExtraLife(getWorld(), getX(), getY());
  };

private:
  int mFreezeCooldown = 0;
  inline void useFreeze() { mFreezeCooldown = 50; }
  inline void decFreezeTimer() { --mFreezeCooldown; }
};

class Barrel : public Enemy {
public:
  Barrel(StudentWorld *world, int x, int y, int dir);
  virtual inline ~Barrel() {}
  inline bool isFlammable() const override { return true; }
  void doSomethingBefore() override;
  void doSomethingTimer() override;
  inline bool isBlastable() const override { return true; }
};

class Burp : public Actor {
public:
  Burp(StudentWorld *world, int x, int y, int dir);
  virtual inline ~Burp() {}
  void doSomethingTimer() override;
  void doSomethingAfter() override;
  inline int resetTime() const override { return 5; }
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and
// StudentWorld.cpp

#endif // ACTOR_H_
