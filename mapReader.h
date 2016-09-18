/**
 * @file
 *
 * @author Christopher Blöcker
 */
#ifndef __MAP_READER_H__
#define __MAP_READER_H__

/* -------------------------------------------------------------------------- */
#include "sampleMap.h"
/* -------------------------------------------------------------------------- */

 /**
  * Read the samples, i.e. city positions, from the given file name and return
  * them.
  *
  * @param[in] filename  File that contains the samples.
  *
  * @return SampleMap.
  */
extern SampleMap mapReaderRead(char * filename);

#endif
