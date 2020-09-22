package org.nanobit.mystory;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
//import android.util.Log;
import android.view.ContextThemeWrapper;


public class NanoAlertDialog {

	static  AlertDialog dialog = null;

    void createDialog(final String title,final String message,final String cancelButtonText,final String okButtonText, final String neutralButtonText , final int objectID)
	{					
//		Builder builder = new Builder(Cocos2dxActivity.getContext());
		Builder builder = new Builder(new ContextThemeWrapper(Cocos2dxActivity.getContext(),android.R.style.Theme_DeviceDefault_Dialog));
		builder.setMessage(message);
		builder.setTitle(title);
				
	    builder.setNegativeButton(cancelButtonText, new DialogInterface.OnClickListener() {
	    public void onClick(DialogInterface dialog, int id) {
			  Cocos2dxActivity myActivity = (Cocos2dxActivity)Cocos2dxActivity.getContext();
			  myActivity.runOnGLThread(new Runnable() {
			        		   
			        @Override
			        public void run()
			        {
			         NanoAlertDialog.buttonClicked(1, objectID);
			        }
			  });
	          }
	    });
		
	    if (!okButtonText.isEmpty())
	    {
			builder.setPositiveButton(okButtonText, new DialogInterface.OnClickListener() {		
				public void onClick(DialogInterface dialog, int id) 
				{
				   Cocos2dxActivity myActivity = (Cocos2dxActivity)Cocos2dxActivity.getContext();
		      	   myActivity.runOnGLThread(new Runnable() {
		      		   
		      		   @Override
		      		   public void run()
		      		   {
		      			  NanoAlertDialog.buttonClicked(0, objectID);
		      		   }
		      	   });
		         }        	  
			    });	
	    }

		if (!neutralButtonText.isEmpty())
		{
			builder.setNeutralButton(neutralButtonText, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int id)
				{
					Cocos2dxActivity myActivity = (Cocos2dxActivity)Cocos2dxActivity.getContext();
					myActivity.runOnGLThread(new Runnable() {

						@Override
						public void run()
						{
							NanoAlertDialog.buttonClicked(2, objectID);
						}
					});
				}
			});
		}


		try
	    {
			dialog = builder.create();
			dialog.setCancelable(false);
			dialog.setCanceledOnTouchOutside(false);
			dialog.show();
	    }
	    catch(Exception e)
	    {
	    	//Log//Log.e("Alert dialog exception", "e:",e);
	    }
	    
	}

	public static void cancelAlert(){if(dialog != null){
			dialog.dismiss();
		}
	}
        
	public static native void buttonClicked(final int buttonNo, final int objectID);
}
