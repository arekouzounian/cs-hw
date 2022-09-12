#include "md_parser.h"

#include "util.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

// To be completed
void MDParser::parse(
        std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks) {

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    string currTerm = "";
    char currChar = istr.get();
    // bool inLink = false;
    // bool inTerm = false;
    PARSE_STATE_T currState = NORMALTEXT;

    // Complete your code below
    // possibly use regex to search for []() format?

    while (!istr.fail()) {
        switch (currState) {
        case NORMALTEXT:
            if (!isalnum(currChar)) {
                if (currChar == '[')
                    currState = LINKTEXT;  // transition to next state

                if (currTerm != "")
                    allSearchableTerms.insert(conv_to_lower(currTerm));
                currTerm = "";
            } else {
                currTerm += currChar;
            }
            break;
        case LINKTEXT:
            if (!isalnum(currChar)) {
                if (currChar == ']')
                    currState = ISLINK;

                if (currTerm != "")
                    allSearchableTerms.insert(conv_to_lower(currTerm));
                currTerm = "";
            } else {
                currTerm += currChar;
            }
            break;
        case ISLINK:
            if (currChar == '[')
                currState = LINKTEXT;
            else if (currChar == '(')
                currState = LINKURL;
            else
                currState = NORMALTEXT;
            break;
        case LINKURL:
            if (currChar == ')') {
                currState = NORMALTEXT;
                if (currTerm != "")
                    allOutgoingLinks.insert(conv_to_lower(currTerm));
                currTerm = "";
            } else {
                currTerm += currChar;
            }
            break;
        default:
            break;
        }
        currChar = istr.get();
    }
    if (currTerm != "") {
        allSearchableTerms.insert(conv_to_lower(currTerm));
    }
}

// To be completed
std::string MDParser::display_text(std::istream& istr) {
    std::string ret = "";
    char curr = istr.get();
    char prev = 0;
    bool inLink = false;

    while (!istr.fail()) {
        if (curr == '(' && prev == ']') {
            inLink = true;
        }

        if (!inLink) {
            ret += curr;
        } else {
            if (curr == ')')
                inLink = false;
        }

        prev = curr;
        curr = istr.get();
    }
    return ret;

    //
}
