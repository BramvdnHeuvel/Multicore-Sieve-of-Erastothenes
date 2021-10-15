#include <stdio.h>
#include "initialize.c"

int main(void) {
    int totients[48] = {2, 6, 12, 18, 30, 42, 60, 66, 90, 120, 126, 150, 210, 240, 270, 330, 420, 462, 510, 630, 660, 690, 840, 870, 1050, 1260, 1320, 1470, 1680, 1890, 2310, 2730, 2940, 3150, 3570, 3990, 4620, 4830, 5460, 5610, 5670, 6090, 6930, 7140, 7350, 8190, 9240, 9660};
    
    for (int i=1; i<48; i++) {
        int x       = totients[i];
        int phi_x   = phi(x);

        printf("%d\t\t%d->\t%d\t%f\t%f\n", i, x, phi_x, x/((float)phi_x), ((float)phi_x)/x);
    }
}