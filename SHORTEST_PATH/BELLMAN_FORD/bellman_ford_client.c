#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include "bellman_ford.h"

struct edge
{
	vertex_t v_start; 
	vertex_t v_end; 
	double w; 
}; 

int main(void)
{
	graph_t* g = NULL; 
	edgelist_t* pe_head_node = NULL; 
	vertex_t v; 
	int i; 
	status_t status; 

	struct edge E[] = {
		{1, 6, 2.0}, {1, 2, 1.0}, {2, 7, 5.0}, {2, 3, 2.0}, 
		{6, 7, 3.0}, {5, 7, 2.0}, {7, 3, 1.0}, {7, 8, 3.0}, 
		{4, 2, 3.0}, {4, 8, 5.0}, {3, 8, 3.0}, {5, 8, 4.0}, 
		{5, 4, 2.0}, {3, 4, 1.0}, {8, 1, 2.5}, {8, 6, 3.0}
	}; 

	g = create_graph(); 
	for(v = 1; v <= 8; ++v)
		assert(add_vertex(g, v) == SUCCESS); 

	for(i = 0; i < (int)(sizeof(E) / sizeof(E[0])); ++i)
		assert(add_edge(g, E[i].v_start, E[i].v_end, E[i].w) == SUCCESS); 

	show_graph(g, "INITIAL STATE:"); 

	status = get_edge_list(g, &pe_head_node); 
	assert(status == SUCCESS); 
	show_edge_list(pe_head_node); 
	status = release_edge_list(&pe_head_node); 
	assert(status == SUCCESS && pe_head_node == NULL); 

	printf("Bellman-Ford: Source Vertex = 2\n"); 
	status = bellman_ford(g, 2); 
	if(status == G_NEGATIVE_PATH_CYCLE)
		puts("Bellman-Ford: negative weight cycle detected"); 
	else if(status == SUCCESS)
		print_all_shortest_paths(g); 

	puts("-------------------------------------------------------"); 

	printf("Bellman-Ford: Source Vertex = 5\n"); 
	status = bellman_ford(g, 5); 
	if(status == G_NEGATIVE_PATH_CYCLE)
		puts("Bellman-Ford: negative weight cycle detected"); 
	else if(status == SUCCESS)
		print_all_shortest_paths(g); 

	status = destroy_graph(&g); 
	assert(status == SUCCESS && g == NULL); 

	return (EXIT_SUCCESS); 
}
