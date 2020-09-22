package org.nanobit.mystory;

public class StringUtils {
	
	public static String join( Iterable<String> items, String joiner ){
		String joinedString = "";
		boolean ignoreSplit = true;
		for( String item : items ){
			if( ignoreSplit ){
				ignoreSplit = false;
			}else{
				joinedString += joiner;
			}
			joinedString += item;				
		}
		return joinedString;
	}
	
}
