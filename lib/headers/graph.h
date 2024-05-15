/*
 *  File Type:  C Header File  
 *
 *  Program:    C library to create graphs, a very powerful and useful mathematical tool used to describe, for example, 
 *              mathematical relations, computer networks, finite state machines, social media relations, etc...
 * 
 *              The library enables the user to create both directed and undirected graphs, but since undirected
 *              graphs are a subset of directed graphs (where each pair of node has both an outward and inward edge),
 *              to implement undirected graphs, each undirected edge is comprised of two directed edges that must
 *              be specified separately, one directed edge for each node.
 * 
 *              There are also some unary and binary operations for graphs, such as:
 * 
 *                  - (UNARY) Vertex Contraction
 *                  - (UNARY) Edge Contraction
 *                  - (UNARY) Complement Graph
 *                  - (UNARY) Minimum Spanning Tree (MST) with Dijkstra's Algorithm
 * 
 *                  - (BINARY) Disjoint Graph Union
 *                  - (BINARY) Cartesian Graph Product
 *                  - (BINARY) Parallel Graph Composition
 *                  - (BINARY) Series Graph Composition
 * 
 *              For a more complete operations list, check this link: 
 *                  https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)
 *
 * 
 *  Made by:    Ransomware3301 (https://www.github.com/ransomware3301)
 *  Date:       21-04-2024
 */


#ifndef _GRAPH_H_
#define _GRAPH_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define STRING_BUFFER_SIZE 256
#define STRING_EMPTY_PLACEHOLDER " "
#define FILE_NODE_EDGE_SEP_STRING "->"
#define END_OF_STRING '\0'
#define NEWLINE_CHAR '\n'
#define ZERO_CHAR '0'
#define ERROR_ID 0
#define DEFAULT_COPIED_EDGE_WEIGHT 0
#define DEFAULT_COPIED_EDGE_LABEL "copied_edge"
#define COMPLEMENTED_EDGE_DEFAULT_LABEL "complemented_edge"
#define COMPLEMENTED_EDGE_DEFAULT_WEIGHT 0
#define SERIES_EDGE_DEFAULT_LABEL "series_composition_edge"
#define SERIES_EDGE_DEFAULT_WEIGHT 0
#define DUPLICATED_NODE_DEFAULT_LABEL_PREFIX "duplicated_node_"
#define DEFAULT_LABEL_CARTESIAN_PRODUCT "cartesian_product_edge"
#define DEFAULT_WEIGHT_CARTESIAN_PRODUCT 0


/* ==== Type Definitions ==== */


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


/* ID Type Definition */
typedef unsigned int id_t;


/* Boolean Type Definition */
typedef enum bool
{
    false,
    true
}
bool_t;


/* Revoked ID List Definition */
typedef struct id_list
{
    id_t id;
    struct id_list *next;
}
id_list_t;


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


/* Edge List Definition */
typedef struct graph_edge_list 
{
    graph_edge_t edge;
    struct graph_edge_list *next;
}
graph_edge_list_t;


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


/* Graph Definition */
typedef struct graph 
{
    graph_node_t node;
    struct graph *next;
}
graph_t;


/* ==== Global Variables ==== */


extern id_t global_edge_id;         /* Global index counter for nodes */
id_list_t *revoked_edge_ids;        /* Stack (FIFO) of edge IDs that can be recycled for new edges */


extern id_t global_node_id;         /* Global index counter for nodes */
id_list_t *revoked_node_ids;        /* Stack (FIFO) of node IDs that can be recycled for new nodes */


/* ==== Function Declarations ==== */


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


/* File Operations */
graph_t * load_graph(char*);
void      save_graph(graph_t*, char*);


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


/* Revoked IDs List Actions */
id_list_t * append_revoked_id(id_list_t*, id_t);
id_list_t * delete_revoked_id(id_list_t*, id_t);
id_list_t * delete_all_revoked_id(id_list_t*);
id_list_t * pop_front_revoked_id(id_list_t*, id_t*);
bool_t      find_revoked_id(id_list_t*, id_t);
bool_t      find_revoked_id_R(id_list_t*, id_t);


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


#endif
