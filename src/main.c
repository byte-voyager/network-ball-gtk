/* network-ball-gtk - display network speed and mem for linux
 * Copyright (C) @Baloneo
 * https://github.com/Baloneo/
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This application is distributed in the hope that it will be useful,
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

#include "app_utils.h"

#define SIZE (52) /* left circle size*/
#define RIGHT_CIRCLE_WIDTH (84)
#define RIGHT_CIRCLE_SIZE (52)
#define PEN_WIDTH (2)
int NET_SPEED_TEXT_MARGIN_LEFT = 0;
#define MEM_FONT "xxx"
#define NET_FONT "xxx"

static gboolean SHOW_NETWORK_SPEED = TRUE;


static unsigned long long int prD, psD;
static unsigned long int crD, csD; // KB
static unsigned int memPercentage = 0;
static float rand_color1 = 0;
static float rand_color2 = 0;
static float rand_color3 = 0;

static void tran_setup(GtkWidget *win) {
    GdkScreen *screen;
    GdkVisual *visual;

    gtk_widget_set_app_paintable(win, TRUE);
    screen = gdk_screen_get_default();
    visual = gdk_screen_get_rgba_visual(screen);

    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(win, visual); // set transparent
        g_print("is_composited=true\n");
    } else { // not support
        g_print("Your system not support transparent window!\nPlease check if you have turned off this feature.\n");
    }
}

gboolean on_move_event(GtkWidget *window,
                       GdkEvent *event,
                       gpointer user_data) {
//    g_print("on_move_event event->button %d; event-type %d\n", event->button, event->type);
//    printf("++++%d\n", ((GdkEventMotion*)event)->state);
//    printf("---%d\n", GDK_BUTTON1_MASK);
    if (((GdkEventMotion*)event)->state == 272) {  // fixed manjaro cannot move
        gtk_window_move(GTK_WINDOW(window), event->button.x_root - (SIZE / 2), event->button.y_root - (SIZE / 2));
        return TRUE;
    }

    if (((GdkEventMotion*)event)->state == GDK_BUTTON1_MASK) {
        gtk_window_move(GTK_WINDOW(window), event->button.x_root - (SIZE / 2), event->button.y_root - (SIZE / 2));
        return TRUE;
    }


    return TRUE;

}

void init_value() {

    unsigned long long int rD, sD; // current receive data, send data
    get_bandwidth(&rD, &sD);

    // calculate one second of network speed
    unsigned long int rSpeed = (rD - prD) / 1024; // recv speed (kb/s)
    unsigned long int sSpeed = (sD - psD) / 1024; // send speed (kb/s)

    prD = rD;
    psD = sD;

    crD = rSpeed;
    csD = sSpeed;

    memPercentage = get_mem_percentage();
}



