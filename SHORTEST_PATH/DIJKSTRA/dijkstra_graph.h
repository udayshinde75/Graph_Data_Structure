#pragma once 

#include <stdio.h> 
#include <stdlib.h> 

#define SUCCESS 1 

#define TRUE 1 
#define FALSE 0 

#define G_VERTEX_EXISTS 	2 
#define G_INVALID_VERTEX 	3 
#define G_EDGE_EXISTS 		4 
#define G_INVALID_EDGE 		5 
#define G_INCONSISTENT 		6

#define V_LIST_EMPTY 		7 

typedef int vertex_t; 
typedef int status_t; 
typedef enum color color_t; 
typedef struct hnode hnode_t; 
typedef hnode_t hlist_t; 
typedef struct vnode vnode_t; 
typedef vnode_t vlist_t; 
typedef struct graph graph_t; 

typedef struct vnodeptr_node vq_node_t; 
typedef vq_node_t vq_list_t; 
typedef vq_list_t vq_stack_t; 
typedef vq_list_t vq_prio_queue_t; 

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
	vnode_t* pv_pred; 
	struct vnode* v_prev; 
	struct vnode* v_next; 
}; 

struct graph
{
	vlist_t* pv_head_node; 
	unsigned int nr_vertices; 
	unsigned int nr_edges; 
}; 

struct vnodeptr_node
{
	vnode_t* pv_node; 
	struct vnodeptr_node* prev; 
	struct vnodeptr_node* next; 
}; 

/* Graph Interface Routines */
graph_t* create_graph(void); 
status_t add_vertex(graph_t* g, vertex_t new_vertex); 
status_t remove_vertex(graph_t* g, vertex_t r_vertex); 
status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end, double w); 
status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end); 
void show_graph(graph_t* g, const char* msg); 
status_t destroy_graph(graph_t** pp_g); 

/* Dijkstra Interface */
status_t dijkstra(graph_t* g, vertex_t s); 

/* Dijkstra Auxillary Routines */
void initialize_single_source(graph_t* g, vnode_t* pv_s); 
void relax(vnode_t* pv_u, vnode_t* pv_v, double w); 
void print_shortest_path(graph_t* g, vnode_t* pv_node); 
void print_all_shortest_paths(graph_t* g); 

/* Vertical List Management */
vlist_t* v_create_list(void); 
status_t v_insert_end(vlist_t* pv_head_node, vertex_t new_vertex); 
void v_generic_insert(vnode_t* pv_beg, vnode_t* pv_mid, vnode_t* pv_end); 
void v_generic_delete(vnode_t* pv_delete_node); 
vnode_t* v_search_node(vlist_t* pv_head_node, vertex_t search_vertex); 
vnode_t* v_get_node(vertex_t new_vertex); 

/* Horizontal List Management */
hlist_t* h_create_list(void); 
status_t h_insert_end(hlist_t* ph_head_node, vertex_t new_vertex, vnode_t* pv_node, double w); 
status_t h_destroy_list(hlist_t** pp_h_list); 
void h_generic_insert(hnode_t* ph_beg, hnode_t* ph_mid, hnode_t* ph_end); 
void h_generic_delete(hnode_t* ph_delete_node); 
hnode_t* h_search_node(hlist_t* ph_head_node, vertex_t search_vertex); 
hnode_t* h_get_node(vertex_t new_vertex, vnode_t* pv_node, double w); 

/* Priority Queue of vnode_t* (keyed on d) */
vq_prio_queue_t* vq_create_prio_queue(void); 
status_t vq_prio_enqueue(vq_prio_queue_t* p_prio_queue, vnode_t* pv_node); 
status_t vq_prio_dequeue_min(vq_prio_queue_t* p_prio_queue, vnode_t** ppv_node); 
int vq_is_prio_queue_empty(vq_prio_queue_t* p_prio_queue); 
status_t vq_destroy_prio_queue(vq_prio_queue_t** pp_prio_queue); 

/* Stack of vnode_t* */
vq_stack_t* vq_create_stack(void); 
status_t vq_push(vq_stack_t* p_stack, vnode_t* pv_node); 
status_t vq_pop(vq_stack_t* p_stack, vnode_t** ppv_node); 
int vq_is_stack_empty(vq_stack_t* p_stack); 
status_t vq_destroy_stack(vq_stack_t** pp_stack); 

/* Underlying List for Stack / Priority Queue */
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
