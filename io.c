#include "io.h"
int read_word(unsigned max, char s[max], FILE * f){
    static int flag_over = 0;
    int c;
    unsigned counter = 0;
    int too_long = 0;
    while ((c = fgetc(f)) != EOF && isspace(c));
    if (c == EOF) {
            return EOF;
    }
    if (counter < max - 1) {
            s[counter++] = c;
        }else {
            too_long = 1;
            if (!flag_over) {
                fprintf(stderr, "word too long\n");
                flag_over = 1;
            }
        }
    while ((c = fgetc(f)) != EOF && !isspace(c)) {
        if(!too_long){
            if (counter < max - 1) {
                            s[counter++] = c;
            }else {
                flag_over = 1;
                if (!flag_over) {
                    fprintf(stderr, "word too long\n");
                    flag_over = 1;
                    }
            }
        }
    }
    s[counter] = '\0';
    return counter;

}
