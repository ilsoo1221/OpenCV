package com.example.finalproject;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import java.io.ByteArrayOutputStream;

public class SubActivity extends AppCompatActivity {
    private static final String TAG = "CamTestActivity";
    static {
        System.loadLibrary("native-lib");
    }
    static{
        if(!OpenCVLoader.initDebug()){
            Log.d(TAG,"OpenCV is not loaded!");
        }
        else{
            Log.d(TAG,"OpenCV is loaded");
        }
    }

    public native void myBlur(Bitmap bitmapIn, float sigma);
    public native void myCon(Bitmap bitmap,Bitmap bitmapIn);
  

    public native void myFlip(Bitmap bitmapIn);

    public native void myEdge(Bitmap bitmapIn);

    public native void myBinary(Bitmap bitmapIn);
    public native void myGray(Bitmap bitmapIn);

    public ImageView capturedImageHolder;

    public Bitmap bitmapIn;





    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.subactivity_main);
        capturedImageHolder = findViewById(R.id.captured_image);
        Bundle extras = getIntent().getExtras();
        byte[] byteArray = getIntent().getByteArrayExtra("image");
        bitmapIn = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.length);
        capturedImageHolder.setImageBitmap(bitmapIn);
        Button btn_gray = (Button) findViewById(R.id.button_gray);
        Button btn_con = (Button)findViewById(R.id.button_con);
        Button btn_edge = (Button) findViewById(R.id.button_edge);
        Button btn_flip = (Button) findViewById(R.id.button_flip);
        SeekBar seekBar = (SeekBar) findViewById(R.id.sldSigma);

        btn_gray.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                myGray(bitmapIn);
                capturedImageHolder.setImageBitmap(bitmapIn);

            }

//
        });
        btn_edge.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                myBinary(bitmapIn);
                capturedImageHolder.setImageBitmap(bitmapIn);
            }
        });


        btn_flip.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                myFlip(bitmapIn);
                capturedImageHolder.setImageBitmap(bitmapIn);
            }
        });
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                float radius = (float) seekBar.getProgress()/10;
                myBlur(bitmapIn, radius);
                capturedImageHolder.setImageBitmap(bitmapIn);
            }
        });
       btn_con.setOnClickListener(new View.OnClickListener() {
           @Override
           public void onClick(View v) {
              Bitmap bMap = BitmapFactory.decodeResource(getResources(),R.drawable.car);
              myCon(bMap,bitmapIn);
              capturedImageHolder.setImageBitmap(bitmapIn);

           }
       });


    }
}


