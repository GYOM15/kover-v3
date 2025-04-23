#include "drawing.h"

#include <cairo.h>
#include <cairo-svg.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "antenna.h"
#include "construction.h"

// Constants
// ---------

// The padding around the scene in pixels
#define PADDING 50
// The width of the SVG in pixels
#define WIDTH 800
// The height of the SVG in pixels
#define HEIGHT 600
// The radius of the antenna center point
#define ANTENNA_CENTER_RADIUS 2
// Font size for labels
#define FONT_SIZE 12
// Grid line width
#define GRID_LINE_WIDTH 0.5
// Construction stroke width
#define CONSTRUCTION_STROKE_WIDTH 1.5
// Antenna stroke width
#define ANTENNA_STROKE_WIDTH 1.5

// Constantes for les couleurs
// RGB for building (blue)
#define BUILDING_FILL_R 0.7
#define BUILDING_FILL_G 0.7
#define BUILDING_FILL_B 0.95
#define BUILDING_FILL_A 0.5

// RGB for building stroke (blue)
#define BUILDING_STROKE_R 0.0
#define BUILDING_STROKE_G 0.0
#define BUILDING_STROKE_B 1.0
#define BUILDING_STROKE_A 1.0

// RGB for house (mauve - light purple)
#define HOUSE_FILL_R 0.8
#define HOUSE_FILL_G 0.7
#define HOUSE_FILL_B 0.9
#define HOUSE_FILL_A 0.5

// RGB for house stroke (dark purple)
#define HOUSE_STROKE_R 0.5
#define HOUSE_STROKE_G 0.0
#define HOUSE_STROKE_B 0.7
#define HOUSE_STROKE_A 1.0

// RGB for antenna (Green)
#define ANTENNA_FILL_R 0.7
#define ANTENNA_FILL_G 0.95
#define ANTENNA_FILL_B 0.7
#define ANTENNA_FILL_A 0.3

// RGB for antenna stroke (Green)
#define ANTENNA_STROKE_R 0.0
#define ANTENNA_STROKE_G 0.7
#define ANTENNA_STROKE_B 0.0
#define ANTENNA_STROKE_A 1.0

// RGB for grid lines (gray clair)
#define GRID_LINE_R 0.85
#define GRID_LINE_G 0.85
#define GRID_LINE_B 0.85
#define GRID_LINE_A 1.0

// RGB for frame (black)
#define FRAME_R 0.0
#define FRAME_G 0.0
#define FRAME_B 0.0

// RGB for text (black)
#define TEXT_R 0.0
#define TEXT_G 0.0
#define TEXT_B 0.0

// Private functions
// ================

/**
 * Calculate the bounding box of a scene
 *
 * @param scene  The scene
 * @param xmin   Pointer to store minimum x
 * @param ymin   Pointer to store minimum y
 * @param xmax   Pointer to store maximum x
 * @param ymax   Pointer to store maximum y
 */
static void calculate_bounding_box(const struct Scene *scene,
                                   int *xmin, int *ymin,
                                   int *xmax, int *ymax)
{
    *xmin = INT_MAX;
    *ymin = INT_MAX;
    *xmax = INT_MIN;
    *ymax = INT_MIN;

    if (scene_is_empty(scene))
    {
        *xmin = -5;
        *ymin = -5;
        *xmax = 5;
        *ymax = 5;
        return;
    }

    for (unsigned int c = 0; c < scene->num_constructions; ++c)
    {
        int x = scene->constructions[c].x,
            y = scene->constructions[c].y,
            w = scene->constructions[c].w,
            h = scene->constructions[c].h;
        *xmin = x - w < *xmin ? x - w : *xmin;
        *xmax = x + w > *xmax ? x + w : *xmax;
        *ymin = y - h < *ymin ? y - h : *ymin;
        *ymax = y + h > *ymax ? y + h : *ymax;
    }

    for (unsigned int a = 0; a < scene->num_antennas; ++a)
    {
        int x = scene->antennas[a].x,
            y = scene->antennas[a].y,
            r = scene->antennas[a].r;
        *xmin = x - r < *xmin ? x - r : *xmin;
        *xmax = x + r > *xmax ? x + r : *xmax;
        *ymin = y - r < *ymin ? y - r : *ymin;
        *ymax = y + r > *ymax ? y + r : *ymax;
    }
}

/**
 * Draw the grid for the scene
 *
 * @param cr      The Cairo context
 * @param xmin    Minimum x coordinate
 * @param ymin    Minimum y coordinate
 * @param xmax    Maximum x coordinate
 * @param ymax    Maximum y coordinate
 * @param scale   Scale factor for drawing
 */
