#include "OPDSDownloader.h"
#include "OPDSSimpleParser.h"
#include <fstream>
#include <iostream>
#include <cstdlib>


int main() {
	std::ofstream fout("opds.txt");
    std::string url = "https://books.fbreader.org/opds";
    while (true){
    	OPDSDownloader downloader;
	    std::string content = downloader.download(url);
	    OPDSSimpleParser parser(content);
	    parser.parse();
	    parser.print_OPDS_tree();
	  	int idx, href_num;
	  	std::cin >> idx;
	  	if (idx == -1){
	  		fout << content;
	  		return 0;
	  	}
	  	std::cin >> href_num;
	  	url = mainDomain + parser.parse_user_input(idx - 1, href_num - 1);
	}
	return 0;
}