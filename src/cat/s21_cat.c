#include "s21_cat.h"

int main(int argc, char **argv) {
  options settings = {0};
  int file_count = 0, line_num = 1;

  if (!getOptions(argc, argv, &settings, &file_count)) {
    for (int i = optind; i < argc; i++) {
      printOutput(argv[i], settings, &line_num);
    }
  }

  return 0;
}

int getOptions(int argc, char **argv, options *settings, int *file_count) {
  int opt = 0;
  int opt_index = 0;
  int error = 0;

  while ((opt = getopt_long(argc, argv, "beEnstTv", long_options,
                            &opt_index)) != -1) {
    switch (opt) {
      case 'b':
        settings->number_nonblank = 1;
        break;
      case 'e':
        settings->show_ends = 1;
        settings->show_nonprinting = 1;
        break;
      case 'E':
        settings->show_ends = 1;
        break;
      case 'n':
        settings->number = 1;
        break;
      case 's':
        settings->squeeze_blank = 1;
        break;
      case 't':
        settings->show_tabs = 1;
        settings->show_nonprinting = 1;
        break;
      case 'T':
        settings->show_tabs = 1;
        break;
      case 'v':
        settings->show_nonprinting = 1;
        break;
      case '?':
        printf("invalid option -- %c\n", optopt);
        error = OPTION_ERR;
        break;
    }
  }

  *file_count = optind;

  return error;
}

int printOutput(const char *file_name, options settings, int *line_num) {
  int error = 0;
  FILE *file = fopen(file_name, "r");

  if (file == NULL) {
    printf("%s: No such file or directory\n", file_name);
    error = FILE_PATH_ERR;
  }

  int ch = 0, last_char = '\n', blank_line = 0;

  while (!error && (ch = fgetc(file)) != EOF) {
    int skip_line = 0;
    if (settings.squeeze_blank && ch == '\n') {
      skip_line = (blank_line > 1) ? 1 : (blank_line += 1, 0);
    } else {
      blank_line = 0;
    }

    if (!skip_line) {
      if (settings.number_nonblank && ch != '\n' && last_char == '\n') {
        printf("%6d\t", (*line_num)++);
      }
      if (settings.number && last_char == '\n' && !settings.number_nonblank) {
        printf("%6d\t", (*line_num)++);
      }
      if (settings.show_ends) {
        ch == 13 ? printf("^"), ch = 'M' : ch;
        ch == '\n' ? printf("$") : ch;
      }
      printNonPrinting(settings, ch);
      last_char = ch;
    }
  }

  if (file != NULL) {
    fclose(file);
  }

  return error;
}

void printNonPrinting(options settings, int ch) {
  if (settings.show_tabs && ch == '\t') {
    printf("^I");
  } else if (settings.show_nonprinting) {
    if (ch < 32 && ch != '\n' && ch != '\t') {
      printf("^%c", ch + 64);
    } else if (ch >= 128) {
      if (ch < 160) {
        printf("M-^%c", (ch - 128) + 64);
      } else {
        printf("M-%c", ch - 128);
      }
    } else if (ch == 127) {
      printf("^?");
    } else {
      putchar(ch);
    }
  } else {
    putchar(ch);
  }
}