#include "Actor.h"

#include "GameConstants.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include <cassert>
#include <cmath>
#include <utility>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and
// StudentWorld.cpp

int Actor::getKeyDir(int key) {
  switch (key) {
  case KEY_PRESS_RIGHT:
    return right;
  case KEY_PRESS_LEFT:
    return left;
  case KEY_PRESS_DOWN:
    return down;
  case KEY_PRESS_UP:
    return up;
  default:
    return -1;
  }
}

Actor::Actor(StudentWorld *world, int imageID, int startX, int startY, int dir)
    : GraphObject(imageID, startX, startY, dir), mAlive(true), mWorld(world) {}

bool Actor::moveIfValid(int x, int y) {
  return moveIf(x, y, [&]() { return getWorld()->validMoveLoc(x, y); });
}
bool Actor::fallIfValid() {
  return moveIf(getX(), getY() - 1, [&]() {
    return !getWorld()->existsClimbable(getX(), getY()) &&
           getWorld()->validFallLoc(getX(), getY() - 1);
  });
}
bool Actor::moveIfValidAndSolid(int x, int y) {
  if (!getWorld()->validFallLoc(x, y - 1)) {
    return moveIfValid(x, y);
  }
  return false;
}

// psuedo code:

//  If not Alive:
//     return
//  If frozen:
//     decrement frozen timer
//     return
//  If actor increments animation frame each tick:
//     increment animation frame
//  decrement timer
//  If timer has elapsed
//     reset timer
//  If player is at the actor's location:
//    doSomethingWithPlayer()
//  If the player did not abort the doSomething method
//    In order, with the suitable condition:
//     	doSomethingBefore
//     	doSomethingTimer // when the timer is zero
//     	doSomethingAfter
//     	doSomethingWithPlayer
void Actor::doSomething() {
  if (!isAlive()) {
    return;
  }
  if (isFrozen()) {
    decFrozen();
    return;
  }
  if (mIncFrame) {
    increaseAnimationNumber();
  }
  if (--mTimer < 0) {
    mTimer = resetTime();
  }
  bool cont = true;
  if (getPlayer()->isAlive() && getWorld()->actorAtPlayer(this)) {
    cont = doSomethingWithPlayer();
  }
  if (cont) {
    // Before and After is important to correctly handle
    // the order in the spec
    if (isAlive()) {
      doSomethingBefore();
    }
    if (isAlive() && mTimer == 0) {
      doSomethingTimer();
    }
    if (isAlive()) {
      doSomethingAfter();
    }
    if (isAlive() && getPlayer()->isAlive() &&
        getWorld()->actorAtPlayer(this)) {
      // try to do something with player if the object has moved its position
      doSomethingWithPlayer();
    }
  }
}
// only used for Kong
// but since there is no conceptual reason why another actor couldn't use this
// I'm putting it on the Actor class
int Actor::distanceBetween(const Actor *a2) const {
  auto x_diff = getX() - a2->getX();
  auto y_diff = getY() - a2->getY();
  return std::hypot(x_diff, y_diff);
}

Enemy::Enemy(StudentWorld *world, int imageID, int x, int y, bool dropGoodie,
             int dir)
    : Actor(world, imageID, x, y, dir), mDropGoodie(dropGoodie) {}

void Enemy::doAttacked() {
  kill();
  getWorld()->playSound(SOUND_ENEMY_DIE);
  getWorld()->increaseScore(100);
  if (mDropGoodie && randInt(1, 3) == 1) {
    Actor *goodie = newGoodie();
    assert(goodie != nullptr);
    getWorld()->addActorNextTick(goodie);
  }
}

void Actor::reverse() {
  int dir;
  switch (getDirection()) {
  case right:
    dir = left;
    break;
  case left:
    dir = right;
    break;
  case up:
    dir = down;
    break;
  case down:
    dir = up;
    break;
  default:
    assert(false);
  }
  setDirection(dir);
}

void Actor::moveInDirIfSolidElseReverse() {
  auto [x, y] = getDirPos();
  if (!moveIfValidAndSolid(x, y)) {
    reverse();
  }
}

// Abstracts away the, in my opinion, awkward int x, int y pattern
// for a clean get coords pattern
std::pair<int, int> Actor::getDirPos(int dir) const {
  int x = 0;
  int y = 0;
  getPositionInThisDirection(dir, 1, x, y);
  return std::pair(x, y);
}

