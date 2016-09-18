/**
 * @file
 *
 * @author Christopher Blöcker
 */
#ifndef __SAMPLEMAP_H__
#define __SAMPLEMAP_H__

/* -------------------------------------------------------------------------- */
#include <stdio.h>
/* -------------------------------------------------------------------------- */
#include "vector.h"
/* -------------------------------------------------------------------------- */

/* A sample map, i.e. a collection of samples */
typedef struct {
  /* Capacity of the sample map */
  unsigned size;

  /* Number of elements in the sample map */
  unsigned items;

  /* The elements of the sample map */
  Vector * samples;
} SampleMap;

/* -------------------------------------------------------------------------- */

/**
 * Created a new sample map.
 *
 * @param[in] size  Size of the sample map to create.
 *
 * @return The new sample map with space for the given number of samples.
 */
extern SampleMap sampleMapMake(int size);

/**
 * Inserts a sample into the sample map.
 *
 * @param[in] s       The sample map.
 * @param[in] sample  The sample that should be intserted into the sample map.
 *
 * @return The new sample map containing the new sample.
 */
extern SampleMap sampleMapPut(SampleMap s, Vector sample);

/**
 * Frees the memory used by the given sample map.
 *
 * @param[in] s  Sample map that should be freed.
 *
 * @return NULL.
 */
extern SampleMap sampleMapFree(SampleMap s);

/**
 * Prints information about the given sample map in stream.
 *
 * @param[in] s       The sample map information should be printed about.
 * @param[in] stream  Where to print the information.
 */
extern void sampleMapPrint(SampleMap s, FILE * stream);

#endif
