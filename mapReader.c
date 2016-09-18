/**
 * @file
 *
 * @author Christopher Blöcker
 */

/* -------------------------------------------------------------------------- */
#include <stdio.h>
/* -------------------------------------------------------------------------- */
#include "mapReader.h"
/* -------------------------------------------------------------------------- */

/**
 * Read the samples, i.e. city positions, from the given file name and return
 * them.
 *
 * @param[in] filename  File that contains the samples.
 *
 * @return SampleMap.
 */
extern SampleMap mapReaderRead(char * filename)
{
  /* Error and number of samples to read */
  int error = 0
    , n
    ;

  /* Sample position */
  Vector p;

  /* Dummies for parsing */
  char space
     , newline
     ;

  /* Resulting sample map */
  SampleMap res;

  FILE * f = fopen(filename, "r");
  error = !f;

  /* Read number of samples */
  if (error)
    fprintf(stderr, "mapReaderRead :: Error opening file %s\n.", filename);
  else
    error = fscanf(f, "%i%c", &n, &newline) != 2 || newline != '\n';

  /* Create sample map for the given number of samples */
  if (error)
    fprintf(stderr, "mapReaderRead :: Error reading sample count.\n");
  else
    res = sampleMapMake(n);

  /* Read the samples and put them into the sample map */
  for (int i = 0; i < n && !error; ++i)
  {
    error = fscanf(f, "%lf%c%lf%c", &p.x, &space, &p.y, &newline) != 4
          || space != ' '
          || newline != '\n';

    if (error)
      fprintf(stderr, "mapReaderRead :: Error reading sample %i.\n", i);
    else
      res = sampleMapPut(res, p);
  }

  /* Clear allocated memory in case of an error */
  if (error)
    res = sampleMapFree(res);

  return res;
}
