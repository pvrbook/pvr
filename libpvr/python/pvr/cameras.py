# ------------------------------------------------------------------------------
# cameras.py
# ------------------------------------------------------------------------------

from math import degrees

import imath

import pvr

# ------------------------------------------------------------------------------

def standard(resMult = 1.0):
    # Settings
    resolution = imath.V2i(int(2048 * resMult), int(1556 * resMult))
    position   = imath.V3f(0.0, 0.0, 10.0)
    fov        = degrees(pvr.calculateVerticalFOV(45.0, 24.0, resolution))
    fovCurve   = pvr.FloatCurve()
    fovCurve.addSample(0.0, fov)
    # Create camera
    cam = pvr.PerspectiveCamera()
    cam.setPosition(position)
    cam.setVerticalFOV(fovCurve)
    cam.setClipPlanes(0.1, 1.0)
    cam.setResolution(resolution)
    # Done
    return cam

# ------------------------------------------------------------------------------
