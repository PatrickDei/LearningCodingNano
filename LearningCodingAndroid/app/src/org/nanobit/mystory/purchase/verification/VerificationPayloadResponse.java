package org.nanobit.mystory.purchase.verification;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.json.JSONObject;

import android.util.Base64;

public class VerificationPayloadResponse extends VerificationBaseReponse {
		
	private String mToken;
	
	public VerificationPayloadResponse(){	
	}
	
	protected void reset(){
		super.reset();
		mToken = "";
	}
	
	public void loadWithJSON( JSONObject object ){		
		super.loadWithJSON(object);
		mToken = getStringFromJSON(object, "Token", "");	
	}
		
	public static String generateLocalPayload( String udid, String productId ) {
		try {
			
        	String secretCombo = udid + productId;
        	MessageDigest md = MessageDigest.getInstance("SHA-1");
        	md.update( secretCombo.getBytes() );
        	return new String(Base64.encode(md.digest(), 0));
        	
		} catch (NoSuchAlgorithmException e) {
		}

		return productId;
	}
	
	public void loadWithLocalPayload( String udid, String productId ){		
        mToken = generateLocalPayload(udid, productId);
        setStatusAndType(STATUS_SUCCESS, TYPE_UNKNOWN);        
	}
	
	public void loadFailedStatus(){
		setStatusAndType(STATUS_FAILED, TYPE_UNKNOWN);  
		mToken = "";
	}
					
	public String getToken(){
		return mToken;
	}

	public boolean didFail() {
		return mStatus == STATUS_FAILED || mToken.length() == 0;
	}
	
	public static JSONObject generateJSONRequest( String game, String packageName ){
		JSONObject object = new JSONObject();
    	
		// TokenRequestBase
    	putStringIntoJSON(object, "Game", game);            	
    	putStringIntoJSON(object, "BundleIdentifier", packageName);
    	
        return object;
	}
	
	public static JSONObject generateConsumeJSONRequest( String game, String packageName, String payload ){
		JSONObject object = generateJSONRequest(game, packageName);
		
		// TokenConsumeRequest
		putStringIntoJSON(object, "Token", payload);
    	
        return object;
	}
	
	public String toString(){
		return "{ " + super.toString() +", token: " + mToken + " }";		
	}
}
