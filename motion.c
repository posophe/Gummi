#include "motion.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "environment.h"
#include "utils.h"

extern Gummi* gummi;

GuMotion* motion_init(gint dum) {
    GuMotion* m = (GuMotion*)g_malloc(sizeof(GuMotion));
    m->dummy = dum;
    return m;
}

void motion_initial_preview(GuEditor* ec) {
    motion_update_workfile(ec);
    motion_update_pdffile(ec);
    // if succes.. 
    preview_set_pdffile(gummi->pdffile);
}


void motion_update_workfile(GuEditor* ec) {
    GtkTextIter start;
    GtkTextIter end;
    gchar *text;
    FILE *fp;

    // TODO: the following line caused hangups in python, attention!
    gtk_widget_set_sensitive(ec->sourceview, FALSE);
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(ec->sourcebuffer),
            &start, &end);
    text = gtk_text_iter_get_text (&start, &end);
    gtk_widget_set_sensitive(ec->sourceview, TRUE);
    
    fp =fopen(gummi->workfile, "w");
    
    if(fp == NULL) {
        perror("failed to open workfile");
        // TODO: do your debug thingie!
    }
    fwrite(text, strlen(text), 1, fp);
    g_free(text);
    fclose(fp);
    // TODO: Maybe add editorviewer grab focus line here if necessary
}

void motion_update_pdffile(GuEditor* ec) {
    FILE *fp;
    int status;
    char path[PATH_MAX];
    char command[PATH_MAX];
    snprintf(command, sizeof command, "pdflatex "
                                    "-interaction=nonstopmode "
                                    "-file-line-error "
                                    "-halt-on-error "
                                    "-output-directory='%s' '%s'", \
                                    gummi->tmpdir, gummi->workfile);
    fp = popen(command, "r");
    if (fp == NULL) {
        // handle error
    }
    while (fgets(path, PATH_MAX, fp) != NULL)
        printf("%s", path);
        
    status = pclose(fp);
    if (status == -1) {
         // handle error
    }
}
