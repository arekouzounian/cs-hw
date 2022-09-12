//
// CS104 Stack test cases
//

#include <stack.h>
#include <random_generator.h>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
using namespace std;

typedef Stack<std::string> StackString;
typedef Stack<int> StackInt;

template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& vec)
{
	for(auto x : vec) { os << x << " "; }
	return os;
}

TEST(Stack, BasicString)
{
	vector<string> words = {"w0", "w1", "w2", "w3", "w4"};
	StackString stack;

	EXPECT_THROW(stack.top(), std::underflow_error);
	EXPECT_THROW(stack.pop(), std::underflow_error);

	EXPECT_EQ(true, stack.empty());
	EXPECT_EQ(0U, stack.size());

	// 1 push -> 1 pop
	stack.push(words[0]);
	EXPECT_EQ(words[0], stack.top());
	stack.pop();
	EXPECT_EQ(true, stack.empty());
	EXPECT_EQ(0, stack.size());

	// Multiple pushes mixed with pops
	stack.push(words[1]);
	EXPECT_EQ(words[1], stack.top());

	EXPECT_EQ(false, stack.empty());
	EXPECT_EQ(1U, stack.size());

	stack.push(words[2]);
	EXPECT_EQ(words[2], stack.top());
	stack.push(words[3]);
	EXPECT_EQ(words[3], stack.top());

	EXPECT_EQ(false, stack.empty());
	EXPECT_EQ(3U, stack.size());

	stack.pop();
	EXPECT_EQ(words[2], stack.top());
	stack.pop();
	EXPECT_EQ(words[1], stack.top());

	EXPECT_EQ(false, stack.empty());
	EXPECT_EQ(1, stack.size());

	stack.push(words[4]);
	EXPECT_EQ(words[4], stack.top());

	stack.pop();
	EXPECT_EQ(words[1], stack.top());
	stack.pop();
	EXPECT_EQ(true, stack.empty());
	EXPECT_EQ(0U, stack.size());
}


TEST(Stack, RandomInt)
{
	const RandomSeed masterSeed = 34379;
	const size_t numElements = 1000;
	const size_t numTrials = 20;

	
	size_t trial = 0U;
	for(RandomSeed trialSeed : makeRandomSeedVector(numTrials, masterSeed))
	{
		auto randomInts = makeRandomNumberVector(
			numElements, -10000, 10000, trialSeed, false);

		StackInt actualStack;
		std::vector<int> expStack;
		std::vector<int> lastStack;
		size_t oper = 0;

		// put the data in
		for(auto r : randomInts)
		{
			// Copy so we can print useful errors
			lastStack = expStack;
			// Even : push,  Odd : pop
			if(r % 2 == 0) {
				actualStack.push(r);
				expStack.push_back(r);
			}
			else {
				if(!expStack.empty()) {
					actualStack.pop();
					expStack.pop_back();
				}
			}
			if(!expStack.empty())
			{
				std::ostringstream ss;
				EXPECT_FALSE(actualStack.empty());
				if(expStack.back() != actualStack.top())
				{
					ss << "Failed on trial " << trial << " operation " << oper << endl;
					if(r % 2 == 0){
						ss << "\tOperation is Push(" << r << ")" << endl;
					}
					else {
						ss << "\tOperation is Pop: " << endl;
					}
					ss << "Before the operation the stack was as expected and contained: "<< endl;
					ss << lastStack << " <- top" << endl;
					FAIL() << ss.str();
				}
			}
		}
		trial++;
	}
}


// /*
//  * The top of empty stack should throw an exception
//  */
// TEST(Stack, TopEmpty)
// {
// 	StackString stack;

// 	EXPECT_THROW(stack.top(), std::underflow_error);
// }


// /*
//  * If we push one element, the stack should function properly
//  */
// TEST(Stack, OneElementPush)
// {
// 	StackString stack;

// 	stack.push("Aaron");
// 	ASSERT_EQ(false, stack.empty());
// 	ASSERT_EQ(1, stack.size());
// 	EXPECT_EQ("Aaron", stack.top());
// }

// /*
//  * If we push multiple elements, the stack should function properly
//  */
// TEST(Stack, FiveElementPush)
// {
// 	StackString stack;

// 	for (int i = 0; i < 5; i++)
// 	{
// 		stack.push(std::to_string(i));
// 		ASSERT_EQ(false, stack.empty());
// 		ASSERT_EQ(i+1, stack.size());
// 		EXPECT_EQ(std::to_string(i), stack.top());
// 	}
// }

// TEST(Stack, OneElementPop)
// {
// 	StackString stack;

// 	stack.push("Aaron");

// 	stack.pop();
// 	ASSERT_EQ(true, stack.empty());
// 	ASSERT_EQ(0, stack.size());
// }

// TEST(Stack, FiveElementPop)
// {
// 	StackString stack;

