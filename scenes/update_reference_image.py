#! /usr/bin/env python

import os, sys

def updateDir(dir):
    print "Updating", dir
    cmd = "cp " + os.path.join(dir, "out/image.png") + " " + \
        os.path.join(dir, "reference.png")
    os.system(cmd)

def handleDir(arg, dir, files):
    if "render.py" in files:
        updateDir(dir)

if len(sys.argv) > 1:
    for dir in sys.argv[1:]:
        os.path.walk(dir, handleDir, None)
else:
    os.path.walk(".", handleDir, None)
