//
//  NGSUtilHeader.h
//  Hollywood
//
//  Created by Matko Juribasic on 10/1/13.
//
//

#ifndef Hollywood_NGSUtilHeader_h
#define Hollywood_NGSUtilHeader_h

#define kUserDefaultNGSUdid "NGS_udid"
#define kUserDefaultNGSIfa "NGS_ifa"

#define kNotificationNGSLostConnection "a4cfcb93-8cf5-490f-8531-e998a1fe4f41"
#define kNotificationNGSTooManyRejected "3d77e502-5d53-48d4-b85b-c84fb22c880d"

#define kNotificationNGSUserShouldRegister "205327bf-33d8-457f-9689-2119e5236022"
#define kNotificationNGSServerTimeReceived "411ee364-c686-408c-a2bc-d941914b1e21"
#define kNotificationNGSFirstCommunicationDone "472c55b1-ddff-464b-b348-72ba5c0aead3"
#define kNotificationNGSNewVersionAvailable "NotificationNGSNewVersionAvailable"
#define kNotificationNGSUserMustUpdate "a7467499-d7d9-4362-9f95-90e1ced77196"
#define kNotificationNGSOtherDevice "19c23c68-af90-408d-979e-accd577db59c"
#define kNotificationNGSProfilesReceived "f2081843-08bd-47a3-bbfc-820117006578"
#define kNotificationNGSParametersReceived "b7d7c13c-388d-4cf8-b6f8-0167eb1ddd7d"
#define kNotificationNGSDownloadsReceived "13bad419-8a26-4259-95ed-8ebf1c2c6705"
#define kNotificationNGSSaveGameReceived "a27d4221-4600-4505-99d0-426adf692791"
#define kNotificationNGSSaveRestoreGCReceived "9b6e54c9-ebcd-46aa-99c9-423627f8a1cf"
#define kNotificationNGSShoudlReconnect "NotificationNGSShoudlReconnect"
#define kNotificationNGSResponseReceived "82425896-81e7-49b0-a40e-29d6c9c5b518"

// parameters
//#define __ngs_URL_websocket_dev             "ws://10.21.0.95:5000/compressed"
#define __ngs_URL_websocket_dev ""
#define __ngs_URL_websocket ""
#define __ngs_URL_connection_check ""
#define __ngs_game_identifier "SG"
#define __ngs_interval_queue_sending 4.0f
#define __ngs_max_queue_batch_size 5
#define __ngs_interval_cache_check 0.2f
#define __ngs_request_timeout 15.0f
#define __ngs_number_of_message_send_tries 2
#define __ngs_ping_interval 7.5
#define __ngs_wait_for_ws 10
#define __ngs_max_inactive_time 15
#define __ngs_save_game_send_interval 120
#define __ngs_websocket_read_buffer 65536
#define __ngs_number_of_reconnect_tries 1
#define __ngs_reconnect_interval_sec 5
#define __ngs_max_rejected_messages 5
#define __ngs_secret_key "ngs"

enum NGSConnectionState {
    CONNECTION_STATE_CLOSED = 0,
    CONNECTION_STATE_WS = 10,
    CONNECTION_STATE_SESSION_INIT = 11,
    CONNECTION_STATE_SERVER_FLUSH = 12,
    CONNECTION_STATE_CLIENT_FLUSH = 13,
    CONNECTION_STATE_FIRST_COMMUNICATION = 14,
    CONNECTION_STATE_REGISTERING = 15,
    CONNECTION_STATE_ACTIVE = 16,
    CONNECTION_STATE_RECONNECTING_WS = 17,
    CONNECTION_STATE_SESSION_END = 18,
};

enum NGSRequestType {
    REQUEST_SESSION_INIT = 100,
    REQUEST_SESSION_END = 110,
    REQUEST_FIRST_COMM = 101,
    REQUEST_SEND_SAVE = 108,
    
    REQUEST_GET_INSTALLATIONS = 232,
    REQUEST_SEND_REVIEW = 219,

    REQUEST_WS_PERSONAL_QUEUE = 300,
    REQUEST_WS_GLOBAL_QUEUE = 301,
    REQUEST_WS_PING = 310,

