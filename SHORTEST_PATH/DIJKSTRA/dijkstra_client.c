#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include "graph.h"

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
	vertex_t s; 
	int i; 
	status_t status; 

	struct edge E[] = {
		{1, 6, 2.0}, {1, 2, 1.0}, {2, 7, 5.0}, {2, 3, 2.0}, 
		{6, 7, 3.0}, {5, 7, 2.0}, {7, 3, 1.0}, {7, 8, 3.0}, 
		{3, 8, 3.0}, {5, 8, 4.0}, {5, 4, 2.0}, {3, 4, 1.0} 
	}; 

	g = create_graph(); 
	for(v = 1; v <= 8; ++v)
		assert(add_vertex(g, v) == SUCCESS); 

	for(i = 0; i < (int)(sizeof(E) / sizeof(E[0])); ++i)
		assert(add_edge(g, E[i].v_start, E[i].v_end, E[i].w) == SUCCESS); 

	show_graph(g, "INITIAL STATE:"); 

	s = 1; 
	printf("Dijkstra: Source Vertex = %d\n", s); 
	assert(dijkstra(g, s) == SUCCESS); 
	print_all_shortest_paths(g); 

	status = destroy_graph(&g); 
	assert(status == SUCCESS && g == NULL); 

	puts("-------------------------------------------------------"); 

	g = create_graph(); 
	for(v = 1; v <= 7; ++v)
		assert(add_vertex(g, v) == SUCCESS); 

	struct edge E1[] = {
		{1, 2, 2.0}, {2, 3, 3.0}, {3, 4, 2.0}, {4, 5, 2.0}, 
		{5, 6, 4.0}, {6, 7, 1.0}, {7, 1, 4.0}, {3, 7, 1.0}, 
		{5, 7, 2.0}, {4, 6, 1.0}
	}; 

	for(i = 0; i < (int)(sizeof(E1) / sizeof(E1[0])); ++i)
		assert(add_edge(g, E1[i].v_start, E1[i].v_end, E1[i].w) == SUCCESS); 

	show_graph(g, "INITIAL STATE:"); 

	s = 1; 
	printf("Dijkstra: Source Vertex = %d\n", s); 
	assert(dijkstra(g, s) == SUCCESS); 
	print_all_shortest_paths(g); 

	puts("-------------------------------------------------------"); 

	s = 5; 
	printf("Dijkstra: Source Vertex = %d\n", s); 
	assert(dijkstra(g, s) == SUCCESS); 
	print_all_shortest_paths(g); 

	status = destroy_graph(&g); 
	assert(status == SUCCESS && g == NULL); 

	return (EXIT_SUCCESS); 
}