static gboolean cb_timeout(gpointer data) {

    init_value();

    // g_print("%.1f", random_color());

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
    cairo_translate(first_cr, SIZE / 2 + PEN_WIDTH, height / 2); // setup center point

    cairo_set_line_width(first_cr, PEN_WIDTH);                                            // setup pen width
    cairo_set_source_rgba(first_cr, get_color(193), get_color(205), get_color(193), 0.5); // setup color

    cairo_arc(first_cr, 0, 0, SIZE / 2, 0, 2 * M_PI);
    cairo_stroke_preserve(first_cr); // draw stroke

    cairo_set_line_width(first_cr, PEN_WIDTH);     // setup pen width
    cairo_set_source_rgba(first_cr, 1, 1, 1, 0.3); // free memory color
    cairo_fill(first_cr);                          // fill free mem

    // calc the arc start angle and end angle
    float start = 0.5 - ((float) memPercentage / 100);
    float end = 0.5 + ((float) memPercentage / 100);


    // fill use memory
    cairo_arc(first_cr, 0, 0, SIZE / 2 - 2, M_PI * start, M_PI * end);
    if (memPercentage <= 50) {
        cairo_set_source_rgb(first_cr, 0.1, 0.7, 0.1); // green
    } else if (memPercentage <= 85) {  // %85 mem usage
        cairo_set_source_rgb(first_cr, 1, 0.6, 0); // yellow
    } else {
        cairo_set_source_rgb(first_cr, 0.9, 0, 0); // red
    }

    // warn the user if memory too hight
    if (memPercentage >= 95) {
        system("notify-send -i dialog-warning -t 10 \"Memory is too high\" \"$(ps -eo pid,ppid,%mem,%cpu,comm --sort=-%mem | head -6 | tail -5)\"");
    }

    if (rand_color1 != 0 || rand_color2 != 0 || rand_color3 != 0)  {
        cairo_set_source_rgb(first_cr, rand_color1, rand_color2, rand_color3);
    }

    cairo_fill(first_cr); // fill use memory

    // draw memory info
    cairo_set_source_rgb(first_cr, 0.1, 0.1, 0);
    cairo_select_font_face(first_cr, MEM_FONT,
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(first_cr, 14);
    cairo_move_to(first_cr, -12, 4);
    sprintf(tmp, "%d%%", memPercentage);
    cairo_show_text(first_cr, tmp);

    if (SHOW_NETWORK_SPEED) {
        // draw an incomplete rectangle, missing a semicircle on the left
        cairo_set_source_rgba(second_cr, get_color(193), get_color(205), get_color(193), 0.9);
        cairo_arc(second_cr, 0, SIZE / 2, SIZE / 2, -0.5 * M_PI, 0.5 * M_PI); // draw semicircle, like ")"
        cairo_line_to(second_cr, RIGHT_CIRCLE_WIDTH,
                      RIGHT_CIRCLE_SIZE + (SIZE - RIGHT_CIRCLE_SIZE) / 2); // line to rectangle (right, bottom)
        cairo_line_to(second_cr, RIGHT_CIRCLE_WIDTH, (SIZE - RIGHT_CIRCLE_SIZE) / 2); // line to (right, top)
        cairo_fill(second_cr);

        cairo_close_path(second_cr);

        // draw right arc
        cairo_set_source_rgba(second_cr, get_color(193), get_color(205), get_color(193),
                              0.9); // The same color as the rectangle
        cairo_translate(second_cr, RIGHT_CIRCLE_WIDTH, SIZE / 2); // set up (0, 0) to （right_edge,center)
        cairo_arc(second_cr, 0, 0, RIGHT_CIRCLE_SIZE / 2, -0.5 * M_PI, 0.5 * M_PI);
        cairo_fill(second_cr);

        // draw net-spped text
        cairo_set_source_rgb(second_cr, 0.1, 0.1, 0.1);

        cairo_select_font_face(second_cr, NET_FONT,
                               CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(second_cr, 12);

        cairo_move_to(second_cr, -((RIGHT_CIRCLE_WIDTH - SIZE / 2) - NET_SPEED_TEXT_MARGIN_LEFT), -8);
        if (csD >= 2048) {
            sprintf(tmp, " ↑%.2f m/s", kb2m(csD));
        } else {
            sprintf(tmp, " ↑ %lu kb/s", csD);
        }

        cairo_show_text(second_cr, tmp);
        cairo_move_to(second_cr, -((RIGHT_CIRCLE_WIDTH - SIZE / 2) - NET_SPEED_TEXT_MARGIN_LEFT), 15);
        if (crD >= 1024) {
            sprintf(tmp, " ↓%.2f m/s", kb2m(crD));
        } else {
            sprintf(tmp, " ↓ %lu kb/s", crD);
        }

        cairo_show_text(second_cr, tmp);
    }

    // add to cr
    cairo_set_operator(first_cr, CAIRO_OPERATOR_DEST_OVER); // let the left circle cover the right rectangle
    cairo_set_source_surface(first_cr, second, 0, -(SIZE) / 2); // set up (0, 0) to right circle's top position
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


static gboolean button_press_event(GtkWidget *widget, GdkEventButton *event) {
    if(event->type == GDK_DOUBLE_BUTTON_PRESS) {
        SHOW_NETWORK_SPEED = !SHOW_NETWORK_SPEED;
        gtk_widget_queue_draw(widget);
    }

    if (event->type == GDK_TRIPLE_BUTTON_PRESS) {

        if (rand_color() > 0.7) {
            rand_color2 = rand_color();
            rand_color1 = rand_color2 /3;
            rand_color3 = rand_color2 /2;
        } else if (rand_color() > 0.2) {
            rand_color3 = rand_color() / 2;
            rand_color2 = rand_color2 /2;
            rand_color1 = rand_color2 /2;
        } else {
            rand_color1 = rand_color() * 4;
            rand_color2 = rand_color1 /2;
            rand_color3 = rand_color1 /2;
        }
    }

    return TRUE;
}

int main(int argc, char *argv[]) {

    int time = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-delay") == 0) {
            if (argc > (i + 1)) {
                time = atoi(argv[i + 1]);
            } else {
                printf("Error: missing delay value\n");
            }

        } else if (strcmp(argv[i], "-margin") == 0) {
            if (argc > (i + 1)) {
                int margin = atoi(argv[i + 1]);
                NET_SPEED_TEXT_MARGIN_LEFT = margin;
            } else {
                printf("Error: missing margin value\n");
            }


        } else if (strcmp(argv[i], "-h") == 0) {
            printf("OPTIONS: \n\t-delay\tdelay load app. \n\t-margin\tset netspeed text left margin.\n");
            return 0;
        }
    }
    sleep(time);


    GtkWidget *window;
    GtkWidget *darea;

    init_value();
    init_value();

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_POPUP);
    tran_setup(window); // let window transparent

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    // draw ball
    darea = gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    gtk_container_add(GTK_CONTAINER(window), darea);


    // gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),
                                RIGHT_CIRCLE_WIDTH + (RIGHT_CIRCLE_SIZE / 2) + (SIZE / 2) + PEN_WIDTH * 2,
                                SIZE + PEN_WIDTH + 4);

    // let the window move enter-notify-event motion-notify-event
    gtk_widget_set_events (window, GDK_POINTER_MOTION_MASK);
    g_signal_connect(G_OBJECT(window), "motion-notify-event", G_CALLBACK(on_move_event), NULL);

    // timer
    gdk_threads_add_timeout_full(G_PRIORITY_DEFAULT_IDLE, 1000, cb_timeout, (gpointer) darea, NULL);

    // click event
    g_signal_connect(G_OBJECT(window), "button_press_event",
                     G_CALLBACK(button_press_event), NULL);

    // GdkMonitor *m =  gdk_display_get_primary_monitor(gdk_display_get_default());

    // gtk_window_move(GTK_WINDOW(window), gdk_monitor_get_width_mm(m), gdk_monitor_get_height_mm(m));

    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);
    gtk_window_move(GTK_WINDOW(window), workarea.width - 200, workarea.height - 100);

    // show window
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
