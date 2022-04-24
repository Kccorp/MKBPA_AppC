void vider_buffer(void);
void remove_n(char *str, int n);
void get_page(char *url, char *file_name);
int getResultApi(char *filename);


void main_menu (int *choice){
    while (*choice > 3 || *choice <= 0 ){
        printf("\n1)Se connecter\n2)S'inscrire\n3)Exit\n");
        scanf("%d", choice);
        vider_buffer();
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
    get_page(url, "../test.html");

    switch (getResultApi("../test.html")) {
        case 0:printf("Password is correct\n");*connected=1;break;
        case 1:printf("Password is incorrect\n");*connected=0;break;
        default:exit(-1);break;
    }
}

void connectUser (int *connected, MYSQL *conn, int argc, char **argv, char *pseudo, char *window_size_x, char *window_size_y){
    int choice;
    char pwd[100];
    char checkPwd[100]={0};
    int i=0;

    //choix entre connexion/inscription
    main_menu(&choice);

    if (choice == 1){//Choisir de se connecter

        do {
            windowConnect(argc, argv, pwd, pseudo, window_size_x, window_size_y);
            showUser (conn, pseudo, checkPwd);
            check_password(pwd, checkPwd, connected);
            if(*connected==0)tmpGtkError=2;
        } while (*connected == 0);


    } else if (choice == 2) {//Choisir de S'inscrire
        int uniqueName=0;

        //demande à l'utilisateur ses infos de comtpes et vérifie si un pseudo identique existe déjà
        do {
            windowConnect(argc, argv, pwd, pseudo, window_size_x, window_size_y);
            remove_n(pseudo, 25);
            verifUser(conn,pseudo, &uniqueName);
            if(uniqueName!=0)tmpGtkError=1;
        } while (pseudo[0]=='\0' || uniqueName !=1);


        //fait l'injection dans la db
        insertUser(pseudo, pwd);
        *connected = 1;
        printf("\nInscription validée - vous êtes connecté\n");

    } else if (choice == 3) {
        printf("\nGoodbye");
        exit(0);
    }
}

