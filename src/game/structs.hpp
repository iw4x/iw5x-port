#pragma once

namespace game
{
	namespace native
	{
		typedef float vec_t;
		typedef vec_t vec2_t[2];
		typedef vec_t vec3_t[3];
		typedef vec_t vec4_t[4];

		enum bdLobbyErrorCode : uint32_t
		{
			BD_NO_ERROR = 0x0,
			BD_TOO_MANY_TASKS = 0x1,
			BD_NOT_CONNECTED = 0x2,
			BD_SEND_FAILED = 0x3,
			BD_HANDLE_TASK_FAILED = 0x4,
			BD_START_TASK_FAILED = 0x5,
			BD_RESULT_EXCEEDS_BUFFER_SIZE = 0x64,
			BD_ACCESS_DENIED = 0x65,
			BD_EXCEPTION_IN_DB = 0x66,
			BD_MALFORMED_TASK_HEADER = 0x67,
			BD_INVALID_ROW = 0x68,
			BD_EMPTY_ARG_LIST = 0x69,
			BD_PARAM_PARSE_ERROR = 0x6A,
			BD_PARAM_MISMATCHED_TYPE = 0x6B,
			BD_SERVICE_NOT_AVAILABLE = 0x6C,
			BD_CONNECTION_RESET = 0x6D,
			BD_INVALID_USER_ID = 0x6E,
			BD_LOBBY_PROTOCOL_VERSION_FAILURE = 0x6F,
			BD_LOBBY_INTERNAL_FAILURE = 0x70,
			BD_LOBBY_PROTOCOL_ERROR = 0x71,
			BD_LOBBY_FAILED_TO_DECODE_UTF8 = 0x72,
			BD_LOBBY_ASCII_EXPECTED = 0x73,
			BD_ASYNCHRONOUS_ERROR = 0xC8,
			BD_STREAMING_COMPLETE = 0xC9,
			BD_MEMBER_NO_PROPOSAL = 0x12C,
			BD_TEAMNAME_ALREADY_EXISTS = 0x12D,
			BD_MAX_TEAM_MEMBERSHIPS_LIMITED = 0x12E,
			BD_MAX_TEAM_OWNERSHIPS_LIMITED = 0x12F,
			BD_NOT_A_TEAM_MEMBER = 0x130,
			BD_INVALID_TEAM_ID = 0x131,
			BD_INVALID_TEAM_NAME = 0x132,
			BD_NOT_A_TEAM_OWNER = 0x133,
			BD_NOT_AN_ADMIN_OR_OWNER = 0x134,
			BD_MEMBER_PROPOSAL_EXISTS = 0x135,
			BD_MEMBER_EXISTS = 0x136,
			BD_TEAM_FULL = 0x137,
			BD_VULGAR_TEAM_NAME = 0x138,
			BD_TEAM_USERID_BANNED = 0x139,
			BD_TEAM_EMPTY = 0x13A,
			BD_INVALID_TEAM_PROFILE_QUERY_ID = 0x13B,
			BD_TEAMNAME_TOO_SHORT = 0x13C,
			BD_UNIQUE_PROFILE_DATA_EXISTS_ALREADY = 0x13D,
			BD_INVALID_LEADERBOARD_ID = 0x190,
			BD_INVALID_STATS_SET = 0x191,
			BD_EMPTY_STATS_SET_IGNORED = 0x193,
			BD_NO_DIRECT_ACCESS_TO_ARBITRATED_LBS = 0x194,
			BD_STATS_WRITE_PERMISSION_DENIED = 0x195,
			BD_STATS_WRITE_TYPE_DATA_TYPE_MISMATCH = 0x196,
			BD_NO_STATS_FOR_USER = 0x197,
			BD_INVALID_ACCESS_TO_UNRANKED_LB = 0x198,
			BD_INVALID_EXTERNAL_TITLE_ID = 0x199,
			BD_DIFFERENT_LEADERBOARD_SCHEMAS = 0x19A,
			BD_TOO_MANY_LEADERBOARDS_REQUESTED = 0x19B,
			BD_ENTITLEMENTS_ERROR = 0x19C,
			BD_ENTITLEMENTS_INVALID_TITLEID = 0x19D,
			BD_ENTITLEMENTS_INVALID_LEADERBOARDID = 0x19E,
			BD_ENTITLEMENTS_INVALID_GET_MODE_FOR_TITLE = 0x19F,
			BD_ENTITLEMENTS_URL_CONNECTION_ERROR = 0x1A0,
			BD_ENTITLEMENTS_CONFIG_ERROR = 0x1A1,
			BD_ENTITLEMENTS_NAMED_PARENT_ERROR = 0x1A2,
			BD_ENTITLEMENTS_NAMED_KEY_ERROR = 0x1A3,
			BD_TOO_MANY_ENTITY_IDS_REQUESTED = 0x1A4,
			BD_STATS_READ_FAILED = 0x1A5,
			BD_INVALID_TITLE_ID = 0x1F4,
			BD_MESSAGING_INVALID_MAIL_ID = 0x258,
			BD_SELF_BLOCK_NOT_ALLOWED = 0x259,
			BD_GLOBAL_MESSAGE_ACCESS_DENIED = 0x25A,
			BD_GLOBAL_MESSAGES_USER_LIMIT_EXCEEDED = 0x25B,
			BD_MESSAGING_SENDER_DOES_NOT_EXIST = 0x25C,
			BD_AUTH_NO_ERROR = 0x2BC,
			BD_AUTH_BAD_REQUEST = 0x2BD,
			BD_AUTH_SERVER_CONFIG_ERROR = 0x2BE,
			BD_AUTH_BAD_TITLE_ID = 0x2BF,
			BD_AUTH_BAD_ACCOUNT = 0x2C0,
			BD_AUTH_ILLEGAL_OPERATION = 0x2C1,
			BD_AUTH_INCORRECT_LICENSE_CODE = 0x2C2,
			BD_AUTH_CREATE_USERNAME_EXISTS = 0x2C3,
			BD_AUTH_CREATE_USERNAME_ILLEGAL = 0x2C4,
			BD_AUTH_CREATE_USERNAME_VULGAR = 0x2C5,
			BD_AUTH_CREATE_MAX_ACC_EXCEEDED = 0x2C6,
			BD_AUTH_MIGRATE_NOT_SUPPORTED = 0x2C7,
			BD_AUTH_TITLE_DISABLED = 0x2C8,
			BD_AUTH_ACCOUNT_EXPIRED = 0x2C9,
			BD_AUTH_ACCOUNT_LOCKED = 0x2CA,
			BD_AUTH_UNKNOWN_ERROR = 0x2CB,
			BD_AUTH_INCORRECT_PASSWORD = 0x2CC,
			BD_AUTH_IP_NOT_IN_ALLOWED_RANGE = 0x2CD,
			BD_AUTH_WII_TOKEN_VERIFICATION_FAILED = 0x2CE,
			BD_AUTH_WII_AUTHENTICATION_FAILED = 0x2CF,
			BD_AUTH_IP_KEY_LIMIT_REACHED = 0x2D0,
			BD_AUTH_INVALID_GSPID = 0x2D1,
			BD_AUTH_INVALID_IP_RANGE_ID = 0x2D2,
			BD_AUTH_3DS_TOKEN_VERIFICATION_FAILED = 0x2D1,
			BD_AUTH_3DS_AUTHENTICATION_FAILED = 0x2D2,
			BD_AUTH_STEAM_APP_ID_MISMATCH = 0x2D3,
			BD_AUTH_ABACCOUNTS_APP_ID_MISMATCH = 0x2D4,
			BD_AUTH_CODO_USERNAME_NOT_SET = 0x2D5,
			BD_AUTH_WIIU_TOKEN_VERIFICATION_FAILED = 0x2D6,
			BD_AUTH_WIIU_AUTHENTICATION_FAILED = 0x2D7,
			BD_AUTH_CODO_USERNAME_NOT_BASE64 = 0x2D8,
			BD_AUTH_CODO_USERNAME_NOT_UTF8 = 0x2D9,
			BD_AUTH_TENCENT_TICKET_EXPIRED = 0x2DA,
			BD_AUTH_PS3_SERVICE_ID_MISMATCH = 0x2DB,
			BD_AUTH_CODOID_NOT_WHITELISTED = 0x2DC,
			BD_AUTH_PLATFORM_TOKEN_ERROR = 0x2DD,
			BD_AUTH_JSON_FORMAT_ERROR = 0x2DE,
			BD_AUTH_REPLY_CONTENT_ERROR = 0x2DF,
			BD_AUTH_THIRD_PARTY_TOKEN_EXPIRED = 0x2E0,
			BD_AUTH_CONTINUING = 0x2E1,
			BD_AUTH_PLATFORM_DEVICE_ID_ERROR = 0x2E4,
			BD_NO_PROFILE_INFO_EXISTS = 0x320,
			BD_FRIENDSHIP_NOT_REQUSTED = 0x384,
			BD_NOT_A_FRIEND = 0x385,
			BD_SELF_FRIENDSHIP_NOT_ALLOWED = 0x387,
			BD_FRIENDSHIP_EXISTS = 0x388,
			BD_PENDING_FRIENDSHIP_EXISTS = 0x389,
			BD_USERID_BANNED = 0x38A,
			BD_FRIENDS_FULL = 0x38C,
			BD_FRIENDS_NO_RICH_PRESENCE = 0x38D,
			BD_RICH_PRESENCE_TOO_LARGE = 0x38E,
			BD_NO_FILE = 0x3E8,
			BD_PERMISSION_DENIED = 0x3E9,
			BD_FILESIZE_LIMIT_EXCEEDED = 0x3EA,
			BD_FILENAME_MAX_LENGTH_EXCEEDED = 0x3EB,
			BD_EXTERNAL_STORAGE_SERVICE_ERROR = 0x3EC,
			BD_CHANNEL_DOES_NOT_EXIST = 0x44D,
			BD_CHANNEL_ALREADY_SUBSCRIBED = 0x44E,
			BD_CHANNEL_NOT_SUBSCRIBED = 0x44F,
			BD_CHANNEL_FULL = 0x450,
			BD_CHANNEL_SUBSCRIPTIONS_FULL = 0x451,
			BD_CHANNEL_NO_SELF_WHISPERING = 0x452,
			BD_CHANNEL_ADMIN_REQUIRED = 0x453,
			BD_CHANNEL_TARGET_NOT_SUBSCRIBED = 0x454,
			BD_CHANNEL_REQUIRES_PASSWORD = 0x455,
			BD_CHANNEL_TARGET_IS_SELF = 0x456,
			BD_CHANNEL_PUBLIC_BAN_NOT_ALLOWED = 0x457,
			BD_CHANNEL_USER_BANNED = 0x458,
			BD_CHANNEL_PUBLIC_PASSWORD_NOT_ALLOWED = 0x459,
			BD_CHANNEL_PUBLIC_KICK_NOT_ALLOWED = 0x45A,
			BD_CHANNEL_MUTED = 0x45B,
			BD_EVENT_DESC_TRUNCATED = 0x4B0,
			BD_CONTENT_UNLOCK_UNKNOWN_ERROR = 0x514,
			BD_UNLOCK_KEY_INVALID = 0x515,
			BD_UNLOCK_KEY_ALREADY_USED_UP = 0x516,
			BD_SHARED_UNLOCK_LIMIT_REACHED = 0x517,
			BD_DIFFERENT_HARDWARE_ID = 0x518,
			BD_INVALID_CONTENT_OWNER = 0x519,
			BD_CONTENT_UNLOCK_INVALID_USER = 0x51A,
			BD_CONTENT_UNLOCK_INVALID_CATEGORY = 0x51B,
			BD_KEY_ARCHIVE_INVALID_WRITE_TYPE = 0x5DC,
			BD_KEY_ARCHIVE_EXCEEDED_MAX_IDS_PER_REQUEST = 0x5DD,
			BD_BANDWIDTH_TEST_TRY_AGAIN = 0x712,
			BD_BANDWIDTH_TEST_STILL_IN_PROGRESS = 0x713,
			BD_BANDWIDTH_TEST_NOT_PROGRESS = 0x714,
			BD_BANDWIDTH_TEST_SOCKET_ERROR = 0x715,
			BD_INVALID_SESSION_NONCE = 0x76D,
			BD_ARBITRATION_FAILURE = 0x76F,
			BD_ARBITRATION_USER_NOT_REGISTERED = 0x771,
			BD_ARBITRATION_NOT_CONFIGURED = 0x772,
			BD_CONTENTSTREAMING_FILE_NOT_AVAILABLE = 0x7D0,
			BD_CONTENTSTREAMING_STORAGE_SPACE_EXCEEDED = 0x7D1,
			BD_CONTENTSTREAMING_NUM_FILES_EXCEEDED = 0x7D2,
			BD_CONTENTSTREAMING_UPLOAD_BANDWIDTH_EXCEEDED = 0x7D3,
			BD_CONTENTSTREAMING_FILENAME_MAX_LENGTH_EXCEEDED = 0x7D4,
			BD_CONTENTSTREAMING_MAX_THUMB_DATA_SIZE_EXCEEDED = 0x7D5,
			BD_CONTENTSTREAMING_DOWNLOAD_BANDWIDTH_EXCEEDED = 0x7D6,
			BD_CONTENTSTREAMING_NOT_ENOUGH_DOWNLOAD_BUFFER_SPACE = 0x7D7,
			BD_CONTENTSTREAMING_SERVER_NOT_CONFIGURED = 0x7D8,
			BD_CONTENTSTREAMING_INVALID_APPLE_RECEIPT = 0x7DA,
			BD_CONTENTSTREAMING_APPLE_STORE_NOT_AVAILABLE = 0x7DB,
			BD_CONTENTSTREAMING_APPLE_RECEIPT_FILENAME_MISMATCH = 0x7DC,
			BD_CONTENTSTREAMING_HTTP_ERROR = 0x7E4,
			BD_CONTENTSTREAMING_FAILED_TO_START_HTTP = 0x7E5,
			BD_CONTENTSTREAMING_LOCALE_INVALID = 0x7E6,
			BD_CONTENTSTREAMING_LOCALE_MISSING = 0x7E7,
			BD_VOTERANK_ERROR_EMPTY_RATING_SUBMISSION = 0x7EE,
			BD_VOTERANK_ERROR_MAX_VOTES_EXCEEDED = 0x7EF,
			BD_VOTERANK_ERROR_INVALID_RATING = 0x7F0,
			BD_MAX_NUM_TAGS_EXCEEDED = 0x82A,
			BD_TAGGED_COLLECTION_DOES_NOT_EXIST = 0x82B,
			BD_EMPTY_TAG_ARRAY = 0x82C,
			BD_INVALID_QUERY_ID = 0x834,
			BD_NO_ENTRY_TO_UPDATE = 0x835,
			BD_SESSION_INVITE_EXISTS = 0x836,
			BD_INVALID_SESSION_ID = 0x837,
			BD_ATTACHMENT_TOO_LARGE = 0x838,
			BD_INVALID_GROUP_ID = 0xAF0,
			BD_MAIL_INVALID_MAIL_ID_ERROR = 0xB55,
			BD_UCD_SERVICE_ERROR = 0xC80,
			BD_UCD_SERVICE_DISABLED = 0xC81,
			BD_UCD_UNINTIALIZED_ERROR = 0xC82,
			BD_UCD_ACCOUNT_ALREADY_REGISTERED = 0xC83,
			BD_UCD_ACCOUNT_NOT_REGISTERED = 0xC84,
			BD_UCD_AUTH_ATTEMPT_FAILED = 0xC85,
			BD_UCD_ACCOUNT_LINKING_ERROR = 0xC86,
			BD_UCD_ENCRYPTION_ERROR = 0xC87,
			BD_UCD_ACCOUNT_DATA_INVALID = 0xC88,
			BD_UCD_ACCOUNT_DATA_INVALID_FIRSTNAME = 0xC89,
			BD_UCD_ACCOUNT_DATA_INVALID_LASTNAME = 0xC8A,
			BD_UCD_ACCOUNT_DATA_INVALID_DOB = 0xC8B,
			BD_UCD_ACCOUNT_DATA_INVALID_EMAIL = 0xC8C,
			BD_UCD_ACCOUNT_DATA_INVALID_COUNTRY = 0xC8D,
			BD_UCD_ACCOUNT_DATA_INVALID_POSTCODE = 0xC8E,
			BD_UCD_ACCOUNT_DATA_INVALID_PASSWORD = 0xC8F,
			BD_UCD_ACCOUNT_NAME_ALREADY_RESISTERED = 0xC94,
			BD_UCD_ACCOUNT_EMAIL_ALREADY_RESISTERED = 0xC95,
			BD_UCD_GUEST_ACCOUNT_AUTH_CONFLICT = 0xC96,
			BD_TWITCH_SERVICE_ERROR = 0xC1D,
			BD_TWITCH_ACCOUNT_ALREADY_LINKED = 0xC1E,
			BD_TWITCH_NO_LINKED_ACCOUNT = 0xC1F,
			BD_YOUTUBE_SERVICE_ERROR = 0xCE5,
			BD_YOUTUBE_SERVICE_COMMUNICATION_ERROR = 0xCE6,
			BD_YOUTUBE_USER_DENIED_AUTHORIZATION = 0xCE7,
			BD_YOUTUBE_AUTH_MAX_TIME_EXCEEDED = 0xCE8,
			BD_YOUTUBE_USER_UNAUTHORIZED = 0xCE9,
			BD_YOUTUBE_UPLOAD_MAX_TIME_EXCEEDED = 0xCEA,
			BD_YOUTUBE_DUPLICATE_UPLOAD = 0xCEB,
			BD_YOUTUBE_FAILED_UPLOAD = 0xCEC,
			BD_YOUTUBE_ACCOUNT_ALREADY_REGISTERED = 0xCED,
			BD_YOUTUBE_ACCOUNT_NOT_REGISTERED = 0xCEE,
			BD_YOUTUBE_CONTENT_SERVER_ERROR = 0xCEF,
			BD_YOUTUBE_UPLOAD_DOES_NOT_EXIST = 0xCF0,
			BD_YOUTUBE_NO_LINKED_ACCOUNT = 0xCF1,
			BD_YOUTUBE_DEVELOPER_TAGS_INVALID = 0xCF2,
			BD_TWITTER_AUTH_ATTEMPT_FAILED = 0xDAD,
			BD_TWITTER_AUTH_TOKEN_INVALID = 0xDAE,
			BD_TWITTER_UPDATE_LIMIT_REACHED = 0xDAF,
			BD_TWITTER_UNAVAILABLE = 0xDB0,
			BD_TWITTER_ERROR = 0xDB1,
			BD_TWITTER_TIMED_OUT = 0xDB2,
			BD_TWITTER_DISABLED_FOR_USER = 0xDB3,
			BD_TWITTER_ACCOUNT_AMBIGUOUS = 0xDB4,
			BD_TWITTER_MAXIMUM_ACCOUNTS_REACHED = 0xDB5,
			BD_TWITTER_ACCOUNT_NOT_REGISTERED = 0xDB6,
			BD_TWITTER_DUPLICATE_STATUS = 0xDB7,
			BD_TWITTER_ACCOUNT_ALREADY_REGISTERED = 0xE1C,
			BD_FACEBOOK_AUTH_ATTEMPT_FAILED = 0xE11,
			BD_FACEBOOK_AUTH_TOKEN_INVALID = 0xE12,
			BD_FACEBOOK_PHOTO_DOES_NOT_EXIST = 0xE13,
			BD_FACEBOOK_PHOTO_INVALID = 0xE14,
			BD_FACEBOOK_PHOTO_ALBUM_FULL = 0xE15,
			BD_FACEBOOK_UNAVAILABLE = 0xE16,
			BD_FACEBOOK_ERROR = 0xE17,
			BD_FACEBOOK_TIMED_OUT = 0xE18,
			BD_FACEBOOK_DISABLED_FOR_USER = 0xE19,
			BD_FACEBOOK_ACCOUNT_AMBIGUOUS = 0xE1A,
			BD_FACEBOOK_MAXIMUM_ACCOUNTS_REACHED = 0xE1B,
			BD_FACEBOOK_INVALID_NUM_PICTURES_REQUESTED = 0xE1C,
			BD_FACEBOOK_VIDEO_DOES_NOT_EXIST = 0xE1D,
			BD_FACEBOOK_ACCOUNT_ALREADY_REGISTERED = 0xE1E,
			BD_APNS_INVALID_PAYLOAD = 0xE74,
			BD_APNS_INVALID_TOKEN_LENGTH_ERROR = 0xE76,
			BD_MAX_CONSOLEID_LENGTH_EXCEEDED = 0xEE1,
			BD_MAX_WHITELIST_LENGTH_EXCEEDED = 0xEE2,
			BD_USERGROUP_NAME_ALREADY_EXISTS = 0x1770,
			BD_INVALID_USERGROUP_ID = 0x1771,
			BD_USER_ALREADY_IN_USERGROUP = 0x1772,
			BD_USER_NOT_IN_USERGROUP = 0x1773,
			BD_INVALID_USERGROUP_MEMBER_TYPE = 0x1774,
			BD_TOO_MANY_MEMBERS_REQUESTED = 0x1775,
			BD_USERGROUP_NAME_TOO_SHORT = 0x1776,
			BD_RICH_PRESENCE_DATA_TOO_LARGE = 0x1A90,
			BD_RICH_PRESENCE_TOO_MANY_USERS = 0x1A91,
			BD_PRESENCE_DATA_TOO_LARGE = 0x283C,
			BD_PRESENCE_TOO_MANY_USERS = 0x283D,
			BD_USER_LOGGED_IN_OTHER_TITLE = 0x283E,
			BD_USER_NOT_LOGGED_IN = 0x283F,
			BD_SUBSCRIPTION_TOO_MANY_USERS = 0x1B58,
			BD_SUBSCRIPTION_TICKET_PARSE_ERROR = 0x1B59,
			BD_CODO_ID_INVALID_DATA = 0x1BBC,
			BD_INVALID_MESSAGE_FORMAT = 0x1BBD,
			BD_TLOG_TOO_MANY_MESSAGES = 0x1BBE,
			BD_CODO_ID_NOT_IN_WHITELIST = 0x1BBF,
			BD_TLOG_MESSAGE_TRANSFORMATION_ERROR = 0x1BC0,
			BD_REWARDS_NOT_ENABLED = 0x1BC1,
			BD_MARKETPLACE_ERROR = 0x1F40,
			BD_MARKETPLACE_RESOURCE_NOT_FOUND = 0x1F41,
			BD_MARKETPLACE_INVALID_CURRENCY = 0x1F42,
			BD_MARKETPLACE_INVALID_PARAMETER = 0x1F43,
			BD_MARKETPLACE_RESOURCE_CONFLICT = 0x1F44,
			BD_MARKETPLACE_STORAGE_ERROR = 0x1F45,
			BD_MARKETPLACE_INTEGRITY_ERROR = 0x1F46,
			BD_MARKETPLACE_INSUFFICIENT_FUNDS_ERROR = 0x1F47,
			BD_MARKETPLACE_MMP_SERVICE_ERROR = 0x1F48,
			BD_MARKETPLACE_PRECONDITION_REQUIRED = 0x1F49,
			BD_MARKETPLACE_ITEM_MULTIPLE_PURCHASE_ERROR = 0x1F4A,
			BD_MARKETPLACE_MISSING_REQUIRED_ENTITLEMENT = 0x1F4B,
			BD_MARKETPLACE_VALIDATION_ERROR = 0x1F4C,
			BD_MARKETPLACE_TENCENT_PAYMENT_ERROR = 0x1F4D,
			BD_MARKETPLACE_SKU_NOT_COUPON_ENABLED_ERROR = 0x1F4E,
			BD_LEAGUE_INVALID_TEAM_SIZE = 0x1FA4,
			BD_LEAGUE_INVALID_TEAM = 0x1FA5,
			BD_LEAGUE_INVALID_SUBDIVISION = 0x1FA6,
			BD_LEAGUE_INVALID_LEAGUE = 0x1FA7,
			BD_LEAGUE_TOO_MANY_RESULTS_REQUESTED = 0x1FA8,
			BD_LEAGUE_METADATA_TOO_LARGE = 0x1FA9,
			BD_LEAGUE_TEAM_ICON_TOO_LARGE = 0x1FAA,
			BD_LEAGUE_TEAM_NAME_TOO_LONG = 0x1FAB,
			BD_LEAGUE_ARRAY_SIZE_MISMATCH = 0x1FAC,
			BD_LEAGUE_SUBDIVISION_MISMATCH = 0x2008,
			BD_LEAGUE_INVALID_WRITE_TYPE = 0x2009,
			BD_LEAGUE_INVALID_STATS_DATA = 0x200A,
			BD_LEAGUE_SUBDIVISION_UNRANKED = 0x200B,
			BD_LEAGUE_CROSS_TEAM_STATS_WRITE_PREVENTED = 0x200C,
			BD_LEAGUE_INVALID_STATS_SEASON = 0x200D,
			BD_COMMERCE_ERROR = 0x206C,
			BD_COMMERCE_RESOURCE_NOT_FOUND = 0x206D,
			BD_COMMERCE_STORAGE_INVALID_PARAMETER = 0x206E,
			BD_COMMERCE_APPLICATION_INVALID_PARAMETER = 0x206F,
			BD_COMMERCE_RESOURCE_CONFLICT = 0x2070,
			BD_COMMERCE_STORAGE_ERROR = 0x2071,
			BD_COMMERCE_INTEGRITY_ERROR = 0x2072,
			BD_COMMERCE_MMP_SERVICE_ERROR = 0x2073,
			BD_COMMERCE_PERMISSION_DENIED = 0x2074,
			BD_COMMERCE_INSUFFICIENT_FUNDS_ERROR = 0x2075,
			BD_COMMERCE_UNKNOWN_CURRENCY = 0x2076,
			BD_COMMERCE_INVALID_RECEIPT = 0x2077,
			BD_COMMERCE_RECEIPT_USED = 0x2078,
			BD_COMMERCE_TRANSACTION_ALREADY_APPLIED = 0x2079,
			BD_COMMERCE_INVALID_CURRENCY_TYPE = 0x207A,
			BD_CONNECTION_COUNTER_ERROR = 0x20D0,
			BD_LINKED_ACCOUNTS_INVALID_CONTEXT = 0x2198,
			BD_LINKED_ACCOUNTS_INVALID_PLATFORM = 0x2199,
			BD_LINKED_ACCOUNTS_LINKED_ACCOUNTS_FETCH_ERROR = 0x219A,
			BD_LINKED_ACCOUNTS_INVALID_ACCOUNT = 0x219B,
			BD_GMSG_INVALID_CATEGORY_ID = 0x27D8,
			BD_GMSG_CATEGORY_MEMBERSHIPS_LIMIT = 0x27D9,
			BD_GMSG_NONMEMBER_POST_DISALLOWED = 0x27DA,
			BD_GMSG_CATEGORY_DISALLOWS_CLIENT_TYPE = 0x27DB,
			BD_GMSG_PAYLOAD_TOO_BIG = 0x27DC,
			BD_GMSG_MEMBER_POST_DISALLOWED = 0x27DD,
			BD_GMSG_OVERLOADED = 0x27DE,
			BD_GMSG_USER_PERCATEGORY_POST_RATE_EXCEEDED = 0x27DF,
			BD_GMSG_USER_GLOBAL_POST_RATE_EXCEEDED = 0x27E0,
			BD_GMSG_GROUP_POST_RATE_EXCEEDED = 0x27E1,
			BD_MAX_ERROR_CODE = 0x27E2,
		};

