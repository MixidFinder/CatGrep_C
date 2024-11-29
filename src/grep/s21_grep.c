#include "s21_grep.h"

int main(int argc, char **argv) {
  options settings = {0};
  char *pattern = NULL;
  int error = 0;

  if (argc < 2) {
    printUsage(settings);
    error = 1;
  }

  error = getOptions(argc, argv, &settings, &pattern);

  if (!pattern && optind < argc && !error) pattern = argv[optind++];
  if (optind >= argc && !error) {
    printError(settings, "missing file");
    error = 1;
  } else if (argc - optind == 1 && !error) {
    settings.no_filename = 1;
  }

  if (!error) {
    for (int i = optind; i < argc; i++) {
      processFile(argv[i], settings, pattern);
    }
  }

  return error;
}

int getOptions(int argc, char **argv, options *settings, char **pattern) {
  int opt;
  int error = 0;

  while (error == 0 && (opt = getopt_long(argc, argv, "e:ivclnhsf:o",
                                          long_options, NULL)) != -1) {
    switch (opt) {
      case 'e':
        *pattern = optarg;
        break;
      case 'i':
        settings->ignore_case = 1;
        break;
      case 'v':
        settings->invert_match = 1;
        break;
      case 'c':
        settings->count_matches = 1;
        break;
      case 'l':
        settings->files_with_match = 1;
        break;
      case 'n':
        settings->line_number = 1;
        break;
      case 'h':
        settings->no_filename = 1;
        break;
      case 's':
        settings->suppress_errors = 1;
        break;
      case 'o':
        settings->only_matching = 1;
        break;
      case 'f':
        settings->read_patterns_from_file = 1;
        settings->pattern_file = optarg;
        break;
      case '?':
        printError(*settings, "Invalid option");
        error = 1;
        break;
    }
  }

  return error;
}

void processFile(const char *file_name, options settings, char *pattern) {
  FILE *file = fopen(file_name, "r");
  int found_match = 0;

  if (file) {
    found_match = readFile(file, settings, pattern, file_name);
    fclose(file);
  } else {
    printError(settings, file_name);
  }

  if (settings.files_with_match && found_match) {
    printf("%s\n", file_name);
  }
}

int readFile(FILE *file, options settings, char *pattern,
             const char *file_name) {
  char line[10000];
  int line_num = 0, match_count = 0, found_match = 0, flag = 0, is_match = 0;

  while (fgets(line, sizeof(line), file) && !flag) {
    line_num++;
    is_match = matchPattern(line, settings, pattern);
    if (is_match) {
      found_match = 1;
      match_count++;
      if (settings.files_with_match) flag = 1;
      if (!settings.count_matches && !flag) {
        printLine(line, settings, file_name, line_num, pattern);
      }
    }
  }

  if (!settings.files_with_match) {
    if (settings.count_matches) {
      printf("%d\n", match_count);
    }
  }

  return found_match;
}

int matchPattern(char *line, options settings, char *pattern) {
  regex_t regex = {0};
  int match = 0;

  if (regcomp(&regex, pattern,
              REG_EXTENDED | (settings.ignore_case ? REG_ICASE : 0)) == 0) {
    match = regexec(&regex, line, 0, NULL, 0) == 0;
    match = settings.invert_match ? !match : match;
    regfree(&regex);
  } else {
    printError(settings, "Regular error");
  }

  return match;
}

void printLine(char *line, options settings, const char *file_name,
               int line_num, char *pattern) {
  if (!settings.no_filename && !settings.files_with_match)
    printf("%s:", file_name);
  if (settings.line_number && !settings.files_with_match)
    printf("%d:", line_num);

  if (settings.only_matching) {
    printOnlyMatching(line, settings, pattern);
  } else {
    printf("%s", line);
    if (line[strlen(line) - 1] != '\n') printf("\n");
  }
}

void printOnlyMatching(char *line, options settings, char *pattern) {
  regex_t regex = {0};

  if (regcomp(&regex, pattern,
              REG_EXTENDED | (settings.ignore_case ? REG_ICASE : 0)) == 0) {
    regmatch_t pmatch[1];
    while (regexec(&regex, line, 1, pmatch, 0) == 0) {
      printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
             line + pmatch[0].rm_so);
      line += pmatch[0].rm_eo;
    }
    regfree(&regex);
  } else {
    printError(settings, "Regular error");
  }
}

void printUsage(options settings) {
  if (!settings.suppress_errors)
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
}

void printError(options settings, const char *message) {
  if (!settings.suppress_errors) fprintf(stderr, "s21_grep: %s\n", message);
}
