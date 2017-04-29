#include <curl/curl.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "json.hpp"
#include "HTTPDownloader.h"
#include "AuthorisationManager.h"

using json = nlohmann::json;

void save_to_string(void * curl, const char * url, std::string& result)
{
  result = HTTPDownloader().download(curl, url);
}


void save_to_string_authorised(void * curl, const char * url, std::string& result)
{
  result = HTTPDownloader().download_authorised(curl, url);
}


void get_page(const char* url, const char* file_name)
{
  CURL* easyhandle = curl_easy_init();

  // access token is received prior to using this program; to refresh it, use refresh_token from Google
  struct curl_slist *slist=NULL; 
  slist = curl_slist_append(slist, std::string("Authorization: Bearer " + AuthorisationManager::getInstance().getAuthorisationToken()).c_str());
  curl_easy_setopt( easyhandle, CURLOPT_HTTPHEADER, slist);

  curl_easy_setopt( easyhandle, CURLOPT_URL, url ) ;

  FILE* file = fopen( file_name, "w");
  curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file) ;

  curl_easy_perform( easyhandle );

  // cleanup
  curl_slist_free_all(slist);
  curl_easy_cleanup(easyhandle);
  fclose(file);

}

std::string to_string(const json& j)
{
  std::stringstream ss;
  ss << j;
  return ss.str().substr(1, ss.str().size()-2);
}



void find_library(const json& filelist, std::string& id)
{
  for(auto it = filelist["items"].begin(); it != filelist["items"].end(); ++it)
  {
    //auto item = *it;
    std::stringstream ss_title;
    ss_title << (*it)["title"];

    if(ss_title.str().find("FBReader") != std::string::npos)
    {
      id = to_string((*it)["id"]);
      return;
    }
  }
}

int main()
{
  AuthorisationManager::getInstance().authorise();

  std::string filelist;
  void * curl = curl_easy_init();
  save_to_string_authorised(curl, "https://www.googleapis.com/drive/v2/files", filelist);
  curl_easy_cleanup(curl);
  
  json filelist_json = json::parse(filelist);
  std::string id;

  find_library(filelist_json, id);
  std::cout << id << std::endl;
  
  for(auto it = filelist_json["items"].begin(); it != filelist_json["items"].end(); ++it)
  {
    auto parents = (*it)["parents"];
    for(auto jt = parents.begin(); jt != parents.end(); ++jt)
    {
      std::string parent_id = to_string((*jt)["id"]);
      //std::cout << "  " << parent_id << std::endl;
      
      if(parent_id == id)
      {
        std::string file_id = to_string((*it)["id"]);
        std::string file_output_name = file_id + ".fb2.zip";
        std::string thumb_output_name = file_id + "_thumb.jpg";

        std::string file_download_link = to_string((*it)["selfLink"]) + "?alt=media";
        std::string thumb_download_link = to_string((*it)["thumbnailLink"]) + "?alt=media";

        get_page(file_download_link.c_str(), file_output_name.c_str());
        get_page(thumb_download_link.c_str(), thumb_output_name.c_str());
      }
    }
  }

  return 0;
}