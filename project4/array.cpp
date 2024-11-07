#include <cassert>
#include <iostream>
#include <string>

#define ARR_EQ(array, ...)                                                     \
  do {                                                                         \
    string expected[] = {__VA_ARGS__};                                         \
    size_t n = sizeof(expected) / sizeof(expected[0]);                         \
    for (size_t i = 0; i < n; ++i) {                                           \
      assert((array)[i] == expected[i]);                                       \
    }                                                                          \
  } while (0)

using namespace std;

// can't use std::min because it's <algorithm>
inline int min(int n1, int n2) { return (n1 < n2) ? n1 : n2; }
// not using std::swap out of an abundance of caution
inline void swap(string &a, string &b) {
  auto temp = a;
  a = b;
  b = temp;
}

int appendToAll(string a[], int n, string value) {
  if (n < 0)
    return -1;
  for (size_t i = 0; i < n; ++i)
    a[i] += value;
  return n;
}

int lookup(const string a[], int n, string target) {
  if (n < 0)
    return -1;
  for (size_t i = 0; i < n; ++i) {
    if (a[i] == target)
      return i;
  }
  return -1;
}

int positionOfMax(const string a[], int n) {
  if (n <= 0)
    return -1;
  size_t largestInd = 0;
  for (size_t i = 0; i < n; ++i) {
    if (a[i] > a[largestInd])
      largestInd = i;
  }
  return largestInd;
}

int rotateLeft(string a[], int n, int pos) {
  if (n < 0 || pos < 0 || pos >= n)
    return -1;

  string popped = a[pos];
  for (size_t i = pos; i < n - 1; ++i) {
    a[i] = a[i + 1];
  }
  a[n - 1] = popped;

  return pos;
}

int countRuns(const string a[], int n) {
  if (n < 0)
    return -1;
  if (n == 0)
    return 0;

  int runs = 0;
  auto last = &a[0];
  for (size_t i = 1; i < n; ++i) {
    if (a[i] != *last)
      runs += 1;
    last = &a[i];
  }
  ++runs;
  return runs;
}

int flip(string a[], int n) {
  if (n < 0)
    return -1;
  for (size_t i = 0; i < n / 2; ++i) {
    swap(a[i], a[n - i - 1]);
  }
  return n;
}

int differ(const string a1[], int n1, const string a2[], int n2) {
  if (n1 < 0 || n2 < 0)
    return -1;
  for (size_t i = 0; i < n1 && i < n2; ++i) {
    if (a1[i] != a2[i])
      return i;
  }
  return min(n1, n2);
}

int subsequence(const string a1[], int n1, const string a2[], int n2) {
  if (n1 < 0 || n2 < 0 || n2 > n1)
    return -1;
  if (n2 == 0)
    return 0;
  size_t subseq_start = 0;
  size_t si = 0;
  for (size_t i = 0; i < n1; ++i) {
    if (a1[i] == a2[si++]) {
      if (si == n2)
        return subseq_start;
    } else {
      subseq_start = i + 1;
      si = 0;
    }
  }
  return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2) {
  if (n1 < 0 || n2 < 0)
    return -1;
  for (size_t i = 0; i < n1; ++i) {
    for (size_t j = 0; j < n2; ++j) {
      if (a1[i] == a2[j])
        return i;
    }
  }
  return -1;
}

int separate(string a[], int n, string separator) {
  if (n < 0)
    return -1;
  // next available index to move small elements
  // intuitive example: think of this as going through a unsorted list of
  // apples, pears, and one hybrid apple pear, and, whenever an apple is
  // encountered, swapping it with the next front object.
  size_t p = 0;
  for (size_t i = 0; i < n; ++i) {
    if (a[i] < separator) {
      swap(a[p], a[i]);
      ++p;
    }
  }
  return p;
}

