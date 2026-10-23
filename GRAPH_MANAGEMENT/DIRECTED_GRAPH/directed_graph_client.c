#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include "directed_graph.h"

#define NR_INIT_VERTICES 8
#define NR_INIT_EDGES 16

struct edge{
	vertex_t v_start; 
	vertex_t v_end; 
	double w; 
}; 

void test_directed_graph(void); 

int main(){
    test_directed_graph();
    return (0); 
}

void test_directed_graph(void){
	graph_t *g = NULL; 
    edgelist_t* p_edge_list; 
	vertex_t v; 
	vertex_t s; 
	int i; 
    status_t status; 

	struct edge E[] = 	{
							{1, 6, 2.0}, {1, 2, 1.0}, {2, 7, 5.0}, {2, 3, 2.0}, 
							{6, 7, 3.0}, {5, 7, 2.0}, {7, 3, 1.0}, {7, 8, 3.0}, 
                            {4, 2, 3.0}, {4, 8, 5.0}, {3, 8, 3.0}, {5, 8, 4.0}, 
                            {5, 4, 2.0}, {3, 4, 1.0}, {8, 1, 2.5}, {8, 6, 3.0}
						}; 

	g = create_graph(); 
	for(v = 1; v <= NR_INIT_VERTICES; ++v)
		add_vertex(g, v); 

	for(i = 0; i < NR_INIT_EDGES; ++i)
		add_edge(g, E[i].v_start, E[i].v_end, E[i].w); 

	show_graph(g, "INITIAL STATE:"); 

    status = get_edge_list(g, &p_edge_list); 
    show_edge_list(p_edge_list); 
    status = release_edge_list(&p_edge_list); 
    assert(!p_edge_list && status); 
    
    status = destroy_graph(&g); 
    assert(!g && status); 
}