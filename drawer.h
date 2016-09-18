/**
 * @file
 *
 * @author Christopher Blöcker
 */
#ifndef __DRAWER_H__
#define __DRAWER_H__

/* -------------------------------------------------------------------------- */
#include <stdio.h>
/* -------------------------------------------------------------------------- */
#include "neuralNet.h"
#include "sampleMap.h"
/* -------------------------------------------------------------------------- */

/**
 * Prepares the data for rendering.
 * The samples are projected into picture space exactly once, since their
 * will remain fixed.
 *
 * @param[in] sampleMap  Samples in object space.
 * @param[in] bounds     Bounding box around the samples in object space.
 */
extern void drawerPrepareData(SampleMap sampleMap, PositionBounds bounds);

/**
 * Frees the allocated memory.
 */
extern void drawerCleanUp();

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
extern void drawerDrawMap(NeuralNet nn, SampleMap s, PositionBounds bounds, char * filename);

#endif
