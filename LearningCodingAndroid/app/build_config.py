import re
import os

CONFIG = {
	'BASE64_PUBLIC_KEY':	{ "type":"String",		"value":"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAkYTHZw8hTYQ/iS9ceBwVbks+Lz/RwJla7Zyv44qwui8/NhZTfdMN6Zz9xOjRNjnDVHepcgsx4T0GC6+wOSufMwGg8L60RAmr+62KaN5ZVGGgWKm6MoH7xwSdMMM6Cwl0BKm5dcq0MgvJ/idrBE5p+Px7vdaG/HAu03XvBKo8up6rSoLAG4szR+XjbV3RHR6iJNMLsgX1rl8liTq/ieaEA00qmcfczoW96DWayg9xSmZ5XovfiiyFRKEZi4wrSZNXyPFLY9nRtmJdQDuddz/vUq0lbVqawo2Kh06tQKDAxLijfs1dWUzYjt2LDiPwDYkF+KIF5TG9l1QL4xK/pGxgwwIDAQAB"},
	'DEBUG': 				{ "type":"boolean", 	"value":True },
	'OBB_KEY':  			{ "type":"String", 		"value":'' },
	'OBB_SIZE': 			{ "type":"long", 		"value":80222929 },
	'PACKAGE_NAME': 		{ "type":"String", 		"value":'a.a.a' },
	'APP_VERSION':  		{ "type":"String", 		"value":'0' },
	'APP_TARGET_PLATFORM': 	{ "type":"String", 		"value":'0' },
	'APP_MIN_PLATFORM': 	{ "type":"String", 		"value":'0' },
	'APP_ABI':  			{ "type":"String", 		"value":'0' },
	'HASH_SALT':  			{ "type":"String", 		"value":'3nano2Pass1save', "skip":True},
}

def setVariable(name, jtype, value):
	global CONFIG
	CONFIG[name] = { "type":jtype, "value":value}

def getVariable(name):
	global CONFIG
	return CONFIG[name]

def setAppVersionCode(app_root_path):
	path = os.path.normpath(app_root_path + "/../StoryGameAndroid/app/AndroidManifest.xml")
	appVersion = ""
	with open(path,"r") as manifest:
		content = manifest.read()
		m = re.search(r'android:versionCode="(.*)"', content)
		appVersion = m.group(1)

	global CONFIG
	CONFIG["APP_VERSION"]["value"] = appVersion


def setAppPackageName(app_root_path):
	path = os.path.normpath(app_root_path + "/../StoryGameAndroid/app/AndroidManifest.xml")
	packageName = ""
	with open(path,"r") as manifest:
		content = manifest.read()
		m = re.search(r'package="(.*)"', content)
		packageName = m.group(1)

	global CONFIG
	CONFIG["PACKAGE_NAME"]["value"] = packageName


def setAppPlatform(app_root_path):
	path = os.path.normpath(app_root_path + "/../StoryGameAndroid/app/AndroidManifest.xml")
	minPlatform = ""
	targetPlatform = ""
	with open(path,"r") as manifest:
		content = manifest.read()
		m = re.search(r'android:targetSdkVersion="(\d+)"', content)
		targetPlatform = m.group(1)
		m = re.search(r'android:minSdkVersion="(\d+)"', content)
		minPlatform = m.group(1)

	global CONFIG
	CONFIG["APP_MIN_PLATFORM"]["value"] = minPlatform
	CONFIG["APP_TARGET_PLATFORM"]["value"] = targetPlatform


def setAppABI(app_root_path, abi):
	global CONFIG
	CONFIG["APP_ABI"]["value"] = abi





