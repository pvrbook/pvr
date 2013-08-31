# ------------------------------------------------------------------------------
# renderers.py
# ------------------------------------------------------------------------------

import pvr

# ------------------------------------------------------------------------------

__stdRaymarcherParams = {
    "use_volume_step_length" : 1,
    "volume_step_length_multiplier" : 1.0, 
    "do_early_termination" : 1,
    "early_termination_threshold" : 0.01
    }

# ------------------------------------------------------------------------------

def standard(params):
    # Merge dictionaries
    p = __stdRaymarcherParams.copy()
    p.update(params)
    # Set up Renderer
    renderer = pvr.Renderer()
    sampler = pvr.PhysicalSampler()
    raymarcher = pvr.UniformRaymarcher()
    raymarcher.setRaymarchSampler(sampler)
    raymarcher.setParams(p)
    renderer.setRaymarcher(raymarcher)
    return renderer

# ------------------------------------------------------------------------------
