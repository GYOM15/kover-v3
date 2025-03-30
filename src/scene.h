#ifndef SCENE_H_
#define SCENE_H_

#include <stdbool.h>

// Constants
// ---------

// The maximum length of an identifier
#define MAX_LENGTH_ID 10
// The maximum number of constructions in a scene
#define NUM_MAX_CONSTRUCTIONS 100
// The maximum number of antennas in a scene
#define NUM_MAX_ANTENNAS 100

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

// A scene
struct Scene {
  // The number of constructions in the scene
  unsigned int num_constructions;
  // The constructions of the scene
  struct Construction constructions[NUM_MAX_CONSTRUCTIONS];
  // The number of antennas in the scene
  unsigned int num_antennas;
  // The antennas of the scene
  struct Antenna antennas[NUM_MAX_ANTENNAS];
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
 * Returns the type of construction as a string
 *
 * @param construction  The construction
 * @return              The type
 */
const char* construction_type(const struct Construction* construction);

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
