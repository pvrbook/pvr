#! /usr/bin/env python

import os, sys

def handleDir(arg, dir, files):
    if "render.py" in files:
        dirs.append(dir)

dirs = []

if len(sys.argv) > 1:
    for dir in sys.argv[1:]:
        os.path.walk(dir, handleDir, None)
else:
    os.path.walk(".", handleDir, None)

files = []

for dir in dirs:
    files.append(os.path.join(dir, "reference.png"))
    files.append(os.path.join(dir, "out", "image.png"))

cmd = "open "
cmd += " ".join(files)

os.system(cmd)


