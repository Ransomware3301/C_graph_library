# C_graph_library
A powerful and flexible C library for creating graphs, a mathematical tool with almost endless applications

- - -
# Brief Introduction

Graphs are powerful mathematical tools used to represent any system that can be boiled down to a set of nodes and edges
(i.e. connections between nodes).

<br>

A few common examples of the use of graphs are:
- Computer networks (computers as nodes, wired/wireless connections as edges)
- Atomic lattice (atoms as nodes, and bonds as edges)
- Finite State Automatons (FSAs), Push-Down Automatons (PDAs) and Turing Machines (TMs) (automaton states as nodes, state changes as edges)
- Relational Database Structures (items as nodes, relations as edges)
etc...

<br>

For more information, see the Wikipedia article page: https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)

- - -
# Graph Structural Definition

The following C code describes the structure of each data type (graphs, nodes, edge lists and edges):

A graph is defined as a linked list of nodes, thus its definition has a pointer to the next node and a "container" for the actual node data

```C
/* Graph Definition */
typedef struct graph 
{
    graph_node_t node;
    struct graph *next;
}
graph_t;
```

Each node is defined as a struct containing the node's label, unique node ID (abbreviated as NID) and a linked list of edges, plus
some additional parameters used when computing the MST from a source node using Dijkstra's Algorithm.

```C
/* Node Definition */
typedef struct graph_node
{
    id_t id;
    char *label;
    graph_edge_list_t *edges;
    /* For Dijkstra */
    unsigned long int dist;
    id_t prev_eid;
    id_t prev_nid;
}
graph_node_t;
```

Then the edges list, the one contained in each node, is defined as a struct containing each edge and a pointer to the next edge in the list

```C
/* Edge List Definition */
typedef struct graph_edge_list 
{
    graph_edge_t edge;
    struct graph_edge_list *next;
}
graph_edge_list_t;
```

And finally, each edge is defined as a struct containing the edge's label, weight, unique edge ID (abbreviated as EID) and an array 
of two endpoints IDs, which are the two nodes' IDs that each edge connects. 

Conventionally, the NID in index 0 of the array is the source node's ID and the NID in index 1 is the destination node's ID.
Also there's a flag "bool_t is_in_mst" used to mark an edge belonging to an MST, calculated with Dijkstra's Algorithm.

```C
/* Edge Definition */
typedef struct graph_edge 
{
    id_t id;
    int weight;
    char *label;
    id_t endpoint_ids[2];
    /* For Dijkstra */
    bool_t is_in_mst;
}
graph_edge_t;
```

- - -

# Available Functions

The library is very flexible and has many built-in functions, organised in sections.

First, we have functions related to the structures of the graph, meaning that they are implemented to create, delete or modify data in graphs or even entire graphs
These operate with both types of linked lists used in the code (i.e. graphs and edge lists) 

```C
/* Graph List Actions */
graph_t * push_node(graph_t*, graph_node_t);
graph_t * append_node(graph_t*, graph_node_t);
graph_t * delete_node(graph_t*, id_t);
graph_t * delete_graph(graph_t*);
graph_t * find_node(graph_t*, id_t);
graph_t * find_node_R(graph_t*, id_t);


/* Edges List Actions */
graph_edge_list_t * push_edge(graph_edge_list_t*, graph_edge_t);
graph_edge_list_t * append_edge(graph_edge_list_t*, graph_edge_t);
graph_edge_list_t * delete_edge(graph_edge_list_t*, id_t);
graph_edge_list_t * delete_edge_list(graph_edge_list_t*);
graph_edge_list_t * find_edge(graph_edge_list_t*, id_t);
graph_edge_list_t * find_edge_R(graph_edge_list_t*, id_t);
```

Also, there's a particular linked list which stores ID data types (id_t) for both nodes and edges that could get deleted throughout the lifespan of a graph
This enables the recycling of previously used node and edge IDs (NIDs and EIDs), wihch belonged to data that was deleted, and now these can be reused as IDs for newly created data.

Since the <code>id_t</code> is defined as <code>unsigned int</code> and the ID $0$ is reserved as an "ERROR_ID", the maximum amount for both nodes and edges globally 
is 4.294.967.295 ($2^{32} - 1$) unique NIDs and EIDs.

```C
/*
 *  This variable is defined in "graph.c", it's the beginning
 *  node ID (NID) counter which starts at 1 (0 is reserved as an ERROR_ID)
 */
extern id_t global_node_id;         /* Global index counter for nodes */
id_list_t *revoked_node_ids;        /* Stack (FIFO) of node IDs that can be recycled for new nodes */


/*
 *  This variable is defined in "graph.c", it's the beginning
 *  edge ID (EID) counter which starts at 1 (0 is reserved as an ERROR_ID)
 */
extern id_t global_edge_id;         /* Global index counter for edges */
id_list_t *revoked_edge_ids;        /* Stack (FIFO) of edge IDs that can be recycled for new edges */


/* Revoked IDs List Actions */
id_list_t * append_revoked_id(id_list_t*, id_t);
id_list_t * delete_revoked_id(id_list_t*, id_t);
id_list_t * delete_all_revoked_id(id_list_t*);
id_list_t * pop_front_revoked_id(id_list_t*, id_t*);
bool_t      find_revoked_id(id_list_t*, id_t);
bool_t      find_revoked_id_R(id_list_t*, id_t);

```

Then, we have functions related to input/output of graphs and generic actions on graphs

