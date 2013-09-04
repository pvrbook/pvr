# ------------------------------------------------------------------------------
# BuildSupport.py
# ------------------------------------------------------------------------------

from SCons import Script

import os, subprocess, sys

# ------------------------------------------------------------------------------
# Strings
# ------------------------------------------------------------------------------

pvrName        = "pvr"

buildDirPath   = "build"
installDirPath = "install"

release        = "release"
debug          = "debug"
export         = "export"
include        = "include"
src            = "src"
lib            = "lib"

python         = "python"
python26       = "python26"
python26apple  = "python26-apple"

siteFile       = "Site.py"

arch32         = "m32"
arch64         = "m64"

darwin         = "darwin"
linux2         = "linux2"

# ------------------------------------------------------------------------------
# Paths
# ------------------------------------------------------------------------------

systemIncludePaths = {
    darwin : { arch32 : ["/usr/local/include",
                         "/opt/local/include"],
               arch64 : ["/usr/local/include",
                         "/opt/local/include"] },
    linux2 : { arch32 : ["/usr/local/include"],
               arch64 : ["/usr/local64/include", "/usr/include"] }
    }

systemLibPaths = {
    darwin : { arch32 : ["/usr/local/lib",
                         "/opt/local/lib"],
               arch64 : ["/usr/local/lib",
                         "/opt/local/lib"] },
    linux2 : { arch32 : ["/usr/local/lib"],
               arch64 : ["/usr/local64/lib",
                         "/usr/lib64"] }
    }

systemPyCppPaths = {
    darwin : { arch32 : { python26 :
                          ["/opt/local/Library/Frameworks/Python.framework/Versions/2.6/Headers"],
                          python26apple : 
                          ["/System/Library/Frameworks/Python.framework/Versions/2.6/Headers"] },
               arch64 : { python26 : 
                          ["/opt/local/Library/Frameworks/Python.framework/Versions/2.6/Headers"],
                          python26apple : 
                          ["/System/Library/Frameworks/Python.framework/Versions/2.6/Headers"] } },
    linux2 : { arch32 : { python26 : 
                          ["/usr/include/python2.6"] },
               arch64 : { python26 :
                          ["/usr/include/python2.6"] } }
    }

systemPyLibPaths = {
    darwin : { arch32 : { python26 :
                          ["/opt/local/Library/Frameworks/Python.framework/Versions/2.6"],
                          python26apple : 
                          ["/System/Library/Frameworks/Python.framework/Versions/2.6"] },
               arch64 : { python26 : 
                          ["/opt/local/Library/Frameworks/Python.framework/Versions/2.6"],
                          python26apple : 
                          ["/System/Library/Frameworks/Python.framework/Versions/2.6"] } },
    linux2 : { arch32 : { python26 :
                          ["/usr/lib/python2.6"] },
               arch64 : { python26 :
                          ["/usr/lib/python2.6"] } }
}

systemLibs = {
    darwin : [],
    linux2 : ["dl"]
    }

# ------------------------------------------------------------------------------
# Functions
# ------------------------------------------------------------------------------

def targetColor(s):
    return "\033[1m" + s + "\033[0m"

# ------------------------------------------------------------------------------

def isDebugBuild():
    return int(Script.ARGUMENTS.get('debug', 0))

# ------------------------------------------------------------------------------

def architectureStr():
    if int(Script.ARGUMENTS.get('do64', 1)):
        return arch64
    else:
        return arch32

# ------------------------------------------------------------------------------

def compilerStr(env):
    baseStr = env["CXX"]
    compilerInfo = subprocess.Popen(
            baseStr,
            shell=True,
            bufsize=4096,
            stderr=subprocess.PIPE).stderr.readlines()

    longName = compilerInfo[0].split(":")[0]
    shortName = "-".join(longName.split("-")[-3:])
    return shortName

# ------------------------------------------------------------------------------

def pythonVer():
    if sys.platform == darwin:
        return "python" + Script.ARGUMENTS.get('python', '26-apple')
    else:
        return "python" + Script.ARGUMENTS.get('python', '26')

# ------------------------------------------------------------------------------

def setupBuildOutput(env):
    if Script.ARGUMENTS.get("verbose") != "1":
        env["ARCOMSTR"]       = "Running ar " + targetColor("$TARGET")
        env["CXXCOMSTR"]      = "Compiling  " + targetColor("$TARGET")
        env["SHCXXCOMSTR"]    = "Compiling  " + targetColor("$TARGET")
        env["LDMODULECOMSTR"] = "Compiling  " + targetColor("$TARGET")
        env["LINKCOMSTR"]     = "Linking    " + targetColor("$TARGET")
        env["SHLINKCOMSTR"]   = "Linking    " + targetColor("$TARGET")
        env["INSTALLSTR"]     = "Installing " + targetColor("$TARGET")

