#include "OPDSSimpleParser.h";
#include <iostream>

static const std::string TAG_OPEN_TITLE = "<title>";
static const std::string TAG_CLOSE_TITLE = "</title>";
static const std::string TAG_HREF = "<link href='";

OPDSSimpleParser::OPDSSimpleParser(const std::string &OPDSFile) : OPDSFile(OPDSFile){}


void OPDSSimpleParser::print_OPDS_tree(){
	if (OPDS_tree.size() == 0){
		std::cout << "Nothing to be done here" << std::endl;
		return;
	}
    for (size_t i = 0; i < OPDS_tree.size(); i++){
        std::cout << i + 1 << " " << OPDS_tree[i].first << std::endl;
    }
}

std::string OPDSSimpleParser::parse_user_input(const size_t &idx){
	if (idx > OPDS_tree.size() - 1 || idx == 0) return "/opds";
	return OPDS_tree[idx].second;
}

void OPDSSimpleParser::parse(){
	size_t last = 0;
	for (size_t i = 0; i != std::string::npos; i = last){
		last = OPDSFile.find(TAG_OPEN_TITLE, i);
		if (last != std::string::npos){
			last += TAG_OPEN_TITLE.length();
			size_t length_of_name = OPDSFile.find(TAG_CLOSE_TITLE, last + 1) - last;
			std::string title = OPDSFile.substr(last, length_of_name);
			last += length_of_name + TAG_CLOSE_TITLE.length();

			if (OPDS_tree.size() > 0){
				last = OPDSFile.find(TAG_HREF, last) + TAG_HREF.length();
				size_t length_of_href = OPDSFile.find("'", last + 1) - last;
				std::string href = OPDSFile.substr(last, length_of_href);
				OPDS_tree.push_back({title, href});
			}
			else{
				OPDS_tree.push_back({title, ""});
			}
		}
	}
}