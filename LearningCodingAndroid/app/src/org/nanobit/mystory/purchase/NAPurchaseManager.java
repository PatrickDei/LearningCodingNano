package org.nanobit.mystory.purchase;

import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.nanobit.mystory.StoryGame;
import org.nanobit.mystory.purchase.util.IabHelper;
import org.nanobit.mystory.purchase.util.IabResult;
import org.nanobit.mystory.purchase.util.Inventory;
import org.nanobit.mystory.purchase.util.Purchase;
import org.nanobit.mystory.purchase.util.SkuDetails;
import org.nanobit.mystory.purchase.verification.VerificationBaseReponse;
import org.nanobit.mystory.purchase.verification.VerificationManager;

import android.content.Intent;
//import android.util.Log;
import android.util.Log;

import static org.nanobit.mystory.purchase.util.IabHelper.BILLING_RESPONSE_RESULT_OK;
import static org.nanobit.mystory.purchase.util.IabHelper.ITEM_TYPE_SUBS;

public class NAPurchaseManager {
	
    // (arbitrary) request code for the purchase flow
    static final int RC_REQUEST = 10001;
    static final boolean USE_LOG = true;
    static String sLogTag = "InApp";
	static String mTestIapId = "d1";
    static NAPurchaseManager sInstance;

    // The helper object
    IabHelper mHelper;
    Cocos2dxActivity mActivity;
    boolean mSetupSuccess;    
    Vector<String> mProductIds;
    
    String mGameName;
    String mUdid;
    boolean mDebug;
    
    public void cleanup() {
        // very important:
        if(USE_LOG) log("IAP: Destroying helper.");
        if (mHelper != null) {
            mHelper.dispose();
            mHelper = null;
        }
        mProductIds.clear();
    }
    
    void setActivity( Cocos2dxActivity a ){
    	mActivity = a;
    }    

    public boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
        if (mHelper == null) return false;

