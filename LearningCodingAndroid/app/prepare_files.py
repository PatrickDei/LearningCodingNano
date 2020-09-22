# -*- coding: utf-8 -*-
import os
import sys
import shutil
import hashlib
import json
import build_config
import re
import argparse

ASSET_DIRECTORIES_TO_PACK = [
    "items-hd",
    "places-hd",
    "Sounds",
    "menus-hd",
    "thumbs-hd"
]
# ASSET_DIRECTORIES_TO_PACK = []

ASSET_SRC_ROOT = ""
ASSET_DST_ROOT = ""
ASSET_ZIP_ROOT = ""

FILE_HASHES = {} 

nCOPIED = 0
nZIPPED = 0
nCPYMOD = 0
nZIPMOD = 0

EXPANSION_ZIP = "zip"
EXPANSION_OBB = "obb"

def getFileHash(filepath):
    sha1 = hashlib.sha1()
    fullpath = ASSET_SRC_ROOT + "/" + filepath
    with open(fullpath,'rb') as afile:
        buf = afile.read()
        buf+=build_config.getVariable("HASH_SALT")["value"]
        sha1.update(buf)
    
    m = re.search(r".+\.(.+)", filepath)
    if m.group(1) is not None:
        if m.group(1) in ("json", "plist", "txt", "xml"):
            global FILE_HASHES
            FILE_HASHES[os.path.split(filepath)[-1]] = sha1.hexdigest()
            # print "HASH of '" + filepath + "': " + sha1.hexdigest()

def exportHashDictionary(outputFilepath):
    outputString = json.dumps(FILE_HASHES)
    with open(ASSET_DST_ROOT + "/" + outputFilepath, 'w') as outputFile:
        # print "HASH DICTIONARY:" + ASSET_DST_ROOT + "/" + outputFilepath
        # print outputString
        outputFile.write(outputString)
    sys.stdout.flush()

def copy2assets(path,asset):
    global nCOPIED
    global nCPYMOD
    # print [path,asset]
    srcPath = ASSET_SRC_ROOT + "/" + path 
    dstPath = ASSET_DST_ROOT + "/" + path 
    fullSrcPath = os.path.normpath(srcPath + "/" + asset)
    fullDstPath = os.path.normpath(dstPath + "/" + asset)

    if not os.path.exists(dstPath):
        os.makedirs(dstPath)

    if os.path.isfile(fullSrcPath):
        if path == "Profiles":
            getFileHash(path+"/"+asset)
        if os.path.exists(fullDstPath):
            if os.stat(fullSrcPath).st_mtime > os.stat(fullDstPath).st_mtime:
                shutil.copy2(fullSrcPath,fullDstPath)
                nCPYMOD+=1
                print ("* COPY * " + path + "/" + asset + " [ modified ]").ljust(100, ' ') + "\r",
        else:
            shutil.copy2(fullSrcPath,fullDstPath)
            nCOPIED+=1
            print ("* COPY * " + path + "/" + asset).ljust(100, ' ') + "\r", 
    else:
        for a in os.listdir(fullSrcPath):
            copy2assets(path+((path!="")*"/")+asset,a)
    sys.stdout.flush()

def copy2expansion(path,asset,expansionType=EXPANSION_OBB):
    global nZIPMOD
    global nZIPPED

    # print "in c2e -> EXPTYPE",expansionType

    assetsPath = ""
    if expansionType == EXPANSION_ZIP:
        assetsPath = "assets/"
    else:
        try:
            print "deleting", ASSET_ZIP_ROOT+"/assets"
            shutil.rmtree(ASSET_ZIP_ROOT+"/assets")
        except OSError:
            print "[i] Previous file '%s' does not exist. Proceeding..." % (asset,)

    # print [path,asset]
    srcPath = ASSET_SRC_ROOT + "/" + path 
    dstPath = ASSET_ZIP_ROOT + "/" + assetsPath + path 
    fullSrcPath = os.path.normpath(srcPath + "/" + asset)
    fullDstPath = os.path.normpath(dstPath + "/" + asset)

    # print fullDstPath

    if not os.path.exists(dstPath):
        os.makedirs(dstPath)

    if os.path.isfile(fullSrcPath):
        if path == "Profiles":
            getFileHash(path+"/"+asset)
        if os.path.exists(fullDstPath):
            if os.stat(fullSrcPath).st_mtime > os.stat(fullDstPath).st_mtime:
                shutil.copy2(fullSrcPath,fullDstPath)
                nZIPMOD+=1
                print ("* ZIP * " + path + "/" + asset + " [ modified ]").ljust(100, ' ') + "\r",
        else:
            shutil.copy2(fullSrcPath,fullDstPath)
            nZIPPED+=1
            print ("* ZIP * " + path + "/" + asset).ljust(100, ' ') + "\r",
    else:
        for a in os.listdir(fullSrcPath):
            copy2expansion(path+((path!="")*"/")+asset,a,expansionType)
    sys.stdout.flush()