std::pair<int, int> Actor::getDirPos() const {
  return getDirPos(getDirection());
}

// abstracts away the "if x, move and return whether it moved" pattern
template <typename F> bool Actor::moveIf(int x, int y, F f) {
  if (f()) {
    moveTo(x, y);
    return true;
  };
  return false;
}

// to make code less verbose
Player *Actor::getPlayer() { return getWorld()->getPlayer(); }

Floor::Floor(StudentWorld *world, int x, int y)
    : Actor(world, IID_FLOOR, x, y) {};
Player::Player(StudentWorld *world, int x, int y)
    : Actor(world, IID_PLAYER, x, y) {};

Barrel::Barrel(StudentWorld *world, int x, int y, int dir)
    : Enemy(world, IID_BARREL, x, y, false, dir) {};

void Barrel::doSomethingBefore() {
  if (moveIfValid(getX(), getY() - 1)) {
    if (getWorld()->existsSolid(getX(), getY() - 1)) {
      reverse();
    }
  }
}

void Barrel::doSomethingTimer() {
  auto [x, y] = getDirPos();
  if (!moveIfValid(x, y)) {
    reverse();
  }
}

void Player::moveDir(int dir) {
  auto [x, y] = getDirPos(dir);
  switch (dir) {
  case right:
  case left: {
    moveIfValid(x, y);
    break;
  }
  case up:
  case down: {
    bool canClimbIn = getWorld()->existsClimbable(getX(), getY());
    bool canClimbOn = getWorld()->existsClimbable(x, y);
    if (canClimbOn || canClimbIn) {
      moveIfValid(x, y);
    }
    break;
  }
  default:
    assert(false);
  }
}

bool Player::doSomethingWithPlayer() {
  if (jumpTick != 0) {
    return false;
  }
  fallIfValid();
  int key = -1;
  bool keyGot = getWorld()->getKey(key);
  if (!keyGot) {
    // nothing to do
    return false;
  }
  auto dir = getKeyDir(key);
  // move
  if (dir != -1) {
    if (dir == getDirection() || (getWorld()->existsClimbable(getX(), getY()) &&
                                  (dir == up || dir == down))) {
      moveDir(dir);

    } else if (dir == right || dir == left) {
      setDirection(dir);
    }
    // jump
  } else if (key == KEY_PRESS_SPACE && jumpTick == 0 &&
             getWorld()->existsStandable(getX(), getY() - 1)) {
    getWorld()->playSound(SOUND_JUMP);
    doJumpTick();
    // burp
  } else if (key == KEY_PRESS_TAB && burps > 0) {

    getWorld()->playSound(SOUND_BURP);
    auto [x, y] = getDirPos();
    Burp *burp = new Burp(getWorld(), x, y, getDirection());
    getWorld()->addActorNextTick(burp);
    --burps;
  }
  // doesn't matter
  return false;
}

void Player::doJumpTick() {
  int dir;
  if (jumpTick == 0) {
    oldDir = getDirection();
    dir = up;
  } else if (jumpTick >= 1 && jumpTick <= 3) {
    dir = oldDir;
  } else if (jumpTick == 4) {
    dir = down;
  } else {
    assert(false);
  }
  auto [x, y] = getDirPos(dir);
  bool validMove = moveIfValid(x, y);
  bool climbable = getWorld()->existsClimbable(x, y);
  if (!validMove || jumpTick == 4 || climbable) {
    oldDir = -1;
    jumpTick = 0;
  } else {
    incJumpTick();
  }
}

void Player::incJumpTick() {
  assert(jumpTick >= 0 && jumpTick <= 4);
  if (++jumpTick > 4)
    jumpTick = 0;
}

void Player::kill() {
  Actor::kill();
  getWorld()->decLives();
}

void Player::doAttacked() {
  getWorld()->playSound(SOUND_PLAYER_DIE);
  kill();
}

// What follows are standard implementations of the logic
// in the spec, following the psuedocode provided

Kong::Kong(StudentWorld *world, int x, int y, int dir)
    : Actor(world, IID_KONG, x, y, dir) {
  incFrameAlways();
}

int Kong::barrelTickDefault() {
  return std::max(200 - 50 * getWorld()->getLevel(), 50);
}

