#include "s21_grep.h"

int main(int argc, char** argv) {
  if (argc <= 2) {
    print_usage(argv[0]);
    exit(1);
  }

  Options options = {false};
  char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH];
  size_t size_array_of_regex = 0;  // проверка на регулярное выражение

  bool correct =
      reading_flags(argc, argv, array_of_regex, &size_array_of_regex, &options);

  if (correct && size_array_of_regex == 0) {
    strncpy(array_of_regex[size_array_of_regex], argv[optind],
            MAX_REGEX_LENGTH);
    size_array_of_regex++;
    optind++;
  }

  bool flag_few_files = (argc > optind + 1) ? 1 : 0;

  while (correct && optind < argc) {
    FILE* file = fopen(argv[optind], "r+");
    if (!file) {
      if (!options.suppress_errors)
        printf("s21_grep: %s: Не удалось открыть файл\n", argv[optind]);
    } else {
#ifdef _DGREP
      check_options(argv[optind], options);
#endif
      s21_grep(file, options, flag_few_files, argv, array_of_regex,
               size_array_of_regex);
      fclose(file);
    }
    optind++;
  }

  return 0;
}

void print_usage(char* argv) {
  printf("Использование: %s [опции] шаблон [файл...]\n", argv);
  printf("Опции:\n");
  printf("  -e    Использовать расширенные регулярные выражения\n");
  printf("  -i    Игнорировать регистр символов\n");
  printf("  -v    Инвертировать совпадения\n");
  printf(
      "  -c    Выводить только количество совпадающих строк для каждого "
      "файла\n");
  printf("  -l    Выводить только имена файлов с совпадающими строками\n");
  printf("  -n    Префикс каждой строки вывода номером строки, начиная с 1\n");
  printf("  -h    Показать это сообщение справки\n");
  printf("  -s    Подавить вывод сообщений об ошибках\n");
  printf("  -f    Рассматривать шаблон как файл и читать шаблоны из него\n");
  printf(
      "  -o    Выводить только совпадающие (не пустые) части совпадающих "
      "строк\n");
}

void check_options(const char* file, Options options) {
  printf("Параметры поиска:\n");
  printf("  Файл: %s\n", file);
  printf("  Использовать регулярные выражения: %s\n",
         options.use_regex ? "Да" : "Нет");
  printf("  Игнорировать регистр символов: %s\n",
         options.ignore_case ? "Да" : "Нет");
  printf("  Инвертировать совпадения: %s\n",
         options.invert_match ? "Да" : "Нет");
  printf("  Выводить только количество совпадающих строк: %s\n",
         options.count_only ? "Да" : "Нет");
  printf("  Выводить только имена файлов с совпадающими строками: %s\n",
         options.filenames_only ? "Да" : "Нет");
  printf("  Префикс каждой строки вывода номером строки: %s\n",
         options.line_number ? "Да" : "Нет");
  printf("  Подавить вывод сообщений об ошибках: %s\n",
         options.suppress_errors ? "Да" : "Нет");
  printf("  Рассматривать шаблон как файл и читать шаблоны из него: %s\n",
         options.pattern_from_file ? "Да" : "Нет");
  printf(
      "  Выводить только совпадающие (не пустые) части совпадающих строк: %s\n",
      options.output_only_match ? "Да" : "Нет");
}

void s21_grep(FILE* file, Options options, bool flag_few_files, char** argv,
              char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
              size_t size_array_of_regex) {
  char line[MAX_LINE_LENGTH];
  size_t number_of_matching_lines = 0;
  size_t number_of_lines = 0;
  bool flag_l = false;

  while (!flag_l && fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    size_t length = strlen(line);
    if (line[length - 1] == '\n') line[length - 1] = '\0';
    regmatch_t matches[MAX_MATCHES];
    size_t number_of_matches = 0;
    bool output_flag = false;
    number_of_lines++;

    find_matches(matches, &number_of_matches, line, array_of_regex,
                 size_array_of_regex, options.output_only_match,
                 options.ignore_case, &output_flag);
    // Обработка флага -o
    for (size_t i = 0; i < number_of_matches; i++) {
      if (i == 0 || matches[i].rm_so >= matches[i - 1].rm_eo) {
        if (flag_few_files && !options.ignore_filenames)
          printf("%s:", argv[optind]);
        if (options.line_number) printf("%zu:", number_of_lines);
        print_match(line, matches[i].rm_so, matches[i].rm_eo);
      }
    }

    if (options.invert_match) output_flag = !output_flag;
    if (options.count_only && output_flag) number_of_matching_lines++;
    if (options.filenames_only && output_flag) flag_l = true;
    if (output_flag && !options.count_only && !options.filenames_only &&
        !options.output_only_match) {
      if (flag_few_files && !options.ignore_filenames)
        printf("%s:", argv[optind]);
      if (options.line_number) printf("%zu:", number_of_lines);
      print_string(line, length);
    }
  }

  if (options.count_only) {
    if (flag_few_files && !options.ignore_filenames)
      printf("%s:", argv[optind]);
    printf("%zu\n", number_of_matching_lines);
  }

  if (options.filenames_only && flag_l) printf("%s\n", argv[optind]);
}

