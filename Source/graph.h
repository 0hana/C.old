//Zero Hanami (C) 2021: graph.h

#ifndef _graph
#define _graph

#include <stdlib.h>
#include "0hana/types.h"

struct graph_link { size_t _(const) Term, Parallels; double _(const) * _(const) Real; };

struct graph_node { size_t _(const) Links; struct graph_link * _(const) Link; };

struct graph { size_t _(const) Nodes; struct graph_node * _(const) Node; };

struct graph
* graph_clone(struct graph const * const Original),
* graph_transpose(struct graph const * const Original);

void graph_free(struct graph * const Memory);

//#include "0hana/graph/random_graph.h"

//BFS_Data --
struct graph_bfs_data { size_t _(const) Start, * _(const) Distance, * _(const) Parent; };

struct graph_bfs_data * graph_bfs(struct graph const * const G, size_t const Start);

void graph_bfs_data_free(struct graph_bfs_data * const BFS_Data);

//DFS_Data --
struct graph_dfs_data { size_t _(const) Start, * _(const) Topological; bool _(const) Acyclic; };

struct graph_dfs_data * graph_dfs(struct graph const * const G, size_t const Start);

void graph_dfs_data_free(struct graph_dfs_data * const DFS_Data);

//SCC_Data --
struct graph_scc_data { struct graph _(const) * _(const) Graph; size_t _(const) Map; };

struct graph_scc_data * graph_scc(struct graph const * const G, struct graph_dfs_data const * const DFS_Data);

void graph_scc_data_free(struct graph_scc_data * const SCC_Data);

//Translation
bool graph_translate_nodes(struct graph * const G, size_t const * const Translation_Array);

#endif//_graph