# ------------------------------------------------------------------------------

def variantDir(env):
    basePath = os.path.join(sys.platform, compilerStr(env), architectureStr())
    variant = ""
    if isDebugBuild():
        variant += debug
    else:
        variant += release
    return os.path.join(basePath, variant)

# ------------------------------------------------------------------------------

def buildDir(env):
    return os.path.join(buildDirPath, variantDir(env))

# ------------------------------------------------------------------------------

def installDir(env):
    return os.path.join(installDirPath, variantDir(env))

# ------------------------------------------------------------------------------

def addSpComp2(env, name):
    env.Append(CPPPATH = [os.path.join("/shots/spi/home/lib/SpComp2", name, 
                               "spinux1-gcc44m64/current/include")])
    env.Append(LIBPATH = [os.path.join("/shots/spi/home/lib/SpComp2", name,
                               "spinux1-gcc44m64/current/lib")])
    env.Append(RPATH =   [os.path.join("/shots/spi/home/lib/SpComp2", name,
                               "spinux1-gcc44m64/current/lib")])
    
# ------------------------------------------------------------------------------

def setupEnv(env, pathToRoot = "."):
    setupBuildOutput(env)
    # Base paths
    baseIncludePaths = systemIncludePaths[sys.platform][architectureStr()]
    baseLibPaths = systemLibPaths[sys.platform][architectureStr()]
    baseLibs = systemLibs[sys.platform]
    # Compiler
    compiler = Script.ARGUMENTS.get('compiler', '')
    if compiler != '':
        env.Replace(CXX = compiler)
    # System include paths
    env.Append(CPPPATH = baseIncludePaths)
    # System lib paths
    env.Append(LIBPATH = baseLibPaths)
    # System libs
    env.Append(LIBS = baseLibs)
    for path in baseIncludePaths:
        env.Append(CPPPATH = os.path.join(path, "OpenEXR"))
    # Sony crap
    if "SP_OS" in os.environ.keys() and "spinux" in os.environ["SP_OS"]:
        # Imath
        env.Append(LIBS = ["SpiHalf"])
        env.Append(LIBS = ["SpiIex"])
        env.Append(LIBS = ["SpiImath"])
        # OIIO
        addSpComp2(env, "OpenImageIO")
        env.Append(CPPDEFINES = {"OPENIMAGEIO_NAMESPACE": "SPI_OpenImageIO_v28"})
        # Others
        addSpComp2(env, "Field3D")
        addSpComp2(env, "GPD")
        # Boost threads
        env.Append(LIBS = ["boost_thread-gcc44-mt"])
    else:
        env.Append(LIBS = ["Half"])
        env.Append(LIBS = ["Iex"])
        env.Append(LIBS = ["Imath"])
        env.Append(LIBS = ["boost_thread-mt"])
    env.Append(LIBS = ["OpenImageIO"])
    env.Append(LIBS = ["Field3D"])
    env.Append(LIBS = ["GPD-pvr"])
    # System libs
    env.Append(LIBS = ["z", "pthread"])
    # Hdf5 lib
    env.Append(LIBS = ["hdf5"])
    # Externals
    env.Append(CPPPATH = os.path.join(pathToRoot, "external/include"))
    env.Append(LIBPATH = os.path.join(pathToRoot, "external/libs/" + variantDir(env)))
    # Compile flags
    if isDebugBuild():
        env.Append(CCFLAGS = ["-g"])
    else:
        env.Append(CCFLAGS = ["-O3"])
    env.Append(CCFLAGS = ["-Wall"])
    # Set number of jobs to use
    env.SetOption("num_jobs", numCPUs())
    # 64 bit setup
    if architectureStr() == arch64:
        env.Append(CCFLAGS = ["-m64"])
        env.Append(LINKFLAGS = ["-m64"])
    else:
        env.Append(CCFLAGS = ["-m32"])
        env.Append(LINKFLAGS = ["-m32"])

# ------------------------------------------------------------------------------

def addLibPVR(env, pathToRoot):
    fullInstallDir = os.path.abspath(os.path.join(pathToRoot, installDir(env)))
    env.Append(CPPPATH = [os.path.join(fullInstallDir, include)])
    env.Append(LIBS = [pvrName])
    env.Append(LIBPATH = [os.path.join(fullInstallDir, lib)])
    env.Append(RPATH = [os.path.join(fullInstallDir, lib)])