		enum bdNATType : uint8_t
		{
			BD_NAT_UNKNOWN = 0x0,
			BD_NAT_OPEN = 0x1,
			BD_NAT_MODERATE = 0x2,
			BD_NAT_STRICT = 0x3,
		};

#pragma pack(push)
#pragma warning(disable: 1)
		struct bdAuthTicket
		{
			unsigned int m_magicNumber;
			char m_type;
			unsigned int m_titleID;
			unsigned int m_timeIssued;
			unsigned int m_timeExpires;
			unsigned __int64 m_licenseID;
			unsigned __int64 m_userID;
			char m_username[64];
			char m_sessionKey[24];
			char m_usingHashMagicNumber[3];
			char m_hash[4];
		};
#pragma pack(pop)

		enum XAssetType
		{
			ASSET_TYPE_PHYSPRESET = 0x0,
			ASSET_TYPE_PHYSCOLLMAP = 0x1,
			ASSET_TYPE_XANIMPARTS = 0x2,
			ASSET_TYPE_XMODEL_SURFS = 0x3,
			ASSET_TYPE_XMODEL = 0x4,
			ASSET_TYPE_MATERIAL = 0x5,
			ASSET_TYPE_PIXELSHADER = 0x6,
			ASSET_TYPE_VERTEXSHADER = 0x7,
			ASSET_TYPE_VERTEXDECL = 0x8,
			ASSET_TYPE_TECHNIQUE_SET = 0x9,
			ASSET_TYPE_IMAGE = 0xA,
			ASSET_TYPE_SOUND = 0xB,
			ASSET_TYPE_SOUND_CURVE = 0xC,
			ASSET_TYPE_LOADED_SOUND = 0xD,
			ASSET_TYPE_CLIPMAP = 0xE,
			ASSET_TYPE_COMWORLD = 0xF,
			ASSET_TYPE_GLASSWORLD = 0x10,
			ASSET_TYPE_PATHDATA = 0x11,
			ASSET_TYPE_VEHICLE_TRACK = 0x12,
			ASSET_TYPE_MAP_ENTS = 0x13,
			ASSET_TYPE_FXWORLD = 0x14,
			ASSET_TYPE_GFXWORLD = 0x15,
			ASSET_TYPE_LIGHT_DEF = 0x16,
			ASSET_TYPE_UI_MAP = 0x17,
			ASSET_TYPE_FONT = 0x18,
			ASSET_TYPE_MENULIST = 0x19,
			ASSET_TYPE_MENU = 0x1A,
			ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
			ASSET_TYPE_ATTACHMENT = 0x1C,
			ASSET_TYPE_WEAPON = 0x1D,
			ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1E,
			ASSET_TYPE_FX = 0x1F,
			ASSET_TYPE_IMPACT_FX = 0x20,
			ASSET_TYPE_SURFACE_FX = 0x21,
			ASSET_TYPE_AITYPE = 0x22,
			ASSET_TYPE_MPTYPE = 0x23,
			ASSET_TYPE_CHARACTER = 0x24,
			ASSET_TYPE_XMODELALIAS = 0x25,
			ASSET_TYPE_RAWFILE = 0x26,
			ASSET_TYPE_SCRIPTFILE = 0x27,
			ASSET_TYPE_STRINGTABLE = 0x28,
			ASSET_TYPE_LEADERBOARD = 0x29,
			ASSET_TYPE_STRUCTURED_DATA_DEF = 0x2A,
			ASSET_TYPE_TRACER = 0x2B,
			ASSET_TYPE_VEHICLE = 0x2C,
			ASSET_TYPE_ADDON_MAP_ENTS = 0x2D,
			ASSET_TYPE_COUNT,

			ASSET_TYPE_STRING = ASSET_TYPE_COUNT,
			ASSET_TYPE_ASSETLIST = 0x2F,

			ASSET_TYPE_FULLCOUNT
		};

		struct ScriptFile
		{
			const char* name;
			int compressedLen;
			unsigned int len;
			int bytecodeLen;
			unsigned char* buffer;
			unsigned char* bytecode;
		};

		struct RawFile
		{
			const char* name;
			unsigned int compressedLen;
			unsigned int len;
			char* buffer;
		};

		struct PhysPreset;
		struct PhysCollmap;
		struct XAnimParts;
		struct XModelSurfs;
		struct XModel;
		struct Material;
		struct MaterialPixelShader;
		struct MaterialVertexShader;
		struct MaterialVertexDeclaration;
		struct MaterialTechniqueSet;
		struct GfxImage;
		struct snd_alias_list_t;
		struct SndCurve;
		struct LoadedSound;
		struct clipMap_t;
		struct ComWorld;
		struct GlassWorld;
		struct PathData;
		struct VehicleTrack;
		struct MapEnts;
		struct FxWorld;
		struct GfxWorld;
		struct GfxLightDef;
		struct Font_s;
		struct MenuList;
		struct menuDef_t;
		struct WeaponAttachment;
		struct WeaponCompleteDef;
		struct FxEffectDef;
		struct FxImpactTable;
		struct SurfaceFxTable;
		struct RawFile;
		struct ScriptFile;
		struct StringTable;
		struct LeaderboardDef;
		struct StructuredDataDefSet;
		struct TracerDef;
		struct VehicleDef;
		struct AddonMapEnts;

		struct LocalizeEntry
		{
			const char* value;
			const char* name;
		};

		struct StringTableCell
		{
			const char* string;
			int hash;
		};

		struct StringTable
		{
			const char* name;
			int columnCount;
			int rowCount;
			StringTableCell* values;
		};

		struct TracerDef
		{
			const char* name;
			Material* material;
			unsigned int drawInterval;
			float speed;
			float beamLength;
			float beamWidth;
			float screwRadius;
			float screwDist;
			float colors[5][4];
		};

		enum activeReticleType_t
		{
			VEH_ACTIVE_RETICLE_NONE = 0x0,
			VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
			VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
			VEH_ACTIVE_RETICLE_COUNT = 0x3,
		};

		enum weapType_t
		{
			WEAPTYPE_NONE = 0x0,
			WEAPTYPE_BULLET = 0x1,
			WEAPTYPE_GRENADE = 0x2,
			WEAPTYPE_PROJECTILE = 0x3,
			WEAPTYPE_RIOTSHIELD = 0x4,
			WEAPTYPE_NUM = 0x5
		};

		enum weapClass_t
		{
			WEAPCLASS_RIFLE = 0x0,
			WEAPCLASS_SNIPER = 0x1,
			WEAPCLASS_MG = 0x2,
			WEAPCLASS_SMG = 0x3,
			WEAPCLASS_SPREAD = 0x4,
			WEAPCLASS_PISTOL = 0x5,
			WEAPCLASS_GRENADE = 0x6,
			WEAPCLASS_ROCKETLAUNCHER = 0x7,
			WEAPCLASS_TURRET = 0x8,
			WEAPCLASS_THROWINGKNIFE = 0x9,
			WEAPCLASS_NON_PLAYER = 0xA,
			WEAPCLASS_ITEM = 0xB,
			WEAPCLASS_NUM = 0xC,
		};


		enum PenetrateType
		{
			PENETRATE_TYPE_NONE = 0x0,
			PENETRATE_TYPE_SMALL = 0x1,
			PENETRATE_TYPE_MEDIUM = 0x2,
			PENETRATE_TYPE_LARGE = 0x3,
			PENETRATE_TYPE_COUNT = 0x4
		};

		enum ImpactType
		{
			IMPACT_TYPE_NONE = 0x0,
			IMPACT_TYPE_BULLET_SMALL = 0x1,
			IMPACT_TYPE_BULLET_LARGE = 0x2,
			IMPACT_TYPE_BULLET_AP = 0x3,
			IMPACT_TYPE_BULLET_EXPLODE = 0x4,
			IMPACT_TYPE_SHOTGUN = 0x5,
			IMPACT_TYPE_SHOTGUN_EXPLODE = 0x6,
			IMPACT_TYPE_GRENADE_BOUNCE = 0x7,
			IMPACT_TYPE_GRENADE_EXPLODE = 0x8,
			IMPACT_TYPE_ROCKET_EXPLODE = 0x9,
			IMPACT_TYPE_PROJECTILE_DUD = 0xA,
			IMPACT_TYPE_COUNT = 0xB
		};

		enum weapInventoryType_t
		{
			WEAPINVENTORY_PRIMARY = 0x0,
			WEAPINVENTORY_OFFHAND = 0x1,
			WEAPINVENTORY_ITEM = 0x2,
			WEAPINVENTORY_ALTMODE = 0x3,
			WEAPINVENTORY_EXCLUSIVE = 0x4,
			WEAPINVENTORY_SCAVENGER = 0x5,
			WEAPINVENTORYCOUNT = 0x6
		};

		enum weapFireType_t
		{
			WEAPON_FIRETYPE_FULLAUTO = 0x0,
			WEAPON_FIRETYPE_SINGLESHOT = 0x1,
			WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
			WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
			WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
			WEAPON_FIRETYPE_DOUBLEBARREL = 0x5,
			WEAPON_FIRETYPECOUNT = 0x6,
			WEAPON_FIRETYPE_BURSTFIRE_FIRST = 0x2,
			WEAPON_FIRETYPE_BURSTFIRE_LAST = 0x4,
		};

		enum OffhandClass
		{
			OFFHAND_CLASS_NONE = 0x0,
			OFFHAND_CLASS_FRAG_GRENADE = 0x1,
			OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
			OFFHAND_CLASS_FLASH_GRENADE = 0x3,
			OFFHAND_CLASS_THROWINGKNIFE = 0x4,
			OFFHAND_CLASS_OTHER = 0x5,
			OFFHAND_CLASS_COUNT = 0x6,
		};

		enum weapStance_t
		{
			WEAPSTANCE_STAND = 0x0,
			WEAPSTANCE_DUCK = 0x1,
			WEAPSTANCE_PRONE = 0x2,
			WEAPSTANCE_NUM = 0x3
		};

		enum weaponIconRatioType_t
		{
			WEAPON_ICON_RATIO_1TO1 = 0x0,
			WEAPON_ICON_RATIO_2TO1 = 0x1,
			WEAPON_ICON_RATIO_4TO1 = 0x2,
			WEAPON_ICON_RATIO_COUNT = 0x3
		};

		enum ammoCounterClipType_t
		{
			AMMO_COUNTER_CLIP_NONE = 0x0,
			AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
			AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
			AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
			AMMO_COUNTER_CLIP_ROCKET = 0x4,
			AMMO_COUNTER_CLIP_BELTFED = 0x5,
			AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
			AMMO_COUNTER_CLIP_COUNT = 0x7
		};


		struct StateTimers
		{
			int iFireDelay;
			int iMeleeDelay;
			int meleeChargeDelay;
			int iDetonateDelay;
			int iRechamberTime;
			int rechamberTimeOneHanded;
			int iRechamberBoltTime;
			int iHoldFireTime;
			int iDetonateTime;
			int iMeleeTime;
			int meleeChargeTime;
			int iReloadTime;
			int reloadShowRocketTime;
			int iReloadEmptyTime;
			int iReloadAddTime;
			int iReloadStartTime;
			int iReloadStartAddTime;
			int iReloadEndTime;
			int iDropTime;
			int iRaiseTime;
			int iAltDropTime;
			int quickDropTime;
			int quickRaiseTime;
			int iBreachRaiseTime;
			int iEmptyRaiseTime;
			int iEmptyDropTime;
			int sprintInTime;
			int sprintLoopTime;
			int sprintOutTime;
			int stunnedTimeBegin;
			int stunnedTimeLoop;
			int stunnedTimeEnd;
			int nightVisionWearTime;
			int nightVisionWearTimeFadeOutEnd;
			int nightVisionWearTimePowerUp;
			int nightVisionRemoveTime;
			int nightVisionRemoveTimePowerDown;
			int nightVisionRemoveTimeFadeInStart;
			int fuseTime;
			int aiFuseTime;
			int blastFrontTime;
			int blastRightTime;
			int blastBackTime;
			int blastLeftTime;
			int raiseInterruptableTime;
			int firstRaiseInterruptableTime;
			int reloadInterruptableTime;
			int reloadEmptyInterruptableTime;
			int fireInterruptableTime;
		};

		enum weapOverlayReticle_t
		{
			WEAPOVERLAYRETICLE_NONE = 0x0,
			WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
			WEAPOVERLAYRETICLE_NUM = 0x2
		};

		struct ADSOverlay
		{
			Material* shader;
			Material* shaderLowRes;
			Material* shaderEMP;
			Material* shaderEMPLowRes;
			weapOverlayReticle_t reticle;
			float width;
			float height;
			float widthSplitscreen;
			float heightSplitscreen;
		};

		enum WeapOverlayInteface_t
		{
			WEAPOVERLAYINTERFACE_NONE = 0x0,
			WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
			WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
			WEAPOVERLAYINTERFACECOUNT = 0x3
		};

		enum weapProjExposion_t
		{
			WEAPPROJEXP_GRENADE = 0x0,
			WEAPPROJEXP_ROCKET = 0x1,
			WEAPPROJEXP_FLASHBANG = 0x2,
			WEAPPROJEXP_NONE = 0x3,
			WEAPPROJEXP_DUD = 0x4,
			WEAPPROJEXP_SMOKE = 0x5,
			WEAPPROJEXP_HEAVY = 0x6,
			WEAPPROJEXP_NUM = 0x7
		};

		enum WeapStickinessType
		{
			WEAPSTICKINESS_NONE = 0x0,
			WEAPSTICKINESS_ALL = 0x1,
			WEAPSTICKINESS_ALL_ORIENT = 0x2,
			WEAPSTICKINESS_GROUND = 0x3,
			WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
			WEAPSTICKINESS_KNIFE = 0x5,
			WEAPSTICKINESS_COUNT = 0x6
		};

		enum guidedMissileType_t
		{
			MISSILE_GUIDANCE_NONE = 0x0,
			MISSILE_GUIDANCE_SIDEWINDER = 0x1,
			MISSILE_GUIDANCE_HELLFIRE = 0x2,
			MISSILE_GUIDANCE_JAVELIN = 0x3,
			MISSILE_GUIDANCE_COUNT = 0x4
		};

