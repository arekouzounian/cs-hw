#include "combiners.h"

#include "searcheng.h"

// Complete the necessary code
WebPageSet AndWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) {
    WebPageSet ret = WebPageSet();
    const WebPageSet* smaller = setA.size() <= setB.size() ? &setA : &setB;
    const WebPageSet* larger = smaller == &setA ? &setB : &setA;

    WebPageSet::iterator it;
    for (it = smaller->begin(); it != smaller->end(); ++it) {
        if (larger->find(*it) != larger->end()) {
            ret.insert(*it);
        }
    }

    return ret;
}

WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) {
    WebPageSet ret = WebPageSet();

    WebPageSet::iterator it;
    for (it = setA.begin(); it != setA.end(); ++it) {
        if (setB.find(*it) == setB.end())
            ret.insert(*it);
    }

    return ret;
}

WebPageSet OrWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) {
    WebPageSet ret = WebPageSet();

    WebPageSet::iterator itA;
    WebPageSet::iterator itB;
    for (itA = setA.begin(); itA != setA.end(); ++itA) {
        ret.insert(*itA);
    }
    for (itB = setB.begin(); itB != setB.end(); ++itB) {
        ret.insert(*itB);
    }

    return ret;
}
