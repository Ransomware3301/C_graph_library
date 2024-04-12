/*
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
 * 
 *                  - (BINARY) Disjoint Graph Union
 *                  - (BINARY) Cartesian Graph Product
 *                  - (BINARY) Tensor Graph Product
 *                  - (BINARY) Strong Graph Product
 *                  - (BINARY) Parallel Graph Composition
 *                  - (BINARY) Series Graph Composition
 * 
 *              For a more complete operations list, check this link: 
 *                  https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)
 *
 * 
 *  Made by:    Ransomware3301 (https://www.github.com/ransomware3301)
 *  Date:       14-03-2024
 */


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
    int weight;
    id_t id;
    char *label;
    id_t endpoint_ids[2];
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


id_t global_node_id = 1;            /* Global index counter for nodes */
id_list_t *revoked_node_ids;        /* Stack (FIFO) of node IDs that can be recycled for new nodes */

id_t global_edge_id = 1;            /* Global index counter for nodes */
id_list_t *revoked_edge_ids;        /* Stack (FIFO) of edge IDs that can be recycled for new edges */


/* ==== Function Declarations ==== */


/* I/O */
void                print_node_connections(graph_t*, graph_node_t*);
void                print_graph(graph_t*);
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


/* Miscellaneous */
id_t   set_node_id(void);
id_t   set_edge_id(void);
id_t   select_node_id(graph_t*, char*, char*);
int    graph_dim(graph_t*);
int    graph_dim_R(graph_t*);
int    edge_list_dim(graph_edge_list_t*);
int    edge_list_dim_R(graph_edge_list_t*);
int *  create_graph_matrix(graph_t*);
int    autoloop_count(graph_edge_list_t*);
bool_t exists_node_from_id(id_t);
bool_t exists_edge_from_id(id_t);
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


/* Binary Graph Operations */
graph_t * disjoint_graph_union(graph_t*, graph_t*);
graph_t * cartesian_graph_product(graph_t*, graph_t*);
graph_t * parallel_graph_composition(graph_t*, graph_t*, id_t, id_t, id_t, id_t);
graph_t * parallel_graph_composition_input(graph_t*, graph_t*);
graph_t * series_graph_composition(graph_t*, graph_t*, id_t, id_t);
graph_t * series_graph_composition_input(graph_t*, graph_t*);


/* ==== MAIN (ONLY FOR TESTING) ==== */


int main(int argc, char **argv)
{   
    graph_t *graph1, *graph2;
    graph_t *parallel, *series;
    graph_t *cartesian;
    graph_t *copy1, *copy2;


    graph1 = load_graph("graph1_desc.txt");
    graph2 = load_graph("graph2_desc.txt");

    if (graph1 && graph2)
    {
        printf("\n//////// GRAPH_1 ////////\n");
        print_graph(graph1);
        printf("\n");

        printf("\n//////// GRAPH_2 ////////\n");
        print_graph(graph2);
        printf("\n");


        printf("\n//////// SERIES-PARALLEL COMPOSITIONS ////////\n");
        
        copy1 = create_graph_copy(graph1);
        copy2 = create_graph_copy(graph2);

        series = series_graph_composition(
            copy1, 
            copy2,
            select_node_id(copy1, 
                "\n[CURRENT OPERATION]\n - Series Graph Composition\n" \
                "\nAvailable node IDs (NIDs):\n", 
                "Select source node ID: "
            ),
            select_node_id(copy2, 
                "\n[CURRENT OPERATION]\n - Series Graph Composition\n" \
                "\nAvailable node IDs (NIDs):\n", 
                "Select sink node ID: "
            )
        );
        printf("\n//////// SERIES ////////\n");
        print_graph(series);

        delete_graph(copy1);
        delete_graph(copy2);


        copy1 = create_graph_copy(graph1);
        copy2 = create_graph_copy(graph2);

        parallel = parallel_graph_composition(
            copy1,
            copy2,
            select_node_id(copy1, 
                "\n[CURRENT OPERATION]\n - Parallel Graph Composition\n" \
                "\nAvailable node IDs (NIDs):\n", 
                "Select source node ID of first graph: "
            ),
            select_node_id(copy1, 
                "\n[CURRENT OPERATION]\n - Parallel Graph Composition\n" \
                "\nAvailable node IDs (NIDs):\n", 
                "Select sink node ID of first graph: "
            ),
            select_node_id(copy2,
                "\n[CURRENT OPERATION]\n - Parallel Graph Composition\n" \
                "\nAvailable node IDs (NIDs):\n",  
                "Select source node ID of second graph: "
            ),
            select_node_id(copy2, 
                "\n[CURRENT OPERATION]\n - Parallel Graph Composition\n" \
                "\nAvailable node IDs (NIDs):\n",  
                "Select sink node ID of second graph: "
            )
        );
        printf("\n//////// PARALLEL ////////\n");
        print_graph(parallel);


        printf("\n//////// CARTESIAN PRODUCT ////////\n");
        cartesian = cartesian_graph_product(graph1, graph2);
        print_graph(cartesian);
        printf("\n");


        delete_graph(graph1);
        delete_graph(graph2);
    }

    return 0;
}


/* ==== Function Definitions ==== */


/*
 *  Prints to terminal all the available paths that you can
 *  use to move from the given node to one of its neighbours
 */
void print_node_connections(graph_t *graph, graph_node_t *node)
{
    graph_edge_list_t *ptr;
    graph_node_t *temp;


    if (node)
    {
        printf("\nNode [%s] connections:\n", node->label);
        printf("(NID = 'Node ID', EID = 'Edge ID')\n");

        printf("\n\t[%s] (NID=%d)\n", node->label, node->id);

        if (node->edges)
        {
            ptr = node->edges;

            while (ptr)
            {
                temp = get_node_from_id(graph, ptr->edge.endpoint_ids[1]);

                printf("\t | \n");
                printf("\t | %s\n", ptr->edge.label);
                printf("\t | (W=%d, EID=%u)\n", ptr->edge.weight, ptr->edge.id);
                printf("\t | \n");

                if (temp)
                {
                    printf("\t |-------------> [%s] (NID=%d)\n", temp->label, temp->id);
                }
                else
                {
                    printf("\t |-------------> [NULL]\n");
                }

                ptr = ptr->next;
            }

            printf("\n");
        }
        else
        {
            printf("\t | \n");
            printf("\t |-------------> [NO OUTWARD EDGES]\n\n");
        }
    }
    else
    {
        printf("\n\t[NODE DOESN'T EXIST]\n\n");
    }
}


/*
 *  Prints to terminal all the available nodes in the 
 *  graph, including each node's adjacent edges
 */
void print_graph(graph_t *graph)
{
    graph_t *ptr;


    if (graph)
    {
        for (ptr = graph; ptr != NULL; ptr = ptr->next)
        {
            print_node_connections(graph, &(ptr->node));
        }
    }
    else
    {
        printf("\n\t[EMPTY GRAPH]\n\n");
    }
}


/*
 *  Prints the corresponding matrix of the given graph
 */
void print_graph_matrix(graph_t *graph)
{
    graph_t *ptr;
    int i, j, dim;
    int *mat;


    if (graph)
    {
        mat = create_graph_matrix(graph);

        if (mat)
        {
            dim = graph_dim(graph);
            ptr = graph;

            printf("[NID] ");
            while (ptr)
            {
                printf("[%d]", ptr->node.id);
                ptr = ptr->next;
            }
            printf("\n");

            /* Printing the actual rows of the graph matrix */
            i = 0;
            ptr = graph;

            while (ptr && i < dim)
            {
                printf(" [%d]  ", ptr->node.id);
                j = 0;

                while (j < dim)
                {
                    printf(" %d ", *(mat + j + (i * dim)));
                    j++;
                }

                printf("\n");

                ptr = ptr->next;
                i++;
            }

            printf("\n");
        }
    }
}


