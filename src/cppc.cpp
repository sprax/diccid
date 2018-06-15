// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl
// Windows:
// BUILD in Developer Command Prompt: cl mscc.cpp && link mscc.obj && mscc.exe

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
// #include <unordered_map>
// #include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;

static const std::vector<std::string> default_map_keys {
    "zero",
    "one",
    "two",
    "forty-two",
    "three",
    "four",
    "ninety"
};

template<class Container, typename T>
int copy_into(Container &container, std::vector<T> vec)
{
    for (auto& elt : vec) {
        container.push_back(elt);
    }
    return container.size();
}

/** Determins if two values are 'close enough' based on a scaled tolerance */
template<typename T>
bool EpsEq(T a, T b, T relTol = std::numeric_limits<T>::epsilon())
{
    T value_range[] = {T(1.0), a, b};
    if (fabs(a - b) <= relTol * *std::max_element(value_range, value_range+3)){
        return true;
    }
    return false;
}


  template<typename T>
  T median_odd(std::vector<T> vec)
  {
      const auto median_it = vec.begin() + vec.size() / 2;
      std::nth_element(vec.begin(), median_it , vec.end());
      return *median_it;
  }

/** returns median value of even-length vector */
  template<typename T>
  T median_even(std::vector<T> vec)
  {
      const auto median_it1 = vec.begin() + int(vec.size() / 2) - 1;
      const auto median_it2 = vec.begin() + int(vec.size() / 2);
      assert(median_it1 == median_it2-1);
      return (*median_it1 + *median_it2) / 2;
  }

/** returns median value of any-length vector.  But the template type must be floating_point
 * (float, double, long double) or it will not compile (as in: `error: no matching function`).
 */
  template<typename T>
  T median(std::vector<T> vec, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = 0)
  {
      if (vec.size() == 0) {
          return std::numeric_limits<T>::signaling_NaN();
      }
      return (vec.size() % 2) ? median_odd(vec) : median_even(vec);
  }


int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    int j = 2;
    for (auto& key : default_map_keys) {
        cout << "key: " << key << endl;
        if (--j == 0) {
            break;
        }
    }

    std::list<std::string> name_list;
    int count = copy_into(name_list, default_map_keys);
    cout << "copy_into copied " << count << " keys from default_map_keys into name_list:" << endl;
    for (auto& name : name_list) {
        cout << name << endl;
    }

    //std::sort(name_list.begin(), name_list.begin());
    name_list.sort();
    cout << "After sorting, name_list:" << endl;
    for (auto& name : name_list) {
        cout << name << endl;
    }

#ifndef _WIN32
    std::vector<std::string> myvector(default_map_keys);
    std::sort( myvector.begin(), myvector.end()
             , [](std::string& aa, std::string& bb) -> bool{ return aa.length() > bb.length(); });
    cout << "sorted vector copy:" << endl;
    for (auto& str : myvector) {
        cout << str << endl;
    }
#endif


    string name_f = name_list.front();
    auto nit = name_list.begin();
    std::advance(nit, 1);
    string name_n = *nit;
    cout << "name_f: " << name_f << "  name_n: " << name_n << endl;

    return 0;
}
