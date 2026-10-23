#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include <math.h> 
#include "bellman_ford.h"

/* Directed Graph Interface Routines */

graph_t* create_graph(void)
{
	graph_t* g = NULL; 

	g = (graph_t*)xmalloc(sizeof(graph_t)); 
	g->pv_head_node = v_create_list(); 
	g->nr_vertices = 0; 
	g->nr_edges = 0; 

	return (g); 
}

status_t add_vertex(graph_t* g, vertex_t new_vertex)
{
	vnode_t* pv_node = NULL; 
	status_t status; 

	pv_node = v_search_node(g->pv_head_node, new_vertex); 
	if(pv_node != NULL)
		return (G_VERTEX_EXISTS); 

	status = v_insert_end(g->pv_head_node, new_vertex); 
	assert(status == SUCCESS); 
	g->nr_vertices += 1; 

	return (SUCCESS); 
}

status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end, double w)
{
	vnode_t* pv_start = NULL; 
	vnode_t* pv_end = NULL; 
	hnode_t* ph_end_in_start = NULL; 
	status_t status; 

	pv_start = v_search_node(g->pv_head_node, v_start); 
	if(pv_start == NULL)
		return (G_INVALID_VERTEX); 

	pv_end = v_search_node(g->pv_head_node, v_end); 
	if(pv_end == NULL)
		return (G_INVALID_VERTEX); 

	ph_end_in_start = h_search_node(pv_start->ph_head_node, v_end); 
	if(ph_end_in_start != NULL)
		return (G_EDGE_EXISTS); 

	status = h_insert_end(pv_start->ph_head_node, v_end, pv_end, w); 
	assert(status == SUCCESS); 
	g->nr_edges += 1; 

	return (SUCCESS); 
}

status_t remove_vertex(graph_t* g, vertex_t r_vertex)
{
	vnode_t* pv_delete = NULL; 
	vnode_t* pv_run = NULL; 
	hnode_t* ph_run = NULL; 
	hnode_t* ph_run_next = NULL; 
	hnode_t* ph_incoming = NULL; 

	pv_delete = v_search_node(g->pv_head_node, r_vertex); 
	if(pv_delete == NULL)
		return (G_INVALID_VERTEX); 

	for(ph_run = pv_delete->ph_head_node->h_next; ph_run != pv_delete->ph_head_node; ph_run = ph_run_next)
	{
		ph_run_next = ph_run->h_next; 
		free(ph_run); 
		g->nr_edges -= 1; 
	}

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
	{
		if(pv_run == pv_delete)
			continue; 
		ph_incoming = h_search_node(pv_run->ph_head_node, r_vertex); 
		if(ph_incoming != NULL)
		{
			h_generic_delete(ph_incoming); 
			g->nr_edges -= 1; 
		}
	}

	free(pv_delete->ph_head_node); 
	v_generic_delete(pv_delete); 
	g->nr_vertices -= 1; 

	return (SUCCESS); 
}

status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end)
{
	vnode_t* pv_start = NULL; 
	hnode_t* ph_end_in_start = NULL; 

	pv_start = v_search_node(g->pv_head_node, v_start); 
	if(pv_start == NULL)
		return (G_INVALID_VERTEX); 

	ph_end_in_start = h_search_node(pv_start->ph_head_node, v_end); 
	if(ph_end_in_start == NULL)
		return (G_INVALID_EDGE); 

	h_generic_delete(ph_end_in_start); 
	g->nr_edges -= 1; 

	return (SUCCESS); 
}

void show_graph(graph_t* g, const char* msg)
{
	vnode_t* pv_run = NULL; 
	hnode_t* ph_run = NULL; 

	if(msg)
		puts(msg); 

	printf("DIRECTED GRAPH G:|G.V|=%u, |G.E|=%u\n", g->nr_vertices, g->nr_edges); 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
	{
		printf("[%d]\t->\t", pv_run->v); 
		for(ph_run = pv_run->ph_head_node->h_next; ph_run != pv_run->ph_head_node; ph_run = ph_run->h_next)
			printf("[%d|%.1lf]<->", ph_run->v, ph_run->w); 
		puts("[END]"); 
	}
	puts("--------------------------------------------------------------"); 
}