void Kong::doSomethingBefore() {
  if (distanceBetween(getPlayer()) <= 2.0) {
    state = KongState::Flee;
  }
  if (--barrelTick == 0) {
    if (state != KongState::Flee) {
      auto [x, y] = getDirPos();
      getWorld()->addActorNextTick(
          new Barrel(getWorld(), x, y, getDirection()));
    }
    barrelTick = barrelTickDefault();
  }
}

void Kong::doSomethingTimer() {
  if (state == KongState::Flee) {
    moveIfValid(getX(), getY() + 1);
    if (getY() >= VIEW_HEIGHT) {
      getWorld()->winLevel();
    }
  }
}

Ladder::Ladder(StudentWorld *world, int x, int y)
    : Actor(world, IID_LADDER, x, y) {}

Bonfire::Bonfire(StudentWorld *world, int x, int y)
    : Actor(world, IID_BONFIRE, x, y) {
  incFrameAlways();
}

void Bonfire::doSomethingAfter() { getWorld()->attackFlammable(this); }

Fireball::Fireball(StudentWorld *world, int x, int y)
    : Enemy(world, IID_FIREBALL, x, y, true) {
  setDirection(randInt(0, 1) ? left : right);
}

bool Enemy::doSomethingWithPlayer() {
  getPlayer()->doAttacked();
  return false;
}

void Fireball::moveToClimbingState(ClimbingState newState) {
  state = newState;
  // note: doesn't change the direction of the object!
  auto [x, y] = getDirPos(getDirFromState());
  moveTo(x, y);
}

bool Fireball::rightState(ClimbingState desired) const {
  return desired == state || randInt(1, 3) == 1;
}

int Fireball::getDirFromState() const {
  switch (state) {
  case ClimbingState::Down:
    return down;
  case ClimbingState::Up:
    return up;
  case ClimbingState::None:
    return getDirection();
  default:
    assert(false);
  }
}

void Fireball::doSomethingTimer() {
  bool canClimb = getWorld()->existsClimbable(getX(), getY());
  bool aboveSolid = getWorld()->existsSolid(getX(), getY() + 1);
  bool belowClimable = getWorld()->existsClimbable(getX(), getY() - 1);

  if (canClimb && !aboveSolid && rightState(ClimbingState::Up)) {
    moveToClimbingState(ClimbingState::Up);
  } else if (belowClimable && rightState(ClimbingState::Down)) {
    moveToClimbingState(ClimbingState::Down);
  } else {
    if ((aboveSolid && state == ClimbingState::Up) ||
        (!belowClimable && state == ClimbingState::Down)) {
      state = ClimbingState::None;
    }
    moveInDirIfSolidElseReverse();
  }
}

Koopa::Koopa(StudentWorld *world, int x, int y)
    : Enemy(world, IID_KOOPA, x, y, true) {}

bool Koopa::doSomethingWithPlayer() {
  if (mFreezeCooldown == 0) {
    getPlayer()->freeze();
    useFreeze();
    return false;
  }
  return true;
}

void Koopa::doSomethingBefore() {
  if (mFreezeCooldown > 0) {
    decFreezeTimer();
  }
}

Goodie::Goodie(StudentWorld *world, int imageID, int x, int y, int scoreBoost)
    : Actor(world, imageID, x, y), mScoreBoost(scoreBoost) {}

bool Goodie::doSomethingWithPlayer() {
  kill();
  getWorld()->increaseScore(mScoreBoost);
  getWorld()->playSound(SOUND_GOT_GOODIE);
  givePowerUp();
  return false;
}

ExtraLife::ExtraLife(StudentWorld *world, int x, int y)
    : Goodie(world, IID_EXTRA_LIFE_GOODIE, x, y, 50) {}

void ExtraLife::givePowerUp() { getWorld()->incLives(); }

Garlic::Garlic(StudentWorld *world, int x, int y)
    : Goodie(world, IID_GARLIC_GOODIE, x, y, 25) {}

void Garlic::givePowerUp() { getPlayer()->addBurps(5); }

// class Burp : public Actor {
//   public:
//     Burp(StudentWorld* world, int x, int y, int dir);
//     void doSomethingTimer() override;
//     void doSomethingUnless() override;
// };

Burp::Burp(StudentWorld *world, int x, int y, int dir)
    : Actor(world, IID_BURP, x, y, dir) {}

void Burp::doSomethingTimer() { kill(); }

void Burp::doSomethingAfter() { getWorld()->attackBlastable(this); }
