#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5]{ 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if (false == debug)
        {
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        unsigned long long w[5];
        int iter = 0;
        for (int i = 4; i >= 0; --i)
        {
            iter++;
            std::string word;

            int left = k.size() - (6 * iter);
            if (left < -5)
            {
                w[i] = 0;
                continue;
            }

            size_t start = left > 0 ? left : 0;
            size_t end; 
            if (start == 0) {
                end = left + 6; 
            }
            else {
                end = start + 6 > k.size() ? k.size() - start : 6;
            }
            word = k.substr(start, end);

            HASH_INDEX_T tot = 0;
            for (int j = 0; j < word.size() - 1; ++j)
            {
                tot += letterDigitToNumber(word[j]);
                tot *= 36;
            }
            tot += letterDigitToNumber(word[word.size() - 1]);

            w[i] = tot;
        }

        HASH_INDEX_T h = 0;
        // w is now filled 
        for (int i = 0; i < 5; ++i)
        {
            h += (rValues[i] * w[i]);
        }

        return h;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if (letter <= 57 && letter >= 48) //ascii values for 0-9
        {
            return (letter - 48) + 26;
        }
        else if (letter <= 90 && letter >= 65) //ascii for uppercase
        {
            letter += 32;
        }

        //for a-z
        return letter - 97;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for (int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
