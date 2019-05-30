/* network-ball-gtk - display network speed for linux
 * Copyright (C) @Baloneo 
 * https://github.com/Baloneo/
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

#define SIZE    (60)        /* left circle size*/
#define RIGHT_CIRCLE_WIDTH  (100)
#define RIGHT_CIRCLE_SIZE (50)
#define PEN_WIDTH (4)
#define MEM_FONT "Ubuntu Mono"
#define NET_FONT "Serif"

static void do_drawing(cairo_t *, GtkWidget *);

static unsigned long long int prD, psD;
static unsigned long int crD, csD; // KB
static unsigned int memPercentage = 0;

int get_mem_percentage() {
    
    // read memory inforation from /proc/meminfo
    FILE *fd_men = fopen("/proc/meminfo", "r");
    if(fd_men == NULL) {
        perror("open file /proc/meminfo error");
        exit(-1);
    }

    char tmp[255];
    fgets(tmp, 255, fd_men);
    unsigned long men = 0;
    unsigned long free = 0;
    sscanf(tmp, "MemTotal: %ld KB", &men);
    fgets(tmp, 255, fd_men);
    fgets(tmp, 255, fd_men);
    sscanf(tmp, "MemAvailable: %ld KB", &free);

    unsigned long use = men - free;
    int percent = ((float)use / (float)men) * 100;

    fclose(fd_men);
    return percent;
}


void get_bandwidth(unsigned long long int *receiveBytes, unsigned long long int *sendBytes) {
    char *buf;
    const int bufSize = 255;
    FILE *devfd;

    buf = (char *) calloc(bufSize, 1);

    devfd = fopen("/proc/net/dev", "r");
    if (devfd == NULL) {
        perror("open file /proc/net/dev failure.");
        exit(-1);
    }

    // Ignore the first and second lines of the file.
    fgets(buf, bufSize, devfd); // fgets will return if reading a newline.
    fgets(buf, bufSize, devfd);
    *receiveBytes = 0;
    *sendBytes = 0;

    while (fgets(buf, bufSize, devfd)) {
        unsigned long long int rBytes, sBytes;
        char *line = strdup(buf);

        char *dev;
        dev = strtok(line, ":");
        gchar *is_lo = g_strrstr(dev, "lo");
        if (is_lo != NULL) { // if end with lo
            continue;
        }
        sscanf(buf + strlen(dev) + 2, "%llu %*d %*d %*d %*d %*d %*d %*d %llu", &rBytes, &sBytes);
        *receiveBytes += rBytes;
        *sendBytes += sBytes;
        free(line);
    }

    fclose(devfd);
    free(buf);
}


static void tran_setup(GtkWidget *win) {
    GdkScreen *screen;
    GdkVisual *visual;

    gtk_widget_set_app_paintable(win, TRUE);
    screen = gdk_screen_get_default();
    visual = gdk_screen_get_rgba_visual(screen);

    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(win, visual); // set transpant
        g_print("is_composited=true");
    } else { // not support
        g_print("is_composited=false");
    }
}

gboolean on_move_event(GtkWidget *window,
              GdkEvent *event,
              gpointer user_data) {
    gtk_window_move(GTK_WINDOW(window), event->button.x_root - (SIZE/2), event->button.y_root - (SIZE/2));
}


void init_value() {

    unsigned long long int rD, sD;// current receive data, send data
    get_bandwidth(&rD, &sD);

    // calculate one second of network speed
    unsigned long int rSpeed = (rD - prD) / 1024;
    unsigned long int sSpeed = (sD - psD) / 1024;

    prD = rD;
    psD = sD;

    crD = rSpeed;
    csD = sSpeed;

    memPercentage = get_mem_percentage();
}

static gboolean cb_timeout(gpointer data) {

    init_value();

    gtk_widget_queue_draw(GTK_WIDGET(data)); // update

    return (TRUE);
}


