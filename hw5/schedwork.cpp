

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleH(const AvailabilityMatrix& avail, 
    const size_t maxShifts, 
    DailySchedule& sched, 
    std::vector<size_t> shiftCount);

bool uniqueRow(const DailySchedule& avail, int row );

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    std::vector<size_t> shiftCount; 

    for (size_t row = 0; row < avail.size(); row++)
    {
        sched.push_back(std::vector<Worker_T>());
        for (int col = 0; col < dailyNeed; col++)
        {
            sched[row].push_back(-1);
        }
    }

    for (size_t i = 0; i < avail[0].size(); i++)
    {
        shiftCount.push_back(0);
    }

    return scheduleH(avail, maxShifts, sched, shiftCount);
}

bool scheduleH(const AvailabilityMatrix& avail, const size_t maxShifts, DailySchedule& sched, std::vector<size_t> shiftCount)
{
    for(size_t row = 0; row < sched.size(); row++)
    {
        for(size_t col = 0; col < sched[row].size(); col++)
        {
            if (sched[row][col] == -1)
            {
                for(size_t worker = 0; worker < avail[0].size(); worker++)
                {
                    if(avail[row][worker] && shiftCount[worker] < maxShifts)
                    {
                        sched[row][col] = worker; 
                        ++shiftCount[worker];
                        if(uniqueRow(sched, row) && scheduleH(avail, maxShifts, sched, shiftCount))
                        {
                            return true;
                        }
                        sched[row][col] = -1;
                        --shiftCount[worker];
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool uniqueRow(const DailySchedule& sched, int row)
{
    std::map<int, bool> workers; 
    for(size_t i = 0; i < sched[row].size(); ++i)
    {
        if(sched[row][i] == -1)
            continue; 
        else if (workers.find(sched[row][i]) != workers.end())
            return false; 
        
        workers.insert(std::make_pair(sched[row][i], true));
    }
    return true; 
}
