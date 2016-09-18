# tspsom

tspsom uses self organising maps (som) with a ring topology to find approximate solutions to instances of the traveling salesman problem (tsp) in 2D eucledian space. cairo is used to render the solutions to images in png format.

## Usage

```
Usage: tspsom <tsp file> [options]
  Required arguments:
    tsp file       File that contains the tsp instance.

  Options:
    -l <number>    Set the number of learning cycles           (default: 10000)
    -p <number>    Rendering images after how many iterations  (default: 1000)
    -d <number>    Set the debug level.                        (default: 0)
```

## Input Format

Input files for tspsom are plain text files where the first line contains the number of cities in the given instance. The following lines list the x and y coordinates of the cities, separated by a space character. The lines are terminated by unix line endings `\n`. An example of an instance with 5 cities is given below:

```
5
0 0
1 1
3 5
5 2
6 4
```

## Example

As an example, we run tspsom on the berlin52 instance from the [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/) and show the rendering of the neural net after different numbers of iterations. The final image shows a candidate solution for the berlin52 problem.

### 0 iterations
The initial net consists of only one neuron.

![0](https://github.com/chrisbloecker/tspsom/blob/master/img/0.png?raw=true)

### 250 iterations
The net has grown more neurons.

![250](https://github.com/chrisbloecker/tspsom/blob/master/img/250.png?raw=true)

### 500 iterations
The net has grown more neurons and starts to adapt to the problem instance's "shape".

![500](https://github.com/chrisbloecker/tspsom/blob/master/img/500.png?raw=true)

### 1000 iterations
The net covers almost all locations in the map now and only fine tuning is left to be done. By now the learning rate has decayed far enough so that the general shape of the net will remain.

![1000](https://github.com/chrisbloecker/tspsom/blob/master/img/1000.png?raw=true)

### 10000 iterations
Fine tuning is done and the net represents a candidate solution to the instance of the tsp.

![10000](https://github.com/chrisbloecker/tspsom/blob/master/img/10000.png?raw=true)
