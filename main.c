#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "mysql.c"


void get_page(char *url, char *file_name){
    CURL* easyhandle = curl_easy_init();

    curl_easy_setopt( easyhandle, CURLOPT_URL, url ) ;

    FILE* file = fopen( file_name, "w");

    curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file) ;

    curl_easy_perform( easyhandle );

    curl_easy_cleanup( easyhandle );

    fclose(file);

}

void parseJson(char *file_name,double *t, char *w, char *i){
    //parse the file file_name and extract the data for "description", "icon" and "temp
    //and store them in weather, iconId and temp
    FILE *fp = fopen(file_name, "r");
    char buffer[1024];
    char *p;
    struct json_object *parsed_json;
    struct json_object *description;
    struct json_object *icon;
    struct json_object *temperature;
    struct json_object *temp;

    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    //parse the json
    parsed_json = json_tokener_parse(buffer);
    //get the temp (we first put the inner json string into a json object then we access again the inside of the json object to get the wanted key
    json_object_object_get_ex(parsed_json, "main", &temperature);
    json_object_object_get_ex(temperature, "temp", &temperature);
    json_object_object_get_ex(parsed_json, "weather", &temp);
    temp=json_object_array_get_idx(temp, 0);
    json_object_object_get_ex(temp, "description", &description);
    json_object_object_get_ex(temp, "icon", &icon);

    //convert the json object to string
    strcpy(i, json_object_get_string(icon));
    strcpy(w, json_object_get_string(description));
    // we put inside a temp named p a string then atof to store it in a double
    p = json_object_get_string(temperature);
    *t = atof(p);
    //for free the memory
    json_object_put(parsed_json);

}

int main() {
    char *filename = "../http.txt"; //path from current dir
    double temp = 0;
    char *weather = malloc(sizeof(char)*100);
    char *iconId = malloc(sizeof(char)*4);


    get_page( "https://api.openweathermap.org/data/2.5/weather?q=Lyon&appid=a74be1f8fd103c83ce4e4c545a33c915&units=metric&lang=fr&mode=json", filename ) ;
    parseJson(filename,&temp,weather,iconId);

    printf("weather : %s\n", weather);
    printf("iconId : %s\n", iconId);
    printf("how hot i am ? : %.2lf\n", temp);

    MYSQL *conn= mysql_init(NULL);/*Create database link pointer*/

    int state = 0, connected = 0;
    char email[145];
    connectBD(&state, conn);/*Connect to database*/
    initPrepareSql(conn);/*Prepare sql*/

    //insert the data into the database
    insertWeather(temp, iconId, weather);



    free(weather);
    free(iconId);
    return 0;
}
