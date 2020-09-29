#pragma once

#include <vector>
#include <queue>
#include <valarray>
#include <map>

#include <iosfwd>

#include "Hash.h"

#define _NO_INLINE __declspec(noinline)

template <typename Ty>
class Graph
{
	typedef unsigned uint;

	using map = std::map<Ty, uint>;

	uint point_size;
	uint line_size;

	map table_point_to_index;
	std::vector<Ty> adjacent_of_point;

public:
	Graph(uint N) :point_size(N), line_size(0) {}

};

template <>
class Graph<unsigned>
{
	typedef unsigned uint;

private:

	uint point_size;
	uint line_size;

	std::valarray<std::vector<uint>> adjacent_of_point;

public:

	Graph(uint N) :point_size(N), line_size(0), adjacent_of_point(N) {}
	Graph(std::istream& is)
	{
		is.sync_with_stdio(false);

		uint l_size;
		is >> point_size >> l_size;

		adjacent_of_point.resize(point_size);
		line_size = 0;

		for (uint i = 0; i < l_size; ++i)
		{
			uint point1, point2;
			is >> point1 >> point2;
			link(point1, point2);
		}
	}

	uint p_size()const { return point_size; }
	uint l_size()const { return line_size; }

	void link(uint u, uint v)
	{
		adjacent_of_point[u].push_back(v);
		adjacent_of_point[v].push_back(u);
		line_size++;
	}
	const std::vector<uint>& adjacents(uint point)const
	{
		return adjacent_of_point[point];
	}

	class depth_first_search
	{
	private:
		std::valarray<bool> marked;
		uint cnt_of_connected;
		_NO_INLINE void dfs(const Graph& parent, uint v)
		{
			std::vector<uint> stack;
			stack.push_back(v);
			for (; stack.size();)
			{
				v = stack.back();
				marked[v] = true;
				stack.pop_back();

				const std::vector<uint>& adjs = parent.adjacents(v);
				for (uint u : adjs)
					if (!marked[u])
						stack.push_back(u);
			}
		}
	public:
		depth_first_search(const Graph& parent, uint source) :marked(parent.p_size()), cnt_of_connected(0)
		{
			dfs(parent, source);
		}
		bool is_connected_with(uint v)const { return marked[v]; }
		uint cnt_connected()const { return cnt_of_connected; }
	};

	class breadth_first_search
	{
	private:
		std::valarray<bool> marked;
		std::valarray<uint> edge_to;
		uint source;
		_NO_INLINE void bfs(const Graph& parent, uint v)
		{
			std::queue<uint> the_queue;
			the_queue.push(v);
			marked[v] = true;

			for (; the_queue.size();)
			{
				v = the_queue.front();
				the_queue.pop();

				const std::vector<uint>& adjs = parent.adjacents(v);
				for (uint i = 0; i < adjs.size(); ++i)
					if (!marked[adjs[i]])
					{
						edge_to[adjs[i]] = v;
						marked[adjs[i]] = true;
						the_queue.push(adjs[i]);
					}
			}
		}
	public:
		breadth_first_search(const Graph& parent, uint s) :marked(parent.p_size()), edge_to(parent.p_size()), source(s)
		{
			bfs(parent, source);
		}
		std::vector<uint> shortest_path_to(uint v)
		{
			std::vector<uint> shortest_path;

			for (; v != source;)
			{
				shortest_path.push_back(v);
				v = edge_to[v];
			}
			shortest_path.push_back(source);

			std::reverse(shortest_path.begin(), shortest_path.end());
			return shortest_path;
		}
	};
};

#undef _NO_INLINE

