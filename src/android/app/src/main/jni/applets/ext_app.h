// Copyright 2020 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <jni.h>
#include "core/frontend/applets/ext_app.h"

namespace ExternalApp {

class AndroidExternalApp final : public Frontend::ExternalApp {
public:
    ~AndroidExternalApp();

    void Setup(const Frontend::ExternalAppConfig& config) override;
};

// Should be called in JNI_Load
void InitJNI(JNIEnv* env);

// Should be called in JNI_Unload
void CleanupJNI(JNIEnv* env);

} // namespace ExternalApp
