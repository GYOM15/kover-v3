#ifndef CONSTRUCTION_H_
#define CONSTRUCTION_H_

#include <stdbool.h>

// Constants
// ---------

// The maximum length of an identifier (from scene.h)
#define MAX_LENGTH_ID 10

// Types
// -----

// A type of construction
enum ConstructionType {
  // A building
  BUILDING,
  // A house
  HOUSE
};

// A construction
struct Construction {
  // The identifier of the construction
  char id[MAX_LENGTH_ID + 1];
  // The type of the construction
  enum ConstructionType type;
  // The x coordinate of the construction
  int x;
  // The y coordinate of the construction
  int y;
  // The half width of the construction (half length in x direction)
  int w;
  // The half height of the construction (half length in y direction)
  int h;
};

// Functions
// ---------

/**
 * Returns the type of construction as a string
 *
 * @param construction  The construction
 * @return              The type
 */
const char* construction_type(const struct Construction* construction);

/**
 * Indicates if two intervals are overlapping
 *
 * @param a1  The start of the first interval
 * @param b1  The end of the first interval
 * @param a2  The start of the second interval
 * @param b2  The end of the second interval
 */
bool are_intervals_overlapping(int a1, int b1, int a2, int b2);

/**
 * Indicates if two constructions are overlapping
 *
 * Two constructions are overlapping if their intersection has a strictly
 * positive area.
 *
 * @param construction1  The first construction
 * @param construction2  The second construction
 */
bool are_constructions_overlapping(const struct Construction* construction1,
                                 const struct Construction* construction2);

#endif