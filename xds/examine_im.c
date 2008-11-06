/* Read the ANMR image header. Returns human-readable version of header file.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <assert_print.h>
#include <anmr/pd.h>

#define IMAGE_HEADER_OFFSET 12
#define IMAGE_DATA_OFFSET 80

#include <stdio.h>

main(int argc, char *argv[])
{ char *file_map;
  struct displayHdr *h;
  int i, anmr_filedes;

  for (i = 1; i < argc; i++) {
    anmr_filedes = open(argv[i], O_RDONLY);

    assert_print(anmr_filedes > 0, "Couldn't open file");
    file_map = (char *) 
      mmap(/*addr=*/ 0, IMAGE_HEADER_OFFSET+sizeof(struct displayHdr),
	   PROT_READ,MAP_SHARED, anmr_filedes, /*offset=*/0);
    h = (struct displayHdr *)  (file_map+IMAGE_HEADER_OFFSET);
    
    printf("\nANMR Header info for file %s:\n\
style=%hd, geSave=%d, anmrSave=%d, rotation=%d,reflectionX=%d, reflectionY=%d,\n\
scale=%f, shift=%f,\n\
displayX=%hd, displayY=%hd, slice=%hd, image=%hd, interImage=%d,\n\
echo=%hd, phase=%hd, numInterImages=%d,\n\
expand1=%lx, expand2=%lx, expand3=%lx, expand4=%lx;\n", argv[i],
	    h->style, h->geSave, h->anmrSave, h->rotation, 
	    h->reflectionX, h->reflectionY,
	    h->scale, h->shift,
	    h->displayX, h->displayY, h->slice, h->image, h->interImage,
	    h->echo, h->phase, h->numInterImages, 
	    h->expand1, h->expand2, h->expand3, h->expand4);
    
    munmap((caddr_t) h, sizeof(struct displayHdr));
    close(anmr_filedes);
  }
  
  exit(0);
}
