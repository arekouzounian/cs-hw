#include "searcheng.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// Helper function that will extract the extension of a filename
std::string extract_extension(const std::string& filename);

std::string extract_extension(const std::string& filename) {
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}

// To be updated as needed
SearchEng::SearchEng() {
    parsers_ = std::map<std::string, PageParser*>();
    webpages_ = std::vector<WebPage*>();
    termMap_ = std::map<std::string, std::set<WebPage*>>();
}

// To be completed
SearchEng::~SearchEng() {
    for (uint i = 0; i < webpages_.size(); i++) {
        delete webpages_[i];
    }
    std::map<std::string, PageParser*>::iterator it;
    for (it = parsers_.begin(); it != parsers_.end(); ++it) {
        delete it->second;
    }
}

// Complete
void SearchEng::register_parser(const std::string& extension, PageParser* parser) {
    if (parsers_.find(extension) != parsers_.end()) {
        throw std::invalid_argument("parser for provided extension already exists");
    }
    parsers_.insert(make_pair(extension, parser));
}

// Complete
void SearchEng::read_pages_from_index(const std::string& index_file) {
    ifstream ifile(index_file.c_str());
    if (ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while (ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

// To be completed
void SearchEng::read_page(const string& filename) {
    /*
        IDEA:
        do checks to see if the file exists and all that
        check if the webpage already exists in webpages_
            if not, create a new webpage
        parse the file
        update all searchable terms
        update all outgoing links
            for each outgoing link
                if there is no webpage for that link, create one
                find that webpage and add an incoming link as well

    */
    string ext = extract_extension(filename);
    if (parsers_.find(ext) == parsers_.end()) {
        throw std::logic_error("No parser registered for the given file type!");
    }
    ifstream file(filename.c_str());
    if (file.fail()) {
        file.close();
        throw std::invalid_argument("File does not exist.");
    }

    WebPage* page = getWebPage(filename);
    std::set<std::string> allOutGoingLinks;
    std::set<std::string> allSearchableTerms;

    parsers_[ext]->parse(file, allSearchableTerms, allOutGoingLinks);

    // add searchable terms
    page->all_terms(allSearchableTerms);
    std::set<std::string>::iterator term;
    for (term = allSearchableTerms.begin(); term != allSearchableTerms.end(); ++term) {
        if (termMap_.find(*term) == termMap_.end()) {
            // create new pair
            termMap_.insert(std::make_pair(*term, std::set<WebPage*>()));
        }
        termMap_[*term].insert(page);
    }

    std::set<std::string>::iterator it;
    for (it = allOutGoingLinks.begin(); it != allOutGoingLinks.end(); ++it) {
        WebPage* outPage = getWebPage(*it);
        page->add_outgoing_link(outPage);
        outPage->add_incoming_link(page);
    }

    file.close();
}

// To be completed
WebPage* SearchEng::retrieve_page(const std::string& page_name) const {
    WebPage* ret = NULL;
    for (uint i = 0; i < webpages_.size(); i++) {
        if (webpages_[i]->filename() == page_name)
            ret = webpages_[i];
    }
    return ret;
}

// To be completed
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const {
    string ext = extract_extension(page_name);
    if (parsers_.find(ext) == parsers_.end()) {
        throw std::logic_error("Parser for this filetype not yet registered.");
    }
    ifstream file(page_name.c_str());
    if (file.fail()) {
        file.close();
        throw std::invalid_argument("File doesn't exist");
    }

    ostr << parsers_.at(ext)->display_text(file);
}

// To be completed
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const {
    // create a set of pages that contain only the first term
    // create a set of pages that contain all the other terms

    if (terms.size() < 1 || termMap_.size() < 1) {
        return WebPageSet();
    }

    std::set<WebPage*> setA = termMap_.at(terms[0]);
    if (terms.size() < 2)
        return setA;

    std::set<WebPage*> setB = std::set<WebPage*>();

    for (uint i = 1; i < terms.size(); ++i) {
        if (termMap_.find(terms[i]) != termMap_.end()) {
            WebPageSet::iterator it;
            WebPageSet currSet = termMap_.at(terms[i]);
            for (it = currSet.begin(); it != currSet.end(); ++it) {
                setB.insert(*it);
            }
        }
    }

    return combiner->combine(setA, setB);
}

// Add private helper function implementations here

WebPage* SearchEng::getWebPage(std::string filename) {
    WebPage* ptr = retrieve_page(filename);
    if (!ptr) {
        ptr = new WebPage(filename);
        webpages_.push_back(ptr);
    }

    return ptr;
}