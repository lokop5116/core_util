#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/openat2.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(void) {

  struct open_how how = {.flags = O_RDONLY, .mode = 0, .resolve = 0};

  long fd = syscall(SYS_openat2, AT_FDCWD, ".", &how, sizeof(how));

  if (fd < 0) {
    perror("openat2");
    return 1;
  }

  char buf[1024];

  int readbuf = syscall(SYS_getdents64, fd, buf, sizeof(buf));

  if (readbuf < 0) {
    printf("oopsies something went wrong: %s", strerror(errno));
  }

  struct dirent *entry;

  for (int pos = 0; pos < readbuf; 1) {

    entry = (struct dirent *)(buf + pos);
    printf("%s\t%d\n", entry->d_name, entry->d_reclen);

    pos = pos + entry->d_reclen;
  }

  close(fd);

  return 0;
}
