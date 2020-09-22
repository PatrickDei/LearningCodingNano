#!/bin/sh

git rm --cached *.pydevproject
git rm --cached *.metadata/*
git rm --cached *.metadata*
git rm --cached */bin/**
git rm --cached */obj/**
git rm --cached */tmp/**
git rm --cached *.tmp
git rm --cached *.bak
git rm --cached *.swp
git rm --cached *~.nib
git rm --cached *local.properties
git rm --cached *.settings/*
git rm --cached *.loadpath
git rm --cached *org.eclipse.wst.common.component
git rm --cached *org.eclipse.wst.common.project.facet.core.xml
git rm --cached *.orig
git rm --cached *.class
git rm --cached *.pyc
git rm --cached *assets/*
git rm --cached *.obb
git rm --cached *zip_output.log
git rm --cached ../StoryGameIOS/StoryGame/cocos2dx/platform/android/java/gen/.gitignore
git rm --cached */gen/**
git rm --cached */proguard/**
