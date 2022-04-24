#define INSERT_WEATHER "INSERT INTO lotte_weather (date, temperature, icon, description) VALUES (now(), ?,  ?, ?)"
#define SELECT_USER "SELECT email, password from lotte_user"

MYSQL_STMT* addWeather = NULL;
MYSQL_STMT* selectUser = NULL;

void err_exit(char* s);

void connectBD (int *state, MYSQL *conn){

    if(conn == NULL) { /*If it returns NULL, the initialization failed*/
        printf("mysql_init failed!\n");
    }

    conn=mysql_real_connect(conn,"ilzyz0heng1bygi8.chr7pe7iynqr.eu-west-1.rds.amazonaws.com","zi079ph5hsli9dsz","uesgcm2e869kdo87","m6qku2mxgghdq37a",3306,NULL,0);
    if (conn) {
        printf("\033[0;34mConnection success! \033[0m\n");
        *state = 1;
    }else{
        printf("\033[0;31mConnection failed! \033[0m\n");
        *state = 0;
    }
}

void initPrepareSql (MYSQL *conn){
    int prepare;
    unsigned int count;

    addWeather = mysql_stmt_init(conn);
    if (addWeather == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(addWeather, INSERT_WEATHER, strlen(INSERT_WEATHER));
    if (prepare != 0) err_exit("prepare stmt failed");
    count = mysql_stmt_param_count(addWeather);
    //printf("Il y a %d parametre dans l'sql preparé Insert User\n", count);

    selectUser = mysql_stmt_init(conn);
    if (selectUser == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(selectUser, SELECT_USER, strlen(SELECT_USER));
    if (prepare != 0) err_exit("prepare stmt failed");
}

void showUser (MYSQL *conn, char *pseudoSaisie, char *password){

    char strPseudo[25];
    char strpwd[150];
    unsigned long lenName;
    unsigned long lenpwd;
    int result;
    int row;
    MYSQL_BIND bind[2]; /*used to get result, not to provide parameters*/
    MYSQL_FIELD *fields;
    MYSQL_RES *metaData;

    metaData = mysql_stmt_result_metadata(selectUser);
    if (metaData == NULL) err_exit("impossible d'obtenir les metadonnées");

    fields = mysql_fetch_fields(metaData);
    memset(bind,0,sizeof(MYSQL_BIND)*2);

    bind[0].buffer_type = fields[0].type;
    bind[0].buffer = strPseudo;
    bind[0].buffer_length = 25;
    bind[0].length = &lenName;

    bind[1].buffer_type = fields[1].type;
    bind[1].buffer = strpwd;
    bind[1].buffer_length = 100;
    bind[1].length = &lenpwd;


    result = mysql_stmt_bind_result(selectUser, bind);
    if (result!=0) err_exit("Le stockage des données à échoué");

    result = mysql_stmt_execute(selectUser);
    if (result!=0) err_exit("l'éxecution du select à échoué");

    row = 0;
    while(1){
        row++;
        result = mysql_stmt_fetch(selectUser);
        if (result == MYSQL_NO_DATA){
            //printf("-- FIN --\n");
            break;
        }
        if (result != 0){
            printf("Il y a eu une erreur code:%d\n", result);
            printf("error str is %s \n", mysql_error(conn));
            break;
        }

        strPseudo[lenName]='\0';
        strpwd[lenpwd]='\0';
        //printf("ligne %d: id=%d pseudo=%s password=%s\n", row, id, strPseudo, strpwd);

        if (strcmp(pseudoSaisie, strPseudo) == 0){
            //printf("\nUn compte existe");
            strcpy(password, strpwd);
            break;
        }

    }

    mysql_free_result(metaData);
    if (metaData==NULL)
        printf("metaData clear");
}

void insertWeather(double temperature, char* icon, char* description){
    MYSQL_BIND bind[3];

    unsigned int array_size = 1; /* the number of row to insert? */
//    unsigned long temperatureLen = strlen(temperature);
    unsigned long iconLen = strlen(icon);
    unsigned long descriptionLen = strlen(description);
    int result;

    memset(bind, 0, sizeof(MYSQL_BIND)*3);

    //insert double value for temperature in bind[0]
    bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[0].buffer = &temperature;
    bind[0].buffer_length = sizeof(temperature);

//    bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
//    bind[0].buffer = temperature;
//    bind[0].buffer_length = strlen(temperature);
//    bind[0].length = &temperatureLen;

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
    mysql_stmt_close(selectUser);
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