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

/*! \file AdaptiveRaymarcher.cpp
  Contains implementations of AdaptiveRaymarcher class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Raymarchers/AdaptiveRaymarcher.h"

// System includes

// Library includes

#include <boost/foreach.hpp>

// Project headers

#include "pvr/Camera.h"
#include "pvr/Constants.h"
#include "pvr/Curve.h"
#include "pvr/Log.h"
#include "pvr/Math.h"
#include "pvr/RenderGlobals.h"
#include "pvr/Scene.h"
#include "pvr/StlUtil.h"
#include "pvr/Types.h"
#include "pvr/Volumes/Volume.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  using namespace pvr;

  //--------------------------------------------------------------------------//
  // Strings
  //--------------------------------------------------------------------------//

  const std::string k_strThreshold("threshold");
  const std::string k_strVolumeStepLengthMult("volume_step_length_multiplier");
  const std::string k_strDoTrapezoidIntegration("do_trapezoid_integration");
  const std::string k_strEarlyTermThresh("early_termination_threshold");

  //--------------------------------------------------------------------------//
  // Helper functions
  //--------------------------------------------------------------------------//

  Color exp(const Color &val)
  {
    return Color(std::exp(val.x), std::exp(val.y), std::exp(val.z));
  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace std;
using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// AdaptiveRaymarcher::Params
//----------------------------------------------------------------------------//

AdaptiveRaymarcher::Params::Params()
  : threshold(0.1), 
    volumeStepLengthMult(4.0),
    doTrapezoidIntegration(1), 
    earlyTerminationThreshold(0.001)
{ 
  // Empty
}

//----------------------------------------------------------------------------//
// AdaptiveRaymarcher
//----------------------------------------------------------------------------//

void AdaptiveRaymarcher::setParams(const Util::ParamMap &params)
{
  getValue(params.floatMap, k_strThreshold, 
           m_params.threshold);
  getValue(params.floatMap, k_strVolumeStepLengthMult, 
           m_params.volumeStepLengthMult);
  getValue(params.intMap, k_strDoTrapezoidIntegration,
           m_params.doTrapezoidIntegration);
  getValue(params.floatMap, k_strEarlyTermThresh, 
           m_params.earlyTerminationThreshold);

  cout << "Threshold: " << m_params.threshold << endl;
}

//----------------------------------------------------------------------------//

IntegrationResult
AdaptiveRaymarcher::integrate(const RayState &state) const
{
  // Integration intervals ---

  IntervalVec rawIntervals = RenderGlobals::scene()->volume->intersect(state);
  IntervalVec intervals = splitIntervals(rawIntervals);

  if (intervals.size() == 0) {
    return IntegrationResult();
  }

  // Output transmittance function ---

  ColorCurve::Ptr lf = setupDeepLCurve(state, intervals[0].t0);
  ColorCurve::Ptr tf = setupDeepTCurve(state, intervals[0].t0);

  // Ray integration variables ---

  VolumeSampleState sampleState(state);
  Color             L = Colors::zero();
  Color             T = Colors::one();

  Color previousL = Colors::zero();
  Color previousT = Colors::one();
  
  // Interval loop ---

  BOOST_FOREACH (const Interval &interval, intervals) {

    // Interval integration variables
    const double tStart = std::max(interval.t0, state.tMin);
    const double tEnd   = std::min(interval.t1, state.tMax);

    // Base step length, used as upper bound
    const double baseStepLength = 
      std::min(interval.stepLength * m_params.volumeStepLengthMult,
               tEnd - tStart);

    // Set up first raymarch step
    double stepT0 = tStart;
    double stepT1 = tStart + baseStepLength;

    // Prevent infinite loops.
    if (stepT0 == stepT1) {
      continue;
    }

    // Raymarch loop ---
 
    bool doTerminate = false;

    while (stepT0 < tEnd) {

      const double stepLength = stepT1 - stepT0;
      const double t = stepT1;

      sampleState.wsP = state.wsRay(t);
      RaymarchSample sample = m_raymarchSampler->sample(sampleState);

#if 1
      const double divisor = std::sqrt(Math::max(T) / m_params.threshold);
#else
      const double divisor = std::log(1.0 + Math::max(T) / m_params.threshold);
#endif
      const double adaptedStepLength = baseStepLength / divisor;

      // Check sample ---

      if (Math::max(sample.extinction) > 0.0) {
        // Find the maximum allowed step length
        const double meanFreePath = 1.0 / Math::max(sample.extinction);
        const double suggestedStep = meanFreePath / divisor;
        // Check if we need to reduce the step size
        if (stepLength > suggestedStep * 1.1) {
          // Recompute step with shorter step length
          stepT1 = std::min(stepT0 + std::min(suggestedStep, adaptedStepLength),
                            tEnd);
          continue;
        } else {
          // Step length was ok, set up next step
          stepT0 = stepT1;
          stepT1 = std::min(stepT1 + std::min(suggestedStep, adaptedStepLength),
                            tEnd);
        }
      } else {
        // No attentuation coefficient. Sample according to base step length.
        stepT0 = stepT1;
        stepT1 = std::min(stepT1 + adaptedStepLength, tEnd);
      }

      // Accept sample ---

      // ... Update transmittance
      if (Math::max(sample.extinction) > 0.0f) {
        if (m_params.doTrapezoidIntegration) {
          Color transmittanceTerm = exp(-sample.extinction * stepLength);
          T *= (transmittanceTerm + previousT) * 0.5;
          previousT = transmittanceTerm;
        } else {
          T *= exp(-sample.extinction * stepLength);
        }
      }

      // ... Add to luminance
      if (m_params.doTrapezoidIntegration) {
        Color luminanceTerm = sample.luminance * T * stepLength;
        L += (luminanceTerm + previousL) * 0.5;
        previousL = luminanceTerm;
      } else {
        L += sample.luminance * T * stepLength;
      }

      // Early termination
      if (Math::max(T) < m_params.earlyTerminationThreshold) {
        T = Colors::zero();
        doTerminate = true;
      }

      // Update transmittance and luminance functions
      if (tf || lf) {
        updateDeepFunctions(stepT1, L, T, lf, tf);
      }

      // Terminate if requested
      if (doTerminate) {
        break;
      }

    }

  } // end for each interval

  if (tf) {
    tf->removeDuplicates();
  }

  if (lf) {
    lf->removeDuplicates();
  }

  return IntegrationResult(L, lf, T, tf);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
