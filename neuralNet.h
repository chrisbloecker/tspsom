/**
 * @file
 *
 * @author Christopher Blöcker
 */
#ifndef __NEURAL_NET_H__
#define __NEURAL_NET_H__

/* -------------------------------------------------------------------------- */
#include "sampleMap.h"
#include "vector.h"
/* -------------------------------------------------------------------------- */

/* A neuron */
typedef struct NeuronData * Neuron;

/* Attributes of a neuron */
struct NeuronData {
  /* Its position in the plane */
  Vector p;

  /* Number of activations */
  unsigned hits;

  /* Neighbour neurons */
  Neuron next
       , prev
       ;
};

/* A neural net */
typedef struct {
  /* Number of neurons in the neural net */
  unsigned long size;

  /* Number of learning steps without growing */
  unsigned long learned;

  /* The net's neurons */
  Neuron neurons;
} NeuralNet;

/* A bounding box */
typedef struct {
  Vector topleft
       , bottomright
       ;
} PositionBounds;

/* -------------------------------------------------------------------------- */
#define neuralNetLearnAfter(n) (n)
#define neuralNetGrowThres(n) ((1.0)/(log(n)))
#define neuralNetShrinkThres(n) (1)
/* -------------------------------------------------------------------------- */

/**
 * Creates a neural net with exactly one neuron. The single neuron of the net
 * is positioned in the middle of the region given by bounds.
 *
 * @param[in] bounds  Bounding box for the neural net.
 *
 * @return Neural net with one neuron.
 */
extern NeuralNet neuralNetMake(PositionBounds bounds);

/**
 * Frees the memory that is used by the given neural net.
 *
 * @param[in] neuralNet  Neural net that should be freed.
 *
 * @return Empty neural net.
 */
extern NeuralNet neuralNetFree(NeuralNet nn);

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
extern NeuralNet neuralNetTrain(NeuralNet nn, SampleMap s, double time);

/**
 * Removes neurons that are considered to be "the same". This is the case  when
 * their distance is smaller than a defined minimum.
 *
 * @param[in] neuralNet  Neural net that should be pruned.
 *
 * @return Neural net after pruning.
 */
extern NeuralNet neuralNetRemoveDoubleNeurons(NeuralNet neuralNet);

/**
 * Calculates the "length" of the neural network, i.e. the approcimate length of
 * round trip that is given by the network.
 *
 * @param[in] neuralNet  Neural net of which the length should be calculated.
 *
 * @return Sum of the lengths of the connecitons between the neurons of the given network.
 */
extern double neuralNetLength(NeuralNet neuralNet);

/**
 * Prints information about the given neural network on stream.
 *
 * @param[in] neuralNet  Neural net about which information should be printed.
 * @param[in] stream     Where to print the information.
 */
extern void neuralNetPrint(FILE * stream, NeuralNet neuralNet);

#endif
