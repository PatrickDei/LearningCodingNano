package org.nanobit.mystory.purchase.verification;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;

import org.json.JSONObject;
import org.nanobit.mystory.purchase.InappPurchaseManager;

import android.os.AsyncTask;
import android.util.Log;

import com.adjust.sdk.Adjust;
import com.adjust.sdk.AdjustAttribution;

import javax.net.ssl.HttpsURLConnection;
//import android.util.Log;

public class VerificationManager {
	
	static private final String SERVER_URL = "https://validate.nanobitgames.com/";	// 10.22.1.200
	static private final String SUFFIX_VERIFICATION = "validate/v3";
	static private final String SUFFIX_VERIFICATION_SUBSCRIPTION = "validate/v3s";
	static private final String SUFFIX_GENERATE = "token/generate";
	static private final String SUFFIX_CONSUME = "token/consume";	
	
	static private VerificationManager singleton;
		
	private OnPayloadRecievedListener mPayloadGenerateListener;
	private OnPayloadConsumedListener mPayloadConsumeListener;
	private OnPurchaseVerifiedListener mPurchaseVerifiedListener;
	
	private String mGameId;
	private String mPackageName;
	private String mUdid;
	
	private VerificationManager(){
		mPayloadGenerateListener = null;
		mPayloadConsumeListener = null;
		mPurchaseVerifiedListener = null;
		mGameId = "";
		mPackageName = "";
		mUdid = "";
	}
	
	private static VerificationManager getInstance(){
		if( singleton == null ){
			singleton = new VerificationManager();
		}
		return singleton;
	}
	
	public static void setup( String gameId, String packageName, String udid ){
		VerificationManager manager = getInstance();
		manager.mGameId = gameId;
		manager.mPackageName = packageName;
		manager.mUdid = udid;
	}
		
	public void requestPayload( String productId, OnPayloadRecievedListener listener ){
		
		mPayloadGenerateListener = listener;
		if( mPayloadGenerateListener == null ) return;
		
		JSONObject object = VerificationPayloadResponse.generateJSONRequest(mGameId, mPackageName);
		VerificationPayloadResponse response = new VerificationPayloadResponse();
		
		log("Requesting payload for " + productId);
		VerificationTask task = new VerificationTask(response);
		task.execute(object.toString(), SUFFIX_GENERATE);
	}
	
	 public void consumePayload( String payload, OnPayloadConsumedListener listener ){
    	
		mPayloadConsumeListener = listener;
		if( mPayloadConsumeListener == null ) return;
				 
    	JSONObject object = VerificationPayloadResponse.generateConsumeJSONRequest(mGameId, mPackageName, payload);
		VerificationPayloadResponse response = new VerificationPayloadResponse();
		
		log("Consuming payload " + payload);
		VerificationTask task = new VerificationTask(response);
		task.execute(object.toString(), SUFFIX_CONSUME);
    }
	 
	public void verifyPurchase( String purchaseData, String signature, String productId, String developerPayload, String username, OnPurchaseVerifiedListener listener){
			
		mPurchaseVerifiedListener = listener;
		if( mPurchaseVerifiedListener == null ) return;

		VerificationPurchaseResponse response = new VerificationPurchaseResponse();

		JSONObject object;
		//if its a subscription, get revenue and adid info
		double revenueSub = InappPurchaseManager.productRevenue(productId);
		Log.d("verifyPurchase","verifyPurchase + " + revenueSub);
		String ver_suffix;
		if(revenueSub > 0)
		{
			String[] subArgs = new String[3];

			subArgs[0] = "USD";
			subArgs[1] = Double.toString(revenueSub);
			subArgs[2] = Adjust.getAdid();

			object = VerificationPurchaseResponse.generateJSONRequest(purchaseData, mUdid,
					username, mGameId, mPackageName, signature, developerPayload, subArgs);
			ver_suffix = SUFFIX_VERIFICATION_SUBSCRIPTION;
		}
		else
		{
			object = VerificationPurchaseResponse.generateJSONRequest(purchaseData, mUdid,
					username, mGameId, mPackageName, signature, developerPayload);
			ver_suffix = SUFFIX_VERIFICATION;
		}

		log("Verifying purchase::" + object.toString() + "::");
		VerificationTask task = new VerificationTask(response);
		task.execute(object.toString(), ver_suffix);
	}
	
	private void responseRecieved( VerificationBaseReponse response ){
		if( response == null ) return;
		
		switch( response.getType() ){
			case VerificationBaseReponse.TYPE_TOKEN_GENERATE:
				{
					VerificationPayloadResponse pResponse = (VerificationPayloadResponse)response;
					log("Payload recieved: " + pResponse.toString());
					if( mPayloadGenerateListener != null ){
						mPayloadGenerateListener.recievedPayload(pResponse.getToken(), pResponse.getStatus());
					}
					mPayloadGenerateListener = null;
				}
				break;
			
			case VerificationBaseReponse.TYPE_TOKEN_CONSUME:
				{
					VerificationPayloadResponse pResponse = (VerificationPayloadResponse)response;
					log("Consumed: " + response.toString());
					if( mPayloadConsumeListener != null ){
						mPayloadConsumeListener.consumedPayload(pResponse.getStatus());
					}
					mPayloadConsumeListener = null;
				}
				break;
				
			case VerificationBaseReponse.TYPE_VERIFICATION:
				{
					VerificationPurchaseResponse pResponse = (VerificationPurchaseResponse)response;
					log("Verification done: " + response.toString());
					if( mPurchaseVerifiedListener != null ){
						mPurchaseVerifiedListener.purchaseVerified(pResponse);
					}
					mPurchaseVerifiedListener = null;
				}
				break;
				
			case VerificationBaseReponse.TYPE_UNKNOWN:
			default:
				{
					if( mPayloadGenerateListener != null ){
						mPayloadGenerateListener.responseError( response.getStatus() );
					}
					mPayloadGenerateListener = null;
					if( mPayloadConsumeListener != null ){
						mPayloadConsumeListener.responseError( response.getStatus() );
					}
					mPayloadConsumeListener = null;
					if( mPurchaseVerifiedListener != null ){
						mPurchaseVerifiedListener.responseError( response.getStatus() );
					}
					mPurchaseVerifiedListener = null;
				}
				break;
		}
	}
			
