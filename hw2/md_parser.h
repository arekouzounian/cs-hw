#ifndef MD_PARSER_H
#define MD_PARSER_H

#include "pageparser.h"
#include <set>
#include <string>

class MDParser : public PageParser {
public:
    /**
     * See documentation in PageParser class
     * [TO BE COMPLETED]
     */
    void parse(std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks);

    /**
     * See documentation in PageParser class
     * [TO BE COMPLETED]
     */
    std::string display_text(std::istream& filename);

private:
    // You may add additional private helpers or data members below
};

#endif
