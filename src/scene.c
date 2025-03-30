#include "scene.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validation.h"

// Private constants, types and functions
// ======================================

// Constants
// ---------

// The maximum length of a line in a scene stream
#define MAX_LENGTH 50
// The maximum number of tokens in a line
#define MAX_NUM_TOKENS 6
// The maximum length of a token in a line
#define MAX_TOKEN_LENGTH 10

// Types
// -----

// A parsed line
struct ParsedLine {
  // The number of parsed token
  unsigned int num_tokens;
  // The tokens
  char tokens[MAX_NUM_TOKENS][MAX_TOKEN_LENGTH];
  // The number of the line
  int line_number;
};

// Validation
// ----------

/**
 * Indicates if two intervals are overlapping
 *
 * @param a1  The start of the first interval
 * @param b1  The end of the first interval
 * @param a2  The start of the second interval
 * @param b2  The end of the second interval
 */
bool are_intervals_overlapping(int a1, int b1, int a2, int b2) {
  return (a1 <= a2 && a2 < b1 && b1 <= b2) ||
         (a2 <= a1 && a1 < b2 && b2 <= b1);
}

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

/**
 * Indicates if two antennas have the same position
 *
 * @param antenna1  The first antenna
 * @param antenna2  The second antenna
 */
bool have_antennas_same_position(const struct Antenna* antenna1,
                                 const struct Antenna* antenna2) {
  return antenna1->x == antenna2->x && antenna1->y == antenna2->y;
}

/**
 * Checks if the constructions of a scene are valid.
 *
 * @param scene     The scene to validate
 * @param validate  Has the validate subcommand been invoked?
 */
void validate_constructions(const struct Scene* scene, bool validate) {
  for (unsigned int c1 = 0; c1 < scene->num_constructions; ++c1)
    for (unsigned int c2 = c1 + 1; c2 < scene->num_constructions; ++c2) {
      const struct Construction
        *construction1 = scene->constructions + c1,
        *construction2 = scene->constructions + c2;
      if (are_constructions_overlapping(construction1, construction2))
        report_error_overlapping_objects(
          construction_type(construction1), construction1->id,
          construction_type(construction2), construction2->id,
          validate);
    }
}

/**
 * Checks if the antennas of a scene are valid.
 *
 * @param scene     The scene to validate
 * @param validate  Has the validate subcommand been invoked?
 */
void validate_antennas(const struct Scene* scene, bool validate) {
  for (unsigned int a1 = 0; a1 < scene->num_antennas; ++a1)
    for (unsigned int a2 = a1 + 1; a2 < scene->num_antennas; ++a2) {
      const struct Antenna* antenna1 = scene->antennas + a1,
                          * antenna2 = scene->antennas + a2;
      if (have_antennas_same_position(antenna1, antenna2))
        report_error_same_position_antennas(antenna1->id, antenna2->id,
                                            validate);
    }
}

// Loading
// -------

/**
 * Indicates if the line is a valid "begin scene" line
 *
 * @param line  The line to check
 * @return      true if and only if the line is valid
 */
bool is_begin_scene_line(const char* line) {
  return strcmp(line, "begin scene") == 0;
}

/**
 * Indicates if the line is a valid "end scene" line
 *
 * @param line  The line to check
 * @return      true if and only if the line is valid
 */
bool is_end_scene_line(const char* line) {
  return strcmp(line, "end scene") == 0;
}

/**
 * Parses a line
 *
 * @param line         The line to parse
 * @param parsed_line  The resulting parsed line
 * @param line_number  The number of the line
 */
void parse_line(const char* line,
                struct ParsedLine* parsed_line,
                int line_number) {
  char line_copy[MAX_LENGTH + 1];
  strncpy(line_copy, line, MAX_LENGTH);
  char *token = strtok(line_copy, " ");
  int t = 0;
  while (token != NULL) {
    strncpy(parsed_line->tokens[t], token, MAX_TOKEN_LENGTH);
    ++t;
    token = strtok(NULL, " ");
  }
  parsed_line->num_tokens = t;
  parsed_line->line_number = line_number;
}

/**
 * Loads a construction from a parsed line
 *
 * @param parsed_line  The parsed line
 * @param scene        The scene in which the construction is loaded
 * @param validate     Has the subcommand validate been invoked?
 * @return             true if and only if the loading was successful
 */
