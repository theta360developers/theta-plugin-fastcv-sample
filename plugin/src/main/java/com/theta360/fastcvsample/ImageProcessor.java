package com.theta360.fastcvsample;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

public class ImageProcessor {
    private static final String TAG = "FastCVSample";

    static {
        // Load JNI Library
        System.loadLibrary("fastcvsample");
    }

    public void init() {
        // Initialize FastCV
        Log.d(TAG, "Initialize FastCV");
        initFastCV();
    }

    public void cleanup() {
        // Cleanup FastCV
        Log.d(TAG, "Cleanup FastCV");
        cleanupFastCV();
    }

    public byte[] process(byte[] data) {
        Log.d(TAG, "ImageProcess");

        /**
         * Pre-process
         */
        // Convert JPEG format to Bitmap class.
        // To get byte[], copy the content of Bitmap to ByteBuffer.
        Bitmap bmp = BitmapFactory.decodeByteArray(data, 0, data.length);
        ByteBuffer byteBuffer = ByteBuffer.allocate(bmp.getByteCount());
        bmp.copyPixelsToBuffer(byteBuffer);

        int width = bmp.getWidth();
        int height = bmp.getHeight();


        /**
         * Image Processing with FastCV（native code）
         */
        // get byte[] from byteBuffer and input to the function.
        byte[] dstBmpArr = cannyFilter(byteBuffer.array(), width, height);
        if (dstBmpArr == null) {
            Log.e(TAG, "Failed to cannyFilter(). dstBmpArr is null.");
            return null;
        }


        /**
         * Post-process
         */
        // copy byte[] to the content of Bitmap class.
        Bitmap dstBmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        dstBmp.copyPixelsFromBuffer(ByteBuffer.wrap(dstBmpArr));

        // compress Bitmap class to JPEG format.
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        dstBmp.compress(Bitmap.CompressFormat.JPEG,100, baos);
        byte[] dst = baos.toByteArray();

        return dst;
    }

    // Native Functions
    private native void initFastCV();
    private native void cleanupFastCV();
    private native byte[] cannyFilter(byte[] data, int width, int height);
}
