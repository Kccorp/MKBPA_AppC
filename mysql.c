#define SELECT_USER "SELECT email, password from lotte_user"
#define SELECT_WEATHER "SELECT temperature, icon, description FROM lotte_weather ORDER BY idWeather DESC LIMIT 1"


MYSQL_STMT* selectUser = NULL;
MYSQL_STMT* selectWeather = NULL;

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

    selectUser = mysql_stmt_init(conn);
    if (selectUser == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(selectUser, SELECT_USER, strlen(SELECT_USER));
    if (prepare != 0) err_exit("prepare stmt failed");

    selectWeather = mysql_stmt_init(conn);
    if (selectWeather == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(selectWeather, SELECT_WEATHER, strlen(SELECT_WEATHER));
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

void showWeather (MYSQL *conn, double *temperature, char *icon, char *weather){

    char strIcon[25];
    char strWeather[150];
    double degre;
    unsigned long lenIcon;
    unsigned long lenWeather;
    int result;
    int row;
    MYSQL_BIND bind[3]; /*used to get result, not to provide parameters*/
    MYSQL_FIELD *fields;
    MYSQL_RES *metaData;

    metaData = mysql_stmt_result_metadata(selectWeather);
    if (metaData == NULL) err_exit("impossible d'obtenir les metadonnées");

    fields = mysql_fetch_fields(metaData);
    memset(bind,0,sizeof(MYSQL_BIND)*3);


    bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[0].buffer = temperature;
    bind[0].buffer_length = sizeof(double);
    bind[0].length = NULL;

    bind[1].buffer_type = fields[1].type;
    bind[1].buffer = icon;
    bind[1].buffer_length = strlen(icon);
    bind[1].length = &lenIcon;

    bind[2].buffer_type = fields[2].type;
    bind[2].buffer = weather;
    bind[2].buffer_length = 50;
    bind[2].length = &lenWeather;


    result = mysql_stmt_bind_result(selectWeather, bind);
    if (result!=0) err_exit("Le stockage des données à échoué");

    result = mysql_stmt_execute(selectWeather);
    if (result!=0) err_exit("l'éxecution du select à échoué");

    row = 0;
    while(1){
        row++;
        result = mysql_stmt_fetch(selectWeather);
        if (result == MYSQL_NO_DATA){
            //printf("-- FIN --\n");
            break;
        }
        if (result != 0){
            printf("Il y a eu une erreur code:%d\n", result);
            printf("error str is %s \n", mysql_error(conn));
            break;
        }
    }

    mysql_free_result(metaData);
    if (metaData==NULL)
        printf("metaData clear");
}


void closePreparedStatements(){
    mysql_stmt_close(selectUser);
    mysql_stmt_close(selectWeather);
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