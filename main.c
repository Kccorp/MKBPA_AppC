#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <json-c/json.h>


/*void parsingData(char *weather, char *iconId, int *temp, char *file_name)
{
    //parse the file file_name and extract the data for "description", "icon" and "temp
    //and store them in weather, iconId and temp
    FILE *fp = fopen(file_name, "r");


}*/

void get_page(char *url, char *file_name){
    CURL* easyhandle = curl_easy_init();

    curl_easy_setopt( easyhandle, CURLOPT_URL, url ) ;

    FILE* file = fopen( file_name, "w");

    curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file) ;

    curl_easy_perform( easyhandle );

    curl_easy_cleanup( easyhandle );

    fclose(file);

}

void parseJson(char *file_name) {
    //parse the file file_name and extract the data for "description", "icon" and "temp
    //and store them in weather, iconId and temp
    FILE *fp = fopen(file_name, "r");
    char buffer[1024];
    struct json_object *parsed_json;
    struct json_object *description;
    struct json_object *icon;
    struct json_object *temp_obj;

    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    //parse the json
    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "description", &description);
    json_object_object_get_ex(parsed_json, "icon", &icon);
    json_object_object_get_ex(parsed_json, "temp", &temp_obj);
    //extract the data
    printf("%s\n", json_object_to_json_string(description));
    printf("%s\n", json_object_to_json_string(icon));
    printf("%s\n", json_object_to_json_string(temp_obj));


}

int main() {
    char *filename = "../http.txt"; //path from current dir

    get_page( "https://api.openweathermap.org/data/2.5/weather?q=Lyon&appid=a74be1f8fd103c83ce4e4c545a33c915&units=metric&lang=fr&mode=json", filename ) ;
    parseJson(filename);
    return 0;
}
