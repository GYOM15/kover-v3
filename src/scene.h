#ifndef SCENE_H_
#define SCENE_H_

#include <stdbool.h>
#include "antenna.h"
#include "construction.h"

// Types
// -----

// A scene
struct Scene {
  // The number of constructions in the scene
  unsigned int num_constructions;
  // The capacity of constructions in the scene
  unsigned int capacity_constructions;
  // The constructions of the scene
  struct Construction* constructions;
  // The number of antennas in the scene
  unsigned int num_antennas;
  // The capacity of antennas in the scene
  unsigned int capacity_antennas;
  // The antennas of the scene
  struct Antenna* antennas;
};

// Construction
// ------------

/**
 * Initializes an empty scene
 *
 * @param scene  The scene to initialize
 */
void initialize_empty_scene(struct Scene* scene);

/**
 * Frees the memory allocated for a scene
 *
 * @param scene  The scene to free
 */
void scene_delete(struct Scene* scene);

/**
 * Loads a scene from the standard input
 *
 * @param scene     The resulting scene
 * @param validate  Has the subcommand validate been invoked?
 */
void load_scene_from_stdin(struct Scene* scene, bool validate);

// Validation
// ----------

/**
 * Checks if a scene is valid
 *
 * If the scene is invalid, an error is printed on stdout and the program exits
 * with 1.
 *
 * @param scene     The scene to validate
 * @param validate  Has the subcommand validate been invoked?
 */
void validate_scene(const struct Scene* scene, bool validate);

// Accessors
// ---------

/**
 * Indicates if a scene is empty
 *
 * @param scene  The scene to check
 * @return       true if and only if the given scene is empty
 */
bool scene_is_empty(const struct Scene* scene);

/**
 * Returns the number of buildings in a scene
 *
 * @param scene  The scene
 * @return       The number of buildings
 */
int scene_num_buildings(const struct Scene* scene);

/**
 * Returns the number of houses in a scene
 *
 * @param scene  The scene
 * @return       The number of houses
 */
int scene_num_houses(const struct Scene* scene);

/**
 * Prints the quality of the scene to stdout
 *
 * @param scene  The scene whose quality is printed
 */
void print_scene_quality(const struct Scene* scene);

/**
 * Prints a summary of the scene on stdout
 *
 * @param scene  The scene to print
 */
void print_scene_summary(const struct Scene* scene);

/**
 * Prints the construction of the scene to stdout
 *
 * @param scene  The scene whose constructions are printed
 */
void print_scene_constructions(const struct Scene* scene);

/**
 * Prints the antenna of the scene to stdout
 *
 * @param scene  The scene whose antennas are printed
 */
void print_scene_antennas(const struct Scene* scene);

/**
 * Prints the bounding box of the scene to stdout
 *
 * @param scene  The scene whose bounding box is printed
 */
void print_scene_bounding_box(const struct Scene* scene);

// Modifiers
// ---------

/**
 * Adds a construction to a scene
 *
 * @param scene         The scene to which the construction is added
 * @param construction  The construction to add
 * @param validate      Has the validate subcommand been invoked?
 */
void add_construction(struct Scene* scene,
                      const struct Construction* construction,
                      bool validate);

/**
 * Adds an antenna to a scene
 *
 * @param scene     The scene to which the antenna is added
 * @param antenna   The antenna to add
 * @param validate  Has the validate subcommand been invoked?
 */
void add_antenna(struct Scene* scene,
                 const struct Antenna* antenna,
                 bool validate);

#endif