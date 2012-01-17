# ------------------------------------------------------------------------------
# cameras.py
# ------------------------------------------------------------------------------

from math import radians, degrees

from pvr import *

# ------------------------------------------------------------------------------

def standard(resMult = 1.0):
    # Settings
    resolution = V2i(int(2048 * resMult), int(1556 * resMult))
    position   = V3f(0.0, 0.0, 10.0)
    fov        = degrees(calculateVerticalFOV(45.0, 24.0, resolution))
    fovCurve   = FloatCurve()
    fovCurve.addSample(0.0, fov)
    # Create camera
    cam = PerspectiveCamera()
    cam.setPosition(position)
    cam.setVerticalFOV(fovCurve)
    cam.setClipPlanes(0.1, 1.0)
    cam.setResolution(resolution)
    # Done
    return cam

# ------------------------------------------------------------------------------
