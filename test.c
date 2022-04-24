#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include "mysql.c"
#include "graphique.c"
#include "account.c"


void vider_buffer(void){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void remove_n(char *chaine, int size){
    int i;
    for ( i = 0; i < size; ++i) {
        if (chaine[i] == '\n'){
            chaine[i] = 0;
        }
    }
}


void get_page(char *url, char *file_name){
    CURL* easyhandle = curl_easy_init();

    curl_easy_setopt( easyhandle, CURLOPT_URL, url ) ;

    FILE* file = fopen( file_name, "w");

    curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file) ;

    curl_easy_perform( easyhandle );

    curl_easy_cleanup( easyhandle );

    fclose(file);

}

int getResultApi(char *filename){
    char line[25];
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fgets(line, 25, f);
    fclose(f);
    if(strcmp(line, "Password is correct") == 0 ) {
        return 0;
    }
    else if(strcmp(line, "Password is incorrect") == 0 ) {
        return 1;
    } else {
        return -1;
    }

}

int main(int argc, char **argv){
    MYSQL *conn= mysql_init(NULL);/*Create database link pointer*/

    int state = 0, connected = 0;
    char email[145];
    connectBD(&state, conn);/*Connect to database*/
    initPrepareSql(conn);/*Prepare sql*/



    connectUser(&connected, conn, argc, argv, email, "450", "200");/*Connect user*/

    windowWeather(argc, argv, "10d", "14", "Pluie", "350", "200");/*Show weather*/

    closeDb(conn);/*Close database & Close prepared statements*/

    return 0;
}

