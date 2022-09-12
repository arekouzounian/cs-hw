#include "move.h"


// Complete
Move::Move(Board* board)
{
    m.first = Vehicle::INVALID_ID;
    m.second = 0;
    b = board;
    g = 0;
    h = 0;
    prev = NULL;
}

// To be completed
Move::Move(const Board::MovePair& move, Board* b,  Move *parent)
    : m(move), b(b), prev(parent), g(parent->g + 1)
{

}

// To be completed
Move::~Move()
{
    //if (prev != nullptr)
    delete b;
}

// To be completed
bool Move::operator<(const Move& rhs) const
{
    // Replace this
    if((this->g + this->h) == (rhs.g + rhs.h)) {
        return this->g > rhs.g;
    }
    return (this->g + this->h) < (rhs.g + rhs.h);
}

// To be completed
void Move::score(Heuristic *heur) 
{
    this->h = heur->compute(*(this->b));
}
