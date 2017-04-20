#include "OPDSSimpleParser.h";
#include <iostream>

static const std::string TAG_OPEN_TITLE = "<title>";
static const std::string TAG_CLOSE_TITLE = "</title>";
static const std::string TAG_HREF = "<link href='";
static const std::string TAG_Entry = "<entry>";
static const std::string TAG_Entry_CLOSE = "</entry>";
static const std::string TAG_TYPE = "type='";

OPDSSimpleParser::OPDSSimpleParser(const std::string &OPDSFile) : OPDSFile(OPDSFile){}

typedef std::vector <std::pair<std::string, std::string> > href_tree_type;

void OPDSSimpleParser::print_OPDS_tree(){
	if (OPDS_Title_nodes.size() == 0){
		std::cout << "Nothing to be done here" << std::endl;
		return;
	}
    for (size_t i = 0; i < OPDS_Title_nodes.size(); i++){
        std::cout << i + 1 << " " << OPDS_Title_nodes[i] << std::endl;
        for (size_t j = 0; j < OPDS_tree_href[i].size(); j++){
        	std::cout << "   " << j + 1 << " " << OPDS_tree_href[i][j].first << std::endl;
        }
    }
}

std::string OPDSSimpleParser::parse_user_input(const size_t &idx, const size_t &href_num){
	if (idx > OPDS_Title_nodes.size() - 1 || idx == 0) return "/opds";
	if (href_num > OPDS_tree_href[idx].size()) return "/opds";
	return OPDS_tree_href[idx][href_num].second;
}

void OPDSSimpleParser::parse(){
	size_t last = 0;
	while (last != std::string::npos){
		last = OPDSFile.find(TAG_OPEN_TITLE, last);
		if (last != std::string::npos){
			last += TAG_OPEN_TITLE.length();
			size_t length_of_name = OPDSFile.find(TAG_CLOSE_TITLE, last + 1) - last;
			std::string title = OPDSFile.substr(last, length_of_name);
			last += length_of_name + TAG_CLOSE_TITLE.length();
			if (OPDS_tree_href.size() > 0){
				size_t closing_tile = OPDSFile.find(TAG_Entry_CLOSE, last);
				if (closing_tile == std::string::npos){
					closing_tile = OPDSFile.length() - 1;
				}
				OPDS_Title_nodes.push_back(title);
				href_tree_type href_arr;
				while(true){
					size_t newHref = OPDSFile.find(TAG_HREF, last);
					if (newHref == std::string::npos || newHref > closing_tile){
						break;
					}
					newHref += TAG_HREF.length();
					last = newHref;
					size_t length_of_href = OPDSFile.find("'", last + 1) - last;
					std::string href = OPDSFile.substr(last, length_of_href);
					last += length_of_href;
					last = OPDSFile.find(TAG_TYPE, last) + TAG_TYPE.length();
					size_t length_of_type = OPDSFile.find("'", last + 1) - last;
					std::string type = OPDSFile.substr(last, length_of_type);
					href_arr.push_back({type, href});
					last += length_of_type;
				}
				OPDS_tree_href.push_back(href_arr);
			}
			else{
				OPDS_Title_nodes.push_back(title);
				href_tree_type tmp;
				OPDS_tree_href.push_back(tmp);
			}
		}
	}
}