# ------------------------------------------------------------------------------

def setupLibBuildEnv(env, pathToRoot = "."):
    setupEnv(env)
    # Build in separate dir
    env.VariantDir(buildDir(env), src, duplicate = 0)    
    # Project headers
    env.Append(CPPPATH = [os.path.join(pathToRoot, installDir(env), include)])

# ------------------------------------------------------------------------------

def setupPyEnv(env, pathToRoot = "."):
    setupEnv(env)
    # Build in separate dir
    env.VariantDir(os.path.join(buildDir(env), pythonVer()), python, duplicate = 0)
    # Build against the 'installed' pvr library
    addLibPVR(env, pathToRoot)
    # Lib paths
    env.Append(LIBPATH = [os.path.join(installDir(env), "lib")])
    # Libraries
    env.Prepend(CPPPATH = systemPyCppPaths[sys.platform][architectureStr()][pythonVer()])
    env.Prepend(LIBPATH = systemPyLibPaths[sys.platform][architectureStr()][pythonVer()])
    env.Append(LIBS = ["boost_python"])
    # OSX specific
    if sys.platform == darwin:
        # env.Append(FRAMEWORKS = ["Python"])
        env.Append(LIBS = ["Python2.6"])
    # Linux specific
    elif sys.platform == linux2:
        env.Append(LIBS = ["python2.6"])
        
# ------------------------------------------------------------------------------

def setupAppEnv(env, pathToRoot = "."):
    setupEnv(env)
    addLibPVR(env, pathToRoot)

# ------------------------------------------------------------------------------

def numCPUs():
    if os.sysconf_names.has_key("SC_NPROCESSORS_ONLN"):
        nCPUs = os.sysconf("SC_NPROCESSORS_ONLN")
        if isinstance(nCPUs, int) and nCPUs > 0:
            return nCPUs
    else: 
        return int(os.popen2("sysctl -n hw.ncpu")[1].read())
    if os.environ.has_key("NUMBER_OF_PROCESSORS"):
         nCPUs = int(os.environ["NUMBER_OF_PROCESSORS"]);
         if nCPUs > 0:
             return nCPUs
    return 1

# ------------------------------------------------------------------------------

def setupHeaderInstall(env):
    targets = []
    for (path, dirs, files) in os.walk(export):
        try:
            dirs.remove(".svn")
        except:
	        pass
        currentDir = "/".join(path.split("/")[1:])
        headerDir = os.path.join(installDir(env), include, pvrName)
        if len(currentDir) > 0:
            headerDir = os.path.join(headerDir, currentDir)
        headerFiles = Script.Glob(os.path.join(path, "*.h"))
        targets += env.Install(headerDir, headerFiles)
    return targets

# ------------------------------------------------------------------------------

def setDylibInternalPath(target, source, env):
    # Copy the library file
    srcName = str(source[0])
    tgtName = str(target[0])
    Script.Execute(Script.Copy(tgtName, srcName))
    # Then run install_name_tool
    cmd = "install_name_tool "
    cmd += "-id " + os.path.abspath(tgtName) + " "
    cmd += tgtName
    os.system(cmd)

# ------------------------------------------------------------------------------

def defineBoostPythonModule(name, files, env):
    shLib = env.SharedLibrary(os.path.join(buildDir(env), pythonVer(), "_" + name), files, 
                              SHLIBPREFIX = "", 
                              SHLIBSUFFIX = ".so")
    return shLib

# ------------------------------------------------------------------------------

def installPyLib(env, lib, files):
    setupBuildOutput(env)
    os.environ['PVR_PYTHON_PATH'] = "/opt/local/lib/python2.7"
    if "PVR_PYTHON_PATH" not in os.environ.keys():
        print "$PVR_PYTHON_PATH was not set. Can't install."
    installDir = os.environ["PVR_PYTHON_PATH"]
    libInstall = env.Install(os.path.join(installDir, pvrName), lib)
    filesInstall = env.Install(os.path.join(installDir, pvrName), files)
    env.Depends(libInstall, lib)
    env.Depends(filesInstall, files)
    return [libInstall, filesInstall]

# ------------------------------------------------------------------------------

def makeSimpleProgram(env, pathToRoot, program, srcDir):
    setupEnv(env, pathToRoot)
    addLibPVR(env, pathToRoot)
    env.VariantDir(buildDir(env), srcDir, duplicate=0)
    files = Script.Glob(os.path.join(buildDir(env), "*.cpp"))
    env.Program(program, files)


# ------------------------------------------------------------------------------
