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

#define DCDS_ELEMENT_EXISTS 	7 
#define DCDS_ELEMENT_NOT_FOUND 	-1 
#define DCDS_INVALID_REPR 		8 

typedef int vertex_t; 
typedef int status_t; 
typedef struct hnode hnode_t; 
typedef hnode_t hlist_t; 
typedef struct vnode vnode_t; 
typedef vnode_t vlist_t; 
typedef struct graph graph_t; 
typedef struct edge edge_t; 
typedef struct edge_vec edge_vec_t; 
typedef struct dcds_set dcds_set_t; 
typedef struct dcds_node dcds_node_t; 
typedef dcds_node_t dcds_t; 

/* Graph Structures */

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
	struct vnode* v_prev; 
	struct vnode* v_next; 
}; 

struct graph
{
	vlist_t* pv_head_node; 
	unsigned int nr_vertices; 
	unsigned int nr_edges; 
}; 

/* Edge Vector Structures */

struct edge
{
	vertex_t v_start; 
	vertex_t v_end; 
	double w; 
}; 

struct edge_vec
{
	edge_t** pp_edges; 
	int N; 
}; 

/* DCDS Structures */

struct dcds_set
{
	vertex_t* arr; 
	int N; 
	vertex_t repr; 
}; 

struct dcds_node
{
	dcds_set_t* p_set; 
	struct dcds_node* prev; 
	struct dcds_node* next; 
}; 

/* Graph Interface Routines */
graph_t* create_graph(void); 
status_t add_vertex(graph_t* g, vertex_t new_vertex); 
status_t remove_vertex(graph_t* g, vertex_t r_vertex); 
status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end, double w); 
status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end); 
void show_graph(graph_t* g, const char* msg); 
status_t destroy_graph(graph_t** pp_g); 

/* Kruskal's Interface */
status_t kruskals(graph_t* g, edge_vec_t** pp_mst); 
void print_mst(edge_vec_t* p_mst); 

/* Edge Vector Routines */
edge_vec_t* ev_create(void); 
status_t ev_push_back(edge_vec_t* p_vec, edge_t* p_edge); 
void ev_sort(edge_vec_t* p_vec); 
void ev_show(edge_vec_t* p_vec, const char* msg); 
status_t ev_destroy(edge_vec_t** pp_vec); 
edge_t* ev_get_edge(vertex_t v_start, vertex_t v_end, double w); 
edge_vec_t* build_edge_vector(graph_t* g); 

/* DCDS Interface Routines */
dcds_t* dcds_create(void); 
status_t dcds_make_set(dcds_t* p_dcds, vertex_t x); 
vertex_t dcds_find_set(dcds_t* p_dcds, vertex_t x); 
status_t dcds_union(dcds_t* p_dcds, vertex_t repr_x, vertex_t repr_y); 
void dcds_show(dcds_t* p_dcds, const char* msg); 
status_t dcds_destroy(dcds_t** pp_dcds); 

/* DCDS Internal Routines */
dcds_set_t* dcds_create_set(vertex_t x); 
status_t dcds_destroy_set(dcds_set_t* p_set); 
dcds_node_t* dcds_get_node(dcds_set_t* p_set); 
void dcds_generic_insert(dcds_node_t* p_beg, dcds_node_t* p_mid, dcds_node_t* p_end); 
void dcds_generic_delete(dcds_node_t* p_delete_node); 

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

/* General Purpose */
void* xmalloc(size_t nr_bytes); 
