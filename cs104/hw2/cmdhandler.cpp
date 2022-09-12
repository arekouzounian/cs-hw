#include "cmdhandler.h"

#include "util.h"
using namespace std;

// Complete
QuitHandler::QuitHandler() {}

// Complete
QuitHandler::QuitHandler(Handler* next) : Handler(next) {}

// Complete
bool QuitHandler::canHandle(const std::string& cmd) const { return cmd == "QUIT"; }

// Complete
Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    return HANDLER_QUIT;
}

// Complete
PrintHandler::PrintHandler() {}

// Complete
PrintHandler::PrintHandler(Handler* next) : Handler(next) {}

// Complete
bool PrintHandler::canHandle(const std::string& cmd) const { return cmd == "PRINT"; }

// Complete
Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    } catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

// Add code for other handler class implementations below
AndHandler::AndHandler() {}
AndHandler::AndHandler(Handler* next) : Handler(next) {}

bool AndHandler::canHandle(const std::string& cmd) const { return cmd == "AND"; }

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    AndWebPageSetCombiner andComb;
    std::vector<std::string> terms;
    std::string currTerm;
    WebPageSet hits;

    if (!(instr >> currTerm))
        return HANDLER_ERROR;
    terms.push_back(currTerm);

    while (instr >> currTerm) {
        terms.push_back(currTerm);
    }

    try {
        hits = eng->search(terms, &andComb);
    } catch (std::exception& e) {
        return HANDLER_ERROR;
    }

    display_hits(hits, ostr);
    return HANDLER_OK;
}

// Or Handler
OrHandler::OrHandler() {}
OrHandler::OrHandler(Handler* next) : Handler(next) {}

bool OrHandler::canHandle(const std::string& cmd) const { return cmd == "OR"; }

Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    // implement
    OrWebPageSetCombiner orComb;
    std::vector<std::string> terms;
    std::string currTerm;
    WebPageSet hits;

    if (!(instr >> currTerm))
        return HANDLER_ERROR;
    terms.push_back(currTerm);

    while (instr >> currTerm)
        terms.push_back(currTerm);

    try {
        hits = eng->search(terms, &orComb);
    } catch (std::exception& e) {
        return HANDLER_ERROR;
    }

    display_hits(hits, ostr);
    return HANDLER_OK;
}

// Diff Handler
DiffHandler::DiffHandler() {}
DiffHandler::DiffHandler(Handler* next) : Handler(next) {}

bool DiffHandler::canHandle(const std::string& cmd) const { return cmd == "DIFF"; }

Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    // implement
    DiffWebPageSetCombiner diffComb;
    std::vector<std::string> terms;
    std::string currTerm;
    WebPageSet hits;
    if (!(instr >> currTerm))
        return HANDLER_ERROR;
    terms.push_back(currTerm);

    while (instr >> currTerm)
        terms.push_back(currTerm);

    try {
        hits = eng->search(terms, &diffComb);
    } catch (const std::exception& e) {
        return HANDLER_ERROR;
    }

    display_hits(hits, ostr);
    return HANDLER_OK;
}

// Incoming Handler
IncomingHandler::IncomingHandler() {}
IncomingHandler::IncomingHandler(Handler* next) : Handler(next) {}

bool IncomingHandler::canHandle(const std::string& cmd) const { return cmd == "INCOMING"; }

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    WebPage* pg;
    std::string currFile;
    if (!(instr >> currFile))
        return HANDLER_ERROR;

    pg = eng->retrieve_page(currFile);
    if (!pg)
        return HANDLER_ERROR;

    display_hits(pg->incoming_links(), ostr);
    return HANDLER_OK;
}

// Outgoing Handler
OutgoingHandler::OutgoingHandler() {}
OutgoingHandler::OutgoingHandler(Handler* next) : Handler(next) {}

bool OutgoingHandler::canHandle(const std::string& cmd) const { return cmd == "OUTGOING"; }

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    WebPage* pg;
    std::string currFile;
    if (!(instr >> currFile))
        return HANDLER_ERROR;

    pg = eng->retrieve_page(currFile);
    if (!pg)
        return HANDLER_ERROR;

    display_hits(pg->outgoing_links(), ostr);
    return HANDLER_OK;
}