/* 
 *  Given a graph, it iterates through all the nodes and
 *  prints each of the available node IDs
 */
void print_all_node_ids(graph_t *graph)
{   
    while (graph)
    {
        printf(" - [%s] (NID=%d)\n", graph->node.label, graph->node.id);
        graph = graph->next;
    }

    printf("\n");
}


/*
 *  Creates an edge list based on user input and returns 
 *  the list head pointer
 */
graph_edge_list_t * input_edge_list(id_t node_id)
{
    graph_edge_list_t *edges;
    int weight; 
    char *label;
    id_t endpoint_ids[2];
    int tmp;
    int quantity;
    int i;


    edges = NULL;

    /* Asking for the quantity of edges to insert next */
    do
    {
        /* printf("Insert amount of edges to input: ");
        scanf("%d", &quantity); */
        quantity = *((int*)safe_input(INT, STRING_BUFFER_SIZE, "Insert amount of edges to input: "));
    } while (quantity < 0);
    
    i = 0;

    while (i < quantity)
    {   
        /* Weight input */
        printf("Insert edge #%d weight: ", i + 1);
        /* scanf("%d", &weight); */
        weight = *((int*)safe_input(INT, STRING_BUFFER_SIZE, NULL));

        /* Label input */
        do
        {
            /* label = get_string_input("Insert edge label: "); */
            label = (char*)safe_input(STRING, STRING_BUFFER_SIZE, "Insert edge label: ");
        } 
        while (strlen(label) == 0);

        /* Endpoint IDs input */
        do
        {
            printf("Insert edge #%d final endpoint ID (SELF_NID=%d): ", i + 1, node_id);
            /* scanf("%d", &tmp); */
            tmp = *((int*)safe_input(INT, STRING_BUFFER_SIZE, NULL));

            if (tmp < 0)
            {
                printf("[input_edge_list()] ERROR: Final Endpoint ID must be positive\n\n");
            }
        } 
        while (tmp < 0);

        endpoint_ids[0] = node_id;
        endpoint_ids[1] = (id_t) tmp;

        edges = append_edge(edges, create_new_edge(weight, label, endpoint_ids));
        i++;
    }

    return edges;
}


/*
 *  Creates a node based on user input and returns
 *  a node element
 */
graph_node_t input_node(void)
{
    graph_node_t node;
    char *label;
    

    printf("\n[NODE]\n");

    /* Label input */
    do
    {
        /* label = get_string_input("Insert node label: "); */
        label = (char*)safe_input(STRING, STRING_BUFFER_SIZE, "Insert node label: ");
    } 
    while (strlen(label) == 0); 

    /* New node creation */
    node = create_new_node(label);
    
    node.edges = input_edge_list(get_id_from_node(&node));

    return node;
}   


/*
 *  Creates a graph based on user input and returns
 *  the list head pointer
 */
graph_t * input_graph(void)
{
    graph_t *graph;
    int graph_dim, i;


    graph = NULL;

    /* Input of graph dimension */
    printf("\n");
    do
    {
        /* printf("Insert graph size: ");
        scanf("%d", &graph_dim); */
        graph_dim = *((int*)safe_input(INT, STRING_BUFFER_SIZE, "Insert graph size: "));
    } 
    while (graph_dim < 0);

    /* Creating the graph */
    for (i = 0; i < graph_dim; i++)
    {
        graph = append_node(graph, input_node());
    }

    return graph;
}


/*
 *  Safe input method that can get an user input in different 
 *  data types (see the "enum type" for the complete list) by
 *  first acquiring user input in a buffer and then formatting it
 *  as specified by the function parameters. Then the function
 *  returns a void pointer that needs to be formatted afterwards
 *  into the correct type
 */
void * safe_input(const type_t type_selector, const unsigned long int bufsize, char *message)
{
    void *result;
    char *buf, *format, *filtered_buf;
    bool_t acquired_input;


    if (bufsize > 0)
    {
        switch (type_selector)
        {
            case SHORT_INT:     format = "%hd";
                                result = (short*)malloc(sizeof(short int));
                                break;
            
            case U_SHORT_INT:   format = "%hu";
                                result = (unsigned short int*)malloc(sizeof(unsigned short int));
                                break;

            case INT:           format = "%d";
                                result = (int*)malloc(sizeof(int));
                                break;
            
            case U_INT:         format = "%u";
                                result = (unsigned int*)malloc(sizeof(unsigned int));
                                break;

            case LONG_INT:      format = "%ld";
                                result = (long int*)malloc(sizeof(long int));
                                break;
            
            case U_LONG_INT:    format = "%lu";
                                result = (unsigned long int*)malloc(sizeof(unsigned long int));
                                break;

            case FLOAT:         format = "%f";
                                result = (float*)malloc(sizeof(float));
                                break;

            case DOUBLE:        format = "%lf";
                                result = (double*)malloc(sizeof(double));
                                break;

            case CHAR:          format = "%c";
                                result = (char*)malloc(sizeof(char));
                                break;

            case STRING:        format = "%s";
                                result = (char*)malloc(sizeof(char) * (bufsize + 1));
                                break;

            default:            format = STRING_EMPTY_PLACEHOLDER;
                                result = NULL;
        }

        if (result)
        {
            if (( buf = (char*)malloc(sizeof(char) * (bufsize + 1)) ))
            {
                acquired_input = false;
                fflush(stdin);

                do
                {
                    if (message)
                    {
                        printf("%s", message);
                    }

                    /* 
                     *  NOTE: 
                     *  -   fgets() puts an END_OF_STRING ('\0') character at the 
                     *      end of the buffer if no errors occur during input
                     */
                    if (fgets(buf, bufsize + 1, stdin))
                    {
                        if (type_selector != STRING)
                        {
                            if (1 == sscanf(buf, format, result))
                            {
                                acquired_input = true;
                            }
                        }
                        else
                        {
                            /* 
                             *  OLD METHOD (which "shouldn't" work but actually does):
                             *
                             *          strcpy(result, buf); 
                             * 
                             *  -   It "shouldn't" work because result is a void*, and not a char*
                             *      and because of this, I believe it works but under an undefined 
                             *      behaviour state which could make the whole copying process unsafe, 
                             *      thus defeating the whole purpouse of the "safe_input()" function.
                             * 
                             *  -   Therefore, the switch from using strcpy() to memcpy() is
                             *      justified by the fact that the latter actually deals with void*
                             *      pointers, that are assumed as being signed char*, thus the use
                             *      of result as a destination is actually expected by the function
                             */
                            filtered_buf = filter(buf, NEWLINE_CHAR);
                            memcpy(result, filtered_buf, strlen(filtered_buf));

                            acquired_input = true;
                        }
                    }
                }
                while ( !acquired_input );

                free(buf);
            }
            else
            {
                printf("[safe_input()] ERROR: Memory allocation was unsuccessful\n");
            }
        }
        else
        {
            if (strcmp(format, STRING_EMPTY_PLACEHOLDER) == 0)
            {
                printf("[safe_input()] ERROR: Memory allocation was unsuccessful\n");
            }
            else
            {
                printf("[safe_input()] ERROR: The selected type does not exist in the list of specified ones\n");
            }
        }
    }
    else
    {
        result = NULL;
        printf("[safe_input()] ERROR: The give buffer size can only be greater than zero\n");
    }

    return result;
}


/*
 *  Given a file pointer containing the graph description, creates the 
 *  graph from such description
 */
