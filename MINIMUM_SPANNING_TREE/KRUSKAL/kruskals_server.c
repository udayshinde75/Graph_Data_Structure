#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include <math.h> 
#include "kruskals.h"

/* Graph Interface Routines */

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

status_t remove_vertex(graph_t* g, vertex_t r_vertex)
{
	vnode_t* pv_delete = NULL; 
	hnode_t* ph_run = NULL; 
	hnode_t* ph_run_next = NULL; 
	hnode_t* ph_delete_node_in_adj_list = NULL; 

	pv_delete = v_search_node(g->pv_head_node, r_vertex); 
	if(pv_delete == NULL)
		return (G_INVALID_VERTEX); 

	ph_run = pv_delete->ph_head_node->h_next; 
	while(ph_run != pv_delete->ph_head_node)
	{
		ph_run_next = ph_run->h_next; 
		ph_delete_node_in_adj_list = h_search_node(ph_run->pv_node->ph_head_node, r_vertex); 
		h_generic_delete(ph_delete_node_in_adj_list); 
		free(ph_run); 
		g->nr_edges -= 1; 
		ph_run = ph_run_next; 
	}

	free(pv_delete->ph_head_node); 
	v_generic_delete(pv_delete); 
	g->nr_vertices -= 1; 

	return (SUCCESS); 
}

status_t add_edge(graph_t* g, vertex_t v_start, vertex_t v_end, double w)
{
	vnode_t* pv_start = NULL; 
	vnode_t* pv_end = NULL; 
	hnode_t* ph_end_in_start = NULL; 
	hnode_t* ph_start_in_end = NULL; 
	status_t status; 

	pv_start = v_search_node(g->pv_head_node, v_start); 
	if(pv_start == NULL)
		return (G_INVALID_VERTEX); 

	pv_end = v_search_node(g->pv_head_node, v_end); 
	if(pv_end == NULL)
		return (G_INVALID_VERTEX); 

	ph_end_in_start = h_search_node(pv_start->ph_head_node, v_end); 
	ph_start_in_end = h_search_node(pv_end->ph_head_node, v_start); 

	if(ph_end_in_start != NULL && ph_start_in_end != NULL)
		return (G_EDGE_EXISTS); 
	if((ph_end_in_start == NULL) != (ph_start_in_end == NULL))
		return (G_INCONSISTENT); 

	status = h_insert_end(pv_start->ph_head_node, v_end, pv_end, w); 
	assert(status == SUCCESS); 
	status = h_insert_end(pv_end->ph_head_node, v_start, pv_start, w); 
	assert(status == SUCCESS); 

	g->nr_edges += 1; 

	return (SUCCESS); 
}

status_t remove_edge(graph_t* g, vertex_t v_start, vertex_t v_end)
{
	vnode_t* pv_start = NULL; 
	vnode_t* pv_end = NULL; 
	hnode_t* ph_end_in_start = NULL; 
	hnode_t* ph_start_in_end = NULL; 

	pv_start = v_search_node(g->pv_head_node, v_start); 
	if(pv_start == NULL)
		return (G_INVALID_VERTEX); 

	pv_end = v_search_node(g->pv_head_node, v_end); 
	if(pv_end == NULL)
		return (G_INVALID_VERTEX); 

	ph_end_in_start = h_search_node(pv_start->ph_head_node, v_end); 
	ph_start_in_end = h_search_node(pv_end->ph_head_node, v_start); 

	if(ph_end_in_start == NULL && ph_start_in_end == NULL)
		return (G_INVALID_EDGE); 
	if((ph_end_in_start == NULL) != (ph_start_in_end == NULL))
		return (G_INCONSISTENT); 

	h_generic_delete(ph_end_in_start); 
	h_generic_delete(ph_start_in_end); 

	g->nr_edges -= 1; 

	return (SUCCESS); 
}

