//----------------------------------------------------------------------------//

/*! \file NaiveRaymarcher.cpp
  Contains implementations of NaiveRaymarcher class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Raymarchers/NaiveRaymarcher.h"

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

  const std::string k_strStepLength("step_length");
  const std::string k_strUseVolumeStepLength("use_volume_step_length");
  const std::string k_strVolumeStepLengthMult("volume_step_length_multiplier");
  const std::string k_strDoEarlyTerm("do_early_termination");
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
// NaiveRaymarcher::Params
//----------------------------------------------------------------------------//

NaiveRaymarcher::Params::Params()
  : stepLength(1.0), useVolumeStepLength(true), volumeStepLengthMult(1.0),
    doEarlyTermination(true), earlyTerminationThreshold(0.001)
{ 
  // Empty
}

//----------------------------------------------------------------------------//
// NaiveRaymarcher
//----------------------------------------------------------------------------//

void NaiveRaymarcher::setParams(const Util::ParamMap &params)
{
  getValue(params.floatMap, k_strStepLength, 
           m_params.stepLength);
  getValue(params.intMap, k_strUseVolumeStepLength, 
           m_params.useVolumeStepLength);
  getValue(params.floatMap, k_strVolumeStepLengthMult, 
           m_params.volumeStepLengthMult);
  getValue(params.intMap, k_strDoEarlyTerm, 
           m_params.doEarlyTermination);
  getValue(params.floatMap, k_strEarlyTermThresh, 
           m_params.earlyTerminationThreshold);
}

//----------------------------------------------------------------------------//

IntegrationResult
NaiveRaymarcher::integrate(const RayState &state) const
{
  // Integration intervals ---

  IntervalVec rawIntervals = RenderGlobals::scene()->volume->intersect(state);
  IntervalVec intervals = splitIntervals(rawIntervals);

  if (intervals.size() == 0) {
    return IntegrationResult();
  }

  // Output transmittance function and luminance function ---

  ColorCurve::Ptr lf = setupDeepLCurve(state, intervals[0].t0);
  ColorCurve::Ptr tf = setupDeepTCurve(state, intervals[0].t0);

  // Ray integration variables ---

  VolumeSampleState sampleState(state);
  Color             L = Colors::zero();
  Color             T = Colors::one();

  // Interval loop ---

  BOOST_FOREACH (const Interval &interval, intervals) {

    // Interval integration variables
    const double tStart = std::max(interval.t0, state.tMin);
    const double tEnd   = std::min(interval.t1, state.tMax);

    // Pick step length
    const double stepLengthToUse =
      m_params.useVolumeStepLength ? 
      interval.stepLength * m_params.volumeStepLengthMult : 
      m_params.stepLength;
    const double baseStepLength = std::min(stepLengthToUse, tEnd - tStart);

    // Set up first raymarch step
    double stepT0 = tStart;
    double stepT1 = tStart + baseStepLength;

    // Prevent infinite loops
    if (stepT0 == stepT1) {
      continue;
    }

    // Raymarch loop ---
 
    bool doTerminate = false;

    while (stepT0 < tEnd) {

      const double stepLength = stepT1 - stepT0;
      const double t = (stepT0 + stepT1) * 0.5;
      sampleState.wsP = state.wsRay(t);

      RaymarchSample sample = m_raymarchSampler->sample(sampleState);

      // Update transmittance
      if (Math::max(sample.extinction) > 0.0f) {
        T *= exp(-sample.extinction * stepLength);
      }

      // Update luminance
      L += sample.luminance * T * stepLength;

      // Early termination
      if (m_params.doEarlyTermination &&
          Math::max(T) < m_params.earlyTerminationThreshold) {
        T = Colors::zero();
        doTerminate = true;
      }

      // Update transmittance and luminance functions
      if (tf || lf) {
        updateDeepFunctions(state.wsRay(stepT1), L, T, lf, tf);
      }

      // Set up next steps
      stepT0 = stepT1;
      stepT1 = min(tEnd, stepT1 + baseStepLength);

      // Terminate if requested
      if (doTerminate) {
        break;
      }

    } // end raymarch of single interval

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
