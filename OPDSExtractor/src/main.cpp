#include "OPDSDownloader.h"
#include <fstream>
#include <iostream>

int main() {
	std::ofstream fout("opds.txt");
    OPDSDownloader downloader;
    std::string content = downloader.download();
    fout << content;
}