def packageExpansion(expansionType=EXPANSION_OBB):
    print 
    print "#== PACKAGING EXPANSION FILES ==#"

    print "Expansion type = " + expansionType
    
    obb_file = "main." + build_config.CONFIG["APP_VERSION"]["value"] + "." + build_config.CONFIG["PACKAGE_NAME"]["value"]  + ".obb"
    obb_file_path = os.path.normpath(ASSET_ZIP_ROOT + "/../" + obb_file)

    print "Deleting previous package..."
    print obb_file_path

    try:
        os.remove(obb_file_path)
    except OSError:
        print "[i] Previous file does not exist. Proceeding..."

    print "Begin packaging..."

    if expansionType == EXPANSION_ZIP:
        # shutil.make_archive(obb_file_path, 'zip', ASSET_ZIP_ROOT)
        # zip2obbcommand = 'mv '+obb_file_path+".zip" + " " + obb_file_path
        # os.system(zip2obbcommand)
        zipCommand = "zip"
        args = [
            "-rn " + ":".join([".ogg", ".mp3", ".wav"]),
            os.path.normpath("../"+os.path.split(obb_file_path)[-1]),
            "assets/"
        ]

        os.chdir(ASSET_ZIP_ROOT)
        print "Working directory = ",
        sys.stdout.flush()
        os.system("pwd")
        sys.stdout.flush()
        zipCommand += " " + " ".join(args) + " > " + "../zip_output.log"   
        print "ZIP COMMAND:\n\t" + zipCommand
        os.system(zipCommand)
    else:
        SDK_ROOT = os.environ.get("SDK_ROOT")
        jobbCommand = os.path.normpath(SDK_ROOT + "/tools/jobb") 
        args = [
            "-d '" + ASSET_ZIP_ROOT + "'",
            "-o '" + obb_file_path + "'",
            "-pn " + build_config.CONFIG["PACKAGE_NAME"]["value"],
            "-pv " + build_config.CONFIG["APP_VERSION"]["value"],
            # "-v",     
        ]
        if len(build_config.CONFIG["OBB_KEY"]["value"]) > 0:
            args.append("-k " + build_config.CONFIG["OBB_KEY"]["value"])
        jobbCommand+=" " + " ".join(args) + " > " + "jobb_output.log"
        print jobbCommand
        os.system(jobbCommand)

    print "Done ✔︎"
    sys.stdout.flush()

    build_config.setVariable("OBB_SIZE", "long", os.stat(obb_file_path).st_size)

