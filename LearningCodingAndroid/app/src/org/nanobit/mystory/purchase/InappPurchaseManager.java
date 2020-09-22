package org.nanobit.mystory.purchase;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.nanobit.mystory.StoryGame;

import android.content.Intent;
import android.os.Handler;


public class InappPurchaseManager {
	
	static NAPurchaseManager sNApurchaseManager;
    // (arbitrary) request code for the purchase flow
    
    public static void cleanup() {
        // very important:
		sNApurchaseManager.cleanup();
    }
    
    public static Handler getHandler(){
    	return StoryGame.getHandler();
    }
    
    public static boolean getIsDebug(){
    	return sNApurchaseManager.mDebug;
    }
    
    public static boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
    	return sNApurchaseManager.handleActivityResult(requestCode, resultCode, data);
    }
        
    public static void setupInappPurchaseManager( Cocos2dxActivity mainActivity, String gameName, String udid, boolean debug, String logTag ){
    	sNApurchaseManager = new NAPurchaseManager(mainActivity, gameName, udid, debug, logTag );
    }
    
    public static void sQuerryInventoryAndSKUs( String productIds, String username ){
    	sNApurchaseManager.querryInventoryAndSKUs(productIds, username);
    }
    
    public static void sInitiatePurchase( String productId, String username, boolean isSub ){
    	sNApurchaseManager.initiatePurchase(productId, username, isSub);
    }
   
    public static native void purchaseSetupFailed(int status, String message);
    public static native void purchaseCompleted(int status, String productId, String message, String transactionId);
    public static native void purchaseFailed(int status, String productId, String message);
    public static native void skuDataRecieved( String jsonData );
    public static native void skuQuerryFailed( int status, String message );
    public static native double productRevenue( String productId ); //subscriptions only!

    public static native void subscriptionReceived(int status, String productId, String message, String transactionId, long purchaseTimestamp, boolean shouldReport);


}

