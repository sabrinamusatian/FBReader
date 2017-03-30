#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <fstream>
#include <string>
#include <iostream>
int main()
{

  curl_global_init( CURL_GLOBAL_ALL );
  CURL * myHandle = curl_easy_init ( );

  curl_easy_setopt(myHandle, CURLOPT_AUTOREFERER, 1 );
  curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1 );

  std::ifstream fin("build-cookiebrowser-Desktop-Debug/cookie.txt");
  std::string s;
  std::getline(fin, s);

  curl_easy_setopt(myHandle, CURLOPT_COOKIE, s.c_str());
  curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/5.");
 
  curl_easy_setopt(myHandle, CURLOPT_URL, "https://books.fbreader.org/opds");
  curl_easy_perform( myHandle );

  curl_easy_cleanup( myHandle );
  curl_global_cleanup();
  return 0;
}