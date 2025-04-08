#ifndef DRAWING_H_
#define DRAWING_H_

#include "scene.h"

/**
 * Draws a scene as an SVG file
 *
 * @param scene     The scene to draw
 * @param filename  The name of the SVG file to create
 */
void draw_scene_svg(const struct Scene* scene, const char* filename);

#endif