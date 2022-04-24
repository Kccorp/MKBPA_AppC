#define INSERT_WEATHER "INSERT INTO lotte_weather (date, temperature, icon, description) VALUES (now(), ?,  ?, ?)"

MYSQL_STMT* addWeather = NULL;

void err_exit(char* s);

void connectBD (int *state, MYSQL *conn){

    if(conn == NULL) { /*If it returns NULL, the initialization failed*/
        printf("mysql_init failed!\n");
    }

    conn=mysql_real_connect(conn,"ilzyz0heng1bygi8.chr7pe7iynqr.eu-west-1.rds.amazonaws.com","zi079ph5hsli9dsz","uesgcm2e869kdo87","m6qku2mxgghdq37a",3306,NULL,0);
    if (conn) {
//        printf("\033[0;34mConnection success! \033[0m\n");
        *state = 1;
    }else{
//        printf("\033[0;31mConnection failed! \033[0m\n");
        *state = 0;
    }
}

void initPrepareSql (MYSQL *conn){
    int prepare;

    addWeather = mysql_stmt_init(conn);
    if (addWeather == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(addWeather, INSERT_WEATHER, strlen(INSERT_WEATHER));
    if (prepare != 0) err_exit("prepare stmt failed");

}

void insertWeather(double temperature, char *icon, char *description){
    unsigned int array_size = 1; /* the number of row to insert? */
    unsigned long iconLen = strlen(icon);
    unsigned long descriptionLen = strlen(description);
    int result;

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(MYSQL_BIND)*3);

    //insert double value for temperature in bind[0]
    bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[0].buffer = &temperature;
    bind[0].buffer_length = sizeof(temperature);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = icon;
    bind[1].buffer_length = strlen(icon);
    bind[1].length = &iconLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = description;
    bind[2].buffer_length = strlen(description);
    bind[2].length = &descriptionLen;

    mysql_stmt_attr_set(addWeather, 2, &array_size);
    result = mysql_stmt_bind_param(addWeather, bind);
    if (result != 0) err_exit("bind stmt insert failed");
    result = mysql_stmt_execute(addWeather);
    if (result != 0) err_exit("execute stmt insert failed");
}

void closePreparedStatements(){
    mysql_stmt_close(addWeather);
}

void closeDb(MYSQL* dbconn){
    closePreparedStatements();
    mysql_close(dbconn);
}

void err_exit(char* s){
    perror(s);
    perror("\n");
    exit(1);
}

