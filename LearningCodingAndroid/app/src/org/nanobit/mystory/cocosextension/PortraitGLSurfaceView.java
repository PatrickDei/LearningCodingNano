package org.nanobit.mystory.cocosextension;

import android.content.Context;
import android.util.AttributeSet;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

/**
 * Created by postenjakm on 07/09/16.
 */
public class PortraitGLSurfaceView extends Cocos2dxGLSurfaceView {


    public PortraitGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public PortraitGLSurfaceView(Context context) {
        super(context);
    }

    @Override
    protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
        if(!this.isInEditMode()) {
            if ( pNewSurfaceHeight < pNewSurfaceWidth ) {
            	this.getCocos2dxRenderer().setScreenWidthAndHeight(pNewSurfaceHeight,pNewSurfaceWidth);
            } else {
                this.getCocos2dxRenderer().setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
            }
        }
    }
}
