#include "modulo.h"

#define modulo(x, y) ((x % y + y) % y) // Fix negative modulo behavior