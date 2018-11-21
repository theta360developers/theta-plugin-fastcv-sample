// FastCVSample.h
#ifndef THETA_PLUGIN_FASTCV_SAMPLE_FASTCVSAMPLE_H
#define THETA_PLUGIN_FASTCV_SAMPLE_FASTCVSAMPLE_H

//==============================================================================
// Include Files
//==============================================================================
#include <jni.h>

//==============================================================================
// Declarations
//==============================================================================
void colorGrayToRGBA8888(const uint8_t* src, unsigned int width, unsigned int height, uint8_t* dst);

//==============================================================================
// Declarations
//==============================================================================
extern "C" {

   //---------------------------------------------------------------------------
   //   Initializes
   //---------------------------------------------------------------------------
    JNIEXPORT void JNICALL Java_com_theta360_fastcvsample_ImageProcessor_initFastCV
            (
                    JNIEnv* env,
                    jobject obj
            );

    //---------------------------------------------------------------------------
    //  Finalize
    //---------------------------------------------------------------------------
    JNIEXPORT void JNICALL Java_com_theta360_fastcvsample_ImageProcessor_cleanupFastCV
            (
                   JNIEnv * env,
                   jobject obj
            );

    //---------------------------------------------------------------------------
    //  Image Processing; Canny Filtering
    //---------------------------------------------------------------------------
    JNIEXPORT jbyteArray JNICALL Java_com_theta360_fastcvsample_ImageProcessor_cannyFilter
            (
                    JNIEnv * env,
                    jobject obj,
                    jbyteArray img,
                    jint width,
                    jint height
            );
};

#endif //THETA_PLUGIN_FASTCV_SAMPLE_FASTCVSAMPLE_H