// 	for (int i = 0; i < 5; i++) {
// 		stack.push(std::to_string(i));
// 	}

// 	for (int i = 4; i >= 0; i--)
// 	{
// 		ASSERT_EQ(i + 1, stack.size());
// 		EXPECT_EQ(std::to_string(i), stack.top());
// 		stack.pop();
// 	}

// 	ASSERT_EQ(true, stack.empty());
// }

// TEST(Stack, PopEmpty)
// {
// 	StackString stack;

// 	EXPECT_THROW(stack.pop(), std::underflow_error);
// 	ASSERT_EQ(true, stack.empty());
// 	ASSERT_EQ(0, stack.size());
// }

// TEST(Stack, EmptyThenRefill)
// {
// 	StackString stack;

// 	stack.push("foo");
// 	stack.push("bar");

// 	EXPECT_EQ("bar", stack.top());

// 	stack.pop();

// 	EXPECT_EQ("foo", stack.top());

// 	stack.pop();

// 	stack.push("baz");

// 	EXPECT_EQ("baz", stack.top());
// 	EXPECT_EQ(1, stack.size());
// }

// TEST(Stack, PopThenAdd)
// {
// 	StackString stack;

// 	EXPECT_THROW(stack.pop(), std::underflow_error);


// 	for (int i = 0; i < 5; i++)
// 	{
// 		stack.push(std::to_string(i));
// 		ASSERT_EQ(i+1, stack.size());
// 		EXPECT_EQ(std::to_string(i), stack.top());
// 	}

// 	for (int i = 4; i >= 0; i--)
// 	{
// 		ASSERT_EQ(i+1, stack.size());
// 		EXPECT_EQ(std::to_string(i), stack.top());
// 		stack.pop();
// 	}
// 	ASSERT_EQ(true, stack.empty());
// }

// TEST(StackStress, 50x50Elements)
// {
// 	const RandomSeed masterSeed = 34379;
// 	const size_t numElements = 50;
// 	const size_t numTrials = 50;
// 	const size_t stringLength = 8;

// 	StackString stack;

// 	for(RandomSeed trialSeed : makeRandomSeedVector(numTrials, masterSeed))
// 	{
// 		std::vector<std::string> contents = makeRandomAlphaStringVector(numElements, trialSeed, stringLength, true);

// 		// put the data in
// 		for(auto contentsIter = contents.begin(); contentsIter != contents.end(); ++contentsIter)
// 		{
// 			stack.push(*contentsIter);
// 		}

// 		//now, read it back out in reverse
// 		for(auto contentsIter = contents.rbegin(); contentsIter != contents.rend(); ++contentsIter)
// 		{
// 			EXPECT_EQ(*contentsIter, stack.top());
// 			stack.pop();
// 		}

// 		EXPECT_TRUE(stack.empty());
// 	}
// }

// TEST(StackStress, 5x1000Elements)
// {
// 	const RandomSeed masterSeed = 34379;
// 	const size_t numElements = 1000;
// 	const size_t numTrials = 5;
// 	const size_t stringLength = 8;

// 	StackString stack;

// 	for(RandomSeed trialSeed : makeRandomSeedVector(numTrials, masterSeed))
// 	{
// 		std::vector<std::string> contents = makeRandomAlphaStringVector(numElements, trialSeed, stringLength, true);

// 		// put the data in
// 		for(auto contentsIter = contents.begin(); contentsIter != contents.end(); ++contentsIter)
// 		{
// 			stack.push(*contentsIter);
// 		}

// 		//now, read it back out in reverse
// 		for(auto contentsIter = contents.rbegin(); contentsIter != contents.rend(); ++contentsIter)
// 		{
// 			EXPECT_EQ(*contentsIter, stack.top());
// 			stack.pop();
// 		}

// 		EXPECT_TRUE(stack.empty());
// 	}
// }

// TEST(StackStress, 2x10000Elements)
// {
// 	const RandomSeed masterSeed = 34379;
// 	const size_t numElements = 10000;
// 	const size_t numTrials = 2;
// 	const size_t stringLength = 10;

// 	StackString stack;

// 	for(RandomSeed trialSeed : makeRandomSeedVector(numTrials, masterSeed))
// 	{
// 		std::vector<std::string> contents = makeRandomAlphaStringVector(numElements, trialSeed, stringLength, true);

// 		// put the data in
// 		for(auto contentsIter = contents.begin(); contentsIter != contents.end(); ++contentsIter)
// 		{
// 			stack.push(*contentsIter);
// 		}

// 		//now, read it back out in reverse
// 		for(auto contentsIter = contents.rbegin(); contentsIter != contents.rend(); ++contentsIter)
// 		{
// 			EXPECT_EQ(*contentsIter, stack.top());
// 			stack.pop();
// 		}

// 		EXPECT_TRUE(stack.empty());
// 	}
// }