bool load_construction_from_parsed_line(const struct ParsedLine* parsed_line,
                                        struct Scene* scene,
                                        bool validate) {
  if (strcmp(parsed_line->tokens[0], "building") != 0 &&
      strcmp(parsed_line->tokens[0], "house") != 0)
    return false;
  if (parsed_line->num_tokens != 6)
    report_error_line_wrong_arguments_number(parsed_line->tokens[0],
                                             parsed_line->line_number,
                                             validate);
  if (!is_valid_id(parsed_line->tokens[1]))
      report_error_invalid_identifier(parsed_line->tokens[1],
                                      parsed_line->line_number,
                                      validate);
  if (!is_valid_integer(parsed_line->tokens[2]))
      report_error_invalid_int(parsed_line->tokens[2],
                               parsed_line->line_number,
                               validate);
  if (!is_valid_integer(parsed_line->tokens[3]))
      report_error_invalid_int(parsed_line->tokens[3],
                               parsed_line->line_number,
                               validate);
  if (!is_valid_positive_integer(parsed_line->tokens[4]))
      report_error_invalid_positive_int(parsed_line->tokens[4],
                                        parsed_line->line_number,
                                        validate);
  if (!is_valid_positive_integer(parsed_line->tokens[5]))
      report_error_invalid_positive_int(parsed_line->tokens[5],
                                        parsed_line->line_number,
                                        validate);
  struct Construction construction;
  strncpy(construction.id, parsed_line->tokens[1], MAX_LENGTH_ID);
  construction.type = strcmp(parsed_line->tokens[0], "building") == 0 ?
    BUILDING : HOUSE;
  construction.x = atoi(parsed_line->tokens[2]);
  construction.y = atoi(parsed_line->tokens[3]);
  construction.w = atoi(parsed_line->tokens[4]);
  construction.h = atoi(parsed_line->tokens[5]);
  add_construction(scene, &construction, validate);
  return true;
}

/**
 * Loads an antenna from a parsed line
 *
 * @param parsed_line  The parsed line
 * @param scene        The scene in which the antenna is loaded
 * @param validate     Has the subcommand validate been invoked?
 * @return             true if and only if the loading was successful
 */
bool load_antenna_from_parsed_line(const struct ParsedLine* parsed_line,
                                   struct Scene* scene,
                                   bool validate) {
  if (strcmp(parsed_line->tokens[0], "antenna") != 0)
    return false;
  if (parsed_line->num_tokens != 5)
    report_error_line_wrong_arguments_number("antenna",
                                             parsed_line->line_number,
                                             validate);
  if (!is_valid_id(parsed_line->tokens[1]))
      report_error_invalid_identifier(parsed_line->tokens[1],
                                      parsed_line->line_number,
                                      validate);
  if (!is_valid_integer(parsed_line->tokens[2]))
      report_error_invalid_int(parsed_line->tokens[2],
                               parsed_line->line_number,
                               validate);
  if (!is_valid_integer(parsed_line->tokens[3]))
      report_error_invalid_int(parsed_line->tokens[3],
                               parsed_line->line_number,
                               validate);
  if (!is_valid_positive_integer(parsed_line->tokens[4]))
      report_error_invalid_positive_int(parsed_line->tokens[4],
                                        parsed_line->line_number,
                                        validate);
  struct Antenna antenna;
  strncpy(antenna.id, parsed_line->tokens[1], MAX_LENGTH_ID);
  antenna.x = atoi(parsed_line->tokens[2]);
  antenna.y = atoi(parsed_line->tokens[3]);
  antenna.r = atoi(parsed_line->tokens[4]);
  add_antenna(scene, &antenna, validate);
  return true;
}

// Public functions definition
// ===========================

// Construction
// ------------

void initialize_empty_scene(struct Scene* scene) {
  scene->num_constructions = 0;
  scene->num_antennas = 0;
}

void load_scene_from_stdin(struct Scene* scene, bool validate) {
  initialize_empty_scene(scene);
  char line[MAX_LENGTH + 1];
  bool first_line = true, last_line = false;
  int line_number = 1;
  while (fgets(line, MAX_LENGTH, stdin) != NULL) {
    last_line = false;
    line[strcspn(line, "\n")] = '\0';
    if (first_line) {
      if (!is_begin_scene_line(line))
        report_error_scene_first_line(validate);
      first_line = false;
    } else if (is_end_scene_line(line)) {
      last_line = true;
    } else {
      struct ParsedLine parsed_line;
      parse_line(line, &parsed_line, line_number);
      if (parsed_line.num_tokens == 0) {
        printf("not ok\n");
        fprintf(stderr, "error: line has no token\n");
        exit(1);
      }
      if (!load_construction_from_parsed_line(&parsed_line, scene, validate) &&
          !load_antenna_from_parsed_line(&parsed_line, scene, validate))
        report_error_unrecognized_line(line_number, validate);
    }
    ++line_number;
  }
  if (!last_line)
    report_error_scene_last_line(validate);
}

// Validation
// ----------

void validate_scene(const struct Scene* scene, bool validate) {
  validate_constructions(scene, validate);
  validate_antennas(scene, validate);
}

// Accessors
// ---------

bool scene_is_empty(const struct Scene* scene) {
  return scene->num_constructions == 0 && scene->num_antennas == 0;
}

int scene_num_buildings(const struct Scene* scene) {
  int num_buildings = 0;
  for (unsigned int c = 0; c < scene->num_constructions; ++c)
    num_buildings += scene->constructions[c].type == BUILDING;
  return num_buildings;
}

int scene_num_houses(const struct Scene* scene) {
  int num_houses = 0;
  for (unsigned int c = 0; c < scene->num_constructions; ++c)
    num_houses += scene->constructions[c].type == HOUSE;
  return num_houses;
}

const char* construction_type(const struct Construction* construction) {
  switch (construction->type) {
    case BUILDING: return "building";
    case HOUSE:    return "house";
  }
  return "??";
}