        return mHelper.handleActivityResult(requestCode, resultCode, data);         
    }
    
    private static void log(String message ){
    	if(USE_LOG) Log.d(sLogTag, message);
    }
    
    public NAPurchaseManager( Cocos2dxActivity mainActivity, String gameName, String udid, boolean debug, String logTag ) {
		sInstance = this;  
    	
    	setActivity(mainActivity);
		mSetupSuccess = false;
		mProductIds = new Vector<String>();
		mGameName = gameName;
		mUdid = udid;
		mDebug = debug;
		sLogTag = logTag;
				
		VerificationManager.setup(mGameName, mainActivity.getPackageName(), mUdid);

        // Create the helper, passing it our context and the public key to verify signatures with
        if(USE_LOG) log("IAP: Creating IAB helper.");
        mHelper = new IabHelper(mainActivity);
        mHelper.enableDebugLogging(mDebug);

        // Start setup. This is asynchronous and the specified listener
        // will be called once setup completes.
        if(USE_LOG) log("IAP: Starting setup.");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
            	if(USE_LOG) log("IAP: Setup finished.");

                if (!result.isSuccess()) {
                	final int status = result.getResponse();
                	final String message = result.getMessage();
                	mActivity.runOnGLThread(new Runnable() {						
						@Override
						public void run() {
							InappPurchaseManager.purchaseSetupFailed(status, message);
						}
					});
                    
                    return;
                }

                // Have we been disposed of in the meantime? If so, quit.
                if (mHelper == null) return;                
                mSetupSuccess = true;
                
                // IAB is fully set up. Now, let's get an inventory of stuff we own.
                if(USE_LOG) log("IAP: Setup successful.");
            }
        });
	}
    
    /*
     * INVENTORY QUERRY
     */
    
    /**
     * Initiate inventory and SKU querry statically (called from native code).
     * @param productIds Space separated product ids.
     */
    public void querryInventoryAndSKUs( String productIds, String username ){
    	if(productIds.isEmpty())
		{
			productIds = mTestIapId;
		}
    	List<String> productIdsArray = Arrays.asList( productIds.split(" ") );
    	Vector<String> productIdsVector = new Vector<String>(productIdsArray.size());
	    productIdsVector.addAll( productIdsArray );
	    
	    if(USE_LOG) log("IAP: Static retrieving SKU details for products:" + productIds);
    	querryInventoryAndSKUs(productIdsVector, username);
    }
    
    public void querryInventoryAndSKUs( Vector<String> productIds, String username ){
    	if(USE_LOG) log("IAP: Retrieving SKU details.");
    	
    	if( mActivity == null || mHelper == null || !mSetupSuccess ) return;

    	mProductIds.clear();
    	mProductIds.addAll(productIds);
    	
    	Log.v("IAP", "Ovo je username: " + username);
    	
    	mHelper.setUsername(username);
    	
    	// IAB is fully set up. Now, let's get an inventory of stuff we own.
        if(USE_LOG) log("IAP: Querying inventory.");
        if(!mHelper.mAsyncInProgress) {
			mHelper.queryInventoryAsync(true, productIds, mGotInventoryListener);
		}
    }
	
	 // Listener that's called when we finish querying the items and subscriptions we own
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            if(USE_LOG) log("IAP: Query inventory finished.");

            // Have we been disposed of in the meantime? If so, quit.
            if (mHelper == null) return;

            // Is it a failure?
            if (result.isFailure()) {
                mProductIds.clear();
                
                final int status = result.getResponse();
                final String message = result.getMessage();
                mActivity.runOnGLThread(new Runnable() {					
					@Override
					public void run() {
		                InappPurchaseManager.skuQuerryFailed(status, message);						
					}
				});
                return;
            }

            if(USE_LOG) log("IAP: Query inventory was successful.");
                        
            // Verify product IDs exist
            Vector<String> invalidProductIds = new Vector<String>();
            for( String productId : mProductIds){
            	if( !inventory.hasDetails(productId) || (productId.startsWith("android.test") && !mDebug) ){
            		invalidProductIds.add(productId);
            	}
            }
            mProductIds.removeAll(invalidProductIds);
            
            // Sending product data to Native
            String jsonString = "{\"products\":[";
            boolean ignoreComma = true;
            for( String productId : mProductIds ){
            	SkuDetails skuDetails = inventory.getSkuDetails(productId);
            	if( skuDetails != null ){
            		if( ignoreComma ){
            			ignoreComma = false;
            		}else{
            			jsonString += ",";
            		}
            		jsonString += skuDetails.getJSON();
            	}
            }
            jsonString += "]}";

            final String fJsonString = jsonString;
            mActivity.runOnGLThread(new Runnable() {				
				@Override
				public void run() {
		            InappPurchaseManager.skuDataRecieved(fJsonString);
		            if(USE_LOG) log("IAP: Updated sku details in native.");
				}
			});
            
            consumeInventory(inventory, false);
            
            if(USE_LOG) log("IAP: Initial inventory query finished.");
        }
    };
    
    void consumeInventory(Inventory inventory, boolean calledFromPurchase){
    	// Consume any non-inapp products left to consume
        Vector<Purchase> purchasesToConsume = new Vector<Purchase>();
        for( String productId : mProductIds){
			Purchase existingPurchase = inventory.getPurchase(productId);

			if(existingPurchase != null && existingPurchase.getItemType() == ITEM_TYPE_SUBS ){

				log("IAP: Got inapp subscription!:");
				final String sku = existingPurchase.getSku();
				final String message = "Successful subscription activation of sku " + existingPurchase.getSku();
				final String transactionId = existingPurchase.getOrderId();
				final long purchaseTimestamp = existingPurchase.getPurchaseTime();
				final boolean calledFromPurchaseBool = calledFromPurchase;
				mActivity.runOnGLThread(new Runnable() {
					@Override
					public void run() {
					InappPurchaseManager.subscriptionReceived(BILLING_RESPONSE_RESULT_OK, sku, message, transactionId, purchaseTimestamp, calledFromPurchaseBool);
					}
				});
			}

        	else if( existingPurchase != null  ){
        		purchasesToConsume.add(existingPurchase);
        	}
        }
        if( purchasesToConsume.size() > 0 ){
        	if(USE_LOG){
        		log("IAP: Consuming purchases:");
        		for( Purchase p : purchasesToConsume ){
        			if(USE_LOG) log("IAP: " + p.getPackageName());
        		}
        	}
        	
        	mHelper.consumeAsync(purchasesToConsume, mConsumeMultiFinishedListener);
        }
    }
    
    /*
     * PURCHASING
     */
       
    public void initiatePurchase(final String productId, String username, boolean isSub) {
       
    	if(USE_LOG) log("IAP: Launching purchase flow for " + productId);
    	
    	if( mActivity == null || mHelper == null ) return;
    	mHelper.setUsername(username);

		//horrible design, redo if you know how
		if(isSub) {
			VerificationManager.sRequestPayload(productId, new VerificationManager.OnPayloadRecievedListener() {
				@Override
				public void recievedPayload(String payload, int status) {
					if (mActivity == null || mHelper == null) return;
					if (status != VerificationBaseReponse.STATUS_SUCCESS) {
                        jPurchaseFailed(status < 4 ? VerificationBaseReponse.STATUS_UNKNOWN_ERROR : status, "", "");
						return;
					}
					((StoryGame) mActivity).blockGLResume(true);
					try {
						mHelper.launchSubscriptionPurchaseFlow(mActivity, productId, RC_REQUEST, mPurchaseFinishedListener, payload);
						if (USE_LOG) log("IAP: payload is:" + payload);

					} catch (NullPointerException e) {
						if (USE_LOG) log("IAP: iap helper crashed.");
					}
				}

				@Override
				public void responseError(int status) {
                    jPurchaseFailed(status < 4 ? VerificationBaseReponse.STATUS_UNKNOWN_ERROR : status, "", "");
				}
			});
		}
		else {
			VerificationManager.sRequestPayload(productId, new VerificationManager.OnPayloadRecievedListener() {
				@Override
				public void recievedPayload(String payload, int status) {
					if (mActivity == null || mHelper == null) return;
					if (status != VerificationBaseReponse.STATUS_SUCCESS) {
                        jPurchaseFailed(status < 4 ? VerificationBaseReponse.STATUS_UNKNOWN_ERROR : status, "", "");
						return;
					}
					((StoryGame) mActivity).blockGLResume(true);
					try {
						mHelper.launchPurchaseFlow(mActivity, productId, RC_REQUEST, mPurchaseFinishedListener, payload);
					} catch (NullPointerException e) {
						if (USE_LOG) log("IAP: iap helper crashed.");
					}
				}

				@Override
				public void responseError(int status) {
                    jPurchaseFailed(status < 4 ? VerificationBaseReponse.STATUS_UNKNOWN_ERROR : status, "", "");
				}
			});
		}
    }
  
    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
        	if( purchase != null ){
        		if(USE_LOG) log("IAP: Purchase finished: " + result + ", purchase: " + purchase + ", signature: " + purchase.getSignature());
        	}else{
        		if(USE_LOG) log("IAP: Purchase failed: " + result);
        	}
        	

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            if (purchase == null) {
            	// Consumption got stuck before, try to consume again
            	if( result.getResponse() == IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED ){
            		mHelper.queryInventoryAsync(false, null, new IabHelper.QueryInventoryFinishedListener() {						
						@Override
						public void onQueryInventoryFinished(IabResult result, Inventory inv) {
				            if (mHelper == null) return;
				            if (result.isFailure()) {
				            	jPurchaseFailed( result.getResponse(), "NONE", result.getMessage());
				            	return;
				            }
				            consumeInventory(inv, false);
						}
					});
            		return;
            	}

				int statusCode = result.getResponse() == IabHelper.IABHELPER_USER_CANCELLED ? 6 :
						result.getResponse();
                jPurchaseFailed(statusCode , "NONE", result.getMessage());
                return;
            }
            
            final String sku = purchase.getSku();
            for( String productId : mProductIds ){
            	if( sku.equals(productId) ){
            		if(USE_LOG) log("IAP: Purchase has a valid SKU, starting consumption.");
                    if(purchase.getItemType() != ITEM_TYPE_SUBS) mHelper.consumeAsync(purchase, mConsumeFinishedListener);
                    else if(purchase.getItemType() == ITEM_TYPE_SUBS){
						final String message = "Successful subscription activation of sku " + purchase.getSku();
						final String transactionId = purchase.getOrderId();
						final long purchaseTimestamp = purchase.getPurchaseTime();
						mActivity.runOnGLThread(new Runnable() {
							@Override
							public void run() {
						InappPurchaseManager.subscriptionReceived(BILLING_RESPONSE_RESULT_OK, sku, message, transactionId, purchaseTimestamp, true);
                            }
							});

					}
                    return;
            	}
            }
            
            if(USE_LOG) log("IAP: Unknown SKU "+ sku + "!");
            jPurchaseFailed( result.getResponse(), sku, result.getMessage());
        }
    };
    
    /**
     * Calls purchase failed inside a runnable.
     * @param status Status code from the IabResult
     * @param productId SKU that was beeing purchased
     * @param message Error message
     */
    private void jPurchaseFailed( final int status, final String productId, final String message ){    	
    	mActivity.runOnGLThread(new Runnable() {			
			@Override
			public void run() {
		        InappPurchaseManager.purchaseFailed(status, productId, message);
			}
		});
    	
    }

    // Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;
            
            if(USE_LOG) log("IAP: Consumption finished. Purchase3: " + purchase + ", result: " + result + ", signature: " + purchase.getSignature());
            
            final int status = purchase.isVerified() ? result.getResponse() : IabHelper.IABHELPER_VERIFICATION_FAILED;
            final String sku = purchase.getSku();
            final String message = result.getMessage();
			final String transactionId = purchase.getOrderId();
            mActivity.runOnGLThread(new Runnable() {				
				@Override
				public void run() {
		            InappPurchaseManager.purchaseCompleted(status, sku, message, transactionId);
				}
			}); 
           // SkuDetails skuDetails = inventory.getSkuDetails(productId);
            if( status == BILLING_RESPONSE_RESULT_OK ){
	            VerificationManager.sConsumePayload(purchase.getDeveloperPayload(), new VerificationManager.OnPayloadConsumedListener() {				
					@Override
					public void consumedPayload(int status) {
						// Not much to do here					
					}
					
					@Override
					public void responseError(int status) {
						// Not much to do here either
						//InappPurchaseManager.purchaseFailed(status < 4 ? 99 : status, "", "");
					}
				});
            }
        }
    };
        
    IabHelper.OnConsumeMultiFinishedListener mConsumeMultiFinishedListener = new IabHelper.OnConsumeMultiFinishedListener() {
        public void onConsumeMultiFinished(List<Purchase> purchases, List<IabResult> results) {
        	if(USE_LOG) log("IAP: MultiConsupmtion.");
            if (mHelper == null) return;
            
        	for( int i = 0; i<purchases.size(); i++ ){
        		Purchase purchase = purchases.get(i);
        		IabResult result = results.get(i);
        		
        		mConsumeFinishedListener.onConsumeFinished(purchase, result);        		
        	}
        }
    };


}
