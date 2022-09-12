//
// Test suite for HW3 Linked List Recursion
//

#include "llrec.h"
#include "llrec_test_utils.h"
#include <vector>
#include <random_generator.h>

//====================================
// TESTS FOR OTHER LLRECURSIVE PAST PROBLEMS
//  REPLACE WITH PIVOT AND FILTER TESTS
//====================================

// Predicates for filter
struct IsEven
{
    bool operator()(int num) {
        return (num % 2) == 0;
    }
};

struct IsOdd
{
    bool operator()(int num) {
        return (num % 2) != 0;
    }
};

bool IsNeg(int v)
{ return v < 0; }

struct IsLessEquals
{
	int num2;
	IsLessEquals(int val) : num2(val){}
    bool operator()(int num1) {
        return (num1 <= num2);
    }
};

struct IsGreater
{
	int num2;
	IsGreater(int val) : num2(val){}
    bool operator()(int num1) {
        return (num1 > num2);
    }
};

/********************************************
 *    Pivot Tests
 ********************************************/

TEST(Pivot, Nominal)
{
	Node* list = makeList({2, 4, 8, 3});
	Node* small = (Node*) &list; // set to a non-null address
	Node* large = (Node*) &list; // set to a non-null address

	llpivot(list, small, large, 5);

	EXPECT_TRUE(checkContent(small, {2, 4, 3}));
	EXPECT_TRUE(checkContent(large, {8}));
	EXPECT_TRUE(checkContent(list, {}));

	deleteList(small);
	deleteList(large);
}

/********************************************
 *    Filter Test
 ********************************************/

TEST(Filter, Size3_nF_F_nF)
{
	Node* list = makeList({3,4,5});

	list = llfilter(list, IsEven());
	EXPECT_TRUE(checkContent(list, {3,5}));

	deleteList(list);
}

