#include <iostream>
#include <sstream>
#include "graphiso.h"

using namespace std;

// ================= Complete - Begin Graph class implementation ===================
Graph::Graph(std::istream& istr)
{
    string aline;
    while (getline(istr, aline))
    {
        istringstream iss(aline);
        string u, v;
        if (iss >> u)
        {
            VERTEX_SET_T neighbors;
            while (iss >> v)
            {
                neighbors.insert(v);
            }
            adj_.insert(std::make_pair(u, neighbors));
        }
    }
}


bool Graph::edgeExists(const VERTEX_T& u, const VERTEX_T& v) const
{
    if ((adj_.find(u) != adj_.end()) && (adj_.at(u).find(v) != adj_.at(u).end()))
    {
        return true;
    }
    return false;
}
const VERTEX_SET_T& Graph::neighbors(const VERTEX_T& v) const
{
    if (adj_.find(v) == adj_.end())
    {
        throw std::invalid_argument("Neighbors: invalid vertex - not in  map");
    }
    return adj_.at(v);
}
VERTEX_LIST_T Graph::vertices() const
{
    VERTEX_LIST_T verts;
    for (const auto& p : adj_)
    {
        verts.push_back(p.first);
    }
    return verts;
}
// ================= Complete - End Graph class implementation ===================

// Prototype and helper functions here

size_t degree(const Graph& g, const VERTEX_T& v)
{
    return g.neighbors(v).size();
}

// To be completed
bool isConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    // Feel free to change or remove this starter code
    VERTEX_LIST_T g1verts = g1.vertices();
    //VERTEX_LIST_T g2verts = g2.vertices(); 
    for (size_t i = 0; i < g1verts.size(); i++)
    {
        auto pair = mapping.find(g1verts[i]);
        if (pair)
        {
            // for both, check if they have the same degree 
            if (degree(g1, pair->first) != degree(g2, pair->second))
                return false;
            
            // we also want to check if each neighbor is consistent
            for(auto neighbor : g1.neighbors(pair->first))
            {
                auto neighborPair = mapping.find(neighbor);
                if (neighborPair && !g2.edgeExists(pair->second, neighborPair->second))
                    return false; 
            }
        }
        //also count num of edges? 
    }
    return true;
}

// Add any helper functions you deem useful

bool graphIsoH(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping, VERTEX_SET_T mapped)
{
    VERTEX_LIST_T v1 = g1.vertices();
    VERTEX_LIST_T v2 = g2.vertices();
    for (size_t i = 0; i < v1.size(); i++)
    {
        if (!mapping.find(v1[i]))
        {
            for (size_t j = 0; j < v2.size(); j++)
            {
                if (mapped.find(v2[j]) != mapped.end()) continue;

                mapping.insert(std::make_pair(v1[i], v2[j]));
                mapped.insert(v2[j]);
                if (isConsistent(g1, g2, mapping) && graphIsoH(g1, g2, mapping, mapped))
                {
                    return true;
                }
                mapping.remove(v1[i]);
                mapped.erase(v2[j]);
            }
            return false; 
        }
    }
    return mapped.size() == v2.size();
}

// To be completed
bool graphIso(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    VERTEX_LIST_T g1verts = g1.vertices();
    if (g1verts.size() != g2.vertices().size())
    {
        return false;
    }
    // Add code here

    return graphIsoH(g1, g2, mapping, VERTEX_SET_T());
}