status_t destroy_graph(graph_t** pp_g)
{
	graph_t* g = NULL; 
	vnode_t* pv_run = NULL; 
	vnode_t* pv_run_next = NULL; 
	status_t status; 

	g = *pp_g; 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run_next)
	{
		pv_run_next = pv_run->v_next; 
		status = h_destroy_list(&pv_run->ph_head_node); 
		assert(status == SUCCESS); 
		free(pv_run); 
	}

	free(g->pv_head_node); 
	free(g); 

	*pp_g = NULL; 

	return (SUCCESS); 
}

/* Bellman-Ford Algorithm */

status_t bellman_ford(graph_t* g, vertex_t s)
{
	vnode_t* pv_s = NULL; 
	edgelist_t* pe_head_node = NULL; 
	edgenode_t* pe_run = NULL; 
	status_t status = SUCCESS; 
	unsigned int i; 

	pv_s = v_search_node(g->pv_head_node, s); 
	if(pv_s == NULL)
		return (G_INVALID_VERTEX); 

	initialize_single_source(g, pv_s); 
	get_edge_list(g, &pe_head_node); 

	for(i = 0; i < g->nr_vertices - 1; ++i)
	{
		for(pe_run = pe_head_node->e_next; pe_run != pe_head_node; pe_run = pe_run->e_next)
			relax(pe_run->pv_start, pe_run->pv_end, pe_run->w); 
	}

	for(pe_run = pe_head_node->e_next; pe_run != pe_head_node; pe_run = pe_run->e_next)
	{
		if(pe_run->pv_end->d > pe_run->pv_start->d + pe_run->w)
		{
			status = G_NEGATIVE_PATH_CYCLE; 
			break; 
		}
	}

	release_edge_list(&pe_head_node); 

	return (status); 
}

/* Bellman-Ford Auxillary Routines */

void initialize_single_source(graph_t* g, vnode_t* pv_s)
{
	vnode_t* pv_run = NULL; 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
	{
		pv_run->d = INFINITY; 
		pv_run->pv_pred = NULL; 
	}

	pv_s->d = 0.0; 
}

void relax(vnode_t* pv_u, vnode_t* pv_v, double w)
{
	if(pv_v->d > pv_u->d + w)
	{
		pv_v->d = pv_u->d + w; 
		pv_v->pv_pred = pv_u; 
	}
}

void print_shortest_path(vnode_t* pv_node)
{
	vq_stack_t* p_stack = NULL; 
	vnode_t* pv_popped = NULL; 
	vnode_t* pv_walk = NULL; 
	vertex_t dest_vertex; 
	double dest_cost; 

	dest_vertex = pv_node->v; 
	dest_cost = pv_node->d; 

	p_stack = vq_create_stack(); 
	for(pv_walk = pv_node; pv_walk != NULL; pv_walk = pv_walk->pv_pred)
		vq_push(p_stack, pv_walk); 

	printf("Shortest path to [%d] : ", dest_vertex); 
	printf("[START]<->"); 
	while(vq_is_stack_empty(p_stack) != 1)
	{
		pv_popped = NULL; 
		vq_pop(p_stack, &pv_popped); 
		printf("[%d]<->", pv_popped->v); 
	}
	printf("[COST:%.2lf]\n", dest_cost); 

	vq_destroy_stack(&p_stack); 
}

void print_all_shortest_paths(graph_t* g)
{
	vnode_t* pv_run = NULL; 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
		print_shortest_path(pv_run); 
}

/* Edge List Management Routines */

