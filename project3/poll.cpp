#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
using namespace std;

//*************************************
//  isValidUppercaseStateCode
//*************************************

// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.

bool isValidUppercaseStateCode(const string stateCode) {
  const string codes = "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
                       "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NJ.NH.NM.NY.NC."
                       "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
  return (stateCode.size() == 2 &&
          stateCode.find('.') == string::npos &&  // no '.' in stateCode
          codes.find(stateCode) != string::npos); // match found
}

bool hasRightSyntax(string pollData) {
  // finite state machine:
  // state: 0 -> processing party code
  // state: 1 -> processing digits
  // state: 2 -> processing state code
  int state = 0;
  int valid = true;
  string code;
  // incrementing i is managed by each state,
  // but after each iter the state must advance
  for (int i = 0; i < pollData.size(); state = (state + 1) % 3) {
    valid = false;
    switch (state) {
    case 0:
      if (!isalpha(pollData[i]))
        return valid;
      i += 1;
      break;
    case 1:
      if (!isdigit(pollData[i]))
        return valid;
      // casting to bool so 0 or 1 since isdigit may not be 1
      i += 1 + (bool) ((i + 1 < pollData.size()) ? isdigit(pollData[i + 1]) : 0);
      break;
    case 2:
      code = pollData.substr(i, 2);
      transform(code.begin(), code.end(), code.begin(), ::toupper);
      if (!isValidUppercaseStateCode(code))
        return valid;
      i += 2;
      valid = true;
      break;
    default:
      __builtin_unreachable();
    }
  }
  return valid;
}

int computeVotes(string pollData, char party, int &voteCount) {
  if (!isalpha(party))
    return 3;
  if (!hasRightSyntax(pollData))
    return 1;
  // since the dpoint is read as upper-case only.
  party = toupper(party);

  // finite state machine:
  // state: 0 -> processing party code
  // state: 1 -> processing digits
  // state: 2 -> processing state code
  int localVoteCount = 0;
  int state = 0;
  int num = 0;
  bool good;
  char dpoint;
  int advance;
  // incrementing i is managed by each state,
  // but after each iter the state must advance
  for (size_t i = 0; i < pollData.size(); state = (state + 1) % 3) {
    dpoint = toupper(pollData[i]);
    switch (state) {
    case 0:
      // whether these votes should be looked at.
      good = party == dpoint;
      i += 1;
      break;
    case 1:
      // i + 1 is always inbounds since it is always a number or state code
      // in an assumed to be valid pollData
      // casting to bool so 0 or 1 since isdigit may not be 1
      advance = 1 + (bool) isdigit(pollData[i + 1]);
      num = stoi(pollData.substr(i, advance)); // to int
      if (num == 0) {
        return 2;
      }
      if (good) {
        localVoteCount += num;
      }
      i += advance;
      break;
    // per-state data isn't taken into account
    case 2:
      i += 2;
      break;
    default:
      // for debugging
      __builtin_unreachable();
    }
  }
  voteCount = localVoteCount;
  return 0;
}

int main() {
    // valid string
    assert(hasRightSyntax("D23NER10CA"));
    assert(hasRightSyntax("D28NYR40TXD4HI"));
    // valid empty string
    assert(hasRightSyntax(""));
    // invalid: extra leading char
    assert(!hasRightSyntax("9D54CA"));
    // invalid: no state
    assert(!hasRightSyntax("D54"));
    // invalid: extra trailing char
    assert(!hasRightSyntax("D54CA#"));
    // valid: lower case char
    assert(hasRightSyntax("r40ms"));
    // invalid: 3-digit votes
    assert(!hasRightSyntax("D100CA"));

    // valid: mixed uppercase and lowercase letters
    assert(hasRightSyntax("d54cAr40tX"));
    // invalid: invalid character between state predictions
    assert(!hasRightSyntax("D54CA@R40TX"));
    // invalid: invalid state code
    assert(!hasRightSyntax("D54XX"));
    // valid: zero electoral votes (syntactically valid)
    assert(hasRightSyntax("D0CA"));
    // invalid: missing party code
    assert(!hasRightSyntax("54CA"));
    // invalid: party instead of electoral votes
    assert(!hasRightSyntax("DRTX"));
    // valid: single-digit electoral votes
    assert(hasRightSyntax("D9CA"));
    // invalid: non-digit characters in electoral votes
    assert(!hasRightSyntax("D5aCA"));
    // valid: multiple state predictions
    assert(hasRightSyntax("D5CAr10TxL6Nv"));

    int voteCount = -1;

    // valid vote counts
    assert(computeVotes("R40ALD54WI", 'R', voteCount) == 0 && voteCount == 40);
    voteCount = -1;
    assert(computeVotes("D54CAR40TX", 'D', voteCount) == 0 && voteCount == 54);
    // invalid: invalid state code
    voteCount = -1;
    assert(computeVotes("R40ZZD54CA", 'D', voteCount) == 1 && voteCount == -1);
    // invalid: zero votes for a state
    voteCount = -1;
    assert(computeVotes("R0TX", 'R', voteCount) == 2 && voteCount == -1);
    // invalid: non-letter party character
    voteCount = -1;
    assert(computeVotes("R40TXD54CA", '9', voteCount) == 3 && voteCount == -1);
    // valid: no votes for the party specified
    voteCount = -1;
    assert(computeVotes("D54CA", 'R', voteCount) == 0 && voteCount == 0);
    // invalid: non-letter party character
    voteCount = -1;
    assert(computeVotes("D28NYR40TXD4HI", '@', voteCount) == 3 && voteCount == -1);
    // invalid: zero electoral votes in a state prediction
    voteCount = -1;
    assert(computeVotes("D0CA", 'D', voteCount) == 2 && voteCount == -1);
    // invalid: invalid poll data string
    voteCount = -1;
    assert(computeVotes("D54CA@", 'D', voteCount) == 1 && voteCount == -1);
    // invalid: non-letter party character
    voteCount = -1;
    assert(computeVotes("D54CA", '1', voteCount) == 3 && voteCount == -1);
    // valid: empty poll data string
    voteCount = -1;
    assert(computeVotes("", 'D', voteCount) == 0 && voteCount == 0);
    // valid: mixed uppercase and lowercase letters
    voteCount = -1;
    assert(computeVotes("d54cAr40tX", 'D', voteCount) == 0 && voteCount == 54);
    // valid: large electoral votes
    voteCount = -1;
    assert(computeVotes("D99CA", 'D', voteCount) == 0 && voteCount == 99);
    // invalid: three-digit electoral votes in computeVotes
    voteCount = -1;
    assert(computeVotes("D100CA", 'D', voteCount) == 1 && voteCount == -1);

    return 0;
}
