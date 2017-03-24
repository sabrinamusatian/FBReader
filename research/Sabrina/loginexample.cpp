#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
//#include <curl/types.h>
//#include <curl/easy.h>


int main()
{

  curl_global_init( CURL_GLOBAL_ALL );
  CURL * myHandle = curl_easy_init ( );

  // Set up a couple initial paramaters that we will not need to mofiy later.
  //curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/4.0");
  curl_easy_setopt(myHandle, CURLOPT_AUTOREFERER, 1 );
  curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1 );
  curl_easy_setopt(myHandle, CURLOPT_COOKIEJAR, "cookies.txt");
  curl_easy_setopt(myHandle, CURLOPT_COOKIEFILE, "cookies.txt");

  //curl_easy_setopt(myHandle, CURLOPT_RETURNTRANSFER, 1);
  curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYPEER, false);

  curl_easy_setopt(myHandle, CURLOPT_UNRESTRICTED_AUTH, 1);
  curl_easy_setopt(myHandle, CURLOPT_USERNAME, "mambo.jambo@gmail.com");
  curl_easy_setopt(myHandle, CURLOPT_PASSWORD, "sabrina1133");
  curl_easy_setopt(myHandle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
  curl_easy_setopt(myHandle, CURLOPT_ENCODING, "");
  curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/5.");
 
  // Visit the login page once to obtain a PHPSESSID cookie
  curl_easy_setopt(myHandle, CURLOPT_URL, "https://books.fbreader.org/catalog#by_title");
  curl_easy_perform( myHandle );
  char *urlct;
  CURLcode url = curl_easy_getinfo(myHandle, CURLINFO_EFFECTIVE_URL, &urlct);

  // Now, can actually login. First we forge the HTTP referer field, or HTS will deny the login
 curl_easy_setopt(myHandle, CURLOPT_REFERER, urlct);
 curl_easy_setopt(myHandle, CURLOPT_POST, 1);
 char *data = "bgresponse=js_disabled&page=PasswordSeparationSignIn&GALX=pg3Z8Ix3Ezw&gxf=AFoagUXn1mbbk69c0V1DNQOPIaN0XCDv5Q%3A1490353548109&ltmpl=nosignup&scc=1&sarp=1&oauth=1&_utf8:%E2%98%83&pstMsg=1&checkConnection=youtube%3A693%3A1&checkedDomains=youtube&Email=mambo.jambo1133%40gmail.com&Passwd=sabrina1133&PersistentCookie=yes";
  curl_easy_setopt(myHandle, CURLOPT_POSTFIELDS, data);
  // Next we tell LibCurl what HTTP POST data to submit
 CURLcode res1 = curl_easy_perform(myHandle);
 /*if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
  */

 if(res1 != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res1));

//curl_easy_setopt(myHandle, CURLOPT_POST, 0);
curl_easy_perform(myHandle);
// get info part DEBUG
char *ct;
CURLcode res = curl_easy_getinfo(myHandle, CURLINFO_EFFECTIVE_URL, &ct);
long response_code;
curl_easy_getinfo(myHandle, CURLINFO_RESPONSE_CODE, &response_code);
printf("%ld\n",response_code);
      if((CURLE_OK == res) && ct)
        printf("We received: %s\n", ct);

curl_easy_cleanup( myHandle );
curl_global_cleanup();
  return 0;
}