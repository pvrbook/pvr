//----------------------------------------------------------------------------//

/*! \file Renderer.cpp
  Contains implementations of Renderer class and related functions.
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Renderer.h"

// System includes

// Library includes

// Project headers

#include "pvr/Constants.h"
#include "pvr/RenderGlobals.h"
#include "pvr/Interrupt.h"
#include "pvr/Log.h"
#include "pvr/Scene.h"
#include "pvr/Strings.h"

//----------------------------------------------------------------------------//
// Local namespace
//----------------------------------------------------------------------------//

namespace {

  //--------------------------------------------------------------------------//

  void printVolumeInfo(pvr::Render::Volume::CPtr volume, 
                       const int &indentLevel = 0)
  {
    using namespace pvr::Render;
    using namespace pvr::Util;

    std::string indentStr = "  ";
    std::string indent;
    for (int i = 0; i < indentLevel; i++) {
      indent += indentStr;
    }

    Volume::AttrNameVec attrs = volume->attributeNames();
    Volume::StringVec info    = volume->info();
    Volume::CVec inputs       = volume->inputs();

    Log::print(indent + "(" + volume->typeName() + ")");

    BOOST_FOREACH (const std::string &attr, attrs) {
      Log::print(indent + indentStr + "a " + attr);
    }

    BOOST_FOREACH (const std::string &line, info) {
      Log::print(indent + indentStr + "i " + line);
    }

    Log::print(indent + indentStr + "p " + volume->phaseFunction()->typeName());

    BOOST_FOREACH (Volume::CPtr volume, inputs) {
      printVolumeInfo(volume, indentLevel + 1);
    }

  }

  //--------------------------------------------------------------------------//

} // local namespace

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;
using namespace Imath;

using namespace pvr::Util;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Render {

//----------------------------------------------------------------------------//
// Renderer::Params
//----------------------------------------------------------------------------//

Renderer::Params::Params()
  : doPrimary(true), doLuminanceMap(false), doTransmittanceMap(false), 
    doRandomizePixelSamples(false)
{ 
  
}

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

Renderer::Ptr Renderer::clone() const
{
  // First copy everything from this
  Ptr renderer(new Renderer(*this));
  // Deep-copy the non-const data members
  renderer->m_primary = m_primary->clone();
  renderer->m_deepTransmittance = m_deepTransmittance->clone();
  renderer->m_deepLuminance = m_deepLuminance->clone();
  if (m_scene) {
    renderer->m_scene = m_scene->clone();
  }
  return renderer;
}

//----------------------------------------------------------------------------//

Renderer::Ptr Renderer::create()
{ 
  return Ptr(new Renderer); 
}

//----------------------------------------------------------------------------//

void Renderer::setCamera(Camera::CPtr camera)
{
  assert(camera != NULL && "Got null pointer in Renderer::setCamera");

  m_camera = camera;
  V2i res = m_camera->resolution();
  m_primary = Image::create();
  m_primary->setSize(res.x, res.y);
  m_deepTransmittance = DeepImage::create();
  m_deepTransmittance->setSize(res.x, res.y);
  m_deepLuminance = DeepImage::create();
  m_deepLuminance->setSize(res.x, res.y);
}

//----------------------------------------------------------------------------//

void Renderer::setRaymarcher(Raymarcher::CPtr raymarcher)
{
  m_raymarcher = raymarcher;
}

//----------------------------------------------------------------------------//

void Renderer::addVolume(Volume::CPtr volume)
{
  if (!m_scene) {
    m_scene = Scene::Ptr(new Scene);
  }
  m_scene->volume = volume;
}

//----------------------------------------------------------------------------//

void Renderer::addLight(Light::CPtr light)
{
  if (!m_scene) {
    m_scene = Scene::Ptr(new Scene);
  }
  m_scene->lights.push_back(light);
}

//----------------------------------------------------------------------------//

void Renderer::printSceneInfo() const
{
  if (!m_scene) {
    throw MissingSceneException();
  }

  if (!m_scene->volume) {
    throw MissingVolumeException();
  }

  Log::print("Scene info:");
  printVolumeInfo(m_scene->volume, 1);
}

//----------------------------------------------------------------------------//

void Renderer::setPrimaryEnabled(const bool enabled)
{
  m_params.doPrimary = enabled;
}

//----------------------------------------------------------------------------//

void Renderer::setTransmittanceMapEnabled(const bool enabled)
{
  m_params.doTransmittanceMap = enabled;
}

//----------------------------------------------------------------------------//

void Renderer::setLuminanceMapEnabled(const bool enabled)
{
  m_params.doLuminanceMap = enabled;
}

//----------------------------------------------------------------------------//

void Renderer::setDoRandomizePixelSamples(const bool enabled)
{
  m_params.doRandomizePixelSamples = enabled;
}

//----------------------------------------------------------------------------//

//! \todo Replace RNG with a proper Sampler!!!
void Renderer::execute()
{
  if (m_params.doPrimary) {
    Log::print("Rendering image " + str(m_primary->size()));
  } else {
    Log::print("Rendering transmittance map " + str(m_primary->size()));
  }

  if (!m_camera) {
    throw MissingCameraException();
  }

  if (!m_scene) {
    throw MissingSceneException();
  }

  if (!m_scene->volume) {
    throw MissingVolumeException();
  }

  if (!m_raymarcher) {
    throw MissingRaymarcherException();
  }

  // Initialization ---

  RenderGlobals::setScene(m_scene);
  RenderGlobals::setCamera(m_camera);

  m_rng.init(0);
  Timer timer;
  ProgressReporter progress(2.5f, "  ");

  // For each pixel ---

  for (Image::pixel_iterator i = m_primary->begin(), end = m_primary->end();
       i != end; ++i) {
    // Check if user terminated
    Sys::Interrupt::throwOnAbort();
    // Print progress
    progress.update(i.progress());
    // Set up pixel sample and time
    float xSample = i.rsX();
    float ySample = i.rsY();
    if (m_params.doRandomizePixelSamples) {
      xSample += m_rng.nextf() - 0.5f;
      ySample += m_rng.nextf() - 0.5f;
    }
    PTime pTime(m_rng.nextf());
    // Render pixel
    IntegrationResult result = integrateRay(xSample, ySample, pTime);
    // Update resulting image and transmittance/luminance maps
    Color alpha = Colors::one() - result.transmittance;
    i.setPixel(result.luminance);
    i.setPixelAlpha((alpha.x + alpha.y + alpha.z) / 3.0f);
    if (result.transmittanceFunction) {
      m_deepTransmittance->setPixel(i.x, i.y, result.transmittanceFunction);
    }
    if (result.luminanceFunction) {
      m_deepLuminance->setPixel(i.x, i.y, result.luminanceFunction);
    }
  }

  Log::print("  Time elapsed: " + str(timer.elapsed()));
}
  
//----------------------------------------------------------------------------//

IntegrationResult Renderer::trace(const RayState &state) const
{
  return m_raymarcher->integrate(state);
}

//----------------------------------------------------------------------------//

Raymarcher::CPtr Renderer::raymarcher() const
{
  return m_raymarcher;
}

//----------------------------------------------------------------------------//

DeepImage::Ptr Renderer::transmittanceMap() const
{
  return m_deepTransmittance;
}

//----------------------------------------------------------------------------//

DeepImage::Ptr Renderer::luminanceMap() const
{
  return m_deepLuminance;
}

//----------------------------------------------------------------------------//

void Renderer::saveImage(const std::string &filename) const
{
  m_primary->write(filename, Image::RGBA);
}

//----------------------------------------------------------------------------//

Ray Renderer::setupRay(const float x, const float y, const PTime time) const
{
  // Find camera position at current time. This will serve as the Ray's origin
  const Vector wsEye = m_camera->position(time);
  // Compute direction by transforming pixel position in raster space to
  // world space
  const Vector rsP(x, y, 0.0);
  Vector wsP = m_camera->rasterToWorld(rsP, time);

  return Ray(wsEye, wsP);
}

//----------------------------------------------------------------------------//

IntegrationResult Renderer::integrateRay(const float x, const float y,
                                         const PTime time) const
{
  // Create default RayState. Rely on its constructor to set reasonable
  // defaults
  RayState state;
  // Update the values that are non-default
  state.wsRay         = setupRay(x, y, time);
  state.time          = time;
  state.doOutputDeepT = m_params.doTransmittanceMap;
  state.doOutputDeepL = m_params.doLuminanceMap;
  // Let the Raymarcher do the integration work
  return m_raymarcher->integrate(state);
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
