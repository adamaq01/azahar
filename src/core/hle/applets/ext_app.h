// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/applets/applet.h"
#include "core/hle/kernel/shared_memory.h"
#include "core/hle/result.h"

namespace Frontend {
class ExternalApp;
} // namespace Frontend

namespace HLE::Applets {

class ExternalApp final : public Applet {
public:
    struct ExternalAppParam {
        static constexpr u32 MESSAGE_SIZE = 256;
        const char message[MESSAGE_SIZE];
    };

    explicit ExternalApp(Core::System& system, Service::APT::AppletId id, Service::APT::AppletId parent,
                     bool preload, std::weak_ptr<Service::APT::AppletManager> manager)
        : Applet(system, id, parent, preload, std::move(manager)) {}

    Result ReceiveParameterImpl(const Service::APT::MessageParameter& parameter) override;
    Result Start(const Service::APT::MessageParameter& parameter) override;
    Result Finalize() override;
    void Update() override;

private:
    /// This SharedMemory will be created when we receive the LibAppJustStarted message.
    /// It holds the framebuffer info retrieved by the application with
    /// GSPGPU::ImportDisplayCaptureInfo
    std::shared_ptr<Kernel::SharedMemory> framebuffer_memory;

    /// Parameter received by the applet on start.
    ExternalAppParam param{};

    std::shared_ptr<Frontend::ExternalApp> frontend_applet;
};

} // namespace HLE::Applets
