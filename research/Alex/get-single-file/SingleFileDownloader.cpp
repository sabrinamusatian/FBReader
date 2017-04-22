#include <curl/curl.h>
#include <stdio.h>

void get_page(const char* url, const char* file_name)
{
  CURL* easyhandle = curl_easy_init();

  // access token is received prior to using this program; to refresh it, use refresh_token from Google
  struct curl_slist *slist=NULL; 
  slist = curl_slist_append(slist, "Authorization: Bearer ya29.GlsuBCQdS6mQazZ3y7LmTv8T6aJwefcInoz4i53sjgY7sNYm3VQ5LpkKBGgkNj3Pn5JbOSuTY098FJeKY1sMEr2RbA5yMwYsfYMnRItYivppVDs53lWMrtMB6KZA");
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

int main()
{
  // i've copied this URL from filelist; this is a thumbnail from "Далёкая радуга"
  get_page("https://lh6.googleusercontent.com/PKn8rfKuoW7DTRXW9VG58YS7RHHczyPpYbizQ-v6qV3yolT2q-seXKL3BJCgbnlcndjJyg=s220?alt=media", "thumbnail.jpg");

  return 0;
}