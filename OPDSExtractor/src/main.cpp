#include "OPDSDownloader.h"
#include <fstream>
#include <iostream>
#include <cstdlib>


int main() {
	std::ofstream fout("opds.txt");
    OPDSDownloader downloader;
    std::system("cookieUtility/cookiebrowser");
    std::string content = downloader.download();
    fout << content;
}