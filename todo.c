#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>


//structur pour une tache a realise 
typedef struct {
    GtkWidget *buttonDone; //bouton done 
    GtkWidget *todoLabel; //la tache 
    GtkWidget *buttonDeletion;//supprinner la todo 
}todoItem;


GtkWidget *vbox1;

typedef struct {
    GtkWidget *entry;
    GtkWidget *addButton;
    GtkWidget *label;
}EntryArea;


static void deleteTodo(GtkWidget *button, todoItem *data)
{
    gtk_widget_destroy(data->todoLabel);
    gtk_widget_destroy(data->buttonDone);
    gtk_widget_destroy(data->buttonDeletion);
}

static void doneTodo(GtkWidget *button, todoItem *data)
{
    const char *format = "<span size=\"x-large\"><s><b>%s</b></s></span>";
    char *markup;
    const char *buff = gtk_label_get_text(GTK_LABEL(data->todoLabel));
    printf("\n%s\n", buff);
    markup = g_markup_printf_escaped (format, buff);
    gtk_label_set_markup (GTK_LABEL (data->todoLabel), markup);
    g_free (markup);
}

    
static void addTodo(GtkWidget* button, EntryArea *data)
{
    char *buff;
    int size = strlen(gtk_entry_get_text(GTK_ENTRY(data->entry)));
    buff = (char*)malloc(size);
    strcpy(buff, gtk_entry_get_text(GTK_ENTRY(data->entry)));
    
    printf("%s\n", buff);
    // label to add 
    if(size != 0)
    {
        GtkWidget *hbox;
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
        gtk_container_add(GTK_CONTAINER(vbox1), hbox);

        //declaration de la tache a faire 
        todoItem *todo = (todoItem*)malloc(sizeof(todoItem));
        todo->todoLabel = gtk_label_new(buff);

        const char *format = "<span foreground=\"#dfe6e9\" size=\"x-large\" font_style=\"normal\"><b>%s</b></span>";
        char *markup;

        

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
        
        gtk_entry_set_text(GTK_ENTRY(data->entry), "");
    }
    gtk_widget_show_all(vbox1);
}



static void activate(GtkApplication *App, gpointer user_data)
{
    GtkWidget *window, *button, *table;
    window = gtk_application_window_new(App);
    gtk_window_set_title(GTK_WINDOW(window), "TODO List");

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GtkStyleContext *WindowContext, *EntryContext, *HboxContext, *LabelContext, *scrollContext;

    EntryArea *entryAdd = (EntryArea*)malloc(sizeof(EntryArea));

    GtkWidget *vbox0;
    vbox0 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(window), vbox0);

    GtkWidget *vbox;
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(vbox0), vbox);

    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    
    GtkWidget *wind = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(wind, 300, 250);
    gtk_container_add(GTK_CONTAINER(wind), vbox1);
    gtk_container_add(GTK_CONTAINER(vbox0), wind);

    /*context for wind*/
    scrollContext = gtk_widget_get_style_context(wind);
    gtk_style_context_add_class(scrollContext, "M-windsc");
    /*end*/

    gtk_window_set_default_size(GTK_WINDOW(window), 100, 500);

    WindowContext = gtk_widget_get_style_context(vbox);
    gtk_style_context_add_class(WindowContext, "M-window");
   
    const char *buff = "AHRRASS' TODO List";

    entryAdd->label = gtk_label_new(buff);
    const char *format = "<span foreground=\"#dfe6e9\" size=\"x-large\" font_style=\"normal\"><b>%s</b></span>";
    char *markup;

    markup = g_markup_printf_escaped (format, buff);
    gtk_label_set_markup (GTK_LABEL (entryAdd->label), markup);
    g_free (markup);
    //add label
    gtk_container_add(GTK_CONTAINER(vbox), entryAdd->label);

    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    HboxContext = gtk_widget_get_style_context(vbox);
    gtk_style_context_add_class(HboxContext, "M-hbox");

    entryAdd->entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entryAdd->entry), TRUE);
    gtk_container_add(GTK_CONTAINER(hbox), entryAdd->entry);

    EntryContext = gtk_widget_get_style_context(vbox);
    gtk_style_context_add_class(EntryContext, "M-entry");

    entryAdd->addButton = gtk_button_new_from_icon_name(GTK_STOCK_ADD, GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(hbox), entryAdd->addButton);
    

    g_signal_connect(entryAdd->addButton, "clicked", G_CALLBACK(addTodo), (EntryArea *) entryAdd);
     
    /*Css*/
    gtk_css_provider_load_from_path(cssProvider, "styles.css", NULL);
    gtk_style_context_add_provider(WindowContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(EntryContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(HboxContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(scrollContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
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