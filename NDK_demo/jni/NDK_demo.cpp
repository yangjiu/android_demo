#include <string.h>
#include <jni.h>





extern "C" {
JNIEXPORT jstring JNICALL Java_com_example_ndkdemo_MainActivity_stringFromJNI(JNIEnv *env,
        jobject thiz);
}

JNIEXPORT jstring JNICALL Java_com_example_ndkdemo_MainActivity_stringFromJNI(JNIEnv *env,
        jobject thiz) {
    return env->NewStringUTF("Hello jni");
}
