#include <vector>

// Utilities functions.
#define array std::vector
#define add(v, x) v.push_back(x)
#define remove(v, i) v.erase(v.begin() + i)
#define contains(v, x) (std::find(v.begin(),v.end(),x) != v.end())
// #define min(v) *std::min_element(v.begin(),v.end());
#define append(v, w) v.reserve(v.size()+w.size()); v.insert( v.end(), w.begin(), w.end() )