		struct WeaponDef
		{
			const char* szOverlayName;
			XModel** gunXModel;
			XModel* handXModel;
			const char** szXAnimsRightHanded;
			const char** szXAnimsLeftHanded;
			const char* szModeName;
			unsigned short* notetrackSoundMapKeys;
			unsigned short* notetrackSoundMapValues;
			unsigned short* notetrackRumbleMapKeys;
			unsigned short* notetrackRumbleMapValues;
			int playerAnimType;
			weapType_t weapType;
			weapClass_t weapClass;
			PenetrateType penetrateType;
			weapInventoryType_t inventoryType;
			weapFireType_t fireType;
			OffhandClass offhandClass;
			weapStance_t stance;
			FxEffectDef* viewFlashEffect;
			FxEffectDef* worldFlashEffect;
			snd_alias_list_t* pickupSound;
			snd_alias_list_t* pickupSoundPlayer;
			snd_alias_list_t* ammoPickupSound;
			snd_alias_list_t* ammoPickupSoundPlayer;
			snd_alias_list_t* projectileSound;
			snd_alias_list_t* pullbackSound;
			snd_alias_list_t* pullbackSoundPlayer;
			snd_alias_list_t* fireSound;
			snd_alias_list_t* fireSoundPlayer;
			snd_alias_list_t* fireSoundPlayerAkimbo;
			snd_alias_list_t* fireLoopSound;
			snd_alias_list_t* fireLoopSoundPlayer;
			snd_alias_list_t* fireStopSound;
			snd_alias_list_t* fireStopSoundPlayer;
			snd_alias_list_t* fireLastSound;
			snd_alias_list_t* fireLastSoundPlayer;
			snd_alias_list_t* emptyFireSound;
			snd_alias_list_t* emptyFireSoundPlayer;
			snd_alias_list_t* meleeSwipeSound;
			snd_alias_list_t* meleeSwipeSoundPlayer;
			snd_alias_list_t* meleeHitSound;
			snd_alias_list_t* meleeMissSound;
			snd_alias_list_t* rechamberSound;
			snd_alias_list_t* rechamberSoundPlayer;
			snd_alias_list_t* reloadSound;
			snd_alias_list_t* reloadSoundPlayer;
			snd_alias_list_t* reloadEmptySound;
			snd_alias_list_t* reloadEmptySoundPlayer;
			snd_alias_list_t* reloadStartSound;
			snd_alias_list_t* reloadStartSoundPlayer;
			snd_alias_list_t* reloadEndSound;
			snd_alias_list_t* reloadEndSoundPlayer;
			snd_alias_list_t* detonateSound;
			snd_alias_list_t* detonateSoundPlayer;
			snd_alias_list_t* nightVisionWearSound;
			snd_alias_list_t* nightVisionWearSoundPlayer;
			snd_alias_list_t* nightVisionRemoveSound;
			snd_alias_list_t* nightVisionRemoveSoundPlayer;
			snd_alias_list_t* altSwitchSound;
			snd_alias_list_t* altSwitchSoundPlayer;
			snd_alias_list_t* raiseSound;
			snd_alias_list_t* raiseSoundPlayer;
			snd_alias_list_t* firstRaiseSound;
			snd_alias_list_t* firstRaiseSoundPlayer;
			snd_alias_list_t* putawaySound;
			snd_alias_list_t* putawaySoundPlayer;
			snd_alias_list_t* scanSound;
			snd_alias_list_t* changeVariableZoomSound;
			snd_alias_list_t** bounceSound;
			snd_alias_list_t** rollingSound;
			FxEffectDef* viewShellEjectEffect;
			FxEffectDef* worldShellEjectEffect;
			FxEffectDef* viewLastShotEjectEffect;
			FxEffectDef* worldLastShotEjectEffect;
			Material* reticleCenter;
			Material* reticleSide;
			int iReticleCenterSize;
			int iReticleSideSize;
			int iReticleMinOfs;
			activeReticleType_t activeReticleType;
			float vStandMove[3];
			float vStandRot[3];
			float strafeMove[3];
			float strafeRot[3];
			float vDuckedOfs[3];
			float vDuckedMove[3];
			float vDuckedRot[3];
			float vProneOfs[3];
			float vProneMove[3];
			float vProneRot[3];
			float fPosMoveRate;
			float fPosProneMoveRate;
			float fStandMoveMinSpeed;
			float fDuckedMoveMinSpeed;
			float fProneMoveMinSpeed;
			float fPosRotRate;
			float fPosProneRotRate;
			float fStandRotMinSpeed;
			float fDuckedRotMinSpeed;
			float fProneRotMinSpeed;
			XModel** worldModel;
			XModel* worldClipModel;
			XModel* rocketModel;
			XModel* knifeModel;
			XModel* worldKnifeModel;
			Material* hudIcon;
			weaponIconRatioType_t hudIconRatio;
			Material* pickupIcon;
			weaponIconRatioType_t pickupIconRatio;
			Material* ammoCounterIcon;
			weaponIconRatioType_t ammoCounterIconRatio;
			ammoCounterClipType_t ammoCounterClip;
			int iStartAmmo;
			const char* szAmmoName;
			int iAmmoIndex;
			const char* szClipName;
			int iClipIndex;
			int iMaxAmmo;
			int shotCount;
			const char* szSharedAmmoCapName;
			int iSharedAmmoCapIndex;
			int iSharedAmmoCap;
			int damage;
			int playerDamage;
			int iMeleeDamage;
			int iDamageType;
			StateTimers stateTimers;
			StateTimers akimboStateTimers;
			float autoAimRange;
			float aimAssistRange;
			float aimAssistRangeAds;
			float aimPadding;
			float enemyCrosshairRange;
			float moveSpeedScale;
			float adsMoveSpeedScale;
			float sprintDurationScale;
			float fAdsZoomInFrac;
			float fAdsZoomOutFrac;
			ADSOverlay overlay;
			WeapOverlayInteface_t overlayInterface;
			float fAdsBobFactor;
			float fAdsViewBobMult;
			float fHipSpreadStandMin;
			float fHipSpreadDuckedMin;
			float fHipSpreadProneMin;
			float hipSpreadStandMax;
			float hipSpreadDuckedMax;
			float hipSpreadProneMax;
			float fHipSpreadDecayRate;
			float fHipSpreadFireAdd;
			float fHipSpreadTurnAdd;
			float fHipSpreadMoveAdd;
			float fHipSpreadDuckedDecay;
			float fHipSpreadProneDecay;
			float fHipReticleSidePos;
			float fAdsIdleAmount;
			float fHipIdleAmount;
			float adsIdleSpeed;
			float hipIdleSpeed;
			float fIdleCrouchFactor;
			float fIdleProneFactor;
			float fGunMaxPitch;
			float fGunMaxYaw;
			float adsIdleLerpStartTime;
			float adsIdleLerpTime;
			float swayMaxAngle;
			float swayLerpSpeed;
			float swayPitchScale;
			float swayYawScale;
			float swayHorizScale;
			float swayVertScale;
			float swayShellShockScale;
			float adsSwayMaxAngle;
			float adsSwayLerpSpeed;
			float adsSwayPitchScale;
			float adsSwayYawScale;
			float adsSwayHorizScale;
			float adsSwayVertScale;
			float adsViewErrorMin;
			float adsViewErrorMax;
			PhysCollmap* physCollmap;
			float dualWieldViewModelOffset;
			weaponIconRatioType_t killIconRatio;
			int iReloadAmmoAdd;
			int iReloadStartAdd;
			int ammoDropStockMin;
			int ammoDropClipPercentMin;
			int ammoDropClipPercentMax;
			int iExplosionRadius;
			int iExplosionRadiusMin;
			int iExplosionInnerDamage;
			int iExplosionOuterDamage;
			float damageConeAngle;
			float bulletExplDmgMult;
			float bulletExplRadiusMult;
			int iProjectileSpeed;
			int iProjectileSpeedUp;
			int iProjectileSpeedForward;
			int iProjectileActivateDist;
			float projLifetime;
			float timeToAccelerate;
			float projectileCurvature;
			XModel* projectileModel;
			weapProjExposion_t projExplosion;
			FxEffectDef* projExplosionEffect;
			FxEffectDef* projDudEffect;
			snd_alias_list_t* projExplosionSound;
			snd_alias_list_t* projDudSound;
			WeapStickinessType stickiness;
			float lowAmmoWarningThreshold;
			float ricochetChance;
			bool riotShieldEnableDamage;
			int riotShieldHealth;
			float riotShieldDamageMult;
			float* parallelBounce;
			float* perpendicularBounce;
			FxEffectDef* projTrailEffect;
			FxEffectDef* projBeaconEffect;
			float vProjectileColor[3];
			guidedMissileType_t guidedMissileType;
			float maxSteeringAccel;
			int projIgnitionDelay;
			FxEffectDef* projIgnitionEffect;
			snd_alias_list_t* projIgnitionSound;
			float fAdsAimPitch;
			float fAdsCrosshairInFrac;
			float fAdsCrosshairOutFrac;
			int adsGunKickReducedKickBullets;
			float adsGunKickReducedKickPercent;
			float fAdsGunKickPitchMin;
			float fAdsGunKickPitchMax;
			float fAdsGunKickYawMin;
			float fAdsGunKickYawMax;
			float fAdsGunKickAccel;
			float fAdsGunKickSpeedMax;
			float fAdsGunKickSpeedDecay;
			float fAdsGunKickStaticDecay;
			float fAdsViewKickPitchMin;
			float fAdsViewKickPitchMax;
			float fAdsViewKickYawMin;
			float fAdsViewKickYawMax;
			float fAdsViewScatterMin;
			float fAdsViewScatterMax;
			float fAdsSpread;
			int hipGunKickReducedKickBullets;
			float hipGunKickReducedKickPercent;
			float fHipGunKickPitchMin;
			float fHipGunKickPitchMax;
			float fHipGunKickYawMin;
			float fHipGunKickYawMax;
			float fHipGunKickAccel;
			float fHipGunKickSpeedMax;
			float fHipGunKickSpeedDecay;
			float fHipGunKickStaticDecay;
			float fHipViewKickPitchMin;
			float fHipViewKickPitchMax;
			float fHipViewKickYawMin;
			float fHipViewKickYawMax;
			float fHipViewScatterMin;
			float fHipViewScatterMax;
			float fightDist;
			float maxDist;
			const char* accuracyGraphName[2];
			float(*originalAccuracyGraphKnots[2])[2];
			unsigned short originalAccuracyGraphKnotCount[2];
			int iPositionReloadTransTime;
			float leftArc;
			float rightArc;
			float topArc;
			float bottomArc;
			float accuracy;
			float aiSpread;
			float playerSpread;
			float minTurnSpeed[2];
			float maxTurnSpeed[2];
			float pitchConvergenceTime;
			float yawConvergenceTime;
			float suppressTime;
			float maxRange;
			float fAnimHorRotateInc;
			float fPlayerPositionDist;
			const char* szUseHintString;
			const char* dropHintString;
			int iUseHintStringIndex;
			int dropHintStringIndex;
			float horizViewJitter;
			float vertViewJitter;
			float scanSpeed;
			float scanAccel;
			int scanPauseTime;
			const char* szScript;
			float fOOPosAnimLength[2];
			int minDamage;
			int minPlayerDamage;
			float fMaxDamageRange;
			float fMinDamageRange;
			float destabilizationRateTime;
			float destabilizationCurvatureMax;
			int destabilizeDistance;
			float* locationDamageMultipliers;
			const char* fireRumble;
			const char* meleeImpactRumble;
			TracerDef* tracerType;
			bool turretADSEnabled;
			float turretADSTime;
			float turretFov;
			float turretFovADS;
			float turretScopeZoomRate;
			float turretScopeZoomMin;
			float turretScopeZoomMax;
			float turretOverheatUpRate;
			float turretOverheatDownRate;
			float turretOverheatPenalty;
			snd_alias_list_t* turretOverheatSound;
			FxEffectDef* turretOverheatEffect;
			const char* turretBarrelSpinRumble;
			float turretBarrelSpinSpeed;
			float turretBarrelSpinUpTime;
			float turretBarrelSpinDownTime;
			snd_alias_list_t* turretBarrelSpinMaxSnd;
			snd_alias_list_t* turretBarrelSpinUpSnd[4];
			snd_alias_list_t* turretBarrelSpinDownSnd[4];
			snd_alias_list_t* missileConeSoundAlias;
			snd_alias_list_t* missileConeSoundAliasAtBase;
			float missileConeSoundRadiusAtTop;
			float missileConeSoundRadiusAtBase;
			float missileConeSoundHeight;
			float missileConeSoundOriginOffset;
			float missileConeSoundVolumescaleAtCore;
			float missileConeSoundVolumescaleAtEdge;
			float missileConeSoundVolumescaleCoreSize;
			float missileConeSoundPitchAtTop;
			float missileConeSoundPitchAtBottom;
			float missileConeSoundPitchTopSize;
			float missileConeSoundPitchBottomSize;
			float missileConeSoundCrossfadeTopSize;
			float missileConeSoundCrossfadeBottomSize;
			bool sharedAmmo;
			bool lockonSupported;
			bool requireLockonToFire;
			bool isAirburstWeapon;
			bool bigExplosion;
			bool noAdsWhenMagEmpty;
			bool avoidDropCleanup;
			bool inheritsPerks;
			bool crosshairColorChange;
			bool bRifleBullet;
			bool armorPiercing;
			bool bBoltAction;
			bool aimDownSight;
			bool canHoldBreath;
			bool canVariableZoom;
			bool bRechamberWhileAds;
			bool bBulletExplosiveDamage;
			bool bCookOffHold;
			bool bClipOnly;
			bool noAmmoPickup;
			bool adsFireOnly;
			bool cancelAutoHolsterWhenEmpty;
			bool disableSwitchToWhenEmpty;
			bool suppressAmmoReserveDisplay;
			bool laserSightDuringNightvision;
			bool markableViewmodel;
			bool noDualWield;
			bool flipKillIcon;
			bool bNoPartialReload;
			bool bSegmentedReload;
			bool blocksProne;
			bool silenced;
			bool isRollingGrenade;
			bool projExplosionEffectForceNormalUp;
			bool bProjImpactExplode;
			bool stickToPlayers;
			bool stickToVehicles;
			bool stickToTurrets;
			bool hasDetonator;
			bool disableFiring;
			bool timedDetonation;
			bool rotate;
			bool holdButtonToThrow;
			bool freezeMovementWhenFiring;
			bool thermalScope;
			bool altModeSameWeapon;
			bool turretBarrelSpinEnabled;
			bool missileConeSoundEnabled;
			bool missileConeSoundPitchshiftEnabled;
			bool missileConeSoundCrossfadeEnabled;
			bool offhandHoldIsCancelable;
			bool doNotAllowAttachmentsToOverrideSpread;
			unsigned short stowTag;
			XModel* stowOffsetModel;
		};




		struct AnimOverrideEntry
		{
			unsigned short attachment1;
			unsigned short attachment2;
			const char* overrideAnim;
			const char* altmodeAnim;
			unsigned int animTreeType;
			int animTime;
			int altTime;
		};

		struct SoundOverrideEntry
		{
			unsigned short attachment1;
			unsigned short attachment2;
			snd_alias_list_t* overrideSound;
			snd_alias_list_t* altmodeSound;
			unsigned int soundType;
		};

		struct FXOverrideEntry
		{
			unsigned short attachment1;
			unsigned short attachment2;
			FxEffectDef* overrideFX;
			FxEffectDef* altmodeFX;
			unsigned int fxType;
		};

		struct ReloadStateTimerEntry
		{
			int attachment;
			int reloadAddTime;
			int reloadStartAddTime;
		};

		struct NoteTrackToSoundEntry
		{
			int attachment;
			unsigned short* notetrackSoundMapKeys;
			unsigned short* notetrackSoundMapValues;
		};

		enum AttachmentType
		{
			ATTACHMENT_SCOPE = 0x0,
			ATTACHMENT_UNDERBARREL = 0x1,
			ATTACHMENT_OTHER = 0x2,
			ATTACHMENT_COUNT = 0x3
		};

		struct AttGeneral
		{
			bool boltAction;
			bool inheritsPerks;
			float enemyCrosshairRange;
			Material* reticleCenter;
			Material* reticleSide;
			int reticleCenterSize;
			int reticleSideSize;
			float moveSpeedScale;
			float adsMoveSpeedScale;
		};


		struct AttAmmoGeneral
		{
			PenetrateType penetrateType;
			float penetrateMultiplier;
			ImpactType impactType;
			weapFireType_t fireType;
			TracerDef* tracerType;
			bool rifleBullet;
			bool armorPiercing;
		};

		struct AttSight
		{
			bool aimDownSight;
			bool adsFire;
			bool rechamberWhileAds;
			bool noAdsWhenMagEmpty;
			bool canHoldBreath;
			bool canVariableZoom;
			bool hideRailWithThisScope;
		};

		struct AttDamage
		{
			int damage;
			int minDamage;
			int meleeDamage;
			float maxDamageRange;
			float minDamageRange;
			int playerDamage;
			int minPlayerDamage;
		};

		struct AttReload
		{
			bool noPartialReload;
			bool segmentedReload;
		};

		struct AttAmmunition
		{
			int maxAmmo;
			int startAmmo;
			int clipSize;
			int shotCount;
			int reloadAmmoAdd;
			int reloadStartAdd;
		};

		struct AttAddOns
		{
			bool motionTracker;
			bool silenced;
		};

		struct AttAimAssist
		{
			float autoAimRange;
			float aimAssistRange;
			float aimAssistRangeAds;
		};

		struct AttLocationDamage
		{
			float locNone;
			float locHelmet;
			float locHead;
			float locNeck;
			float locTorsoUpper;
			float locTorsoLower;
			float locRightArmUpper;
			float locRightArmLower;
			float locRightHand;
			float locLeftArmUpper;
			float locLeftArmLower;
			float locLeftHand;
			float locRightLegUpper;
			float locRightLegLower;
			float locRightFoot;
			float locLeftLegUpper;
			float locLeftLegLower;
			float locLeftFoot;
			float locGun;
		};

		struct AttIdleSettings
		{
			float hipIdleAmount;
			float hipIdleSpeed;
			float idleCrouchFactor;
			float idleProneFactor;
			float adsIdleLerpStartTime;
			float adsIdleLerpTime;
		};

		struct AttADSSettings
		{
			float adsSpread;
			float adsAimPitch;
			float adsTransInTime;
			float adsTransOutTime;
			int adsReloadTransTime;
			float adsCrosshairInFrac;
			float adsCrosshairOutFrac;
			float adsZoomFov;
			float adsZoomInFrac;
			float adsZoomOutFrac;
			float adsBobFactor;
			float adsViewBobMult;
			float adsViewErrorMin;
			float adsViewErrorMax;
		};

		struct AttHipSpread
		{
			float hipSpreadStandMin;
			float hipSpreadDuckedMin;
			float hipSpreadProneMin;
			float hipSpreadMax;
			float hipSpreadDuckedMax;
			float hipSpreadProneMax;
			float hipSpreadFireAdd;
			float hipSpreadTurnAdd;
			float hipSpreadMoveAdd;
			float hipSpreadDecayRate;
			float hipSpreadDuckedDecay;
			float hipSpreadProneDecay;
		};

		struct AttGunKick
		{
			int hipGunKickReducedKickBullets;
			float hipGunKickReducedKickPercent;
			float hipGunKickPitchMin;
			float hipGunKickPitchMax;
			float hipGunKickYawMin;
			float hipGunKickYawMax;
			float hipGunKickAccel;
			float hipGunKickSpeedMax;
			float hipGunKickSpeedDecay;
			float hipGunKickStaticDecay;
			int adsGunKickReducedKickBullets;
			float adsGunKickReducedKickPercent;
			float adsGunKickPitchMin;
			float adsGunKickPitchMax;
			float adsGunKickYawMin;
			float adsGunKickYawMax;
			float adsGunKickAccel;
			float adsGunKickSpeedMax;
			float adsGunKickSpeedDecay;
			float adsGunKickStaticDecay;
		};



		struct AttViewKick
		{
			float hipViewKickPitchMin;
			float hipViewKickPitchMax;
			float hipViewKickYawMin;
			float hipViewKickYawMax;
			float hipViewKickCenterSpeed;
			float adsViewKickPitchMin;
			float adsViewKickPitchMax;
			float adsViewKickYawMin;
			float adsViewKickYawMax;
			float adsViewKickCenterSpeed;
		};

		struct AttADSOverlay
		{
			ADSOverlay overlay;
			bool thermalScope;
		};

		struct AttUI
		{
			Material* dpadIcon;
			Material* ammoCounterIcon;
			weaponIconRatioType_t dpadIconRatio;
			weaponIconRatioType_t ammoCounterIconRatio;
			ammoCounterClipType_t ammoCounterClip;
		};

		struct AttRumbles
		{
			const char* fireRumble;
			const char* meleeImpactRumble;
		};

		struct AttProjectile
		{
			int explosionRadius;
			int explosionInnerDamage;
			int explosionOuterDamage;
			float damageConeAngle;
			int projectileSpeed;
			int projectileSpeedUp;
			int projectileActivateDist;
			float projectileLifetime;
			XModel* projectileModel;
			weapProjExposion_t projExplosionType;
			FxEffectDef* projExplosionEffect;
			bool projExplosionEffectForceNormalUp;
			snd_alias_list_t* projExplosionSound;
			FxEffectDef* projDudEffect;
			snd_alias_list_t* projDudSound;
			bool projImpactExplode;
			float destabilizationRateTime;
			float destabilizationCurvatureMax;
			int destabilizeDistance;
			FxEffectDef* projTrailEffect;
			int projIgnitionDelay;
			FxEffectDef* projIgnitionEffect;
			snd_alias_list_t* projIgnitionSound;
		};