graph_t * load_graph(char *filename)
{
    FILE *src;
    graph_t *graph, *ptr;
    char *buf;
    char *src_node_label, *dest_node_label, *edge_label;
    id_t endpoints[2];
    int label_len, edge_count, weight;


    graph = NULL;
    ptr = NULL;

    if (( buf = (char*)malloc(sizeof(char) * (STRING_BUFFER_SIZE + 1)) ))
    {
        if (( src = fopen(filename, "r") ))
        {
            while (1 == fscanf(src, "%s", buf))
            {
                label_len = strlen(buf);
                *(buf + label_len) = END_OF_STRING;

                if (( src_node_label = (char*)malloc(sizeof(char) * (label_len + 1)) ))
                {   
                    strcpy(src_node_label, buf);
                }
                else
                {
                    printf("[load_graph()] ERROR: Memory allocation was unsuccessful\n");
                    strcpy(src_node_label, STRING_EMPTY_PLACEHOLDER);
                }

                graph = append_node(
                    graph, 
                    create_new_node(src_node_label)
                );

                fscanf(src, "%[^\n]", buf);
            }

            rewind(src);

            while (1 == fscanf(src, "%s", buf))
            {
                label_len = strlen(buf);
                *(buf + label_len) = END_OF_STRING;

                if (1 != fscanf(src, " (%d)", &edge_count))
                {
                    edge_count = 0;
                }

                if (ptr == NULL)
                {
                    ptr = graph;
                }
                else
                {
                    ptr = ptr->next;
                }  
  
                if (
                    (1 == fscanf(src, "%s", buf)) 
                    && (0 == strcmp(buf, FILE_NODE_EDGE_SEP_STRING))
                )
                {
                    endpoints[0] = ptr->node.id;

                    while (edge_count)
                    {
                        if (
                            ( dest_node_label = (char*)malloc(sizeof(char) * (STRING_BUFFER_SIZE + 1)) )
                            && ( edge_label = (char*)malloc(sizeof(char) * (STRING_BUFFER_SIZE + 1)) )
                        )
                        {
                            fscanf(src, " %[^(]", dest_node_label);
                            *(dest_node_label + STRING_BUFFER_SIZE) = END_OF_STRING;

                            fscanf(src, "(%[^,]", edge_label);
                            *(edge_label + STRING_BUFFER_SIZE) = END_OF_STRING;                            

                            fscanf(src, ", %[^),]", buf);
                            weight = *(buf) - ZERO_CHAR;

                            if (edge_count > 1)
                            {
                                fscanf(src, "%[^ \n]", buf);
                            }

                            endpoints[1] = get_id_from_node_label(graph, dest_node_label);

                            ptr->node.edges = append_edge(
                                ptr->node.edges,
                                create_new_edge(
                                    weight,
                                    edge_label,
                                    endpoints
                                )
                            );
                        }
                        else
                        {
                            printf("[load_graph()] ERROR: Memory allocation was unsuccessful\n");
                        }

                        edge_count--;
                    }

                    fscanf(src, "%[^\n]", buf);
                }                     
            }

            fclose(src);
        }
        else
        {
            printf("[load_graph()] ERROR: The given file '%s' does not exist\n", filename);
        }
    }
    else
    {
        printf("[load_graph()] ERROR: Memory allocation was unsuccessful\n");
    }

    return graph;
}


/*
 *  Given a graph and a filename, the function saves the graph as follows:
 *      
 *      "src_node_label (src_node_edges_count) -> dest_node_label(edge_label, edge_weight), "
 * 
 *  NOTE:
 *   - If a node has more that one edge, they are saved concatenated on the same line
 * 
 *   - If the provided filename is of an unexisting file, fopen() creates a new file
 *     thus there'll be always a savefile, either already existing or newly created
 */
void save_graph(graph_t *graph, char *filename)
{
    FILE *f;
    graph_t *ptr;
    graph_edge_list_t *ptr2;
    char  *buf;
    long int edge_count;
    id_t dest_node_id;


    if (( buf = (char*)malloc(sizeof(char) * (4 * STRING_BUFFER_SIZE + 1))))
    {
        if (( f = fopen(filename, "w") ))
        {
            ptr = graph;

            while (ptr)
            {
                strcat(buf, ptr->node.label);

                ptr2 = ptr->node.edges;
                edge_count = 0;
                
                while (ptr2)
                {
                    edge_count++;
                    ptr2 = ptr2->next;
                }

                strcat(buf, " (");
                strcat(buf, int_to_string(edge_count));
                strcat(buf, ")");
                strcat(buf, " "FILE_NODE_EDGE_SEP_STRING" ");

                ptr2 = ptr->node.edges;

                while (ptr2)
                {
                    dest_node_id = ptr2->edge.endpoint_ids[1];

                    if (dest_node_id != ERROR_ID)
                    {
                        strcat(buf, (get_node_from_id(graph, dest_node_id))->label);
                        strcat(buf, "(");
                        strcat(buf, ptr2->edge.label);
                        strcat(buf, ", ");
                        strcat(buf, int_to_string(ptr2->edge.weight));
                        strcat(buf, "), ");
                    }
                    
                    ptr2 = ptr2->next;
                }

                strcat(buf, "\n");
                *(buf + 4 * STRING_BUFFER_SIZE) = END_OF_STRING;

                fputs(buf, f);
                *(buf) = END_OF_STRING;
                
                ptr = ptr->next;
            }
            
            fclose(f);
        }
        else
        {
            printf("[save_graph()] ERROR: The given file '%s' does not exist\n", filename);
        }
        
        free(buf);
    }
    else
    {
        printf("[save_file()] ERROR: Memory allocation was unsuccessful\n");
    }
}


/* 
 *  Creates a standalone node (meaning that it has 0 edges at time of creation)
 *  with an additional label 
 * 
 *  (+) Prioritizes the use of revoked_node_ids to initialize the
 *      new node instead of straight up using a new node ID
 */
graph_node_t create_new_node(char *label)
{
    graph_node_t new_node;


    new_node.label = label;
    new_node.edges = NULL;

    if (revoked_node_ids)
    {
        revoked_node_ids = pop_front_revoked_id(revoked_node_ids, &(new_node.id));
    }
    else
    {
        new_node.id = set_node_id();
    }

    return new_node;
}


/*
 *  Creates a new standalone edge element with all its properties 
 *  set with the given parameters
 * 
 *  (+) Prioritizes the use of revoked_edge_ids to initialize the
 *      new edge instead of straight up using a new edge ID
 */
graph_edge_t create_new_edge(int weight, char *label, id_t endpoint_ids[2])
{
    graph_edge_t new_edge;
    char *new_label;
    int len, i;


    len = strlen(label);

    new_edge.weight = weight;
    new_edge.endpoint_ids[0] = endpoint_ids[0];
    new_edge.endpoint_ids[1] = endpoint_ids[1];

    if (revoked_edge_ids)
    {
        revoked_edge_ids = pop_front_revoked_id(revoked_edge_ids, &(new_edge.id));
    }
    else
    {
        new_edge.id = set_edge_id();
    }

    if (( new_label =  (char*)malloc(sizeof(char) * (len + 1)) ))
    {
        for (i = 0; i < len; i++)
        {
            *(new_label + i) = *(label + i);
        }
        *(new_label + len) = END_OF_STRING;

        new_edge.label = new_label;
    }
    else
    {
        printf("[create_new_edge()] ERROR: Memory allocation was unsuccessful\n");
        new_edge.label = NULL;
    }    

    return new_edge;
}


/* 
 *  Given a graph, it returns a new graph that has the same nodes
 *  and edges but with newly attributed node and edge IDs
 */
