#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "solver.h"

using namespace std;

// To be completed
Solver::Solver(const Board& b, Heuristic* heur)
    : b_(b), heur_(heur)
{
    solution_ = Board::MovePairList();
    expansions_ = 0;
    rootBoard = new Board(b_);
    root = new Move(rootBoard);
    openSet = MoveHeap();
    closedSet = MoveSet();
}

// To be completed
Solver::~Solver()
{
    MoveSet::iterator it; 
    for(it = closedSet.begin(); it != closedSet.end(); ++it)
    {
        delete *it;
    }
}

// To be completed
bool Solver::run()
{
    openSet.push(root);
    closedSet.insert(root);
    while (!openSet.empty())
    {
        Move* currState = openSet.top();
        openSet.pop();

        if (currState->b->solved()) {
            while (currState->prev) {
                if (currState == root) {
                    continue;
                }

                solution_.insert(solution_.begin(), currState->m);
                currState = currState->prev; 
            }
            //cout << "Expansions: " << expansions_ << endl;
            return true;
        }
        
        Board::MovePairList moves = currState->b->potentialMoves();
        Board::MovePairList::iterator it; 
        for(it = moves.begin(); it != moves.end(); ++it) {
            Board* b = new Board(*(currState->b));
            b->move((*it).first, (*it).second);
            Move* m = new Move((*it), b, currState);
            m->score(heur_);
            if(closedSet.find(m) == closedSet.end()) {
                expansions_++;
                openSet.push(m);
                closedSet.insert(m);
            }
            else {
                delete m;
            }
        }
    }
    return false; 
}

// To be completed
Board::MovePairList Solver::solution() const
{
    return solution_;
}

// Complete
size_t Solver::numExpansions() const
{
    return expansions_;
}