		struct WeaponAttachment
		{
			const char* szInternalName;
			const char* szDisplayName;
			AttachmentType type;
			weapType_t weaponType;
			weapClass_t weapClass;
			XModel** worldModels;
			XModel** viewModels;
			XModel** reticleViewModels;
			AttAmmoGeneral* ammogeneral; // Tracer
			AttSight* sight;
			AttReload* reload;
			AttAddOns* addOns;
			AttGeneral* general; // Reticle
			AttAimAssist* aimAssist;
			AttAmmunition* ammunition;
			AttDamage* damage;
			AttLocationDamage* locationDamage;
			AttIdleSettings* idleSettings;
			AttADSSettings* adsSettings;
			AttADSSettings* adsSettingsMain;
			AttHipSpread* hipSpread;
			AttGunKick* gunKick;
			AttViewKick* viewKick;
			AttADSOverlay* adsOverlay; // Reticle materials
			AttUI* ui; // DPad Icon
			AttRumbles* rumbles;
			AttProjectile* projectile; // Proj (probably ignorable)
			float ammunitionScale;
			float damageScale;
			float damageScaleMin;
			float stateTimersScale;
			float fireTimersScale;
			float idleSettingsScale;
			float adsSettingsScale;
			float adsSettingsScaleMain;
			float hipSpreadScale;
			float gunKickScale;
			float viewKickScale;
			float viewCenterScale;
			int loadIndex;
			bool hideIronSightsWithThisAttachment;
			bool shareAmmoWithAlt;
		};

		struct WeaponCompleteDef
		{
			const char* szInternalName;
			WeaponDef* weapDef;
			const char* szDisplayName;
			unsigned short* hideTags;
			WeaponAttachment** scopes;
			WeaponAttachment** underBarrels;
			WeaponAttachment** others;
			const char** szXAnims;
			unsigned int numAnimOverrides;
			AnimOverrideEntry* animOverrides;
			unsigned int numSoundOverrides;
			SoundOverrideEntry* soundOverrides;
			unsigned int numFXOverrides;
			FXOverrideEntry* fxOverrides;
			unsigned int numReloadStateTimerOverrides;
			ReloadStateTimerEntry* reloadOverrides;
			unsigned int numNotetrackOverrides;
			NoteTrackToSoundEntry* notetrackOverrides;
			float fAdsZoomFov;
			int iAdsTransInTime;
			int iAdsTransOutTime;
			int iClipSize;
			ImpactType impactType;
			int iFireTime;
			int iFireTimeAkimbo;
			weaponIconRatioType_t dpadIconRatio;
			float penetrateMultiplier;
			float fAdsViewKickCenterSpeed;
			float fHipViewKickCenterSpeed;
			const char* szAltWeaponName;
			int altWeapon;
			int iAltRaiseTime;
			int iAltRaiseTimeAkimbo;
			Material* killIcon;
			Material* dpadIcon;
			int fireAnimLength;
			int fireAnimLengthAkimbo;
			int iFirstRaiseTime;
			int iFirstRaiseTimeAkimbo;
			int ammoDropStockMax;
			float adsDofStart;
			float adsDofEnd;
			unsigned short accuracyGraphKnotCount[2];
			float(*accuracyGraphKnots[2])[2];
			bool motionTracker;
			bool enhanced;
			bool dpadIconShowsAmmo;
		};

		union XAssetHeader
		{
			void* data;
			PhysPreset* physPreset;
			PhysCollmap* physCollmap;
			XAnimParts* parts;
			XModelSurfs* modelSurfs;
			XModel* model;
			Material* material;
			MaterialPixelShader* pixelShader;
			MaterialVertexShader* vertexShader;
			MaterialVertexDeclaration* vertexDecl;
			MaterialTechniqueSet* techniqueSet;
			GfxImage* image;
			snd_alias_list_t* sound;
			SndCurve* sndCurve;
			LoadedSound* loadSnd;
			clipMap_t* clipMap;
			ComWorld* comWorld;
			GlassWorld* glassWorld;
			PathData* pathData;
			VehicleTrack* vehicleTrack;
			MapEnts* mapEnts;
			FxWorld* fxWorld;
			GfxWorld* gfxWorld;
			GfxLightDef* lightDef;
			Font_s* font;
			MenuList* menuList;
			menuDef_t* menu;
			LocalizeEntry* localize;
			WeaponAttachment* attachment;
			WeaponCompleteDef* weapon;
			// SndDriverGlobals* sndDriverGlobals; // UNUSED ON PC
			FxEffectDef* fx;
			FxImpactTable* impactFx;
			SurfaceFxTable* surfaceFx;
			RawFile* rawfile;
			ScriptFile* scriptfile;
			StringTable* stringTable;
			LeaderboardDef* leaderboardDef;
			StructuredDataDefSet* structuredDataDefSet;
			TracerDef* tracerDef;
			VehicleDef* vehDef;
			AddonMapEnts* addonMapEnts;
		};

		struct ComPrimaryLight
		{
			unsigned char type;
			unsigned char canUseShadowMap;
			unsigned char exponent;
			unsigned char unused;
			float color[3];
			float dir[3];
			float up[3];
			float origin[3];
			float radius;
			float cosHalfFovOuter;
			float cosHalfFovInner;
			float cosHalfFovExpanded;
			float rotationLimit;
			float translationLimit;
			const char* defName;
		};

		struct ComWorld
		{
			const char* name;
			int isInUse;
			unsigned int primaryLightCount;
			ComPrimaryLight* primaryLights;
		};


		struct G_GlassPiece
		{
			unsigned __int16 damageTaken;
			unsigned __int16 collapseTime;
			int lastStateChangeTime;
			char impactDir;
			char impactPos[2];
		};

		struct G_GlassName
		{
			char* nameStr;
			unsigned __int16 name;
			unsigned __int16 pieceCount;
			unsigned __int16* pieceIndices;
		};

		struct G_GlassData
		{
			G_GlassPiece* glassPieces;
			unsigned int pieceCount;
			unsigned __int16 damageToWeaken;
			unsigned __int16 damageToDestroy;
			unsigned int glassNameCount;
			G_GlassName* glassNames;
			char pad[108];
		};


		struct GlassWorld
		{
			const char* name;
			G_GlassData* g_glassData;
		};

		struct GfxImageLoadDef
		{
			char levelCount;
			char pad[3];
			int flags;
			int format;
			int resourceSize;
			char data[1];
		};

		struct Picmip
		{
			char platform[2];
		};

		struct CardMemory
		{
			int platform[2];
		};

		union GfxTexture
		{
			// IDirect3DBaseTexture9* basemap;
			// IDirect3DTexture9* map;
			// IDirect3DVolumeTexture9* volmap;
			// IDirect3DCubeTexture9* cubemap;
			GfxImageLoadDef* loadDef;
		};

		struct GfxImageStreamData
		{
			unsigned short width;
			unsigned short height;
			unsigned int pixelSize;
		};

		enum GfxImageCategory : unsigned char
		{
			IMG_CATEGORY_UNKNOWN = 0x0,
			IMG_CATEGORY_AUTO_GENERATED = 0x1,
			IMG_CATEGORY_LIGHTMAP = 0x2,
			IMG_CATEGORY_LOAD_FROM_FILE = 0x3,
			IMG_CATEGORY_RAW = 0x4,
			IMG_CATEGORY_FIRST_UNMANAGED = 0x5,
			IMG_CATEGORY_WATER = 0x5,
			IMG_CATEGORY_RENDERTARGET = 0x6,
			IMG_CATEGORY_TEMP = 0x7,
		};

		struct GfxImage
		{
			GfxTexture texture;
			char mapType;
			char semantic;
			char category;
			unsigned char flags;
			Picmip picmip;
			bool noPicmip;
			char track;
			CardMemory cardMemory;
			unsigned short width;
			unsigned short height;
			unsigned short depth;
			unsigned char levelCount;
			const char* name;
		};

		struct GfxLightImage
		{
			GfxImage* image;
			unsigned char samplerState;
		};

		struct GfxLightDef
		{
			const char* name;
			GfxLightImage attenuation;
			GfxLightImage cucoloris;
			int lmapLookupStart;
		};

		enum MaterialTechniqueType
		{
			// From iw4, looks correct
			TECHNIQUE_DEPTH_PREPASS = 0x0,
			TECHNIQUE_BUILD_FLOAT_Z = 0x1,
			TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
			TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
			TECHNIQUE_UNLIT = 0x4,
			TECHNIQUE_EMISSIVE = 0x5,
			TECHNIQUE_EMISSIVE_DFOG = 0x6,
			TECHNIQUE_EMISSIVE_SHADOW = 0x7,
			TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
			TECHNIQUE_LIT_BEGIN = 0x9,
			TECHNIQUE_LIT = 0x9,
			TECHNIQUE_LIT_DFOG = 0xA,
			TECHNIQUE_LIT_SUN = 0xB,
			TECHNIQUE_LIT_SUN_DFOG = 0xC,
			TECHNIQUE_LIT_SUN_SHADOW = 0xD,
			TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
			TECHNIQUE_LIT_SPOT = 0xF,
			TECHNIQUE_LIT_SPOT_DFOG = 0x10,

			// Two unknowns

			TECHNIQUE_LIT_SPOT_SHADOW = 0x13,
			TECHNIQUE_LIT_SPOT_SHADOW_DFOG,
			TECHNIQUE_LIT_OMNI,
			TECHNIQUE_LIT_OMNI_DFOG,
			TECHNIQUE_LIT_OMNI_SHADOW,
			TECHNIQUE_LIT_OMNI_SHADOW_DFOG,
			TECHNIQUE_LIT_INSTANCED,
			TECHNIQUE_LIT_INSTANCED_DFOG,
			TECHNIQUE_LIT_INSTANCED_SUN,
			TECHNIQUE_LIT_INSTANCED_SUN_DFOG,
			TECHNIQUE_LIT_INSTANCED_SUN_SHADOW,
			TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG,
			TECHNIQUE_LIT_INSTANCED_SPOT,
			TECHNIQUE_LIT_INSTANCED_SPOT_DFOG,
			TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW,
			TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG,

			// <2 instance spot shadows here>

			TECHNIQUE_LIT_INSTANCED_OMNI = 0x25,
			TECHNIQUE_LIT_INSTANCED_OMNI_DFOG,
			TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW,
			TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG,
			TECHNIQUE_LIGHT_SPOT,
			TECHNIQUE_LIGHT_OMNI,
			TECHNIQUE_LIGHT_SPOT_SHADOW,

			// <1 unknown here>

			TECHNIQUE_FAKELIGHT_NORMAL = 0x2D,
			TECHNIQUE_FAKELIGHT_VIEW = 0x2E,
			TECHNIQUE_SUNLIGHT_PREVIEW = 0x2F,
			TECHNIQUE_CASE_TEXTURE = 0x30,
			TECHNIQUE_WIREFRAME_SOLID = 0x31,
			TECHNIQUE_WIREFRAME_SHADED = 0x32,
			TECHNIQUE_THERMAL = 0x33, // Confirmed
			TECHNIQUE_DEBUG_BUMPMAP = 0x34,
			TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x35,
			//

			TECHNIQUE_COUNT = 0x39,
		};

		static_assert(MaterialTechniqueType::TECHNIQUE_THERMAL == 51U); // Confirmed

		enum ShaderCodeConstants : unsigned short
		{
			CONST_SRC_CODE_LIGHT_POSITION,
			CONST_SRC_CODE_LIGHT_DIFFUSE,
			CONST_SRC_CODE_LIGHT_SPECULAR,
			CONST_SRC_CODE_LIGHT_SPOTDIR,
			CONST_SRC_CODE_LIGHT_SPOTFACTORS,
			CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT,
			CONST_SRC_CODE_PARTICLE_CLOUD_COLOR,
			CONST_SRC_CODE_GAMETIME,
			CONST_SRC_CODE_EYEOFFSET,
			CONST_SRC_CODE_COLOR_SATURATION_R,
			CONST_SRC_CODE_COLOR_SATURATION_G,
			CONST_SRC_CODE_COLOR_SATURATION_B,
			CONST_SRC_CODE_PIXEL_COST_FRACS,
			CONST_SRC_CODE_PIXEL_COST_DECODE,
			CONST_SRC_CODE_FILTER_TAP_0,
			CONST_SRC_CODE_FILTER_TAP_1,
			CONST_SRC_CODE_FILTER_TAP_2,
			CONST_SRC_CODE_FILTER_TAP_3,
			CONST_SRC_CODE_FILTER_TAP_4,
			CONST_SRC_CODE_FILTER_TAP_5,
			CONST_SRC_CODE_FILTER_TAP_6,
			CONST_SRC_CODE_FILTER_TAP_7,
			CONST_SRC_CODE_COLOR_MATRIX_R,
			CONST_SRC_CODE_COLOR_MATRIX_G,
			CONST_SRC_CODE_COLOR_MATRIX_B,
			CONST_SRC_CODE_UNK1,
			CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET,
			CONST_SRC_CODE_RENDER_TARGET_SIZE,
			CONST_SRC_CODE_RENDER_SOURCE_SIZE,
			CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR,
			CONST_SRC_CODE_DOF_EQUATION_SCENE,
			CONST_SRC_CODE_DOF_LERP_SCALE,
			CONST_SRC_CODE_DOF_LERP_BIAS,
			CONST_SRC_CODE_DOF_ROW_DELTA,
			CONST_SRC_CODE_MOTION_MATRIX_X,
			CONST_SRC_CODE_MOTION_MATRIX_Y,
			CONST_SRC_CODE_MOTION_MATRIX_W,
			CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION,
			CONST_SRC_CODE_SHADOWMAP_SCALE,
			CONST_SRC_CODE_ZNEAR,
			CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE,
			CONST_SRC_CODE_DEBUG_BUMPMAP,
			CONST_SRC_CODE_MATERIAL_COLOR,
			CONST_SRC_CODE_FOG,
			CONST_SRC_CODE_FOG_COLOR_LINEAR,
			CONST_SRC_CODE_FOG_COLOR_GAMMA,
			CONST_SRC_CODE_FOG_SUN_CONSTS,
			CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR,
			CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA,
			CONST_SRC_CODE_FOG_SUN_DIR,
			CONST_SRC_CODE_GLOW_SETUP,
			CONST_SRC_CODE_GLOW_APPLY,
			CONST_SRC_CODE_COLOR_BIAS,
			CONST_SRC_CODE_COLOR_TINT_BASE,
			CONST_SRC_CODE_COLOR_TINT_DELTA,
			CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA,
			CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS,
			CONST_SRC_CODE_ENVMAP_PARMS,
			CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST,
			CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST,
			CONST_SRC_CODE_COMPOSITE_FX_DISTORTION,
			CONST_SRC_CODE_POSTFX_FADE_EFFECT,
			CONST_SRC_CODE_VIEWPORT_DIMENSIONS,
			CONST_SRC_CODE_FRAMEBUFFER_READ,
			CONST_SRC_CODE_THERMAL_COLOR_OFFSET,
			CONST_SRC_CODE_PLAYLIST_POPULATION_PARAMS,
			CONST_SRC_CODE_BASE_LIGHTING_COORDS,
			CONST_SRC_CODE_LIGHT_PROBE_AMBIENT,
			CONST_SRC_CODE_NEARPLANE_ORG,
			CONST_SRC_CODE_NEARPLANE_DX,
			CONST_SRC_CODE_NEARPLANE_DY,
			CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE,
			CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET,
			CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0,
			CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1,
			CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2,
			CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0,
			CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1,
			CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2,
			CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0,
			CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1,
			CONST_SRC_CODE_DEPTH_FROM_CLIP,
			CONST_SRC_CODE_CODE_MESH_ARG_0,
			CONST_SRC_CODE_CODE_MESH_ARG_1,
			CONST_SRC_CODE_VIEW_MATRIX,
			CONST_SRC_CODE_INVERSE_VIEW_MATRIX,
			CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX,
			CONST_SRC_CODE_PROJECTION_MATRIX,
			CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX,
			CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX,
			CONST_SRC_CODE_VIEW_PROJECTION_MATRIX,
			CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX,
			CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX,
			CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX,
			CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX,
			CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX,
			CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX,
			CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX,
			CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX,
			CONST_SRC_CODE_WORLD_MATRIX0,
			CONST_SRC_CODE_INVERSE_WORLD_MATRIX0,
			CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0,
			CONST_SRC_CODE_WORLD_VIEW_MATRIX0,
			CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0,
			CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0,
			CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0,
			CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0,
			CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0,
			CONST_SRC_CODE_WORLD_MATRIX1,
			CONST_SRC_CODE_INVERSE_WORLD_MATRIX1,
			CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1,
			CONST_SRC_CODE_WORLD_VIEW_MATRIX1,
			CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1,
			CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1,
			CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1,
			CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1,
			CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1,
			CONST_SRC_CODE_WORLD_MATRIX2,
			CONST_SRC_CODE_INVERSE_WORLD_MATRIX2,
			CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2,
			CONST_SRC_CODE_WORLD_VIEW_MATRIX2,
			CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2,
			CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2,
			CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2,
			CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2,
			CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2,
			CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2,
			CONST_SRC_TOTAL_COUNT,
			CONST_SRC_NONE,
		};

		static_assert(ShaderCodeConstants::CONST_SRC_CODE_MATERIAL_COLOR == 42U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_FOG == 43U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR_LINEAR == 44U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR_GAMMA == 45U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_FOG_SUN_CONSTS == 46U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_FOG_SUN_DIR == 49U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST == 59U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_BASE_LIGHTING_COORDS == 66U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX == 94U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX == 98U);
		static_assert(ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 == 106U);



		/// <summary>
		/// Taken from Xbox, might be completely wrong!
		/// </summary>
		enum MaterialTextureSource
		{
			TEXTURE_SRC_CODE_BLACK,
			TEXTURE_SRC_CODE_WHITE,
			TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP,
			TEXTURE_SRC_CODE_MODEL_LIGHTING,
			TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY,
			TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY,
			TEXTURE_SRC_CODE_SHADOWMAP_SUN,
			TEXTURE_SRC_CODE_SHADOWMAP_SPOT,
			TEXTURE_SRC_CODE_FEEDBACK,
			TEXTURE_SRC_CODE_RESOLVED_POST_SUN,
			TEXTURE_SRC_CODE_RESOLVED_SCENE,
			TEXTURE_SRC_CODE_POST_EFFECT_0,
			TEXTURE_SRC_CODE_POST_EFFECT_1,
			TEXTURE_SRC_CODE_LIGHT_ATTENUATION,
			TEXTURE_SRC_CODE_LIGHT_CUCOLORIS,
			TEXTURE_SRC_CODE_OUTDOOR,
			TEXTURE_SRC_CODE_FLOATZ,
			TEXTURE_SRC_CODE_PROCESSED_FLOATZ,
			TEXTURE_SRC_CODE_RAW_FLOATZ,
			TEXTURE_SRC_CODE_HALF_PARTICLES,
			TEXTURE_SRC_CODE_HALF_PARTICLES_Z,
			TEXTURE_SRC_CODE_CASE_TEXTURE,
			TEXTURE_SRC_CODE_CINEMATIC_Y,
			TEXTURE_SRC_CODE_CINEMATIC_CR,
			TEXTURE_SRC_CODE_CINEMATIC_CB,
			TEXTURE_SRC_CODE_CINEMATIC_A,
			TEXTURE_SRC_CODE_REFLECTION_PROBE,
			TEXTURE_SRC_CODE_PIP_SCENE,
			TEXTURE_SRC_CODE_COLOR_MANIPULATION,
			TEXTURE_SRC_CODE_STREAMING_LOADING,
			TEXTURE_SRC_CODE_COUNT
		};

		static_assert(MaterialTextureSource::TEXTURE_SRC_CODE_MODEL_LIGHTING == 3);
		static_assert(MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SUN == 6);
		static_assert(MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SPOT == 7);
		static_assert(MaterialTextureSource::TEXTURE_SRC_CODE_LIGHT_ATTENUATION == 13);
		static_assert(MaterialTextureSource::TEXTURE_SRC_CODE_FLOATZ == 16);

		enum MaterialShaderArgumentType
		{
			MTL_ARG_MATERIAL_VERTEX_CONST = 0x0,
			MTL_ARG_LITERAL_VERTEX_CONST = 0x1,
			MTL_ARG_MATERIAL_VERTEX_SAMPLER = 0x2,
			MTL_ARG_MATERIAL_PIXEL_SAMPLER = 0x3,
			MTL_ARG_CODE_PRIM_BEGIN = 0x4,
			MTL_ARG_CODE_VERTEX_CONST = 0x4,
			MTL_ARG_CODE_PIXEL_SAMPLER = 0x5,
			MTL_ARG_CODE_PIXEL_CONST = 0x6,
			MTL_ARG_CODE_PRIM_END = 0x7,
			MTL_ARG_MATERIAL_PIXEL_CONST = 0x7,
			MTL_ARG_LITERAL_PIXEL_CONST = 0x8,
			MTL_ARG_COUNT = 0x9
		};

		struct MaterialArgumentCodeConst
		{
			unsigned short index;
			unsigned char firstRow;
			unsigned char rowCount;
		};

		union MaterialArgumentDef
		{
			const float(*literalConst)[4];
			MaterialArgumentCodeConst codeConst;
			unsigned int codeSampler;
			unsigned int nameHash;
		};

