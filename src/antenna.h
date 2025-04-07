#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <stdbool.h>

// Constants
// ---------

// The maximum length of an identifier (from scene.h)
#define MAX_LENGTH_ID 10

// Types
// -----

// An antenna
struct Antenna {
  // The identifier of the antenna
  char id[MAX_LENGTH_ID + 1];
  // The x coordinate of the antenna
  int x;
  // The y coordinate of the antenna
  int y;
  // The radius of the antenna
  int r;
};

// Functions
// ---------

/**
 * Indicates if two antennas have the same position
 *
 * @param antenna1  The first antenna
 * @param antenna2  The second antenna
 */
bool have_antennas_same_position(const struct Antenna* antenna1,
                               const struct Antenna* antenna2);

#endif