#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 8192
#define MAX_REGEX_COUNT 10
#define MAX_REGEX_LENGTH 256
#define MAX_MATCHES 500

typedef struct {
  bool use_regex;          // e
  bool ignore_case;        // i
  bool invert_match;       // v
  bool count_only;         // c
  bool filenames_only;     // l
  bool line_number;        // n
  bool suppress_errors;    // s
  bool pattern_from_file;  // f
  bool output_only_match;  // o
  bool ignore_filenames;   // h
} Options;

bool reading_flags(int argc, char** argv,
                   char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
                   size_t* number_of_regex, Options* options);
char match(char* string, char* pattern, bool flag_icase, regmatch_t* pmatch,
           int eflag);
void print_string(char* str, size_t size);
void s21_grep(FILE* file, Options options, bool flag_few_files, char** argv,
              char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
              size_t size_array_of_regex);
void read_regular_expressions(
    FILE* file, char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
    size_t* number_of_regex, bool flag_o, bool* flag_empty, bool* correct);
void update_flags(size_t cond, bool* correct, bool* flag_empty, bool flag_o);
void find_matches(regmatch_t matches[], size_t* number_of_matches, char* line,
                  char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
                  size_t size_array_of_regex, bool flag_o, bool flag_i,
                  bool* output_flag);

void print_match(char* line, regoff_t start, regoff_t end);
void print_usage(char* argv);
void check_options(const char* pattern, Options options);
void process_flag(int rez, char* optarg,
                  char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
                  size_t* number_of_regex, Options* options, bool* correct,
                  bool* flag_empty);

#endif  // S21_GREP_H
