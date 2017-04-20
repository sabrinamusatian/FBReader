#ifndef OPDSSIMPLEPARSER_HPP
#define OPDSSIMPLEPARSER_HPP

#include <string>
#include <vector> 
#include <utility> 

class OPDSSimpleParser{
public:
    explicit OPDSSimpleParser(const std::string &OPDSFile);
    ~OPDSSimpleParser(){};
    void parse();
    void print_OPDS_tree();
    std::string parse_user_input(const size_t &idx, const size_t &href_num);
private:
	std::string OPDSFile;
	std::vector <std::string> OPDS_Title_nodes;
    std::vector <std::vector <std::pair<std::string, std::string> > > OPDS_tree_href;
};

#endif  /* OPDSSIMPLEPARSER_HPP */