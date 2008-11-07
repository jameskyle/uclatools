#include "pd.h" /* for OLD ANMR format (ANMR_read_image() */

/*NEW*/
extern void APD2_read_image(char *fname, struct image *im, int image_num);

extern void ANMR_read_image(char *fname, struct image *im, 
			    unsigned int *NumRet,
			    void (*report)(struct displayHdr *anmr_header));
extern void report_header(struct displayHdr *h);
