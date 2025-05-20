#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

// List of all "synonyms"
vector<string> words {
    "back", "ball", "band", "bare", "bass", "beads", "bean", "belt", "berry",
    "bids", "bite", "bold", "bone", "bones", "book", "cafe", "caps", "card",
    "cash", "cell", "cent", "chan", "code", "cool", "cork", "cost", "cure",
    "dale", "days", "dead", "dish", "dive", "dome", "ears", "east", "eight",
    "face", "fail", "fear", "feed", "file", "files", "find", "fired", "food",
    "form", "full", "gage", "grad", "hang", "hart", "hate", "heat", "hill",
    "hole", "jake", "jose", "king", "lane", "leading", "lets", "life",
    "lines", "list", "logs", "long", "lover", "mark", "math", "mens", "mice",
    "mild", "mint", "miss", "modes", "nest", "nick", "nike", "paid", "pine",
    "poll", "pope", "port", "real", "ride", "ross", "same", "sans", "seas",
    "seeds", "seek", "shake", "shot", "slow", "span", "takes", "then",
    "tire", "tons", "track", "wales", "walt", "warm", "wave", "wise",
    "word", "wound"
};

bool is_synonyms(string const& word, string const& other)
{
    // 1. If word and other don't have the same size, return false
    // 2. Find the first position where word and other have different
    //    characters
    // 3. If such a position is found, search for the next position
    //    where they differ
    // 4. If no other difference is found, return true
    // 5. return false in every other case

    if (word.size() != other.size()) {
        return false;
    }

    auto pos = mismatch(word.begin(), word.end(), other.begin());
    if (pos.first != word.end()) {
        auto next_pos = mismatch(pos.first, word.end(), pos.second);
        if (next_pos.first != word.end()) {
            return false;
        } else {
            return true;
        }
    }


    return false;
}

string get_synonym(string const& word)
{
    // 1. Try to find a synonym to word in the words vector; use
    //    the is_synonym function
    // 2. If a synonym was found, return it
    // 3. If no synonym was found, return word
    auto syn = [&word](string const& other) { return is_synonyms(word, other); };
    auto res = find_if(begin(words), end(words), syn);
    if (res != end(words)) {
        return *res;
    }
    return word;
}

int main()
{
    //cout << "Enter a sentence: ";
    vector<string> text{istream_iterator<string>{cin}, istream_iterator<string>{}};
    transform(text.begin(), text.end(), text.begin(), get_synonym);
    copy(text.begin(), text.end(), ostream_iterator<string>{cout, " "});
    // 1. Read all words from cin to a container named text
    // 2. Replace each word in text with a synonym; use get_synonym
    // 3. Print the new text
}
