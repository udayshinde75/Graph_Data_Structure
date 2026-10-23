#pragma once 

#define SUCCESS 1 
#define G_INVALID_VERTEX 2 
#define G_INVALID_EDGE 3 
#define G_VERTEX_EXISTS 4 
#define G_EDGE_EXISTS 5 
#define G_INCONSISTENT 6 
#define G_NEGATIVE_PATH_CYCLE 7 

#define V_LIST_EMPTY 8

typedef int vertex_t; 
typedef int status_t; 
typedef enum color color_t; 
typedef struct hnode hnode_t; 
typedef hnode_t hlist_t; 
typedef struct vnode vnode_t; 
typedef vnode_t vlist_t; 
typedef struct edge_node edgenode_t; 
typedef edgenode_t edgelist_t; 
typedef struct vq_node vq_node_t; 
typedef vq_node_t vq_list_t; 
typedef vq_list_t vq_stack_t; 
typedef struct graph graph_t; 

enum color
{
	WHITE = 0, 
	GRAY, 
	BLACK
}; 

struct hnode
{
	vertex_t v; 
	vnode_t* pv_node; 
	double w; 
	struct hnode* h_prev; 
	struct hnode* h_next; 
}; 

struct vnode
{
	vertex_t v; 
	hlist_t* ph_head_node; 
	color_t color; 
	double d; 
	struct vnode* pv_pred; 
	struct vnode* v_prev; 
	struct vnode* v_next; 
}; 

struct edge_node
{
	vnode_t* pv_start; 
	vnode_t* pv_end; 
	double w; 
	struct edge_node* e_prev; 
	struct edge_node* e_next; 
}; 

struct vq_node
{
	vnode_t* pv_node; 
	struct vq_node* prev; 
	struct vq_node* next; 
}; 

struct graph
{
	vlist_t* pv_head_node; 
	unsigned int nr_vertices; 
	unsigned int nr_edges; 
}; 

/* Directed Graph Interface Routines */
graph_t* create_graph(void); 
status_t add_vertex(graph_t* g, vertex_t new_vertex); 
status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end, double w); 
status_t remove_vertex(graph_t* g, vertex_t r_vertex); 
status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end); 
void show_graph(graph_t* g, const char* msg); 
status_t destroy_graph(graph_t** pp_g); 

/* Bellman-Ford Interface */
status_t bellman_ford(graph_t* g, vertex_t s); 

/* Bellman-Ford Auxillary Routines */
void initialize_single_source(graph_t* g, vnode_t* pv_s); 
void relax(vnode_t* pv_u, vnode_t* pv_v, double w); 
void print_shortest_path(vnode_t* pv_node); 
void print_all_shortest_paths(graph_t* g); 

/* Edge List Management Routines */
status_t get_edge_list(graph_t* g, edgelist_t** pp_edge_list); 
void show_edge_list(edgelist_t* pe_head_node); 
status_t release_edge_list(edgelist_t** pp_edge_list); 
edgelist_t* e_create_list(void); 
status_t e_insert_end(edgelist_t* pe_head_node, vnode_t* pv_start, vnode_t* pv_end, double w); 
void e_generic_insert(edgenode_t* pe_beg, edgenode_t* pe_mid, edgenode_t* pe_end); 
edgenode_t* e_get_node(vnode_t* pv_start, vnode_t* pv_end, double w); 

/* Vertical List Management Routines */
vlist_t* v_create_list(void); 
status_t v_insert_end(vlist_t* pv_head_node, vertex_t new_vertex); 
void v_generic_insert(vnode_t* pv_beg, vnode_t* pv_mid, vnode_t* pv_end); 
void v_generic_delete(vnode_t* pv_delete_node); 
vnode_t* v_search_node(vlist_t* pv_head_node, vertex_t search_vertex); 
vnode_t* v_get_node(vertex_t new_vertex); 

/* Horizontal List Management Routines */
hlist_t* h_create_list(void); 
status_t h_insert_end(hlist_t* ph_head_node, vertex_t new_vertex, vnode_t* pv_node, double w); 
status_t h_destroy_list(hlist_t** pp_h_list); 
void h_generic_insert(hnode_t* ph_beg, hnode_t* ph_mid, hnode_t* ph_end); 
void h_generic_delete(hnode_t* ph_delete_node); 
hnode_t* h_search_node(hlist_t* ph_head_node, vertex_t search_vertex); 
hnode_t* h_get_node(vertex_t new_vertex, vnode_t* pv_node, double w); 

/* Stack of vnode_t* */
vq_stack_t* vq_create_stack(void); 
status_t vq_push(vq_stack_t* p_stack, vnode_t* pv_node); 
status_t vq_pop(vq_stack_t* p_stack, vnode_t** ppv_node); 
int vq_is_stack_empty(vq_stack_t* p_stack); 
status_t vq_destroy_stack(vq_stack_t** pp_stack); 

/* Underlying List for Stack */
vq_list_t* vq_create_list(void); 
status_t vq_insert_end(vq_list_t* p_list, vnode_t* pv_node); 
status_t vq_remove_end(vq_list_t* p_list, vnode_t** ppv_node); 
int vq_is_list_empty(vq_list_t* p_list); 
status_t vq_destroy_list(vq_list_t** pp_list); 
void vq_generic_insert(vq_node_t* p_beg, vq_node_t* p_mid, vq_node_t* p_end); 
void vq_generic_delete(vq_node_t* p_delete_node); 
vq_node_t* vq_get_node(vnode_t* pv_node); 

/* General Purpose */
void* xmalloc(size_t nr_bytes); 
