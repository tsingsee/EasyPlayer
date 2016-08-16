package org.easydarwin.easyplayer.views;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

import org.esaydarwin.rtsp.player.R;


/**
 * TODO: document your custom view class.
 */
public class AngleView extends View {

    private int mArcColor = Color.WHITE;
    private int mStartAngle = 30;
    private int mSweepAngle = 150;
    private int mCurrentSweepAngle = 45;
    private float mArcWidth = 0f;
    private RectF mArcRec;
    private Paint mArcPaint;
    private Paint mCirclePaint;
    private int mAnchorRadius;

    public AngleView(Context context) {
        super(context);
        init(null, 0);
    }

    public AngleView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public AngleView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
    }

    private void init(AttributeSet attrs, int defStyle) {
        // Load attributes
        final TypedArray a = getContext().obtainStyledAttributes(attrs, R.styleable.AngleView, defStyle, 0);

        mArcColor = a.getColor(R.styleable.AngleView_arcColor, mArcColor);
        mStartAngle = a.getInteger(R.styleable.AngleView_startAngle, mStartAngle);
        mSweepAngle = a.getInteger(R.styleable.AngleView_sweepAngle, mSweepAngle);
        mCurrentSweepAngle = a.getInteger(R.styleable.AngleView_currentSweepAngle, mCurrentSweepAngle);

        mArcWidth = a.getDimensionPixelSize(R.styleable.AngleView_arcWidth, 1);
        mAnchorRadius = a.getDimensionPixelSize(R.styleable.AngleView_anchorRadio, (int) (10 * getResources().getDisplayMetrics().density * 4 + 0.5f));
        a.recycle();

        // Set up a default TextPaint object

        mArcPaint = new Paint();
        mArcPaint.setFlags(Paint.ANTI_ALIAS_FLAG);
        mArcPaint.setStyle(Paint.Style.STROKE);
        mArcPaint.setStrokeWidth(mArcWidth);
        mArcPaint.setColor(mArcColor);


        mCirclePaint = new Paint(mArcPaint);
        mCirclePaint.setStyle(Paint.Style.FILL);
    }

    public void setCurrentSweepAngle(int sweepAngle) {
        mCurrentSweepAngle = sweepAngle % mSweepAngle;
        invalidate();
    }

    /**
     * @param progress -50~50，-50表示最左边，50表示最右边，0表示中间
     */
    public void setCurrentProgress(int progress) {
        mCurrentSweepAngle = mSweepAngle / 2;

        mCurrentSweepAngle += mSweepAngle * progress / 100;
        invalidate();
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        int paddingLeft = getPaddingLeft();
        int paddingTop = getPaddingTop();
        int paddingRight = getPaddingRight();
        int paddingBottom = getPaddingBottom();

        float l = paddingLeft;
        float r = getWidth() - paddingRight;

        int h = getHeight() - paddingTop - paddingBottom;

        mArcRec = new RectF(l, paddingTop, r, r - l);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawArc(mArcRec, mStartAngle, mSweepAngle, false, mArcPaint);
//        canvas.drawRect(mArcRec, mArcPaint);
        final float r = mArcRec.width() * 0.5f; // 半径
//            double tan = Math.tan(mCurrentAngle);
//            float x = (float) Math.sqrt(r * r / (1 + tan * tan));//
//            float y = (float) (tan * x);
        float radians = (float) ((360 - (mStartAngle + mCurrentSweepAngle)) * Math.PI / 180);

        float x = (float) (Math.cos(radians) * r);
        float y = (float) (Math.sin(radians) * r);

        float x0 = mArcRec.left + r;
        x += x0;
        float y0 = mArcRec.top + r;
        y = y0 - y;

//        canvas.drawLine(x0, y0, x, y, mArcPaint);
//        canvas.drawCircle(x0, y0, 5, mCirclePaint);

        drawSide(canvas, r, x0, y0, true);
        drawSide(canvas, r, x0, y0, false);

        canvas.drawCircle(x, y, mAnchorRadius, mCirclePaint);
    }

    private void drawSide(Canvas canvas, float r, float x0, float y0, boolean leftSide) {
        float sideAngle = leftSide ? mStartAngle : mStartAngle + mSweepAngle;
        float radians = (float) ((360 - sideAngle) * Math.PI / 180);
        float x1_near = (float) (Math.cos(radians) * r * 0.8);
        x1_near += x0;
        float y1_near = (float) (Math.sin(radians) * r * 0.8);
        y1_near = y0 - y1_near;

        float x1_far = (float) (Math.cos(radians) * r * 1.2);
        x1_far += x0;
        float y1_far = (float) (Math.sin(radians) * r * 1.2);
        y1_far = y0 - y1_far;

        canvas.drawLine(x1_near, y1_near, x1_far, y1_far, mArcPaint);
    }


}
