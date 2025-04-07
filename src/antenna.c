#include "antenna.h"
#include <string.h>

// Function implementation
// -----------------------

bool have_antennas_same_position(const struct Antenna* antenna1,
                               const struct Antenna* antenna2) {
  return antenna1->x == antenna2->x && antenna1->y == antenna2->y;
}