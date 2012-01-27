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
#include "pvr/PhaseFunction.h"
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

  void printLightInfo(const pvr::Render::Scene::LightVec &lights, 
                      const int &indentLevel = 0)
  {
    using namespace pvr;
    using namespace pvr::Render;
    using namespace pvr::Util;

    std::string indentStr = "  ";
    std::string indent;
    for (int i = 0; i < indentLevel; i++) {
      indent += indentStr;
    }

    BOOST_FOREACH (Scene::LightPtr light, lights) {
      Log::print(indent + "(" + light->typeName() + ")");
      Color intensity = light->intensity() * Phase::k_isotropic;
      Log::print(indent + indentStr + "i " + str(intensity));
      Log::print(indent + indentStr + "o " + light->occluder()->typeName());
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
    doRandomizePixelSamples(false), numPixelSamples(1)
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
    RenderGlobals::setScene(m_scene);
  }
  m_scene->volume = volume;
}

//----------------------------------------------------------------------------//

void Renderer::addLight(Light::CPtr light)
{
  if (!light) {
    Log::warning("Tried to add null pointer with Renderer::addLight()");
    return;
  }
  if (!m_scene) {
    m_scene = Scene::Ptr(new Scene);
    RenderGlobals::setScene(m_scene);
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
  printLightInfo(m_scene->lights, 1);
}

//----------------------------------------------------------------------------//

Scene::Ptr Renderer::scene() const
{
  return m_scene;
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

void Renderer::setNumPixelSamples(const size_t numSamples)
{
  m_params.numPixelSamples = numSamples;
}

//----------------------------------------------------------------------------//

size_t Renderer::numPixelSamples() const
{
  return m_params.numPixelSamples;
}
  
//----------------------------------------------------------------------------//

void Renderer::setNumDeepSamples(const size_t numSamples)
{
  if (m_deepTransmittance) {
    m_deepTransmittance->setNumSamples(numSamples);
  }
  if (m_deepLuminance) {
    m_deepLuminance->setNumSamples(numSamples);
  }
}

//----------------------------------------------------------------------------//

void Renderer::execute()
{
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

  // Log reporting ---

  const size_t numSamples = m_params.numPixelSamples;

  if (m_params.doPrimary) {
    Log::print("Rendering image " + str(m_primary->size()) + " (" + 
               str(numSamples) + " x " + str(numSamples) + ")");
  } else {
    Log::print("Rendering transmittance map " + str(m_primary->size()) + " (" + 
               str(numSamples) + " x " + str(numSamples) + ")");
  }

  // Initialization ---

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
    // Pixel result
    Color luminance = Colors::zero();
    Color alpha = Colors::zero();
    // Transmittance functions to be averaged
    std::vector<ColorCurve::CPtr> tf, lf;
    // For each pixel sample (in each dimension)
    for (size_t iX = 0; iX < numSamples; iX++) {
      for (size_t iY = 0; iY < numSamples; iY++) {
        float xSample = i.rsX();
        float ySample = i.rsY();
        if (m_params.doRandomizePixelSamples) {
          xSample += m_rng.nextf() - 0.5f;
          ySample += m_rng.nextf() - 0.5f;
        }
        PTime pTime((iX + iY * numSamples + m_rng.nextf()) / 
                    (numSamples * numSamples));
        // Render pixel
        IntegrationResult result = integrateRay(xSample, ySample, pTime);
        // Update accumulated result
        luminance += result.luminance;
        alpha += Colors::one() - result.transmittance;
        if (result.transmittanceFunction) {
          tf.push_back(result.transmittanceFunction);
        }
        if (result.luminanceFunction) {
          lf.push_back(result.luminanceFunction);
        }
      }
    }
    // Normalize luminance and transmittance
    luminance *= 1.0 / m_params.numPixelSamples;
    alpha *= 1.0 / m_params.numPixelSamples;
    // Update resulting image and transmittance/luminance maps
    i.setPixel(luminance);
    i.setPixelAlpha((alpha.x + alpha.y + alpha.z) / 3.0f);
    if (tf.size() > 0) {
      m_deepTransmittance->setPixel(i.x, i.y, ColorCurve::average(tf));
    }
    if (lf.size() > 0) {
      m_deepLuminance->setPixel(i.x, i.y, ColorCurve::average(lf));
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

IntegrationResult Renderer::integrateRay(const float x, const float y,
                                         const PTime time) const
{
  // Create default RayState. Rely on its constructor to set reasonable
  // defaults
  RayState state;
  // Update the values that are non-default
  state.wsRay         = setupRay(m_camera, x, y, time);
  state.time          = time;
  if (!m_params.doPrimary) {
    state.rayType = RayState::TransmittanceOnly;
    state.rayDepth = 1;
  }
  state.doOutputDeepT = m_params.doTransmittanceMap;
  state.doOutputDeepL = m_params.doLuminanceMap;
  // Let the Raymarcher do the integration work
  return m_raymarcher->integrate(state);
}

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

Ray setupRay(Camera::CPtr camera, const float x, const float y, 
             const PTime time) 
{
  // Find camera position at current time. This will serve as the Ray's origin
  const Vector wsEye = camera->position(time);
  // Compute direction by transforming pixel position in raster space to
  // world space
  const Vector rsP(x, y, 1.0);
  Vector wsP = camera->rasterToWorld(rsP, time);

  return Ray(wsEye, wsP);  
}

//----------------------------------------------------------------------------//

} // namespace Render
} // namespace pvr

//----------------------------------------------------------------------------//