		struct MaterialShaderArgument
		{
			unsigned short type;
			unsigned short dest;
			MaterialArgumentDef u;
		};

		struct GfxPixelShaderLoadDef
		{
			unsigned int* program;
			unsigned short programSize;
			unsigned short loadForRenderer;
		};

		struct MaterialPixelShaderProgram
		{
			void/*IDirect3DPixelShader9*/* ps;
			GfxPixelShaderLoadDef loadDef;
		};

		struct MaterialPixelShader
		{
			const char* name;
			MaterialPixelShaderProgram prog;
		};


		struct GfxVertexShaderLoadDef
		{
			unsigned int* program;
			unsigned short programSize;
			unsigned short loadForRenderer;
		};

		struct MaterialVertexShaderProgram
		{
			void* vs;
			GfxVertexShaderLoadDef loadDef;
		};

		struct MaterialVertexShader
		{
			const char* name;
			MaterialVertexShaderProgram prog;
		};

		enum MaterialStreamRoutingSource : unsigned char
		{
			STREAM_SRC_POSITION = 0x0,
			STREAM_SRC_COLOR = 0x1,
			STREAM_SRC_TEXCOORD_0 = 0x2,
			STREAM_SRC_NORMAL = 0x3,
			STREAM_SRC_TANGENT = 0x4,
			STREAM_SRC_OPTIONAL_BEGIN = 0x5,
			STREAM_SRC_PRE_OPTIONAL_BEGIN = 0x4,
			STREAM_SRC_TEXCOORD_1 = 0x5,
			STREAM_SRC_TEXCOORD_2 = 0x6,
			STREAM_SRC_NORMAL_TRANSFORM_0 = 0x7,
			STREAM_SRC_NORMAL_TRANSFORM_1 = 0x8,
			STREAM_SRC_COUNT = 0x9,
		};

		enum MaterialStreamRoutingDestination : unsigned char
		{
			STREAM_DST_POSITION = 0x0,
			STREAM_DST_NORMAL = 0x1,
			STREAM_DST_COLOR_0 = 0x2,
			STREAM_DST_COLOR_1 = 0x3,
			STREAM_DST_DEPTH = 0x4,
			STREAM_DST_TEXCOORD_0 = 0x5,
			STREAM_DST_TEXCOORD_1 = 0x6,
			STREAM_DST_TEXCOORD_2 = 0x7,
			STREAM_DST_TEXCOORD_3 = 0x8,
			STREAM_DST_TEXCOORD_4 = 0x9,
			STREAM_DST_TEXCOORD_5 = 0xA,
			STREAM_DST_TEXCOORD_6 = 0xB,
			STREAM_DST_TEXCOORD_7 = 0xC,
			STREAM_DST_COUNT = 0xD,
		};


		struct MaterialStreamRouting
		{
			MaterialStreamRoutingSource source;
			MaterialStreamRoutingDestination dest;
		};

		union MaterialVertexStreamRouting
		{
			MaterialStreamRouting data[13];
			void* decl[21];
		};

		struct MaterialVertexDeclaration
		{
			const char* name;
			unsigned char streamCount;
			bool hasOptionalSource;
			MaterialVertexStreamRouting routing;
		};

		struct MaterialPass
		{
			MaterialVertexDeclaration* vertexDecl;
			MaterialVertexShader* vertexShader;
			MaterialPixelShader* pixelShader;
			unsigned char perPrimArgCount;
			unsigned char perObjArgCount;
			unsigned char stableArgCount;
			unsigned char customSamplerFlags;
			MaterialShaderArgument* args;
		};

		struct MaterialTechnique
		{
			const char* name;
			unsigned short flags;
			unsigned short passCount;
			MaterialPass passArray[1];
		};

		struct MaterialTechniqueSet
		{
			const char* name;
			unsigned char worldVertFormat;
			unsigned char unused[2];
			MaterialTechniqueSet* remappedTechniqueSet;
			MaterialTechnique* techniques[54];
		};

		struct GfxDrawSurfFields
		{
			unsigned long long unused : 1;
			unsigned long long primarySortKey : 6;
			unsigned long long surfType : 4;
			unsigned long long viewModelRender : 1;
			unsigned long long sceneLightIndex : 8;
			unsigned long long useHeroLighting : 1;
			unsigned long long prepass : 2;
			unsigned long long materialSortedIndex : 12;
			unsigned long long customIndex : 5;
			unsigned long long hasGfxEntIndex : 1;
			unsigned long long reflectionProbeIndex : 8;
			unsigned long long objectId : 15;
		};

		union GfxDrawSurf
		{
			GfxDrawSurfFields fields;
			unsigned long long packed;
		};

		struct MaterialInfo
		{
			char* name;
			unsigned char gameFlags;
			unsigned char sortKey;
			unsigned char textureAtlasRowCount;
			unsigned char textureAtlasColumnCount;
			GfxDrawSurf drawSurf;
			unsigned int surfaceTypeBits;
		};

		struct WaterWritable
		{
			float floatTime;
		};

		struct complex_s
		{
			float real;
			float imag;
		};

		struct water_t
		{
			WaterWritable writable;
			complex_s* H0;
			float* wTerm;
			int M;
			int N;
			float Lx;
			float Lz;
			float gravity;
			float windvel;
			float winddir[2];
			float amplitude;
			float codeConstant[4];
			GfxImage* image;
		};

		union MaterialTextureDefInfo
		{
			GfxImage* image;
			water_t* water;
		};

		struct MaterialTextureDef
		{
			unsigned int nameHash;
			char nameStart;
			char nameEnd;
			unsigned char samplerState;
			unsigned char semantic;
			MaterialTextureDefInfo u;
		};

		struct MaterialConstantDef
		{
			unsigned int nameHash;
			char name[12];
			float literal[4];
		};

		enum GfxSurfaceStatebitOp0 : unsigned int
		{
			GFXS0_SRCBLEND_RGB_SHIFT = 0x0,
			GFXS0_SRCBLEND_RGB_MASK = 0xF,
			GFXS0_DSTBLEND_RGB_SHIFT = 0x4,
			GFXS0_DSTBLEND_RGB_MASK = 0xF0,
			GFXS0_BLENDOP_RGB_SHIFT = 0x8,
			GFXS0_BLENDOP_RGB_MASK = 0x700,
			GFXS0_BLEND_RGB_MASK = 0x7FF,
			GFXS0_ATEST_DISABLE = 0x800,
			GFXS0_ATEST_GT_0 = 0x1000,
			GFXS0_ATEST_LT_128 = 0x2000,
			GFXS0_ATEST_GE_128 = 0x3000,
			GFXS0_ATEST_MASK = 0x3000,
			GFXS0_CULL_SHIFT = 0xE,
			GFXS0_CULL_NONE = 0x4000,
			GFXS0_CULL_BACK = 0x8000,
			GFXS0_CULL_FRONT = 0xC000,
			GFXS0_CULL_MASK = 0xC000,
			GFXS0_SRCBLEND_ALPHA_SHIFT = 0x10,
			GFXS0_SRCBLEND_ALPHA_MASK = 0xF0000,
			GFXS0_DSTBLEND_ALPHA_SHIFT = 0x14,
			GFXS0_DSTBLEND_ALPHA_MASK = 0xF00000,
			GFXS0_BLENDOP_ALPHA_SHIFT = 0x18,
			GFXS0_BLENDOP_ALPHA_MASK = 0x7000000,
			GFXS0_BLEND_ALPHA_MASK = 0x7FF0000,
			GFXS0_COLORWRITE_RGB = 0x8000000,
			GFXS0_COLORWRITE_ALPHA = 0x10000000,
			GFXS0_COLORWRITE_MASK = 0x18000000,
			GFXS0_GAMMAWRITE = 0x40000000,
			GFXS0_POLYMODE_LINE = 0x80000000
		};

		enum GfxSurfaceStatebitOp1 : unsigned int
		{
			GFXS1_DEPTHWRITE = 0x1,
			GFXS1_DEPTHTEST_DISABLE = 0x2,
			GFXS1_DEPTHTEST_SHIFT = 0x2,
			GFXS1_DEPTHTEST_ALWAYS = 0x0,
			GFXS1_DEPTHTEST_LESS = 0x4,
			GFXS1_DEPTHTEST_EQUAL = 0x8,
			GFXS1_DEPTHTEST_LESSEQUAL = 0xC,
			GFXS1_DEPTHTEST_MASK = 0xC,
			GFXS1_POLYGON_OFFSET_SHIFT = 0x4,
			GFXS1_POLYGON_OFFSET_0 = 0x0,
			GFXS1_POLYGON_OFFSET_1 = 0x10,
			GFXS1_POLYGON_OFFSET_2 = 0x20,
			GFXS1_POLYGON_OFFSET_SHADOWMAP = 0x30,
			GFXS1_POLYGON_OFFSET_MASK = 0x30,
			GFXS1_STENCIL_FRONT_ENABLE = 0x40,
			GFXS1_STENCIL_BACK_ENABLE = 0x80,
			GFXS1_STENCIL_MASK = 0xC0,
			GFXS1_STENCIL_FRONT_PASS_SHIFT = 0x8,
			GFXS1_STENCIL_FRONT_FAIL_SHIFT = 0xB,
			GFXS1_STENCIL_FRONT_ZFAIL_SHIFT = 0xE,
			GFXS1_STENCIL_FRONT_FUNC_SHIFT = 0x11,
			GFXS1_STENCIL_FRONT_MASK = 0xFFF00,
			GFXS1_STENCIL_BACK_PASS_SHIFT = 0x14,
			GFXS1_STENCIL_BACK_FAIL_SHIFT = 0x17,
			GFXS1_STENCIL_BACK_ZFAIL_SHIFT = 0x1A,
			GFXS1_STENCIL_BACK_FUNC_SHIFT = 0x1D,
			GFXS1_STENCIL_BACK_MASK = 0xFFF00000,
			GFXS1_STENCILFUNC_FRONTBACK_MASK = 0xE00E0000,
			GFXS1_STENCILOP_FRONTBACK_MASK = 0x1FF1FF00,
		};

		enum GfxStencilOp
		{
			GFXS_STENCILOP_KEEP = 0x0,
			GFXS_STENCILOP_ZERO = 0x1,
			GFXS_STENCILOP_REPLACE = 0x2,
			GFXS_STENCILOP_INCRSAT = 0x3,
			GFXS_STENCILOP_DECRSAT = 0x4,
			GFXS_STENCILOP_INVERT = 0x5,
			GFXS_STENCILOP_INCR = 0x6,
			GFXS_STENCILOP_DECR = 0x7,

			GFXS_STENCILOP_COUNT,
			GFXS_STENCILOP_MASK = 0x7
		};

		struct GfxStatebitsFlags {
			GfxSurfaceStatebitOp0 loadbit0;
			GfxSurfaceStatebitOp1 loadbit1;
		};

		union GfxStateBits
		{
			GfxStatebitsFlags flags;
			unsigned int loadBits[2];
		};

		struct Material
		{
			MaterialInfo info;
			unsigned char stateBitsEntry[54];
			unsigned char textureCount;
			unsigned char constantCount;
			unsigned char stateBitsCount;
			unsigned char stateFlags;
			unsigned char cameraRegion;
			MaterialTechniqueSet* techniqueSet;
			MaterialTextureDef* textureTable;
			MaterialConstantDef* constantTable;
			GfxStateBits* stateBitsTable;
			const char** subMaterials;
		};


		enum snd_alias_type_t
		{
			SAT_UNKNOWN = 0x0,
			SAT_LOADED = 0x1,
			SAT_STREAMED = 0x2,
			SAT_VOICED = 0x3,
			SAT_COUNT = 0x4
		};

		struct StreamedSound
		{
			const char* dir;
			const char* name;
		};

		struct AILSOUNDINFO
		{
			int format;
			const void* data_ptr;
			unsigned int data_len;
			unsigned int rate;
			int bits;
			int channels;
			unsigned int samples;
			unsigned int block_size;
			const void* initial_ptr;
		};

		struct MssSound
		{
			AILSOUNDINFO info;
			char* data;
		};

		struct LoadedSound
		{
			char* name;
			MssSound sound;
		};

		union SoundFileRef
		{
			LoadedSound* loadSnd;
			StreamedSound streamSnd;
		};

		struct SoundFile
		{
			unsigned char type;
			unsigned char exists;
			SoundFileRef u;
		};

		struct MSSSpeakerLevels
		{
			int speaker;
			int numLevels;
			float levels[2];
		};

		struct MSSChannelMap
		{
			int speakerCount;
			MSSSpeakerLevels speakers[6];
		};

		struct SpeakerMap
		{
			bool isDefault;
			const char* name;
			MSSChannelMap channelMaps[2][2];
		};

		struct SndCurve
		{
			const char* filename;
			unsigned short knotCount;
			float knots[16][2];
		};

		// Taken from wii, might be 100% correct
		enum SndChannel
		{
			SND_CHANNEL_PHYSICS = 0,		// CONFIRMED
			SND_CHANNEL_AMBDIST1 = 1,		// CONFIRMED
			SND_CHANNEL_AMBDIST2 = 2,
			SND_CHANNEL_ALARM1,
			SND_CHANNEL_AUTO = 4,		// CONFIRMED
			SND_CHANNEL_AUTO2,
			SND_CHANNEL_AUTO2D = 6,		// CONFIRMED
			SND_CHANNEL_AUTODOG,
			SND_CHANNEL_EXPLOSIONDIST1 = 8,	// CONFIRMED => should become EFFECTS1
			SND_CHANNEL_EXPLOSIONDIST2 = 9,	// CONFIRMED => should become AUTO
			SND_CHANNEL_EXPLOSIVEIMPACT,
			SND_CHANNEL_ELEMENT = 11,	// CONFIRMED
			SND_CHANNEL_ELEMENT_INT = 12,	// CONFIRMED => should become AUTO
			SND_CHANNEL_ELEMENT_EXT = 13,	// CONFIRMED => should become AUTO
			SND_CHANNEL_BULLETIMPACT = 14,	// CONFIRMED
			SND_CHANNEL_BULLETFLESH1,
			SND_CHANNEL_BULLETFLESH2,
			SND_CHANNEL_BULLETWHIZBY,
			SND_CHANNEL_VEHICLE,
			SND_CHANNEL_VEHICLELIMITED,
			SND_CHANNEL_MENU = 20,	// CONFIRMED
			SND_CHANNEL_BODY = 21,	// CONFIRMED
			SND_CHANNEL_BODY2D,
			SND_CHANNEL_RELOAD,
			SND_CHANNEL_RELOAD2D,
			SND_CHANNEL_ITEM,
			SND_CHANNEL_EXPLOSION1,
			SND_CHANNEL_EXPLOSION2,
			SND_CHANNEL_EXPLOSION3,
			SND_CHANNEL_EXPLOSION4,
			SND_CHANNEL_EXPLOSION5,
			SND_CHANNEL_EFFECTS1, // 31
			SND_CHANNEL_EFFECTS2,
			SND_CHANNEL_EFFECTS3,
			SND_CHANNEL_EFFECTS2D1,
			SND_CHANNEL_EFFECTS2D2,
			SND_CHANNEL_NORESTRICT,
			SND_CHANNEL_NORESTRICT2D,
			SND_CHANNEL_AIRCRAFT,
			SND_CHANNEL_VEHICLE2D,
			SND_CHANNEL_WEAPON_DIST,
			SND_CHANNEL_WEAPON_MID,
			SND_CHANNEL_WEAPON,
			SND_CHANNEL_WEAPON2D, // 43
			SND_CHANNEL_NONSHOCK,
			SND_CHANNEL_NONSHOCK2,
			SND_CHANNEL_GRONDO3D,
			SND_CHANNEL_GRONDO2D,
			SND_CHANNEL_VOICE,	// 48
			SND_CHANNEL_LOCAL,
			SND_CHANNEL_LOCAL2,
			SND_CHANNEL_LOCAL3,
			SND_CHANNEL_AMBIENT = 52,	// CONFIRMED
			SND_CHANNEL_HURT = 53,	// CONFIRMED
			SND_CHANNEL_PLAYER1,
			SND_CHANNEL_PLAYER2,
			SND_CHANNEL_MUSIC,
			SND_CHANNEL_MUSICNOPAUSE,
			SND_CHANNEL_MISSION,
			SND_CHANNEL_CRITICAL,	// 59
			SND_CHANNEL_ANNOUNCER,
			SND_CHANNEL_SHELLSHOCK,

			SND_CHANNEL_COUNT
		};

		union SoundAliasFlags
		{
#pragma warning(push)
#pragma warning(disable: 4201)
			struct
			{
				unsigned int looping : 1;		// & 1	/ 0x1			/ 0000 0000 0000 0001
				unsigned int isMaster : 1;		// & 2	/ 0x2			/ 0000 0000 0000 0010
				unsigned int isSlave : 1;		// & 4	/ 0x4			/ 0000 0000 0000 0100
				unsigned int fullDryLevel : 1;	//	& 8	/ 0x8			/ 0000 0000 0000 1000
				unsigned int noWetLevel : 1;	// & 16	/ 0x10			/ 0000 0000 0001 0000
				unsigned int unknown : 1;		// & 32	/ 0x20			/ 0000 0000 0010 0000
				unsigned int unk_is3D : 1;		// & 64	/ 0x40			/ 0000 0000 0100 0000		// CONFIRMED IW4 IW5
				unsigned int type : 2;			// & 384	/ 0x180		/ 0000 0001 1000 0000		// CONFIRMED IW4 IW5
				unsigned int channel : 6;		// & 32256	/ 0x7E00	/ 0111 1110 0000 0000		// CONFIRMED IW4 IW5
			};
#pragma warning(pop)
			unsigned int intValue;
		};

		struct snd_alias_t
		{
			const char* aliasName;
			const char* subtitle;
			const char* secondaryAliasName;
			const char* chainAliasName;
			const char* mixerGroup;
			SoundFile* soundFile;
			int sequence;
			float volMin;
			float volMax;
			int volModIndex;
			float pitchMin;
			float pitchMax;
			float distMin;
			float distMax;
			float velocityMin;
			SoundAliasFlags flags;
			unsigned char masterPriority;
			float masterPercentage;
			float slavePercentage;
			float probability;
			float lfePercentage;
			float centerPercentage;
			int startDelay;
			SndCurve* volumeFalloffCurve;
			float envelopMin;
			float envelopMax;
			float envelopPercentage;
			SpeakerMap* speakerMap;
		};

		struct snd_alias_list_t
		{
			const char* aliasName;
			snd_alias_t* head;
			int count;
		};

		struct XAsset
		{
			XAssetType type;
			XAssetHeader header;
		};

		struct XAssetEntry
		{
			XAsset asset;
			char zoneIndex;
			volatile char inuseMask;
			bool printedMissingAsset;
			unsigned __int16 nextHash;
			unsigned __int16 nextOverride;
		};

		struct TempPriority
		{
			void* threadHandle;
			int oldPriority;
		};

		struct FastCriticalSection
		{
			volatile long readCount;
			volatile long writeCount;
			TempPriority tempPriority;
		};

		enum errorParm_t
		{
			ERR_FATAL = 0x0,
			ERR_DROP = 0x1,
			ERR_SERVERDISCONNECT = 0x2,
			ERR_DISCONNECT = 0x3,
			ERR_SCRIPT = 0x4,
			ERR_SCRIPT_DROP = 0x5,
			ERR_LOCALIZATION = 0x6,
			ERR_MAPLOADERRORSUMMARY = 0x7
		};

		enum svscmd_type
		{
			SV_CMD_CAN_IGNORE,
			SV_CMD_RELIABLE,
		};

		enum LocalClientNum_t
		{
			LOCAL_CLIENT_0 = 0,
			LOCAL_CLIENT_1 = 1,
			LOCAL_CLIENT_2 = 2,
			LOCAL_CLIENT_3 = 3,
			LOCAL_CLIENT_LAST = 3,
			LOCAL_CLIENT_COUNT = 4,
		};

		enum fsMode_t
		{
			FS_READ,
			FS_WRITE,
			FS_APPEND,
			FS_APPEND_SYNC,
		};

		enum msgLocErrType_t
		{
			LOCMSG_SAFE,
			LOCMSG_NOERR,
		};

		enum PMem_Source
		{
			PMEM_SOURCE_EXTERNAL,
			PMEM_SOURCE_DATABASE,
			PMEM_SOURCE_MOVIE,
			PMEM_SOURCE_SCRIPT,
		};

		struct cmd_function_t
		{
			cmd_function_t* next;
			const char* name;
			const char* autoCompleteDir;
			const char* autoCompleteExt;
			void(__cdecl* function)();
			int flags;
		};

		struct CmdArgs
		{
			int nesting;
			int localClientNum[8];
			int controllerIndex[8];
			int argc[8];
			const char** argv[8];
		};

		struct msg_t
		{
			int overflowed;
			int readOnly;
			char* data;
			char* splitData;
			int maxsize;
			int cursize;
			int splitSize;
			int readcount;
			int bit;
			int lastEntityRef;
		};

		struct XZoneInfo
		{
			const char* name;
			int allocFlags;
			int freeFlags;
		};

