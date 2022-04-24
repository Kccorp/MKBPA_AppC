void verify_password(char *password, int *verif);
void remove_n(char *chaine, int size);

static GtkWidget *inputname;
static GtkWidget *inputpassword;
static GtkWidget *resultPseudo;
static GtkWidget *resultPassword;
static GtkWidget *error;

static char logPseudo [25];
static char logPassword [100];

static int tmpGtkError = 0;

void newUser(GtkWidget *widget, gpointer user_data){
    char buffer[25] ;
    int check=1, checkPassword=0;


    verify_password((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword)), &checkPassword);
//    remove_n((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword)), 100);

    if (strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputname))) > 25 ||
        strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputname))) <= 0 ||
        strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputname))) <= 3    ){

        gtk_label_set_text(GTK_LABEL(resultPseudo), "Email non conforme");
        check = 0;

    } else if (strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputname))) <= 25 && strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputname))) > 3){

        gtk_label_set_text(GTK_LABEL(resultPseudo), "Email OK");
        strcpy(logPseudo, (char *)gtk_entry_get_text(GTK_ENTRY(inputname)));

    }

    if (strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword))) > 100 ||
        strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword))) <= 0 ||
        strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword))) < 4 ||
        checkPassword == 0){

        gtk_label_set_text(GTK_LABEL(resultPassword), "Mot de passe non conforme");
        check = 0;

    } else if (strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword))) <= 25 &&
               strlen((char *)gtk_entry_get_text(GTK_ENTRY(inputpassword))) > 3){

        gtk_label_set_text(GTK_LABEL(resultPassword), "Mot de passe OK ");
        strcpy(logPassword, (char *)gtk_entry_get_text(GTK_ENTRY(inputpassword)));

    }


    if (check == 1 && checkPassword == 1){
        gtk_window_close(widget);
    }
}

void windowConnect(int argc, char **argv, char *pwd, char *psd, char *window_size_x, char *window_size_y) {
    GtkWidget *window, *grid, *login;
    gtk_init(&argc, &argv);

    GtkWidget *name;
    GtkWidget *password;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "loTTe");
    if (atoi(window_size_x) != 0 && atoi(window_size_y) != 0)
        gtk_window_set_default_size(GTK_WINDOW(window), atoi(window_size_x), atoi(window_size_y));
    gtk_window_set_resizable (GTK_WINDOW(window), TRUE);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // ajout du champ de gestion des erreurs
    error = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), error, 1, 0, 1, 1);
    if (tmpGtkError==1)gtk_label_set_text(GTK_LABEL(error), "Email déjà utilisé");
    if (tmpGtkError==2)gtk_label_set_text(GTK_LABEL(error), "Mot de passe ou Email incorrect");

    // creation du label Email
    name = gtk_label_new("Email :");
    gtk_grid_attach(GTK_GRID(grid), name, 0, 1, 1, 1);
    inputname = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), inputname, 0, 3, 1, 1);

    // creation du label password
    password = gtk_label_new("Mot de passe :");
    gtk_grid_attach(GTK_GRID(grid), password, 0, 5, 1, 1);
    inputpassword = gtk_entry_new();
    gtk_entry_set_visibility(inputpassword, FALSE);
    gtk_grid_attach(GTK_GRID(grid), inputpassword, 0, 6, 1, 1);

    // Champ de saisie du pseudo et du mot de passe
    resultPseudo = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), resultPseudo, 2, 3, 1, 1);

    resultPassword = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), resultPassword, 2, 6, 1, 1);

    //creation du bouton de connexion
    login = gtk_button_new_with_label("Se connecter");

    g_signal_connect(login, "clicked", G_CALLBACK(newUser), NULL);
    gtk_grid_attach(GTK_GRID(grid), login, 2, 7, 1, 1);


    gtk_widget_show_all(window);
    gtk_main();

    strcpy(pwd, logPassword);
    strcpy(psd, logPseudo);

}

void windowWeather(int argc, char **argv, char *icon, char *temperature, char *describe, char *window_size_x, char *window_size_y) {
    GtkWidget *window, *grid;
    gtk_init(&argc, &argv);

    GtkWidget *displayTemperature;
    GtkWidget *displayDescribe;
    GtkWidget *displayIcon;
    char *iconPath;

    GdkRGBA gray;
    gray.red = 0.29; //74/256
    gray.green = 0.4; //102/256
    gray.blue = 0.44; //112/256
    gray.alpha = 1;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "loTTe");
    gtk_widget_override_background_color(window, (GtkStateFlags) GTK_STATE_NORMAL, &gray);
    if (atoi(window_size_x) != 0 && atoi(window_size_y) != 0)
        gtk_window_set_default_size(GTK_WINDOW(window), atoi(window_size_x), atoi(window_size_y));
    gtk_window_set_resizable (GTK_WINDOW(window), TRUE);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Affiche de l'icone
    iconPath = g_strconcat("../assets/icons/", icon, ".png", NULL);
    displayIcon = gtk_image_new_from_file(iconPath);
//    gtk_widget_set_halign(displayIcon, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), displayIcon, 5, 0, 1, 1);


    // Affichage de la temperature
    displayTemperature = gtk_label_new(g_strconcat(temperature, "°C", NULL));
    gtk_grid_attach(GTK_GRID(grid), displayTemperature, 2, 5, 1, 1);

    // creation du label password
    displayDescribe = gtk_label_new(describe);
    gtk_grid_attach(GTK_GRID(grid), displayDescribe, 10, 5, 1, 1);

    //change the font size
    PangoFontDescription *font_desc;
    font_desc = pango_font_description_from_string("Sans Bold 14");
    gtk_widget_override_font(displayTemperature, font_desc);
    gtk_widget_override_font(displayDescribe, font_desc);



    gtk_widget_show_all(window);
    gtk_main();


}