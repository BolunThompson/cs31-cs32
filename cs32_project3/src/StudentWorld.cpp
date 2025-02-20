#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <cassert>
#include <string>
using namespace std;

GameWorld *createStudentWorld(string assetPath) {
  return new StudentWorld(assetPath);
}

vector<Actor*> generate_actors(const Level& level) {
  vector<Actor*> actors;
  for (int i = 0; i < VIEW_HEIGHT; ++i) {
    for (int j = 0; j < VIEW_WIDTH; ++j) {
      switch (level.getContentsOf(i, j)) {
        case Level::empty:
            // No actor to create for an empty cell.
            break;
        case Level::player:
            actors.push_back(new Player(i, j));
            break;
        case Level::left_kong:
            actors.push_back(new LeftKong(i, j));
            break;
        case Level::right_kong:
            actors.push_back(new RightKong(i, j));
            break;
        case Level::floor:
            actors.push_back(new Floor(i, j));
            break;
        case Level::ladder:
            actors.push_back(new Ladder(i, j));
            break;
        case Level::bonfire:
            actors.push_back(new Bonfire(i, j));
            break;
        case Level::fireball:
            actors.push_back(new Fireball(i, j));
            break;
        case Level::koopa:
            actors.push_back(new Koopa(i, j));
            break;
        case Level::extra_life:
            actors.push_back(new ExtraLife(i, j));
            break;
        case Level::garlic:
            actors.push_back(new Garlic(i, j));
            break;
        default:
          assert(false);
      }
    }
  }
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {
}

int StudentWorld::init() {
  Level level(assetPath());
  // Get level number
  string ind = to_string(getLevel());
  if (ind.length() == 1) {
    ind = "0" + ind;
  }
  string fname = "level" + ind + ".txt";
  auto res = level.loadLevel(fname);
  if (res != Level::load_success) {
    return GWSTATUS_LEVEL_ERROR;
  }
  actors = generate_actors(level);

  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  // This code is here merely to allow the game to build, run, and terminate
  // after you type q
  for (const auto& actor : actors) {
    actor->doSomething();
  }
  setGameStatText("Game will end when you type q");

  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
  for (auto const& actor : actors) {
    delete actor;
  }
  actors.clear();
}
