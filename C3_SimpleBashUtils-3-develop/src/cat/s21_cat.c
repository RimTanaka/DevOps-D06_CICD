#include "s21_cat.h"

int main(int argc, char **argv) {
  int flag_er = 0;
  char arguments[FLAG_COUNT] = {0};  // Массив флагов ascii[256]
  int i_last_arg = 1;
  flag_er = check_flag(arguments, argc, argv, &i_last_arg);
#ifdef _DCAT
  printf("ERRPR STATUS: %d\n\n", flag_er);
#endif
  if (!(flag_er)) {
#ifdef _DCAT
    for (int i = 0; i < FLAG_COUNT; ++i) {
      printf("Number: %d, status: %d, ASCII: %c\n", i, arguments[i], i);
    }
#endif

    if (i_last_arg == argc) print_file("-", arguments);

    for (int i = i_last_arg; i < argc; ++i) {
      // printf("\nas\n");
      if (strcmp(argv[i], "--") == 0) continue;
      print_file(argv[i], arguments);
    }
  } else {
    printf("Flag error!");
  }
  return flag_er;
}

int check_flag(char *arguments, int argc, char **argv, int *index) {
  int flag_er = 0;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--number-nonblank") == 0) {
      flag_er = all_flags(arguments, 'b');
      continue;
    } else if (strcmp(argv[i], "--number") == 0) {
      flag_er = all_flags(arguments, 'n');
      continue;
    } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
      flag_er = all_flags(arguments, 's');
      continue;
    }

    if (argv[i][0] != '-' || strcmp(argv[i], "--") == 0 ||
        strcmp(argv[i], "-") == 0) {
      // printf(argv[i]);
      break;
    } else {
      *index = i;
      for (size_t j = 1; j < strlen(argv[i]); ++j) {
        flag_er = all_flags(arguments, argv[i][j]);
      }
    }
  }
  return flag_er;
}

int all_flags(char *arguments, char flag) {
  int flag_er = 1;

  struct cat_flags cat_flag[8] = {{'b', "b"},  {'E', "E"}, {'e', "Ev"},
                                  {'n', "n"},  {'s', "s"}, {'T', "T"},
                                  {'t', "Tv"}, {'v', "v"}};

  for (int i = 0; i < 8; ++i) {
    if (cat_flag[i].flag == flag) {
      for (size_t j = 0; j < strlen(cat_flag[i].real_flag); ++j) {
        arguments[(int)cat_flag[i].real_flag[j]] = 1;
      }
      flag_er = 0;
      break;
    }
  }
  return flag_er;
}

int print_file(char *argv, char *arguments) {
  FILE *f = (strcmp("-", argv) == 0) ? stdin : fopen(argv, "r+");
  int flag_er = 0;
  if (f != NULL) {
    int index = 0;
    int eline_printed = 0;
    int c = fgetc(f);
    int prev = '\n';
    while (c != EOF) {
      print_symb(c, &prev, arguments, &index, &eline_printed);
      c = fgetc(f);
    }
    if (f != stdin) fclose(f);
  } else {
    flag_er = 1;
  }
  return flag_er;
}

// Сначала b, e, n, s, t, u, v, A, E, T, ((s, t, b, n, e))
void print_symb(int c, int *prev, char *flags, int *index, int *eline_printed) {
  if (!(flags['s'] && *prev == '\n' && c == '\n' && *eline_printed)) {
    (*prev == '\n' && c == '\n') ? (*eline_printed = 1) : (*eline_printed = 0);

    if (((flags['n'] && !flags['b']) || (flags['b'] && c != '\n')) &&
        *prev == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    } else if ((flags['b'] && (flags['E'])) && *prev == '\n') {
      printf("      \t");
    }

    if (flags['E'] && c == '\n') printf("$");

    if (flags['T'] && c == '\t') {
      printf("^");
      c += ASCII_OFFSET;
    }

    if (flags['v']) {
      if (c > 127 && c < 160) printf("M-^");
      if ((c < CONTROL_END && c != '\n' && c != '\t') || c == 127) printf("^");
      if ((c < CONTROL_END || (c > 126 && c < 160)) && c != '\n' && c != '\t')
        c = c > 126 ? c - 128 + ASCII_OFFSET : c + ASCII_OFFSET;
    }

    fputc(c, stdout);
  }
  *prev = c;
}
