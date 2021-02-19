#ifndef TUTORIALS_H
#define TUTORIALS_H

#define LINE_EXAMPLE(str, ntabs)    \
    for (int i = 0; i < ntabs; ++i) \
        printf ("\t");              \
    printf ("%s\n", #str);          \
    str

void Tutorial_BinShift ();
void Tutorial_BinShift_Casting ();
#endif /*TUTORIALS_H*/