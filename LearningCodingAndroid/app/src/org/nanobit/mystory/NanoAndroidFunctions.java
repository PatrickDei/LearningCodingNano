package org.nanobit.mystory;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.provider.Settings;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.ProgressBar;

import com.adjust.sdk.Adjust;
import com.adjust.sdk.AdjustAttribution;
import com.adjust.sdk.AdjustEvent;
import com.adjust.sdk.OnDeviceIdsRead;
import com.bugfender.sdk.Bugfender;
import com.crashlytics.android.Crashlytics;
import com.soomla.traceback.SoomlaTraceback;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.StringWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.UUID;
import java.util.concurrent.ExecutionException;

import javax.net.ssl.HttpsURLConnection;

import static com.adjust.sdk.Adjust.getGoogleAdId;

//import android.util.Log;

@SuppressLint("DefaultLocale") public class NanoAndroidFunctions {
	
	static  ProgressDialog mDialog = null;
	static ProgressBar spinner;
	static boolean keepProgressHidden = false;
	static int spinnerVisibility = View.VISIBLE;
	public static int savesCount = 0;

	//mixpanel variables
    static String mixpanelApiKeyDev = "f3061172b4ef6f464fc6d3facfbb6534";
    static String mixpanelApiKeyProd = "b1be076a2efac76fa9253fb9c84dad92";

