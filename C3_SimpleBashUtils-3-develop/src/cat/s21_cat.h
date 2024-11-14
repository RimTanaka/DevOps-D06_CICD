#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <string.h>

#define FLAG_COUNT 256  // всего возможных Ascii кодов
#define CONTROL_START 0
#define CONTROL_END 32
#define ASCII_OFFSET 64

struct cat_flags {
  char flag;
  char *real_flag;
};

int check_flag(char *arguments, int argc, char **argv, int *index);
int print_file(char *argv, char *arguments);
int all_flags(char *arguments, char flag);
void append_flag(char *flags, char flag);
void print_symb(int c, int *prev, char *flags, int *index, int *line_prt);

#endif  // S21_CAT_H
