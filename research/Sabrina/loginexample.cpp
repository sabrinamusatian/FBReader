#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int main()
{

  curl_global_init( CURL_GLOBAL_ALL );
  CURL * myHandle = curl_easy_init ( );

  curl_easy_setopt(myHandle, CURLOPT_AUTOREFERER, 1 );
  curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1 );
  curl_easy_setopt(myHandle, CURLOPT_COOKIE, "cookie.txt");
  curl_easy_setopt(myHandle, CURLOPT_COOKIELIST, "cookie.txt");
  curl_easy_setopt(myHandle, CURLOPT_COOKIEFILE, "cookie.txt");

  curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYPEER, false);

  curl_easy_setopt(myHandle, CURLOPT_UNRESTRICTED_AUTH, 1);
  curl_easy_setopt(myHandle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
  curl_easy_setopt(myHandle, CURLOPT_ENCODING, "");
  curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/5.");
 
  curl_easy_setopt(myHandle, CURLOPT_URL, "https://books.fbreader.org/opds");
  curl_easy_perform( myHandle );
  char *urlct;
  CURLcode url = curl_easy_getinfo(myHandle, CURLINFO_EFFECTIVE_URL, &urlct);
  curl_easy_cleanup( myHandle );
  curl_global_cleanup();
  return 0;
}