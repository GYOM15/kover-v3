#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "validation.h"
#include "drawing.h"

// Constants
// ---------

// The help to display
#define HELP "Usage: kover SUBCOMMAND\n\
Handles positioning of communication antennas by reading a scene on stdin.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  bounding-box: returns a bounding box of the loaded scene\n\
  describe: describes the loaded scene in details\n\
  help: shows this message\n\
  quality: reports the cover quality of each construction\n\
  summarize: summarizes the loaded scene\n\
  validate: validates the loaded scene\n\
  draw: creates an SVG image of the scene (requires filename)\n\
\n\
A scene is a text stream that must satisfy the following syntax:\n\
\n\
  1. The first line must be exactly 'begin scene'\n\
  2. The last line must be exactly 'end scene'\n\
  3. Any line between the first and last line must either be a building line,\n\
     a house line or an antenna line\n\
  4. A building line has the form 'building ID X Y W H' (with any number of\n\
     blank characters before or after), where\n\
       ID is the building identifier\n\
       X is the x-coordinate of the building\n\
       Y is the y-coordinate of the building\n\
       W is the half-width of the building\n\
       H is the half-height of the building\n\
  5. A house line has the form 'house ID X Y W H' (with any number of\n\
     blank characters before or after), where\n\
       ID is the house identifier\n\
       X is the x-coordinate of the house\n\
       Y is the y-coordinate of the house\n\
       W is the half-width of the house\n\
       H is the half-height of the house\n\
  6. An antenna line has the form 'antenna ID X Y R' (with any number of\n\
     blank characters before or after), where\n\
       ID is the antenna identifier\n\
       X is the x-coordinate of the antenna\n\
       Y is the y-coordinate of the antenna\n\
       R is the radius scope of the antenna\n"

// Subcommands processing
// ----------------------

/**
 * Runs the bounding-box subcommand
 */
void run_bounding_box_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene, false);
  validate_scene(&scene, false);
  print_scene_bounding_box(&scene);
  scene_delete(&scene);
}

/**
 * Runs the describe subcommand
 */
void run_describe_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene, false);
  validate_scene(&scene, false);
  print_scene_summary(&scene);
  print_scene_constructions(&scene);
  print_scene_antennas(&scene);
  scene_delete(&scene);
}

/**
 * Runs the help subcommand
 */
void run_help_subcommand(void) {
  printf("%s", HELP);
}

/**
 * Runs the quality subcommand
 */
void run_quality_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene, false);
  validate_scene(&scene, false);
  print_scene_quality(&scene);
  scene_delete(&scene);
}

/**
 * Runs the summarize subcommand
 */
void run_summarize_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene, false);
  validate_scene(&scene, false);
  print_scene_summary(&scene);
  scene_delete(&scene);
}

/**
 * Runs the draw subcommand
 *
 * @param filename  The name of the SVG file to create
 */
void run_draw_subcommand(const char* filename) {
  struct Scene scene;
  load_scene_from_stdin(&scene, false);
  validate_scene(&scene, false);
  draw_scene_svg(&scene, filename);
  scene_delete(&scene);
}

/**
 * Runs the validate subcommand
 */
void run_validate_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene, true);
  validate_scene(&scene, true);
  printf("ok\n");
  scene_delete(&scene);
}

// Main function
// -------------

/**
 * Main function
 *
 * @param argc  The number of arguments
 * @param argv  The arguments
 */
int main(int argc, char* argv[]) {
  if (argc < 2)
    report_error_mandatory_subcommand();
  const char* subcommand = argv[1];
  if (strcmp(subcommand, "bounding-box") == 0)
    run_bounding_box_subcommand();
  else if (strcmp(subcommand, "describe") == 0)
    run_describe_subcommand();
  else if (strcmp(subcommand, "help") == 0)
    run_help_subcommand();
  else if (strcmp(subcommand, "quality") == 0)
    run_quality_subcommand();
  else if (strcmp(subcommand, "summarize") == 0)
    run_summarize_subcommand();
  else if (strcmp(subcommand, "validate") == 0)
    run_validate_subcommand();
  else if (strcmp(subcommand, "draw") == 0) {
    if (argc < 3)
      report_error_missing_filename("draw");
    run_draw_subcommand(argv[2]);
  }
  else
    report_error_unrecognized_subcommand(subcommand);
  return 0;
}
