// Copyright 2023 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

package org.citra.citra_emu.applets

import androidx.annotation.Keep
import org.citra.citra_emu.NativeLibrary
import java.io.Serializable

@Keep
object ExternalApp {
    lateinit var data: ExternalAppData
    val finishLock = Object()

    private fun ExecuteImpl(config: ExternalAppConfig) {
        val emulationActivity = NativeLibrary.sEmulationActivity.get()
        data = ExternalAppData(0)

        val launchIntent = emulationActivity.getPackageManager().getLaunchIntentForPackage("com.google.android.youtube");
        if (launchIntent != null) {
            emulationActivity.startActivity(launchIntent);
        }
    }

    @JvmStatic
    fun Execute(config: ExternalAppConfig): ExternalAppData {
        NativeLibrary.sEmulationActivity.get()!!.runOnUiThread { ExecuteImpl(config) }
        synchronized(finishLock) {
            try {
                finishLock.wait()
            } catch (ignored: Exception) {
            }
        }
        return data
    }

    @Keep
    class ExternalAppConfig : Serializable {
        var title: String? = null
    }

    class ExternalAppData (var returnCode: Long)
}
