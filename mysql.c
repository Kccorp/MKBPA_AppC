#define INSERT_USER "INSERT INTO lotte_weather (date, temperature, icon, description) VALUES (now(), ?,  ?, ?)"
#define SELECT_USER "SELECT email, password from lotte_user"
#define CHECK_USER "SELECT email from lotte_user"

MYSQL_STMT* addUser = NULL;
MYSQL_STMT* selectUser = NULL;
MYSQL_STMT* checkUser = NULL;

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

    addUser = mysql_stmt_init(conn);
    if (addUser == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(addUser, INSERT_USER, strlen(INSERT_USER));
    if (prepare != 0) err_exit("prepare stmt failed");
    count = mysql_stmt_param_count(addUser);
    printf("Il y a %d parametre dans l'sql preparé Insert User\n", count);

    selectUser = mysql_stmt_init(conn);
    if (selectUser == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(selectUser, SELECT_USER, strlen(SELECT_USER));
    if (prepare != 0) err_exit("prepare stmt failed");

    checkUser = mysql_stmt_init(conn);
    if (checkUser == NULL) err_exit("init stmt failed");
    prepare = mysql_stmt_prepare(checkUser, CHECK_USER, strlen(CHECK_USER));
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

void verifUser (MYSQL *conn, char *pseudoSaisie, int *verif){
    char strPseudo[25];
    unsigned long lenName;
    int result;
    int row;
    MYSQL_BIND bind[1]; /*used to get result, not to provide parameters*/
    MYSQL_FIELD *fields;
    MYSQL_RES *metaData;
    //my_bool isNull[1];


    metaData = mysql_stmt_result_metadata(checkUser);
    if (metaData == NULL) err_exit("impossible d'obtenir les metadonnées");

    fields = mysql_fetch_fields(metaData);
    memset(bind,0,sizeof(MYSQL_BIND));

    bind[0].buffer_type = fields[0].type;
    bind[0].buffer = strPseudo;
    bind[0].buffer_length = 25;
    //bind[0].is_null = &isNull[0];
    bind[0].length = &lenName;


    result = mysql_stmt_bind_result(checkUser, bind);
    if (result!=0) err_exit("Le stockage des données à échoué");

    result = mysql_stmt_execute(checkUser);
    if (result!=0) err_exit("l'éxecution du select à échoué");

    row = 0;
    while(1){

        result = mysql_stmt_fetch(checkUser);


        if (result != 0 && row == 0){
            printf("Il y a eu une erreur code:%d\n", result);
            printf("error str is %s \n", mysql_error(conn));
            break;
        }

        strPseudo[lenName]='\0';
        //printf("ligne %d: pseudo=%s Pseudo Saisi=%s \n", row, strPseudo, pseudoSaisie);

        if (strcmp(pseudoSaisie, strPseudo) == 0){
            *verif = 2;
            break;
        }

        if (result != 0 && row != 0){
            *verif = 1;
            break;
        }
        row++;

    }

    mysql_free_result(metaData);
}

void insertUser(char* pseudo, char* pwd ){
    MYSQL_BIND bind[2]; // il y a deux places pseudo et mdp

    unsigned int array_size = 1; /* the number of row to insert? */
    unsigned long pseudoLen = strlen(pseudo);
    unsigned long pwdLen = strlen(pwd);
    int result;

    memset(bind, 0, sizeof(MYSQL_BIND)*2);

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = pseudo;
    bind[0].buffer_length = strlen(pseudo);
    bind[0].length = &pseudoLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = pwd;
    bind[1].buffer_length = strlen(pwd);
    bind[1].length = &pwdLen;

    mysql_stmt_attr_set(addUser, 2, &array_size);
    result = mysql_stmt_bind_param(addUser, bind);
    if (result != 0) err_exit("bind stmt insert failed");
    result = mysql_stmt_execute(addUser);
    if (result != 0) err_exit("execute stmt insert failed");
}

void closePreparedStatements(){
    mysql_stmt_close(addUser);
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