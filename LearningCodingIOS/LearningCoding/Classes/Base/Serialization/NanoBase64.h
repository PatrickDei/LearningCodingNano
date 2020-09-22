//
//  Base64.h
//  Hollywood
//
//  Created by Matko Juribasic on 20/05/14.
//
//

#ifndef __Hollywood__Base64__
#define __Hollywood__Base64__

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_encode_from_file(const char *);
std::string base64_decode(std::string const& s);
void base64_decode_to_file(const char *, std::string const& s);

#endif /* defined(__Hollywood__Base64__) */
