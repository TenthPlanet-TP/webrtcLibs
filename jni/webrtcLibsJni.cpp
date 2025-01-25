#include <jni.h>
// #include <JNIHelp.h>

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <unordered_map>

#include <inttypes.h>

#include "common_video/h264/h264_bitstream_parser.h"
#include "common_video/h265/h265_bitstream_parser.h"
#include "api/video_codecs/bitstream_parser.h"


#include <android/log.h>

#ifdef LOG_TAG
    #undef LOG_TAG
#endif
#define LOG_TAG "webrtcLibsJni"

#define PRINT_MOD_A(level, format, arg...) do {\
    __android_log_print(level, LOG_TAG, "[%s,%d] " format, __FUNCTION__,__LINE__, ##arg);\
} while(0)

#define LOG_V(...) PRINT_MOD_A(ANDROID_LOG_VERBOSE, __VA_ARGS__)
#define LOG_D(...) PRINT_MOD_A(ANDROID_LOG_DEBUG, __VA_ARGS__)
#define LOG_I(...) PRINT_MOD_A(ANDROID_LOG_INFO,  __VA_ARGS__)
#define LOG_W(...) PRINT_MOD_A(ANDROID_LOG_WARN,  __VA_ARGS__)
#define LOG_E(...) PRINT_MOD_A(ANDROID_LOG_ERROR, __VA_ARGS__)
#define LOG_F(...) PRINT_MOD_A(ANDROID_LOG_FATAL, __VA_ARGS__)

#define ALOGV(...) PRINT_MOD_A(ANDROID_LOG_VERBOSE, __VA_ARGS__)
#define ALOGD(...) PRINT_MOD_A(ANDROID_LOG_DEBUG, __VA_ARGS__)
#define ALOGI(...) PRINT_MOD_A(ANDROID_LOG_INFO,  __VA_ARGS__)
#define ALOGW(...) PRINT_MOD_A(ANDROID_LOG_WARN,  __VA_ARGS__)
#define ALOGE(...) PRINT_MOD_A(ANDROID_LOG_ERROR, __VA_ARGS__)
#define ALOGF(...) PRINT_MOD_A(ANDROID_LOG_FATAL, __VA_ARGS__)


using namespace webrtc;


enum VideoCodecType {
  // There are various memset(..., 0, ...) calls in the code that rely on
  // kVideoCodecGeneric being zero.
  kVideoCodecGeneric = 0,
  kVideoCodecVP8,
  kVideoCodecVP9,
  kVideoCodecAV1,
  kVideoCodecH264,
  kVideoCodecMultiplex,
  kVideoCodecH265,
};

static std::unordered_map<jlong, std::shared_ptr<BitstreamParser>> g_handler_map;


static jlong nativeCreateBitstreamParser(JNIEnv* env, jobject obj, jint type)
{
    int ret = 0;
    std::shared_ptr<BitstreamParser> parser = nullptr;

    switch (type)
    {
    case kVideoCodecH264:
        parser = std::make_shared<H264BitstreamParser>();
        break;
    case kVideoCodecH265:
        parser = std::make_shared<H265BitstreamParser>();
        break;
    default:
        ALOGE("error, unsupport type=%d", type);
        break;
    }

#if 0
    jlong handle = static_cast<jlong>(reinterpret_cast<std::uintptr_t>(parser));
    // jlong handle = reinterpret_cast<jlong>(parser);
    if (handle < 0) {
        ALOGE("error, parser: %p, handle: %" PRId64 "", parser.get(), handle);
        // assert(handle > 0);
    } else {
        ALOGD("parser: %p, handle: %" PRId64 "", parser.get(), handle);
    }
#else
    jlong handle = (jlong)type;
    g_handler_map.insert(std::make_pair(handle, parser));
    ALOGI("g_handler_map size: %zu, handle: %" PRId64 ", parser: %p", g_handler_map.size(), handle, parser.get());
#endif
    return handle;
}

static jint nativeReleaseBitstreamParser(JNIEnv* env, jobject obj, jlong handle)
{
    jint ret = -1;

    if (handle > 0) {
#if 0
        std::shared_ptr<BitstreamParser> parser = reinterpret_cast<BitstreamParser *>(handle);
#else
        std::shared_ptr<BitstreamParser> parser = nullptr;
        auto iter_handler = g_handler_map.find(handle);
        if (iter_handler != g_handler_map.end()) {
            parser = iter_handler->second;
            ALOGD("erase g_handler_map, size: %zu", g_handler_map.size());
            g_handler_map.erase(handle);
        }
#endif
        if (parser == nullptr) {
            ALOGE("error, BitstreamParser is null");
            return -1;
        }

        parser = nullptr;
        ret = 0;
    }

    return ret;
}


