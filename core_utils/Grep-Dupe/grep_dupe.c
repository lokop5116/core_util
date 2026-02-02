#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {

  if (argc < 2) {

    fprintf(stderr, "Usage: %s pattern [file]\n", argv[0]);
    return 1;
  }

  char *pattern = argv[1];

  FILE *fp = (argc > 2) ? fopen(argv[2], "r") : stdin;

  if (!fp) {
    perror("fopen");
    return 1;
  }

  char line[MAX_LINE];

  while (fgets(line, sizeof(line), fp)) {

    if (strstr(line, pattern)) {
      fputs(line, stdout);
    }
  }

  if (fp != stdin)
    fclose(fp);

  return 0;
}
