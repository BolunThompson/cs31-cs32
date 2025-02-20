#include "Actor.h"

#include "GraphObject.h"
#include "GameController.h"
#include "GameConstants.h"
#include <cassert>
#include <pthread.h>

constexpr int KEY_INVALID = -1;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and
// StudentWorld.cpp

int getKey() {
    int key;
    bool res = Game().getKeyIfAny(key);
    return res ? key : -1;
}

int Actor::getKeyDir() {
    switch (getKey()) {
        case KEY_PRESS_RIGHT:
            return right;
        case KEY_PRESS_LEFT:
            return left;
        case KEY_PRESS_DOWN:
            return down;
        case KEY_PRESS_UP:
            return up;
        case KEY_INVALID:
            return -1;
        default:
            assert(false);
    }
}

Actor::Actor(int imageID, int startX, int startY, int dir, double size)
    : GraphObject(imageID, startX, startY, dir, size), alive(true) {}
// int imageID, int startX, int startY, int dir = 0, double size = 1.0
Floor::Floor(int x, int y) : Actor(IID_FLOOR, x, y) {};
Player::Player(int x, int y) : Actor(IID_PLAYER, x, y) {};

void Player::doSomething() {
    if (!isAlive()) return;
    auto dir = getKeyDir();
    int x, y;
    getPositionInThisDirection(dir, 1, x, y);
    // TODO: Finish
}
