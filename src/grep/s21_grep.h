#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define OPTION_ERR 1
#define FILE_PATH_ERR 2

typedef struct {
  int ignore_case;
  int invert_match;
  int count_matches;
  int files_with_match;
  int line_number;
  int no_filename;
  int suppress_errors;
  int read_patterns_from_file;
  int only_matching;
  char *pattern_file;
} options;

static struct option long_options[] = {
    {"ignore-case", no_argument, NULL, 'i'},
    {"invert-match", no_argument, NULL, 'v'},
    {"count", no_argument, NULL, 'c'},
    {"files-with-matches", no_argument, NULL, 'l'},
    {"line-number", no_argument, NULL, 'n'},
    {"no-filename", no_argument, NULL, 'h'},
    {"no-messages", no_argument, NULL, 's'},
    {"only-matching", no_argument, NULL, 'o'},
    {"pattern-file", required_argument, NULL, 'f'},
    {NULL, 0, NULL, 0}};

int getOptions(int argc, char **argv, options *settings, char **pattern);
void processFile(const char *file_name, options settings, char *pattern);
int readFile(FILE *file, options settings, char *pattern,
             const char *file_name);
int matchPattern(char *line, options settings, char *pattern);
void printLine(char *line, options settings, const char *file_name,
               int line_num, char *pattern);
void printOnlyMatching(char *line, options settings, char *pattern);
void printUsage(options settings);
void printError(options settings, const char *message);

#endif