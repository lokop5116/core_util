#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/openat2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

/* NOTE: This is only a very rudimentary imitation likely with a lot of issues.
 * Written purely for educational purposes. Any imrprovements to it are very
 * much welcome as is any constructive criticism.*/

int SHOW_HIDDEN = 0;

char show_help[] =
    "LS_Dupe - this is a ( attempt at ) rewriting the GNU ls util.\n"
    "Intended to work the same as GNU ls. Supports only -a flag. See "
    "usage below.\n"
    "\n"
    "ls [-a] [path]\n\n"
    "\t-a,\t do not ignore entries starting with '.'\n"
    "\t-h,\t display this help message\n";

int ListShit(const char *path);

void sort_strings(char *names[1024], int n);
int compare_strings(const void *a, const void *b);

int main(int argc, char *argv[]) {

  char *paths[16] = {"."};
  int iter = 0;
  int end = 1;

  // no path specified, display items in current directory
  if (argc == 1) {
    SHOW_HIDDEN = 0;
    paths[0] = ".";
  }

  // print help message when -h specified
  else if (argc == 2 && strcmp(argv[1], "-h") == 0) {
    printf("%s", show_help);
    return 0;
  }

  // go through specified flags, if -a present then show hiddens items ( entries
  // beginning with '.' ) add any paths to array of paths and list items in all
  // of them
  else {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-a") == 0) {
        SHOW_HIDDEN = 1;
      } else {
        paths[iter++] = strdup(argv[i]);
        end = iter;
      }
    }
  }

  // return from ls function
  int ret = 0;
  for (int i = 0; i < end; i++) {

    // printf("%s\n", paths[i]);

    printf("%s:\n", paths[i]);

    // add return code
    ret += ListShit(paths[i]);
    printf("\n\n");
  }

  // something went wrong
  if (ret > 0) {
    return 2;
  }

  return 0;
}

int ListShit(const char *path) {

  struct open_how how = {.flags = O_RDONLY, .mode = 0, .resolve = 0};

  char *names[1024]; /* if your directory has more than 1024 items then you
                        should really not be using this */
  int num = 0;

  // using openat2 as specified in manual
  long fd = syscall(SYS_openat2, AT_FDCWD, path, &how, sizeof(how));

  // fd less than 0, somethign went wrong while using openat2
  if (fd < 0) {
    printf("ls_dupe: fatal - %s", strerror(errno));
    return 1;
  }

  char buf[16384];

  // get entries
  int readbuf = syscall(SYS_getdents64, fd, buf, sizeof(buf));

  // something went wrong again
  if (readbuf < 0) {
    printf("ls_dupe: fatal - %s", strerror(errno));
  }

  struct dirent *entry;

  for (int pos = 0; pos < readbuf; 1) {

    // manually walk through the buffer
    entry = (struct dirent *)(buf + pos);

    if (SHOW_HIDDEN == 1 | entry->d_name[0] != '.') {

      names[num++] = strdup(entry->d_name);
      // printf("%s\n", entry->d_name);
    }
    pos = pos + entry->d_reclen;
  }

  close(fd);

  // sort our array or entries, not really necessary
  sort_strings(names, num);

  for (int i = 0; i < num; i++) {
    printf("%s\n", names[i]);
  }
  return 0;
}

void sort_strings(char *names[1024], int n) {
  qsort(names, n, sizeof(char *), compare_strings);
}

int compare_strings(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}
