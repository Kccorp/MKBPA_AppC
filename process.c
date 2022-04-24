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

void startConnection(MYSQL *connOld, MYSQL *connNew, int *state){
    closeDb(connOld);
    connectBD(state, connNew);
    initPrepareSql(connNew);
}
