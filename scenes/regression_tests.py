#! /usr/bin/env python

import os, sys
from optparse import OptionParser

# Settings ------------

# Exceptions ------------

class TestFail(Exception):
    def __init__(self, dir, err):
        self.dir = dir
        self.err = err

class ResultFail(Exception):
    def __init__(self, dir, err):
        self.dir = dir
        self.err = err

# Functions ------------

def getIdiff():
    if os.environ.has_key("PVR_IDIFF"):
        return os.environ["PVR_IDIFF"]
    else:
        return "idiff"

def compareResults(dir):
    print "[ regression_tests ] Comparing", dir
    idiff = getIdiff()
    cmd = idiff + " " + os.path.join(dir, "reference.png") + " " + \
        os.path.join(dir, "out", "image.png")
    result = os.system(cmd)
    if result != 0:
        raise ResultFail(dir, "Image difference")

def runTest(dir):
    print "[ regression_tests ] Running test in", dir
    scriptPath = os.path.join(dir, "render.py")
    if os.path.exists(scriptPath):
        cmd = "cd " + dir + ";"
        cmd += "./render.py"
        result = os.system(cmd)
        if result != 0:
            raise TestFail(dir, "Bad return code")

def handleDir(arg, dir, files):
    if "render.py" in files:
        try:
            if not options.compare:
                runTest(dir)
            compareResults(dir)
        except TestFail as e:
            failureDirs.append(e.dir)
        except ResultFail as e:
            failedResults.append(e.dir)

# Script ------------

failureDirs = []
failedResults = []

parser = OptionParser()
parser.add_option("-c", "--compare", dest="compare", default = False,
                  action = "store_true", help="Only compare files")

(options, args) = parser.parse_args()

if len(args) >= 1:
    for dir in args:
        os.path.walk(dir, handleDir, None)
else:
    os.path.walk(".", handleDir, None)
   
print "" 
print "########################"
print " PVR TEST SUITE RESULTS "
print "########################"
print ""

if len(failureDirs) == 0 and len(failedResults) == 0:
    print "All tests passed"
else:
    for dir in failureDirs:
        print "ERROR: Failed to run: " + dir
    for dir in failedResults:
        print "ERROR: Image difference: " + dir

print ""
