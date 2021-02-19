#include "Tutorials.h"

#include <stdio.h>
#include <stdint.h>
#include "binlib.h"

void Tutorial_BinShift () {
    char temp_scanf = '\0';
    
    printf ("Hello everyone.\nIn this program, I'll show you "
            "that you need to be very careful when using shifts in C. \n");
    
    printf ("Let's start?\n");
    scanf ("%c", &temp_scanf);

    // 1 ---------

    printf ("First simple example:\n\n");
    LINE_EXAMPLE (uint8_t byte = 0b11000000;, 0)

    printf ("\nWhat do you think the (%s) is equal to?\n", "byte >> 1");
    scanf ("%c", &temp_scanf);

    printf ("(%s) is ", "byte >> 1");
    print_byte_nl (byte >> 1);

    printf ("\nIf everything is clear here, then let's go further.\n");
    scanf ("%c", &temp_scanf);

    // 2 ---------

    printf ("Second example:\n\n");
    LINE_EXAMPLE (byte = 0b11111111;, 0)
    printf ("\n(byte << 6) == ?");

    scanf ("%c", &temp_scanf);

    printf ("(byte << 6) == ");
    print_byte (byte << 6);
    printf (" <-- It is good. Quite understandable\n");

    scanf ("%c", &temp_scanf);

    printf ("But: (byte << 6) >> 1 == ?");
    scanf ("%c", &temp_scanf);
    
    printf ("Are you thinking (byte << 6) >> 1 == (1100 0000) >> 1 == 0110 0000 ?\n");
    scanf ("%c", &temp_scanf);

    printf ("No))\n");
    scanf ("%c", &temp_scanf);

    printf ("(byte << 6) >> 1 == ");
    print_byte ((byte << 6) >> 1);
    printf (" <-- How? Where did another 1 come from, in case we have an unsigned number?");
    scanf ("%c", &temp_scanf);

    printf ("The fact is that (byte << 6) in ((byte << 6) >> 1) turned into int," 
            "and therefore the symbolic unit remained.\n");
    scanf ("%c", &temp_scanf);

    printf ("Let's convert () to unsigned:\n\n"
            "(uint8_t)(byte << 6) >> 1 == ?\n");
    scanf ("%c", &temp_scanf);
    printf ("(uint8_t)(byte << 6) >> 1 == ");
    print_byte ((uint8_t)(byte << 6) >> 1);
    printf (" <-- That's what we need!\ns");

    // 3 ---------
}

void Tutorial_BinShift_Casting () {
    char temp_scanf = '\0';

	printf ("Hello everyone!\n"
			"Now we will see how type casting\n"
			"affects the bitwise shift operation.\n");
	
	printf ("Simple:\n0xFF >> 2: ");
	print_byte (0xFF >> 2);
    printf ("\n");
    scanf ("%c", &temp_scanf);

    LINE_EXAMPLE (uint8_t ubyte = 0xFF;, 0);
    printf ("ubyte >> 2: ");
    print_byte (ubyte >> 2);
    printf ("\n");
    scanf ("%c", &temp_scanf);

    LINE_EXAMPLE (int8_t sbyte = 0xFF;, 0);
    printf ("sbyte >> 2: ");
    print_byte (sbyte >> 2);
    printf ("\n");
    scanf ("%c", &temp_scanf);

    printf ("Okey. Let's add type casting:\n");
    scanf ("%c", &temp_scanf);

	printf ("Simple:\n(int8_t) 0xFF >> 2: ");
	print_byte ((int8_t) 0xFF >> 2);
    printf (" <-- Wow! Amazing, the casting work!\n");
    scanf ("%c", &temp_scanf);

    LINE_EXAMPLE (ubyte = 0xFF;, 0);
    printf ("(int8_t) ubyte >> 2: ");
    print_byte ((int8_t) ubyte >> 2);
    printf ("\n");
    scanf ("%c", &temp_scanf);

    LINE_EXAMPLE (sbyte = 0xFF;, 0);
    printf ("(uint8_t) sbyte >> 2: ");
    print_byte ((uint8_t) sbyte >> 2);
    printf ("\n");
    scanf ("%c", &temp_scanf);

    printf ("Types casting works!\n");
}