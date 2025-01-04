import org.jetbrains.kotlin.ir.backend.js.compile

plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
    `maven-publish`
}

android {
    namespace = "com.muye.hook.hiddenapi"
    compileSdk = 34

    defaultConfig {
        minSdk = 21

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        consumerProguardFiles("consumer-rules.pro")
        ndk {
            abiFilters += listOf("armeabi-v7a", "arm64-v8a")
        }
        externalNativeBuild {
            cmake {
                arguments("-DANDROID_STL=c++_shared")
                cppFlags("-std=c++17")
                abiFilters += listOf("armeabi-v7a", "arm64-v8a")
            }
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
    externalNativeBuild {
        cmake {
            path("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = "17"
    }
    buildFeatures {
        prefabPublishing = true
        prefab = true
    }
    prefab {
        create("hiddenapi")
        create("bypass_by_jni_on_load")
    }
    packagingOptions {
        excludes += listOf("**/libshadowhook.so", "**/libshadowhook_nothing.so")
    }
    ndkVersion = "23.2.8568313"
    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }
}

publishing {
    publications {
        register<MavenPublication>("release") {
            groupId = "com.muye"
            artifactId = "hiddenapi"
            version = "1.0.0"
            afterEvaluate {
                from(components["release"])
            }
        }

        repositories {
            maven {
                name = "localRepo"
                url = uri(layout.buildDirectory.dir("repo"))
            }
        }
    }
}


dependencies {
    implementation(libs.androidx.core.ktx)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)

    implementation(libs.shadowhook)
    implementation(libs.unsafereflect)
    compileOnly(project(":unsafe"))
}