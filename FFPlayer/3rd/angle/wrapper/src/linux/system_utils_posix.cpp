//
// Copyright 2018 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// system_utils_posix.cpp: Implementation of POSIX OS-specific functions.

#include "system_utils.h"

#include <array>

#include <dlfcn.h>

namespace angle
{

class PosixLibrary : public Library
{
  public:
    PosixLibrary(const char *libraryName, SearchType searchType)
    {
        std::string directory;
        if (searchType == SearchType::ApplicationDir)
        {
            static int dummySymbol = 0;
            Dl_info dlInfo;
            if (dladdr(&dummySymbol, &dlInfo) != 0)
            {
                std::string moduleName = dlInfo.dli_fname;
                directory              = moduleName.substr(0, moduleName.find_last_of('/') + 1);
            }
        }

        std::string fullPath = directory + libraryName;
        mModule              = dlopen(fullPath.c_str(), RTLD_NOW);
    }

    ~PosixLibrary() override
    {
        if (mModule)
        {
            dlclose(mModule);
        }
    }

    void *getSymbol(const char *symbolName) override
    {
        if (!mModule)
        {
            return nullptr;
        }

        return dlsym(mModule, symbolName);
    }

    void *getNative() const override { return mModule; }

  private:
    void *mModule = nullptr;
};

Library *OpenSharedLibrary(const char *libraryName, SearchType searchType)
{
    return new PosixLibrary(libraryName, searchType);
}

void CloseSharedLibrary(Library *library)
{
    if (library)
        delete library;
}

}  // namespace angle