	private static void log(String message){
		Log.d("VERIFICATION", message);
	}
	
	public static void sRequestPayload( String productId, OnPayloadRecievedListener listener ){
		getInstance().requestPayload(productId, listener);
	}
	
	public interface OnResponseErrorListener{
		void responseError( int status );
	}
		
	public interface OnPayloadRecievedListener extends OnResponseErrorListener {
		void recievedPayload( String payload, int status );
	}
	    
    public static void sConsumePayload( String payload, OnPayloadConsumedListener listener ){    	
		getInstance().consumePayload(payload, listener);
    }
    
    public interface OnPayloadConsumedListener extends OnResponseErrorListener {
		void consumedPayload( int status );
	}
	
	public static void sVerifyPurchase( String purchaseData, String signature, String productId, String developerPayload, String username, OnPurchaseVerifiedListener listener ){
		getInstance().verifyPurchase(purchaseData, signature, productId, developerPayload, username, listener);
	}
	
	public static boolean sSyncVerifyPurchase( String purchaseData, String signature, String productId, String developerPayload, String username, boolean isSub ,String... subArgs ){
		
		VerificationPurchaseResponse response = new VerificationPurchaseResponse();
		
		VerificationManager manager = getInstance();

		JSONObject object = VerificationPurchaseResponse.generateJSONRequest(purchaseData, manager.mUdid,
				username, manager.mGameId, manager.mPackageName, signature, developerPayload, subArgs);

		log("Verifying purchase::" + object.toString() + "::");

		String ver_suffix;
		if(isSub){
			ver_suffix = SUFFIX_VERIFICATION_SUBSCRIPTION;
		}
		else{
			ver_suffix = SUFFIX_VERIFICATION;
		}
		sendHTTPPost(response, object.toString(), ver_suffix);
		log("Verification done: " + response.toString());
		
		return response.isVerified();
	}
	
	public interface OnPurchaseVerifiedListener extends OnResponseErrorListener {
		void purchaseVerified( VerificationPurchaseResponse response );
	}
	
	private static void sendHTTPPost( VerificationBaseReponse response, String data, String serverSuffix ){
		response.reset();
		HttpURLConnection client = null;
		try {
			URL url = new URL(SERVER_URL + serverSuffix);
			client = (HttpURLConnection) url.openConnection();
            client.setConnectTimeout(10000);
            client.setReadTimeout(5000);
            client.setRequestProperty("Content-Type", "application/json");
            client.setDoOutput(true);
            client.setDoInput(true);
            client.setRequestMethod("POST");

            OutputStreamWriter os = new OutputStreamWriter(client.getOutputStream());
			os.write(data);

			os.flush();
            os.close();

			int responseCode = client.getResponseCode();
		    if( responseCode ==  HttpsURLConnection.HTTP_OK ){
                BufferedReader in = new BufferedReader(
                        new InputStreamReader(client.getInputStream()));
                StringBuilder stringBuilder = new StringBuilder();
                String currentLine;

                while ((currentLine = in.readLine()) != null)
                    stringBuilder.append(currentLine);

                in.close();
                log("Recieved response::" + stringBuilder.toString() + "::");
                response.loadWithResponse(stringBuilder.toString());
			        
			    } else{
			        //Closes the connection.
			        response.setStatus(VerificationBaseReponse.STATUS_CONNECTION_FAILED);
			        log("Connection failed.");
		    }
			
		} catch (MalformedURLException e) {
			response.setStatus(VerificationBaseReponse.STATUS_MALFORMED_URL);
			e.printStackTrace();
		} catch (IOException e) {
			response.setStatus(VerificationBaseReponse.STATUS_HTTP_PROBLEM);
			e.printStackTrace();
		} catch (RuntimeException e){
			response.setStatus(VerificationBaseReponse.STATUS_RUNTIME_EXCEPTION);
			e.printStackTrace();
		}
		finally {
			if(client != null)
				client.disconnect();
		}
	}
	
	private class VerificationTask extends AsyncTask<String, String, VerificationBaseReponse>{

		VerificationBaseReponse mResponse;
		
		VerificationTask( VerificationBaseReponse response ){
			mResponse = response;
		}
		
		@Override
		protected VerificationBaseReponse doInBackground(String... params) {
			mResponse.reset();
			String jsonData = params[0];
			String serverSuffix = params[1];
			
			VerificationManager.sendHTTPPost(mResponse, jsonData, serverSuffix);
						
			return mResponse;
		}
		
		@Override
		protected void onPostExecute(VerificationBaseReponse result) {
			VerificationManager.getInstance().responseRecieved(result);
		}		
		
	}

}
