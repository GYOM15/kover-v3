#include "construction.h"
#include <string.h>

// Function implementation
// -----------------------

const char* construction_type(const struct Construction* construction) {
  switch (construction->type) {
    case BUILDING: return "building";
    case HOUSE:    return "house";
  }
  return "??";
}

bool are_intervals_overlapping(int a1, int b1, int a2, int b2) {
  return (a1 <= a2 && a2 < b1 && b1 <= b2) ||
         (a2 <= a1 && a1 < b2 && b2 <= b1);
}

bool are_constructions_overlapping(const struct Construction* construction1,
                                 const struct Construction* construction2) {
  return are_intervals_overlapping(construction1->x - construction1->w,
                                 construction1->x + construction1->w,
                                 construction2->x - construction2->w,
                                 construction2->x + construction2->w) &&
         are_intervals_overlapping(construction1->y - construction1->h,
                                 construction1->y + construction1->h,
                                 construction2->y - construction2->h,
                                 construction2->y + construction2->h);
}