def prepareFiles(pwd=None, debug=True, packExpansion=False, expType="obb"):
    print 
    print "#== PREPARING TO SYNC ASSETS ==#"

    if pwd is None:
        print "[!] Cannot prepare assets, working directory not set properly."
        return

    excludeList = []
    if packExpansion:
        excludeList = ASSET_DIRECTORIES_TO_PACK[:]

    build_config.setVariable("DEBUG","boolean",debug)

    build_config.setAppVersionCode(pwd)
    build_config.setAppPackageName(pwd)

    # reset OBB size in case we don't package
    if not packExpansion:
        build_config.setVariable("OBB_SIZE", "long", 0) 

    # print build_config.CONFIG

    global ASSET_SRC_ROOT
    global ASSET_DST_ROOT
    global ASSET_ZIP_ROOT
    ASSET_SRC_ROOT = os.path.normpath(pwd + "/../StoryGame/assets")
    ASSET_DST_ROOT = os.path.normpath(pwd + "/app/assets")
    ASSET_ZIP_ROOT = os.path.normpath(pwd + "/expansionResources")

    print "Source directory = '" + ASSET_SRC_ROOT + "'"
    print "Destination directory = '" + ASSET_DST_ROOT + "'"
    print "Temporary zip directory = '" + ASSET_ZIP_ROOT + "'"

    asset_root_files = os.listdir(ASSET_SRC_ROOT)

    if packExpansion:
        for asset in ASSET_DIRECTORIES_TO_PACK:
            if asset in asset_root_files:
                print "[i] Debug build assets found in '" + ASSET_DST_ROOT + "'"
                print "    Attempting to clean..."
                try:
                    shutil.rmtree(ASSET_DST_ROOT)
                    print "Directory deleted ✔︎"
                except OSError as e:
                    print "[!] Cannot delete directory '" + ASSET_DST_ROOT + "'"
                    print e
                try:
                    os.makedirs(ASSET_DST_ROOT)
                    print "Directory created ✔︎"
                except OSError as e:
                    print "[!] Cannot create directory '" + ASSET_DST_ROOT + "'"
                    print e

                break # it's enough to clean once


    for asset in asset_root_files:
        # print [asset]
        if asset not in excludeList:
            copy2assets("", asset)
        else:
            if expType==EXPANSION_ZIP:
                try:
                    # print "deleting", ASSET_ZIP_ROOT+"/"+asset
                    shutil.rmtree(ASSET_ZIP_ROOT + "/" + asset)
                except OSError:
                    pass
                    # print "[i] Previous file '%s' does not exist. Proceeding..." % (asset,)
            copy2expansion("", asset, expansionType=expType)
            # print "* COPY *", ASSET_SRC_ROOT + "/" + asset, " => ", "expansion_file.obb"

    print " "*100 + "\r\n"
    print "Copied %d files (%d modified)" % (nCOPIED+nCPYMOD, nCPYMOD,)
    print "Zipped %d files (%d modified)" % (nZIPPED+nZIPMOD, nZIPMOD,)

    exportHashDictionary("hashes.json")

    sys.stdout.flush()


# def generateStardroidConfig():
#   build_config.generateStardroidConfig()


def init(pwd=None, debug=True):
    print 
    print "#== PREPARING TO INIT ==#"

    if pwd is None:
        print "[PWD IS NONE"
        return

    build_config.setVariable("DEBUG","boolean",debug)

    build_config.setAppVersionCode(pwd)
    build_config.setAppPackageName(pwd)

    # print build_config.CONFIG

    global ASSET_SRC_ROOT
    global ASSET_DST_ROOT
    global ASSET_ZIP_ROOT
    ASSET_SRC_ROOT = os.path.normpath(pwd + "/../StoryGame/assets")
    ASSET_DST_ROOT = os.path.normpath(pwd + "/app/assets")
    ASSET_ZIP_ROOT = os.path.normpath(pwd + "/expansionResources")

    print "Source directory = '" + ASSET_SRC_ROOT + "'"
    print "Destination directory = '" + ASSET_DST_ROOT + "'"
    print "Temporary zip directory = '" + ASSET_ZIP_ROOT + "'"

    sys.stdout.flush()


# def generateStardroidConfig():
#   build_config.generateStardroidConfig()


def generateStardroidConfig():
    # global CONFIG
    conf = build_config.CONFIG
    app_root_path = os.path.normpath(ASSET_SRC_ROOT+"/../../StoryGameAndroid/")

    content = ""
    content += "package " + conf["PACKAGE_NAME"]["value"] + ";" + "\n"
    content += "" + "\n"
    content += "public final class StardroidConfig" + "\n"
    content += "{" + "\n"

    for key in conf:
        if "skip" in conf[key]:
            continue
        jtype = conf[key]["type"]
        value = conf[key]["value"]
        jvalue = ""
        if jtype == "boolean":
            jvalue = "true" if value else "false"
        elif jtype in ("int", "long", "double", "float"):
            jvalue = str(value);
        elif jtype == "String":
            jvalue = "\"" + value + "\""
        line = "\tpublic final static %s %s = %s;" % (jtype, key, jvalue,)
        # print line
        content += line + "\n"
 
    content += "}" + "\n"

    # print content

    path = os.path.normpath( \
        app_root_path + "/app/src/" + \
        "/".join(conf["PACKAGE_NAME"]["value"].split(".")) + \
        "/StardroidConfig.java" \
        )
    # print path
    with open(path,'w') as afile:
        afile.write(content)