void find_matches(regmatch_t matches[], size_t* number_of_matches, char* line,
                  char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
                  size_t size_array_of_regex, bool flag_o, bool flag_i,
                  bool* output_flag) {
  size_t size_matches = MAX_MATCHES;
  regmatch_t pmatch;

  *output_flag = 0;

  for (size_t i = 0; i < size_array_of_regex; i++) {
    int eflag = 0;
    char* ptr = line;
    regoff_t prev_rm_so = 0;

    if (!strcmp(array_of_regex[i], "") ||
        (!flag_o && match(line, array_of_regex[i], flag_i, &pmatch, eflag))) {
      *output_flag = 1;
      break;
    }

    while (flag_o && match(ptr, array_of_regex[i], flag_i, &pmatch, eflag)) {
      ptr += pmatch.rm_eo;
      eflag = REG_NOTBOL;

      if (pmatch.rm_so != pmatch.rm_eo) {
        pmatch.rm_so += prev_rm_so;
        pmatch.rm_eo += prev_rm_so;

        prev_rm_so = pmatch.rm_eo;

        if (*number_of_matches < size_matches) {
          matches[*number_of_matches] = pmatch;
          (*number_of_matches)++;
        }

        if (*number_of_matches >= size_matches) {
          break;
        }
      }
    }
  }
}

bool reading_flags(int argc, char** argv,
                   char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
                   size_t* number_of_regex, Options* options) {
  int rez = 0;
  bool correct = 1;
  bool flag_empty = 0;

  while (correct &&
         (rez = getopt_long(argc, argv, "voice:slnhf:", NULL, NULL)) != -1) {
    process_flag(rez, optarg, array_of_regex, number_of_regex, options,
                 &correct, &flag_empty);
  }
  if (options->invert_match || options->count_only || options->filenames_only)
    options->output_only_match = 0;

  return correct;
}

void process_flag(int rez, char* optarg,
                  char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
                  size_t* number_of_regex, Options* options, bool* correct,
                  bool* flag_empty) {
  if (rez == 'e') {
    if (*number_of_regex < MAX_REGEX_COUNT) {
      strncpy(array_of_regex[*number_of_regex], optarg, MAX_REGEX_LENGTH);
      (*number_of_regex)++;
      update_flags(strlen(optarg), correct, flag_empty,
                   options->output_only_match);
      options->use_regex = 1;
    }
  }
  if (rez == 'i') options->ignore_case = 1;
  if (rez == 'v') options->invert_match = 1;
  if (rez == 'c') options->count_only = 1;
  if (rez == 'l') options->filenames_only = 1;
  if (rez == 'n') options->line_number = 1;
  if (rez == 'h') options->ignore_filenames = 1;
  if (rez == 's') options->suppress_errors = 1;
  if (rez == 'f') {
    FILE* file = fopen(optarg, "r+");
    if (!file) {
      if (!options->suppress_errors)
        printf("s21_grep: Не удалось открыть файл с шаблоном\n");
      *correct = 0;
    } else {
      read_regular_expressions(file, array_of_regex, number_of_regex,
                               options->output_only_match, flag_empty, correct);
      fclose(file);
    }
    options->pattern_from_file = 1;
  }
  if (rez == 'o') {
    options->output_only_match = 1;
  }
  if (rez == '?') *correct = 0;
}

void read_regular_expressions(
    FILE* file, char array_of_regex[MAX_REGEX_COUNT][MAX_REGEX_LENGTH],
    size_t* number_of_regex, bool flag_o, bool* flag_empty, bool* correct) {
  char regex[MAX_REGEX_LENGTH];
  while (fgets(regex, MAX_REGEX_LENGTH, file) != NULL) {
    size_t length = strlen(regex);
    if (regex[length - 1] == '\n') regex[length - 1] = '\0';
    if (*number_of_regex < MAX_REGEX_COUNT) {
      strncpy(array_of_regex[*number_of_regex], regex, MAX_REGEX_LENGTH);
      (*number_of_regex)++;
      update_flags(length, correct, flag_empty, flag_o);
    }
  }
}

void update_flags(size_t cond, bool* correct, bool* flag_empty, bool flag_o) {
  if (!cond) {
    *correct = !flag_o;
    *flag_empty = 1;
  }
}

void print_string(char* str, size_t size) {
  printf("%s", str);
  if (str[size - 1] != '\n') printf("\n");
}

char match(char* string, char* pattern, bool flag_icase, regmatch_t* pmatch,
           int eflag) {
  int status;
  regex_t re;
  int nmatch = (pmatch) ? 1 : 0;
  if (!(status = regcomp(&re, pattern, flag_icase ? REG_ICASE : 0)))
    status = regexec(&re, string, nmatch, pmatch, eflag);
  regfree(&re);
  //    printf("(status != 0) %d \n", (status != 0) ? 0 : 1);
  return (status != 0) ? 0 : 1;
}

void print_match(char* line, regoff_t start, regoff_t end) {
  for (regoff_t j = start; j < end; j++) {
    printf("%c", line[j]);
  }
  printf("\n");
}