		struct scr_entref_t
		{
			unsigned __int16 entnum;
			unsigned __int16 classnum;
		};

		typedef void(*BuiltinFunction)();
		typedef void(*BuiltinMethod)(scr_entref_t);

		typedef unsigned __int16 scr_string_t;

		enum
		{
			VAR_UNDEFINED = 0x0,
			VAR_BEGIN_REF = 0x1,
			VAR_POINTER = 0x1,
			VAR_STRING = 0x2,
			VAR_ISTRING = 0x3,
			VAR_VECTOR = 0x4,
			VAR_END_REF = 0x5,
			VAR_FLOAT = 0x5,
			VAR_INTEGER = 0x6,
			VAR_CODEPOS = 0x7,
			VAR_PRECODEPOS = 0x8,
			VAR_FUNCTION = 0x9,
			VAR_BUILTIN_FUNCTION = 0xA,
			VAR_BUILTIN_METHOD = 0xB,
			VAR_STACK = 0xC,
			VAR_ANIMATION = 0xD,
			VAR_PRE_ANIMATION = 0xE,
			VAR_THREAD = 0xF,
			VAR_NOTIFY_THREAD = 0x10,
			VAR_TIME_THREAD = 0x11,
			VAR_CHILD_THREAD = 0x12,
			VAR_OBJECT = 0x13,
			VAR_DEAD_ENTITY = 0x14,
			VAR_ENTITY = 0x15,
			VAR_ARRAY = 0x16,
			VAR_DEAD_THREAD = 0x17,
			VAR_COUNT = 0x18,
			VAR_FREE = 0x18,
			VAR_THREAD_LIST = 0x19,
			VAR_ENDON_LIST = 0x1A,
			VAR_TOTAL_COUNT = 0x1B,
		};

		struct VariableStackBuffer
		{
			const char* pos;
			unsigned __int16 size;
			unsigned __int16 bufLen;
			unsigned __int16 localId;
			char time;
			char buf[1];
		};

		union VariableUnion
		{
			int intValue;
			float floatValue;
			unsigned int stringValue;
			const float* vectorValue;
			const char* codePosValue;
			unsigned int pointerValue;
			VariableStackBuffer* stackValue;
			unsigned int entityId;
		};

		struct VariableValue
		{
			VariableUnion u;
			int type;
		};

		struct function_stack_t
		{
			const char* pos;
			unsigned int localId;
			unsigned int localVarCount;
			VariableValue* top;
			VariableValue* startTop;
		};

		struct function_frame_t
		{
			function_stack_t fs;
			int topType;
		};

		struct scrVmPub_t
		{
			unsigned int* localVars;
			VariableValue* maxStack;
			int function_count;
			function_frame_t* function_frame;
			VariableValue* top;
			unsigned int inparamcount;
			unsigned int outparamcount;
			unsigned int breakpointOutparamcount;
			bool showError;
			function_frame_t function_frame_start[32];
			VariableValue stack[2048];
		};

		struct HunkUser
		{
			HunkUser* current;
			HunkUser* next;
			int maxSize;
			int end;
			int pos;
			const char* name;
			bool fixed;
			int type;
			unsigned char buf[1];
		};

		static_assert(sizeof(HunkUser) == 0x24);

		struct scrVarPub_t
		{
			const char* fieldBuffer;
			bool evaluate;
			unsigned int time;
			unsigned int timeArrayId;
			unsigned int pauseArrayId;
			unsigned int notifyArrayId;
			unsigned int objectStackId;
			unsigned int levelId;
			unsigned int gameId;
			unsigned int animId;
			unsigned int freeEntList;
			unsigned int tempVariable;
			unsigned int numScriptValues[2];
			bool bInited;
			bool abort;
			unsigned __int16 savecount;
			unsigned __int16 savecountMark;
			unsigned int entId;
			unsigned int entFieldName;
			unsigned int checksum;
			HunkUser* programHunkUser;
			HunkUser* canonicalStringHunkUser;
			const char* programBuffer;
			unsigned __int16 saveIdMap[36864];
			unsigned __int16 saveIdMapRev[36864];
		};

		static_assert(sizeof(scrVarPub_t) == 0x24058);

		struct scr_classStruct_t
		{
			unsigned __int16 id;
			unsigned __int16 entArrayId;
			char charId;
			const char* name;
		};

		enum : std::uint16_t
		{
			DVAR_NONE = 0,
			DVAR_ARCHIVE = 1 << 0,
			DVAR_LATCH = 1 << 1,
			DVAR_CHEAT = 1 << 2,
			DVAR_CODINFO = 1 << 3,
			DVAR_SCRIPTINFO = 1 << 4,
			DVAR_SAVED = 1 << 6,
			DVAR_USERINFO = 1 << 9,
			DVAR_SERVERINFO = 1 << 10,
			DVAR_INIT = 1 << 11,
			DVAR_ROM = 1 << 13,
			DVAR_AUTOEXEC = 1 << 15,
		}; // Incomplete

		enum dvar_type : std::int8_t
		{
			DVAR_TYPE_BOOL = 0x0,
			DVAR_TYPE_FLOAT = 0x1,
			DVAR_TYPE_FLOAT_2 = 0x2,
			DVAR_TYPE_FLOAT_3 = 0x3,
			DVAR_TYPE_FLOAT_4 = 0x4,
			DVAR_TYPE_INT = 0x5,
			DVAR_TYPE_ENUM = 0x6,
			DVAR_TYPE_STRING = 0x7,
			DVAR_TYPE_COLOR = 0x8,
			DVAR_TYPE_FLOAT_3_COLOR = 0x9,

			DVAR_TYPE_COUNT = 0xA
		};

		union DvarValue
		{
			bool enabled;
			int integer;
			unsigned int unsignedInt;
			float value;
			float vector[4];
			const char* string;
			char color[4];
		};

		static_assert(sizeof(DvarValue) == 0x10);

		struct enum_limit
		{
			int stringCount;
			const char** strings;
		};

		struct int_limit
		{
			int min;
			int max;
		};

		struct float_limit
		{
			float min;
			float max;
		};

		union DvarLimits
		{
			enum_limit enumeration;
			int_limit integer;
			float_limit value;
			float_limit vector;
		};

		struct dvar_t
		{
			const char* name;
			unsigned int flags;
			char type;
			bool modified;
			DvarValue current;
			DvarValue latched;
			DvarValue reset;
			DvarLimits domain;
			bool (*domainFunc)(dvar_t*, DvarValue);
			dvar_t* hashNext;
		};

		struct DvarDumpInfo
		{
			int count;
			int channel;
			const char* match;
		};

		struct Bounds
		{
			float midPoint[3];
			float halfSize[3];
		};



		struct TriggerModel
		{
			int contents;
			unsigned short hullCount;
			unsigned short firstHull;
		};

		struct TriggerHull
		{
			Bounds bounds;
			int contents;
			unsigned short slabCount;
			unsigned short firstSlab;
		};

		struct TriggerSlab
		{
			float dir[3];
			float midPoint;
			float halfSize;
		};

		struct MapTriggers
		{
			unsigned int count;
			TriggerModel* models;
			unsigned int hullCount;
			TriggerHull* hulls;
			unsigned int slabCount;
			TriggerSlab* slabs;
		};

		struct ClientTriggerAabbNode
		{
			Bounds bounds;
			unsigned short firstChild;
			unsigned short childCount;
		};

		struct ClientTriggers
		{
			MapTriggers trigger;
			unsigned short numClientTriggerNodes;
			ClientTriggerAabbNode* clientTriggerAabbTree;
			unsigned int triggerStringLength;
			char* triggerString;
			unsigned short* triggerStringOffsets;
			unsigned char* triggerType;
			vec3_t* origins;
			float* scriptDelay;
			short* audioTriggers;
		};

		struct Stage
		{
			const char* name;
			float origin[3];
			unsigned short triggerIndex;
			unsigned char sunPrimaryLightIndex;
		};

		struct MapEnts
		{
			const char* name;
			char* entityString;
			int numEntityChars;
			MapTriggers trigger;
			ClientTriggers clientTrigger;
		};

		enum TraceHitType
		{
			TRACE_HITTYPE_NONE = 0x0,
			TRACE_HITTYPE_ENTITY = 0x1,
			TRACE_HITTYPE_DYNENT_MODEL = 0x2,
			TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
			TRACE_HITTYPE_GLASS = 0x4,
		};

		struct trace_t
		{
			float fraction;
			float normal[3];
			int surfaceFlags;
			int contents;
			const char* material;
			TraceHitType hitType;
			unsigned __int16 hitId;
			unsigned __int16 modelIndex;
			unsigned __int16 partName;
			unsigned __int16 partGroup;
			bool allsolid;
			bool startsolid;
			bool walkable;
		};

		static_assert(sizeof(trace_t) == 0x2C);

		struct pml_t
		{
			float forward[3];
			float right[3];
			float up[3];
			float frametime;
			int msec;
			int walking;
			int groundPlane;
			int almostGroundPlane;
			trace_t groundTrace;
			float impactSpeed;
			float previous_origin[3];
			float previous_velocity[3];
			unsigned int holdrand;
		};

		static_assert(sizeof(pml_t) == 0x84);

		struct usercmd_s
		{
			int serverTime;
			int buttons;
			int angles[3];
			unsigned int weapon;
			unsigned int offHand;
			char forwardmove;
			char rightmove;
			unsigned __int16 airburstMarkDistance;
			unsigned __int16 meleeChargeEnt;
			unsigned char meleeChargeDist;
			char selectedLoc[2];
			unsigned char selectedLocAngle;
			char remoteControlAngles[2];
			char remoteControlMove[3];
		};

		static_assert(sizeof(usercmd_s) == 0x2C);

		enum PlayerHandIndex
		{
			WEAPON_HAND_RIGHT = 0,
			WEAPON_HAND_LEFT = 1,
			NUM_WEAPON_HANDS = 2,
			WEAPON_HAND_DEFAULT = 0,
		};

		struct Weapon_s
		{
			unsigned int weaponIdx : 8;
			unsigned int weaponVariation : 4;
			unsigned int weaponScopes : 3;
			unsigned int weaponUnderBarrels : 2;
			unsigned int weaponOthers : 4;
			unsigned int scopeVariation : 3;
			unsigned int padding : 8;
		};

		union Weapon
		{
			Weapon_s __s0;
			unsigned int data;
		};

		static_assert(sizeof(Weapon) == 4);

		struct weaponParms
		{
			float forward[3];
			float right[3];
			float up[3];
			float muzzleTrace[3];
			float gunForward[3];
			Weapon weapon;
			bool isAlternate;
			const void* weapDef;
			const void* weapCompleteDef;
		};

		static_assert(sizeof(weaponParms) == 0x4C);

		enum hitLocation_t
		{
			HITLOC_NONE = 0x0,
			HITLOC_HELMET = 0x1,
			HITLOC_HEAD = 0x2,
			HITLOC_NECK = 0x3,
			HITLOC_TORSO_UPR = 0x4,
			HITLOC_TORSO_LWR = 0x5,
			HITLOC_R_ARM_UPR = 0x6,
			HITLOC_L_ARM_UPR = 0x7,
			HITLOC_R_ARM_LWR = 0x8,
			HITLOC_L_ARM_LWR = 0x9,
			HITLOC_R_HAND = 0xA,
			HITLOC_L_HAND = 0xB,
			HITLOC_R_LEG_UPR = 0xC,
			HITLOC_L_LEG_UPR = 0xD,
			HITLOC_R_LEG_LWR = 0xE,
			HITLOC_L_LEG_LWR = 0xF,
			HITLOC_R_FOOT = 0x10,
			HITLOC_L_FOOT = 0x11,
			HITLOC_GUN = 0x12,
			HITLOC_SHIELD = 0x13,

			HITLOC_NUM = 0x14,
		};

		enum ViewLockTypes
		{
			PLAYERVIEWLOCK_NONE = 0x0,
			PLAYERVIEWLOCK_FULL = 0x1,
			PLAYERVIEWLOCK_WEAPONJITTER = 0x2,
			PLAYERVIEWLOCKCOUNT = 0x3,
		};

		enum
		{
			PMF_PRONE = 0x1,
			PMF_DUCKED = 0x2,
			PMF_MANTLE = 0x4,
			PMF_LADDER = 0x8,
			PMF_SIGHT_AIMING = 0x10,
			PMF_BACKWARDS_RUN = 0x20,
			PMF_WALKING = 0x40,
			PMF_TIME_HARDLANDING = 0x80,
			PMF_TIME_KNOCKBACK = 0x100,
			PMF_PRONEMOVE_OVERRIDDEN = 0x200,
			PMF_RESPAWNED = 0x400,
			PMF_FROZEN = 0x800,
			PMF_LADDER_FALL = 0x1000,
			PMF_JUMPING = 0x2000,
			PMF_SPRINTING = 0x4000,
			PMF_SHELLSHOCKED = 0x8000,
			PMF_MELEE_CHARGE = 0x10000,
			PMF_NO_SPRINT = 0x20000,
			PMF_NO_JUMP = 0x40000,
			PMF_REMOTE_CONTROLLING = 0x80000,
			PMF_ANIM_SCRIPTED = 0x100000,
			PMF_DIVING = 0x400000,
		};

		struct playerState_s
		{
			int commandTime;
			int pm_type;
			int pm_time;
			int pm_flags;
			int otherFlags;
			int linkFlags;
			int bobCycle;
			float origin[3];
			float velocity[3];
			int grenadeTimeLeft;
			int throwbackGrenadeOwner;
			int throwbackGrenadeTimeLeft;
			unsigned int throwbackWeapon;
			int remoteEyesEnt;
			int remoteEyesTagname;
			int remoteControlEnt;
			int remoteTurretEnt;
			int foliageSoundTime;
			int gravity;
			int speed;
			int delta_angles;
			int groundEntityNum;
			float vLadderVec[3];
			int jumpTime;
			float jumpOriginZ;
			unsigned char __pad0[0x470];
			unsigned int perks[0x2];
			unsigned int perkSlots[0x9];
			unsigned char __pad1[0x2DF4];
		};

		static_assert(sizeof(playerState_s) == 0x330C);

		struct pmove_t
		{
			playerState_s* ps;
			usercmd_s cmd;
			usercmd_s oldcmd;
			int tracemask;
			int numtouch;
			int touchents[32];
			Bounds bounds;
			float speed;
			int proneChange;
			float maxSprintTimeMultiplier;
			bool mantleStarted;
			float mantleEndPos[3];
			int mantleDuration;
			int viewChangeTime;
			float viewChange;
			float fTorsoPitch;
			float fWaistPitch;
			int remoteTurretFireTime;
			int lastUpdateCMDServerTime;
			unsigned char handler;
		};

		static_assert(sizeof(pmove_t) == 0x138);

		enum sessionState_t
		{
			SESS_STATE_PLAYING = 0x0,
			SESS_STATE_DEAD = 0x1,
			SESS_STATE_SPECTATOR = 0x2,
			SESS_STATE_INTERMISSION = 0x3,
		};

		enum clientConnected_t
		{
			CON_DISCONNECTED = 0x0,
			CON_CONNECTING = 0x1,
			CON_CONNECTED = 0x2,
		};

		struct clientSession_t
		{
			sessionState_t sessionState;
			int forceSpectatorClient;
			int killCamEntity;
			int killCamLookAtEntity;
			int status_icon;
			int archiveTime;
			int score;
			int deaths;
			int kills;
			int assists;
			unsigned __int16 scriptPersId;
			clientConnected_t connected;
			unsigned char __pad0[0x290];
		};

		static_assert(sizeof(clientSession_t) == 0x2C0);

		struct gclient_s
		{
			playerState_s ps;
			clientSession_t sess;
			int flags;
			unsigned char __pad0[0x3B0];
		};

		static_assert(sizeof(gclient_s) == 0x3980);

		enum
		{
			FL_GODMODE = 0x1,
			FL_DEMI_GODMODE = 0x2,
			FL_NOTARGET = 0x4,
			FL_NO_KNOCKBACK = 0x8,
			FL_NO_RADIUS_DAMAGE = 0x10,
			FL_SUPPORTS_LINKTO = 0x1000,
			FL_NO_AUTO_ANIM_UPDATE = 0x2000,
			FL_GRENADE_TOUCH_DAMAGE = 0x4000,
			FL_STABLE_MISSILES = 0x20000,
			FL_REPEAT_ANIM_UPDATE = 0x40000,
			FL_VEHICLE_TARGET = 0x80000,
			FL_GROUND_ENT = 0x100000,
			FL_CURSOR_HINT = 0x200000,
			FL_MISSILE_ATTRACTOR = 0x800000,
			FL_WEAPON_BEING_GRABBED = 0x1000000,
			FL_DELETE = 0x2000000,
			FL_BOUNCE = 0x4000000,
			FL_MOVER_SLIDE = 0x8000000
		};

		enum entityType
		{
			ET_GENERAL,
			ET_PLAYER,
			ET_ITEM,
			ET_MISSILE,
			ET_INVISIBLE,
			ET_SCRIPTMOVER,
			ET_SOUND_BLEND,
			ET_PRIMARY_LIGHT,
			ET_TURRET,
			ET_VEHICLE,
			ET_VEHICLE_COLLMAP,
			ET_VEHICLE_CORPSE,
			ET_VEHICLE_SPAWNER,
			ET_ACTOR,
			ET_ACTOR_SPAWNER,
			ET_ACTOR_CORPSE,
		};

		struct entityState_s
		{
			int number;
		};

		struct EntHandle
		{
			unsigned __int16 number;
			unsigned __int16 infoIndex;
		};

		struct gentity_s
		{
			entityState_s s;
			unsigned char __pad0[0x154];
			gclient_s* client; // 0x158
			unsigned char __pad1[0x28];
			int flags;
			int eventTime;
			int clipmask;
			int processedFrame;
			EntHandle parent;
			int nextthink;
			int health;
			int maxHealth;
			int damage;
			int count;
			unsigned char __pad2[0xC8];
		};

		static_assert(sizeof(gentity_s) == 0x274);

		struct missileFireParms
		{
			gentity_s* target;
			float targetPosOrOffset[3];
			float autoDetonateTime;
			bool lockon;
			bool topFire;
		};

		static_assert(sizeof(missileFireParms) == 0x18);

		enum clientState_t
		{
			CS_FREE = 0,
			CS_ZOMBIE = 1,
			CS_RECONNECTING = 2,
			CS_CONNECTED = 3,
			CS_CLIENTLOADING = 4,
			CS_ACTIVE = 5,
		};

		enum netsrc_t
		{
			NS_CLIENT1 = 0x0,
			NS_CLIENT2 = 0x1,
			NS_CLIENT3 = 0x2,
			NS_CLIENT4 = 0x3,
			NS_MAXCLIENTS = 0x4,
			NS_SERVER = 0x4,
			NS_PACKET = 0x5,
			NS_INVALID_NETSRC = 0x6,
		};

		enum netadrtype_t
		{
			NA_BOT = 0x0,
			NA_BAD = 0x1,
			NA_LOOPBACK = 0x2,
			NA_BROADCAST = 0x3,
			NA_IP = 0x4,
		};

		struct netadr_s
		{
			netadrtype_t type;
			unsigned char ip[4];
			unsigned __int16 port;
			unsigned char ipx[10];
			unsigned int addrHandleIndex;
		};

		static_assert(sizeof(netadr_s) == 24);

		struct DeferredMsg
		{
			netadr_s addr;
			unsigned char data[1262];
			int datalen;
		};

		static_assert(sizeof(DeferredMsg) == 0x50C);

		struct DeferredQueue
		{
			DeferredMsg msgs[16];
			volatile long get;
			volatile long send;
		};

		static_assert(sizeof(DeferredQueue) == 0x50C8);

		struct netProfileInfo_t // Unused
		{
			unsigned char __pad0[0x5E0];
		};

		struct netchan_t
		{
			int outgoingSequence;
			netsrc_t sock;
			int dropped;
			int incomingSequence;
			netadr_s remoteAddress;
			int qport;
			int fragmentSequence;
			int fragmentLength;
			unsigned char* fragmentBuffer;
			int fragmentBufferSize;
			int unsentFragments;
			int unsentFragmentStart;
			int unsentLength;
			unsigned char* unsentBuffer;
			int unsentBufferSize;
			netProfileInfo_t prof;
		};

		static_assert(sizeof(netchan_t) == 0x630);

		struct clientHeader_t
		{
			clientState_t state;
			int sendAsActive;
			int deltaMessage;
			int rateDealyed;
			int hasAckedBaselineData;
			int hugeSnapshotSent;
			netchan_t netchan;
			float predictedOrigin[3];
			int predictedOriginServerTime;
			int migrationState;
			float predictedVehicleOrigin[3];
			int predictedVehicleServerTime;
		};

		static_assert(sizeof(clientHeader_t) == 0x66C);

		enum objectiveState_t
		{
			OBJST_EMPTY = 0x0,
			OBJST_ACTIVE = 0x1,
			OBJST_INVISIBLE = 0x2,
			OBJST_DONE = 0x3,
			OBJST_CURRENT = 0x4,
			OBJST_FAILED = 0x5,
			OBJST_NUMSTATES = 0x6,
		};

		struct objective_t
		{
			objectiveState_t state;
			float origin[3];
			int entNum;
			int teamNum;
			int clientNum;
			int invertVisibilityByClientNum;
			int icon;
		};

