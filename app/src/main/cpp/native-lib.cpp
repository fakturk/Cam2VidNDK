#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <time.h>
#include <android/looper.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "accNative", __VA_ARGS__))

ASensorEventQueue* sensorEventQueue;
static int get_sensor_events(int fd, int events, void* data);
bool sensorPrint=true;

extern "C"
jstring
Java_netlab_fakturk_accndksdkcompare_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
void Java_netlab_fakturk_cam2vidndk_Camera2VideoFragment_sensorValue(JNIEnv* env, jobject thiz)
{
    ASensorEvent event;
    int events, ident;
    ASensorManager* sensorManager;
    const ASensor* accSensor;
    void* sensor_data = malloc(1000);

// Identify the looper associated with the calling thread, or create one if it does not exist.
// A looper is a message loop for a thread and will handle the sensor event callbacks.

    ALooper* looper = ALooper_forThread();

    if(looper == NULL)
    {
        looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    }

    sensorManager = ASensorManager_getInstance();

    accSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);

    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 3, get_sensor_events, sensor_data);
    ASensorEventQueue_enableSensor(sensorEventQueue, accSensor);
    int a = ASensor_getMinDelay(accSensor);
    LOGI("min-delay: %d",a);
    ASensorEventQueue_setEventRate(sensorEventQueue, accSensor, 10000);

}
extern "C"
void Java_netlab_fakturk_cam2vidndk_Camera2VideoFragment_stopSensorPrint(JNIEnv* env, jobject thiz)
{
    sensorPrint=false;
}
extern "C"
void Java_netlab_fakturk_cam2vidndk_Camera2VideoFragment_startSensorPrint(JNIEnv* env, jobject thiz)
{
    sensorPrint=true;
}


static int get_sensor_events(int fd, int events, void* data)
{
    ASensorEvent event;
    while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
        if (event.type == ASENSOR_TYPE_ACCELEROMETER) {
            if (sensorPrint)
            {
                LOGI(" %lld %f %f %f ", event.timestamp, event.acceleration.x, event.acceleration.y,
                     event.acceleration.z);
            }

        }
    }

        //should return 1 to continue receiving callbacks, or 0 to unregister

        return 1;
}
