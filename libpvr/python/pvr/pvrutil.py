# ------------------------------------------------------------------------------
# pvrutil.py
# ------------------------------------------------------------------------------

from math import radians, degrees

from pvr import *

# ------------------------------------------------------------------------------

def makeCamera(filename, mult = 1.0):
    # Exec the file
    code = "\n".join(open(filename).readlines())
    cam = eval(code)
    # Get static parameters
    res = V2i(int(cam["resx"] * mult), int(cam["resy"] * mult))
    # Get (potentially) time varying parameters
    t0 = cam
    t1 = cam
    if "current_frame" in cam.keys() and "next_frame" in cam.keys():
        t0 = cam["current_frame"]
        t1 = cam["next_frame"]
    posT0 = Vector(t0["tx"], t0["ty"], t0["tz"])
    eulerT0 = Euler(radians(t0["rx"]), radians(t0["ry"]), radians(t0["rz"]))
    rotT0 = eulerT0.toQuat()
    fovT0 = degrees(calculateVerticalFOV(t0["focal"], t0["aperture"], res))
    posT1 = Vector(t1["tx"], t1["ty"], t1["tz"])
    eulerT1 = Euler(radians(t1["rx"]), radians(t1["ry"]), radians(t1["rz"]))
    rotT1 = eulerT1.toQuat()
    fovT1 = degrees(calculateVerticalFOV(t1["focal"], t1["aperture"], res))
    # Create animation curves
    pTimeNext = 1.0 / RenderGlobals.shutter();
    pos = VectorCurve()
    rot = QuatCurve()
    fov = FloatCurve()
    pos.addSample(0.0, posT0)
    pos.addSample(pTimeNext, posT1)
    rot.addSample(0.0, rotT0)
    rot.addSample(pTimeNext, rotT1)
    fov.addSample(0.0, fovT0)
    fov.addSample(pTimeNext, fovT1)
    # Construct camera
    camera = PerspectiveCamera()
    camera.setPosition(pos)
    camera.setOrientation(rot)
    camera.setVerticalFOV(fov)
    camera.setClipPlanes(0.1, 1.0)
    camera.setResolution(res)
    return camera

# ------------------------------------------------------------------------------

def makeLights(filename, renderer, mult = 1.0):
    # Exec the file
    code = "\n".join(open(filename).readlines())
    lights = eval(code)
    retLights = []
    # Handle each light
    for lParms in lights:
        # Name
        logPrint("Adding light " + lParms["name"])
        # Parms
        res = V2i(int(lParms["resx"] * mult), int(lParms["resy"] * mult))
        pos = Vector(lParms["tx"], lParms["ty"], lParms["tz"])
        euler = Euler(radians(lParms["rx"]), 
                      radians(lParms["ry"]), 
                      radians(lParms["rz"]))
        rot = euler.toQuat()
        fov = lParms["fov"]
        color = Color(lParms["color_r"], lParms["color_g"], lParms["color_b"])
        intensity = lParms["intensity"]
        color.r = color.r * intensity
        color.g = color.g * intensity
        color.b = color.b * intensity
        # Create light
        light = PointLight()
        light.setPosition(pos)
        light.setIntensity(color)
        # Handle occluder type
        if lParms["shadow_type"] == "transmittance_map":
            if lParms["is_spotlight"]:
                # Create camera
                cam = PerspectiveCamera()
                cam.setPosition(pos)
                cam.setOrientation(rot)
                cam.setVerticalFOV(fov)
                cam.setResolution(res)
                # Create map render job
                mapRenderer = renderer.clone()
                mapRenderer.setCamera(cam)
                mapRenderer.setPrimaryEnabled(False)
                mapRenderer.setTransmittanceMapEnabled(True)
                mapRenderer.execute()
                # Set up occluder
                tMap = mapRenderer.transmittanceMap()
                tMap.printStats()
                occluder = TransmittanceMapOccluder()
                occluder.setTransmittanceMap(tMap)
                occluder.setCamera(cam)
                light.setOccluder(occluder)
            else:
                logWarning("Only spot lights supported for trans maps")
        elif lParms["shadow_type"] == "raymarch":
            occluder = RaymarchOccluder()
            occluder.setRaymarcher(renderer.raymarcher())
            light.setOccluder(occluder)
        retLights.append(light)
    return retLights
        
# ------------------------------------------------------------------------------

def setupTransmittanceMap(baseRenderer, light, resolution, orientation, fov, 
                          raymarcherType, raymarcherParams, samplerType):
    rend = baseRenderer.clone()
    # raymarcher = createRaymarcher(raymarcherType)
    # raymarcher.setParams(raymarcherParams)
    # sampler = createRaymarchSampler(samplerType)
    # raymarcher.setRaymarchSampler(sampler)
    # rend.setRaymarcher(raymarcher)
    cam = PerspectiveCamera()
    cam.setPosition(light.position())
    cam.setOrientation(orientation)
    cam.setVerticalFOV(fov)
    cam.setResolution(resolution)
    rend.setCamera(cam)
    rend.setPrimaryEnabled(False)
    rend.setTransmittanceMapEnabled(True)
    rend.execute()
    tMap = rend.transmittanceMap()
    tMap.printStats()
    occluder = TransmittanceMapOccluder()
    occluder.setTransmittanceMap(tMap)
    occluder.setCamera(cam)
    light.setOccluder(occluder)

# ------------------------------------------------------------------------------

def printArrayAttrs(table, namesFunc, refFunc, valFunc, typeStr):
    names = namesFunc()
    print "  " + typeStr + " attrs:"
    if len(names) == 0:
        print "     None"
        return
    for attr in names:
        print "    ", attr
        for i in range(0, table.size()):
            ref = refFunc(attr)
            vals = []
            for c in range(0, ref.arraySize()):
                vals.append(str(valFunc(ref, i, c)))
            print "       " + str(i) + ":", ", ".join(vals)

# ------------------------------------------------------------------------------

def printAttrs(table, namesFunc, refFunc, valFunc, typeStr):
    names = namesFunc()
    print "  " + typeStr + " attrs:"
    if len(names) == 0:
        print "     None"
        return
    for attr in names:
        print "    ", attr
        for i in range(0, table.size()):
            ref = refFunc(attr)
            val = valFunc(ref, i)
            print "       " + str(i) + ":", str(val)

# ------------------------------------------------------------------------------

def printStrAttrs(table):
    names = table.stringAttrNames()
    print "  string attrs:"
    if len(names) == 0:
        print "     None"
        return
    for attr in names:
        print "    ", attr
        for i in range(0, table.size()):
            ref = table.stringAttrRef(attr)
            val = table.stringAttr(ref, i)
            print "       " + str(i) + ":", str(val)

# ------------------------------------------------------------------------------

def printAttrTable(t):
    print "AttrTable:", t
    printArrayAttrs(t, t.intAttrNames, t.intAttrRef, t.intAttr, "int")
    printArrayAttrs(t, t.floatAttrNames, t.floatAttrRef, t.floatAttr, "float")
    printAttrs(t, t.vectorAttrNames, t.vectorAttrRef, t.vectorAttr, "vector")
    printAttrs(t, t.stringAttrNames, t.stringAttrRef, t.stringIdxAttr, "string index")
    printStrAttrs(t)
    
# ------------------------------------------------------------------------------

