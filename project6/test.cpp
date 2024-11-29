#include <iostream>
using namespace std;

void removeS(char *msg) {
  char *head = msg;
  for (; *msg != '\0'; ++msg) {
    if (*msg != 's' && *msg != 'S') {
      *head = *msg;
      ++head;
    }
  }
  *head = '\0';
}

int main() {
  char msg[50] = "She'll blossom like a massless princess.";
  removeS(msg);
  cout << msg << endl; // prints   he'll bloom like a male prince.
}
