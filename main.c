/**
 * @file
 *
 * @author Christopher Blöcker
 */

/* -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>
/* -------------------------------------------------------------------------- */
#include "types.h"
#include "sampleMap.h"
#include "mapReader.h"
#include "neuralNet.h"
#include "drawer.h"
/* -------------------------------------------------------------------------- */

/* A type for the config */
typedef struct {
  unsigned maxLearn
         , print
         , debugLevel
         ;

  Boolean help
        , error
        ;

  char * filename;
} Config;

typedef struct timespec TimeSpec;

/* -------------------------------------------------------------------------- */
#define DEFAULT_MAXLEARN   (10000)
#define DEFAULT_PRINT      ( 1000)
#define DEFAULT_DEBUGLEVEL (    0)
#define DEFAULT_HELP       (FALSE)
/* -------------------------------------------------------------------------- */

TimeSpec diff(TimeSpec start, TimeSpec end)
{
  TimeSpec res;

  res.tv_sec  = abs(end.tv_sec  - start.tv_sec);
  res.tv_nsec = abs(end.tv_nsec - start.tv_nsec);

  return res;
}

/**
 * Creates the default config
 *
 * @return Default config.
 */
Config getDefaultConfig(void)
{
  Config c;

  c.maxLearn   = DEFAULT_MAXLEARN;
  c.print      = DEFAULT_PRINT;
  c.debugLevel = DEFAULT_DEBUGLEVEL;
  c.help       = FALSE;
  c.error      = FALSE;
  c.filename = '\0';

  return c;
}

/**
 * Prints the help message to stream.
 *
 * @param stream Where to print the help message.
 */
void help(FILE* stream)
{
  fprintf(stream, "tspsom, a program for finding approximate solutions to instances of the traveling salesman problem in 2D eucledian space.\n");
  fprintf(stream, "\n");
  fprintf(stream, "Usage: tspsom <tsp file> [options]\n");
  fprintf(stream, "  Required arguments:\n");
  fprintf(stream, "    tsp file       File that contains the tsp instance.\n");
  fprintf(stream, "\n");
  fprintf(stream, "  Options:\n");
  fprintf(stream, "    -l <number>    Set the number of learning cycles           (default: %i)\n", DEFAULT_MAXLEARN);
  fprintf(stream, "    -p <number>    Rendering images after how many iterations  (default: %i)\n", DEFAULT_PRINT);
  fprintf(stream, "    -d <number>    Set the debug level.                        (default: %i)\n", DEFAULT_DEBUGLEVEL);
}

/**
 * Parses the command line arguments.
 *
 * @param[in] argc Parameter count.
 * @param[in] argv The parameters.
 *
 * @return Config parsed from the command line arguments.
 */
static Config parseArgs(int argc, char * argv[])
{
  Config c = getDefaultConfig();

  /* First parameter is the input file name */
  c.filename = argv[1];
  int i = 2;

  while (i < argc)
  {
    if (strcmp(argv[i], "-d") == 0)
      c.error = sscanf(argv[++i], "%u", &c.debugLevel) != 1;

    else if (strcmp(argv[i], "-h") == 0)
      c.help = TRUE;

    else if (strcmp(argv[i], "-l") == 0)
      c.error = sscanf(argv[++i], "%u", &c.maxLearn) != 1;

    else if (strcmp(argv[i], "-p") == 0)
      c.error = sscanf(argv[++i], "%u", &c.print) != 1;

    else
    {
      fprintf(stdout, "[ERROR] Option not recognised: %s. Exiting.\n", argv[i]);
      help(stderr);
      exit(1);
    }

    ++i;
  }

  return c;
}

/**
 *
 */
int main(int argc, char * argv[])
{
  Config c = parseArgs(argc, argv);

  /* Time measurements */
  TimeSpec start
         , end
         , elapsed
         ;

  clock_gettime(CLOCK_REALTIME, &start);

  if (argc > 1)
  {
    /* dirty... */
    char filename[100];

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG] Reading Samples from %s.\n", c.filename);
    #endif

    /* Read samples from input file (the city positions) */
    SampleMap s = mapReaderRead(c.filename);

    /* Find the bounding box around the given samples */
    PositionBounds bounds;
    bounds.topleft     = s.samples[0];
    bounds.bottomright = s.samples[0];

    for (unsigned sample = 1; sample < s.items; ++sample)
    {
      if (s.samples[sample].x < bounds.topleft.x)
        bounds.topleft.x = s.samples[sample].x;

      if (s.samples[sample].y < bounds.topleft.y)
        bounds.topleft.y = s.samples[sample].y;

      if (s.samples[sample].x > bounds.bottomright.x)
        bounds.bottomright.x = s.samples[sample].x;

      if (s.samples[sample].y > bounds.bottomright.y)
        bounds.bottomright.y = s.samples[sample].y;
    }

    #ifdef INFO
    fprintf(stderr, "[INFO ] Samples read.\n");
    fprintf(stderr, "[INFO ] Bounds are :: left   : %lf\n",  bounds.topleft.x);
    fprintf(stderr, "[INFO ]               right  : %lf\n",  bounds.bottomright.x);
    fprintf(stderr, "[INFO ]               top    : %lf\n",  bounds.topleft.y);
    fprintf(stderr, "[INFO ]               bottom : %lf.\n", bounds.bottomright.y);
    fprintf(stderr, "[INFO ] Learning after %lu cycles.\n",  (unsigned long) neuralNetLearnAfter(s.items));
    fprintf(stderr, "[INFO ] Learning threshold is %lf.\n",  neuralNetGrowThres(s.items));
    #endif

    /* Create the neural net, i.e. the self organising map */
    NeuralNet nn = neuralNetMake(bounds);

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG] Created neural net\n");
    #endif

    srand(time(NULL));

    /* Prepare paingin */
    drawerPrepareData(s, bounds);

    /* Initial "solution" */
    drawerDrawMap(nn, s, bounds, "./img/0.png");

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG] Training ...\n");
    #endif

    /* Train the neural net and render images */
    unsigned time;
    for (time = 1; time <= c.maxLearn; ++time)
    {
      #ifdef DEBUG
      fprintf(stderr, "[DEBUG] cycle %i from %i :: %.2lf%% done\n", time, c.maxLearn, 100.0 * time / c.maxLearn);
      #endif

      sprintf(filename, "./img/%i.png", time);

      nn = neuralNetTrain(nn, s, (double) (c.maxLearn - time) / c.maxLearn);

      if (!(time % c.print))
        drawerDrawMap(nn, s, bounds, filename);
    }

    #ifdef INFO
    fprintf(stderr, "[INFO ] Length of tour : %lf.\n", neuralNetLength(nn));
    #endif

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG] Cleaning up.\n");
    #endif

    /* Clean up... */
    drawerCleanUp();
    nn = neuralNetFree(nn);
    s = sampleMapFree(s);
  }
  else
    help(stderr);

  clock_gettime(CLOCK_REALTIME, &end);
  elapsed = diff(start, end);

  /* Print a summary */
  fprintf(stderr, "Elapsed time: %li s, %li ms, %li µs, %li ns.\n"
                , elapsed.tv_sec
                , elapsed.tv_nsec / 1000000
                , (elapsed.tv_nsec / 1000) % 1000
                , elapsed.tv_nsec % 1000);

  return 0;
}
