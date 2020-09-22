package org.nanobit.mystory.purchase.verification;

import org.json.JSONException;
import org.json.JSONObject;

public class VerificationBaseReponse {	

	public static final int STATUS_UNDEFINED = 0;
	public static final int STATUS_SUCCESS = 1;
	public static final int STATUS_FAILED = 2;
	
	public static final int STATUS_UNKNOWN_ERROR = 99;
	public static final int STATUS_CONNECTION_FAILED = 100;
	public static final int STATUS_MALFORMED_URL = 101;
	public static final int STATUS_HTTP_PROBLEM = 102;
	public static final int STATUS_RUNTIME_EXCEPTION = 103;
	
	public static final int TYPE_UNKNOWN = 0;
	public static final int TYPE_VERIFICATION = 1;
	public static final int TYPE_TOKEN_GENERATE = 2;
	public static final int TYPE_TOKEN_CONSUME = 3;
	
	protected int mStatus;
	protected int mType;	
	
	public VerificationBaseReponse(){
		reset();
	}
	
	public void loadWithResponse( String jsonString ){
		try {
			JSONObject responseJson = new JSONObject(jsonString);
			loadWithJSON(responseJson);
		} catch (JSONException e) {
			reset();
		}
	}
	
	public void loadWithJSON( JSONObject object ){
		mStatus = getIntFromJSON(object, "Status", STATUS_UNDEFINED);
		mType = getIntFromJSON(object, "Type", TYPE_UNKNOWN);
	}
	
	protected void reset(){
		mStatus = STATUS_UNDEFINED;
		mType = TYPE_UNKNOWN;
	}
	
	protected static String getStringFromJSON( JSONObject object, String key, String defaultValue ){
		try {			
			return object.has(key) ? object.getString(key) : defaultValue;
		} catch (JSONException e) {			
		}
		return defaultValue;
	}
	
	protected static int getIntFromJSON( JSONObject object, String key, int defaultValue ){
		try {			
			return object.has(key) ? object.getInt(key) : defaultValue;
		} catch (JSONException e) {			
		}
		return Integer.getInteger( getStringFromJSON(object, key, String.valueOf(defaultValue)) );
	}	

	protected static void putStringIntoJSON( JSONObject object, String key, String value ){
		try {
            object.put( key, value != null ? value : "" );          
		} catch (JSONException e) {
		}
	}

	protected static void putObjectIntoJSON( JSONObject object, String key, JSONObject value ){
		try {
			object.put( key, value != null ? value : "" );
		} catch (JSONException e) {
		}
	}


	public void setStatus( int status ){
		mStatus = status;
	}
	
	protected void setStatusAndType( int status, int type ){
		mStatus = status;
		mType = type;
	}
	
	public int getStatus(){
		return mStatus;
	}
			
	public int getType(){
		return mType;
	}
	
	public String toString(){
		return "{ status: " + mStatus + ", type: " + mType + " }";
		
	}

}
