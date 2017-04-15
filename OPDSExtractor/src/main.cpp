#include "OPDSDownloader.h"
#include "OPDSSimpleParser.h"
#include <fstream>
#include <iostream>
#include <cstdlib>


int main() {
	std::ofstream fout("opds.txt");
	const std::string mainDomain = "https://books.fbreader.org";
    std::string url = "https://books.fbreader.org/opds";
    while (true){
    	OPDSDownloader downloader;
	    std::string content = downloader.download(url);
	    OPDSSimpleParser parser(content);
	    parser.parse();
	    parser.print_OPDS_tree();
	  	size_t idx;
	  	std::cin >> idx;
	  	if (idx == -1){
	  		fout << content;
	  		return 0;
	  	}
	  	url = mainDomain + parser.parse_user_input(idx - 1);
	  	std::cout << url << std::endl;
	}
	return 0;
    //fout << content;
}