static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
                              gpointer user_data) {

    cairo_t *first_cr, *second_cr;
    cairo_surface_t *first, *second;

    int width, height;
    char tmp[255];

    GtkWidget *win = gtk_widget_get_toplevel(widget);

    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    first = cairo_surface_create_similar(cairo_get_target(cr),
                                         CAIRO_CONTENT_COLOR_ALPHA, width, height);

    second = cairo_surface_create_similar(cairo_get_target(cr),
                                          CAIRO_CONTENT_COLOR_ALPHA, width, height);

    first_cr = cairo_create(first);
    second_cr = cairo_create(second);


    // draw left circle
    cairo_translate(first_cr, SIZE/2+6, height / 2); // setup center point

    cairo_set_line_width(first_cr, PEN_WIDTH); // setup pen width
    cairo_set_source_rgb(first_cr, 0.9, 0.9, 0.9); // setup color
    cairo_arc(first_cr, 0, 0, SIZE / 2, 0, 2 * M_PI);
    cairo_stroke_preserve(first_cr); // draw stroke

    cairo_set_source_rgb(first_cr, 0.85, 0.8, 0.8); // free memery color
    cairo_fill(first_cr); // fill it

    // calc the arc start angle and end angle
    float start = 0.5 - ((float)memPercentage / 100); 
    float end = 0.5 + ((float)memPercentage / 100);

    // fill memory "water"
    cairo_arc(first_cr, 0, 0, SIZE / 2, M_PI * start, M_PI * end);
    if(memPercentage <=50) {
        cairo_set_source_rgb(first_cr, 0.1, 0.7, 0.1); // green
    } else if(memPercentage <= 79) {
        cairo_set_source_rgb(first_cr, 1, 0.6, 0); // yellow
    } else {
        cairo_set_source_rgb(first_cr, 0.9, 0, 0); // red
    }

    cairo_fill(first_cr); // fill memory

    // draw memory info
    cairo_set_source_rgb(first_cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(first_cr, MEM_FONT,
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(first_cr, 16);
    cairo_move_to(first_cr, -12, 4);
    sprintf(tmp, "%d%%", memPercentage);
    cairo_show_text(first_cr, tmp);


    // draw rectangle
    cairo_set_source_rgb(second_cr, 0.9, 0.9, 0.9);
    cairo_rectangle(second_cr, 0, 0, RIGHT_CIRCLE_WIDTH, RIGHT_CIRCLE_SIZE);
    cairo_fill(second_cr);

    // draw right circle
    cairo_set_source_rgb(second_cr, 0.9, 0.9, 0.9); // The same color as the rectangle
    cairo_translate(second_cr, 100, 25);
    cairo_arc(second_cr, 0, 0, RIGHT_CIRCLE_SIZE / 2, 0, 2 * M_PI);
    cairo_fill(second_cr);

    // draw net-spped text
    cairo_set_source_rgb(second_cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(second_cr, NET_FONT,
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(second_cr, 11);

    cairo_move_to(second_cr, -(RIGHT_CIRCLE_WIDTH-SIZE/2), -8);
    sprintf(tmp, " ↑ %lu kb/s", csD);
    cairo_show_text(second_cr, tmp);
    cairo_move_to(second_cr, -(RIGHT_CIRCLE_WIDTH-SIZE/2), 15);
    sprintf(tmp, " ↓ %lu kb/s", crD);
    cairo_show_text(second_cr, tmp);

    // add to cr
    cairo_set_operator(first_cr, CAIRO_OPERATOR_DEST_OVER); // let the left circle cover the right rectangle
    cairo_set_source_surface(first_cr, second, 0, -SIZE/2+5);
    cairo_paint(first_cr);

    cairo_set_source_surface(cr, first, 0, 0);
    cairo_paint(cr);

    // destroy
    cairo_surface_destroy(first);
    cairo_surface_destroy(second);
    cairo_destroy(first_cr);
    cairo_destroy(second_cr);

    return FALSE;
}

static gboolean
button_press_event(GtkWidget *widget, GdkEventButton *event) {
    return TRUE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *darea;

    init_value();
    init_value();

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_POPUP);
    tran_setup(window);  // let window transpant

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);


    // draw ball
    darea = gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    gtk_container_add(GTK_CONTAINER(window), darea);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), RIGHT_CIRCLE_WIDTH + (RIGHT_CIRCLE_SIZE/2) + (SIZE/2) + PEN_WIDTH*2, SIZE+PEN_WIDTH);

    // let the window move
    g_signal_connect(G_OBJECT(window), "motion-notify-event", G_CALLBACK(on_move_event), NULL);

    // timer
    gdk_threads_add_timeout_full(G_PRIORITY_DEFAULT_IDLE, 1000, cb_timeout, (gpointer) darea, NULL);

    // click event (did not use)
    g_signal_connect (G_OBJECT(window), "button_press_event",
                      G_CALLBACK(button_press_event), NULL);

    // show window
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}