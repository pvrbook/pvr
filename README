###########################
Production Volume Rendering
###########################

This is the companion rendering library for the book Production Volume
Rendering. 

####################
### Dependencies ###
####################

PVR depends on the following libraries:

  Imath/Iex/Half
  boost_thread
  HDF5
  OpenImageIO
  Field3D
  GPD            (Included in code distribution)

On MacOSX, all of the above except OpenImageIO are available from MacPorts, which 
helps simplify their installation.

####################
### Building PVR ###
####################

After the code is checked out, cd into 'libpvr' and type 'scons -h'. This will 
list the available build targets.

scons lib
  Builds the shared library. (Which won't be of much use, but lets you check
  that the base builds ok.)

scons pylib
  Builds the python module, without installing it.

scons pyinstall
  Installs the python .so module and associated python files.
  This requires that you have set the PVR_PYTHON_PATH to indicate where the
  library gets installed. Typically this would be '~/python_libs', or something
  similar. The installation creates a 'pvr' directory in this path, and stores
  all the PVR python files there.

scons pypackage
  Creates a self-contained python module. This recursively analyzes which
  shared libraries the _pvr.so python module references, copies them to a
  libs/ subdir, and re-links the python module so that those are picked up
  upon import.

###############################
### Building on MacOSX Lion ###
###############################

The Python libraries that ship with OSX Lion are incompatible with GCC, and a 
small modification is required to the file "pyport.h". Scary, but necessary.

sudo emacs /System/Library/Frameworks/Python.framework/Versions/2.6/Headers/pyport.h

--- Include/pyport.h     (revision 933)
+++ Include/pyport.h     (working copy)
@@ -514,22 +514,40 @@
 #if __FreeBSD_version > 500039
 #include <ctype.h>
 #include <wctype.h>
+/* If the source file #includes <cctype> before "Python.h" then
+ * these macros will not be defined at this point and we
+ * should not define them either.
+ */
+#ifdef isalnum
 #undef isalnum
 #define isalnum(c) iswalnum(btowc(c))
+#endif
+#ifdef isalpha
 #undef isalpha
 #define isalpha(c) iswalpha(btowc(c))
+#endif
+#ifdef islower
 #undef islower
 #define islower(c) iswlower(btowc(c))
+#endif
+#ifdef isspace
 #undef isspace
 #define isspace(c) iswspace(btowc(c))
+#endif
+#ifdef isupper
 #undef isupper
 #define isupper(c) iswupper(btowc(c))
+#endif
+#ifdef tolower
 #undef tolower
 #define tolower(c) towlower(btowc(c))
+#endif
+#ifdef toupper
 #undef toupper
 #define toupper(c) towupper(btowc(c))
 #endif
 #endif
+#endif

