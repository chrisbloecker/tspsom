/**
 * @file
 *
 * @author Christopher Blöcker
 */

/* -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <float.h>
/* -------------------------------------------------------------------------- */
#include "neuralNet.h"
/* -------------------------------------------------------------------------- */

/**
 * Defines how many neighbouring neurons will be affected when the position of
 * a neuron is updated.
 */
#define SPREAD (3)

/* Distance at which neighbouring neurons are considered to be equal */
#define NEURON_REMOVE_DISTANCE (1)

/* -------------------------------------------------------------------------- */

/**
 * Calculates the adjustment for a neuron that's activated
 *
 * @param[in] d  Distance to activated neuron (neighbourhood)
 * @param[in] t  Time progress (learning rate decay)
 */
#define NEURONMOVE(d, t) (exp(-(((1)+(d))/((2)*(t)))))

/* -------------------------------------------------------------------------- */

/**
 * Invariant for a neural network.
 * A neural network is valid if its pointers are valid, i.e. if forward and
 * backward neighbouring relations are symmetric.
 *
 * @param[in] neuralNet  Neural net.
 *
 * @return 1  if the neural net is valid
 *         0  otherwise.
 */
static int neuralNetInv(NeuralNet neuralNet)
{
  int valid = 1;

  Neuron neuron = neuralNet.neurons->next;

  /* Check for correct links between neurons */
  while (valid && neuron != neuralNet.neurons)
  {
    valid &= neuron->next->prev == neuron
          && neuron->prev->next == neuron
          ;

    neuron = neuron->next;
  }

  return valid;
}

/**
 * Creates a neuron and returns it.
 *
 * @return Neuron.
 */
static Neuron neuronMake()
{
  Neuron res = malloc(sizeof(*res));

  if (!res)
    perror("neuronMake :: malloc failed.");

  /* The neuron has not been activated yet */
  res->hits = 0;

  return res;
}

/**
 * Creates a new neuron and inserts it after the passed neuron.
 *
 * @param[in] neuron  Neuron after which the new neuron should be inserted.
 *
 * @return The newly created neuron.
 */
static Neuron neuronInsert(Neuron neuron)
{
  Neuron newNeuron = neuronMake();

  #ifdef DEBUG
  fprintf(stderr, "[DEBUG] Adding Neuron at (%.2lf, %.2lf).\n", neuron->p.x, neuron->p.y);
  #endif

  neuron->hits = 0;

  /* Set the position of the new neuron and insert it into the ring */
  newNeuron->p = vectorAdd( neuron->p
                          , vectorScale( vectorSub( neuron->next->p
                                                  , neuron->p
                                                  )
                                       , 0.5
                                       )
                          );

  newNeuron->next = neuron->next;
  newNeuron->prev = neuron;
  newNeuron->next->prev = newNeuron;
  newNeuron->prev->next = newNeuron;

  return newNeuron;
}

/**
 * Removes the neuron after the given neuron.
 *
 * @param[in] neuron  Neuron of which the successor neuron should be removed.
 *
 * @return The updated neuron.
 */
static Neuron neuronRemoveNext(Neuron neuron)
{
  #ifdef DEBUG
  fprintf(stderr, "[DEBUG] Removing Neuron at (%.2lf, %.2lf).\n", neuron->p.x, neuron->p.y);
  #endif

  Neuron tmp = neuron->next;

  /* Update neighbourhood */
  neuron->next       = neuron->next->next;
  neuron->next->prev = neuron;

  free(tmp);

  return neuron;
}

/**
 * Let's the neural net grow by creating new neurons.
 *
 * @param[in] neuralNet      Neural net that should grow.
 * @param[in] growThreshold  Defines how often a neuron must have been activated
 *                           in order to grow new neighbouring neurons.
 *
 * @return New neural net after growing new neurons.
 */
static NeuralNet neuralNetGrow(NeuralNet neuralNet, double growThreshold)
{
  Neuron neuron = neuralNet.neurons;

  /**
   * Insert a new neuron if the current neuron has been activated enough times.
   * Continue with all other neurons thereafter.
   */
  if (neuron->hits >= growThreshold)
  {
    neuron = neuronInsert(neuron);
    ++neuralNet.size;
  }

  neuron = neuron->next;

  /* Grow neighbouring neurons for the rest of the neurons */
  while (neuron != neuralNet.neurons)
  {
    if (neuron->hits >= growThreshold)
    {
      neuron = neuronInsert(neuron);
      ++neuralNet.size;
    }
    else
    {
      neuron = neuron->next;
    }
  }

  neuralNet.learned = 0;

  /* The neural net must remain valid after growing */
  assert(neuralNetInv(neuralNet));

  return neuralNet;
}

/**
 * Recursively free the memory that is used bei the given neuron and its successors.
 *
 * @param[in] neuron  Neuron that should be freed.
 *
 * @return NULL.
 */
static Neuron neuronsFree(Neuron neuron)
{
  if (neuron->next)
    neuronsFree(neuron->next);

  free(neuron);

  return NULL;
}

/* -------------------------------------------------------------------------- */

/**
 * Creates a neural net with exactly one neuron. The single neuron of the net
 * is positioned in the middle of the region given by bounds.
 *
 * @param[in] bounds  Bounding box for the neural net.
 *
 * @return Neural net with one neuron.
 */