    // WORKERS
    REQUEST_PUSH_NOTIFICATION_TOKEN = 270,
    REQUEST_AB_TEST = 271,
    REQUEST_FETCH_SAVE = 283,
    REQUEST_RESTORE_SAVE_GC = 284,
    REQUEST_ANALYTICS_EVENT = 1100,
    
    REQUEST_UPDATE_GEMS = 282,
    REQUEST_INIT_SUBSCRIPTION = 286,

    REQUEST_UPDATE_FB_GC = 217,
    REQUEST_SOCIAL_UNREGISTER_PLAYER = 222,
    REQUEST_SOCIAL_SEND_FRIEND_REQUEST = 202,
    REQUEST_SOCIAL_ACCEPT_FRIEND_REQUEST = 203,
    REQUEST_SOCIAL_GET_VISIT_SAVE_GAME = 205,
    REQUEST_SOCIAL_REJECT_FRIEND_REQUEST = 206,
    REQUEST_SOCIAL_REMOVE_FRIEND = 207,
    REQUEST_SOCIAL_GET_OLD_FRIENDS = 208,
    REQUEST_FETCH_STORY_REVIEWS = 287,
    REQUEST_NEWS_FEED_EVENT = 220,
    REQUEST_GET_NEWS_FEED = 221,
    
    REQUEST_USER_SEGMENT = 295
};

enum NGSResponseType {
    RESPONSE_NO_RESPONSE_TYPE = 0,

    // WEBSERVER
    RESPONSE_SESSION_INIT = 100,
    RESPONSE_SESSION_END_ACCEPTED = 110,
    RESPONSE_OTHER_DEVICE = 601,
    
    // WORKER
    RESPONSE_SESSION_START = 101,
    RESPONSE_FIRST_COMM_END = 102,
    RESPONSE_SESSION_END = 111,

    RESPONSE_UDID_UPDATED = 105,
    RESPONSE_IFA_UPDATED = 106,

    RESPONSE_EVENT_PROCESSED = 200,
    RESPONSE_USER_LOGGED_IN = 201,
    RESPONSE_USER_REGISTERED = 202,
    RESPONSE_FRIEND_REQUEST_SENT = 203,
    RESPONSE_FRIEND_REQUEST = 204,
    RESPONSE_FRIEND_REQUEST_ACCEPTED = 205,
    RESPONSE_FRIEND_REQUEST_ACCEPT = 206,
    RESPONSE_FRIENDS = 207,
    RESPONSE_VISIT_SAVE_GAME = 208,
    RESPONSE_FRIEND_REQUEST_REJECTED = 209,
    RESPONSE_FRIEND_REQUEST_REJECT = 210,
    RESPONSE_FRIEND_REMOVED = 211,
    RESPONSE_FRIEND_REMOVE = 212,
    RESPONSE_FRIEND_ADD = 216,
    RESPONSE_FB_GC_UPDATED = 217,
    

    RESPONSE_INSTALLATIONS = 231,

    RESPONSE_SAVE_GAME = 250,
    RESPONSE_PROFILES = 251,
    RESPONSE_PARAMETERS = 252,
    RESPONSE_DOWNLOADS = 253,

    RESPONSE_SAVE_GAME_WHOLE = 264,
    RESPONSE_SAVE_RESTORED_GC = 265,
    RESPONSE_AB_TEST = 271,
    RESPONSE_LAST_PLAYED = 272,
    RESPONSE_RESOURCE = 284,
    
    RESPONSE_STORY_REVIEWS = 287,
    RESPONSE_NEWS_FEED_EVENT = 220,
    RESPONSE_NEWS_FEED = 221,
    
    RESPONSE_WS_PONG = 300,

    RESPONSE_FYBER_WATCHED = 501,
    
    RESPONSE_CONNECTION_FAILED = 701,
    RESPONSE_INIT_SUBSCRIPTIONS = 286,

    RESPONSE_REJECTED = 999,
    
    RESPONSE_USER_SEGMENT = 295
};

