#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <string>

using namespace std;

// much longer than needed to be safe
constexpr int MAX_STAT_LEN = 100;

StudentWorld *createStudentWorld(string assetPath) {
  return new StudentWorld(assetPath);
}

void StudentWorld::generateActors(const Level &level) {
  for (int i = 0; i < VIEW_HEIGHT; ++i) {
    for (int j = 0; j < VIEW_WIDTH; ++j) {
      switch (level.getContentsOf(i, j)) {
      case Level::empty:
        break;
      case Level::player:
        mPlayer = new Player(this, i, j);
        addActor(mPlayer);
        break;
      case Level::left_kong:
        addActor(new Kong(this, i, j, Actor::left));
        break;
      case Level::right_kong:
        addActor(new Kong(this, i, j, Actor::right));
        break;
      case Level::floor:
        addActor(new Floor(this, i, j));
        break;
      case Level::ladder:
        addActor(new Ladder(this, i, j));
        break;
      case Level::bonfire:
        addActor(new Bonfire(this, i, j));
        break;
      case Level::fireball:
        addActor(new Fireball(this, i, j));
        break;
      case Level::koopa:
        addActor(new Koopa(this, i, j));
        break;
      case Level::extra_life:
        addActor(new ExtraLife(this, i, j));
        break;
      case Level::garlic:
        addActor(new Garlic(this, i, j));
        break;
      default:
        assert(false);
      }
    }
  }
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {}

// finds level and attempts to load actors for it
int StudentWorld::init() {
  levelWon = false;
  Level level(assetPath());
  int levelNumber = getLevel();
  // because spec specificaly states that if the last level is 99, the player has won
  if (levelNumber >= 100) {
    return GWSTATUS_PLAYER_WON;
  }
  string ind = to_string(levelNumber);
  if (ind.length() == 1) {
    ind = "0" + ind;
  }
  string fname = "level" + ind + ".txt";

  auto res = level.loadLevel(fname);
  switch (res) {
  case Level::load_fail_file_not_found:
    return GWSTATUS_PLAYER_WON;
  case Level::load_success:
    generateActors(level);
    return GWSTATUS_CONTINUE_GAME;
  default:
    return GWSTATUS_LEVEL_ERROR;
  }
}

void StudentWorld::addNextActors() {
  actors.insert(actors.end(), actorsToAdd.begin(), actorsToAdd.end());
  actorsToAdd.clear();
}

int StudentWorld::move() {
  setGameStatText(getStatusLine());

  for (const auto &actor : actors) {
    actor->doSomething();
    // this would leave some number of not alive actors not deleted
    // but it's fine since the levelis going to be cleaned up after this
    if (!mPlayer->isAlive()) {
      return GWSTATUS_PLAYER_DIED;
    }
    if (levelWon) {
      return GWSTATUS_FINISHED_LEVEL;
    }
  }

  auto toBeRemoved =
      remove_if(actors.begin(), actors.end(), [&](const auto &actor) {
        bool dead = !actor->isAlive();
        if (dead) {
          delete actor;
        }
        return dead;
      });
  actors.erase(toBeRemoved, actors.end());

  addNextActors();
  return GWSTATUS_CONTINUE_GAME;
}

std::string StudentWorld::getStatusLine() const {
  int score = getScore();
  int level = getLevel();
  int livesLeft = getLives();
  int burps = mPlayer->getBurps();

  // snprintf since we aren't using c++20 (for fmt) and stringstream is very
  // verbose
  char stat[MAX_STAT_LEN];
  // snprintf into a buffer of the same size to prevent buffer overflows
  // printf style formatting
  snprintf(stat, MAX_STAT_LEN,
           "Score %07d  Level: %02d  Lives: %02d Burps: %02d", score, level,
           livesLeft, burps);
  return std::string(stat);
}

void StudentWorld::cleanUp() {
  for (auto const &actor : actors) {
    delete actor;
  }
  // very unlikely that the player would die on the exact tick
  // an actor would be added, but still possible
  for (auto const &actor : actorsToAdd) {
    delete actor;
  }
  actors.clear();
  actorsToAdd.clear();
}

// using anonymous functions to simplify the
// for each actor:
//    determine whether an actor with this property exists / do something with
//    actor
// pattern
bool StudentWorld::validMoveLoc(int x, int y) const {
  return !actorExists(x, y, [](auto a) { return a->isSolid(); });
}
bool StudentWorld::validFallLoc(int x, int y) const {
  return validMoveLoc(x, y) &&
         !actorExists(x, y, [](auto a) { return a->isStandable(); });
}

void StudentWorld::attackFlammable(Bonfire *fire) const {
  return attackPred(fire, [](auto a) { return a->isFlammable(); });
}

void StudentWorld::attackBlastable(Burp *burp) const {
  return attackPred(burp, [](auto a) { return a->isBlastable(); });
}

void StudentWorld::winLevel() {
  increaseScore(1000);
  playSound(GWSTATUS_FINISHED_LEVEL);
  levelWon = true;
}

// templates are for private functions so they can be in the implementation
// safely

template <typename F>
auto StudentWorld::getAnActor(int x, int y, F func) const {
  auto it = std::find_if(actors.begin(), actors.end(), [&](Actor *v) {
    return rightLoc(x, y, v) && func(v);
  });
  return it;
}

template <typename F>
bool StudentWorld::actorExists(int x, int y, F func) const {
  auto it = getAnActor(x, y, func);
  auto actor = it != actors.end() ? std::make_optional(*it) : std::nullopt;
  return actor.has_value() && func(actor.value());
}

template <typename F>
bool StudentWorld::actorExists(const Actor *actor, F func) const {
  return actorExists(actor->getX(), actor->getY(), func);
}

template <typename F>
void StudentWorld::attackPred(const Actor *source, F func) const {
  for (const auto actor : actors) {
    if (actor->isAlive() && actorsTogether(source, actor) && func(actor)) {
      actor->doAttacked();
    }
  }
}

bool StudentWorld::existsStandable(int x, int y) const {
  return actorExists(x, y, [](auto a) { return a->isStandable(); });
}

bool StudentWorld::existsClimbable(int x, int y) const {
  return actorExists(x, y, [](auto a) { return a->isClimbable(); });
}

bool StudentWorld::existsSolid(int x, int y) const {
  return actorExists(x, y, [](auto a) { return a->isSolid(); });
}
bool StudentWorld::actorAtPlayer(const Actor *actor) const {
  return actorExists(mPlayer, [=](auto a) { return a == actor; });
}
