#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;
float sigma;

void bitmaptoMat(JNIEnv * env,jobject bitmap, Mat& dst, jboolean needUnPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*            pixels = 0;


    try {

        CV_Assert( AndroidBitmap_getInfo(env, bitmap, &info) >= 0 );
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        CV_Assert( AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0 );
        CV_Assert( pixels );
        dst.create(info.height, info.width, CV_8UC4);
        if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
        {

            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if(needUnPremultiplyAlpha) cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
            else tmp.copyTo(dst);
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565

            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            cvtColor(tmp, dst, COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(env, bitmap);
        return;
    } catch(const cv::Exception& e) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);

        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
        return;
    }
}
void matToBitmap(JNIEnv * env, Mat src, jobject bitmap, jboolean needPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;


    try {
        CV_Assert( AndroidBitmap_getInfo(env, bitmap, &info) >= 0 );
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        CV_Assert( src.dims == 2 && info.height == (uint32_t)src.rows && info.width == (uint32_t)src.cols );
        CV_Assert( src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4 );
        CV_Assert( AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0 );
        CV_Assert( pixels );
        if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
        {
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if(src.type() == CV_8UC1)
            {

                cvtColor(src, tmp, COLOR_GRAY2RGBA);
            } else if(src.type() == CV_8UC3){

                cvtColor(src, tmp, COLOR_RGB2RGBA);
            } else if(src.type() == CV_8UC4){

                if(needPremultiplyAlpha) cvtColor(src, tmp, COLOR_RGBA2mRGBA);
                else src.copyTo(tmp);
            }
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            if(src.type() == CV_8UC1)
            {

                cvtColor(src, tmp, COLOR_GRAY2BGR565);
            } else if(src.type() == CV_8UC3){

                cvtColor(src, tmp, COLOR_RGB2BGR565);
            } else if(src.type() == CV_8UC4){

                cvtColor(src, tmp, COLOR_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, bitmap);
        return;
    } catch(const cv::Exception& e) {
        AndroidBitmap_unlockPixels(env, bitmap);

        jclass je = env->FindClass("java/lang/Exception");

        env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);

        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return;
    }
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_finalproject_SubActivity_myFlip(JNIEnv *env, jobject thiz, jobject bitmapIn
) {

    Mat src;
    bitmaptoMat(env,bitmapIn,src, false);
    flip(src,src,0);
    matToBitmap(env,src,bitmapIn,false);

}
extern "C" JNIEXPORT void JNICALL
Java_com_example_finalproject_SubActivity_myBlur(JNIEnv *env, jobject thiz, jobject bitmapIn, jfloat sigma
) {

    Mat src;
    bitmaptoMat(env,bitmapIn,src, false);
    GaussianBlur(src,src,Size(),sigma);
    matToBitmap(env,src,bitmapIn,false);


}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_finalproject_SubActivity_myEdge(JNIEnv *env, jobject thiz, jobject bitmapIn) {
    // TODO: implement myShow()
    Mat img_color;
    bitmaptoMat(env,bitmapIn,img_color,false);
    if(img_color.empty()){
       cout<<"no image"<<endl;
    }
    Mat img_gray;
    cvtColor(img_color,img_gray,COLOR_BGR2GRAY);
    Mat img_canny;
    Canny(img_gray,img_canny,90,180);
    matToBitmap(env,img_canny,bitmapIn,false);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_finalproject_SubActivity_myBinary(JNIEnv *env, jobject thiz, jobject bitmapIn) {
    // TODO: implement myBinary()
    Mat img_color;
    bitmaptoMat(env,bitmapIn,img_color,false);
    if(img_color.empty()){
        cout<<"no image"<<endl;
    }
    Mat img_gray;
    cvtColor(img_color,img_gray,COLOR_BGR2GRAY);
    Mat img_binary;
    adaptiveThreshold(img_gray,img_binary,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,4);
    matToBitmap(env,img_binary,bitmapIn, false);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_finalproject_SubActivity_myGray(JNIEnv *env, jobject thiz, jobject bitmapIn) {
    // TODO: implement myGray()
    Mat img_color;
    bitmaptoMat(env,bitmapIn,img_color,false);
    Mat img_gray;
    cvtColor(img_color,img_gray,COLOR_BGR2GRAY);
    matToBitmap(env,img_gray,bitmapIn,false);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_finalproject_SubActivity_myCon(JNIEnv *env, jobject thiz, jobject bitmap,jobject bitmapIn) {
    // TODO: implement myCon()
    Mat img_logo,img_background;
    bitmaptoMat(env,bitmap,img_logo,false);
    bitmaptoMat(env,bitmapIn,img_background,false);
    Mat img_gray;
    cvtColor(img_logo,img_gray,COLOR_BGR2GRAY);
    Mat img_mask;
    threshold(img_gray,img_mask,200,255,THRESH_BINARY);
    Mat img_mask_inv;
    bitwise_not(img_mask,img_mask_inv);
    int height = img_logo.rows;
    int width = img_logo.cols;
    Mat img_roi(img_background,Rect(0,0,width,height));
    Mat img1,img2;
    bitwise_and(img_logo,img_logo,img1,img_mask_inv);
    bitwise_and(img_roi,img_roi,img2,img_mask);
    Mat dst;
    add(img1,img2,dst);
    dst.copyTo(img_background(Rect(0,0,width,height)));
    matToBitmap(env,img_background,bitmapIn, false);
}
