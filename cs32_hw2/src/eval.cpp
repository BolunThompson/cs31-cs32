#include <cassert>
#include <cstring>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <algorithm>

using namespace std;
constexpr char PREC_ORDER[] = {'!', '&', '|'};

static bool isOpt(char op) {
  return op == '&' || op == '|' || op == '!';
}

static bool isOptOrNull(char op) {
  return isOpt(op) || op == '\0';
}

static bool higherPrec(char op, char top) {
  for (char prec : PREC_ORDER) {
    if (prec == op) {
      return true;
    }
    if (prec == top) {
      return false;
    }
  }
  assert(false);
}

static bool validOpds(char op, char prec, char next) {
  switch (op) {
    case '!':
      return isOptOrNull(prec) && (isdigit(next) || next == '(' || next == '!');
    case '|':
    case '&':
      return (isdigit(prec) || prec == ')') && (isdigit(next) || next == '(' || next == '!');
    case '(':
      return (isOptOrNull(prec) || prec == '(') && (isdigit(next) || next == '(' || next == ')' || next == '!');
    case ')':
      return (isdigit(prec) || prec == '(' || prec == ')') && (next == ')' || next == '&' || next == '|' || next == '\0');
    case ' ':
      return true;
    default:
      if (isdigit(op)) {
        return (isOptOrNull(prec) || prec == '(') && (isOptOrNull(next) || next == ')');
      }
      return false;
  }
}

static optional<string> toPostfix(const string& infix) {
  string postfix = "";
  auto op_stack = stack<char>();
  char prev = '\0';
  // NOTE: This is why they teach about proper recursive descent parses
  // however, I do not care enough to learn the theory when it's just a handful of cases
  int paren_count = 0;
  for (auto it = infix.begin(); it != infix.end(); ++it) {

    auto c = *it;
    if (c == ' ') {
      continue;
    }

    // possibly bad time complexity depending on stack impl
    // but simple
    auto next_it = find_if(it + 1, infix.end(), [](auto v) { return v != ' '; });
    auto next = next_it != infix.end() ? *next_it : '\0';
    // could be simpler to do properly during evaluation, but this should work
    if (!validOpds(c, prev, next)) {
      return std::nullopt;
    }
    if (isdigit(c)) {
      postfix += c;
    } else if (c == '(') {
      op_stack.push(c);
      ++paren_count;
    } else if (c == ')') {
      if (--paren_count < 0) {
        return std::nullopt;
      }
      while (op_stack.top() != '(') {
        postfix += op_stack.top();
        op_stack.pop();
      }
      op_stack.pop();
    } else if (c == '!') {
      op_stack.push(c);
    } else if (isOpt(c)) {
      // TODO: Fix precedence.
      while (!op_stack.empty() && op_stack.top() != '(' &&
             higherPrec(op_stack.top(), c)) {
        postfix += op_stack.top();
        op_stack.pop();
      }
      op_stack.push(c);
    } else {
      return std::nullopt;
    }
    prev = c;
  }
  if (paren_count != 0) {
    return std::nullopt;
  }
  while (!op_stack.empty()) {
    postfix += op_stack.top();
    op_stack.pop();
  }
  return postfix;
}

int evaluate(string infix, const bool values[], string &postfix, bool &result) {
  auto opd_stack = stack<bool>();
  auto postfix_opt = toPostfix(infix);
  if (!postfix_opt) {
    return 1;
  }
  postfix = postfix_opt.value();
  bool res;

  for (char c : postfix) {
    if (isdigit(c)) {
      opd_stack.push(values[c - '0']);
    } else if (c == '!') {
      res = !opd_stack.top();
      opd_stack.pop();
      opd_stack.push(res);
    } else if (c == '|' || c == '&') {
      auto v1 = opd_stack.top();
      opd_stack.pop();
      auto v2 = opd_stack.top();
      opd_stack.pop();
      switch (c) {
        case '|':
          res = v1 || v2;
          break;
        case '&':
          res = v1 && v2;
          break;
        default:
          return 1;
      }
      opd_stack.push(res);
    }
    else {
      return 1;
    }
  }
  if (opd_stack.size() == 1) {
    result = opd_stack.top();
    return 0;
  }
  return 1;

}

int main() {
  bool ba[10] = {//  0      1      2      3      4      5      6      7      8 9
                 true,  true, true,  false, false,
                 false, true, false, true,  false};
  string pf;
  bool answer;
  assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && answer);
  assert(evaluate("", ba, pf, answer) == 1);
  assert(evaluate("8|", ba, pf, answer) == 1);
  assert(evaluate(" &6", ba, pf, answer) == 1);
  assert(evaluate("4 5", ba, pf, answer) == 1);
  assert(evaluate("01", ba, pf, answer) == 1);
  assert(evaluate("()", ba, pf, answer) == 1);
  assert(evaluate("()4", ba, pf, answer) == 1);
  assert(evaluate("2(9|8)", ba, pf, answer) == 1);
  assert(evaluate("2(&8)", ba, pf, answer) == 1);
  assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
  assert(evaluate("x+5", ba, pf, answer) == 1);
  assert(evaluate("2|3|4", ba, pf, answer) == 0 && pf == "23|4|" && answer);
  assert(evaluate("2|(3|4)", ba, pf, answer) == 0 && pf == "234||" && answer);
  assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 &&
         pf == "43!03&&|" && !answer);
  assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
  assert(evaluate("((6))", ba, pf, answer) == 0 && pf == "6" && answer);
  ba[2] = false;
  ba[9] = true;
  assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9" && answer);
  assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
  cout << "Passed all tests" << endl;
}


