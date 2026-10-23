#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include "graph.h"

struct edge
{
	int v_start; 
	int v_end; 
}; 

int main(void)
{
	graph_t* g = NULL; 
	vertex_t V[] = { 1, 2, 3, 4, 5, 6 }; 
	struct edge E[] = { {1, 2}, {1, 6}, {2, 6}, {2, 5}, {2, 3}, {3, 4}, {3, 6}, {3, 5}, {4, 5}, {5, 6} };
	int i; 
	status_t status; 

	g = create_graph(); 

	for (i = 0; i < sizeof(V) / sizeof(V[0]); ++i)
	{
		status = add_vertex(g, V[i]); 
		assert(status == SUCCESS); 
	}

	for (i = 0; i < sizeof(E) / sizeof(E[0]); ++i)
	{
		status = add_edge(g, E[i].v_start, E[i].v_end); 
		assert(status == SUCCESS); 
	}

	show_graph(g, "Initial State:"); 

	status = remove_edge(g, 2, 5); 
	assert(status == SUCCESS); 
	status = remove_edge(g, 2, 3);
	assert(status == SUCCESS);
	status = remove_edge(g, 6, 2);
	assert(status == SUCCESS);
	status = remove_edge(g, 6, 5);
	assert(status == SUCCESS);
		
	show_graph(g, "After removing edges, (2, 5), (2, 3), (6, 2), (6, 5):"); 

	status = remove_vertex(g, 3); 
	assert(status == SUCCESS); 

	show_graph(g, "After removing vertex: 3"); 

	status = destroy_graph(&g); 
	assert(status == SUCCESS && g == NULL); 

	return (0); 
}