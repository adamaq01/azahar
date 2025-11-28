// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/string_util.h"
#include "core/core.h"
#include "core/frontend/applets/ext_app.h"
#include "core/hle/applets/ext_app.h"
#include "core/hle/service/apt/apt.h"

namespace HLE::Applets {

Result ExternalApp::ReceiveParameterImpl(const Service::APT::MessageParameter& parameter) {
    if (parameter.signal != Service::APT::SignalType::Request) {
        LOG_ERROR(Service_APT, "unsupported signal {}", parameter.signal);
        UNIMPLEMENTED();
        // TODO(Subv): Find the right error code
        return ResultUnknown;
    }

    // The LibAppJustStarted message contains a buffer with the size of the framebuffer shared
    // memory.
    // Create the SharedMemory that will hold the framebuffer data
    Service::APT::CaptureBufferInfo capture_info;
    ASSERT(sizeof(capture_info) == parameter.buffer.size());

    std::memcpy(&capture_info, parameter.buffer.data(), sizeof(capture_info));

    // TODO: allocated memory never released
    using Kernel::MemoryPermission;
    // Create a SharedMemory that directly points to this heap block.
    framebuffer_memory = system.Kernel().CreateSharedMemoryForApplet(
        0, capture_info.size, MemoryPermission::ReadWrite, MemoryPermission::ReadWrite,
        "ExternalApp Memory");

    // Send the response message with the newly created SharedMemory
    SendParameter({
        .sender_id = id,
        .destination_id = parent,
        .signal = Service::APT::SignalType::Response,
        .object = framebuffer_memory,
    });

    return ResultSuccess;
}

Result ExternalApp::Start(const Service::APT::MessageParameter& parameter) {
    std::memcpy(&param, parameter.buffer.data(), parameter.buffer.size());

    using namespace Frontend;
    frontend_applet = system.GetExternalApp();
    ASSERT(frontend_applet);

    // Convert program from char8_t[] to std::string
    std::string str(param.message);
    ExternalAppConfig frontend_config = {
        .title = str,
    };
    frontend_applet->Setup(frontend_config);

    // printf("ExternalApp started %s\n", param.message);
    return ResultSuccess;
}

Result ExternalApp::Finalize() {
    std::vector<u8> buffer(0);
    //std::vector<u8> buffer(sizeof(param));
    //memcpy(buffer.data(), &param, buffer.size());
    CloseApplet(nullptr, buffer);
    // printf("ExternalApp finalized\n");
    return ResultSuccess;
}

void ExternalApp::Update() {
    using namespace Frontend;
    const ExternalAppData& data = frontend_applet->ReceiveData();

    printf("ExternalApp update, return code: %d\n", data.return_code);

    // Store the return code to send it back to the application
    // when finalizing the applet.
    // For now we just return 0
    // TODO(Subv): Find a way to get this return code from the frontend applet
    // and set it here.
    // For now we just return 0
    // result.return_code = data.return_code;

    // Finalize the applet immediately
    Finalize();
}

} // namespace HLE::Applets