static void draw_grid(cairo_t *cr,
                      int xmin, int ymin,
                      int xmax, int ymax,
                      double scale)
{
    // Set grid line properties
    cairo_set_line_width(cr, GRID_LINE_WIDTH);
    cairo_set_source_rgba(cr, GRID_LINE_R, GRID_LINE_G, GRID_LINE_B, GRID_LINE_A);

    // Draw vertical grid lines
    cairo_set_dash(cr, (double[]){2.0, 2.0}, 2, 0);
    for (int x = xmin; x <= xmax; ++x)
    {
        cairo_move_to(cr, PADDING + (x - xmin) * scale, PADDING);
        cairo_line_to(cr, PADDING + (x - xmin) * scale, PADDING + (ymax - ymin) * scale);
        cairo_stroke(cr);

        // Draw x-axis labels
        char label[32];
        sprintf(label, "%d", x);
        cairo_set_source_rgb(cr, TEXT_R, TEXT_G, TEXT_B);
        cairo_move_to(cr, PADDING + (x - xmin) * scale - 3, PADDING + (ymax - ymin) * scale + 15);
        cairo_show_text(cr, label);

        // Draw second x-axis labels at the top
        cairo_move_to(cr, PADDING + (x - xmin) * scale - 3, PADDING - 5);
        cairo_show_text(cr, label);
    }

    // Draw horizontal grid lines
    for (int y = ymin; y <= ymax; ++y)
    {
        cairo_move_to(cr, PADDING, PADDING + (ymax - y) * scale);
        cairo_line_to(cr, PADDING + (xmax - xmin) * scale, PADDING + (ymax - y) * scale);
        cairo_stroke(cr);

        // Draw y-axis labels
        char label[32];
        sprintf(label, "%d", y);
        cairo_set_source_rgb(cr, TEXT_R, TEXT_G, TEXT_B);
        cairo_move_to(cr, PADDING - 20, PADDING + (ymax - y) * scale + 5);
        cairo_show_text(cr, label);

        // Draw second y-axis labels at the right
        cairo_move_to(cr, PADDING + (xmax - xmin) * scale + 5, PADDING + (ymax - y) * scale + 5);
        cairo_show_text(cr, label);
    }
}

/**
 * Draw the frame for the scene
 *
 * @param cr      The Cairo context
 * @param xmin    Minimum x coordinate
 * @param ymin    Minimum y coordinate
 * @param xmax    Maximum x coordinate
 * @param ymax    Maximum y coordinate
 * @param scale   Scale factor for drawing
 */
static void draw_frame(cairo_t *cr,
                       int xmin, int ymin,
                       int xmax, int ymax,
                       double scale)
{
    // Set frame properties
    cairo_set_line_width(cr, 2.0);
    cairo_set_source_rgb(cr, FRAME_R, FRAME_G, FRAME_B);
    cairo_set_dash(cr, NULL, 0, 0);

    // Draw the frame rectangle
    cairo_rectangle(cr,
                    PADDING, PADDING,
                    (xmax - xmin) * scale, (ymax - ymin) * scale);
    cairo_stroke(cr);
}

/**
 * Draw a building in the scene
 *
 * @param cr            The Cairo context
 * @param construction  The construction to draw
 * @param xmin          Minimum x coordinate of the scene
 * @param ymax          Maximum y coordinate of the scene
 * @param scale         Scale factor for drawing
 */
static void draw_building(cairo_t *cr,
                          const struct Construction *construction,
                          int xmin, int ymax,
                          double scale)
{
    // Calculate coordinates for the rectangle
    double x = PADDING + (construction->x - construction->w - xmin) * scale;
    double y = PADDING + (ymax - (construction->y + construction->h)) * scale;
    double width = 2 * construction->w * scale;
    double height = 2 * construction->h * scale;

    // Draw the building rectangle with blue fill
    cairo_set_source_rgba(cr, BUILDING_FILL_R, BUILDING_FILL_G, BUILDING_FILL_B, BUILDING_FILL_A);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill_preserve(cr);

    // Draw the building outline
    cairo_set_line_width(cr, CONSTRUCTION_STROKE_WIDTH);
    cairo_set_source_rgba(cr, BUILDING_STROKE_R, BUILDING_STROKE_G, BUILDING_STROKE_B, BUILDING_STROKE_A);
    cairo_stroke(cr);

    // Draw the building label
    cairo_set_source_rgb(cr, TEXT_R, TEXT_G, TEXT_B);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, FONT_SIZE);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, construction->id, &extents);

    // Center the text in the rectangle
    cairo_move_to(cr,
                  x + (width - extents.width) / 2 - extents.x_bearing,
                  y + (height + extents.height) / 2);
    cairo_show_text(cr, construction->id);
}

/**
 * Draw a house in the scene
 *
 * @param cr            The Cairo context
 * @param construction  The construction to draw
 * @param xmin          Minimum x coordinate of the scene
 * @param ymax          Maximum y coordinate of the scene
 * @param scale         Scale factor for drawing
 */
