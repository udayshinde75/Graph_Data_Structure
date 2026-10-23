#pragma once
#pragma once

#include <stdio.h> 

#define TRUE	1 
#define FALSE	0 
#define SUCCESS 1 

#define G_VERTEX_EXISTS		2 
#define G_INVALID_VERTEX	3 
#define G_EDGE_EXISTS		4 
#define G_INVALID_EDGE		5
#define QUEUE_EMPTY			6 

typedef int vertex_t;
typedef int status_t;
typedef enum color color_t; 
typedef struct hnode hnode_t;
typedef hnode_t hlist_t;
typedef struct vnode vnode_t;
typedef vnode_t vlist_t;
typedef struct graph graph_t;
typedef struct queue_node queue_node_t; 

enum color { WHITE = 0, GRAY, BLACK	};

struct hnode
{
	vertex_t v;
	struct hnode* h_prev;
	struct hnode* h_next;
};

struct vnode
{
	vertex_t v;
	hnode_t* ph_head_node;
	color_t color; 
	struct vnode* v_prev;
	struct vnode* v_next;
};

struct graph
{
	vnode_t* pv_head_node;
	unsigned long nr_vertices;
	unsigned long nr_edges;
};

struct queue_node
{
	vnode_t* pv_node; 
	struct queue_node* q_prev; 
	struct queue_node* q_next; 
};

/* Graph Interface Routines */

graph_t* create_graph(void);

status_t add_vertex(graph_t* g, vertex_t new_vertex);
status_t remove_vertex(graph_t* g, vertex_t r_vertex);
status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end);
status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end);

void dfs(graph_t* g); 
void bfs(graph_t* g, vertex_t v); 

void show_graph(graph_t* g, const char* msg);
status_t destroy_graph(graph_t** pp_g);

/* graph helper routines */
void reset(graph_t* g); 
void dfs_visit(graph_t* g, vnode_t* pv); 

/* Graph helper routines -> Queue of vnode_t */ 
queue_node_t* create_queue(void); 
status_t enqueue(queue_node_t* p_queue, vnode_t* pv_node); 
status_t dequeue(queue_node_t* p_queue, vnode_t** pp_node); 
int is_queue_empty(queue_node_t* p_queue); 
status_t destroy_queue(queue_node_t* p_queue); 

void q_generic_insert(queue_node_t* p_beg, queue_node_t* p_mid, queue_node_t* p_end); 
void q_generic_delete(queue_node_t* p_delete_node); 
queue_node_t* q_get_node(vnode_t* pv_node); 

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
status_t h_insert_end(hlist_t* ph_list, vertex_t new_vertex);

/* Graph helper routines -> Horizontal List Management -> Horizontal list helper*/
void h_generic_insert(hnode_t* ph_beg, hnode_t* ph_mid, hnode_t* ph_end);
void h_generic_delete(hnode_t* ph_delete_node);
hnode_t* h_search_node(hlist_t* ph_list, vertex_t search_vertex);
hnode_t* h_get_list_node(vertex_t new_vertex);

/* Graph helper rouines -> general purpose */
void* xmalloc(size_t nr_bytes);