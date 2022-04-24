#include <gtk/gtk.h>
#include <stdlib.h>

static void CallBrowser (GtkAboutDialog *about, const gchar *data1, void *data2)
{
    char *cmd = NULL;

    cmd = g_strdup_printf ("%s %s &", "firefox", (char*)data1);
    system (cmd);
    g_free (cmd), cmd = NULL;

    (void)about;
    (void)data2;
}

int main(int argc, char **argv)
{
    GtkWidget *dlg = NULL;

    gtk_init (&argc, &argv);

    dlg = gtk_about_dialog_new ();
    //gtk_about_dialog_set_url_hook (CallBrowser, NULL, NULL);
    gtk_about_dialo;
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dlg), "http://gtk.developpez.com/faq/");
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (dlg), "FAQ GTK+");
    gtk_dialog_run (GTK_DIALOG (dlg));

    gtk_main ();
    return 0;
}