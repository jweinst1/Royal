# Royal


## Intro

`royal` is a library and framework for handling graph data. Unlike other data file formats or libraries, `royal` specializes in storing relationships between objects and data points. `royal` is not a graph database, it allows abstraction and processing of
graph relationships in the form of transportable data.


## Development Status

Royal is currently in the alpha development phase. The API is being determined.

## Python API

`royal` provides both a python API and a C api. The python api calls the C library via a python extension module.

### Installation

To install royal, you can use pip

```
$ pip install royal
```

### Usage

You can import `royal` like a normal python module. The primary object in `royal` is the `RoyalGraph`, an object which contains a sequence of connections.

```py
from royal import RoyalGraph
```

A connection symbolizes the relationship between two values. Each connection has two values, and one relationship. These are also called edges and vertices, respectively. Edges and vertices are always strings. A `RoyalGraph` has a maximum size for each part of the connection. This size is determined when an instance of a `RoyalGraph` is created:

```py
g = RoyalGraph(5)
g2 = RoyalGraph(field=5)
```

You can also tune the capacity of memory allocated for each graph, and the rate at which the graph will grow in memory consumption, via the `capacity` and `growth` keyword arguments.