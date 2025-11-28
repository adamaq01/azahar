// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/file_util.h"
#include "common/string_util.h"
#include "core/file_sys/archive_extsavedata.h"
#include "core/file_sys/file_backend.h"
#include "core/frontend/applets/ext_app.h"
#include "core/hle/service/ptm/ptm.h"

namespace Frontend {

void ExternalApp::Finalize(u32 return_code) {
    data = {return_code};
}

std::vector<u32> LoadApps() {
    std::vector<u32> apps;
    return apps;
}

void DefaultExternalApp::Setup(const Frontend::ExternalAppConfig& config) {
    ExternalApp::Setup(config);
    LOG_INFO(Frontend,
              "DefaultExternalApp::Setup called with title: {}",
              config.title);
    printf("DefaultExternalApp::Setup called with title: %s\n", config.title.c_str());
    Finalize(system(config.title.c_str()));
}

} // namespace Frontend
