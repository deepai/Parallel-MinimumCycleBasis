#ifndef _H_DIJKSTRA
#define _H_DIJKSTRA

#include <queue>

struct dijkstra
{
	int Nodes;
	std::vector<int> distance;
	std::vector<bool> in_tree;
	std::vector<int> edge_offsets;
	std::vector<int> level;
	std::vector<int> parent;

	std::vector<unsigned> *tree_edges;


	csr_multi_graph *graph;

	int *fvs_array;

	struct Compare
	{
		bool operator()(std::pair<int,int> &a,std::pair<int,int> &b)
		{
			return (a.second > b.second);
		}
	};
	std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,Compare> pq; 

	dijkstra(int nodes,csr_multi_graph *input_graph,int *fvs_array)
	{
		Nodes = nodes;
		graph = input_graph;
		distance.resize(nodes);
		in_tree.resize(nodes);
		parent.resize(nodes);
		level.resize(nodes);
		edge_offsets.resize(nodes);

		this->fvs_array = fvs_array;

		for(int i=0;i<nodes;i++)
			distance[i] = -1;
	}
	~dijkstra()
	{
		distance.clear();
		in_tree.clear();
		parent.clear();
		level.clear();
		edge_offsets.clear();
	}


	void reset()
	{
		for(int i=0;i<Nodes;i++)
		{
			distance[i] = -1;
			in_tree[i] = false;
			parent[i] = -1;
			level[i] = -1;
			edge_offsets[i] = -1;
		}	
		tree_edges = NULL;
	}

	void dijkstra_sp(unsigned src)
	{
		tree_edges = new std::vector<unsigned>();

		distance[src] = 0;
		level[src] = 0;
		parent[src] = -1;
		edge_offsets[src] = -1;

		pq.push(std::make_pair(src,0));

		while(!pq.empty())
		{
			std::pair<int,int> val = pq.top();
			pq.pop();

			if(in_tree[val.first])
				continue;

			if(val.first != src)
			{
				tree_edges->push_back(edge_offsets[val.first]);
			}

			in_tree[val.first] = true;

			for(unsigned offset = graph->rowOffsets->at(val.first);
				offset < graph->rowOffsets->at(val.first + 1) ;offset++)
			{
				unsigned column = graph->columns->at(offset);
				if(!in_tree[column])
				{
					int edge_weight = graph->weights->at(offset);
					if(distance[column] == -1)
					{
						distance[column] = distance[val.first] + edge_weight;
						pq.push(std::make_pair(column,distance[column]));
						parent[column] = val.first;
						edge_offsets[column] = offset;
						level[column] = level[val.first] + 1;
					}
					else if(distance[val.first] + edge_weight < distance[column])
					{
						distance[column] = distance[val.first] + edge_weight;
						pq.push(std::make_pair(column,distance[column]));
						parent[column] = val.first;
						edge_offsets[column] = offset;
						level[column] = level[val.first] + 1;
					}
				}
			}
		}

	#ifndef NDEBUG
		assert_correctness(src);
	#endif
	}

	void compute_non_tree_edges(std::vector<unsigned> **non_tree_edges)
	{
		std::vector<unsigned char> is_tree_edge(graph->rows->size());

		for(int i=0;i<tree_edges->size();i++)
			is_tree_edge[tree_edges->at(i)] = 1;

		for(int i=0;i<graph->rows->size();i++)
		{
			if(is_tree_edge[i] == 1)
				continue;
			else if(is_tree_edge[graph->reverse_edge->at(i)] == 1)
			{
				is_tree_edge[i] = 1;
				continue;
			}
			else if(is_tree_edge[graph->reverse_edge->at(i)] == 2)
			{
				is_tree_edge[i] = 2;
				continue;
			}
			else
			{
				is_tree_edge[i] = 2;
				(*non_tree_edges)->push_back(i);
			}
		}
	}

	bool is_edge_cycle(unsigned edge_offset, int &total_weight , unsigned src)
	{
		unsigned row,col,orig_row,orig_col;
		total_weight = 0;

		orig_row = row = graph->rows->at(edge_offset);
		orig_col = col = graph->columns->at(edge_offset);

		if((fvs_array[row] >= 0) && (src > row))
			return false;

		if((fvs_array[col] >= 0) && (src > col))
			return false;

		while(level[row] != level[col])
		{
			if(level[row] < level[col])
				col = parent[col];
			else
				row = parent[row];

			if((fvs_array[row] >= 0) && (src > row))
				return false;

			if((fvs_array[col] >= 0) && (src > col))
				return false;
		}

		if((fvs_array[row] >= 0) && (src > row))
			return false;

		if((fvs_array[col] >= 0) && (src > col))
			return false;

		while(row != col)
		{
			row = parent[row];
			col = parent[col];

			if((fvs_array[row] >= 0) && (src > row))
				return false;

			if((fvs_array[col] >= 0) && (src > col))
				return false;
		}

		if( row == src )
		{
			total_weight += distance[orig_row] + distance[orig_col] + graph->weights->at(edge_offset);
		}

		return (row == src);
	}
	void assert_correctness(unsigned src)
	{
		for(int i=0;i<graph->Nodes;i++)
		{
			if(i == src)
			{
				assert(distance[i] == 0);
				assert(parent[i] == -1);
				assert(level[i] == 0);
				assert(in_tree[i] == true);
			}
			else
			{
				assert(distance[i] > 0);
				assert(parent[i] >= 0);
				assert(level[i] > 0);
				assert(in_tree[i] == true);
			}
		}
	}
};


#endif