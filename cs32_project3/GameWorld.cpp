#include "GameWorld.h"
#include "GameController.h"
#include <cstdlib>
#include <string>
using namespace std;

bool GameWorld::getKey(int &value) {
  bool gotKey = m_controller->getKeyIfAny(value);

  if (gotKey) {
    if (value == 'q' || value == '\x03') // CTRL-C
      m_controller->quitGame();
  }
  return gotKey;
}

void GameWorld::playSound(int soundID) { m_controller->playSound(soundID); }

void GameWorld::setGameStatText(string text) {
  m_controller->setGameStatText(text);
}