```C
/* I/O */
void                print_node_connections(graph_t*, graph_node_t*);
void                print_graph(graph_t*);
void                print_dijkstra(graph_t*, id_t);
void                print_dijkstra_input(graph_t*);
void                print_graph_matrix(graph_t*);
void                print_all_node_ids(graph_t*);
graph_edge_list_t * input_edge_list(id_t);
graph_node_t        input_node(void);
graph_t *           input_graph(void);
void *              safe_input(const type_t, const unsigned long int, char*);


/* Actions */
graph_node_t   create_new_node(char*);
graph_edge_t   create_new_edge(int, char*, id_t[2]);
graph_t *      create_graph_copy(graph_t*);
graph_node_t * get_node_from_id(graph_t*, id_t);
id_t           get_id_from_node(graph_node_t*);
id_t           get_id_from_node_label(graph_t*, char*);
void           add_new_edges_to_node(graph_t*, id_t, graph_edge_list_t*);
void           change_node_label(graph_t*, id_t, char*);
void           change_edge_label(graph_t*, id_t, char*);
graph_t *      change_duplicated_node_labels(graph_t*, char*);
void           delete_edge_from_node(graph_t*, id_t, id_t);
graph_t *      delete_all_duplicate_edges(graph_t*);

```

The only function that actually needs an explanation is <code>safe_input()</code>, which was included as a safer way to input ALL C base types with the use of a buffer

```C
/* 
 *  ALL types that can be read from user input 
 *  with the "safe_input()" function 
 */
typedef enum type 
{
    SHORT_INT,
    U_SHORT_INT,
    INT,
    U_INT,
    LONG_INT,
    U_LONG_INT,
    FLOAT,
    DOUBLE,
    CHAR,
    STRING
}
type_t;


/*
 *  Returns a void pointer to a memory zone of size "bufsize", thus this function
 *  needs to be first casted as a selected type pointer and then dereferenced if necessary
 *
 *  - Type must be one of the types defined in the enumerator structure above
 *  - Buffer size can be of any dimension, as long as it's enough for the selected data type to input
 *  - Message is an optional string that can be output before reading from stdin 
 *
 *  EXAMPLE:
 *   - Suppose:
 *      - type = INT,
 *      - message = NULL (a.k.a. no message is output to terminal before acquiring input)
 *      - bufsize = 256
 *
 *   // IN file: "main.c"
 *   int a;
 *  
 *   a = *( (*int) safe_input(INT, 256, NULL) );   // Creates memory block of 256B of type "void*", then it's casted as "int*" and
 *                                                 // finally stored in the variable through a dereferencing operation
 *
 *   printf("OUTPUT: %d\n", a);   // prints the variable "a" as an "int"
 *   // END file: "main.c"
 */

void * safe_input(const type_t type, const unsigned long int bufsize, char *message);

```

Then we have file operations, defined as follows:

```C
/* File Operations */
graph_t * load_graph(char*);
void      save_graph(graph_t*, char*);
```

### NOTE:
- The operation <code>save_graph()</code> writes the graph from memory to a file, and <code>load_graph()</code> loads a graph from a file to memory
- Both use a text template to either print or read in order to describe the graph, and such template is the following:

```
src_node_label (edge_count) -> dest_node_label(edge_label, edge_weight),
```

Where:
- src_node_label = The label of the beginning node, from where the edge begins
- edge_count = The amount of edges contained in the beginning node (a.k.a.: edge_count = edges_list_dim(src_node.edges) )
- dest_node_label = The label of the destination node, to where the edge ends
- edge_label = The label of the edge that connects the beginning node to the destination node
- edge_weight = The weight of the edge that connects the beginning node to the destination node

In addition, if a node has more edges in its edge list, the template has the following structure:

**EXAMPLE:**
```
/* Assume the beginning node "node1" has edge_count=2 edges, then it'll have this template */

node1 (2) -> node1(edge1, 1), node2(edge2, 1),

/* Which means that the template is extensible for any finite number n of edges in a node */ 
```

Then there are some miscellaneous operations used to manipulate data or do other stuff
In particular, the last three functions are helper functions needed to perform other functions's tasks (see their implementation for more details)

```C

/* Miscellaneous */
id_t   set_edge_id(void);
id_t   set_node_id(void);
id_t   select_node_id(graph_t*, char*, char*);
int    graph_dim(graph_t*);
int    graph_dim_R(graph_t*);
int    edge_list_dim(graph_edge_list_t*);
int    edge_list_dim_R(graph_edge_list_t*);
int *  create_graph_matrix(graph_t*);
int    autoloop_count(graph_edge_list_t*);
char * filter(char*, char);
char * int_to_string(long int);
char * strconcat(char*, char*);
```

And finally there are some specific operations that involve one or two graphs, which are unary and binary operations
These are the currently available operations on graphs:

```C
/* Unary Graph Operations */
graph_t * vertex_contraction(graph_t*, id_t, id_t);
graph_t * vertex_contraction_input(graph_t*);
graph_t * complement_graph(graph_t*);
graph_t * dijkstra_mst(graph_t*, id_t);
graph_t * dijkstra_mst_input(graph_t*);


/* Binary Graph Operations */
graph_t * disjoint_graph_union(graph_t*, graph_t*);
graph_t * cartesian_graph_product(graph_t*, graph_t*);
graph_t * parallel_graph_composition(graph_t*, graph_t*, id_t, id_t, id_t, id_t);
graph_t * parallel_graph_composition_input(graph_t*, graph_t*);
graph_t * series_graph_composition(graph_t*, graph_t*, id_t, id_t);
graph_t * series_graph_composition_input(graph_t*, graph_t*);
```


- - -
# Additional Information

For any additional detail on the inner workings of each function, just head to its definition in the "graph.c" file where a short explanation is written just above
- - -
