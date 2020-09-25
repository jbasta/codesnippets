# euchre.cpp 

This file contains the euchre class and driver function for my implementation of the card game [Euchre](https://en.wikipedia.org/wiki/Euchre). It is playable through the terminal with up to four human and computer players.


# randmst.cpp
This is a program that calculates the weight and average edge length for a minimum spanning tree on a complete graph with user-specified number of vertices and dimension. A dimension 0 graph has random edge lengths, while a graph with dimension 2 - 4 is geometric, with vertices representing random points in unit square, cube, or hypercube, respectively. 

The task was to handle up to 256000 vertices, meaning over 68 billion edge values, too much for my machine to hold. My solution was to modify [Prim's algorithm](https://en.wikipedia.org/wiki/Prim%27s_algorithm). In my implementation, I throw away uneeded edge lengths as you calculate them, resulting in O(V) space complexity rather than O(V^2).
