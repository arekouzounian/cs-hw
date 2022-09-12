#include "util.h"

#include "webpage.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

using namespace std;
std::string conv_to_lower(std::string src) {
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

std::string conv_to_upper(std::string src) {
    std::transform(src.begin(), src.end(), src.begin(), ::toupper);
    return src;
}

void display_hits(const WebPageSet& hits, std::ostream& ostr) {
    ostr << hits.size() << std::endl;
    for (WebPage* p : hits) {
        ostr << p->filename() << std::endl;
    }
}
