// FastCVSample.cpp

//==============================================================================
// Include Files
//==============================================================================
#include <stdlib.h>
#include <android/log.h>
#include <fastcv/fastcv.h>
#include "FastCVSample.h"

//==============================================================================
// Declarations
//==============================================================================
#define LOG_TAG    "FastCVSample[cpp]"
#define DPRINTF(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

//==============================================================================
// Function Definitions
//==============================================================================

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void colorGrayToRGBA8888(const uint8_t* src, unsigned int width, unsigned int height, uint8_t* dst){

    if (src == NULL) {
        DPRINTF("colorGrayToRGBA8888() src is NULL.");
        return;
    }
    if (dst == NULL) {
        DPRINTF("colorGrayToRGBA8888() dst is NULL.");
        return;
    }

    // Gray scale -> RGBA8888
    // copy the same value to R, G and B channels.
    for (unsigned int i = 0; i < width*height; i++) {
        dst[4*i] = src[i];
        dst[4*i+1] = src[i];
        dst[4*i+2] = src[i];
        dst[4*i+3] = 0xFF;    // Alpha channel
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_theta360_fastcvsample_ImageProcessor_initFastCV
        (
                JNIEnv* env,
                jobject obj
        )
{
    char sVersion[32];

    fcvSetOperationMode( (fcvOperationMode) FASTCV_OP_PERFORMANCE );

    fcvGetVersion(sVersion, 32);
    DPRINTF( "Using FastCV version %s \n", sVersion );

    return;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_theta360_fastcvsample_ImageProcessor_cleanupFastCV
        (
                JNIEnv * env,
                jobject obj
        )
{
    fcvCleanUp();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
JNIEXPORT jbyteArray
JNICALL Java_com_theta360_fastcvsample_ImageProcessor_cannyFilter
        (
                JNIEnv* env,
                jobject obj,
                jbyteArray img,
                jint w,
                jint h
        )
{
    DPRINTF("cannyFilter()");

    /**
     * convert input data to jbyte object
     */
    jbyte* jimgData = NULL;
    jboolean isCopy = 0;
    jimgData = env->GetByteArrayElements( img, &isCopy);
    if (jimgData == NULL) {
        DPRINTF("jimgData is NULL");
        return NULL;
    }

    /**
     * process
     */
    // RGBA8888 -> RGB888
    void* rgb888 = fcvMemAlloc( w*h*4, 16);
    fcvColorRGBA8888ToRGB888u8((uint8_t*) jimgData, w, h, 0, (uint8_t*)rgb888, 0);

    // Convert to Gray scale
    void* gray = fcvMemAlloc( w*h, 16);
    fcvColorRGB888ToGrayu8((uint8_t*)rgb888, w, h, 0, (uint8_t*)gray, 0);

    // Canny filter
    void* canny = fcvMemAlloc(w*h, 16);
    // The value of lowThresh and highThresh are set appropriately.
    fcvFilterCanny3x3u8((uint8_t*)gray,w,h,(uint8_t*)canny, 10, 30);

    // Reverse the image color
    void* bitwise_not = fcvMemAlloc( w*h, 16);
    fcvBitwiseNotu8((uint8_t*)canny, w, h, 0, (uint8_t*)bitwise_not, 0);

    // Gray scale -> RGBA8888
    void* dst_rgba8888 = fcvMemAlloc( w*h*4, 16);
    colorGrayToRGBA8888((uint8_t*)bitwise_not, w, h, (uint8_t*)dst_rgba8888);

    /**
     * copy to destination jbyte object
     */
    jbyteArray dst = env->NewByteArray(w*h*4);
    if (dst == NULL){
        DPRINTF("dst is NULL");
        // release
        fcvMemFree(dst_rgba8888);
        fcvMemFree(bitwise_not);
        fcvMemFree(canny);
        fcvMemFree(gray);
        fcvMemFree(rgb888);
        env->ReleaseByteArrayElements(img, jimgData, 0);
        return NULL;
    }
    env->SetByteArrayRegion(dst,0,w*h*4,(jbyte*)dst_rgba8888);
    DPRINTF("copy");

    // release
    fcvMemFree(dst_rgba8888);
    fcvMemFree(bitwise_not);
    fcvMemFree(canny);
    fcvMemFree(gray);
    fcvMemFree(rgb888);
    env->ReleaseByteArrayElements(img,jimgData,0);

    DPRINTF("processImage end");
    return dst;
}
