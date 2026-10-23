#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include "kruskals.h"

struct init_edge
{
	vertex_t v_start; 
	vertex_t v_end; 
	double w; 
}; 

int main(void)
{
	graph_t* g = NULL; 
	edge_vec_t* p_mst = NULL; 
	vertex_t v; 
	int i; 
	status_t status; 

	struct init_edge E[] = {
		{1, 2, 2.0}, {1, 6, 3.0}, {2, 3, 1.0}, {2, 6, 1.0}, {2, 5, 2.0}, 
		{3, 4, 4.0}, {3, 6, 1.0}, {3, 5, 3.0}, {4, 5, 2.0}, {5, 6, 1.0}
	}; 

	g = create_graph(); 
	for(v = 1; v <= 6; ++v)
		assert(add_vertex(g, v) == SUCCESS); 

	for(i = 0; i < (int)(sizeof(E) / sizeof(E[0])); ++i)
		assert(add_edge(g, E[i].v_start, E[i].v_end, E[i].w) == SUCCESS); 

	show_graph(g, "INITIAL STATE:"); 

	status = kruskals(g, &p_mst); 
	assert(status == SUCCESS); 
	print_mst(p_mst); 

	ev_destroy(&p_mst); 
	status = destroy_graph(&g); 
	assert(status == SUCCESS && g == NULL); 

	return (EXIT_SUCCESS); 
}