graph_t * create_graph_copy(graph_t *old_graph)
{
    graph_t *graph, *ptr, *ptr2, *ptr3;
    graph_edge_list_t *ptr4;
    id_t endpoints[2];
    int i, j, k, dim;
    int *old_mat;


    graph = NULL;

    if (old_graph)
    {
        old_mat = create_graph_matrix(old_graph);
        ptr = old_graph;

        while (ptr)
        {       
            graph = append_node(
                graph,
                create_new_node(
                    ptr->node.label
                )
            );

            ptr = ptr->next;
        }

        dim = graph_dim(old_graph);
        ptr = graph;
        i = 0;

        while (ptr)
        {
            endpoints[0] = ptr->node.id;
            ptr2 = graph;
            j = 0;

            while (ptr2)
            {
                if (*(old_mat + j + (i * dim)) == 1)
                {
                    endpoints[1] = ptr2->node.id;
                    ptr3 = old_graph;
                    k = 0;

                    while (ptr3->next && k < i)
                    {
                        ptr3 = ptr3->next;
                        k++;
                    }

                    k = 0;
                    ptr4 = ptr3->node.edges;

                    while (ptr4->next && k < j)
                    {
                        ptr4 = ptr4->next;
                        k++;
                    }

                    ptr->node.edges = append_edge(
                        ptr->node.edges,
                        create_new_edge(
                            ptr4->edge.weight,
                            ptr4->edge.label,
                            endpoints
                        )
                    );
                }

                ptr2 = ptr2->next;
                j++;
            }

            ptr = ptr->next;
            i++;
        }
    }

    return graph;
}


/*
 *  Returns a pointer to the node with the given id if
 *  it is found in the graph, NULL otherwise
 */
graph_node_t * get_node_from_id(graph_t *graph, id_t id)
{
    graph_t *ptr;


    ptr = graph;

    while (ptr && ptr->node.id != id)
    {
        ptr = ptr->next;
    }

    return &(ptr->node);
}


/*
 *  Returns the given node's ID if it exists, 0 (ERROR_ID) otherwise
 */
id_t get_id_from_node(graph_node_t *node)
{
    if (node)
    {
        return node->id;
    }
    else
    {
        return ERROR_ID;
    }
}


/*
 *  Given a node label belonging to a node of the given graph,
 *  it returns the corresponding node ID
 */
id_t get_id_from_node_label(graph_t *graph, char *label)
{
    id_t node_id;
    bool_t found;


    node_id = ERROR_ID;
    found = false;

    while (graph && !found)
    {
        if (strcmp(graph->node.label, label) == 0)
        {
            node_id = graph->node.id;
            found = true;
        }

        graph = graph->next;
    }

    return node_id;
}


/*
 *  Searches the graph for the node with ID 'node_id' and, if it's found, 
 *  adds to the edges list the new edges contained in the new_edges list
 */
void add_new_edges_to_node(graph_t *graph, id_t node_id, graph_edge_list_t *new_edges)
{
    while (graph && (graph->node.id != node_id))
    {
        graph = graph->next;
    }

    if (graph)
    {
        while (new_edges)
        {
            graph->node.edges = append_edge(graph->node.edges, new_edges->edge);
            new_edges = new_edges->next;
        }
    }
}


/*
 *  Searches the graph for the node with ID 'node_id' and, if it's found, 
 *  then it proceeds to change the node's label with the new_label string
 */
void change_node_label(graph_t *graph, id_t node_id, char *new_label)
{
    while (graph && (graph->node.id != node_id))
    {
        graph = graph->next;
    }

    if (graph)
    {
        if (graph->node.label)
        {
            free(graph->node.label);
        }

        graph->node.label = new_label;
    }
}


/*
 *  Searches for each node in the graph the edge with ID 'edge_id' and, if it's 
 *  found, then it proceeds to change the edge's label with the new_label string
 */
void change_edge_label(graph_t *graph, id_t edge_id, char *new_label)
{
    bool_t changed;


    changed = false;

    while (graph && !changed)
    {
        while (graph->node.edges && !changed)
        {
            if (graph->node.edges->edge.id == edge_id)
            {
                if (graph->node.edges->edge.label)
                {
                    free(graph->node.edges->edge.label);
                }
                graph->node.edges->edge.label = new_label;

                changed = true;
            }
            else
            {
                graph->node.edges = graph->node.edges->next;
            }
        }

        graph = graph->next;
    }
}


/*
 *  Given a graph, it finds each duplicated node label  
 *  and substitutes it with an unique label 
 *  (Achieved through the use of the ID as a 'hash' for the node label)
 */
graph_t * change_duplicated_node_labels(graph_t *graph, char *substitute)
{
    graph_t *ptr, *ptr2;


    ptr = graph;

    while (ptr)
    {
        ptr2 = graph;

        while (ptr2)
        {
            if ((ptr != ptr2) && strcmp(ptr->node.label, ptr2->node.label) == 0)
            {
                if (ptr2->node.label)
                {
                    free(ptr2->node.label);
                }

                ptr2->node.label = strconcat(substitute, int_to_string(ptr2->node.id));
            }

            ptr2 = ptr2->next;
        }

        ptr = ptr->next;
    }

    return graph;
}


/*
 *  Searches the graph for the node with ID 'node_id' and, if it's found, then
 *  it searches its edges for the edge with edge ID 'edge_id' and, if it's found,
 *  then proceeds to deleting that edge from the edges list of the node
 */
void delete_edge_from_node(graph_t *graph, id_t node_id, id_t edge_id)
{
    while (graph && (graph->node.id != node_id))
    {
        graph = graph->next;
    }

    if (graph)
    {
        graph->node.edges = delete_edge(graph->node.edges, edge_id);
    }
}


/*
 *  Sets the node ID of the node the function is called on
 *  and proceeds to increment the global_node_id counter 
 */
id_t set_node_id(void)
{
    return global_node_id++;
}


/*
 *  Sets the edge ID of the edge the function is called on
 *  and proceeds to increment the global_edge_id counter 
 */
id_t set_edge_id(void)
{
    return global_edge_id++;
}


/*
 *  Prompts the user to choose a node ID between
 *  the ones present in the given graph
 */
id_t select_node_id(graph_t *graph, char *msg_before_ids, char *msg_after_ids)
{
    id_t nid;

    if (graph)
    {
        if (msg_before_ids)
        {
            printf("%s\n", msg_before_ids);
        }

        print_all_node_ids(graph);

        do
        {
            nid = *((id_t*)safe_input(U_INT, STRING_BUFFER_SIZE, msg_after_ids));
        } 
        while (find_node(graph, nid) == NULL);
    }
    else
    {
        nid = ERROR_ID;
    }

    return nid;
}


/*
 *  Given a starting node, the function returns the number of unique 
 *  nodes that it encounters, thus returning the number of nodes
 */
int graph_dim(graph_t *graph)
{
    graph_t *ptr;
    int len;
    

    ptr = graph;
    len = 0;

    while (ptr)
    {
        len++;
        ptr = ptr->next;
    }

    return len;
}


/*
 *  Given a starting node, the function returns the number of unique 
 *  nodes that it encounters, thus returning the number of nodes
 *
 *  (RECURSIVE VERSION)
 */
int graph_dim_R(graph_t *graph)
{
    if (graph == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + graph_dim_R(graph->next);
    }
}


/*
 *  Given the beginning of an edge list, returns its length
 */
int edge_list_dim(graph_edge_list_t *edges)
{
    int dim;


    dim = 0;

    while (edges)
    {
        dim++;
        edges = edges->next;
    }

    return dim;
}


/*
 *  Given the beginning of an edge list, returns its length
 *
 *  (RECURSIVE VERSION)
 */
int edge_list_dim_R(graph_edge_list_t *edges)
{
    if (edges)
    {
        return 1 + edge_list_dim_R(edges->next);
    }
    else
    {
        return 0;
    }
}


/*
 *  If the graph exists, creates the corresponding matrix of the given graph
 *  otherwise returns NULL
 */
