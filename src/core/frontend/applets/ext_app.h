// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <string>
#include "core/hle/applets/ext_app.h"

namespace Frontend {

/// Configuration that's relevant to frontend implementation of applet. Anything missing that we
/// later learn is needed can be added here and filled in by the backend HLE applet
struct ExternalAppConfig {
    std::string title;
};

struct ExternalAppData {
    u32 return_code;
};

class ExternalApp {
public:
    virtual ~ExternalApp() = default;
    virtual void Setup(const ExternalAppConfig& config_) {
        config = ExternalAppConfig(config_);
    }

    const ExternalAppData& ReceiveData() const {
        return data;
    }

    /**
     * Stores the data so that the HLE applet in core can
     * send this to the calling application
     */
    void Finalize(u32 return_code);

protected:
    ExternalAppConfig config;
    ExternalAppData data;
};

std::vector<u32> LoadApps();

class DefaultExternalApp final : public ExternalApp {
public:
    void Setup(const ExternalAppConfig& config) override;
};

} // namespace Frontend
