//
// Test suite for HW5 Scheduling 
//

#include <gtest/gtest.h>
#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "graphiso.h"

using namespace std;

void printVertexMapping(std::ostream& ostr, const Graph& g1, VERTEX_ID_MAP_T& mapping)
{
	auto g1Verts = g1.vertices();
	for(const auto& g1v : g1Verts){
		if(mapping.find(g1v) != nullptr)
		{
			ostr << "G1: " << g1v << " <=> G2: " << mapping[g1v]  << endl;
		}
		else 
		{
			ostr << "G1: " << g1v << ": NO MAPPING!" << endl;
		}
	}
}


testing::AssertionResult runAndVerifySolution(
	const Graph& g1, const Graph& g2, bool solutionExists, bool actual, VERTEX_ID_MAP_T& mapping)
{
	bool result = true;
	ostringstream err;

    VERTEX_LIST_T g1verts = g1.vertices();
    VERTEX_LIST_T g2verts = g2.vertices();
	std::set<VERTEX_T> g1VertexSet(g1verts.begin(), g1verts.end());
	std::set<VERTEX_T> g2VertexSet(g2verts.begin(), g2verts.end());

	// If we expected false and got false, then that is a successful test run
	if(false == actual && false == solutionExists)
	{
		return testing::AssertionSuccess();
	}
	
	// If results don't match that is an error.
	else if(actual != solutionExists)
	{
		err  << "Mismatch with boolean return result (expected: " << solutionExists << " / actual: " << actual << ")";
		return testing::AssertionFailure() << err.str() << endl;

	}

	// Expected true and got a true return value so let's check the mapping.
    for(const auto& g1u : g1verts)
    {
        if( mapping.find(g1u) == nullptr)
        {
			err << "Solution does not contain a mapping for G1 vertex: " << g1u << endl;
			result = false;
			continue;
		}
		auto g2u = mapping.at(g1u);
		if(g2VertexSet.find(g2u) == g2VertexSet.end())
		{
			err << "G1: " << g1u << " maps to " << g2u << " but that vertex doesn't exist in G2 or was already used" << endl;
			result = false;
			continue;
		}
		else 
		{
			g2VertexSet.erase(g2u);
		}
		auto g1Neighbors = g1.neighbors(g1u);
		auto g2Neighbors = g2.neighbors(g2u);
		if(g1Neighbors.size() != g2Neighbors.size())
		{
			err << "G1: " << g1u << " has " << g1Neighbors.size() << " neighbors while "
			    << "G2: " << g2u << " has " << g1Neighbors.size() << " neighbors" << endl;
			result = false;
			continue;
		}
		for(const auto& g1v : g1Neighbors)
		{
			if( !g2.edgeExists(mapping[g1u], mapping[g1v]))
			{
				err << "Expected G1 edge: " << g1u << "," << g1v << " does not have a corresponding edge between:"
				    << "G2: " << g2u << "," << mapping[g1v] << endl;
				result = false;
			}
		}

    }
	if(false == result)
	{
		printVertexMapping(err, g1, mapping);
		return testing::AssertionFailure() << err.str() << endl;			
	}
	return testing::AssertionSuccess();
}

/********************************************
 *    Scheduling Tests
 ********************************************/
TEST(GraphIso, Graph1True)
{
	stringstream ss1(
		"a1 a2 a4\n"
		"a2 a1 a4 a3\n"
		"a3 a2 a4\n"
		"a4 a2 a3 a1\n"
	);
	stringstream ss2(
		"b1 b3 b2 b4\n"
		"b2 b3 b1\n"
		"b3 b1 b2 b4\n"
		"b4 b3 b1\n"
	);

	Graph g1(ss1);
	Graph g2(ss2);

	VERTEX_ID_MAP_T mapping;
	bool actual = graphIso(g1, g2, mapping);

	testing::AssertionResult result = runAndVerifySolution(g1, g2, true, actual, mapping);
	EXPECT_TRUE(result);
}

