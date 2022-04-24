void vider_buffer(void);
void remove_n(char *str, int n);
void get_page(char *url, char *file_name);
int getResultApi(char *filename);


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

void verify_password (char *password, int *check){
    int i;
    int checkMaj=0, checkMin=0, checkNumber=0;

    remove_n(password, 100);

    for (i = 0; i < strlen(password); ++i) {
        if (password[i] >= 65 && password[i] <= 90)checkMaj=1;
        if (password[i] >= 97 && password[i] <= 122)checkMin=1;
        if (password[i] >= 48 && password[i] <= 57)checkNumber=1;
    }

    if (checkMaj == 1 && checkMin == 1 && checkNumber == 1)*check=1;
}

void createUrl(char *url, char *pwd, char *hashedPwd){
    strcpy(url, "https://db-for-lotte.herokuapp.com/?pwd=");
    strcat(url, pwd);
    strcat(url, "&pwd_hash=");
    strcat(url, hashedPwd);
}

void check_password(char *pwd, char *checkPwd, int *connected){
    char url[150];
    createUrl(url, pwd, checkPwd);
    get_page(url, "../hashVerif.html");

    switch (getResultApi("../hashVerif.html")) {
        case 0:*connected=1;break;
        case 1:*connected=0;break;
        default:exit(-1);break;
    }
}

void connectUser (int *connected, MYSQL *conn, int argc, char **argv, char *pseudo, char *window_size_x, char *window_size_y){
    char pwd[100];
    char checkPwd[100]={0};

    do {
        windowConnect(argc, argv, pwd, pseudo, window_size_x, window_size_y);
        showUser (conn, pseudo, checkPwd);
        check_password(pwd, checkPwd, connected);
        if(*connected==0)tmpGtkError=2;
    } while (*connected == 0);
}