int main() {
  int res; // to test the return value

  // appendToAll
  string pt1[5] = {"donald", "kamala", "jd", "tim", "jill"};
  res = appendToAll(pt1, 5, "!!!");
  assert(res == 5);
  ARR_EQ(pt1, "donald!!!", "kamala!!!", "jd!!!", "tim!!!", "jill!!!");

  // n equals zero for appendToAll
  string pt2[3] = {"donald", "kamala", "jd"};
  res = appendToAll(pt2, 0, "!!!");
  assert(res == 0);
  ARR_EQ(pt2, "donald", "kamala", "jd");

  // negative n for appendToAll
  string pt3[3] = {"donald", "kamala", "jd"};
  res = appendToAll(pt3, -1, "!!!");
  assert(res == -1);
  ARR_EQ(pt3, "donald", "kamala", "jd");

  // empty strings in array for appendToAll
  string c1[3] = {"", "", ""};
  res = appendToAll(c1, 3, "test");
  assert(res == 3);
  ARR_EQ(c1, "test", "test", "test");

  // appending empty string for appendToAll
  string c2[3] = {"a", "b", "c"};
  res = appendToAll(c2, 3, "");
  assert(res == 3);
  ARR_EQ(c2, "a", "b", "c");

  // lookup
  string pt4[7] = {"melania", "kamala", "donald", "tim", "", "doug", "jd"};
  res = lookup(pt4, 7, "doug");
  assert(res == 5);

  // target not in array for lookup
  res = lookup(pt4, 7, "joe");
  assert(res == -1);

  // multiple occurrences of target for lookup
  string pt5[7] = {"melania", "donald", "donald", "tim", "", "doug", "jd"};
  res = lookup(pt5, 7, "donald");
  assert(res == 1);

  // empty string as target for lookup
  res = lookup(pt4, 7, "");
  assert(res == 4);

  // n equals zero for lookup
  res = lookup(pt4, 0, "melania");
  assert(res == -1);

  // negative n for lookup
  res = lookup(pt4, -3, "melania");
  assert(res == -1);

  // positionOfMax
  string pt6[6] = {"doug", "kamala", "melania", "usha", "gwen", "donald"};
  res = positionOfMax(pt6, 6);
  assert(res == 3); // "usha" is the largest

  // duplicate maximum values for positionOfMax
  string pt7[5] = {"donald", "donald", "donald", "donald", "donald"};
  res = positionOfMax(pt7, 5);
  assert(res == 0);

  // n equals zero for positionOfMax
  res = positionOfMax(pt6, 0);
  assert(res == -1);

  // negative n for positionOfMax
  res = positionOfMax(pt6, -2);
  assert(res == -1);

  // rotateLeft
  string pt8[5] = {"kamala", "donald", "jd", "tim", "joe"};
  res = rotateLeft(pt8, 5, 1);
  assert(res == 1);
  ARR_EQ(pt8, "kamala", "jd", "tim", "joe", "donald");

  // rotate first element for rotateLeft
  string pt9[5] = {"kamala", "donald", "jd", "tim", "joe"};
  res = rotateLeft(pt9, 5, 0);
  assert(res == 0);
  ARR_EQ(pt9, "donald", "jd", "tim", "joe", "kamala");

  // rotate last element for rotateLeft
  string pt10[5] = {"kamala", "donald", "jd", "tim", "joe"};
  res = rotateLeft(pt10, 5, 4);
  assert(res == 4);
  ARR_EQ(pt10, "kamala", "donald", "jd", "tim", "joe");

  // n equals zero for rotateLeft
  res = rotateLeft(pt8, 0, 0);
  assert(res == -1);

  // pos out of bounds for rotateLeft
  res = rotateLeft(pt8, 5, 5);
  assert(res == -1);

  // negative n for rotateLeft
  res = rotateLeft(pt8, -2, 1);
  assert(res == -1);

  // countRuns
  string pt11[9] = {"melania", "doug", "gwen", "gwen", "jill",
                    "jill",    "jill", "gwen", "gwen"};
  res = countRuns(pt11, 9);
  assert(res == 5);

  // all elements identical for countRuns
  string a[5] = {"same", "same", "same", "same", "same"};
  res = countRuns(a, 5);
  assert(res == 1);

  // all elements unique for countRuns
  string b[5] = {"a", "b", "c", "d", "e"};
  res = countRuns(b, 5);
  assert(res == 5);

  // n equals zero for countRuns
  res = countRuns(b, 0);
  assert(res == 0);

  // single element array for countRuns
  string c[1] = {"onlyone"};
  res = countRuns(c, 1);
  assert(res == 1);

  // negative n for countRuns
  res = countRuns(c, -3);
  assert(res == -1);

  // flip
  string pt12[6] = {"kamala", "doug", "", "jill", "jd", "donald"};
  res = flip(pt12, 4);
  assert(res == 4);
  ARR_EQ(pt12, "jill", "", "doug", "kamala", "jd", "donald");

  // n equals zero for flip
  string arr1[4] = {"one", "two", "three", "four"};
  res = flip(arr1, 0);
  assert(res == 0);
  ARR_EQ(arr1, "one", "two", "three", "four");

  // single element array for flip
  string arr2[1] = {"onlyone"};
  res = flip(arr2, 1);
  assert(res == 1);
  ARR_EQ(arr2, "onlyone");

  // even number of elements for flip
  string arr3[4] = {"one", "two", "three", "four"};
  res = flip(arr3, 4);
  assert(res == 4);
  ARR_EQ(arr3, "four", "three", "two", "one");

  // negative n for flip
  res = flip(arr3, -2);
  assert(res == -1);

  // differ
  string pt13[6] = {"kamala", "doug", "", "jill", "jd", "donald"};
  string pt14[5] = {"kamala", "doug", "donald", "", "jd"};
  res = differ(pt13, 6, pt14, 5);
  assert(res == 2);

  // arrays are identical for differ
  res = differ(pt13, 6, pt13, 6);
  assert(res == 6);

  // one array is prefix of another for differ
  res = differ(pt13, 6, pt14, 2);
  assert(res == 2);

  // n1 equals zero for differ
  res = differ(pt13, 0, pt14, 5);
  assert(res == 0);

  // n2 equals zero for differ
  res = differ(pt13, 6, pt14, 0);
  assert(res == 0);

  // negative n1 for differ
  res = differ(pt13, -3, pt14, 5);
  assert(res == -1);

  // negative n2 for differ
  res = differ(pt13, -3, pt14, 5);
  assert(res == -1);

  // subsequence
  string pt15[6] = {"kamala", "tim", "usha", "gwen", "donald", "jd"};
  string pt16[3] = {"tim", "usha", "gwen"};
  res = subsequence(pt15, 6, pt16, 3);
  assert(res == 1);

  // subsequence not found
  string pt17[2] = {"kamala", "gwen"};
  res = subsequence(pt15, 5, pt17, 2);
  assert(res == -1);

  // subsequence at beginning
  string pt18[2] = {"kamala", "tim"};
  res = subsequence(pt15, 6, pt18, 2);
  assert(res == 0);

  // subsequence equals a1
  res = subsequence(pt15, 6, pt15, 6);
  assert(res == 0);

  // n2 equals zero for subsequence
  res = subsequence(pt15, 6, pt16, 0);
  assert(res == 0);

  // n1 equals zero, n2 greater than zero for subsequence
  res = subsequence(pt15, 0, pt16, 2);
  assert(res == -1);

  // negative n1 for subsequence
  res = subsequence(pt15, -6, pt16, 2);
  assert(res == -1);

  // negative n2 for subsequence
  res = subsequence(pt15, 6, pt16, -2);
  assert(res == -1);

  // lookupAny
  string pt19[4] = {"donald", "melania", "gwen", "tim"};
  res = lookupAny(pt15, 6, pt19, 4);
  assert(res == 1); // "tim" is at index 1

  // no matches for lookupAny
  string pt20[2] = {"jill", "joe"};
  res = lookupAny(pt15, 6, pt20, 2);
  assert(res == -1);

  // multiple possible matches for lookupAny
  string pt21[3] = {"jd", "kamala", "usha"};
  res = lookupAny(pt15, 6, pt21, 3);
  assert(res == 0); // "kamala" is at index 0

  // n1 equals zero for lookupAny
  res = lookupAny(pt15, 0, pt19, 4);
  assert(res == -1);

  // n2 equals zero for lookupAny
  res = lookupAny(pt15, 6, pt19, 0);
  assert(res == -1);

  // negative n1 for lookupAny
  res = lookupAny(pt15, -2, pt19, 4);
  assert(res == -1);

  // negative n2 for lookupAny
  res = lookupAny(pt15, 2, pt19, -4);
  assert(res == -1);

  // separate
  string pt22[6] = {"doug", "kamala", "melania", "usha", "gwen", "donald"};
  res = separate(pt22, 6, "joe");
  assert(res == 3);
  // verify that elements less than "joe" are before res
  for (size_t i = 0; i < res; i++) {
    assert(pt22[i] < "joe");
  }
  // verify that elements greater than or equal to "joe" are at or after res
  for (size_t i = res; i < 6; i++) {
    assert(pt22[i] >= "joe");
  }

  // separator not present in array for separate
  string pt23[6] = {"doug", "kamala", "melania", "usha", "gwen", "donald"};
  res = separate(pt23, 6, "zoe");
  assert(res == 6);
  // all elements should be less separator
  for (size_t i = 0; i < 6; i++) {
    assert(pt23[i] < "zoe");
  }

  // all elements greater than separator for separate
  string pt24[6] = {"doug", "kamala", "melania", "usha", "gwen", "donald"};
  res = separate(pt24, 6, "a");
  assert(res == 0); // no elements less than
  // all elements should be greater than or equal to the separator
  for (size_t i = 0; i < 6; i++) {
    assert(pt24[i] >= "a");
  }

  // elements equal to separator for separate
  string pt25[6] = {"donald", "gwen", "donald", "kamala", "melania", "donald"};
  res = separate(pt25, 6, "donald");
  assert(res == 0); // no elements less than "donald"

  // n equals zero for separate
  res = separate(pt6, 0, "joe");
  assert(res == 0);

  // negative n for separate
  res = separate(pt6, -3, "joe");
  assert(res == -1);

  clog << "All tests succeeded" << endl;

  return 0;
}
