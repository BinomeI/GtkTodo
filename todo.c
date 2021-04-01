#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
// typedef struct todoArea{
//     GtkWidget *entry;
//     GtkWidget *button;
// }todoArea

//structur pour une tache a realise 
typedef struct {
    GtkWidget *buttonDone; //bouton done 
    GtkWidget *todoLabel; //la tache 
    GtkWidget *buttonDeletion;//supprinner la todo 
}todoItem;

GtkWidget *entry;
GtkWidget *vbox;

static void deleteTodo(GtkWidget *button, todoItem *data)
{
    

    gtk_widget_destroy(data->todoLabel);
    gtk_widget_destroy(data->buttonDone);
    gtk_widget_destroy(data->buttonDeletion);


}

static void doneTodo(GtkWidget *button, todoItem *data)
{
    const char *format = "<span size=\"x-large\" strikethrough=\"true\"><b>%s</b></span>";
    char *markup;
    const char *buff = gtk_label_get_text(GTK_LABEL(data->todoLabel));
    printf("\n%s\n", buff);
    markup = g_markup_printf_escaped (format, buff);
    // gtk_label_set_label(GTK_LABEL(), buff);
    gtk_label_set_markup (GTK_LABEL (data->todoLabel), markup);
    g_free (markup);
}

    

static void addTodo(GtkWidget* button, gpointer data)
{
    char *buff;
    int size = strlen(gtk_entry_get_text(GTK_ENTRY(entry)));
    buff = (char*)malloc(size);
    strcpy(buff, gtk_entry_get_text(GTK_ENTRY(entry)));
    

    printf("%s\n", buff);
    // label to add 
    if(size != 0){
        GtkWidget *hbox;
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
        gtk_container_add(GTK_CONTAINER(vbox), hbox);
        todoItem *todo = (todoItem*)malloc(sizeof(todoItem));
        todo->todoLabel = gtk_label_new(buff);
        const char *format = "<span foreground=\"#dfe6e9\" size=\"x-large\" font_style=\"normal\"><b>%s</b></span>";
        char *markup;

        // pango_parse_markup()

        markup = g_markup_printf_escaped (format, buff);
        gtk_label_set_markup (GTK_LABEL (todo->todoLabel), markup);
        g_free (markup);


        gtk_container_add(GTK_CONTAINER(hbox), todo->todoLabel);

        // todo->buttonDone = gtk_button_new_with_label("Done");
        todo->buttonDone = gtk_button_new_from_icon_name(GTK_STOCK_OK, GTK_ICON_SIZE_BUTTON);
        gtk_container_add(GTK_CONTAINER(hbox), todo->buttonDone);

        todo->buttonDeletion = gtk_button_new_from_icon_name(GTK_STOCK_CLOSE, GTK_ICON_SIZE_BUTTON);
        gtk_container_add(GTK_CONTAINER(hbox), todo->buttonDeletion);

        g_signal_connect(todo->buttonDeletion, "clicked", G_CALLBACK(deleteTodo), (todoItem *) todo);
        g_signal_connect(todo->buttonDone, "clicked", G_CALLBACK(doneTodo), (todoItem *) todo);
        
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    
    gtk_widget_show_all(vbox);
}



static void activate(GtkApplication *App, gpointer user_data)
{
    GtkWidget *window, *button, *table;
    window = gtk_application_window_new(App);
    gtk_window_set_title(GTK_WINDOW(window), "TODO List");

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GtkStyleContext *WindowContext, *EntryContext, *HboxContext, *LabelContext;


    GtkWidget *scrollBar;


    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 500);

    WindowContext = gtk_widget_get_style_context(vbox);
    gtk_style_context_add_class(WindowContext, "M-window");
   
    GtkWidget *label;
    
    const char *buff = "AHRRASS' TODO List";

    label = gtk_label_new(buff);
    const char *format = "<span foreground=\"#dfe6e9\" size=\"x-large\" font_style=\"normal\"><b>%s</b></span>";
    char *markup;

    // pango_parse_markup()

    markup = g_markup_printf_escaped (format, buff);
    gtk_label_set_markup (GTK_LABEL (label), markup);
    g_free (markup);
    //add label
    gtk_container_add(GTK_CONTAINER(vbox), label);

    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    HboxContext = gtk_widget_get_style_context(vbox);
    gtk_style_context_add_class(HboxContext, "M-hbox");

    entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);
    gtk_container_add(GTK_CONTAINER(hbox), entry);

    EntryContext = gtk_widget_get_style_context(vbox);
    gtk_style_context_add_class(EntryContext, "M-entry");

    button = gtk_button_new_from_icon_name(GTK_STOCK_ADD, GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(hbox), button);
    

    g_signal_connect(button, "clicked", G_CALLBACK(addTodo), NULL);
     
    /*Css*/
    gtk_css_provider_load_from_path(cssProvider, "styles.css", NULL);
    gtk_style_context_add_provider(WindowContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(EntryContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(HboxContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    /*End*/

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
}


int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}