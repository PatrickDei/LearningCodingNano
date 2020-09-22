package org.nanobit.mystory.purchase.verification;

import android.util.Log;

import org.json.JSONObject;

public class VerificationPurchaseResponse extends VerificationBaseReponse {
		
	public static final int ERROR_NOERROR = 0;
	public static final int ERROR_SERVER_ERROR = 1;
	public static final int ERROR_REQUEST_JSON_MALFORMED = 2;
	public static final int ERROR_BUNDLE_IDENTIFIER_MISMATCH = 3;
	public static final int ERROR_TRANSACTION_ID_MISMATCH = 4;
	public static final int ERROR_PRODUCTID_MISMATCH = 5;
	public static final int ERROR_QUANTITY_MISMATCH = 6;
	public static final int ERROR_VERSION_MISMATCH = 7;
	public static final int ERROR_TRANSACTION_NOT_FOUND = 8;
	public static final int ERROR_LOCAL_VERIFICAITON_FAILED = 1000;
	
	private int mError;
	private String mDescription;
	private String mProductId;
	private String mTransactionId;
	
	public VerificationPurchaseResponse(){	
	}
	
	protected void reset(){
		super.reset();
		mError = ERROR_NOERROR;
		mDescription = "";
		mProductId = "";
		mTransactionId = "";
	}
	
	public void loadWithJSON( JSONObject object ){		
		super.loadWithJSON(object);
		mError = getIntFromJSON(object, "Error", ERROR_NOERROR );
		mDescription = getStringFromJSON(object, "Description", "");
		mProductId = getStringFromJSON(object, "ProductId", "");
		mTransactionId = getStringFromJSON(object, "TransactionId", "");
	}
		
	public boolean isUndefined(){
		return (mStatus == STATUS_UNDEFINED && mError == ERROR_NOERROR) || mStatus >= STATUS_MALFORMED_URL; 
	}
	
	public void loadWithLocalVerificationResult( boolean result ){
		mStatus = result ? STATUS_SUCCESS : STATUS_FAILED;
		mError = result ? ERROR_NOERROR : ERROR_LOCAL_VERIFICAITON_FAILED;
	}
	
	public void loadUnableToConnect(){
		mStatus = STATUS_UNDEFINED;
		mError = ERROR_SERVER_ERROR;
	}
	
	public void loadDeveloperError( int error){
		mStatus = STATUS_UNDEFINED;
		mError = error;
	}
		
	public int getError(){
		return mError;
	}
			
	public String getDescription(){
		return mDescription;
	}
	
	public String getProductId(){
		return mProductId;
	}
	
	public String getTransactionId(){
		return mTransactionId;
	}

	public boolean isVerified() {
		return mStatus == STATUS_SUCCESS;
	}
	
	public static JSONObject generateJSONRequest(String purchaseData, String udid, String username, String game, String packageName, String signature, String payload, String... supArgs ){
		JSONObject object = new JSONObject();
    	// ReceiptRequestBase
    	putStringIntoJSON(object, "Receipt", purchaseData);            	
    	putStringIntoJSON(object, "Udid", udid);
    	putStringIntoJSON(object, "Username", username );				
    	putStringIntoJSON(object, "Game", game);
    	putStringIntoJSON(object, "BundleIdentifier", packageName);
		
		// ReceiptRequestAndroid
    	putStringIntoJSON(object, "Signature", signature);
    	putStringIntoJSON(object, "Token", payload);

		JSONObject extra = null;

		//SubscriptionRequest - adjust data mostly
		if(supArgs.length > 0) {
			extra = new JSONObject();
			putStringIntoJSON(extra, "Currency", supArgs[0]);

			Log.d("verifyPurchase","Currency + " + supArgs[0]);
		}
		if(supArgs.length > 1) {
			putStringIntoJSON(extra, "Revenue", supArgs[1]);
			Log.d("verifyPurchase", "Revenue + " + supArgs[1]);
		}
		if(supArgs.length > 2) {
			putStringIntoJSON(extra, "Adid", supArgs[2]);
			Log.d("verifyPurchase", "Adid + " + supArgs[2]);
		}

		if(extra != null)
		{
			putObjectIntoJSON(object, "Extra", extra);
		}

		return object;
	}
	
	public String toString(){
		return "{ " + super.toString() +", error: " + mError + ", description: " + mDescription + ", productId: " + mProductId + ", transactionId: " + mTransactionId + " }";		
	}
}
