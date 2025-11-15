#include "angleloader.h"
#include "gles_loader_autogen.h"
#include "egl_loader_autogen.h"

namespace angle {
	Library *g_eglLibrary = nullptr;
	Library *g_glesLibrary = nullptr;
}   // namespace angle

static angle::GenericProc KHRONOS_APIENTRY getEGLProcAddress(const char* symbol) {
	return reinterpret_cast<angle::GenericProc>(angle::g_eglLibrary->getSymbol(symbol));
}

static angle::GenericProc KHRONOS_APIENTRY getGLESProcAddress(const char* symbol) {
	return reinterpret_cast<angle::GenericProc>(angle::g_glesLibrary->getSymbol(symbol));
}

void angle::loadEGLLibrary(const char* libraryName)
{ 
	if (!g_eglLibrary) {
        g_eglLibrary = angle::OpenSharedLibraryWithExtension(libraryName, angle::SearchType::ModuleDir);
		if (g_eglLibrary) {
			angle::LoadEGL(getEGLProcAddress);
		}
	}
}

void angle::unloadEGLLibrary() {
    if (g_eglLibrary) {
        angle::CloseSystemLibrary(g_eglLibrary);
        g_eglLibrary = nullptr;
	}
}

void angle::loadGLESLibrary(const char* libraryName)
{ 
	if (!g_glesLibrary) {
        g_glesLibrary = angle::OpenSharedLibraryWithExtension(libraryName, angle::SearchType::ModuleDir);
		if (g_glesLibrary) {
			angle::LoadGLES(getGLESProcAddress);
		}
	}
}

void angle::unloadGLESLibrary() {
    if (g_glesLibrary) {
        angle::CloseSystemLibrary(g_glesLibrary);
        g_glesLibrary = nullptr;
    }
}