void print_scene_quality(const struct Scene* scene) {
  if (scene->num_constructions == 0) {
    puts("Nothing to report, scene contains no construction");
    return;
  }
  for (unsigned int c = 0; c < scene->num_constructions; ++c)
    printf("building %s: E\n", scene->constructions[c].id);
}

void print_scene_summary(const struct Scene* scene) {
  int num_buildings = scene_num_buildings(scene),
      num_houses = scene_num_houses(scene),
      num_antennas = scene->num_antennas;
  if (scene_is_empty(scene)) {
    puts("An empty scene");
    return;
  }
  printf("A scene with ");
  if (num_buildings > 0) {
    printf("%d building%s", num_buildings, num_buildings > 1 ? "s" : "");
    if (num_houses > 0 && num_antennas > 0)
      printf(", ");
    else if (num_houses > 0 || num_antennas > 0)
      printf(" and ");
  }
  if (num_houses > 0) {
    printf("%d house%s", num_houses, num_houses > 1 ? "s" : "");
    if (num_antennas > 0)
      printf(" and ");
  }
  if (num_antennas > 0)
    printf("%d antenna%s", num_antennas, num_antennas > 1 ? "s" : "");
  printf("\n");
}

void print_scene_constructions(const struct Scene* scene) {
  for (unsigned int c = 0; c < scene->num_constructions; ++c) {
    const struct Construction* construction = scene->constructions + c;
    printf("  %s %s at %d %d with dimensions %d %d\n",
           construction_type(construction),
           construction->id,
           construction->x, construction->y,
           construction->w, construction->h);
  }
}

void print_scene_antennas(const struct Scene* scene) {
  for (unsigned int a = 0; a < scene->num_antennas; ++a) {
    const struct Antenna* antenna = scene->antennas + a;
    printf("  antenna %s at %d %d with range %d\n",
           antenna->id, antenna->x, antenna->y, antenna->r);
  }
}

void print_scene_bounding_box(const struct Scene* scene) {
  if (scene_is_empty(scene)) {
    printf("undefined (empty scene)\n");
    return;
  }
  int xmin = INT_MAX, xmax = INT_MIN,
      ymin = INT_MAX, ymax = INT_MIN;
  for (unsigned int c = 0; c < scene->num_constructions; ++c) {
    int x = scene->constructions[c].x,
        y = scene->constructions[c].y,
        w = scene->constructions[c].w,
        h = scene->constructions[c].h;
    xmin = x - w < xmin ? x - w : xmin;
    xmax = x + w > xmax ? x + w : xmax;
    ymin = y - h < ymin ? y - h : ymin;
    ymax = y + h > ymax ? y + h : ymax;
  }
  for (unsigned int a = 0; a < scene->num_antennas; ++a) {
    int x = scene->antennas[a].x,
        y = scene->antennas[a].y,
        r = scene->antennas[a].r;
    xmin = x - r < xmin ? x - r : xmin;
    xmax = x + r > xmax ? x + r : xmax;
    ymin = y - r < ymin ? y - r : ymin;
    ymax = y + r > ymax ? y + r : ymax;
  }
  printf("bounding box [%d, %d] x [%d, %d]\n", xmin, xmax, ymin, ymax);
}

// Modifiers
// ---------

void add_construction(struct Scene* scene,
                      const struct Construction* construction,
                      bool validate) {
  unsigned int c = 0;
  while (c < scene->num_constructions &&
         strcmp(construction->id, scene->constructions[c].id) > 0)
    ++c;
  if (c < scene->num_constructions &&
      strcmp(construction->id, scene->constructions[c].id) == 0)
    report_error_non_unique_identifiers(construction_type(construction),
                                        construction->id, validate);
  for (unsigned int c2 = scene->num_constructions; c2 > c; --c2)
    scene->constructions[c2] = scene->constructions[c2 - 1];
  struct Construction* scene_construction = scene->constructions + c;
  strncpy(scene_construction->id, construction->id, MAX_LENGTH_ID);
  scene_construction->type = construction->type;
  scene_construction->x = construction->x;
  scene_construction->y = construction->y;
  scene_construction->w = construction->w;
  scene_construction->h = construction->h;
  ++scene->num_constructions;
}

void add_antenna(struct Scene* scene,
                 const struct Antenna* antenna,
                 bool validate) {
  unsigned int a = 0;
  while (a < scene->num_antennas &&
         strcmp(antenna->id, scene->antennas[a].id) > 0)
    ++a;
  if (a < scene->num_antennas &&
      strcmp(antenna->id, scene->antennas[a].id) == 0)
    report_error_non_unique_identifiers("antenna", antenna->id, validate);
  for (unsigned int a2 = scene->num_antennas; a2 > a; --a2)
    scene->antennas[a2] = scene->antennas[a2 - 1];
  struct Antenna* scene_antenna = scene->antennas + a;
  strncpy(scene_antenna->id, antenna->id, MAX_LENGTH_ID);
  scene_antenna->x = antenna->x;
  scene_antenna->y = antenna->y;
  scene_antenna->r = antenna->r;
  ++scene->num_antennas;
}