status_t get_edge_list(graph_t* g, edgelist_t** pp_edge_list)
{
	vnode_t* pv_run = NULL; 
	hnode_t* ph_run = NULL; 
	edgelist_t* pe_head_node = NULL; 
	status_t status; 

	pe_head_node = e_create_list(); 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
	{
		for(ph_run = pv_run->ph_head_node->h_next; ph_run != pv_run->ph_head_node; ph_run = ph_run->h_next)
		{
			status = e_insert_end(pe_head_node, pv_run, ph_run->pv_node, ph_run->w); 
			assert(status == SUCCESS); 
		}
	}

	*pp_edge_list = pe_head_node; 
	return (SUCCESS); 
}

void show_edge_list(edgelist_t* pe_head_node)
{
	edgenode_t* pe_run = NULL; 

	puts("--------[START EDGE LIST]--------"); 
	for(pe_run = pe_head_node->e_next; pe_run != pe_head_node; pe_run = pe_run->e_next)
		printf("{[%d]->[%d]|%.3lf}\n", pe_run->pv_start->v, pe_run->pv_end->v, pe_run->w); 
	puts("--------[END EDGE LIST]--------"); 
}

status_t release_edge_list(edgelist_t** pp_edge_list)
{
	edgelist_t* pe_head_node = NULL; 
	edgenode_t* pe_run = NULL; 
	edgenode_t* pe_run_next = NULL; 

	pe_head_node = *pp_edge_list; 
	for(pe_run = pe_head_node->e_next; pe_run != pe_head_node; pe_run = pe_run_next)
	{
		pe_run_next = pe_run->e_next; 
		free(pe_run); 
	}

	free(pe_head_node); 
	*pp_edge_list = NULL; 

	return (SUCCESS); 
}

edgelist_t* e_create_list(void)
{
	edgelist_t* pe_head_node = NULL; 

	pe_head_node = (edgelist_t*)xmalloc(sizeof(edgelist_t)); 
	pe_head_node->pv_start = NULL; 
	pe_head_node->pv_end = NULL; 
	pe_head_node->w = 0.0; 
	pe_head_node->e_prev = pe_head_node; 
	pe_head_node->e_next = pe_head_node; 

	return (pe_head_node); 
}

status_t e_insert_end(edgelist_t* pe_head_node, vnode_t* pv_start, vnode_t* pv_end, double w)
{
	e_generic_insert(pe_head_node->e_prev, e_get_node(pv_start, pv_end, w), pe_head_node); 
	return (SUCCESS); 
}

void e_generic_insert(edgenode_t* pe_beg, edgenode_t* pe_mid, edgenode_t* pe_end)
{
	pe_mid->e_next = pe_end; 
	pe_mid->e_prev = pe_beg; 
	pe_beg->e_next = pe_mid; 
	pe_end->e_prev = pe_mid; 
}

edgenode_t* e_get_node(vnode_t* pv_start, vnode_t* pv_end, double w)
{
	edgenode_t* pe_new_node = NULL; 

	pe_new_node = (edgenode_t*)xmalloc(sizeof(edgenode_t)); 
	pe_new_node->pv_start = pv_start; 
	pe_new_node->pv_end = pv_end; 
	pe_new_node->w = w; 
	pe_new_node->e_prev = NULL; 
	pe_new_node->e_next = NULL; 

	return (pe_new_node); 
}

/* Vertical List Management Routines */

vlist_t* v_create_list(void)
{
	vnode_t* pv_head_node = NULL; 

	pv_head_node = (vnode_t*)xmalloc(sizeof(vnode_t)); 
	pv_head_node->v = -1; 
	pv_head_node->ph_head_node = NULL; 
	pv_head_node->color = WHITE; 
	pv_head_node->d = INFINITY; 
	pv_head_node->pv_pred = NULL; 
	pv_head_node->v_prev = pv_head_node; 
	pv_head_node->v_next = pv_head_node; 

	return (pv_head_node); 
}

