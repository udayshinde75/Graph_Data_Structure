#pragma once

#define TRUE	1 
#define FALSE	0 
#define SUCCESS 1 

#define G_VERTEX_EXISTS		2 
#define G_INVALID_VERTEX	3 
#define G_EDGE_EXISTS		4 
#define G_INVALID_EDGE		5
#define G_INCONSISTENT		6

typedef int vertex_t; 
typedef int status_t; 
typedef struct hnode hnode_t; 
typedef hnode_t hlist_t; 
typedef struct vnode vnode_t; 
typedef vnode_t vlist_t; 
typedef struct graph graph_t; 

struct hnode
{
	vertex_t v; 
	struct vnode* pv_node; 
	struct hnode* h_prev; 
	struct hnode* h_next; 
}; 

struct vnode
{
	vertex_t v; 
	hnode_t* ph_head_node; 
	struct vnode* v_prev; 
	struct vnode* v_next; 
};

struct graph
{
	vnode_t* pv_head_node; 
	unsigned long nr_vertices; 
	unsigned long nr_edges; 
};

/* Graph Interface Routines */

graph_t* create_graph(void);

status_t add_vertex(graph_t* g, vertex_t new_vertex); 
status_t remove_vertex(graph_t* g, vertex_t r_vertex); 
status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end); 
status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end); 

void show_graph(graph_t* g, const char* msg); 
status_t destroy_graph(graph_t** pp_g); 

/* Graph helper routines -> Vertical List Management -> Vertical List Interface*/
vlist_t* v_create_list(void); 
status_t v_insert_end(vlist_t* pv_list, vertex_t new_vertex); 

/* Graph helper routines -> Vertical List Management -> Vertical List Helper */
void v_generic_insert(vnode_t* pv_beg, vnode_t* pv_mid, vnode_t* pv_end);
void v_generic_delete(vnode_t* p_delete_node);
vnode_t* v_search_node(vlist_t* pv_list, vertex_t search_vertex);
vnode_t* v_get_list_node(vertex_t new_vertex);


/* Graph helper routines -> Horizontal List Management -> Horizontal list interface*/
hlist_t* h_create_list(void); 
status_t h_insert_end(hlist_t* ph_list, vertex_t new_vertex, vnode_t* pv_node); 

/* Graph helper routines -> Horizontal List Management -> Horizontal list helper*/
void h_generic_insert(hnode_t* ph_beg, hnode_t* ph_mid, hnode_t* ph_end); 
void h_generic_delete(hnode_t* ph_delete_node); 
hnode_t* h_search_node(hlist_t* ph_list, vertex_t search_vertex); 
hnode_t* h_get_list_node(vertex_t new_vertex, vnode_t* pv_node); 

/* Graph helper rouines -> general purpose */
void* xmalloc(size_t nr_bytes); 