#include <iostream>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(handle) setmode(handle, O_BINARY)
#else
# define SET_BINARY_MODE(handle) ((void)0)
#endif

#include "discohash.h"

const int SIZE = 1024;

long getSizeOfInput(FILE *input, char **buf){
  char *nb;
  long retvalue = 0;
  long size = SIZE;
  int c;

  if (input != stdin) {
    if (-1 == fseek(input, 0L, SEEK_END)) {
      fprintf(stderr, "Error seek end: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
    if (-1 == (retvalue = ftell(input))) {
      fprintf(stderr, "ftell failed: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
    if (-1 == fseek(input, 0L, SEEK_SET)) {
      fprintf(stderr, "Error seek start: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
    size = retvalue;
    nb = (char *)realloc(buf[0], size);
    if ( nb == NULL ) {
      free(buf[0]);
      printf("Out of memory");
      exit(EXIT_FAILURE);
    }
    buf[0] = nb;
    fread(buf[0], 1, retvalue, input);
    fclose(input);
  } else {
    while (EOF != (c = fgetc(input))) {
      if ( retvalue >= size-1 ) {
        size *= 1.618;
        nb = (char *)realloc(buf[0], size);
        if ( nb == NULL ) {
          free(buf[0]);
          printf("Out of memory");
          exit(EXIT_FAILURE);
        }
        buf[0] = nb;
      }
      buf[0][retvalue] = c;
      retvalue++;
    } 
  }

  //printf("Size %ld\n", size);

  return retvalue;
}

int main(int argc, char **argv) {
  FILE *input;
  long size = SIZE;
  char *buf[1];
  buf[0] = (char *)malloc(size);
  if ( buf[0] == NULL ) {
    printf("Out of memory");
    exit(EXIT_FAILURE);
  }
  uint8_t hash[8] = {0};
  uint64_t *H = (uint64_t *)hash;

  if (argc > 1) {
    if(!strcmp(argv[1],"-")) {
       input = stdin;
    } else {
       input = fopen(argv[1],"rb");
       if (NULL == input) {
          fprintf(stderr, "Unable to open '%s': %s\n",
                argv[1], strerror(errno));
          exit(EXIT_FAILURE);
       }
    }
  } else {
    input = stdin;
    freopen(NULL, "rb", stdin);
  }

  SET_BINARY_MODE(input);

  size = getSizeOfInput(input, buf);

  //printf("OK %.*s\n", size, buf[0]);

  //printf("Size of file: %ld\n", size);

  BEBB4185_64(buf[0], size, 0, hash);

  printf("%#018" PRIx64 "\n", H[0]);

  //BEBB4185_64(buf, size, 0, hash);

  //printf("Hash %#018" PRIx64 "\n", H[0]);

  free(buf[0]);

  return EXIT_SUCCESS;
}
