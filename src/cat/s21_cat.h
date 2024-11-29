#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTION_ERR 1
#define FILE_PATH_ERR 2

typedef struct {
  int number_nonblank;
  int show_ends;
  int number;
  int squeeze_blank;
  int show_tabs;
  int show_nonprinting;
} options;

static struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"show-ends", no_argument, NULL, 'E'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}};

int getOptions(int argc, char **argv, options *settings, int *file_count);
int printOutput(const char *file_name, options settings, int *line_num);
void printNonPrinting(options settings, int ch);

#endif