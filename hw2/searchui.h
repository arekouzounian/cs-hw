#ifndef SEARCHUI_H
#define SEARCHUI_H
#include "handler.h"
#include "searcheng.h"
#include <istream>
#include <string>

/****** THIS CLASS IS COMPLETED AND NEED NOT BE MODIFIED **********/

class SearchUI {
public:
    /**
     *  Initializing constructor
     *  [COMPLETED]
     *
     *  @param[in] eng
     *     Pointer to the search engine that will process inputs
     */
    SearchUI(SearchEng* eng);

    /**
     * Destructor
     *  [COMPLETED]
     */
    ~SearchUI();

    /**
     *  Add a command handler
     *  [COMPLETED]
     *
     *  @param[in] handler
     *     Pointer to the handler for a particular command.
     */
    void add_handler(Handler* handler);

    /**
     * Run the UI
     *  [COMPLETED]
     *
     *  @param [inout] istr
     *     Input stream to read additional command arguments, if needed
     *  @param [inout] ostr
     *     Output stream to output results of the command
     */
    virtual void run(std::istream& istr, std::ostream& ostr);

protected:
    SearchEng* eng_;

private:
    Handler* handlers_;
};

#endif
