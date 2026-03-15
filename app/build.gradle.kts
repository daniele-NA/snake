@file:Suppress(
    "UseTomlInstead",
    "KaptUsageInsteadOfKsp",
    "UnstableApiUsage",
    "NewerVersionAvailable",
    "DataBindingWithoutKapt"
)
plugins {
    id("com.android.application")
}

android {
    namespace = "me.app.cherry"
    ndkVersion = "29.0.14206865"

    compileSdk {
        version = release(36) {
            minorApiLevel = 1
        }
    }

    defaultConfig {
        applicationId = "me.app.cherry"
        minSdk = 30
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        ndk {
            abiFilters.clear()
            abiFilters += listOf("arm64-v8a", "armeabi-v7a")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    buildFeatures {
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/native/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.18.0")
    implementation("androidx.appcompat:appcompat:1.7.1")
    implementation("com.google.android.material:material:1.13.0")
    implementation("androidx.games:games-activity:4.0.0")
    implementation("androidx.core:core-splashscreen:1.0.1")
}
