//----------------------------------------------------------------------------//

/*! \file Raymarcher.cpp
  Contains implementations of Raymarcher class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Raymarchers/Raymarcher.h"

// System includes

#include <list>

// Library includes

#include <boost/foreach.hpp>

// Project headers

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace boost;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Utility function implementations
//----------------------------------------------------------------------------//

IntervalVec splitIntervals(const IntervalVec &intervals)
{
  // If we have zero or one intervals we do nothing
  if (intervals.size() < 2) {
    return intervals;
  }

  // Gather all interval start/end points
  vector<double> points;
  points.reserve(intervals.size() * 2);
  BOOST_FOREACH (const Interval &i, intervals) {
    points.push_back(i.t0);
    points.push_back(i.t1);
  }

  // Sort and unique start/end points 
  sort(points.begin(), points.end());
  vector<double>::iterator newEnd = unique(points.begin(), points.end());
  points.resize(newEnd - points.begin());

  // For each endpoint pair, find the incoming interval(s) that overlap it
  IntervalVec outIntervals;
  for (size_t i = 0; i < points.size() - 1; ++i) {
    Interval newInterval(points[i], points[i + 1], 0);
    bool foundInterval = false;
    BOOST_FOREACH (const Interval &interval, intervals) {
      if (interval.t0 < newInterval.t1 && interval.t1 > newInterval.t0) {
        newInterval.stepLength = std::min(interval.stepLength, 
                                          newInterval.stepLength);
        foundInterval = true;
      }
    }
    if (foundInterval) {
      outIntervals.push_back(newInterval);
    }
  }
  
  return outIntervals;
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
