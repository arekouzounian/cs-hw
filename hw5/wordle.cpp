// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleH(const string& in, const set<string>& dict, const string& floating, string& build, set<string>& words);
string getOptions(const string& in, const string& floating, int pos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    string build = ""; 
    set<string> out; 

    wordleH(in, dict, floating, build, out);
    return out;
}

string getOptions(const string& in, const string& floating, int pos)
{
    if(pos >= in.size())
        return string();
    if(in[pos] != '-')
        return string(1, in[pos]);

    int blanks = 0; 
    for(int i = pos; i < in.size(); ++i)
    {
        if(in[i] == '-')
            blanks++;
    }

    if(blanks > floating.size())
    {
        return "abcdefghijklmnopqrstuvwxyz";
    }
    else if (blanks < floating.size())
    {
        //there is a major issue here
        cout << "something terrible has happened." << endl;
        throw std::out_of_range("There are more floating letters than there are blanks");
    }
    else 
    {
        return floating;
    }
}

void wordleH(const string& in, const set<string>& dict, const string& floating, string& build, set<string>& words)
{
    if (build.size() > in.size())
    {
        return; 
    }

    char letter;

    string options = getOptions(in, floating, build.size());
    if(options == string())
        return;
    
    for(int i = 0; i < options.size(); i++)
    {
        letter = options[i];
        
        string floatCp = floating; 
        
        size_t pos = floating.find(letter);
        if (pos != string::npos) {
            floatCp.erase(pos, 1); 
        }
        //has to be a string, not a set to allow multiples
        
        string curr = build + letter;
        wordleH(in, dict, floatCp, curr, words); //generate the rest of the word
        if(curr.size() == in.size() && floatCp.begin() == floatCp.end())
        {
            if(dict.find(curr) != dict.end()) 
                words.insert(curr);
        }
    }
}