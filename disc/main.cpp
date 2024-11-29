#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <assert.h>

#define N 3

// void reflect(int matrix[][N], int n) {
//   for (size_t i = 0; i < n; ++i) {
//     for (size_t j = i; j < n; ++j) {
//       std::swap(matrix[i][j], matrix[j][i]);
//     }
//   }
// }

bool charInsert(char str[], int n, int idx, char c) {
  // check if there's space to insert c assuming it's null padded up to the
  // actual size
  if (idx >= (n - 1) || idx < 0 || (str[n - 2] != '\0')) {
    return false;
  }
  strcpy(&str[idx + 1], &str[idx]);
  str[idx] = c;
  return true;
}

void reverse(char *arr) {
  auto l = strlen(arr);
  auto hl = l / 2;
  for (size_t i = 0; i < hl; ++i) {
    char *first = arr + i;
    char *last = arr + l - i - 1;
    auto temp = *first;
    *first = *last;
    *last = temp;
  }
}

// void printChars(const char *str) {
//   int i = 0;
//   while (*(str + i) != '\0') {
//     std::cout << *(str + i) << std::endl;
//     i++;
//   }
// }

void minMax(int *arr, int n, int *&min, int *&max) {
  if (n <= 0) return;
  min = &arr[0];
  max = &arr[0];
  for (int i = 0; i < n; ++i) {
    if (arr[i] < *min) {
      min = &arr[i];
    }
    if (arr[i] > *max) {
      max = &arr[i];
    }
  }
}

bool valid_parenthesis(char* text) {
  int lp = 0;
  int rp = 0;
  for (; *text != '\0'; ++text) {
    lp += *text == '(';
    rp += *text == ')';
  }
  return lp == rp;
}

void sum(int* list1, int list1_size, int* list2, int list2_size) {
  int sum = 0;
  for (int i = 0; i < list1_size; ++i) {
    sum += list1[i];
  }
  for (int i = 0; i < list1_size; ++i) {
    sum += list2[i];
  }
  std::cout << sum << std::endl;
}

int main() {
    char text1[1] = {'('};
    assert(valid_parenthesis(text1) == false);

    char text2[2] = {'(', ')'};
    assert(valid_parenthesis(text2) == true);

    char text3[4] = {'(', ')', '(', ')'};
    assert(valid_parenthesis(text3) == true);

    char text4[4] = {'(', '(', ')', ')'};
    assert(valid_parenthesis(text4) == true);

    char text5[2] = {'(', '('};
    assert(valid_parenthesis(text5) == false);

    char text6[] = "";
    assert(valid_parenthesis(text6) == true); // empty string is valid

    std::cout << "all valid!" << std::endl;

    int arr1[] = {8, 1, 3, 4};
    int arr2[] = {1, 2, 3, 4};
    sum(arr1, 4, arr2, 4);

    return 0;
}
