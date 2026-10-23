#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include "prims.h"

struct edge
{
	vertex_t v_start; 
	vertex_t v_end; 
	double w; 
}; 

int main(void)
{
	graph_t* g = NULL; 
	vertex_t v; 
	int i; 
	status_t status; 

	struct edge E[] = {
		{1, 2, 0.5}, {2, 3, 2.0}, {3, 4, 1.5}, {4, 5, 0.7}, {5, 6, 1.2}, 
		{6, 1, 1.0}, {2, 6, 0.5}, {2, 5, 0.9}, {3, 5, 0.8}, {3, 6, 3.0}
	}; 

	g = create_graph(); 
	for(v = 1; v <= 6; ++v)
		assert(add_vertex(g, v) == SUCCESS); 

	for(i = 0; i < (int)(sizeof(E) / sizeof(E[0])); ++i)
		assert(add_edge(g, E[i].v_start, E[i].v_end, E[i].w) == SUCCESS); 

	show_graph(g, "INITIAL STATE:"); 

	status = prims(g, 1); 
	assert(status == SUCCESS); 
	print_mst(g, 1); 

	status = destroy_graph(&g); 
	assert(status == SUCCESS && g == NULL); 

	return (EXIT_SUCCESS); 
}
