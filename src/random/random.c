#include "random.h"

#include <stdlib.h>

// Returns a random number between `min` and `max` (both included)
int random_number(int min, int max)
{
    return min + (rand() % (max - min + 1));
}