status_t v_insert_end(vlist_t* pv_head_node, vertex_t new_vertex)
{
	v_generic_insert(pv_head_node->v_prev, v_get_node(new_vertex), pv_head_node); 
	return (SUCCESS); 
}

void v_generic_insert(vnode_t* pv_beg, vnode_t* pv_mid, vnode_t* pv_end)
{
	pv_mid->v_next = pv_end; 
	pv_mid->v_prev = pv_beg; 
	pv_beg->v_next = pv_mid; 
	pv_end->v_prev = pv_mid; 
}

void v_generic_delete(vnode_t* pv_delete_node)
{
	pv_delete_node->v_prev->v_next = pv_delete_node->v_next; 
	pv_delete_node->v_next->v_prev = pv_delete_node->v_prev; 
	free(pv_delete_node); 
}

vnode_t* v_search_node(vlist_t* pv_head_node, vertex_t search_vertex)
{
	vnode_t* pv_run = NULL; 

	for(pv_run = pv_head_node->v_next; pv_run != pv_head_node; pv_run = pv_run->v_next)
		if(pv_run->v == search_vertex)
			return (pv_run); 

	return (NULL); 
}

vnode_t* v_get_node(vertex_t new_vertex)
{
	vnode_t* pv_new_node = NULL; 

	pv_new_node = (vnode_t*)xmalloc(sizeof(vnode_t)); 
	pv_new_node->v = new_vertex; 
	pv_new_node->ph_head_node = h_create_list(); 
	pv_new_node->color = WHITE; 
	pv_new_node->d = INFINITY; 
	pv_new_node->pv_pred = NULL; 
	pv_new_node->v_prev = NULL; 
	pv_new_node->v_next = NULL; 

	return (pv_new_node); 
}

/* Horizontal List Management Routines */

hlist_t* h_create_list(void)
{
	hlist_t* ph_head_node = NULL; 

	ph_head_node = h_get_node(-1, NULL, 0.0); 
	ph_head_node->h_prev = ph_head_node; 
	ph_head_node->h_next = ph_head_node; 

	return (ph_head_node); 
}

status_t h_insert_end(hlist_t* ph_head_node, vertex_t new_vertex, vnode_t* pv_node, double w)
{
	h_generic_insert(ph_head_node->h_prev, h_get_node(new_vertex, pv_node, w), ph_head_node); 
	return (SUCCESS); 
}

status_t h_destroy_list(hlist_t** pp_h_list)
{
	hlist_t* ph_head_node = NULL; 
	hnode_t* ph_run = NULL; 
	hnode_t* ph_run_next = NULL; 

	ph_head_node = *pp_h_list; 
	for(ph_run = ph_head_node->h_next; ph_run != ph_head_node; ph_run = ph_run_next)
	{
		ph_run_next = ph_run->h_next; 
		free(ph_run); 
	}

	free(ph_head_node); 
	*pp_h_list = NULL; 

	return (SUCCESS); 
}

void h_generic_insert(hnode_t* ph_beg, hnode_t* ph_mid, hnode_t* ph_end)
{
	ph_mid->h_next = ph_end; 
	ph_mid->h_prev = ph_beg; 
	ph_beg->h_next = ph_mid; 
	ph_end->h_prev = ph_mid; 
}

void h_generic_delete(hnode_t* ph_delete_node)
{
	ph_delete_node->h_prev->h_next = ph_delete_node->h_next; 
	ph_delete_node->h_next->h_prev = ph_delete_node->h_prev; 
	free(ph_delete_node); 
}

hnode_t* h_search_node(hlist_t* ph_head_node, vertex_t search_vertex)
{
	hnode_t* ph_run = NULL; 

	for(ph_run = ph_head_node->h_next; ph_run != ph_head_node; ph_run = ph_run->h_next)
		if(ph_run->v == search_vertex)
			return (ph_run); 

	return (NULL); 
}

