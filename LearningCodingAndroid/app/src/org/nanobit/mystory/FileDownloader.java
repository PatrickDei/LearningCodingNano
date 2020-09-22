package org.nanobit.mystory;

import android.os.AsyncTask;
import android.os.Build;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxHelper;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;
import java.security.DigestOutputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Executor;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by paradzikm on 08/02/17.
 */

public class FileDownloader {
    private static final String digestAlgorithm = "MD5";

    private static final int CPU_COUNT = Runtime.getRuntime().availableProcessors();
    private static final int CORE_POOL_SIZE = CPU_COUNT + 1;
    private static final int MAXIMUM_POOL_SIZE = CPU_COUNT * 2 + 1;
    private static final int KEEP_ALIVE = 1;
    private static List<DownloadTask> downloadTasks = new ArrayList<DownloadTask>();

    private static final ThreadFactory sThreadFactory = new ThreadFactory() {
        private final AtomicInteger mCount = new AtomicInteger(1);

        public Thread newThread(Runnable r) {
            Thread thread = new Thread(r, "AsyncTask #" + mCount.getAndIncrement());
            thread.setPriority(Thread.MIN_PRIORITY);

            return thread;
        }
    };

    private static final BlockingQueue<Runnable> sPoolWorkQueue =
            new LinkedBlockingQueue<Runnable>(1024);


    private static final Executor THREAD_POOL_EXECUTOR
            = new ThreadPoolExecutor(CORE_POOL_SIZE, MAXIMUM_POOL_SIZE, KEEP_ALIVE,
            TimeUnit.SECONDS, sPoolWorkQueue, sThreadFactory);

    private static final Cocos2dxActivity mActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

    public static void downloadFromURL(String url , String path, final String fileName, final String fileHash) {
        if (Build.VERSION.SDK_INT >= 11) {
            //--post GB use serial executor by default --
            try
            {
                DownloadTask task = new DownloadTask();
                task.executeOnExecutor(THREAD_POOL_EXECUTOR, url, path, fileName, fileHash);
                downloadTasks.add(task);
            }
            catch (Exception e)
            {
                mActivity.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        onDownloadFailed(fileName, fileHash);
                    }
                 });
            }
        } else {
            //--GB uses ThreadPoolExecutor by default--
            new DownloadTask().execute(url, path, fileName, fileHash);
        }
    }

    public static void stopAllDownloads() {
        for( DownloadTask task : downloadTasks){
            Log.d("download", "try cancel");
            task.cancelTask();
        }
        downloadTasks.clear();

    }

    public static native void onDownloadSuccess(String filename, String fileHash);
    public static native void onDownloadFailed(String filename, String fileHash);

    private static class DownloadTask extends AsyncTask<String, Integer, DownloadStatus> {
        private boolean isTaskCancelled = false;

        public void cancelTask(){
            isTaskCancelled = true;
        }

        private boolean isTaskCancelled(){
            return isTaskCancelled;
        }

        @Override
        protected DownloadStatus doInBackground(String... params) {

            String urlString = params[0];
            String path = params[1];
            String fName = params[2];
            String hash = params[3];

            String appRoot = mActivity.getFilesDir().getAbsolutePath();

            File outFile = new File(appRoot + "/" + path  + "/" + fName);
            outFile.getParentFile().mkdirs();

            DownloadStatus status = new DownloadStatus();;
            status.file = fName;
            status.fileHash = hash;
            status.success = false;

            // if canceled
            if (isTaskCancelled()){
                Log.d("downlads", "canceles" + fName);
                return status;
            }

            int count;
            HttpURLConnection urlConnection=null;
            MessageDigest digest = null;
            
            try {

            URL url = new URL(urlString);
            urlConnection =  (HttpURLConnection) url.openConnection();
            urlConnection.connect();

            int statusCode = urlConnection.getResponseCode();
            if (statusCode != HttpURLConnection.HTTP_OK) {
                status.success = false;
                return status;
            }

                ReadableByteChannel in = Channels.newChannel(urlConnection.getInputStream());
                digest = MessageDigest.getInstance(digestAlgorithm);

                WritableByteChannel out = Channels.newChannel(new DigestOutputStream(
                        new FileOutputStream(outFile),
                        digest
                ));

                Log.d("FileDownloader", "downloading " + urlString + " to " + outFile.getAbsolutePath());
                ByteBuffer buffer = ByteBuffer.allocate(2048);
                while (in.read(buffer) != -1) {
                    buffer.flip();
                    out.write(buffer);
                    buffer.clear();
                }

                in.close();
                out.close();

            StringBuilder hexHash = new StringBuilder();
            for (byte aMessageDigest : digest.digest()) {
                hexHash.append(String.format("%02x", aMessageDigest));
            }
            String calculatedHash = hexHash.toString().toUpperCase();

            if(hash.length() > 0 && !hash.equalsIgnoreCase(calculatedHash)) {
                status.success = false;
                return status;
            }

        } catch (UnknownHostException e) {
                if (urlConnection != null) {
                    urlConnection.disconnect();
                }
                e.printStackTrace();
            }
            catch (Exception e) {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
            e.printStackTrace();
        }
        finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }

            }
            status.success = true;
            return status;
        }

        @Override
        protected void onPostExecute(final DownloadStatus downloadStatus) {
            super.onPostExecute(downloadStatus);

            mActivity.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    if(downloadStatus.success) {
                        onDownloadSuccess(downloadStatus.file, downloadStatus.fileHash);
                    } else {
                        onDownloadFailed(downloadStatus.file, downloadStatus.fileHash);
                    }
                }
            });
        }
    }

    protected static class DownloadStatus {
        String file;
        String fileHash;
        boolean success;
    }
}