def generateApplicationMK(abi):
    
    app_root_path = os.path.normpath(ASSET_SRC_ROOT+"/..")
    build_config.setAppPlatform(app_root_path)
    build_config.setAppABI(app_root_path, abi)
    # build_config.generateApplicationMK(app_root_path)

    # global CONFIG

    debug = "1" if build_config.getVariable("DEBUG")["value"] else "0"
    platform = build_config.getVariable("APP_MIN_PLATFORM")["value"]
    abi = build_config.getVariable("APP_ABI")["value"]

    content = ""
    content += "APP_STL := c++_static" + "\n"
    content += "APP_CPPFLAGS := -frtti -g -DCOCOS2D_DEBUG=" + debug + (" -DDEBUG=1\n" if debug == "1" else "\n" )
    # content += "APP_CPPFLAGS := -frtti -g -DCOCOS2D_DEBUG=" + debug + "\n"
    content += "APP_CPPFLAGS += -std=c++11" + "\n"
    content += "APP_PLATFORM := android-" + platform + "\n"
    content += "APP_ABI := " + abi + "\n"

    # print content

    path = os.path.normpath( \
        app_root_path + "/../StoryGameAndroid/app/" + \
        "/Application.mk" \
        )
    with open(path,'r') as afile:
        if afile.read() == content:
            return
    with open(path,'w') as afile:
        afile.write(content)


def generateAndroidMK(app_root_path):
    pass


def generateJniAndroidMK(app_root_path):
    pass


print
print "#== PREPARING TO BUILD PROJECT ==#"

b_PRODUCTION 	= "PRODUCTION"
b_RELEASE 		= "RELEASE"
b_DEBUG 		= "DEBUG"
buildType = (b_PRODUCTION, b_RELEASE, b_DEBUG)

abiType = ("armeabi", "armeabi-v7a", "x86")

global CONFIG
debug = 2 if build_config.getVariable("DEBUG")["value"] else 0
abi = abiType[1] # default is armeabi_v7a
packageOBB = False
packageZIP = False

parser = argparse.ArgumentParser()
parser.add_argument('-d', type=int, choices=tuple(range(len(buildType))), default=2)
parser.add_argument('--abi', type=str, choices=abiType, default=abiType[1])
parser.add_argument('--obb', action='store_true', default=False)
parser.add_argument('--zip', action='store_true', default=False)
parser.add_argument('--x86', action='store_true', default=False)
parser.add_argument('args', nargs=argparse.REMAINDER);
arguments = vars(parser.parse_args())
print arguments

for arg in arguments:
    if arg == "d":
    	debug = arguments[arg]
    if arg == "abi":
    	abi = arguments[arg]
    if arg == "obb":
    	packageOBB = arguments[arg]
    	packageZIP = not packageOBB and packageZIP
    if arg == "zip":
    	packageZIP = arguments[arg]
    	packageOBB = packageOBB and not packageZIP
    if arg == "x86":
    	if abi != arg and arguments[arg]:
    		abi += " " + arg

print
print "BUILD SETTINGS =", str(debug), " #", buildType[debug]
print "APP_ABI =", abi
print "+ Package OBB"*packageOBB
print "+ Package ZIP"*packageZIP

DIR = os.path.dirname(os.path.abspath(__file__))

APP_ROOT = DIR + "/.."

expansionType = ("obb" * packageOBB) + ("zip" * packageZIP)
prepareFiles(APP_ROOT, debug=(debug == buildType.index(b_DEBUG)), packExpansion=(packageOBB or packageZIP), expType=expansionType)
if packageOBB or packageZIP:
	packageExpansion(expansionType=expansionType)
generateApplicationMK(abi)
generateStardroidConfig()

sys.stdout.flush()
