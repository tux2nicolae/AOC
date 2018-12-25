﻿/**
 * Advent of code 2018
 * @author : Nicolae Telechi
 */
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <queue>
#include <stack>
#include <numeric>
#include <set>
#include <regex>

using namespace std;

int Abs(const int x)
{
  if (x > 0)
    return x;

  return -x;
};

//----------------------------------------------------------------------------

struct Nanobot
{
  int id{};

  int x{};
  int y{};
  int z{};
  int range{};

  int intersections{};
};

bool intersects[1001][1001];

int Distance(const Nanobot & first, const Nanobot & second)
{
  return abs(first.x - second.x) + abs(first.y - second.y) + abs(first.z - second.z);
};

//----------------------------------------------------------------------------

int main()
{
  ifstream in("C:\\Users\\Telechi\\Desktop\\Day23.in");
  ofstream out("C:\\Users\\Telechi\\Desktop\\Day23.out");
  assert(in.good());
  assert(out.good());

  vector<Nanobot> nanobots;

  int id = 0;

  // read data
  string line;
  while (getline(in, line))
  {
    smatch match;
    regex_match(line, match, regex("pos=<(.*),(.*),(.*)>, r=(.*)"));
    assert(match.size() == 5);

    Nanobot nanobot;
    nanobot.id = id++;
    nanobot.x = stoi(match[1]);
    nanobot.y = stoi(match[2]);
    nanobot.z = stoi(match[3]);
    nanobot.range = stoi(match[4]);
    
    nanobots.push_back(nanobot);
  }

  auto maxNanobot = *max_element(begin(nanobots), end(nanobots), [](const auto & first, const auto & second) {
    return first.range < second.range;
  });

  // calculate intersections
  for (auto & currentNanobot : nanobots)
  {
	  int intersections = 0;
	  for (auto & intersectNanobot : nanobots)
	  {
		  auto d = Distance(currentNanobot, intersectNanobot) - intersectNanobot.range;
		  if (d <= currentNanobot.range)
		  {
        intersections++;
        intersects[currentNanobot.id][intersectNanobot.id] = true;
		  }
	  }

    currentNanobot.intersections = intersections;
  }

  sort(begin(nanobots), end(nanobots), [](const auto & first, const auto & second) {
    return first.intersections < second.intersections;
  });

  for (auto & first : nanobots)
  {
    for (auto & second : nanobots)
    {
      out << (intersects[first.id][second.id] ? '#' : '.');
    }
  
    out << endl;
  }

  // remove noise only based on my input (I have 976 points with all intersect each other)
  nanobots.erase(nanobots.begin(), nanobots.begin() + 31);

  auto m = numeric_limits<int>::max();
  for (auto & first : nanobots)
  {
    for (auto & second : nanobots)
    {
      if(Distance(first, second) == first.range + second.range)
      {
        cout << "Evrica : " << first.id << ' ' << second.id << endl;
      }
    }
  }

  // any two points from evrica will fit our condition

  auto firstmatch = find_if(begin(nanobots), end(nanobots), [](const auto & nanobot) {
    return nanobot.id == 602;
  });

  auto secondmatch = find_if(begin(nanobots), end(nanobots), [](const auto & nanobot) {
    return nanobot.id == 3;
  });

  assert(firstmatch != nanobots.end());
  assert(secondmatch != nanobots.end());

  auto checkPerfectIntersect = [&]()
  {
    return Distance(*firstmatch, *secondmatch) == firstmatch->range + secondmatch->range;
  };

  assert(checkPerfectIntersect());

  // move to intersection point
  while (true)
  {
    if (!firstmatch->range)
      break;
  
    // move x
    firstmatch->x--;
    firstmatch->range--;
    assert(checkPerfectIntersect());
  
    if (!firstmatch->range)
      break;
  
    // move y
    firstmatch->y--;
    firstmatch->range--;
    assert(checkPerfectIntersect());
  
    if (!firstmatch->range)
      break;
  
    // move z
    firstmatch->z--;
    firstmatch->range--;
    assert(checkPerfectIntersect());
  }

  assert(checkPerfectIntersect());

  // find any matching point
  Nanobot me = *firstmatch;

  assert(Distance(*firstmatch, me) == firstmatch->range);
  assert(Distance(*secondmatch, me) == secondmatch->range);
  assert(Distance(*firstmatch, *secondmatch) == firstmatch->range + secondmatch->range);
  assert(Distance(*firstmatch, me) + Distance(me, *secondmatch) == Distance(*firstmatch, *secondmatch));

  Nanobot origin;
  origin.x = 0;
  origin.y = 0;
  origin.z = 0;

  auto distanceToMe = Distance(origin, me);
  cout << distanceToMe;

  return 0;
}
