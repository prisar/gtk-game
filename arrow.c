#include<stdio.h>
#include<string.h>
#include <gtk/gtk.h>
GtkWidget *fixed,*window;
GtkWidget *bckgrnd_wid,*archr_wid,*arrow_wid,*balloon_wid,*burst_wid;
GtkWidget *label;
int timer;
int y_cord=0;
int x_cord=50;
int arrow_tag=0;
int score=0,hit=0,miss=0;


GtkWidget *xpm_create( GtkWidget *parent,gchar *xpm_filename)
{
    GtkWidget *box1;
    GtkWidget *label;
    GtkWidget *pixmapwid;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle *style;
    style = gtk_widget_get_style(parent);
    pixmap = gdk_pixmap_create_from_xpm (parent->window, &mask,
                                         &style->bg[GTK_STATE_NORMAL],
                                         xpm_filename);
    pixmapwid = gtk_pixmap_new (pixmap, mask);
    return(pixmapwid);
}
int move_balloon(GtkWidget *data)
{
   y_cord+=2;
   if(y_cord>650)
   {
   gtk_widget_destroy(balloon_wid);
   balloon_wid=xpm_create(window, "balloon.png");
   gtk_widget_show(balloon_wid);
   y_cord=0;
   data=balloon_wid;
   gtk_fixed_put (GTK_FIXED (fixed), balloon_wid , 1100, y_cord);   
   }
   else
   gtk_fixed_move (GTK_FIXED (fixed), balloon_wid, 1100, y_cord);
   return TRUE;
}
int move_arrow(GtkWidget *data)
{
   if(x_cord>=990 && x_cord<=1005 && y_cord>=570 && y_cord<=605)
   {
    gtk_widget_destroy(balloon_wid);
    balloon_wid=xpm_create(window, "burst.png");
    gtk_widget_show(balloon_wid);
    gtk_fixed_put (GTK_FIXED (fixed), balloon_wid , 1100, y_cord);
    score++;
    //g_print("Score : %d \n",score);
    set_score();
    hit=1;
    miss=0;
   }
   if(x_cord>1150)
   {   
   x_cord=50;
   gtk_fixed_move (GTK_FIXED (fixed), data , x_cord, 600);
   gtk_timeout_remove (arrow_tag);
   if(hit==0)
   miss++;
   if(miss==3)
   game_over();
   arrow_tag=0;
   hit=0;
   }
   gtk_fixed_move (GTK_FIXED (fixed), data , x_cord, 600);
   x_cord+=15;
   return TRUE;
}
void shoot(GtkWidget *wid, GtkWidget *data)
{
if(arrow_tag!=0)
return;
arrow_tag = gtk_timeout_add (10, move_arrow, data);
}    

void set_score()
{
char score_arr[5],point[12];
sprintf (score_arr, "%d", (int) score);
char *lab="SCORE : ";
strcpy(point,lab);
strcat(point,score_arr);
gtk_label_set(GTK_LABEL(label),&point);
}

void restart(GtkWidget *wid, GtkWidget *data)
{
gtk_widget_destroy(data);
score=0;
set_score();
x_cord=50;
y_cord=0;
timer = gtk_timeout_add (10, move_balloon, balloon_wid);
}
void game_over()
{
gtk_timeout_remove (timer);
GtkWidget *pop,*pop_lab,*pop_button,*box,*pop_play;
pop=gtk_window_new(GTK_WINDOW_TOPLEVEL);
 gtk_widget_set_usize(GTK_WIDGET(pop),250,250);
pop_lab=gtk_label_new(" ");
char status[100];
strcpy(status,"Game over! Final score : ");
char score_arr[5];
sprintf (score_arr, "%d", (int) score);
strcat(status,score_arr);
gtk_label_set(GTK_LABEL(pop_lab),&status);
pop_button=gtk_button_new_with_label("Exit!");
pop_play=gtk_button_new_with_label("Play Again!");
g_signal_connect(G_OBJECT(pop_button),"clicked",gtk_main_quit,NULL);
g_signal_connect(G_OBJECT(pop_play),"clicked",restart,pop);
box=gtk_vbox_new(TRUE,3);
gtk_box_pack_start (GTK_BOX (box),pop_lab, FALSE, FALSE, 5);
gtk_box_pack_start (GTK_BOX (box), pop_play, FALSE, FALSE, 5);
gtk_box_pack_start (GTK_BOX (box), pop_button, FALSE, FALSE, 5);
gtk_container_add (GTK_CONTAINER (pop), box);
gtk_widget_show_all(pop);
}



int main( int   argc,
          char *argv[] )
{
    /* GtkWidget is the storage type for widgets */
    GtkWidget *button;
    //int timer;
    gtk_init (&argc, &argv);

    /* Create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    fixed = gtk_fixed_new ();   
    gtk_window_set_title (GTK_WINDOW (window), "Get A Life!");

    
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                        GTK_SIGNAL_FUNC (gtk_exit), NULL);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (gtk_exit), NULL);

   
    gtk_widget_realize(window);
    button = gtk_button_new_with_label("SHOOT!");
    label=gtk_label_new("SCORE : 0");
    gtk_widget_show(label);
    bckgrnd_wid= xpm_create(window, "backgrnd.jpg");
    gtk_widget_set_usize(GTK_WIDGET(bckgrnd_wid),1500,350);
    gtk_widget_show(bckgrnd_wid);  
    gtk_fixed_put (GTK_FIXED (fixed), button, 200, 200);
    gtk_fixed_put (GTK_FIXED (fixed), bckgrnd_wid, 100, 100);
    gtk_widget_show(button);  
    gtk_widget_set_usize(GTK_WIDGET(label),100,100);
    gtk_fixed_put (GTK_FIXED (fixed), label, 20, 20);
    archr_wid= xpm_create(window, "archer.png");
    gtk_widget_show(archr_wid);
    gtk_fixed_put (GTK_FIXED (fixed), archr_wid, 50, 550);
    arrow_wid= xpm_create(window, "arrow.png");
    gtk_widget_show(arrow_wid);
    gtk_fixed_put (GTK_FIXED (fixed), arrow_wid, x_cord, 600);    
    g_signal_connect(G_OBJECT (button), "clicked",G_CALLBACK(shoot),arrow_wid);     
    balloon_wid= xpm_create(window, "balloon.png");
    gtk_widget_show(balloon_wid);
    gtk_fixed_put (GTK_FIXED (fixed), balloon_wid, 1100, y_cord);
    timer = gtk_timeout_add (10, move_balloon, balloon_wid);
/* Pack and show all our widgets */
    gtk_widget_show(fixed);
    gtk_container_add (GTK_CONTAINER (window), fixed);
    gtk_widget_set_usize(GTK_WIDGET(window),1200,700);
    gtk_widget_show (window);

    /* Rest in gtk_main and wait for the fun to begin! */
    gtk_main ();

    return(0);
}
/* example-end */