TEST(GraphIso, Graph2True)
{
	stringstream ss1(
		"a1 a2\n"
		"a2 a1 a3 a4 a5\n"
		"a3 a2\n" 
		"a4 a2\n"
		"a5 a2 a6\n" 
		"a6 a5\n" 
	);
	stringstream ss2(
		"b1 b2\n"
		"b2 b3 b1\n"
		"b3 b4 b5 b6 b2\n"
		"b4 b3\n"
		"b5 b3\n"
		"b6 b3\n"
	);

	Graph g1(ss1);
	Graph g2(ss2);

	VERTEX_ID_MAP_T mapping;
	bool actual = graphIso(g1, g2, mapping);

	testing::AssertionResult result = runAndVerifySolution(g1, g2, true, actual, mapping);
	EXPECT_TRUE(result);
}

TEST(GraphIso, Graph3False)
{
	stringstream ss1(
		"a1 a2\n"
		"a2 a1 a3 a4 a5\n"
		"a3 a2\n" 
		"a4 a2\n"
		"a5 a2 a6\n" 
		"a6 a5\n" 
	);
	stringstream ss2(
		"b1 b2 b3\n"
		"b2 b3 b1\n"
		"b3 b4 b5 b6 b2 b1\n"
		"b4 b3\n"
		"b5 b3\n"
		"b6 b3\n"
	);

	Graph g1(ss1);
	Graph g2(ss2);

	VERTEX_ID_MAP_T mapping;
	bool actual = graphIso(g1, g2, mapping);

	testing::AssertionResult result = runAndVerifySolution(g1, g2, false, actual, mapping);
	EXPECT_TRUE(result);
}

TEST(GraphIso, Graph4aTrue)
{
	stringstream ss1(
		"a1 a6 a8 a2\n"
		"a2 a3 a4 a1\n"
		"a3 a8 a7 a2\n"
		"a4 a7 a6 a2\n"		
		"a5 a6 a8 a7\n"
		"a6 a1 a4 a5\n"
		"a7 a5 a3 a4\n"
		"a8 a1 a5 a3\n"
	);
	stringstream ss2(
		"b1 b6 b8 b7\n"
		"b2 b3 b4 b5\n"
		"b3 b8 b7 b2\n"
		"b4 b7 b6 b2\n"		
		"b5 b6 b8 b2\n"
		"b6 b1 b4 b5\n"
		"b7 b1 b3 b4\n"
		"b8 b1 b5 b3\n"
	);

	Graph g1(ss1);
	Graph g2(ss2);

	VERTEX_ID_MAP_T mapping;
	bool actual = graphIso(g1, g2, mapping);
	testing::AssertionResult result = runAndVerifySolution(g1, g2, true, actual, mapping);
	EXPECT_TRUE(result);
}



TEST(GraphIso, Graph4bFalse)
{
	stringstream ss1(
		"a1 a6 a8 a2\n"
		"a2 a3 a4 a1\n"
		"a3 a8 a7 a2\n"
		"a4 a7 a6 a2\n"		
		"a5 a6 a8 a7\n"
		"a6 a1 a4 a5\n"
		"a7 a5 a3 a4\n"
		"a8 a1 a5 a3\n"
	);
	stringstream ss2(
		"b1 b6 b8 b1\n"
		"b2 b3 b4 b5\n"
		"b3 b8 b7 b2\n"
		"b4 b7 b6 b2\n"		
		"b5 b6 b8 b2\n"
		"b6 b1 b4 b5\n"
		"b7 b7 b3 b4\n"
		"b8 b1 b5 b3\n"
	);

	Graph g1(ss1);
	Graph g2(ss2);

	VERTEX_ID_MAP_T mapping;
	bool actual = graphIso(g1, g2, mapping);
	testing::AssertionResult result = runAndVerifySolution(g1, g2, false, actual, mapping);
	EXPECT_TRUE(result);
}

