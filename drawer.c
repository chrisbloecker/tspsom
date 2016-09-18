/**
 * @file
 *
 * @author Christopher Blöcker
 */

/* -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cairo.h>
#include <math.h>
/* -------------------------------------------------------------------------- */
#include "drawer.h"
/* -------------------------------------------------------------------------- */
#define WIDTH  (1024)
#define HEIGHT ( 768)
#define RADIUS (0.005 * (WIDTH < HEIGHT ? WIDTH : HEIGHT))
/* -------------------------------------------------------------------------- */

/* Scaling factor for projection in picture space */
Vector scale;

/* Memory for projected sample positions in picture space */
Vector * samplePos = NULL;

/* -------------------------------------------------------------------------- */

/**
 * Prepares the data for rendering.
 * The samples are projected into picture space exactly once, since their
 * will remain fixed.
 *
 * @param[in] sampleMap  Samples in object space.
 * @param[in] bounds     Bounding box around the samples in object space.
 */
extern void drawerPrepareData(SampleMap sampleMap, PositionBounds bounds)
{
  /* Scaling factors for projection in picture space */
  scale = vectorMake( (WIDTH  - 4 * RADIUS) / (bounds.bottomright.x - bounds.topleft.x)
                    , (HEIGHT - 4 * RADIUS) / (bounds.bottomright.y - bounds.topleft.y));

  /* Allocate memory for projected samples in picture space */
  samplePos = malloc(sampleMap.items * sizeof(Vector));

  if (!samplePos)
    perror("[ERROR] drawerPrepareData :: malloc failed.");

  /* Project the samples into picture space */
  for (unsigned sample = 0; sample < sampleMap.items; ++sample)
  {
    samplePos[sample].x = scale.x * (sampleMap.samples[sample].x - bounds.topleft.x) + 2 * RADIUS;
    samplePos[sample].y = HEIGHT - scale.y * (sampleMap.samples[sample].y - bounds.topleft.y) - 2 * RADIUS;
  }
}

/**
 * Frees the allocated memory.
 */
extern void drawerCleanUp()
{
  if (samplePos)
    free(samplePos);
  samplePos = NULL;
}

/**
 * Render the samples from sampleMap and the neurons of the neural net as a
 * picture under filename. For that, the neurons are projected into from object
 * space into picture space with linear interpolation.
 *
 * @param[in] neuralNet  Neural net.
 * @param[in] sampleMap  Map that contains the samples.
 * @param[in] bounds     Bounding box around the samples.
 * @param[in] filename   Output filename.
 */
extern void drawerDrawMap(NeuralNet neuralNet, SampleMap sampleMap, PositionBounds bounds, char * filename)
{
  assert(samplePos);

  #ifdef DEBUG
  fprintf(stderr, "[DEBUG] Rendering %s\n", filename);
  #endif

  /* Memory for projected neuron positions */
  Vector * neuronPos = malloc(neuralNet.size  * sizeof(Vector));

  if (!neuronPos)
    perror("[ERROR] drawerDrawMap :: malloc failed.");

  /* Create image region and surface */
  cairo_surface_t * surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
  cairo_t * cr = cairo_create(surface);

  /* Render background */
  cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_fill(cr);

  /* Project the neurons into picture space  */
  Neuron neuron = neuralNet.neurons;
  for (unsigned long i = 0; i < neuralNet.size; ++i)
  {
    neuronPos[i].x = scale.x * (neuron->p.x - bounds.topleft.x) + 2 * RADIUS;
    neuronPos[i].y = HEIGHT - scale.y * (neuron->p.y - bounds.topleft.y) - 2 * RADIUS;

    neuron = neuron->next;
  }

  /* Render samples, i.e. the cities */
  cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
  for (unsigned sample = 0; sample < sampleMap.items; ++sample)
  {
    cairo_save(cr);
    {
      cairo_translate(cr, samplePos[sample].x, samplePos[sample].y);
      cairo_scale(cr, RADIUS, RADIUS);
      cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
      cairo_fill(cr);
    }
    cairo_restore(cr);
  }

  /* Render connections between neurons, i.e. the "topology" of the self organising map */
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(cr, 0.5);
  for (unsigned neuron = 0; neuron < neuralNet.size; ++neuron)
  {
    cairo_move_to(cr, neuronPos[neuron].x, neuronPos[neuron].y);
    cairo_line_to(cr, neuronPos[(neuron + 1) % neuralNet.size].x, neuronPos[(neuron + 1) % neuralNet.size].y);
  }

  cairo_stroke(cr);

  /* Render the neurons */
  cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
  for (unsigned neuron = 0; neuron < neuralNet.size; ++neuron)
  {
    cairo_save(cr);
    {
      cairo_translate(cr, neuronPos[neuron].x, neuronPos[neuron].y);
      cairo_scale(cr, RADIUS, RADIUS);
      cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
      cairo_fill(cr);
    }
    cairo_restore(cr);
  }

  /* Clean up and free memory */
  cairo_destroy(cr);
  cairo_surface_write_to_png(surface, filename);
  cairo_surface_destroy(surface);

  if (neuronPos)
    free(neuronPos);
  neuronPos = NULL;
}