static void draw_house(cairo_t *cr,
                       const struct Construction *construction,
                       int xmin, int ymax,
                       double scale)
{
    // Calculate coordinates for the rectangle
    double x = PADDING + (construction->x - construction->w - xmin) * scale;
    double y = PADDING + (ymax - (construction->y + construction->h)) * scale;
    double width = 2 * construction->w * scale;
    double height = 2 * construction->h * scale;

    // Draw the house rectangle
    cairo_set_source_rgba(cr, HOUSE_FILL_R, HOUSE_FILL_G, HOUSE_FILL_B, HOUSE_FILL_A);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill_preserve(cr);

    // Draw the house outline
    cairo_set_line_width(cr, CONSTRUCTION_STROKE_WIDTH);
    cairo_set_source_rgba(cr, HOUSE_STROKE_R, HOUSE_STROKE_G, HOUSE_STROKE_B, HOUSE_STROKE_A);
    cairo_stroke(cr);

    // Draw the house label
    cairo_set_source_rgb(cr, TEXT_R, TEXT_G, TEXT_B);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, FONT_SIZE);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, construction->id, &extents);

    // Center the text in the rectangle
    cairo_move_to(cr,
                  x + (width - extents.width) / 2 - extents.x_bearing,
                  y + (height + extents.height) / 2);
    cairo_show_text(cr, construction->id);
}

/**
 * Draw an antenna in the scene
 *
 * @param cr       The Cairo context
 * @param antenna  The antenna to draw
 * @param xmin     Minimum x coordinate of the scene
 * @param ymax     Maximum y coordinate of the scene
 * @param scale    Scale factor for drawing
 */
static void draw_antenna(cairo_t *cr,
                         const struct Antenna *antenna,
                         int xmin, int ymax,
                         double scale)
{
    // Calculate coordinates for the circle
    double x = PADDING + (antenna->x - xmin) * scale;
    double y = PADDING + (ymax - antenna->y) * scale;
    double radius = antenna->r * scale;
    cairo_new_path(cr);

    // Draw the antenna circle
    cairo_set_source_rgba(cr, ANTENNA_FILL_R, ANTENNA_FILL_G, ANTENNA_FILL_B, ANTENNA_FILL_A);
    cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
    cairo_fill_preserve(cr);

    // Draw the antenna outline
    cairo_set_line_width(cr, ANTENNA_STROKE_WIDTH);
    cairo_set_source_rgba(cr, ANTENNA_STROKE_R, ANTENNA_STROKE_G, ANTENNA_STROKE_B, ANTENNA_STROKE_A);
    cairo_stroke(cr);
    cairo_new_path(cr);

    // Draw center point
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_arc(cr, x, y, ANTENNA_CENTER_RADIUS, 0, 2 * M_PI);
    cairo_fill(cr);

    // Draw the antenna label
    cairo_set_source_rgb(cr, TEXT_R, TEXT_G, TEXT_B);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, FONT_SIZE);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, antenna->id, &extents);

    // Center the text in the circle
    cairo_move_to(cr,
                  x - extents.width / 2 - extents.x_bearing,
                  y + extents.height / 2);
    cairo_show_text(cr, antenna->id);
}

// Public functions
// ===============

void draw_scene_svg(const struct Scene *scene, const char *filename)
{
    int xmin, ymin, xmax, ymax;
    calculate_bounding_box(scene, &xmin, &ymin, &xmax, &ymax);

    // Add some padding to the bounding box
    xmin -= 1;
    ymin -= 1;
    xmax += 1;
    ymax += 1;

    // Calculate scale to fit the scene in the SVG
    double scale_x = (WIDTH - 2 * PADDING) / (double)(xmax - xmin);
    double scale_y = (HEIGHT - 2 * PADDING) / (double)(ymax - ymin);
    double scale = scale_x < scale_y ? scale_x : scale_y;

    // Create SVG surface and context
    cairo_surface_t *surface = cairo_svg_surface_create(filename, WIDTH, HEIGHT);
    cairo_t *cr = cairo_create(surface);

    // Set white background
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // Draw grid and frame
    draw_grid(cr, xmin, ymin, xmax, ymax, scale);
    draw_frame(cr, xmin, ymin, xmax, ymax, scale);

    // Draw constructions
    for (unsigned int c = 0; c < scene->num_constructions; ++c)
    {
        const struct Construction *construction = &scene->constructions[c];
        if (construction->type == BUILDING)
        {
            draw_building(cr, construction, xmin, ymax, scale);
        }
        else
        {
            draw_house(cr, construction, xmin, ymax, scale);
        }
    }

    // Draw antennas
    for (unsigned int a = 0; a < scene->num_antennas; ++a)
    {
        const struct Antenna *antenna = &scene->antennas[a];
        draw_antenna(cr, antenna, xmin, ymax, scale);
    }

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}