		static_assert(sizeof(objective_t) == 0x24);

		struct level_locals_t
		{
			gclient_s* clients;
			gentity_s* gentities;
			int num_entities;
			gentity_s* firstFreeEnt;
			gentity_s* lastFreeEnt;
			void* turrets;
			int initializing;
			int clientIsSpawning;
			objective_t objectives[32];
			int maxclients;
			int framenum;
			int time;
			unsigned char __pad0[0x2BD4];
		};

		static_assert(sizeof(level_locals_t) == 0x3080);

		enum CriticalSection
		{
			CRITSECT_CONSOLE = 0x0,
			CRITSECT_CBUF = 0x1F,
			CRITSECT_COUNT = 0x27,
		};

		enum ScopedCriticalSectionType
		{
			SCOPED_CRITSECT_NORMAL = 0x0,
			SCOPED_CRITSECT_DISABLED = 0x1,
			SCOPED_CRITSECT_RELEASE = 0x2,
			SCOPED_CRITSECT_TRY = 0x3,
		};

		struct fileInIwd_s
		{
			unsigned int pos;
			char* name;
			fileInIwd_s* next;
		};

		struct iwd_t
		{
			char iwdFilename[256];
			char iwdBasename[256];
			char iwdGamename[256];
			char* handle;
			int checksum;
			int pure_checksum;
			volatile int hasOpenFile;
			int numfiles;
			char referenced;
			unsigned int hashSize;
			fileInIwd_s** hashTable;
			fileInIwd_s* buildBuffer;
		};

		struct directory_t
		{
			char path[256];
			char gamedir[256];
		};

		struct searchpath_s
		{
			searchpath_s* next;
			iwd_t* iwd;
			directory_t* dir;
			int bLocalized;
			int ignore;
			int ignorePureCheck;
			int language;
		};

		enum
		{
			THREAD_CONTEXT_MAIN,
			THREAD_CONTEXT_BACKEND,
			THREAD_CONTEXT_WORKER0,
			THREAD_CONTEXT_WORKER1,
			THREAD_CONTEXT_SERVER,
			THREAD_CONTEXT_CINEMATIC,
			THREAD_CONTEXT_DATABASE,
			THREAD_CONTEXT_STREAM,
			THREAD_CONTEXT_STATS_WRITE,
			THREAD_CONTEXT_MJPEG,
			THREAD_CONTEXT_COUNT,
		};

		enum FsThread
		{
			FS_THREAD_MAIN = 0x0,
			FS_THREAD_STREAM = 0x1,
			FS_THREAD_DATABASE = 0x2,
			FS_THREAD_BACKEND = 0x3,
			FS_THREAD_SERVER = 0x4,
			FS_THREAD_COUNT = 0x5,
			FS_THREAD_INVALID = 0x6,
		};

		union qfile_gus
		{
			_iobuf* o;
			char* z;
		};

		struct qfile_us
		{
			qfile_gus file;
			int iwdIsClone;
		};

		struct fileHandleData_t
		{
			qfile_us handleFiles;
			int handleSync;
			int fileSize;
			int zipFilePos;
			iwd_t* zipFile;
			int streamed;
			char name[256];
		};

		static_assert(sizeof(fileHandleData_t) == 0x11C);

		struct GfxSky
		{
			int skySurfCount;
			int* skyStartSurfs;
			GfxImage* skyImage;
			unsigned char skySamplerState;
		};

		struct cplane_s
		{
			float normal[3];
			float dist;
			unsigned char type;
			unsigned char pad[3];
		};

		struct cbrushside_t
		{
			cplane_s* plane;
			unsigned short materialNum;
			unsigned char firstAdjacentSideOffset;
			unsigned char edgeCount;
		};

		struct cbrushWrapper_t
		{
			unsigned short numsides;
			unsigned short glassPieceIndex;
			cbrushside_t* sides;
			unsigned char* baseAdjacentSide;
			short axialMaterialNum[2][3];
			unsigned char firstAdjacentSideOffsets[2][3];
			unsigned char edgeCount[2][3];
		};

		struct BrushWrapper
		{
			Bounds bounds;
			cbrushWrapper_t brush;
			int totalEdgeCount;
			cplane_s* planes;
		};

		enum PhysicsGeomType
		{
			PHYS_GEOM_NONE = 0x0,
			PHYS_GEOM_BOX = 0x1,
			PHYS_GEOM_BRUSHMODEL = 0x2,
			PHYS_GEOM_BRUSH = 0x3,
			PHYS_GEOM_COLLMAP = 0x4,
			PHYS_GEOM_CYLINDER = 0x5,
			PHYS_GEOM_CAPSULE = 0x6,
			PHYS_GEOM_GLASS = 0x7,
			PHYS_GEOM_COUNT = 0x8
		};

		struct PhysGeomInfo
		{
			BrushWrapper* brushWrapper;
			PhysicsGeomType type;
			float orientation[3][3];
			Bounds bounds;
		};

		struct PhysMass
		{
			float centerOfMass[3];
			float momentsOfInertia[3];
			float productsOfInertia[3];
		};

		struct PhysCollmap
		{
			const char* name;
			unsigned int count;
			PhysGeomInfo* geoms;
			PhysMass mass;
			Bounds bounds;
		};

		struct GfxWorldDpvsPlanes
		{
			int cellCount;
			cplane_s* planes;
			unsigned short* nodes;
			unsigned int* sceneEntCellBits;
		};

		struct GfxAabbTree
		{
			Bounds bounds;
			unsigned short childCount;
			unsigned short surfaceCount;
			unsigned short startSurfIndex;
			unsigned short surfaceCountNoDecal;
			unsigned short startSurfIndexNoDecal;
			unsigned short smodelIndexCount;
			unsigned short* smodelIndexes;
			int childrenOffset;
		};

		struct GfxCellTree
		{
			GfxAabbTree* aabbTree;
		};

		struct GfxCellTreeCount
		{
			int aabbTreeCount;
		};

		struct GfxPortalWritable
		{
			bool isQueued;
			bool isAncestor;
			unsigned char recursionDepth;
			unsigned char hullPointCount;
			float(*hullPoints)[2];
			struct GfxPortal* queuedParent;
		};

		struct DpvsPlane
		{
			float coeffs[4];
		};

		struct GfxPortal
		{
			GfxPortalWritable writable;
			DpvsPlane plane;
			vec3_t* vertices;
			unsigned short cellIndex;
			unsigned char vertexCount;
			float hullAxis[2][3];
		};

		struct GfxCell
		{
			Bounds bounds;
			int portalCount;
			GfxPortal* portals;
			unsigned char reflectionProbeCount;
			unsigned char* reflectionProbes;
			unsigned char reflectionProbeReferenceCount;
			unsigned char* reflectionProbeReferences;
		};

		struct GfxReflectionProbe
		{
			float origin[3];
		};

		struct GfxReflectionProbeReferenceOrigin
		{
			float origin[3];
		};

		struct GfxReflectionProbeReference
		{
			unsigned char index;
		};

		struct GfxLightmapArray
		{
			GfxImage* primary;
			GfxImage* secondary;
		};

		union PackedUnitVec
		{
			unsigned int packed;
			char array[4];
		};

		union GfxColor
		{
			unsigned int packed;
			unsigned char array[4];
		};

		struct GfxPackedPlacement
		{
			float origin[3];
			float axis[3][3];
			float scale;
		};

		struct GfxWorldVertex
		{
			float xyz[3];
			float binormalSign;
			GfxColor color;
			float texCoord[2];
			float lmapCoord[2];
			PackedUnitVec normal;
			PackedUnitVec tangent;
		};

		struct GfxWorldVertexData
		{
			GfxWorldVertex* vertices;
			void* worldVb;
		};

		struct GfxWorldVertexLayerData
		{
			unsigned char* data;
			void* layerVb;
		};

		struct GfxWorldDraw
		{
			unsigned int reflectionProbeCount;
			GfxImage** reflectionProbes;
			GfxReflectionProbe* reflectionProbeOrigins;
			GfxTexture* reflectionProbeTextures;
			unsigned int reflectionProbeReferenceCount;
			GfxReflectionProbeReferenceOrigin* reflectionProbeReferenceOrigins;
			GfxReflectionProbeReference* reflectionProbeReferences;
			int lightmapCount;
			GfxLightmapArray* lightmaps;
			GfxTexture* lightmapPrimaryTextures;
			GfxTexture* lightmapSecondaryTextures;
			GfxImage* lightmapOverridePrimary;
			GfxImage* lightmapOverrideSecondary;
			unsigned int vertexCount;
			GfxWorldVertexData vd;
			unsigned int vertexLayerDataSize;
			GfxWorldVertexLayerData vld;
			unsigned int indexCount;
			unsigned short* indices;
		};

		struct GfxLightGridEntry
		{
			unsigned short colorsIndex;
			unsigned char primaryLightIndex;
			unsigned char needsTrace;
		};

		struct GfxLightGridColors
		{
			unsigned char rgb[56][3];
		};

		struct GfxLightGrid
		{
			bool hasLightRegions;
			bool useSkyForLowZ;
			unsigned int lastSunPrimaryLightIndex;
			unsigned short mins[3];
			unsigned short maxs[3];
			unsigned int rowAxis;
			unsigned int colAxis;
			unsigned short* rowDataStart;
			unsigned int rawRowDataSize;
			unsigned char* rawRowData;
			unsigned int entryCount;
			GfxLightGridEntry* entries;
			unsigned int colorCount;
			GfxLightGridColors* colors;
		};

		struct GfxBrushModelWritable
		{
			game::native::Bounds bounds;
		};

		struct GfxBrushModel
		{
			GfxBrushModelWritable writable;
			game::native::Bounds bounds;
			float radius;
			unsigned short surfaceCount;
			unsigned short startSurfIndex;
			unsigned short surfaceCountNoDecal;
		};

		struct MaterialMemory
		{
			Material* material;
			int memory;
		};

		struct sunflare_t
		{
			bool hasValidData;
			Material* spriteMaterial;
			Material* flareMaterial;
			float spriteSize;
			float flareMinSize;
			float flareMinDot;
			float flareMaxSize;
			float flareMaxDot;
			float flareMaxAlpha;
			int flareFadeInTime;
			int flareFadeOutTime;
			float blindMinDot;
			float blindMaxDot;
			float blindMaxDarken;
			int blindFadeInTime;
			int blindFadeOutTime;
			float glareMinDot;
			float glareMaxDot;
			float glareMaxLighten;
			int glareFadeInTime;
			int glareFadeOutTime;
			float sunFxPosition[3];
		};

		struct BModelDrawInfo
		{
			unsigned short surfId;
		};

		struct GfxSceneDynBrush
		{
			BModelDrawInfo info;
			unsigned short dynEntId;
		};

		struct XModelDrawInfo
		{
			unsigned char hasGfxEntIndex;
			unsigned char lod;
			unsigned short surfId;
		};

		struct GfxSceneDynModel
		{
			XModelDrawInfo info;
			unsigned short dynEntId;
		};

		struct GfxShadowGeometry
		{
			unsigned short  surfaceCount;
			unsigned short  smodelCount;
			unsigned short* sortedSurfIndex;
			unsigned short* smodelIndex;
		};

		struct GfxLightRegionAxis
		{
			float dir[3];
			float midPoint;
			float halfSize;
		};

		struct GfxLightRegionHull
		{
			float kdopMidPoint[9];
			float kdopHalfSize[9];
			unsigned int axisCount;
			GfxLightRegionAxis* axis;
		};

		struct GfxLightRegion
		{
			unsigned int hullCount;
			GfxLightRegionHull* hulls;
		};

		struct GfxWorldDpvsDynamic
		{
			unsigned int dynEntClientWordCount[2];
			unsigned int dynEntClientCount[2];
			unsigned int* dynEntCellBits[2];
			char* dynEntVisData[2][3];
		};

		union PackedTexCoords
		{
			unsigned int packed;
		};

		struct GfxQuantizedNoColorVertex
		{
			short xyz[3];
			short binormalSign;
			PackedUnitVec normal;
			PackedUnitVec tangent;
			PackedTexCoords texCoord;
		};

		struct GfxQuantizedVertex
		{
			short xyz[3];
			short binormalSign;
			PackedUnitVec normal;
			PackedUnitVec tangent;
			PackedTexCoords texCoord;
			GfxColor color;
		};

		struct GfxPackedVertex
		{
			float xyz[3];
			float binormalSign;
			GfxColor color;
			PackedTexCoords texCoord;
			PackedUnitVec normal;
			PackedUnitVec tangent;
		};

		union GfxVertexUnion0
		{
			GfxQuantizedNoColorVertex* quantizedNoColorVerts0;
			GfxQuantizedVertex* quantizedVerts0;
			GfxPackedVertex* packedVerts0;
			void* verts0;
		};

		struct XSurfaceCollisionAabb
		{
			unsigned short mins[3];
			unsigned short maxs[3];
		};

		struct XSurfaceCollisionNode
		{
			XSurfaceCollisionAabb aabb;
			unsigned short childBeginIndex;
			unsigned short childCount;
		};

		struct XSurfaceCollisionLeaf
		{
			unsigned short triangleBeginIndex;
		};

		struct XSurfaceCollisionTree
		{
			float trans[3];
			float scale[3];
			unsigned int nodeCount;
			XSurfaceCollisionNode* nodes;
			unsigned int leafCount;
			XSurfaceCollisionLeaf* leafs;
		};

		struct XRigidVertList
		{
			unsigned short boneOffset;
			unsigned short vertCount;
			unsigned short triOffset;
			unsigned short triCount;
			XSurfaceCollisionTree* collisionTree;
		};

		struct XSurfaceVertexInfo
		{
			short vertCount[4];
			unsigned short* vertsBlend;
		};

		struct XSurface
		{
			unsigned char tileMode;
			unsigned char flags;
			unsigned short vertCount;
			unsigned short triCount;
			char zoneHandle;
			uint16_t baseTriIndex;
			uint16_t baseVertIndex;
			float quantizeScale;
			unsigned short* triIndices;
			XSurfaceVertexInfo vertInfo;
			GfxVertexUnion0 verts0;
			unsigned int vertListCount;
			XRigidVertList* vertList;
			int partBits[6];
		};


		enum PhysPresetScaling
		{
			PHYSPRESET_SCALING_LINEAR = 0x0,
			PHYSPRESET_SCALING_QUADRATIC = 0x1,
			PHYSPRESET_SCALING_COUNT = 0x2
		};

		struct PhysPreset
		{
			const char* name;
			int type;
			float mass;
			float bounce;
			float friction;
			float bulletForceScale;
			float explosiveForceScale;
			const char* sndAliasPrefix;
			float piecesSpreadFraction;
			float piecesUpwardVelocity;
			float minMomentum;
			float maxMomentum;
			float minPitch;
			float maxPitch;
			PhysPresetScaling volumeType;
			PhysPresetScaling pitchType;
			bool tempDefaultToCylinder;
			bool perSurfaceSndAlias;
		};


		struct XModelSurfs
		{
			const char* name;
			XSurface* surfs;
			unsigned short numsurfs;
			int partBits[6];
		};

		struct DObjAnimMat
		{
			float quat[4];
			float trans[3];
			float transWeight;
		};

		struct XModelLodInfo
		{
			float dist;
			unsigned short numsurfs;
			unsigned short surfIndex;
			XModelSurfs* modelSurfs;
			int partBits[6];
			XSurface* surfs;
			char lod;
			char smcBaseIndexPlusOne;
			char smcSubIndexMask;
			char smcBucket;
		};

		struct XModelCollTri_s
		{
			float plane[4];
			float svec[4];
			float tvec[4];
		};

		struct XModelCollSurf_s
		{
			XModelCollTri_s* collTris;
			int numCollTris;
			Bounds bounds;
			int boneIdx;
			int contents;
			int surfFlags;
		};

		struct XBoneInfo
		{
			Bounds bounds;
			float radiusSquared;
		};



		struct XModel
		{
			const char* name;
			unsigned char numBones;
			unsigned char numRootBones;
			unsigned char numsurfs;
			float scale;
			unsigned int noScalePartBits[6];
			unsigned short* boneNames;
			unsigned char* parentList;
			short* quats;
			float * trans;
			unsigned char* partClassification;
			DObjAnimMat* baseMat;
			Material** materialHandles;
			XModelLodInfo lodInfo[4];
			char maxLoadedLod;
			unsigned char numLods;
			unsigned char collLod;
			unsigned char flags;
			XModelCollSurf_s* collSurfs;
			int numCollSurfs;
			int contents;
			XBoneInfo* boneInfo;
			float radius;
			Bounds bounds;
			unsigned short* invHighMipRadius;
			int memUsage;
			PhysPreset* physPreset;
			PhysCollmap* physCollmap;
			float quantization;
		};

		struct GfxStaticModelDrawInst
		{
			game::native::GfxPackedPlacement placement;
			XModel* model;
			unsigned short cullDist;
			unsigned short lightingHandle;
			unsigned char reflectionProbeIndex;
			unsigned char primaryLightIndex;
			unsigned char flags;
			unsigned char firstMtlSkinIndex;
			game::native::GfxColor groundLighting;
			unsigned short cacheId[4];
		};

		struct GfxStaticModelInst
		{
			Bounds bounds;
			float lightingOrigin[3];
		};


		struct srfTriangles_t
		{
			unsigned int vertexLayerData;
			unsigned int firstVertex;
			unsigned short vertexCount;
			unsigned short triCount;
			unsigned int baseIndex;
		};

		struct GfxSurfaceLightingAndFlagsFields
		{
			unsigned char lightmapIndex;
			unsigned char reflectionProbeIndex;
			unsigned char primaryLightIndex;
			unsigned char flags;
		};

		union GfxSurfaceLightingAndFlags
		{
			GfxSurfaceLightingAndFlagsFields fields;
			unsigned int packed;
		};

		struct GfxSurface
		{
			srfTriangles_t tris;
			Material* material;
			GfxSurfaceLightingAndFlags laf;
		};

		struct GfxSurfaceBounds
		{
			Bounds bounds;
		};

		struct GfxWorldDpvsStatic
		{
			unsigned int smodelCount;
			unsigned int staticSurfaceCount;
			unsigned int staticSurfaceCountNoDecal;
			unsigned int litOpaqueSurfsBegin;
			unsigned int litOpaqueSurfsEnd;
			unsigned int litTransSurfsBegin;
			unsigned int litTransSurfsEnd;
			unsigned int shadowCasterSurfsBegin;
			unsigned int shadowCasterSurfsEnd;
			unsigned int emissiveSurfsBegin;
			unsigned int emissiveSurfsEnd;
			unsigned int smodelVisDataCount;
			unsigned int surfaceVisDataCount;
			char* smodelVisData[3];
			char* surfaceVisData[3];
			unsigned short* sortedSurfIndex;
			GfxStaticModelInst* smodelInsts;
			GfxSurface* surfaces;
			GfxSurfaceBounds* surfacesBounds;
			GfxStaticModelDrawInst* smodelDrawInsts;
			GfxDrawSurf* surfaceMaterials;
			unsigned int* surfaceCastsSunShadow;
			volatile int usageCount;
		};

		struct GfxWorld
		{
			const char* name;
			const char* baseName;
			int planeCount;
			int nodeCount;
			unsigned int surfaceCount;
			int skyCount;
			GfxSky* skies;
			unsigned int lastSunPrimaryLightIndex;
			unsigned int primaryLightCount;
			unsigned int sortKeyLitDecal;
			unsigned int sortKeyEffectDecal;
			unsigned int sortKeyEffectAuto;
			unsigned int sortKeyDistortion;
			GfxWorldDpvsPlanes dpvsPlanes;
			GfxCellTreeCount* aabbTreeCounts;
			GfxCellTree* aabbTrees;
			GfxCell* cells;
			GfxWorldDraw draw;
			GfxLightGrid lightGrid;
			int modelCount;
			GfxBrushModel* models;
			Bounds bounds;
			unsigned int checksum;
			int materialMemoryCount;
			MaterialMemory* materialMemory;
			sunflare_t sun;
			float outdoorLookupMatrix[4][4];
			GfxImage* outdoorImage;
			unsigned int* cellCasterBits;
			unsigned int* cellHasSunLitSurfsBits;
			GfxSceneDynModel* sceneDynModel;
			GfxSceneDynBrush* sceneDynBrush;
			unsigned int* primaryLightEntityShadowVis;
			unsigned int* primaryLightDynEntShadowVis[2];
			unsigned char* nonSunPrimaryLightForModelDynEnt;
			GfxShadowGeometry* shadowGeom;
			GfxLightRegion* lightRegion;
			GfxWorldDpvsStatic dpvs;
			GfxWorldDpvsDynamic dpvsDyn;
			unsigned int mapVtxChecksum;
			unsigned int heroOnlyLightCount;
			struct GfxHeroOnlyLight* heroOnlyLights;
			unsigned char fogTypesAllowed;
		};


		struct GfxHeroOnlyLight
		{
			unsigned char type;
			unsigned char unused[3];
			float color[3];
			float dir[3];
			float up[3];
			float origin[3];
			float radius;
			float cosHalfFovOuter;
			float cosHalfFovInner;
			int exponent;
		};