jint nativeParseBitstream(JNIEnv* env, jobject obj,
        jlong handle, jobject byteBuffer)
{
    int ret = -1;

    // 获取 ByteBuffer 类
    jclass bufferClass = env->GetObjectClass(byteBuffer);

    // 获取 position 方法的 ID
    jmethodID positionMethodID = env->GetMethodID(bufferClass, "position", "()I");
    if (positionMethodID == NULL) {
        printf("Failed to get position method ID\n");
        return -1;
    }

    // 获取 limit 方法的 ID
    jmethodID limitMethodID = env->GetMethodID(bufferClass, "limit", "()I");
    if (limitMethodID == NULL) {
        printf("Failed to get limit method ID\n");
        return -1;
    }

    // 调用 position 方法获取 position 值
    jint position = env->CallIntMethod(byteBuffer, positionMethodID);

    // 调用 limit 方法获取 limit 值
    jint limit = env->CallIntMethod(byteBuffer, limitMethodID);

    // 打印 position 和 limit 的值
    ALOGD("Position: %d, Limit: %d\n", position, limit);

    size_t byteBufferLength = limit - position;

    // 获取 ByteBuffer 的内存地址
    void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
    if (bufferAddress == NULL) {
        ALOGE("Failed to get buffer address\n");
        return -1;
    }

    if (handle > 0) {
#if 0
        std::shared_ptr<BitstreamParser> parser = reinterpret_cast<BitstreamParser *>(handle);
#else
        std::shared_ptr<BitstreamParser> parser = nullptr;
        auto iter_handler = g_handler_map.find(handle);
        if (iter_handler != g_handler_map.end()) {
            parser = iter_handler->second;
        }
#endif
        if (parser == nullptr) {
            ALOGE("error, BitstreamParser is null");
            return -1;
        }
        parser->ParseBitstream(rtc::ArrayView<const uint8_t>((const uint8_t*)bufferAddress, byteBufferLength));
        // printf("ret=%d\n", h264_parser.GetLastSliceQp().has_value());
        ret = 0;
    }

    return ret;
}

jint nativeGetLastSliceQp(JNIEnv* env, jobject obj, jlong handle)
{
    if (handle > 0) {
#if 0
        std::shared_ptr<BitstreamParser> parser = reinterpret_cast<BitstreamParser *>(handle);
#else
        std::shared_ptr<BitstreamParser> parser = nullptr;
        auto iter_handler = g_handler_map.find(handle);
        if (iter_handler != g_handler_map.end()) {
            parser = iter_handler->second;
        }
#endif
        if (parser == nullptr) {
            ALOGE("error, BitstreamParser is null");
            return -1;
        }
        absl::optional<int> qp = parser->GetLastSliceQp();
        if (qp.has_value()) {
            return *qp;
        } else {
            return -1;
        }
    }
    return -1;
}

static const JNINativeMethod methods[] = {
	{"nativeCreateBitstreamParser", "(I)J",   (void *)nativeCreateBitstreamParser},
	{"nativeReleaseBitstreamParser", "(J)I",   (void *)nativeReleaseBitstreamParser},
	{"nativeParseBitstream",  "(JLjava/nio/ByteBuffer;)I",   (void *)nativeParseBitstream},
	{"nativeGetLastSliceQp",  "(J)I",   (void *)nativeGetLastSliceQp},
};

/* System.loadLibrary */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env;
	jclass cls;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

	cls = env->FindClass("com/webrtc/utils");
	if (cls == nullptr) {
		return JNI_ERR;
	}

	if (env->RegisterNatives(cls, methods, sizeof(methods)/sizeof(methods[0])) < 0) {
		return JNI_ERR;
    }

	return JNI_VERSION_1_6;
}

#if 1
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    // __android_log_print(ANDROID_LOG_INFO, "native", "JNI_OnUnload");
    ALOGI("JNI_OnUnload start\n");

	JNIEnv *env;
	jclass cls;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        return ;
    }

	cls = env->FindClass("com/webrtc/utils");
	if (cls == nullptr) {
		return ;
	}

	if (env->UnregisterNatives(cls) < 0) {
		return ;
    }
    
    ALOGI("JNI_OnUnload end\n");
}
#endif