int * create_graph_matrix(graph_t *graph)
{
    graph_t *row_ptr, *col_ptr;
    graph_edge_list_t *row_ptr_edges;
    bool_t found_common_edge;
    int dim, i, j;
    int *mat;
    

    mat = NULL;

    if (graph)
    {
        dim = graph_dim(graph);

        if (( mat = (int*)malloc(sizeof(int) * dim * dim) ))
        {
            row_ptr = graph;
            i = 0;

            while (i < dim && row_ptr)
            {
                col_ptr = graph;
                j = 0;

                while (j < dim && col_ptr)
                {
                    found_common_edge = false;
                    row_ptr_edges = row_ptr->node.edges;

                    while (!found_common_edge && row_ptr_edges)
                    {
                        if (row_ptr_edges->edge.endpoint_ids[1] == col_ptr->node.id)
                        {
                            found_common_edge = true;
                        }

                        row_ptr_edges = row_ptr_edges->next;
                    }

                    if (found_common_edge)
                    {
                        *(mat + j + (i * dim)) = 1;
                    }
                    else
                    {
                        *(mat + j + (i * dim)) = 0;
                    }

                    col_ptr = col_ptr->next;   
                    j++;
                }

                row_ptr = row_ptr->next;
                i++;
            }
            
        }
        else
        {
            printf("[create_graph_matrix()] ERROR: Memory allocation was unsuccessful\n");
        }
    }

    return mat;
}


/* 
 *  Given an edges list, it returns 0 if the node doesn't have an autoloop
 *  and, in case they exist and are duplicated, returns the amount of
 *  autoloops found in the edges list
 */
int autoloop_count(graph_edge_list_t *edges)
{
    int count;


    count = 0;

    while (edges)
    {
        if (edges->edge.endpoint_ids[0] == edges->edge.endpoint_ids[1])
        {
            count++;
        }

        edges = edges->next;
    }

    return count;
}


/*
 *  Looks up if the given node_id exists, meaning that it's
 *  currently in use for a node
 */
bool_t exists_node_from_id(id_t node_id)
{
    id_list_t *ptr;
    bool_t exists;


    exists = false;

    if (node_id <= global_node_id)
    {
        exists = true;
        ptr = revoked_node_ids;

        while (ptr && (ptr->id != node_id))
        {
            ptr = ptr->next;
        }

        if (ptr)
        {
            exists = false;
        }
    }

    return exists;
}


/*
 *  Looks up if the given edge_id exists, meaning that it's
 *  currently in use for an edge
 */
bool_t exists_edge_from_id(id_t edge_id)
{
    id_list_t *ptr;
    bool_t exists;


    exists = false;

    if (edge_id <= global_edge_id)
    {
        exists = true;
        ptr = revoked_edge_ids;

        while (ptr && (ptr->id != edge_id))
        {
            ptr = ptr->next;
        }

        if (ptr)
        {
            exists = false;
        }
    }

    return exists;
}


/*
 *  Filters out all the given 'remove' characters found in str
 */
char * filter(char *str, char remove)
{
    char *filtered;
    int i, j, len;
   

    filtered = NULL;

    if (str)
    {
        len = strlen(str);
        i = 0;

        while ((str + i) && *(str + i) != END_OF_STRING)
        {   
            if ( *(str + i) == remove)
            {
                len--;
            }
            i++;
        }

        if (( filtered = (char*)malloc(sizeof(char) * (len + 1)) ))
        {
            i = 0;
            j = 0;

            while ((str + i) && *(str + i) != END_OF_STRING)
            {
                if (*(str + i) != remove)
                {
                    *(filtered + j) = *(str + i);
                    j++;
                }
                i++;
            }

            *(filtered + len) = END_OF_STRING;
        }
        else
        {
            printf("[filter()] ERROR: Memory allocation was unsuccessful\n");
        }
    }

    return filtered;
}


/*
 *  Given an integer, it converts it into a string
 */
char * int_to_string(long int val)
{   
    char *str;
    int digits, i, digit;
    long int tmp, divider;


    tmp = val;
    digits = 0;

    while (tmp)
    {
        tmp /= 10;
        digits++;
    }

    if (( str = (char*)malloc(sizeof(char) * (digits + 1)) ))
    {
        i = 0;
        tmp = val;

        while (i < digits)
        {
            divider = pow(10, digits - i - 1);
            digit = tmp / divider;

            *(str + i) = ZERO_CHAR + digit;
            tmp -= digit * divider;

            i++;
        }

        *(str + digits) = END_OF_STRING;
    }
    else
    {
        printf("[int_to_string()] ERROR: Memory allocation was unsuccessful\n");
    }

    return str;
}


/*
 *  Given two source strings src1 and src2, it returns the concatenation 
 *  of the beginning of src2 to the end of src1
 */
char * strconcat(char *src1, char *src2)
{
    char *str;
    int len1, len2, len;
    int i;


    str = NULL;

    if (src1 && src2)
    {
        len1 = strlen(src1);
        len2 = strlen(src2);
        len = len1 + len2;

        if (( str = (char*)malloc(sizeof(char) * (len + 1)) ))
        {
            i = 0;

            while (i < len1)
            {
                *(str + i) = *(src1 + i);
                i++;
            }

            i = 0;

            while (i < len2)
            {
                *(str + len1 + i) = *(src2 + i);
                i++;
            }

            *(str + len) = END_OF_STRING;
        }
        else
        {
            printf("[strconcat()] ERROR: Memory allocation was unsuccessful\n");
        }
    }

    return str;
}


/*
 *  Pushes the passed node at the beginning of the graph list
 *  and returns the updated graph 
 */
graph_t * push_node(graph_t *graph, graph_node_t node)
{
    graph_t *elem;


    if (graph)
    {
        if (( elem = (graph_t*)malloc(sizeof(graph_t)) ))
        {
            elem->node = node;
            elem->next = graph;
            graph = elem;
        }
        else
        {
            printf("[push_node()] ERROR: Memory allocation was unsuccessful\n");
        }
    }

    return graph;
}


/* 
 *  Appends the passed node at the end of the graph list
 *  and returns the updated graph 
 */
graph_t * append_node(graph_t *graph, graph_node_t node)
{
    graph_t *elem, *ptr;


    if (( elem = (graph_t*)malloc(sizeof(graph_t)) ))
    {
        if (graph)
        {
            for (ptr = graph; ptr->next != NULL; ptr = ptr->next)
                ;

            elem->node = node;
            elem->next = NULL;
            ptr->next = elem;
        }
        else
        {
            graph = elem;
            graph->next = NULL;
            graph->node = node;
        }        
    }   
    else
    {
        printf("[append_node()] ERROR: Memory allocation was unsuccessful\n");
    }

    return graph;
}


/*
 *  Deletes the node in the graph that matches the given node ID and
 *  then proceeds to return the updated graph
 */
graph_t * delete_node(graph_t *graph, id_t id)
{
    graph_t *prev, *del;


    if (graph)
    {   
        prev = NULL;
        del = graph;

        while (del && del->node.id != id)
        {
            prev = del;
            del = del->next;
        }

        if (del)
        {
            /* Revoke the node ID */
            revoked_node_ids = append_revoked_id(revoked_node_ids, del->node.id);

            if (prev == NULL)
            {
                graph = del->next;
                free(del);
            }
            else
            {
                prev->next = del->next;
                free(del);
            }
        }
    }

    return graph;
}


/*
 *  Deletes all nodes in the graph
 */
graph_t * delete_graph(graph_t *graph)
{
    graph_t *del;
    graph_edge_list_t *ptr;


    while (graph)
    {
        /* Revoking each node ID */
        revoked_node_ids = append_revoked_id(revoked_node_ids, graph->node.id);

        /* Revoking all edge IDs for each node */
        for (ptr = graph->node.edges; ptr != NULL; ptr = ptr->next)
        {
            revoked_edge_ids = append_revoked_id(revoked_edge_ids, ptr->edge.id);
        }

        /* Finally, delete the node from the graph */
        del = graph;
        graph = graph->next;
        free(del);
    }

    return graph;
}


/*
 *  Returns a pointer to the node if it's been found
 *  in the graph, NULL otherwise
 */
graph_t * find_node(graph_t *graph, id_t id)
{
    graph_t *ptr;


    ptr = graph;

    while (ptr && ptr->node.id != id)
    {
        ptr = ptr->next;
    }

    return ptr;
}


/*
 *  Returns a pointer to the node if it's been found
 *  in the graph, NULL otherwise
 * 
 *  (RECURSIVE VERSION)
 */
