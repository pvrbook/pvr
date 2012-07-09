//----------------------------------------------------------------------------//

/*
    This file is part of PVR. Copyright (C) 2012 Magnus Wrenninge

    PVR is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PVR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#include "pvr/Camera.h"
#include "pvr/RenderGlobals.h"

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
  for (size_t i = 0, size = points.size() - 1; i < size; ++i) {
    Interval newInterval(points[i], points[i + 1], 
                         std::numeric_limits<float>::max());
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

Util::ColorCurve::Ptr setupDeepLCurve(const RayState &state, const float first)
{
  Util::ColorCurve::Ptr lf;
  
  if (state.doOutputDeepL) {
    lf = Util::ColorCurve::create();
    lf->addSample(first, Colors::zero());
  }

  return lf;
}

//----------------------------------------------------------------------------//

Util::ColorCurve::Ptr setupDeepTCurve(const RayState &state, const float first)
{
  Util::ColorCurve::Ptr tf; 

  if (state.doOutputDeepT) {
    tf = Util::ColorCurve::create();
    tf->addSample(first, Colors::one());
  }

  return tf;
}

//----------------------------------------------------------------------------//

void updateDeepFunctions(const float t, const Color &L, const Color &T, 
                         Util::ColorCurve::Ptr lf, Util::ColorCurve::Ptr tf)
{
  if (tf) {
    tf->addSample(t, T);
  }
  if (lf) {
    lf->addSample(t, L);
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
