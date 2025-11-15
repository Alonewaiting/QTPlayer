#pragma once
#include "system_utils.h"
namespace angle {
	extern Library* g_eglLibrary;
	extern Library* g_glesLibrary;

	 void loadEGLLibrary(const char* libraryName);
     void unloadEGLLibrary();
     void loadGLESLibrary(const char *libraryName);
     void unloadGLESLibrary();
}
