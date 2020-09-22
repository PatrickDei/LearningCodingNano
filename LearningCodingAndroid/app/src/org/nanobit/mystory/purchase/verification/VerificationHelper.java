package org.nanobit.mystory.purchase.verification;

public class VerificationHelper {
	/*
	public static void requestPayload( String productId ){
		VerificationManager.sRequestPayload(productId, new VerificationManager.OnPayloadRecievedListener() {			
			@Override
			public void recievedPayload(String payload, int status) {
				verificationPayloadRecieved(payload != null ? payload : "", status);
			}

			@Override
			public void responseError(int status) {	
				verificationPayloadRecieved("", status);
			}
		});
	}
	
	public static void consumePayload( String payload ){
		VerificationManager.sConsumePayload( payload, new VerificationManager.OnPayloadConsumedListener() {				
			@Override
			public void consumedPayload(int status) {
				verificationPayloadConsumed(status);				
			}
			
			@Override
			public void responseError(int status) {
				verificationPayloadConsumed( Math.max(status, VerificationBaseReponse.STATUS_UNKNOWN_ERROR) );
			}
		});
	}
	
	public static void verifyPurchase( String purchaseData, String dataSignature, String productId, String payload, String username ){
		VerificationManager.sVerifyPurchase(purchaseData, dataSignature, productId, payload, username, 
        		new VerificationManager.OnPurchaseVerifiedListener() {
			
			@Override
			public void purchaseVerified(VerificationPurchaseResponse response) {				
				verificationPurchaseVerified( response.getProductId(), response.getStatus() );
			}
			
			@Override
			public void responseError(int status) {	
				verificationPurchaseVerified("", status);
			}
			
        });   
	}

	
	public static native void verificationPayloadRecieved( String payload, int status );
	public static native void verificationPayloadConsumed( int status );
	public static native void verificationPurchaseVerified( String productId, int status );*/
}
