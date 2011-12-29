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

      // Check sample ---

      if (Math::max(sample.extinction) > 0.0) {
        // Find the maximum allowed step length
        const double meanFreePath = 1.0 / Math::max(sample.extinction);
        const double suggestedStep = 
          m_params.threshold * meanFreePath / std::sqrt(Math::max(T));
        // Check if we need to reduce the step size
        if (stepLength > suggestedStep * 1.1) {
          // Recompute step with shorter step length
          stepT1 = std::min(stepT0 + std::min(suggestedStep, baseStepLength),
                            tEnd);
          continue;
        } else {
          // Step length was ok, set up next step
          stepT0 = stepT1;
          stepT1 = std::min(stepT1 + std::min(suggestedStep, baseStepLength),
                            tEnd);
        }
      } else {
        // No attentuation coefficient. Sample according to base step length.
        stepT0 = stepT1;
        stepT1 = std::min(stepT1 + baseStepLength, tEnd);
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
        updateDeepFunctions(state.wsRay(stepT1), L, T, lf, tf);
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
