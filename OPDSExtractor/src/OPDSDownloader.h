#ifndef OPDSDOWNLOADER_HPP
#define OPDSDOWNLOADER_HPP

#include <string>
#include <curl/curl.h>

class OPDSDownloader{
public:
    OPDSDownloader();
    ~OPDSDownloader();
    std::string download();
private:
    CURL *curl;
};

#endif  /* OPDSDOWNLOADER_HPP */