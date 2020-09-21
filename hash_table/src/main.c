#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "hashtable.h"
#include "hashtable_dev.h"


int main(void)
{
    int i;
    struct hashtable hashtable = hashtable_create();
    char *words[] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
        "ten",
        "eleven",
        "twelve",
        "thirteen",
        "fourteen",
        "fifteen",
        "sixteen",
        "seventeen",
        "eighteen",
        "nineteen",
        "twenty",
        "twenty-one",
        "twenty-two",
        "twenty-three",
        "twenty-four",
        "twenty-five",
        "twenty-six",
        "twenty-seven",
        "twenty-eight",
        "twenty-nine",
        "thirty"
    };
    for (i = 0; i < 30; ++i) {
        hashtable_put(&hashtable, words[i], i+1);
        hashtable_print(&hashtable);
    }

    hashtable_delete(&hashtable, "twenty-eight");
    hashtable_print(&hashtable);
    hashtable_delete(&hashtable, "thirty");
    hashtable_print(&hashtable);


    return 0;
}
