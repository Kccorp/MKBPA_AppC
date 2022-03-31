#include <stdio.h>
#include <curl/curl.h>
#include <string.h>


void get_page(char *url, char *file_name){
    CURL* easyhandle = curl_easy_init();

    curl_easy_setopt( easyhandle, CURLOPT_URL, url ) ;

    FILE* file = fopen( file_name, "w");

    curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file) ;

    curl_easy_perform( easyhandle );

    curl_easy_cleanup( easyhandle );

    fclose(file);

}

int main() {
    char *filename = "../http.txt"; //path from current dir

    get_page( "https://api.openweathermap.org/data/2.5/weather?q=Lyon&appid=a74be1f8fd103c83ce4e4c545a33c915&units=metric&lang=fr&mode=xml", filename ) ;

    return 0;
}
