#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char * argv[]) {
  if (argc < 2) {
    printf("Usage: %s [filename]", argv[0]);
    exit(EXIT_FAILURE);
  }
  FILE * bmpstream = fopen(argv[1], "r");
  char encoding[2] = "";
  fread(encoding, 1, 2, bmpstream);
  if (!(encoding[0] == 'B' && encoding[1] == 'M')) {
    printf("%s is not a BMP file.\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  // Get size of BMP file (specified in file itself).
  int size = 0;
  fread(&size, 1, 4, bmpstream);
  printf("Size of file is: %d\n", size);
  // Throw away application-specific values
  fseek(bmpstream, 10, SEEK_SET);
  int begoffset = 0;
  fread(&begoffset, 1, 4, bmpstream);
  fseek(bmpstream, begoffset, SEEK_SET);
  char * bitmap = NULL;
  bitmap = malloc(size);
  memset(bitmap, 0, size);
  int realsize = 0;
  realsize = fread(bitmap, 1, size, bmpstream);
  fclose(bmpstream);
  char * data = malloc((int)realsize/9+1);
  memset(data, 0, (int)realsize/9+1);
  char letter = 0;
  for (int i = 0, dloc = 0; i < realsize; i++) {
    if ((i+1) % 9 == 0) {
      data[dloc] = letter;
      dloc++;
      letter = 0;
    } else {
      if (bitmap[i] & 1 == 1) letter |= (1 << 7) >> ((i+1)%9)-1;
    }
  }
  printf("%s\n", data);
  free(data);
  free(bitmap);
  exit(EXIT_SUCCESS);
}