    public static void openURL(String str)
	{
		
		final Cocos2dxActivity context = (Cocos2dxActivity) Cocos2dxActivity.getContext();
		
		try {
		    Intent myIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(str));
		    
		    context.startActivity(myIntent);
		} catch (final ActivityNotFoundException e) {
			
		}	
		
	};

	public static void openAdjustURL(String str)
	{
		final Cocos2dxActivity context = (Cocos2dxActivity) Cocos2dxActivity.getContext();
		final String adjustUrl = str.substring(0, str.length() - 5);
		getGoogleAdId(context, new OnDeviceIdsRead() {
			@Override
			public void onGoogleAdIdRead(String googleAdId) {
				HttpURLConnection urlConnection = null;
				try {
					String androidId = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
					String output = String.format("%sgps_adid=%s&android_id=%s",adjustUrl, googleAdId, androidId);

					URL url = new URL(output);
					urlConnection = (HttpsURLConnection) url.openConnection();
					urlConnection.setRequestMethod("GET");
					urlConnection.setConnectTimeout(60);
					urlConnection.connect();
					if (urlConnection.getResponseCode() == HttpsURLConnection.HTTP_OK) {
						Log.d("NanoAndroidFunct", "openAdjustURL status ok");
					} else {
						Log.d("NanoAndroidFunct", "openAdjustURL status " + urlConnection.getResponseCode() + " " + urlConnection.getErrorStream());
					}
				} catch (final Exception e) {
					e.printStackTrace();
				}
				finally {
					if (urlConnection != null) {
						urlConnection.disconnect();
					}
				}
			}
		});
	}

	public static boolean canOpenURL(String str)
	{
		try
		{
			URL url = new URL(str);
			HttpURLConnection connection = (HttpURLConnection) url.openConnection();
			int code = connection.getResponseCode();

			if(code == 200)
		    {
		    	return true;
		    } 
		}
		catch (Exception e)
		{
		}
		return false;
	}
	
    @SuppressLint("NewApi") @SuppressWarnings("deprecation")
    public static void copyToClipboard(final String text) {
    	
    	Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        activity.runOnUiThread( new Runnable() {
        	
        	public void run(){
        		Context context =  Cocos2dxActivity.getContext();
                try {
                    int sdk = android.os.Build.VERSION.SDK_INT;
                    if (sdk < android.os.Build.VERSION_CODES.HONEYCOMB) {
                        android.text.ClipboardManager clipboard = (android.text.ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
                        clipboard.setText(text);
                    } else {
                        ClipboardManager clipboard = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
                        ClipData clip = ClipData.newPlainText(text, text);
                        clipboard.setPrimaryClip(clip);
                    }
                    
                } catch (Exception e) {
                    //Log//Log.e("clipboardManagerexcp", "e:",e);
                }
        	}
        });
    }

    public static void nanoDialogSpawner(final String param, final int objectID){
    	
    	String[] parts = param.split(";");
    	int lenght = parts.length;
    	if(lenght < 2) return;
    	Log.d("alert", param + parts);
    	final String title = parts[0];
    	final String message = parts[1];
    	String cancelButton = "";
    	String okButton = "";
		String neutralButton = "";
    	if(lenght > 2){
    		cancelButton = parts[2];}
    	if(lenght > 3){
    		okButton = parts[3];}
		if(lenght > 4){
			neutralButton = parts[4];}
    	final String cancelButtonText = cancelButton;
    	final String okButtonText = okButton;
		final String neutralButtonText = neutralButton;
    	Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        activity.runOnUiThread( new Runnable() {
        
        	@Override
        	public void run(){
        		NanoAlertDialog nanoDialog = new NanoAlertDialog();
        		nanoDialog.createDialog(title, message, cancelButtonText, okButtonText, neutralButtonText, objectID);
        	}
        	
        });
    }
    
    private static void updateSpinnerVisibility( int visibility )
    {
    	spinnerVisibility = visibility;
    	if( spinner != null ){
    	   spinner.setVisibility( keepProgressHidden ? View.GONE : spinnerVisibility );
    	}
    }
    
    public static void setKeepProgressHidden( boolean state ){
    	keepProgressHidden = state;
    	updateSpinnerVisibility( spinnerVisibility );
    }

     
    public static void hideActivityDialog(){
    	    	
    	Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        activity.runOnUiThread( new Runnable() {
        	@Override
        	public void run(){
        		updateSpinnerVisibility( View.GONE );
        	}
        });
    }
    
    public static String generateUUID(){
    	
    	String uuidString;
    	UUID newUUIDU = UUID.randomUUID();
    	uuidString = newUUIDU.toString();
    	return uuidString;
    }
       
    public static String decodeFromBase64(String input){
    	String toReturnString = input;
    	try {
    		byte[] data = Base64.decode(input, Base64.DEFAULT);
    		toReturnString = new String(data, "UTF-8");
		} catch (Exception e) {
			toReturnString = "";
		}
    	
    	return toReturnString;
    }
    
    public static String encodeToBase64(String input)
    {
    	byte[] data;
    	String toReturnString = input;
    	try {
    	    data = input.getBytes("UTF-8");
    	    toReturnString = Base64.encodeToString(data, Base64.DEFAULT);

    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	return toReturnString;
    }
    
    private static String checkFilesDir()
    {
    	String filesPath = StoryGame.getContext().getFilesDir().getAbsolutePath();
		File file = new File(filesPath);
		file.mkdirs();
		if(!file.exists()) 
		{					
			return "";
		}   
		return filesPath;
    }
    private static void createFile(String fullPath)
    {
    	File f = new File(fullPath);
    	// Works for both Windows and Linux
 		f.getParentFile().mkdirs(); 
 		try {
 		f.createNewFile();
 		 } 
 		catch (IOException e) {
             System.err.println("already exists: " + e.getMessage());
         }
    }
    
    private static boolean fileOrDirectoryExists(String filesPath)
    {
    	File file = new File(filesPath);
		if(file.exists()) 
		{					
			return true;
		}
		return false;
    }

    private static boolean fileExistInAssets(String filesPath){

        InputStream in = null;
        try {
            filesPath = filesPath.substring(filesPath.lastIndexOf("/") + 1);
            in = StoryGame.getContext().getAssets().open( filesPath);

        } catch(IOException e) {
            e.printStackTrace();
        }
         finally {
            if (in != null) {
                try {
                    in.close();
                    return true;
                }
                catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return false;
    }

    private static boolean createDirectory(String path)
    {
    	File folder = new File(path);
    	boolean success = true;
    	if (!folder.exists()) {
    		success = folder.mkdir();
    	}
    	if (success) {
    		return true;
    	} else {;
	    // Do something else on failure 
    	}
    	return false;
    }

    private static void logIOExceptionOnFileClose( Exception e, String fileName, boolean writing )
    {
    	if (fileName.equals("savegame_json2.sav"))
		{
			e.printStackTrace();
		}
    }
    private static boolean removeFileOrDirectory(String path)
    {
    	try {

         File fileOrDirectory = new File (path); 
         fileOrDirectory.delete();
    	}
          catch (Exception e) {
            // Log.e("tag", e.getMessage());
            return false;
        }
    	return true;
    }
    
    private static boolean removeAllFilesInDirectory(String path)
    {
    	try {
			File directory = new File (path);
			for (File file: directory.listFiles()) {
				deleteRecursive(file);
			}
    	}
          catch (Exception e) {
            // Log.e("tag", e.getMessage());
            return false;
        }
    	return true;
    }
    
    public static void deleteRecursive(File fileOrDirectory) {

    	 if (fileOrDirectory.isDirectory())
    	    for (File child : fileOrDirectory.listFiles())
    	        deleteRecursive(child);
    	    fileOrDirectory.delete();
   } 
    
    private static boolean copyItem(String inputPath, String outputPath) {

        InputStream in = null;
        OutputStream out = null;
        try {

            //create output directory if it doesn't exist
            File dir = new File (outputPath); 
            if (!dir.exists())
            {
            	new File(dir.getParent()).mkdirs();
               // dir.mkdirs();
            }


            in = new FileInputStream(inputPath );        
            out = new FileOutputStream(outputPath );

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;

                // write the output file (You have now copied the file)
                out.flush();
            out.close();
            out = null;        

        }  catch (FileNotFoundException fnfe1) {
            // Log.e("tag", fnfe1.getMessage());
            return false;
        }
                catch (Exception e) {
            // Log.e("tag", e.getMessage());
            return false;
        }
        return true;

    }
      
    
    /////////////
    // WRITING //
    /////////////

    public static boolean saveToFileAndHash(String input, String fileName)
    {
    	//store data
    	boolean toRet = saveToFile(input, fileName);
    	
    	//store hash
    	input += "3nano2Pass1save";
    	String hashString = createHash(input.getBytes());
    	toRet = saveToFile(hashString, "SLSI" + fileName) ? toRet : false;
    	
    	return toRet;
    }
    
    public static boolean saveToFile(String input, String fileName)
    {
    	if (fileName.equals("savegame_json2.sav")) {
			savesCount++;
		}    	    	    	
    	
    	FileOutputStream outputDataStream;    
    	boolean success = true;
    	// 1. Try regular file output
		try {
			  outputDataStream = ((StoryGame)StoryGame.getContext()).openFileOutput(fileName, StoryGame.MODE_PRIVATE);
			  success = writeFileStream( outputDataStream, input, fileName );
			  outputDataStream.close();
		} 
		catch (FileNotFoundException e1)
		{
			if (fileName.equals("savegame_json2.sav"))
			{
				e1.printStackTrace();
			    success = false;
			}
			throw new AssertionError();
		}
		catch( IOException e )
		{			
			logIOExceptionOnFileClose(e, fileName, true);
			throw new AssertionError();
		}
		
		File file = StoryGame.getContext().getFileStreamPath(fileName);
        if ( !success || file == null || !file.exists() ) 
        {	
        	// 2. Check if folder where Android saves files exists
			String filesPath = checkFilesDir();
			if( filesPath.length() > 0 )
			{
				// 3. Try to write the file directly to the path
				try 
				{
					outputDataStream = new FileOutputStream( filesPath + File.separator + fileName );
					success = writeFileStream( outputDataStream, input, fileName );
					outputDataStream.close();
				} 
				catch (FileNotFoundException e) 
				{
					if (fileName.equals("savegame_json2.sav"))
					{					
						e.printStackTrace();
					    success = false;
					}
					throw new AssertionError();
				}
				catch( IOException e )
				{
					logIOExceptionOnFileClose(e, fileName, true);
					throw new AssertionError();
				}
			}
			else
			{        	
				savesCount = 0;
				success = false;
				throw new AssertionError();
			}
		}
		
    	return success;
    }
    
    private static boolean writeFileStream( FileOutputStream outputDataStream, String input, String fileName )
    {    	 	
		try {
	    	  outputDataStream.write(input.getBytes());	    	  
		} 
		catch (IOException e2) 
		{
			if (fileName.equals("savegame_json2.sav")) {
				e2.printStackTrace();
				return false;
			}
			throw new AssertionError();
		}
//		File file = Stardroid.getContext().getFileStreamPath(fileName);
//        if (file == null || (file !=null && !file.exists()) ) {
//			savesCount = 0;
//			throw new AssertionError();
//		}
		
    	return true;
    }
    
	/////////////
	// READING //
	/////////////
	    
    /**
     * Reads from a given file input stream.
     * @param inStream
     * @param fileName Name of the read file, used for error logging.
     * @return File contents.
     */
    private static String readFileStream( FileInputStream inStream, String fileName )
    {    	
    	String contents = new String("");
    	
		try {
			 int length = (int)inStream.getChannel().size();
			 byte[] bytes = new byte[length];
			 inStream.read(bytes);
			 //inStream.close();
			 contents = new String(bytes);
		} 
		catch (IOException e) 
		{
			if (fileName.equals("savegame_json2.sav"))
			{
				e.printStackTrace();
	
				throw new AssertionError();
			}
		}
		catch (Exception e) 
		{
			if (fileName.equals("savegame_json2.sav"))
			{
				e.printStackTrace();	
				throw new AssertionError();
			}
		}
		
    	return contents;
    
    }
    
    private static String readFileStream( FileInputStream inStream, String fileName, int length )
    {    	
    	String contents = new String("");
    	
		try {
			 byte[] bytes = new byte[length];
			 inStream.read(bytes);
			 //inStream.close();
			 contents = new String(bytes);
		} 
		catch (IOException e) 
		{
			if (fileName.equals("savegame_json2.sav"))
			{
				e.printStackTrace();
	
				throw new AssertionError();
			}
		}
		catch (Exception e) 
		{
			if (fileName.equals("savegame_json2.sav"))
			{
				e.printStackTrace();	
				throw new AssertionError();
			}
		}
		
    	return contents;
    
    }
          
    public static String readFromFile(String fileName)
    {
    	String contents = new String("");
    	// 1. Normal Android file reading
		try {
			FileInputStream inStream = StoryGame.getContext().openFileInput(fileName);
			contents = readFileStream(inStream, fileName);	
			inStream.close();
		} 
		catch (FileNotFoundException e)
		{
			try{
				// 2. Check if folder where Android saves files exists
				String filesPath = checkFilesDir();
				if( filesPath.length() > 0 )
				{		
					// 3. Read from the file that now hopefully exists
					FileInputStream inStream = new FileInputStream( filesPath + "/" + fileName );
					contents = readFileStream(inStream, fileName);
					inStream.close();
				}
			}
			catch( FileNotFoundException e2 )
			{					
				e.printStackTrace();
				SharedPreferences settings = ((Activity)StoryGame.getContext()).getSharedPreferences("Cocos2dxPrefsFile", 0);
				//if (fileName.equals("savegame_json2.sav")) throw new AssertionError();
			}
			catch( IOException e3 )
			{				
				logIOExceptionOnFileClose(e3, fileName, false);
			}
			//if (fileName.equals("savegame_json2.sav")) throw new AssertionError();
		}
		catch (IOException e2) 
		{
			logIOExceptionOnFileClose(e2, fileName, false);
			//if (fileName.equals("savegame_json2.sav")) throw new AssertionError();
		}
		
    	return contents;
    }
    
    public static String readFromFileAndCheckHash(String fileName)
    {
    	String dataString = readFromFile(fileName);
    	
    	String hashString = readFromFile("SLSI" + fileName);
    	hashString = hashString.toUpperCase(Locale.getDefault());

    	boolean hashCheck = checkHash(dataString, hashString);	
    	
    	String toReturn = "";
    	if (hashCheck) {
    		SharedPreferences settings = ((Activity)StoryGame.getContext()).getSharedPreferences("Cocos2dxPrefsFile", 0);
    		if( settings.getBoolean("saveUsingBase64", true) ) {
    			String decodedString = decodeFromBase64(dataString);
    			toReturn = decodedString;
    		}
    		else toReturn = dataString;
    	}
        return toReturn;
    }
    
    private static String createHash(byte[] input)
    {
        final char[] HEX_CHARS = "0123456789ABCDEF".toCharArray();
        MessageDigest md = null;
        try {
            md = MessageDigest.getInstance("SHA-1");
        }
        catch(NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        byte[] buf = md.digest(input);
        char[] chars = new char[2 * buf.length];
        for (int i = 0; i < buf.length; ++i) {
            chars[2 * i] = HEX_CHARS[(buf[i] & 0xF0) >>> 4];
            chars[2 * i + 1] = HEX_CHARS[buf[i] & 0x0F];
        }
        return new String(chars);
    }
    
    static boolean checkHash(String dataToCheck, String hash)
    {    	
    	String helperString = dataToCheck + "3nano2Pass1save";
    	String currentHash = createHash(helperString.getBytes());
    	
    	if (currentHash.equals(hash)) {
			return true;
		}
    	 return false;
    }

    
    //Checking if WiFi is enabled on the device
    public static boolean isWifiAvailable()
    {
    	ConnectivityManager connectivityManager= (ConnectivityManager) StoryGame.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
    	NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
    	return activeNetworkInfo != null && activeNetworkInfo.isConnectedOrConnecting();
    }

	public static void bugfenderInit(String udid)
	{
		Bugfender.setDeviceString( "udid", udid);
	}

	public static void bugfenderSetUsername(String username)
	{
		Bugfender.setDeviceString( "username", username);
	}

	public static void bugfenderLog(String log) {
		Bugfender.d("storygame-android", log);
	}

    public static void bugfenderWarn(String log) {
        Bugfender.w("storygame-android", log);
    }

    public static void bugfenderError(String log) {
        Bugfender.e("storygame-android", log);
    }

	public static void setCostumFabricLog(String msg)
	{
		Crashlytics.log(msg);
	}

	public static String getNGSExtraParameters()
	{
		//TODO - merge this and addAdjustParameters

		Log.d("getNGSExtraParameters","getNGSExtraParameters");

		JSONObject parameters = new JSONObject();

		try{

			AdjustAttribution attribution = Adjust.getAttribution();
			if(attribution == null)
				return "";

			//Adjust parameters - called here instead of MixPanelController to avoid multiple JNI calls
			if (Adjust.getAdid() != null);
				parameters.put("Adid", Adjust.getAdid());
			if (attribution.network != null);
				parameters.put("Network", attribution.network);
			if (attribution.campaign != null);
				parameters.put("Campaign", attribution.campaign );
			if (attribution.adgroup != null);
				parameters.put("Adgroup", attribution.adgroup);
			if (attribution.creative != null)
				parameters.put("Creative", attribution.creative);

		}
		catch (Exception e)
		{
			Log.e("Extr parameters for NGS", e.getMessage());
			return "";

		}

		Log.d("getNGSExtraParameters","getNGSExtraParameters");
		if(parameters.toString().equals(""))
			return "";
		else
			return parameters.toString();
	}

	public static void reportAdjustFirstSubscriptionPurchase()
	{
		AdjustEvent event = null;
		if (BuildConfig.BUILD_TYPE.equals("flexion")) {
			event = new AdjustEvent("c0choz");
		}
		else {
			event = new AdjustEvent("kt1i4y");
		}
		Adjust.trackEvent(event);
		Log.d("Adjust", "Track first subscription purchase");
	}

	public static void reportAdjustFirstPurchase()
	{
		AdjustEvent event = null;
		if (BuildConfig.BUILD_TYPE.equals("flexion")) {
			event = new AdjustEvent("391fj8");
		}
		else {
			event = new AdjustEvent("kftbxf");
		}
		Adjust.trackEvent(event);
		Log.d("Adjust", "Track first purchase");
	}

	//Adjust reporting 70% of the current purchase
	public static void reportAdjustSubscriptionTransaction(double dollarValue)
	{
		AdjustEvent event = null;
		if (BuildConfig.BUILD_TYPE.equals("flexion")) {
			event = new AdjustEvent("28qa00");
		}
		else {
			event = new AdjustEvent("9j62fz");
		}
		event.setRevenue(dollarValue*0.7, "USD");
		Adjust.trackEvent(event);

		Log.d("Adjust", "Track subscription purchase with: " + dollarValue * 0.7);
	}


	//Adjust reporting 70% of the current purchase
	public static void reportAdjustTransaction(double dollarValue)
	{
		AdjustEvent event = null;
		if (BuildConfig.BUILD_TYPE.equals("flexion")) {
			event = new AdjustEvent("tv2pny");
		}
		else {
			event = new AdjustEvent("9malyr");
		}
		event.setRevenue(dollarValue*0.7, "USD");
		Adjust.trackEvent(event);

		Log.d("Adjust", "Track purchase with: " + dollarValue * 0.7);
	}

	public static void trackAdjustEvent(String token)
	{
		AdjustEvent event = new AdjustEvent(token);
		Adjust.trackEvent(event);

		Log.d("Adjust", "Track event with token: " + token);
	}

	public static void soolmaTagUser(String param)
	{
		String[] parts = param.split(";");
		final String sMoneySpent = parts[0];
		final String sChaptersRead = parts[1];
		Float moneySpent = Float.parseFloat(sMoneySpent);
		if(moneySpent == Float.POSITIVE_INFINITY || moneySpent == Float.NEGATIVE_INFINITY || moneySpent == Float.NaN)
		{
			moneySpent = 0.f;
		}

		Float chaptersRead = Float.parseFloat(sChaptersRead);
		if(chaptersRead == Float.POSITIVE_INFINITY || chaptersRead == Float.NEGATIVE_INFINITY || chaptersRead == Float.NaN)
		{
			chaptersRead = 0.f;
		}

		List<String> tags = new ArrayList<String>();
		if(moneySpent > 0) {
			tags.add("spender");
		}
		if(chaptersRead > 20) {
			tags.add("reader");
		}
		if(tags.size() > 0) {
			SoomlaTraceback.getInstance().addTags(tags);
		}

		Log.d("Soomla", "soolmaTagUser sMoneySpent: " + sMoneySpent + "chaptersRead " + chaptersRead);
	}

	public static void shareUserInvite(String link, String title)
	{
		String shareBody = link;
		Intent sharingIntent = new Intent(android.content.Intent.ACTION_SEND);
		sharingIntent.setType("text/plain");
		sharingIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, title);
		sharingIntent.putExtra(android.content.Intent.EXTRA_TEXT, shareBody);
		final Cocos2dxActivity context = (Cocos2dxActivity) Cocos2dxActivity.getContext();
		((StoryGame)StoryGame.getContext()).blockGLResume(true);
		context.startActivity(Intent.createChooser(sharingIntent, "Share via: "));
	}

	public static boolean isPushNotificationEnabled()
	{
		Boolean notificationsEnabled = true;

		try {
			notificationsEnabled = FCMHelper.areNotificationsEnabled();
		} catch (final ActivityNotFoundException e) {

		}

		return notificationsEnabled;
	}
}
