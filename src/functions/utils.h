#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types/constants.h"

void clearBuffer() {
  char c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void systemPause() {
  printf("Pressione enter para continuar... ");
  getchar();
}

void inputString(char *string, int max) {
  unsigned int i = 0;

  fgets(string, max, stdin);

  do {
    if (string[i] == '\n') {
      string[i] = '\0';
      break;
    }
    i++;
  } while (i < strlen(string));
}

  #ifdef WIN32

    void clearScreen(void) {
      system("cls");
    }

  #else

    void clearScreen(void) {
      system("clear");
    }

  #endif


#endif