extern NeuralNet neuralNetMake(PositionBounds bounds)
{
  /* Create the neural net */
  NeuralNet neuralNet;
  Neuron neuron = neuronMake();

  neuralNet.size    = 1;
  neuralNet.learned = 0;

  /* Initialise neuron */
  neuron->p = vectorAdd( bounds.topleft
                       , vectorScale( vectorSub( bounds.bottomright
                                               , bounds.topleft
                                               )
                                    , 0.5
                                    )
                       );

  neuron->next = neuron;
  neuron->prev = neuron;

  neuralNet.neurons = neuron;

  return neuralNet;
}

/**
 * Frees the memory that is used by the given neural net.
 *
 * @param[in] neuralNet  Neural net that should be freed.
 *
 * @return Empty neural net.
 */
extern NeuralNet neuralNetFree(NeuralNet neuralNet)
{
  neuralNet.size = 0;

  if (neuralNet.neurons)
  {
    neuralNet.neurons->prev->next = NULL;
    neuronsFree(neuralNet.neurons);
  }

  neuralNet.neurons = NULL;

  return neuralNet;
}

/**
 * Trains the neural net based on the given samples. Time states how much time
 * of the learning has passed already and is used for learning rate decay.
 *
 * @param[in] neuralNet  Neural net that should be trained.
 * @param[in] samples    The samples that should be used for training.
 * @param[in] time       Progression of training time, used for learning rate decay.
 *
 * @return Neural net after training.
 */
extern NeuralNet neuralNetTrain(NeuralNet neuralNet, SampleMap samples, double time)
{
  /* Pick a sample, this is where it gets "nondeterministic" */
  Vector sample = samples.samples[random() % samples.items];

  /* Find closest neuron */
  Neuron currentNeuron = neuralNet.neurons
       , nearestNeuron = neuralNet.neurons
       ;
  double distance = vectorLength(vectorSub(currentNeuron->p, sample))
       , tmp
       ;

  for (unsigned i = 0; i < neuralNet.size; ++i, currentNeuron = currentNeuron->next)
    if ((tmp = vectorLength(vectorSub(sample, currentNeuron->p))) < distance)
    {
      nearestNeuron = currentNeuron;
      distance      = tmp;
    }

  /* Mark nearest neuron as activated */
  ++nearestNeuron->hits;
  currentNeuron = nearestNeuron;
  for (int i = 0; i <= SPREAD + 1; ++i)
    currentNeuron = currentNeuron->prev;

  /* Let the activated neuron and its neighbours learn */
  for (int i = -SPREAD; i <= SPREAD; ++i)
  {
    currentNeuron = currentNeuron->next;

    currentNeuron->p = vectorAdd( currentNeuron->p
                                , vectorScale( vectorSub( sample
                                                        , currentNeuron->p
                                                        )
                                             , NEURONMOVE( fabs(i)
                                                         , time
                                                         )
                                             )
                                );
  }

  ++neuralNet.learned;

  if (neuralNet.size < log(samples.items) * samples.items
   && neuralNet.learned >= neuralNetLearnAfter(samples.items))
  {
    neuralNet = neuralNetGrow(neuralNet, neuralNetGrowThres(samples.items));
  }

  return neuralNet;
}

/**
 * Removes neurons that are considered to be "the same". This is the case  when
 * their distance is smaller than a defined minimum.
 *
 * @param[in] neuralNet  Neural net that should be pruned.
 *
 * @return Neural net after pruning.
 */
extern NeuralNet neuralNetRemoveDoubleNeurons(NeuralNet neuralNet)
{
  Neuron neuron = neuralNet.neurons;

  /**
   * Prune the net as long as it has more than one neuron and there are neurons
   * that are considered to be the same
   */
  while (neuralNet.size > 1
     &&  vectorLength(vectorSub(neuron->p, neuron->next->p)) <= NEURON_REMOVE_DISTANCE)
  {
    neuron = neuronRemoveNext(neuron);
    neuralNet.neurons = neuron;
    --neuralNet.size;
  }

  neuron = neuron->next;

  while (neuron != neuralNet.neurons)
  {
    while (neuralNet.size > 1
       &&  vectorLength(vectorSub(neuron->p, neuron->next->p)) <= NEURON_REMOVE_DISTANCE)
    {
      neuron = neuronRemoveNext(neuron);
      neuralNet.neurons = neuron;
      --neuralNet.size;
    }

    neuron = neuron->next;
  }

  assert(neuralNetInv(neuralNet));

  return neuralNet;
}

/**
 * Calculates the "length" of the neural network, i.e. the approcimate length of
 * round trip that is given by the network.
 *
 * @param[in] neuralNet  Neural net of which the length should be calculated.
 *
 * @return Sum of the lengths of the connecitons between the neurons of the given network.
 */
extern double neuralNetLength(NeuralNet neuralNet)
{
  double res = 0.0;

  res += vectorLength(vectorSub(neuralNet.neurons->p, neuralNet.neurons->next->p));

  for (Neuron neuron = neuralNet.neurons->next; neuron != neuralNet.neurons; neuron = neuron->next)
    res += vectorLength(vectorSub(neuron->p, neuron->next->p));

  return res;
}

/**
 * Prints information about the given neural network on stream.
 *
 * @param[in] neuralNet  Neural net about which information should be printed.
 * @param[in] stream     Where to print the information.
 */
extern void neuralNetPrint(FILE * stream, NeuralNet neuralNet)
{
  Neuron neuron = neuralNet.neurons->next;

  fprintf(stream, "Neural net ::\n");
  fprintf(stream, "  size : %li\n", neuralNet.size);
  for (unsigned i = 0; i < neuralNet.size; ++i, neuron = neuron->next)
    fprintf(stream, "  neuron %i at (%lf, %lf)\n", i, neuron->p.x, neuron->p.y);
}
