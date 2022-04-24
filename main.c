#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include "mysql.c"
#include "graphique.c"
#include "account.c"
#include "process.c"


int main(int argc, char **argv){
    MYSQL *conn= mysql_init(NULL);/*Create database link pointer*/

    int state = 0, connected = 0;
    char email[145];
    connectBD(&state, conn);/*Connect to database*/
    initPrepareSql(conn);/*Prepare sql*/

    connectUser(&connected, conn, argc, argv, email, "450", "200");/*Connect user*/

    double temp = 0;
    char icon[5];
    char weather[50];


    MYSQL *conn1= mysql_init(NULL);/*Create database link pointer*/
    startConnection(conn, conn1, &state);/*Connect to database*/


    showWeather(conn1, &temp, icon, weather);/*Get weather info*/


    //convert temp to string
    char temp_str[4];
    snprintf(temp_str, 4,"%.02lf", temp);

    windowWeather(argc, argv, icon, temp_str, weather, "350", "200");/*Show weather*/

    closeDb(conn1);/*Close database & Close prepared statements*/

    return 0;
}

