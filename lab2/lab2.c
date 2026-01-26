#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *command = 0;
  size_t len = 0;

  while (1) {
    printf("Enter program to run.\n > ");

    if (getline(&command, &len, stdin) == -1) {
      break;
    }
    command[strcspn(command, "\n")] = '\0';

    pid_t pid = fork();
    if (pid == 0) {
      execl(command, command, (char *)NULL);
      printf("Exec failure\n");
      exit(1);
    } else if (pid > 0) {
      waitpid(pid, NULL, 0);
    } else {
      printf("Fork failed\n");
    }
  }

  free(command);
  return 0;
}
