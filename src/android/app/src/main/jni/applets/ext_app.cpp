// Copyright 2020 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/string_util.h"
#include "jni/android_common/android_common.h"
#include "jni/applets/ext_app.h"
#include "jni/id_cache.h"

static jclass s_ext_app_class;
static jclass s_ext_app_config_class;
static jclass s_ext_app_data_class;
static jmethodID s_ext_app_execute;

namespace ExternalApp {

AndroidExternalApp::~AndroidExternalApp() = default;

void AndroidExternalApp::Setup(const Frontend::ExternalAppConfig& config) {
    JNIEnv* env = IDCache::GetEnvForThread();

    // Create the Java ExternalAppConfig object
    jobject java_config = env->AllocObject(s_ext_app_config_class);
    env->SetObjectField(java_config,
                        env->GetFieldID(s_ext_app_config_class, "title", "Ljava/lang/String;"),
                        ToJString(env, config.title));

    // Invoke backend Execute method
    jobject data =
        env->CallStaticObjectMethod(s_ext_app_class, s_ext_app_execute, java_config);

    const u32 return_code = static_cast<u32>(
        env->GetLongField(data, env->GetFieldID(s_ext_app_data_class, "returnCode", "J")));
    Finalize(return_code);
}

void InitJNI(JNIEnv* env) {
    s_ext_app_class = reinterpret_cast<jclass>(
        env->NewGlobalRef(env->FindClass("org/citra/citra_emu/applets/ExternalApp")));
    s_ext_app_config_class = reinterpret_cast<jclass>(env->NewGlobalRef(
        env->FindClass("org/citra/citra_emu/applets/ExternalApp$ExternalAppConfig")));
    s_ext_app_data_class = reinterpret_cast<jclass>(env->NewGlobalRef(
        env->FindClass("org/citra/citra_emu/applets/ExternalApp$ExternalAppData")));

    s_ext_app_execute =
        env->GetStaticMethodID(s_ext_app_class, "Execute",
                               "(Lorg/citra/citra_emu/applets/ExternalApp$ExternalAppConfig;)Lorg/"
                               "citra/citra_emu/applets/ExternalApp$ExternalAppData;");
}

void CleanupJNI(JNIEnv* env) {
    env->DeleteGlobalRef(s_ext_app_class);
    env->DeleteGlobalRef(s_ext_app_config_class);
    env->DeleteGlobalRef(s_ext_app_data_class);
}

} // namespace ExternalApp