		struct SModelAabbNode
		{
			Bounds bounds;
			unsigned short firstChild;
			unsigned short childCount;
		};

		enum DynEntityType
		{
			DYNENT_TYPE_INVALID = 0x0,
			DYNENT_TYPE_CLUTTER = 0x1,
			DYNENT_TYPE_DESTRUCT = 0x2,
			DYNENT_TYPE_HINGE = 0x3,
			DYNENT_TYPE_COUNT = 0x4
		};

		struct GfxPlacement
		{
			float quat[4];
			float origin[3];
		};

		struct DynEntityHingeDef
		{
			float axisOrigin[3];
			float axisDir[3];
			bool isLimited;
			float angleMin;
			float angleMax;
			float momentOfInertia;
			float friction;
		};

		struct DynEntityDef
		{
			DynEntityType type;
			GfxPlacement pose;
			XModel* xModel;
			unsigned short brushModel;
			unsigned short physicsBrushModel;
			FxEffectDef* destroyFx;
			PhysPreset* physPreset;
			int health;
			DynEntityHingeDef* hinge;
			PhysMass mass;
			int contents;
		};

		struct DynEntityPose
		{
			GfxPlacement pose;
			float radius;
		};

		struct Hinge
		{
			float angle;
			float quat[4];
			float angularVel;
			float torqueAccum;
			bool active;
			float autoDisableTimeLeft;
			DynEntityHingeDef* def;
			PhysPreset* physPreset;
			float centerOfMassRelToAxisOriginAtAngleZero[3];
		};

		struct DynEntityClient
		{
			int physObjId;
			unsigned short flags;
			unsigned short lightingHandle;
			int health;
			Hinge* hinge;
		};

		struct DynEntityColl
		{
			unsigned short sector;
			unsigned short nextEntInSector;
			float linkMins[2];
			float linkMaxs[2];
		};


		struct ClipMaterial
		{
			const char* name;
			int surfaceFlags;
			int contents;
		};

		struct cLeafBrushNodeLeaf_t
		{
			unsigned short* brushes;
		};

		struct cLeafBrushNodeChildren_t
		{
			float dist;
			float range;
			unsigned short childOffset[2];
		};

		union cLeafBrushNodeData_t
		{
			cLeafBrushNodeLeaf_t leaf;
			cLeafBrushNodeChildren_t children;
		};

		struct cLeafBrushNode_s
		{
			unsigned char axis;
			short leafBrushCount;
			int contents;
			cLeafBrushNodeData_t data;
		};


		struct cbrush_t
		{
			unsigned short numsides;
			unsigned short glassPieceIndex;
			cbrushside_t* sides;
			unsigned char* baseAdjacentSide;
			unsigned short axialMaterialNum[2][3];
			unsigned char firstAdjacentSideOffsets[2][3];
			unsigned char edgeCount[2][3];
		};

		struct ClipInfo
		{
			int planeCount;
			cplane_s* planes;
			unsigned int numMaterials;
			ClipMaterial* materials;
			unsigned int numBrushSides;
			cbrushside_t* brushsides;
			unsigned int numBrushEdges;
			unsigned char* brushEdges;
			unsigned int leafbrushNodesCount;
			cLeafBrushNode_s* leafbrushNodes;
			unsigned int numLeafBrushes;
			unsigned short* leafbrushes;
			unsigned short numBrushes;
			cbrush_t* brushes;
			Bounds* brushBounds;
			int* brushContents;
		};

		struct cStaticModel_s
		{
			XModel* xmodel;
			float origin[3];
			float invScaledAxis[3][3];
			Bounds absBounds;
		};

		struct cNode_t
		{
			cplane_s* plane;
			short children[2];
		};


		struct cLeaf_t
		{
			unsigned short firstCollAabbIndex;
			unsigned short collAabbCount;
			int brushContents;
			int terrainContents;
			Bounds bounds;
			int leafBrushNode;
		};

		struct CollisionBorder
		{
			float distEq[3];
			float zBase;
			float zSlope;
			float start;
			float length;
		};

		struct CollisionPartition
		{
			unsigned char triCount;
			unsigned char borderCount;
			unsigned char firstVertSegment;
			int firstTri;
			CollisionBorder* borders;
		};

		struct CollisionAabbTree
		{
			float midPoint[3];
			unsigned short materialIndex;
			unsigned short childCount;
			float halfSize[3];
			unsigned int u;
		};

		struct cmodel_t
		{
			Bounds bounds;
			float radius;
			ClipInfo* info;
			cLeaf_t leaf;
		};

		struct clipMap_t
		{
			const char* name;
			int isInUse;
			ClipInfo info;
			ClipInfo* pInfo;
			unsigned int numStaticModels;
			cStaticModel_s* staticModelList;
			unsigned int numNodes;
			cNode_t* nodes;
			unsigned int numLeafs;
			cLeaf_t* leafs;
			unsigned int vertCount;
			vec3_t* verts;
			int triCount;
			unsigned short* triIndices;
			unsigned char* triEdgeIsWalkable;
			int borderCount;
			CollisionBorder* borders;
			int partitionCount;
			CollisionPartition* partitions;
			int aabbTreeCount;
			CollisionAabbTree* aabbTrees;
			unsigned int numSubModels;
			cmodel_t* cmodels;
			MapEnts* mapEnts;
			Stage* stages;
			unsigned char stageCount;
			MapTriggers stageTrigger;
			unsigned short smodelNodeCount;
			SModelAabbNode* smodelNodes;
			unsigned short dynEntCount[2];
			DynEntityDef* dynEntDefList[2];
			DynEntityPose* dynEntPoseList[2];
			DynEntityClient* dynEntClientList[2];
			DynEntityColl* dynEntCollList[2];
			unsigned int checksum;
			unsigned char padding[20];
		};

		struct FxGlassDef
		{
			float halfThickness;
			float texVecs[2][2];
			GfxColor color;
			Material* material;
			Material* materialShattered;
			PhysPreset* physPreset;
		};

		struct FxSpatialFrame
		{
			float quat[4];
			float origin[3];
		};

		union FxGlassPiecePlace
		{
#pragma warning(push)
#pragma warning(disable: 4201)
			struct
			{
				FxSpatialFrame frame;
				float radius;
			};
#pragma warning(pop)
			unsigned int nextFree;
		};

		struct FxGlassPieceState
		{
			float texCoordOrigin[2];
			unsigned int supportMask;
			unsigned short initIndex;
			unsigned short geoDataStart;
			unsigned char defIndex;
			unsigned char pad[5];
			unsigned char vertCount;
			unsigned char holeDataCount;
			unsigned char crackDataCount;
			unsigned char fanDataCount;
			unsigned short flags;
			float areaX2;
		};

		struct FxGlassPieceDynamics
		{
			int fallTime;
			int physObjId;
			int physJointId;
			float vel[3];
			float avel[3];
		};


		struct FxGlassVertex
		{
			short x;
			short y;
		};

		struct FxGlassHoleHeader
		{
			unsigned short uniqueVertCount;
			unsigned char touchVert;
			unsigned char pad[1];
		};

		struct FxGlassCrackHeader
		{
			unsigned short uniqueVertCount;
			unsigned char beginVertIndex;
			unsigned char endVertIndex;
		};

		union FxGlassGeometryData
		{
			FxGlassVertex vert;
			FxGlassHoleHeader hole;
			FxGlassCrackHeader crack;
			unsigned char asBytes[4];
			short anonymous[2];
		};

		struct FxGlassInitPieceState
		{
			FxSpatialFrame frame;
			float radius;
			float texCoordOrigin[2];
			unsigned int supportMask;
			float areaX2;
			unsigned char defIndex;
			unsigned char vertCount;
			unsigned char fanDataCount;
			unsigned char pad[1];
		};

		struct FxGlassSystem
		{
			int time;
			int prevTime;
			unsigned int defCount;
			unsigned int pieceLimit;
			unsigned int pieceWordCount;
			unsigned int initPieceCount;
			unsigned int cellCount;
			unsigned int activePieceCount;
			unsigned int firstFreePiece;
			unsigned int geoDataLimit;
			unsigned int geoDataCount;
			unsigned int initGeoDataCount;
			FxGlassDef* defs;
			FxGlassPiecePlace* piecePlaces;
			FxGlassPieceState* pieceStates;
			FxGlassPieceDynamics* pieceDynamics;
			FxGlassGeometryData* geoData;
			unsigned int* isInUse;
			unsigned int* cellBits;
			unsigned char* visData;
			float(*linkOrg)[3];
			float* halfThickness;
			unsigned short* lightingHandles;
			FxGlassInitPieceState* initPieceStates;
			FxGlassGeometryData* initGeoData;
			bool needToCompactData;
			unsigned char initCount;
			float effectChanceAccum;
			int lastPieceDeletionTime;
		};

		struct FxWorld
		{
			const char* name;
			FxGlassSystem glassSys;
		};

		struct FxIntRange
		{
			int base;
			int amplitude;
		};

		struct FxSpawnDefLooping
		{
			int intervalMsec;
			int count;
		};

		struct FxSpawnDefOneShot
		{
			FxIntRange count;
		};

		union FxSpawnDef
		{
			FxSpawnDefLooping looping;
			FxSpawnDefOneShot oneShot;
		};

		struct FxFloatRange
		{
			float base;
			float amplitude;
		};

		struct FxElemAtlas
		{
			unsigned char behavior;
			unsigned char index;
			unsigned char fps;
			unsigned char loopCount;
			unsigned char colIndexBits;
			unsigned char rowIndexBits;
			short entryCount;
		};

		struct FxElemVec3Range
		{
			float base[3];
			float amplitude[3];
		};

		struct FxElemVelStateInFrame
		{
			FxElemVec3Range velocity;
			FxElemVec3Range totalDelta;
		};


		struct FxElemVelStateSample
		{
			FxElemVelStateInFrame local;
			FxElemVelStateInFrame world;
		};

		struct FxElemVisualState
		{
			unsigned char color[4];
			float rotationDelta;
			float rotationTotal;
			float size[2];
			float scale;
		};

		struct FxElemVisStateSample
		{
			FxElemVisualState base;
			FxElemVisualState amplitude;
		};

		struct FxElemMarkVisuals
		{
			Material* materials[2];
		};

		union FxEffectDefRef
		{
			FxEffectDef* handle;
			const char* name;
		};

		union FxElemVisuals
		{
			const void* anonymous;
			Material* material;
			XModel* model;
			FxEffectDefRef effectDef;
			const char* soundName;
			GfxLightDef* lightDef;
		};

		union FxElemDefVisuals
		{
			FxElemMarkVisuals* markArray;
			FxElemVisuals* array;
			FxElemVisuals instance;
		};

		struct FxTrailVertex
		{
			float pos[2];
			float normal[2];
			float texCoord;
		};

		struct FxTrailDef
		{
			int scrollTimeMsec;
			int repeatDist;
			float invSplitDist;
			float invSplitArcDist;
			float invSplitTime;
			int vertCount;
			FxTrailVertex* verts;
			int indCount;
			unsigned short* inds;
		};

		struct FxSparkFountainDef
		{
			float gravity;
			float bounceFrac;
			float bounceRand;
			float sparkSpacing;
			float sparkLength;
			int sparkCount;
			float loopTime;
			float velMin;
			float velMax;
			float velConeFrac;
			float restSpeed;
			float boostTime;
			float boostFactor;
		};

		struct FxSpotLightDef
		{
			float fovInnerFraction;
			float startRadius;
			float endRadius;
			float brightness;
			float maxLength;
			int exponent;
		};

		union FxElemExtendedDefPtr
		{
			FxTrailDef* trailDef;
			FxSparkFountainDef* sparkFountainDef;
			FxSpotLightDef* spotLightDef;
			char* unknownDef;
		};

		enum FxElemType : char
		{
			FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
			FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
			FX_ELEM_TYPE_TAIL = 0x2,
			FX_ELEM_TYPE_TRAIL = 0x3,
			FX_ELEM_TYPE_CLOUD = 0x4,
			FX_ELEM_TYPE_SPARKCLOUD = 0x5,
			FX_ELEM_TYPE_SPARKFOUNTAIN = 0x6,
			FX_ELEM_TYPE_MODEL = 0x7,
			FX_ELEM_TYPE_OMNI_LIGHT = 0x8,
			FX_ELEM_TYPE_SPOT_LIGHT = 0x9,
			FX_ELEM_TYPE_SOUND = 0xA,
			FX_ELEM_TYPE_DECAL = 0xB,
			FX_ELEM_TYPE_RUNNER = 0xC,
			FX_ELEM_TYPE_COUNT = 0xD,
			FX_ELEM_TYPE_LAST_SPRITE = 0x3,
			FX_ELEM_TYPE_LAST_DRAWN = 0x9,
		};

		struct FxElemDef
		{
			int flags;
			FxSpawnDef spawn;
			FxFloatRange spawnRange;
			FxFloatRange fadeInRange;
			FxFloatRange fadeOutRange;
			float spawnFrustumCullRadius;
			FxIntRange spawnDelayMsec;
			FxIntRange lifeSpanMsec;
			FxFloatRange spawnOrigin[3];
			FxFloatRange spawnOffsetRadius;
			FxFloatRange spawnOffsetHeight;
			FxFloatRange spawnAngles[3];
			FxFloatRange angularVelocity[3];
			FxFloatRange initialRotation;
			FxFloatRange gravity;
			FxFloatRange reflectionFactor;
			FxElemAtlas atlas;
			unsigned char elemType;
			unsigned char visualCount;
			unsigned char velIntervalCount;
			unsigned char visStateIntervalCount;
			FxElemVelStateSample* velSamples;
			FxElemVisStateSample* visSamples;
			FxElemDefVisuals visuals;
			Bounds collBounds;
			FxEffectDefRef effectOnImpact;
			FxEffectDefRef effectOnDeath;
			FxEffectDefRef effectEmitted;
			FxFloatRange emitDist;
			FxFloatRange emitDistVariance;
			FxElemExtendedDefPtr extended;
			unsigned char sortOrder;
			unsigned char lightingFrac;
			unsigned char useItemClip;
			unsigned char fadeInfo;
			int randomSeed;
		};

		struct FxEffectDef
		{
			const char* name;
			int flags;
			int totalSize;
			int msecLoopingLife;
			int elemDefCountLooping;
			int elemDefCountOneShot;
			int elemDefCountEmission;
			float occlusionQueryDepthBias;
			int occlusionQueryFadeIn;
			int occlusionQueryFadeOut;
			FxFloatRange occlusionQueryScaleRange;
			FxElemDef* elemDefs;
		};

		enum XAnimPartType
		{
			PART_TYPE_NO_QUAT = 0x0,
			PART_TYPE_HALF_QUAT = 0x1,
			PART_TYPE_FULL_QUAT = 0x2,
			PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
			PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
			PART_TYPE_SMALL_TRANS = 0x5,
			PART_TYPE_TRANS = 0x6,
			PART_TYPE_TRANS_NO_SIZE = 0x7,
			PART_TYPE_NO_TRANS = 0x8,
			PART_TYPE_ALL = 0x9,
			PART_TYPE_COUNT = 0xA,
		};

		union XAnimIndices
		{
			char* _1;
			unsigned short* _2;
			void* data;
		};

		struct XAnimNotifyInfo
		{
			unsigned short name;
			float time;
		};

		union XAnimDynamicFrames
		{
			char(*_1)[3];
			unsigned __int16(*_2)[3];
		};

		union XAnimDynamicIndicesTrans
		{
			char _1[1];
			unsigned short _2[1];
		};

		struct XAnimPartTransFrames
		{
			float mins[3];
			float size[3];
			XAnimDynamicFrames frames;
			XAnimDynamicIndicesTrans indices;
		};

		union XAnimPartTransData
		{
			XAnimPartTransFrames frames;
			vec3_t frame0;
		};

		struct XAnimPartTrans
		{
			unsigned short size;
			char smallTrans;
			XAnimPartTransData u;
		};

		union XAnimDynamicIndicesQuat2
		{
			char _1[1];
			unsigned short _2[1];
		};

		struct XAnimDeltaPartQuatDataFrames2
		{
			__int16(*frames)[2];
			XAnimDynamicIndicesQuat2 indices;
		};

		union XAnimDeltaPartQuatData2
		{
			XAnimDeltaPartQuatDataFrames2 frames;
			int16_t frame0[2];
		};

		struct XAnimDeltaPartQuat2
		{
			unsigned short size;
			XAnimDeltaPartQuatData2 u;
		};

		union XAnimDynamicIndicesQuat
		{
			char _1[1];
			unsigned short _2[1];
		};

		struct XAnimDeltaPartQuatDataFrames
		{
			__int16(*frames)[4];
			XAnimDynamicIndicesQuat indices;
		};

		union XAnimDeltaPartQuatData
		{
			XAnimDeltaPartQuatDataFrames frames;
			short frame0[4];
		};

		struct XAnimDeltaPartQuat
		{
			unsigned short size;
			XAnimDeltaPartQuatData u;
		};

		struct XAnimDeltaPart
		{
			XAnimPartTrans* trans;
			XAnimDeltaPartQuat2* quat2;
			XAnimDeltaPartQuat* quat;
		};

		struct XAnimParts
		{
			const char* name;
			unsigned short dataByteCount;
			unsigned short dataShortCount;
			unsigned short dataIntCount;
			unsigned short randomDataByteCount;
			unsigned short randomDataIntCount;
			unsigned short numframes;
			char flags;
			unsigned char boneCount[10];
			char notifyCount;
			char assetType;
			bool isDefault;
			unsigned int randomDataShortCount;
			unsigned int indexCount;
			float framerate;
			float frequency;
			unsigned short* names;
			char* dataByte;
			unsigned short* dataShort;
			int* dataInt;
			unsigned short* randomDataShort;
			char* randomDataByte;
			int* randomDataInt;
			XAnimIndices indices;
			XAnimNotifyInfo* notify;
			XAnimDeltaPart* deltaPart;
		};

		namespace mp
		{
			enum ConfigString
			{
				CS_EFFECT_NAMES = 0x9DC,
			};

			struct client_t
			{
				clientHeader_t header;
				const char* dropReason; // 0x66C
				char userinfo[1024]; // 0x670
				unsigned char __pad0[0x209B8];
				gentity_s* gentity; // 0x21428
				char name[16]; // 0x2142C
				unsigned char __pad1[0x20876];
				unsigned __int16 scriptId; // 0x41CB2
				int bIsTestClient; // 0x41CB4
				int serverId; // 0x41CB8
				unsigned char __pad2[0x369DC];
			};

			static_assert(sizeof(client_t) == 0x78698);
		}

		namespace sp
		{
			enum ConfigString
			{
				CS_EFFECT_NAMES = 0xB8F,
			};

			struct usercmd_s
			{
				int serverTime;
				int buttons;
				int angles[3];
				Weapon weapon;
				Weapon offHand;
				char forwardmove;
				char rightmove;
				unsigned char upmove;
				unsigned char downmove;
				char pitchmove;
				char yawmove;
				float gunPitch;
				float gunYaw;
				float gunXOfs;
				float gunYOfs;
				float gunZOfs;
				short meleeChargeYaw;
				unsigned char meleeChargeDist;
				char selectedLoc[2];
				unsigned char selectedLocAngle;
				char remoteControlAngles[2];
			};

			static_assert(sizeof(usercmd_s) == 0x40);

			struct clientPersistent_t
			{
				usercmd_s cmd;
				usercmd_s oldcmd;
				int maxHealth;
				float moveSpeedScaleMultiplier;
				int motionTrackerEnabled;
				char playerName[0x20];
			};

			static_assert(sizeof(clientPersistent_t) == 0xAC);

			struct playerState_s
			{
				int commandTime;
				int pm_type;
				int pm_time;
				int pm_flags;
				int otherFlags;
				int linkFlags;
				int bobCycle;
				float origin[3];
				float velocity[3];
				unsigned char __pad0[0xAD24];
			};

			static_assert(sizeof(playerState_s) == 0xAD58);

			struct gclient_s
			{
				playerState_s ps;
				clientPersistent_t pers;
				int flags;
				unsigned char __pad1[0x2BC];
			};

			static_assert(sizeof(gclient_s) == 0xB0C4);

			struct entityState_s
			{
				int eType;
				unsigned char __pad0[0x80];
				int number;
				unsigned char __pad1[0x28];
			};

			static_assert(sizeof(entityState_s) == 0xB0);

			struct entityShared_t
			{
				unsigned char isLinked;
				unsigned char modelType;
				unsigned char svFlags;
				unsigned char eventType;
				unsigned char isInUse;
				Bounds box;
				int contents;
				Bounds absBox;
				float currentOrigin[3];
				float currentAngles[3];
				EntHandle ownerNum;
				int eventTime;
			};

			static_assert(sizeof(entityShared_t) == 0x5C);

			struct gentity_s
			{
				entityState_s s;
				entityShared_t r;
				gclient_s* client; // 0x10C
				unsigned char __pad0[0x2C];
				int flags;
				int clipmask;
				int processedFrame;
				EntHandle parent;
				int nextthink;
				int health;
				int maxHealth;
				int damage;
				unsigned char __pad1[0x114];
			};

			static_assert(sizeof(gentity_s) == 0x270);
		}
	}
}