graph_t * find_node_R(graph_t *ptr, id_t id)
{
    if (ptr)
    {
        if (ptr->node.id == id)
        {
            return ptr;
        }
        else
        {
            return find_node_R(ptr->next, id);
        }  
    }
    else
    {
        return NULL;
    }
}


/*
 *  Pushes the given edge at the beginning of the given edges list
 *  and proceeds to return the updated edges list
 */
graph_edge_list_t * push_edge(graph_edge_list_t *edges, graph_edge_t edge)
{
    graph_edge_list_t *elem;


    if (edges)
    {
        if (( elem = (graph_edge_list_t*)malloc(sizeof(graph_edge_list_t)) ))
        {
            elem->edge = edge;
            elem->next = edges;
            edges = elem;
        }
        else
        {
            printf("[push_edge()] ERROR: Memory allocation was unsuccessful\n");
        }
    }

    return edges;
}


/*
 *  Appends the given edge at the end of the given edges lit
 *  and proceeds to return the updated edges list
 */
graph_edge_list_t * append_edge(graph_edge_list_t *edges, graph_edge_t edge)
{
    graph_edge_list_t *elem, *ptr;


    if (( elem = (graph_edge_list_t*)malloc(sizeof(graph_edge_list_t)) ))
    {
        if (edges)
        {  
            for (ptr = edges; ptr->next != NULL; ptr = ptr->next)
                ;

            elem->edge = edge;
            elem->next = NULL;
            ptr->next = elem;
        }
        else
        {
            edges = elem;
            edges->edge = edge;
            edges->next = NULL;
        }
    }
    else
    {
        printf("[append_edge()] ERROR: Memory allocation was unsuccessful\n");
    }

    return edges;
}


/*
 *  Deletes the edge with ID equal to the passed ID from the passed 
 *  edges list, which is then returned updated
 */
graph_edge_list_t * delete_edge(graph_edge_list_t *edges, id_t id)
{
    graph_edge_list_t *del, *prev;


    if (edges && exists_edge_from_id(id))
    {
        prev = NULL; 
        del = edges;

        while (del && del->edge.id != id)
        {
            prev = del;
            del = del->next;
        }

        if (del)
        {
            /* Revoke this edge's ID */
            revoked_edge_ids = append_revoked_id(revoked_edge_ids, del->edge.id);

            if (prev == NULL)
            {
                edges = del->next;
                free(del);
            }
            else
            {
                prev->next = del->next;
                free(del);
            }
        }
    }

    return edges;
}


/*
 *  Deletes all edges in the given edges list
 */
graph_edge_list_t * delete_edge_list(graph_edge_list_t *edges)
{
    graph_edge_list_t *del;


    if (edges)
    {   
        while (edges)
        {
            /* Revoke each edge's ID */
            revoked_edge_ids = append_revoked_id(revoked_edge_ids, edges->edge.id);

            del = edges;
            edges = edges->next;
            free(del);
        }
    }

    return edges;
}


/*
 *  Returns a pointer to the edge in edges list if the edge with ID id
 *  was found, NULL otherwise
 */
graph_edge_list_t * find_edge(graph_edge_list_t *edges, id_t id)
{
    graph_edge_list_t *ptr;


    ptr = edges;

    while (ptr && ptr->edge.id != id)
    {
        ptr = ptr->next;
    }

    return ptr;
}


/*
 *  Returns a pointer to the edge in edges list if the edge with ID id
 *  was found, NULL otherwise
 * 
 * (RECURSIVE)
 */
graph_edge_list_t * find_edge_R(graph_edge_list_t *ptr, id_t id)
{
    if (ptr)
    {
        if (ptr->edge.id == id)
        {
            return ptr;
        }
        else
        {
            return find_edge_R(ptr->next, id);
        }
    }
    else
    {
        return NULL;
    }
}


/*
 *  Appends the passed ID to the top of the revoked ID stack (FIFO) 
 *  and returns the pointer to the bottom of the updated list.
 */
id_list_t * append_revoked_id(id_list_t *list, id_t id)
{
    id_list_t *ptr, *new_revoked_id;


    if (( new_revoked_id = (id_list_t*)malloc(sizeof(id_list_t)) ))
    {
        if (list)
        {
            for (ptr = list; ptr->next != NULL; ptr = ptr->next)
                ;

            new_revoked_id->id = id;
            new_revoked_id->next = NULL;
            ptr->next = new_revoked_id;
        }
        else
        {
            list = new_revoked_id;
            list->next = NULL;
            list->id = id;
        }
    }
    else
    {
        printf("[append_revoked_id()] ERROR: Memory allocation was unsuccessful\n");
    }

    return list;
}


/*
 *  Deletes the provided ID 'id' from the given ID list
 */
id_list_t * delete_revoked_id(id_list_t *list, id_t id)
{
    id_list_t *del, *prev;


    if (list)
    {
        prev = NULL;
        del = list;

        while (del && del->id != id)
        {
            prev = del;
            del = del->next;
        }    

        if (del)
        {
            if (prev == NULL)
            {
                list = del->next;
                free(del);
            }
            else
            {
                prev->next = del->next;
                free(del);
            }
        }
    }

    return list;
}


/*
 *  Deletes all IDs in the given ID list
 */
id_list_t * delete_all_revoked_id(id_list_t *list)
{
    id_list_t *del;


    while (list)
    {
        del = list;
        list = list->next;
        free(del);
    }

    return list;
}   


/*
 *  Pops the revoked ID that sits at the bottom of the stack (FIFO) 
 *  and puts it in the id parameter. If the list is empty, id contains 0 (ERROR_ID)
 */
id_list_t * pop_front_revoked_id(id_list_t *list, id_t *id)
{
    id_list_t *del;

    if (list)
    {
        *(id) = list->id;

        del = list;
        list = list->next;
        free(del);
    }
    else
    {
        *(id) = ERROR_ID;
    }

    return list;
}


/*
 *  Returns 'true' (1 but bool_t) if the given ID 'id' is found in
 *  the ID list, false (0 but bool_t) otherwise
 */
bool_t find_revoked_id(id_list_t *list, id_t id)
{
    id_list_t *ptr;


    ptr = list;

    if (list)
    {
        while (ptr && ptr->id != id)
        {
            ptr = ptr->next;
        }
    }    

    return (ptr && ptr->id == id);
}


/*
 *  Returns 'true' (1 but bool_t) if the given ID 'id' is found in
 *  the ID list, false (0 but bool_t) otherwise
 * 
 * (RECURSIVE)
 */
bool_t find_revoked_id_R(id_list_t *ptr, id_t id)
{
    if (ptr)
    {
        if (ptr->id == id)
        {
            return true;
        }
        else
        {
            return find_revoked_id_R(ptr->next, id);
        }
    }
    else
    {
        return false;
    }
}


/*
 *  In graph theory, vertex contraction is an operation that, given two nodes, 
 *  combines them and their relative edges into a single node
 *  
 *  Given the two node IDs, where first_node_id is the ID of the merge_node (or the 'surviving' node)
 *  and second_node_id is the ID of the donor_node (or the node that will be deleted), the program
 *  unlinks any edge between each node (in both directions) and also corrects each edge's beginning
 *  or destination node ID of each edge that is either going outwards or inwards the donor_node, thus
 *  relinking each edge to the node merge_node. Finally, the donor_node gets deleted and the vertex
 *  contraction is complete.
 */
/* 
 *  (1.1) - Only performs the vertex contraction operation
 */
