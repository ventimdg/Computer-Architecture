#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

char* COLOR_GREEN = "";
char* COLOR_RESET = "";

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) {
  return x > 42;
}

bool is_vowel(char c) {
  char* vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  if (!assert_true("Test", is_tail('w'))){
    return false;
  }
  if (!assert_true("Test", is_tail('a'))){
    return false;
  }
  if (!assert_true("Test", is_tail('s'))){
    return false;
  }
  if (!assert_true("Test", is_tail('d'))){
    return false;
  }
  if (!assert_false("Test", is_tail('W'))){
    return false;
  }
  if (!assert_false("Test", is_tail('A'))){
    return false;
  }
  if (!assert_false("Test", is_tail('S'))){
    return false;
  }
  if (!assert_false("Test", is_tail('D'))){
    return false;
  }
  return true;
}

bool test_is_head() {
  if (!assert_false("Test", is_head('w'))){
    return false;
  }
  if (!assert_false("Test", is_head('a'))){
    return false;
  }
  if (!assert_false("Test", is_head('s'))){
    return false;
  }
  if (!assert_false("Test", is_head('d'))){
    return false;
  }
  if (!assert_true("Test", is_head('W'))){
    return false;
  }
  if (!assert_true("Test", is_head('A'))){
    return false;
  }
  if (!assert_true("Test", is_head('S'))){
    return false;
  }
  if (!assert_true("Test", is_head('D'))){
    return false;
  }
  if (!assert_true("Test", is_head('x'))){
    return false;
  }
  return true;
}

bool test_is_snake() {
  if (!assert_true("Test", is_snake('x'))){
    return false;
  }
  if (!assert_true("Test", is_snake('w'))){
    return false;
  }
  if (!assert_true("Test", is_snake('a'))){
    return false;
  }
  if (!assert_true("Test", is_snake('s'))){
    return false;
  }
  if (!assert_true("Test", is_snake('d'))){
    return false;
  }
  if (!assert_true("Test", is_snake('W'))){
    return false;
  }
  if (!assert_true("Test", is_snake('A'))){
    return false;
  }
  if (!assert_true("Test", is_snake('S'))){
    return false;
  }
  if (!assert_true("Test", is_snake('D'))){
    return false;
  }
  if (!assert_true("Test", is_snake('^'))){
    return false;
  }
  if (!assert_true("Test", is_snake('<'))){
    return false;
  }
  if (!assert_true("Test", is_snake('>'))){
    return false;
  }
  if (!assert_true("Test", is_snake('v'))){
    return false;
  }
  if (!assert_false("Test", is_snake('h'))){
    return false;
  }
  return true;
}

bool test_body_to_tail() {
  if (!assert_equals_char("Test", 'a', body_to_tail('<'))){
    return false;
  }
  if (!assert_equals_char("Test", 'w', body_to_tail('^'))){
    return false;
  }
  if (!assert_equals_char("Test", 'd', body_to_tail('>'))){
    return false;
  }
  if (!assert_equals_char("Test", 's', body_to_tail('v'))){
    return false;
  }
  return true;
}

bool test_head_to_body() {
  if (!assert_equals_char("Test", '^', head_to_body('W'))){
    return false;
  }
  if (!assert_equals_char("Test", '<', head_to_body('A'))){
    return false;
  }
  if (!assert_equals_char("Test", 'v', head_to_body('S'))){
    return false;
  }
  if (!assert_equals_char("Test", '>', head_to_body('D'))){
    return false;
  }
  return true;
}

bool test_get_next_x() {
  if (!assert_equals_unsigned_int("Test", 1, get_next_x(1, 'w'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 1, get_next_x(1, 's'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 2, get_next_x(1, 'D'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 2, get_next_x(1, 'd'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 2, get_next_x(1, '>'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 0, get_next_x(1, 'A'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 0, get_next_x(1, 'a'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 0, get_next_x(1, '<'))){
    return false;
  }
  return true;
}

bool test_get_next_y() {
  if (!assert_equals_unsigned_int("Test", 1, get_next_y(1, 'a'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 1, get_next_y(1, 'd'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 0, get_next_y(1, 'W'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 0, get_next_y(1, 'w'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 0, get_next_y(1, '^'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 2, get_next_y(1, 'S'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 2, get_next_y(1, 's'))){
    return false;
  }
  if (!assert_equals_unsigned_int("Test", 2, get_next_y(1, 'v'))){
    return false;
  }
  return true;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_get_next_x()) {
    printf("%s\n", "test_get_next_x failed");
    return false;
  }
  if (!test_get_next_y()) {
    printf("%s\n", "test_get_next_y failed");
    return false;
  }
  return true;
}

void init_colors() {
  if (!isatty(STDOUT_FILENO)) {
    return;
  }

  if (getenv("NO_COLOR") != NULL) {
    return;
  }

  char* term = getenv("TERM");
  if (term == NULL || strstr(term, "xterm") == NULL) {
    return;
  }

  COLOR_GREEN = "\033[0;32m";
  COLOR_RESET = "\033[0m";
}

bool test_and_print(char* label, bool (*run_test)()) {
  printf("\nTesting %s...\n", label);
  bool result = run_test();
  if (result) {
    printf("%sAll %s tests passed!%s\n", COLOR_GREEN, label, COLOR_RESET);
  } else {
    printf("Not all %s tests passed.\n", label);
  }
  return result;
}

int main(int argc, char* argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
