//----------------------------------------------------------------------------//

/*! \file UniformRaymarcher.cpp
  Contains implementations of UniformRaymarcher class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Raymarchers/UniformRaymarcher.h"

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

  void updateTransmittance(const Render::RayState &state, 
                           const double stepLength, 
                           Color &sigma_e, const Color &sigma_h, 
                           Color &T_e, Color &T_h, Color &T_alpha, Color &T_m)
  {
    Color expSigmaE = Colors::one();
    Color expSigmaH = Colors::one();

    // Update holdout transmittance 
    if (state.rayDepth > 0) {
      sigma_e += sigma_h;
    } else {
      // Update accumulated holdout
      if (Math::max(sigma_h) > 0.0f) {
        expSigmaH = exp(-sigma_h * stepLength);
        T_h *= expSigmaH;
      }
    }

    // Update transmittance
    if (Math::max(sigma_e) > 0.0f) {
      expSigmaE = exp(-sigma_e * stepLength);
      T_e *= expSigmaE;
    }

    // Update output transmittance
    if (state.rayDepth == 0) {
      T_m     = Math::lerp(T_alpha, T_m, expSigmaH);
      T_alpha = Math::lerp(T_m, T_alpha, expSigmaE);
    }
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
// UniformRaymarcher::Params
//----------------------------------------------------------------------------//

UniformRaymarcher::Params::Params()
  : stepLength(1.0), useVolumeStepLength(true), volumeStepLengthMult(1.0),
    doEarlyTermination(true), earlyTerminationThreshold(0.001)
{ 
  // Empty
}

//----------------------------------------------------------------------------//
// UniformRaymarcher
//----------------------------------------------------------------------------//

UniformRaymarcher::UniformRaymarcher()
  : m_holdoutAttr("holdout")
{

}

//----------------------------------------------------------------------------//

void UniformRaymarcher::setParams(const Util::ParamMap &params)
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
UniformRaymarcher::integrate(const RayState &state) const
{
  // Integration intervals ---

  IntervalVec rawIntervals = RenderGlobals::scene()->volume->intersect(state);
  IntervalVec intervals = splitIntervals(rawIntervals);

  if (intervals.size() == 0) {
    return IntegrationResult();
  }

  // Set up transmittance function and luminance function ---

  ColorCurve::Ptr lf = setupDeepLCurve(state, intervals[0].t0);
  ColorCurve::Ptr tf = setupDeepTCurve(state, intervals[0].t0);

  // Ray integration variables ---

  VolumeSampleState sampleState(state);
  Color             L       = Colors::zero();
  Color             T_e     = Colors::one();
  Color             T_h     = Colors::one();
  Color             T_alpha = Colors::one();
  Color             T_m     = Colors::zero();

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

      // Information about current step
      const double stepLength = stepT1 - stepT0;
      const double t          = (stepT0 + stepT1) * 0.5;
      sampleState.wsP         = state.wsRay(t);

      // Get holdout, luminance and extinction from the scene
      VolumeSample hoSample = 
        RenderGlobals::scene()->volume->sample(sampleState, m_holdoutAttr);
      RaymarchSample sample = m_raymarchSampler->sample(sampleState);

      // Update transmittance
      updateTransmittance(state, stepLength, sample.extinction, hoSample.value,
                          T_e, T_h, T_alpha, T_m);

      // Update luminance
      L += sample.luminance * T_e * T_h * stepLength;

      // Early termination
      if (m_params.doEarlyTermination &&
          Math::max(T_e) < m_params.earlyTerminationThreshold) {
        T_e         = Colors::zero();
        T_alpha     = Colors::zero();
        doTerminate = true;
      }

      // Update transmittance and luminance functions
      updateDeepFunctions(stepT1, L, T_e, lf, tf);

      // Set up next raymarch step
      stepT0 = stepT1;
      stepT1 = min(tEnd, stepT1 + baseStepLength);

      // Terminate if requested
      if (doTerminate) {
        break;
      }

    } // end raymarch of single interval

    if (doTerminate) {
      break;
    }

  } // end for each interval

  if (tf) {
    tf->removeDuplicates();
  }

  if (lf) {
    lf->removeDuplicates();
  }

  if (state.rayDepth == 0) {
    return IntegrationResult(L, lf, T_alpha, tf);
  } else {
    return IntegrationResult(L, lf, T_e, tf);
  }
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