graph_t * vertex_contraction(graph_t *graph, id_t first_node_id, id_t second_node_id)
{
    graph_node_t *merge_node, *donor_node;
    graph_edge_list_t *ptr, *ptr2, *ptr3;
    graph_t *ptr4;


    merge_node = get_node_from_id(graph, first_node_id);
    donor_node = get_node_from_id(graph, second_node_id);

    if (merge_node && donor_node)
    {
        /* If it exists, remove the edge pointing from the merge_node to the donor_node */
        ptr = merge_node->edges;

        while (ptr && (ptr->edge.endpoint_ids[1] != second_node_id))
        {
            ptr = ptr->next;
        }

        if (ptr)
        {
            delete_edge_from_node(graph, first_node_id, ptr->edge.id);
        }

        /* If it exists, remove the edge pointing from the donor_node to the merge_node */
        ptr = donor_node->edges;
        
        while (ptr && (ptr->edge.endpoint_ids[1] != first_node_id))
        {
            ptr = ptr->next;
        }

        if (ptr)
        {
            delete_edge_from_node(graph, second_node_id, ptr->edge.id);
        }

        /*
        *  For each edge that points OUTWARDS from the donor_node, append
        *  it to the merge_node and then change each new edge's beginning node ID 
        *  to the merge_node's node ID
        */
        ptr = donor_node->edges;
        ptr2 = merge_node->edges;

        while (ptr)
        {
            ptr2 = append_edge(ptr2, ptr->edge);
            ptr = ptr->next;
        }

        while (ptr2)
        {
            if (ptr2->edge.endpoint_ids[0] != first_node_id)
            {
                if (ptr2->edge.endpoint_ids[0] == ptr2->edge.endpoint_ids[1])
                {
                    ptr2->edge.endpoint_ids[1] = first_node_id;
                }

                ptr2->edge.endpoint_ids[0] = first_node_id;
            }

            ptr2 = ptr2->next;
        }

        /*
        *  For each node in the graph that has an INWARD edge pointing to the 
        *  donor_node, change its edge destination node ID to the merge_node's node ID
        * 
        *  NOTE:   This is the most processing-intensive part of the operation, since
        *          due to the assumption that a graph can be either directed or undirected, 
        *          it's mandatory to check for all edges for each node in the graph.
        */
        ptr4 = graph;

        while (ptr4)
        {
            if (ptr4->node.id != second_node_id)
            {
                ptr3 = ptr4->node.edges;

                while (ptr3)
                {
                    if (ptr3->edge.endpoint_ids[1] == second_node_id)
                    {
                        ptr3->edge.endpoint_ids[1] = first_node_id;
                    }

                    ptr3 = ptr3->next;
                }

                /* Resetting the head of the edges list */
                ptr3 = ptr4->node.edges;
            }

            ptr4 = ptr4->next;
        }

        /* Finally, delete the donor_node and complete the merge */
        graph = delete_node(graph, second_node_id);
    }
    else
    {
        printf("[vertex_contraction()] ERROR: The given node IDs are of non-existing nodes\n");
    }

    return graph;
}


/* 
 *  (1.1) - Only performs the vertex contraction operation
 */
graph_t * vertex_contraction_input(graph_t *graph)
{
    id_t nid1, nid2;


    nid1 = select_node_id(graph, 
        "\nAvailable node IDs (NIDs):\n",
        "Select first node ID: "
    );

    nid2 = select_node_id(graph, 
        "\nAvailable node IDs (NIDs):\n",
        "Select second node ID: "
    );

    return vertex_contraction(graph, nid1, nid2);
}


/*
 *  In the mathematical field of graph theory, the complement or inverse of 
 *  a graph G is a graph H on the same vertices such that two distinct vertices 
 *  of H are adjacent if and only if they are not adjacent in G. 
 * 
 *  That is, to generate the complement of a graph, one fills in all the missing 
 *  edges required to form a complete graph, and removes all the edges that were 
 *  previously there.
 * 
 *  The complement is not the set complement of the graph; only the edges are complemented.
 */
graph_t * complement_graph(graph_t *graph)
{
    graph_t *ptr, *ptr2;
    graph_edge_list_t *template, *edges, *template_ptr;
    id_t endpoints[2];


    ptr = graph;

    while (ptr)
    {
        /* 
         *  Creating the template of all possible edges for each node by
         *  iterating for all possible destinations reachable by a single node
         *  which corresponds to all the nodes in the graph, including itself
         */
        template = NULL;
        ptr2 = graph;
        endpoints[0] = ptr->node.id;

        while (ptr2)
        {   
            endpoints[1] = ptr2->node.id;

            template = append_edge(
                template,
                create_new_edge(
                    COMPLEMENTED_EDGE_DEFAULT_WEIGHT,
                    COMPLEMENTED_EDGE_DEFAULT_LABEL,
                    endpoints
                )
            );
            
            ptr2 = ptr2->next;
        }

        /* 
         *  Iterating through all the old edges so that they are removed 
         *  from the template, thus leaving only the complementary edges list 
         *  that contains all the node's possible destinations except the old ones
         */
        edges = ptr->node.edges;

        while (edges)
        {
            template_ptr = template;

            while (template_ptr)
            {
                if (
                    edges->edge.endpoint_ids[0] == template_ptr->edge.endpoint_ids[0]
                    && edges->edge.endpoint_ids[1] == template_ptr->edge.endpoint_ids[1]
                )
                {
                    template = delete_edge(template, template_ptr->edge.id);
                }

                template_ptr = template_ptr->next;
            }

            edges = edges->next;
        }

        /* 
         *  Now that the template contains only the complementary edges, substitute 
         *  the pointer reference of the node's edges to the template
         */
        ptr->node.edges = delete_edge_list(ptr->node.edges);
        ptr->node.edges = template;

        ptr = ptr->next;
    }

    return graph;
}


/*
 *  In graph theory, a branch of mathematics, the disjoint union of graphs is an operation 
 *  that combines two or more graphs to form a larger graph. It is analogous to the disjoint 
 *  union of sets, and is constructed by making the vertex set of the result be the disjoint
 *  union of the vertex sets of the given graphs, and by making the edge set of the result be 
 *  the disjoint union of the edge sets of the given graphs. Any disjoint union of two or more 
 *  nonempty graphs is necessarily disconnected.
 */
graph_t * disjoint_graph_union(graph_t *graph1, graph_t *graph2)
{
    graph_t *union_graph;


    union_graph = NULL;

    while (graph1)
    {
        union_graph = append_node(union_graph, graph1->node);
        graph1 = graph1->next;
    }

    while (graph2)
    {
        union_graph = append_node(union_graph, graph2->node);
        graph2 = graph2->next;
    }

    return union_graph;
}


/*
 *  In graph theory, the Cartesian product G □ H of graphs G and H is a graph such that:
 *
 *      - the vertex set of G □ H is the Cartesian product V(G) × V(H); and
 *      - two vertices (u,v) and (u' ,v' ) are adjacent in G □ H if and only if either
 *      - u = u' and v is adjacent to v' in H, or
 *      - v = v' and u is adjacent to u' in G.
 * 
 *  The Cartesian product of graphs is sometimes called the box product of graphs [Harary 1969].
 */