hnode_t* h_get_node(vertex_t new_vertex, vnode_t* pv_node, double w)
{
	hnode_t* ph_new_node = NULL; 

	ph_new_node = (hnode_t*)xmalloc(sizeof(hnode_t)); 
	ph_new_node->v = new_vertex; 
	ph_new_node->pv_node = pv_node; 
	ph_new_node->w = w; 
	ph_new_node->h_prev = NULL; 
	ph_new_node->h_next = NULL; 

	return (ph_new_node); 
}

/* Stack of vnode_t* */

vq_stack_t* vq_create_stack(void)
{
	return (vq_create_list()); 
}

status_t vq_push(vq_stack_t* p_stack, vnode_t* pv_node)
{
	return (vq_insert_end(p_stack, pv_node)); 
}

status_t vq_pop(vq_stack_t* p_stack, vnode_t** ppv_node)
{
	return (vq_remove_end(p_stack, ppv_node)); 
}

int vq_is_stack_empty(vq_stack_t* p_stack)
{
	return (vq_is_list_empty(p_stack)); 
}

status_t vq_destroy_stack(vq_stack_t** pp_stack)
{
	return (vq_destroy_list(pp_stack)); 
}

/* Underlying List for Stack */

vq_list_t* vq_create_list(void)
{
	vq_list_t* p_list = NULL; 

	p_list = vq_get_node(NULL); 
	p_list->prev = p_list; 
	p_list->next = p_list; 

	return (p_list); 
}

status_t vq_insert_end(vq_list_t* p_list, vnode_t* pv_node)
{
	vq_generic_insert(p_list->prev, vq_get_node(pv_node), p_list); 
	return (SUCCESS); 
}

status_t vq_remove_end(vq_list_t* p_list, vnode_t** ppv_node)
{
	if(p_list->prev == p_list && p_list->next == p_list)
		return (V_LIST_EMPTY); 

	*ppv_node = p_list->prev->pv_node; 
	vq_generic_delete(p_list->prev); 

	return (SUCCESS); 
}

int vq_is_list_empty(vq_list_t* p_list)
{
	return (p_list->prev == p_list && p_list->next == p_list); 
}

status_t vq_destroy_list(vq_list_t** pp_list)
{
	vq_list_t* p_list = NULL; 
	vq_node_t* pvq_run = NULL; 
	vq_node_t* pvq_run_next = NULL; 

	p_list = *pp_list; 
	for(pvq_run = p_list->next; pvq_run != p_list; pvq_run = pvq_run_next)
	{
		pvq_run_next = pvq_run->next; 
		free(pvq_run); 
	}

	free(p_list); 
	*pp_list = NULL; 

	return (SUCCESS); 
}

void vq_generic_insert(vq_node_t* p_beg, vq_node_t* p_mid, vq_node_t* p_end)
{
	p_mid->next = p_end; 
	p_mid->prev = p_beg; 
	p_beg->next = p_mid; 
	p_end->prev = p_mid; 
}

void vq_generic_delete(vq_node_t* p_delete_node)
{
	p_delete_node->prev->next = p_delete_node->next; 
	p_delete_node->next->prev = p_delete_node->prev; 
	free(p_delete_node); 
}

vq_node_t* vq_get_node(vnode_t* pv_node)
{
	vq_node_t* p_new_node = NULL; 

	p_new_node = (vq_node_t*)xmalloc(sizeof(vq_node_t)); 
	p_new_node->pv_node = pv_node; 
	p_new_node->prev = NULL; 
	p_new_node->next = NULL; 

	return (p_new_node); 
}

/* General Purpose */

void* xmalloc(size_t nr_bytes)
{
	void* p = NULL; 

	p = malloc(nr_bytes); 
	if(NULL == p)
	{
		fprintf(stderr, "fatal:out of memory\n"); 
		exit(EXIT_FAILURE); 
	}

	return (p); 
}