enum NGSStatusType {
    STATUS_OK = 0,
    STATUS_EXCEPTION = 1,
    STATUS_DISALLOWED = 2,
    STATUS_UNKNOWN = 3,
    STATUS_NO_STATUS = 4,
    STATUS_DUPLICATE = 5,
    STATUS_TIMEOUT = 6,
    STATUS_NOT_FOUND = 7,
    STATUS_NO_FACEBOOK = 8,
    STATUS_SOCIAL_FAILED = 9,
    STATUS_TOO_MANY_FRIENDS = 10,
    STATUS_INCOMPATIBLE_STATE = 11,

    STATUS_USER_NOT_EXISTS = 100,
    STATUS_ALREADY_FRIENDS = 101,
    STATUS_FRIEND_REQUEST_ALREADY_SENT = 102,
    STATUS_RECEIVED_FRIEND_REQUEST_LIMIT = 103,
    STATUS_FRIEND_REQUEST_INVITING_SELF = 104,
    STATUS_FRIEND_REQUEST_ALREADY_RECEIVED = 105,
    STATUS_FRIEND_REQUEST_SEND_COOLDOWN = 106,
    STATUS_FRIEND_REQUEST_NOT_EXISTS = 107,
    STATUS_MAX_NUMBER_OF_FRIENDS = 108,
    STATUS_USERNAME_ALREADY_TAKEN = 109,
    STATUS_FRIEND_NOT_EXISTS = 110,
    STATUS_BAD_USERNAME = 114,
    STATUS_USER_SHOULD_REGISTER = 111,
    STATUS_NEW_LOGIN = 112,
    STATUS_UNKNOWN_LEADERBOARD = 113,
    STATUS_VERSION_OLD = 115,
    STATUS_LIMIT_REACHED = 116,
    STATUS_ALREADY_EXISTS = 117,
    STATUS_NO_DAILY_ACTIVE_MOVIE = 118,
    STATUS_OTHER_MAX_NUMBER_OF_FRIENDS = 120,
    STATUS_FAILED = 121,
};

enum NGSMessageType {
    MESSAGE_TYPE_NO_MESSAGE_TYPE = 0,

    MESSAGE_TYPE_CONTROL = 1,
    MESSAGE_TYPE_GAME = 2,
    MESSAGE_TYPE_WEBSERVER_RESPONSE = 3,
    MESSAGE_TYPE_WORKER_RESPONSE = 4,
};

enum NGSLeaderboaordType { LEADERBOARD_FANS = 0, LEADERBOARD_MOVIES_30_DAYS = 1, LEADERBOARD_RICHEST = 2 };

enum NGSLikeType {
    LIKE_CLOSET = 0,
    LIKE_MOVIE = 1,
};

class NGSTypesHelper
{
  public:
    static bool isEvent(NGSRequestType type)
    {
        switch(type) {
            case REQUEST_INIT_SUBSCRIPTION:
            case REQUEST_PUSH_NOTIFICATION_TOKEN:
            case REQUEST_UPDATE_GEMS:
            case REQUEST_ANALYTICS_EVENT:
            case REQUEST_UPDATE_FB_GC:
            case REQUEST_NEWS_FEED_EVENT:
                return true;

            default:
                return false;
        }
    }

    // we receive some responses from server without our request
    static bool isServerMessage(NGSResponseType type)
    {
        switch(type) {
            case RESPONSE_FRIEND_ADD:
            case RESPONSE_FRIEND_REMOVE:
            case RESPONSE_FRIEND_REQUEST:
            case RESPONSE_FRIEND_REQUEST_ACCEPT:
            case RESPONSE_FRIEND_REQUEST_REJECT:
            case RESPONSE_FRIEND_REQUEST_ACCEPTED:
            case RESPONSE_FRIEND_REQUEST_REJECTED:
            case RESPONSE_FRIEND_REMOVED:
            case RESPONSE_FYBER_WATCHED:
            case RESPONSE_NEWS_FEED_EVENT:
            case RESPONSE_RESOURCE:
                return true;

            default:
                return false;
        }
    }

    static bool shouldPersistMessageInCache(NGSRequestType type)
    {
        // don't persist REQUEST_UPDATE_SAVE (to prevent editing in cache.ngs)
        if( type == REQUEST_PUSH_NOTIFICATION_TOKEN)
            return false;

        return isEvent(type);
    }
};

#endif