graph_t * cartesian_graph_product(graph_t *graph1, graph_t *graph2)
{
    graph_t *cartesian, *copy, *ptr;
    graph_t **layers;
    graph_edge_list_t *ptr2;
    id_t endpoints[2];
    id_t *node_ids;
    int i, j, k, dim1, dim2;
    bool_t found;


    cartesian = NULL;

    if (graph1 && graph2)
    {
        dim1 = graph_dim(graph1);
        dim2 = graph_dim(graph2);

        if (
            ( layers = (graph_t**)malloc(sizeof(graph_t*) * dim1) )
            && ( node_ids = (id_t*)malloc(sizeof(id_t) * dim1) )
        )
        {
            i = 0;

            while (i < dim1)
            {
                copy = create_graph_copy(graph2);
                *(layers + i) = copy;
                ptr = copy;

                while (ptr)
                {
                    cartesian = append_node(cartesian, ptr->node);
                    ptr = ptr->next;
                }
                
                i++;
            }

            /* MISSING: COPYING THE EDGES SCHEMA IN graph1 ONTO THE "j-th layer", WHICH IS A SET OF NODES */

            i = 0;

            while (i < dim2)
            {
                ptr = graph1;
                j = 0;
                
                while (ptr && j < dim1)
                {
                    *(node_ids + j) = ptr->node.id;
                    ptr = ptr->next;
                    j++;
                }

                ptr = graph1;
                j = 0;

                while (ptr && j < dim1)
                {
                    endpoints[0] = (*(layers + j))->node.id;
                    ptr2 = ptr->node.edges;

                    while (ptr2)
                    {
                        found = false;
                        k = 0;

                        while (k < dim1 && !found)
                        {
                            if (node_ids[k] == ptr2->edge.endpoint_ids[1])
                            {
                                found = true;
                            }
                            else
                            {
                                k++;
                            }
                        }

                        if (found)
                        {
                            endpoints[1] = (*(layers + k))->node.id;

                            (*(layers + j))->node.edges = append_edge(
                                (*(layers + j))->node.edges,
                                create_new_edge(
                                    DEFAULT_WEIGHT_CARTESIAN_PRODUCT,
                                    DEFAULT_LABEL_CARTESIAN_PRODUCT,
                                    endpoints
                                )
                            );
                        }

                        ptr2 = ptr2->next;
                    }                    

                    ptr = ptr->next;
                    j++;
                }

                j = 0;

                while (j < dim1)
                {
                    *(layers + j) = (*(layers + j))->next;
                    j++;
                }

                i++;
            }
        }
        else
        {
            printf("[cartesian_graph_product()] ERROR: Memory allocation was unsuccessful\n");
        }        
    }

    return cartesian;
}


/*
 *  A two-terminal graph (TTG) is a graph with two distinguished vertices, s and t called source 
 *  and sink, respectively.
 *  
 *  (1) - The parallel composition Pc = Pc(X,Y) of two TTGs X and Y is a TTG created from the disjoint union 
 *        of graphs X and Y by merging the sources of X and Y to create the source of Pc and merging the 
 *        sinks of X and Y to create the sink of Pc.
 * 
 *  (2) - The series composition Sc = Sc(X,Y) of two TTGs X and Y is a TTG created from the disjoint union of 
 *        graphs X and Y by merging the sink of X with the source of Y. The source of X becomes the source 
 *        of Sc and the sink of Y becomes the sink of Sc.
 * 
 *  A two-terminal series–parallel graph (TTSPG) is a graph that may be constructed by a sequence 
 *  of series and parallel compositions starting from a set of copies of a single-edge graph K2 
 *  with assigned terminals.
 * 
 *  Finally, a graph is called series–parallel (SP-graph), if it is a TTSPG when some two 
 *  of its vertices are regarded as source and sink.
 */
/* 
 *  (1.1) - Only performs the parallel composition operation
 */
graph_t * parallel_graph_composition(graph_t *graph1, graph_t *graph2, id_t source_1, id_t sink_1, id_t source_2, id_t sink_2)
{
    graph_t *union_graph;


    union_graph = NULL;

    if (
        find_node(graph1, source_1) != NULL
        && find_node(graph1, sink_1) != NULL
        && find_node(graph2, source_2) != NULL
        && find_node(graph2, sink_2) != NULL
    )
    {
        printf("\n[CURRENT OPERATION]\n - Parallel Graph Composition\n");

        /* 
         *  (PARALLEL COMPOSITION)
         *  After selecting the source and sink node for both graphs, proceed
         *  to perform a vertex contraction between the two source nodes and
         *  also between the two sink nodes
         */
        union_graph = disjoint_graph_union(graph1, graph2);
        union_graph = vertex_contraction(union_graph, source_1, source_2);
        union_graph = vertex_contraction(union_graph, sink_1, sink_2);
    }
    else
    {
        printf("[parallel_graph_composition()] ERROR: Some of the given IDs don't belong to any of the nodes in either graph\n");
    }
    
    return union_graph;
}

/* 
 *  (1.2) - Gets the source and sink IDs for both graphs and then performs 
 *          the parallel composition operation 
 */
graph_t * parallel_graph_composition_input(graph_t *graph1, graph_t *graph2)
{
    id_t source_ids[2], sink_ids[2];


    if (graph1 && graph2)
    {
        printf("\n[CURRENT OPERATION]\n - Parallel Graph Composition\n");

        /* Source and sink IDs for graph 1 */ 
        source_ids[0] = select_node_id(graph1, 
            "\nAvailable node IDs (NIDs) from the first graph:\n", 
            "Insert the source ID from the first graph: "
        );
        sink_ids[0] = select_node_id(graph1, 
            "\nAvailable node IDs (NIDs) from the first graph:\n", 
            "Insert the sink ID from the first graph: "
        );

        /* Source and sink IDs for graph2 */ 
        source_ids[1] = select_node_id(graph2, 
            "\nAvailable node IDs (NIDs) from the second graph:\n", 
            "Insert the source ID from the second graph: "
        );
        sink_ids[1] = select_node_id(graph2, 
            "\nAvailable node IDs (NIDs) from the second graph:\n", 
            "Insert the sink ID from the second graph: "
        );

        return parallel_graph_composition(graph1, graph2, source_ids[0], sink_ids[0], source_ids[1], sink_ids[1]);
    }

    return NULL;
}


/* 
 *  (2.1) - Only performs the series composition operation
 */
graph_t * series_graph_composition(graph_t *graph1, graph_t *graph2, id_t source_id, id_t sink_id)
{
    graph_t *union_graph;
    graph_node_t *left_node, *right_node;
    id_t endpoints[2];


    union_graph = NULL;

    /* 
     *  (SERIES COMPOSITION)
     *  After selecting the source and sink node for both graphs, proceed
     *  to add an edge (both ways) between the sink_node of the first
     *  graph and the source_node of the second graph
     */
    left_node = get_node_from_id(graph1, source_id);
    right_node = get_node_from_id(graph2, sink_id);

    if (left_node && right_node)
    {
        printf("\n[CURRENT OPERATION]\n - Series Graph Composition\n");

        union_graph = disjoint_graph_union(graph1, graph2);

        /* left_node --[edge]--> right_node */
        endpoints[0] = left_node->id;
        endpoints[1] = right_node->id;

        left_node->edges = append_edge(
            left_node->edges, 
            create_new_edge(
                SERIES_EDGE_DEFAULT_WEIGHT,
                SERIES_EDGE_DEFAULT_LABEL,
                endpoints
            )
        );

        /* right_node --[edge]--> left_node */
        endpoints[0] = right_node->id;
        endpoints[1] = left_node->id;

        right_node->edges = append_edge(
            right_node->edges, 
            create_new_edge(
                SERIES_EDGE_DEFAULT_WEIGHT,
                SERIES_EDGE_DEFAULT_LABEL,
                endpoints
            )
        );
    }
    else
    {
        printf("[series_graph_composition()] ERROR: One or both of the given IDs don't belong to any of the nodes in either graph\n");
    }
    
    return union_graph;
}

/* 
 *  (2.2) - Gets both the source ID from the first graph and sink ID from the second, then 
 *          the series composition operation 
 */
graph_t * series_graph_composition_input(graph_t *graph1, graph_t *graph2)
{
    id_t source_id, sink_id;


    if (graph1 && graph2)
    {
        printf("\n[CURRENT OPERATION]\n - Series Graph Composition\n");

        /* Source ID of graph1 */
        source_id = select_node_id(graph1, 
            "\nAvailable node IDs (NIDs) from the first graph:\n",
            "Insert the source ID from the first graph: "
        );

        /* Sink ID of graph2 */
        sink_id = select_node_id(graph2, 
            "\nAvailable node IDs (NIDs) from the second graph:\n",
            "Insert the sink ID from the second graph: "
        );

        return series_graph_composition(graph1, graph2, source_id, sink_id);
    }

    return NULL;
}
