#include <stdio.h>
main()
{ unsigned short int x;
  while (fread(&x,sizeof(short),1,stdin)) {
    x = x>>4;
    fwrite(&x,sizeof(short),1,stdout);
  }
}
