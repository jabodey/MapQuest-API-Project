// Digraph.hpp
//
// ICS 46 Spring 2017
// Project #4: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <queue>

// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException
{
public:
    DigraphException(const std::string& reason): reason_{reason} { }

    std::string reason() const { return reason_; }

private:
    std::string reason_;
};



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a template
// struct.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a template struct.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};


// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//

// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph()
    {
    }

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d)
    {
    adjList = d.adjList;
    }

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d)
    {
    std::swap(d.adjList, adjList);
    }

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph()
    {
    }

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d)
    {
    if (this != &d)
        {
        adjList.clear();
        adjList = d.adjList;
        }
    return *this;
    }

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d)
    {
    std::swap(d.adjList, adjList);
    return *this;
    }

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const
    {
    std::vector<int> verticesVector;
    for(auto i = adjList.begin(); i != adjList.end(); ++i)
        {
        verticesVector.push_back(i->first);
        }
    return verticesVector;
    }

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const
    {
    std::vector<std::pair<int, int>> edgesVector;
    for(auto i = adjList.begin(); i != adjList.end(); ++i)
        {
        DigraphVertex<VertexInfo, EdgeInfo> digraphVertex = i->second;
        std::list<DigraphEdge<EdgeInfo>> linkedList = digraphVertex.edges;
        for(auto j = linkedList.begin(); j != linkedList.end(); ++j)
            {
            edgesVector.push_back(std::make_pair(j->fromVertex, j->toVertex));
            }
        }
    return edgesVector;
    }

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const
    {
    std::vector<std::pair<int, int>> edgesVector;
    for(auto i = adjList.begin(); i != adjList.end(); ++i)
        {
        DigraphVertex<VertexInfo, EdgeInfo> digraphVertex = i->second;
        std::list<DigraphEdge<EdgeInfo>> linkedList = digraphVertex.edges;
        for(auto j = linkedList.begin(); j != linkedList.end(); ++j)
            {
            if (j->fromVertex == vertex)
                {
                edgesVector.push_back(std::make_pair(j->fromVertex, j->toVertex));
                }
            }
        }
    if(edgesVector.empty())
        {
        throw DigraphException("Vertex does not exist");
        }
    return edgesVector;
    }

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const
    {
    if(adjList.find(vertex) != adjList.end())
        {
        return adjList.at(vertex).vinfo;
        }
    else
        {
        throw DigraphException("Vertex does not exist.");
        }
    }

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const
    {
    if(adjList.find(fromVertex) != adjList.end())
        {
        std::list<DigraphEdge<EdgeInfo>> linkedList = adjList.at(fromVertex).edges;
        for(auto i = linkedList.begin(); i != linkedList.end(); ++i)
        //for(auto i = adjList.at(fromVertex).edges.begin(); i != adjList.at(fromVertex).edges.end(); ++i)
            {
            if(i->toVertex == toVertex)
                {
                return i->einfo;
                }
            }
        throw DigraphException("Ingoing vertex does not exist.");
        }
    else
        {
        throw DigraphException("Outgoing vertex does not exist.");
        }
    }

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo)
    {
    if(adjList.find(vertex) == adjList.end())
        {
        DigraphVertex<VertexInfo, EdgeInfo> digraphVertex;
        adjList[vertex] =  digraphVertex;
        adjList[vertex].vinfo = vinfo;
        
        }
    else
        {
        throw DigraphException("Vertex is already in the Digraph.");
        }
    }

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
    {
    if(adjList.find(fromVertex) != adjList.end())
        {
        std::list<DigraphEdge<EdgeInfo>> linkedList = adjList[fromVertex].edges;
        for(auto i = linkedList.begin(); i != linkedList.end(); ++i)
        //for(auto i = adjList[fromVertex].edges.begin(); i != adjList[fromVertex].edges.end(); ++i)
            {
            if(i->toVertex == toVertex)
                {
                throw DigraphException("Edge already exists");
                }
            }
        DigraphEdge<EdgeInfo> digraphEdge;
        digraphEdge.einfo = einfo;
        digraphEdge.fromVertex = fromVertex;
        digraphEdge.toVertex = toVertex;
        adjList[fromVertex].edges.push_back(digraphEdge);
        }
    else
        {
        throw DigraphException("Outgoing vertex does not exist");
        }
    }
    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex)
    {
    if(adjList.find(vertex) != adjList.end())
        {
        adjList.erase(vertex);
        }
    else
        {
        throw DigraphException("Vertex does not exist.");
        }
    }

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex)
    {
    if(adjList.find(fromVertex) != adjList.end())
        {
        std::list<DigraphEdge<EdgeInfo>> linkedList = adjList[fromVertex].edges;
        for(auto i = linkedList.begin(); i != linkedList.end(); ++i)
            {
            if(i->toVertex == toVertex)
                {
                linkedList.erase(i);
                return;
                }
            }
        throw DigraphException("Edge does not exist.");
        }
    else
        {
        throw DigraphException("Outgoing vertex does not exist.");
        }
    }

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const
    {
    return adjList.size();
    }

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const
    {
    int count = 0;
    for(auto i = adjList.begin(); i != adjList.end(); ++i)
        {
        DigraphVertex<VertexInfo, EdgeInfo> digraphVertex = i->second;
        count += digraphVertex.edges.size();
        }
    return count;
    }

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const
    {
    if(adjList.find(vertex) != adjList.end())
        {
        return adjList.at(vertex).edges.size();
        }
    else
        {
        throw DigraphException("Vertex does not exist.");
        }
    }

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const
    {
    std::vector<int> allVertices;
    for(auto i = adjList.begin(); i != adjList.end(); ++i)
        {
        allVertices.push_back(i->first);
        }
    int numReached = 0;
    //for(auto i = allVertices.begin(); i != allVertices.end(); ++i)
    for(int i : allVertices)
        {
        for(auto j = adjList.at(i).edges.begin(); j != adjList.at(i).edges.end(); ++j)
            {
            if(std::find(allVertices.begin(), allVertices.end(), j->toVertex) != allVertices.end())
                {
                numReached++;
                }
            }
        if(numReached != allVertices.size())
            {
            return false;
            }
        numReached = 0;    
        }
    return true;
    }
    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const
    {
    
    std::map<int, int> vertexP;
    std::map<int, bool> shortestFound;
    std::map<int, double> shortestDistance;
    std::vector<int> allVertices = vertices();
    
    
    for(int i = 0; i < allVertices.size(); ++i)
        {
        vertexP[allVertices[i]] = 0;
        shortestFound[allVertices[i]] = false;
        shortestDistance[allVertices[i]] = 13337;
        if(allVertices[i] == startVertex)
            {
            shortestDistance[startVertex] = 0;
            vertexP[startVertex] = startVertex;
            }
        }
    
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, CompareDistance> priorityQ;
    priorityQ.push(std::make_pair(startVertex, 0));
    
   
    while(priorityQ.size() != 0)
        {
        std::pair<int, double> vertexDistance;
        double distance;
        int vertex = priorityQ.top().first;
        priorityQ.pop();

        if(!shortestFound[vertex])
            {
            shortestFound[vertex] = true;
            for(auto i = adjList.at(vertex).edges.begin(); i != adjList.at(vertex).edges.end(); ++i)
                {
                distance = edgeWeightFunc(i->einfo);
                for(int j = 0; j < allVertices.size(); ++j)
                    {
                    if(i->toVertex == allVertices[j] && shortestFound.at(allVertices[j]) == false)
                    
                        if(shortestDistance.at(i->toVertex) > (shortestDistance.at(vertex) + distance))
                        {
                            shortestDistance.at(i->toVertex) = shortestDistance.at(vertex) + distance;
                            vertexP.at(i->toVertex) = vertex;
                            vertexDistance.first = i->toVertex;
                            vertexDistance.second = distance;
                            priorityQ.push(vertexDistance);
                        }
                    }
                }
            }

        }
        return vertexP;
    
    }

    class CompareDistance
    {
    public:
        bool operator()(std::pair<int, double> firstPair, std::pair<int, double> secondPair)
            {
            return firstPair.second < secondPair.second;
            }
    };


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> adjList;

    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.
};
// You'll need to define the member functions of your Digraph class
// template here.


#endif // DIGRAPH_HPP

