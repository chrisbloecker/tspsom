/**
 * @file
 *
 * @author Christopher Blöcker
 */

/* -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <assert.h>
/* -------------------------------------------------------------------------- */
#include "sampleMap.h"
#include "vector.h"
/* -------------------------------------------------------------------------- */

/**
 * Created a new sample map.
 *
 * @param[in] size  Size of the sample map to create.
 *
 * @return The new sample map with space for the given number of samples.
 */
extern SampleMap sampleMapMake(int size)
{
  SampleMap res;

  res.size  = size;
  res.items = 0;

  res.samples = malloc(size * sizeof(Vector));

  return res;
}

/**
 * Inserts a sample into the sample map.
 *
 * @param[in] s       The sample map.
 * @param[in] sample  The sample that should be intserted into the sample map.
 *
 * @return The new sample map containing the new sample.
 */
extern SampleMap sampleMapPut(SampleMap s, Vector sample)
{
  assert(s.items < s.size);

  s.samples[s.items++] = sample;

  return s;
}

/**
 * Frees the memory used by the given sample map.
 *
 * @param[in] s  Sample map that should be freed.
 *
 * @return NULL.
 */
extern SampleMap sampleMapFree(SampleMap s)
{
  s.size = 0;

  if (s.samples)
    free(s.samples);

  s.samples = NULL;

  return s;
}

/**
 * Prints information about the given sample map in stream.
 *
 * @param[in] s       The sample map information should be printed about.
 * @param[in] stream  Where to print the information.
 */
extern void sampleMapPrint(SampleMap s, FILE * stream)
{
  fprintf(stream, "Sample Map ::\n");
  fprintf(stream, "  size  : %i\n", s.size);
  fprintf(stream, "  items : %i\n", s.items);

  for (unsigned i = 0; i < s.items; ++i)
    fprintf(stream, "  %i : (%lf, %lf)\n", i, s.samples[i].x, s.samples[i].y);
}
