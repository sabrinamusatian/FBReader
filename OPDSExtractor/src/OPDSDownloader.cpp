#include "OPDSDownloader.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::string data((const char*) ptr, (size_t) size * nmemb);
    *((std::stringstream*) stream) << data;
    return size * nmemb;
}

OPDSDownloader::OPDSDownloader() {
    curl = curl_easy_init();
}

OPDSDownloader::~OPDSDownloader() {
    curl_easy_cleanup(curl);
}

void OPDSDownloader::googleOAuth(){
    std::system("cookieUtility/cookiebrowser");
}

std::string OPDSDownloader::download() {
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1 );
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1 );

    std::ifstream fin("src/cookie.txt");
    std::string s;
    std::cout << s;
    std::getline(fin, s);

    curl_easy_setopt(curl, CURLOPT_COOKIE, s.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.");
 
    curl_easy_setopt(curl, CURLOPT_URL, "https://books.fbreader.org/opds");
    
    std::stringstream out;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);

    CURLcode res = curl_easy_perform(curl);
    
    /* Check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    
    return out.str();
}