#include <cmath>
#include "heur.h"
#include <vector>
#include <set>
using namespace std;

// Complete
size_t BFSHeuristic::compute(const Board& b)
{
    return 0U;
}


// To be completed
//size_t BlockingHeuristic::compute(const Board& b)
size_t DirectHeuristic::compute(const Board& b)
{
    // Avoid compiler warnings -- replace this
    Vehicle v = b.escapeVehicle();
    std::set<Board::VID_T> cars;
    for (int i = v.startc + v.length; i < b.size(); ++i)
    {
        Board::VID_T curr = b.at(v.startr, i);
        if (curr != Vehicle::INVALID_ID)
            cars.insert(curr);
    }

    return cars.size();
}

// To be completed
//size_t DependencyHeuristic::compute(const Board& b)
size_t IndirectHeuristic::compute(const Board& b)
{
    std::set<Vehicle> direct;
    std::set<Vehicle> blockers;
    //first we find and store all direct blocking vehicles
    //loop through each direct blocking vehicle
        //if the curr vehicle is horizontal, check left and right sides
        //if verticle, check above and below 
            //depending on which one is on either side, check if 
            //the vehicle can move in the right direction but is blocked by 
            //another vehicle. if so, add the blocking vehicle to 
            //set of indirect blocking vehicles
    Vehicle v = b.escapeVehicle();
    for (int i = v.startc + v.length; i < b.size(); ++i)
    {
        Board::VID_T curr = b.at(v.startr, i);
        if (curr != Vehicle::INVALID_ID) {
            Vehicle hicle = b.vehicle(curr);
            direct.insert(hicle);
            blockers.insert(hicle);
        }
    }

    std::set<Vehicle>::iterator it;

    for (it = direct.begin(); it != direct.end() && it->direction == Vehicle::VERTICAL; ++it)
    {
        Vehicle currV = *it;
        int prvSpace = 0, nxtSpace = 0;
        Vehicle prv = v, nxt = v;

        //get prev 
        for (int i = currV.startr - 1; i >= 0; --i)
        {
            Vehicle::VID_T car = b.at(i, currV.startc);
            if (car == Vehicle::INVALID_ID)
            {
                prvSpace++;
            }
            else
            {
                prv = b.vehicle(car);
                break;
            }
        }

        //get next
        for (int i = currV.startr + currV.length; i < b.size(); ++i)
        {
            Vehicle::VID_T car = b.at(i, currV.startc);
            if (car == Vehicle::INVALID_ID)
            {
                nxtSpace++;
            }
            else
            {
                nxt = b.vehicle(car);
                break;
            }
        }

        int prvRowIfMoved = currV.startr + (currV.length - 1) - prvSpace;
        int nxtRowIfMoved = currV.startr + nxtSpace;
        if (prvRowIfMoved < v.startr || nxtRowIfMoved > v.startr) {
            continue; //it can move up or down fully
        }
        else {
            //can move up and/or down but neither fully
            bool viablePrv = false;
            bool viableNxt = false;
            int viablePaths = 0;
            if(prv.id != v.id)
            {
                int spaceWithoutPrv = 0;
                for(int i = currV.startr; i >= 0; --i) {
                    Vehicle::VID_T currChar = b.at(i, currV.startc);
                    if(currChar == Vehicle::INVALID_ID || currChar == prv.id)
                        spaceWithoutPrv++;
                }
                if((currV.startr + currV.length - 1) - spaceWithoutPrv < v.startr) {
                    viablePaths++;
                    viablePrv = true;
                }
            }
            if(nxt.id != v.id)
            {
                int spaceWithoutNxt = 0;
                for(int i = currV.startr + currV.length - 1; i < b.size(); ++i) {
                    Vehicle::VID_T currChar = b.at(i, currV.startc);
                    if(currChar == Vehicle::INVALID_ID || currChar == nxt.id)
                        spaceWithoutNxt++;
                }
                if(currV.startr + spaceWithoutNxt > v.startr) {
                    viablePaths++;
                    viableNxt = true;
                }
            }

            if(viablePaths == 1)
            {
                if(viablePrv) {
                    blockers.insert(prv);
                }
                else {
                    blockers.insert(nxt);
                }
            }
        }
    }

    return blockers.size();
}