void show_graph(graph_t* g, const char* msg)
{
	vnode_t* pv_run = NULL; 
	hnode_t* ph_run = NULL; 

	if(msg)
		puts(msg); 

	printf("GRAPH G:|G.V|=%u, |G.E|=%u\n", g->nr_vertices, g->nr_edges); 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
	{
		printf("[%d]\t<->\t", pv_run->v); 
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

/* Kruskal's Algorithm */

status_t kruskals(graph_t* g, edge_vec_t** pp_mst)
{
	edge_vec_t* pE = NULL; 
	edge_vec_t* pA = NULL; 
	dcds_t* p_dcds = NULL; 
	vnode_t* pv_run = NULL; 
	vertex_t repr_u; 
	vertex_t repr_v; 
	int i; 
	status_t status; 

	pE = build_edge_vector(g); 
	ev_sort(pE); 

	pA = ev_create(); 
	p_dcds = dcds_create(); 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
	{
		status = dcds_make_set(p_dcds, pv_run->v); 
		assert(status == SUCCESS); 
	}

	for(i = 0; i < pE->N; ++i)
	{
		repr_u = dcds_find_set(p_dcds, pE->pp_edges[i]->v_start); 
		repr_v = dcds_find_set(p_dcds, pE->pp_edges[i]->v_end); 

		if(repr_u != repr_v)
		{
			ev_push_back(pA, ev_get_edge(pE->pp_edges[i]->v_start, 
										pE->pp_edges[i]->v_end, 
										pE->pp_edges[i]->w)); 
			status = dcds_union(p_dcds, repr_u, repr_v); 
			assert(status == SUCCESS); 
		}
	}

	ev_destroy(&pE); 
	dcds_destroy(&p_dcds); 

	*pp_mst = pA; 
	return (SUCCESS); 
}

void print_mst(edge_vec_t* p_mst)
{
	double total_weight = 0.0; 
	int i; 

	puts("MST Edges:"); 
	for(i = 0; i < p_mst->N; ++i)
	{
		printf("EDGE[%d-%d]:%.2lf\n", p_mst->pp_edges[i]->v_start, 
									p_mst->pp_edges[i]->v_end, 
									p_mst->pp_edges[i]->w); 
		total_weight += p_mst->pp_edges[i]->w; 
	}
	printf("MST Total Weight: %.2lf\n", total_weight); 
}

/* Edge Vector Routines */

edge_vec_t* build_edge_vector(graph_t* g)
{
	edge_vec_t* p_vec = NULL; 
	vnode_t* pv_run = NULL; 
	hnode_t* ph_run = NULL; 

	p_vec = ev_create(); 

	for(pv_run = g->pv_head_node->v_next; pv_run != g->pv_head_node; pv_run = pv_run->v_next)
		for(ph_run = pv_run->ph_head_node->h_next; ph_run != pv_run->ph_head_node; ph_run = ph_run->h_next)
			if(pv_run->v < ph_run->v)
				ev_push_back(p_vec, ev_get_edge(pv_run->v, ph_run->v, ph_run->w)); 

	return (p_vec); 
}

edge_vec_t* ev_create(void)
{
	edge_vec_t* p_vec = NULL; 

	p_vec = (edge_vec_t*)xmalloc(sizeof(edge_vec_t)); 
	p_vec->pp_edges = NULL; 
	p_vec->N = 0; 

	return (p_vec); 
}

status_t ev_push_back(edge_vec_t* p_vec, edge_t* p_edge)
{
	p_vec->pp_edges = (edge_t**)realloc(p_vec->pp_edges, 
										(p_vec->N + 1) * sizeof(edge_t*)); 
	if(p_vec->pp_edges == NULL)
	{
		fprintf(stderr, "fatal:realloc failed in ev_push_back\n"); 
		exit(EXIT_FAILURE); 
	}

	p_vec->pp_edges[p_vec->N] = p_edge; 
	p_vec->N += 1; 

	return (SUCCESS); 
}

void ev_sort(edge_vec_t* p_vec)
{
	int i; 
	int j; 
	edge_t* key = NULL; 

	for(j = 1; j < p_vec->N; ++j)
	{
		key = p_vec->pp_edges[j]; 
		i = j - 1; 
		while(i > -1 && p_vec->pp_edges[i]->w > key->w)
		{
			p_vec->pp_edges[i + 1] = p_vec->pp_edges[i]; 
			i = i - 1; 
		}
		p_vec->pp_edges[i + 1] = key; 
	}
}

void ev_show(edge_vec_t* p_vec, const char* msg)
{
	int i; 

	if(msg)
		puts(msg); 

	for(i = 0; i < p_vec->N; ++i)
		printf("EDGE[%d-%d]:%.2lf\n", p_vec->pp_edges[i]->v_start, 
									p_vec->pp_edges[i]->v_end, 
									p_vec->pp_edges[i]->w); 
}

status_t ev_destroy(edge_vec_t** pp_vec)
{
	edge_vec_t* p_vec = NULL; 
	int i; 

	p_vec = *pp_vec; 

	for(i = 0; i < p_vec->N; ++i)
		free(p_vec->pp_edges[i]); 

	free(p_vec->pp_edges); 
	free(p_vec); 
	*pp_vec = NULL; 

	return (SUCCESS); 
}

edge_t* ev_get_edge(vertex_t v_start, vertex_t v_end, double w)
{
	edge_t* p_edge = NULL; 

	p_edge = (edge_t*)xmalloc(sizeof(edge_t)); 
	p_edge->v_start = v_start; 
	p_edge->v_end = v_end; 
	p_edge->w = w; 

	return (p_edge); 
}

/* DCDS Interface Routines */

dcds_t* dcds_create(void)
{
	dcds_t* p_dcds = NULL; 

	p_dcds = dcds_get_node(NULL); 
	p_dcds->prev = p_dcds; 
	p_dcds->next = p_dcds; 

	return (p_dcds); 
}

status_t dcds_make_set(dcds_t* p_dcds, vertex_t x)
{
	dcds_node_t* p_run = NULL; 
	int i; 

	for(p_run = p_dcds->next; p_run != p_dcds; p_run = p_run->next)
		for(i = 0; i < p_run->p_set->N; ++i)
			if(p_run->p_set->arr[i] == x)
				return (DCDS_ELEMENT_EXISTS); 

	dcds_generic_insert(p_dcds->prev, dcds_get_node(dcds_create_set(x)), p_dcds); 

	return (SUCCESS); 
}

vertex_t dcds_find_set(dcds_t* p_dcds, vertex_t x)
{
	dcds_node_t* p_run = NULL; 
	int i; 

	for(p_run = p_dcds->next; p_run != p_dcds; p_run = p_run->next)
		for(i = 0; i < p_run->p_set->N; ++i)
			if(p_run->p_set->arr[i] == x)
				return (p_run->p_set->repr); 

	return (DCDS_ELEMENT_NOT_FOUND); 
}

status_t dcds_union(dcds_t* p_dcds, vertex_t repr_x, vertex_t repr_y)
{
	dcds_node_t* p_node_x = NULL; 
	dcds_node_t* p_node_y = NULL; 
	dcds_node_t* p_run = NULL; 
	int i; 

	for(p_run = p_dcds->next; p_run != p_dcds; p_run = p_run->next)
	{
		if(p_run->p_set->repr == repr_x)
			p_node_x = p_run; 
		if(p_run->p_set->repr == repr_y)
			p_node_y = p_run; 
	}

	if(p_node_x == NULL || p_node_y == NULL)
		return (DCDS_INVALID_REPR); 

	p_node_x->p_set->arr = (vertex_t*)realloc(p_node_x->p_set->arr, 
								(p_node_x->p_set->N + p_node_y->p_set->N) * sizeof(vertex_t)); 
	if(p_node_x->p_set->arr == NULL)
	{
		fprintf(stderr, "fatal:realloc failed in dcds_union\n"); 
		exit(EXIT_FAILURE); 
	}

	for(i = 0; i < p_node_y->p_set->N; ++i)
		p_node_x->p_set->arr[p_node_x->p_set->N + i] = p_node_y->p_set->arr[i]; 

	p_node_x->p_set->N += p_node_y->p_set->N; 

	dcds_destroy_set(p_node_y->p_set); 
	dcds_generic_delete(p_node_y); 

	return (SUCCESS); 
}

void dcds_show(dcds_t* p_dcds, const char* msg)
{
	dcds_node_t* p_run = NULL; 
	int i; 

	if(msg)
		puts(msg); 

	for(p_run = p_dcds->next; p_run != p_dcds; p_run = p_run->next)
	{
		printf("SET(REPR=%d):\t", p_run->p_set->repr); 
		for(i = 0; i < p_run->p_set->N; ++i)
			printf("[%d]", p_run->p_set->arr[i]); 
		puts("[END]"); 
	}
}

status_t dcds_destroy(dcds_t** pp_dcds)
{
	dcds_t* p_dcds = NULL; 
	dcds_node_t* p_run = NULL; 
	dcds_node_t* p_run_next = NULL; 

	p_dcds = *pp_dcds; 

	for(p_run = p_dcds->next; p_run != p_dcds; p_run = p_run_next)
	{
		p_run_next = p_run->next; 
		dcds_destroy_set(p_run->p_set); 
		free(p_run); 
	}

	free(p_dcds); 
	*pp_dcds = NULL; 

	return (SUCCESS); 
}

/* DCDS Internal Routines */

dcds_set_t* dcds_create_set(vertex_t x)
{
	dcds_set_t* p_set = NULL; 

	p_set = (dcds_set_t*)xmalloc(sizeof(dcds_set_t)); 
	p_set->repr = x; 
	p_set->N = 1; 
	p_set->arr = (vertex_t*)xmalloc(1 * sizeof(vertex_t)); 
	p_set->arr[0] = x; 

	return (p_set); 
}

status_t dcds_destroy_set(dcds_set_t* p_set)
{
	free(p_set->arr); 
	free(p_set); 
	return (SUCCESS); 
}

dcds_node_t* dcds_get_node(dcds_set_t* p_set)
{
	dcds_node_t* p_new_node = NULL; 

	p_new_node = (dcds_node_t*)xmalloc(sizeof(dcds_node_t)); 
	p_new_node->p_set = p_set; 
	p_new_node->prev = NULL; 
	p_new_node->next = NULL; 

	return (p_new_node); 
}

void dcds_generic_insert(dcds_node_t* p_beg, dcds_node_t* p_mid, dcds_node_t* p_end)
{
	p_mid->next = p_end; 
	p_mid->prev = p_beg; 
	p_beg->next = p_mid; 
	p_end->prev = p_mid; 
}

void dcds_generic_delete(dcds_node_t* p_delete_node)
{
	p_delete_node->prev->next = p_delete_node->next; 
	p_delete_node->next->prev = p_delete_node->prev; 
	free(p_delete_node); 
}

/* Vertical List Management */

vlist_t* v_create_list(void)
{
	vnode_t* pv_head_node = NULL; 

	pv_head_node = (vnode_t*)xmalloc(sizeof(vnode_t)); 
	pv_head_node->v = -1; 
	pv_head_node->ph_head_node = NULL; 
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
	pv_new_node->v_prev = NULL; 
	pv_new_node->v_next = NULL; 

	return (pv_new_node); 
}

/* Horizontal List Management */

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
