#ifndef __GUARD_AUX_GRAPH__
#define __GUARD_AUX_GRAPH__

#include <vector>
#include <limits>
#include <iostream>

/*
*   => Defaults for this library
*   -> The graphs follow the following logic:
*   > Each line represents the connection from the i-th vertex
*   > Each column represent the connetion to the j-th vertex
*   -> All functions that return something (except 'AMDirectedGraph::at(.)')
*   have as return values the reference of the first N parameters, where
*   N is the number of returning values
*/

namespace aux
{
  /*
  *   => Adjacency Matrix Graph
  */
  template <typename GridT>
  class AMGraph
  {
  protected:
    std::vector<  std::vector< GridT > > graph;
    std::size_t size;

  public:

    AMGraph<GridT>(std::size_t graph_size)
    {
      this->resize(graph_size);
      return ;
    }

    // > Set edge connection value
    void setEdge(std::size_t i_from, std::size_t i_to, GridT val)
    {
      if(i_from < this->size && i_to < this->size)
        this->graph[i_from][i_to] = val;
      else
      {
        std::string msg = "Failed to add edge: graph of size " +
            std::to_string(this->size) +
            " can't insert connection " + std::to_string(i_from) + "->" +
            std::to_string(i_to);
        throw msg.c_str();
      }
      return ;
    }

    void getSize(std::size_t &val)
    {
      val = size;
      return ;
    }

    void resize(std::size_t graph_size)
    {
      this->size = graph_size;
      graph.resize(this->size);
      for(std::size_t i = 0; i < this->size; i++)
      {
        graph[i].resize(this->size);
        for(std::size_t j = 0; j < this->size; j++)
          graph[i][j] = 0;
      }
      return ;
    }

    void get(GridT &val, std::size_t i, std::size_t j)
    {
      val = this->graph[i][j];
      return ;
    }

    void print()
    {

      for(std::size_t i = 0; i < this->size; i++)
        std::cout << "-------" ;
      std::cout << std::endl;

      std::cout << "|  v  |  ";
      for(std::size_t i = 0; i < this->size; i++)
        std::cout << i << "  |  ";
      std::cout << std::endl;

      for(std::size_t i = 0; i < this->size; i++)
        std::cout << "-------" ;
      std::cout << std::endl;

      for(std::size_t i = 0; i < this->size; i++)
      {
        auto line = this->graph[i];
        std::cout << "|  " << i << "  |  ";
        for(auto elem : line)
          std::cout << elem << "  |  ";
        std::cout << std::endl;
      }

      for(std::size_t i = 0; i < this->size; i++)
        std::cout << "-------" ;
      std::cout << std::endl;

    }

  };

  /*
  *   Undirected Adjacency Matrix Graph
  */
  template <typename GridT>
  class AMUndirectedGraph: public AMGraph<GridT>
  {
  public:

    AMUndirectedGraph<GridT>(std::size_t graph_size=0) : AMGraph<GridT>(graph_size)
    {};

    // > Set edge connection value
    void setEdge(std::size_t i, std::size_t j, GridT val)
    {
      if(i < this->size && j < this->size)
      {
        this->graph[i][j] = val;
        this->graph[j][i] = val;
      }
      else
      {
        std::string msg = "Failed to add edge: graph of size " +
            std::to_string(this->size) +
            " can't insert connection " + std::to_string(i) + "->" +
            std::to_string(j);
        throw msg.c_str();
      }
      return ;
    }

    // > Sums the edge connection value with another
    void addToEdge(std::size_t i, std::size_t j, GridT val)
    {
      if(i < this->size && j < this->size)
      {
        this->graph[i][j] += val;
        this->graph[j][i] += val;
      }
      else
      {
        std::string msg = "Failed to add edge: graph of size " +
            std::to_string(this->size) +
            " can't insert connection " + std::to_string(i) + "->" +
            std::to_string(j);
        throw msg.c_str();
      }
      return ;
    }

    void getGraphDegree(std::vector<std::size_t> &degrees, GridT notEdgeVal=0)
    {
      for(auto line : this->graph)
      {
        std::size_t degree = 0;
        for(auto cost : line)
        {
          if(cost != notEdgeVal)
            degree++;
        }
        degrees.push_back(degree);
      }
      return ;
    }

  };

  /*
  *   Directed Adjacency Matrix Graph
  */
  template <typename GridT>
  class AMDirectedGraph: public AMGraph<GridT>
  {
  public:

    AMDirectedGraph<GridT>(std::size_t graph_size=0) : AMGraph<GridT>(graph_size)
    {}

    // > Sums the edge connection value with another
    void addToEdge(std::size_t i_from, std::size_t i_to, GridT val)
    {
      if(i_from < this->size && i_to < this->size)
        this->graph[i_from][i_to] += val;
      else
      {
        std::string msg = "Failed to add edge: graph of size " +
            std::to_string(this->size) +
            " can't insert connection " + std::to_string(i_from) + "->" +
            std::to_string(i_to);
        throw msg.c_str();
      }
      return ;
    }

    // > Getter and setter: can replace 'setEdge(.)' and 'get(.)'.
    GridT & at(std::size_t i_from, std::size_t i_to)
    {
      return this->graph[i_from][i_to];
    }

  };

  template<typename T>
  void normalizeGraphMaxMin(AMGraph<double> &out_graph, AMGraph<T> graph)
  {
    T max;
    T min;
    std::size_t size = 0;

    graph.get(max, 0, 0);
    graph.get(min, 0, 0);
    graph.getSize(size);

    out_graph.resize(size);

    for(std::size_t i = 0; i < size ; i++)
    {
      for(std::size_t j = 0; j < size ; j++)
      {
        T val;
        graph.get(val, i, j);
        max = (max < val) ? val : max;
        min = (min > val) ? val : min;
      }
    }
    
    for(std::size_t i = 0; i < size ; i++)
    {
      for(std::size_t j = 0; j < size ; j++)
      {
        T val;
        graph.get(val, i, j);
        double norm = ((double)(val - min))/((double)(max - min));
        out_graph.setEdge(i, j, norm);
      }
    }

    return ;
  }

  template<typename T>
  void segmentGraphEdgesBoolean(
    AMGraph<bool> &out_graph,
    AMGraph<T> graph,
    T inf_lim,
    T sup_lim
  )
  {
    std::size_t size;
    graph.getSize(size);
    out_graph.resize(size);
    for(std::size_t i = 0; i < size; i++)
    {
      for(std::size_t j = 0; j < size; j++)
      {
        T val;
        graph.get(val,i, j);
        if(val >= inf_lim && val <= sup_lim)
          out_graph.setEdge(i, j, true);
      }
    }
    return ;
  }


}

#endif
