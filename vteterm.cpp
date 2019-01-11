/// -----------------------------------------------------------------
/// Jean-Pierre Laroche
/// projet 2018-08-08  (C) 2018   Copyright 2018  <laroche.jeanpierre@gmail.com>
/// but : 	terminal rapide	/ flexible / respectant le code escape
///			fast / flexible terminal / respecting the escape code
/// 		console dédié uniquement à une application multi-process
///			console dedicated to an application only multi-process
///			-no-pie EXÉCUTABLE  programme maitre mini server terminal
///			+ souple que XTERM et + sécuritaire que les terminal public  pour des applicatifs



///			outil pour développer une application de type 5250 / 3270 ou terminal semi-graphic
///			tool to develop a 5250/3270 or terminal semi-graphic application







/// THANK YOU   MERCI BEAUCOUP
/// thanks Mr. CHRISTOPHE BLAESS for the book development system LINUX 1..4 edition en Français " ouf ;) "

/// GTK platform and GNOME for VTE-TERMINAL

/// https://github.com/SgtWiggles/ume   thank you that inspired me to set up a dedicated terminal

/// https://vincent.bernat.im/fr/blog/2017-ecrire-emulateur-terminal

/// tank you github for exemple open-source for etude

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
*/

/// à été compiler avec  C++17

///------------------------------------------
/// paramétrage spécifique
/// ex:
///------------------------------------------

#define WORKLIB		"/home/soleil/VTETERM/pgm/"
#define WORKPGM		"./jplecr"


bool _DEBUG_  = true; /// ALT_F4 ATVIVE  _DEBUG_ = true

#define MESSAGE_ALT_F4 "vous devez activer uniquement \n en développemnt  \n Confirm destroy Application --> DEBUG"


/// ----------------------------------------
/// par default
///-----------------------------------------
#define VTENAME "VTE-TERM3270"

#define COL		132	/// max 132

#define ROW		42	/// max 43 including a line for the system

/// defined not optional
#define VTEFONT	"DejaVu Sans Mono"

//*******************************************************
// PROGRAME
//*******************************************************

#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include <X11/Xlib.h>
#include <vte/vte.h>
#include <pango/pango.h>
#include <sys/types.h>


#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include <gdk/gdkx.h>



GtkWidget	*window, *terminal;

GPid child_pid = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		procédure de gestion de message SHM
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
int *key__num ;
int shmid;
 
void init_Keyboard(void)							// init smh GDK keyboard communication server vteterm 
{
	char *t_key =  new char[30];
	sprintf(t_key, "SHMGDK%d",getpid());
	key_t vtekey= *t_key;
	shmid = shmget(vtekey, sizeof(int), 0666 | IPC_CREAT |SHM_RND);
	key__num = ( int *) shmat(shmid, NULL, 0);

	shmctl(shmid,SHM_LOCK,0);
	delete [] t_key;
}

void close_Keyboard(void)							// init smh GDK keyboard communication server vteterm 
{

	shmctl(shmid,IPC_RMID,0);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		procédure de call execv ... system shell pour diverse commande .pgm.... or  VTETERM application
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///--------------------------------------------
/// traitement for Kill not excute system(...)
///--------------------------------------------
int exec_prog(const char* commande)
{
	int retour;
	pid_t pid;
	char * P_cmd = new char[strlen(commande)+1];
	sprintf(P_cmd,"%s",commande);
	char *argx[] = {(char*)"sh",(char*)"-c",P_cmd,NULL};
	free(P_cmd);


	if((pid = fork()) <0)
		return EXIT_FAILURE ;

	if(pid == 0)
	{
		execv("/bin/sh",argx);
	}

	delete argx[3];delete argx[2];delete argx[1];delete argx[0];
	
	while ( waitpid(pid, &retour,0) <0)
		if (errno!= EINTR)
			return EXIT_FAILURE ;

	return EXIT_SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		function mouse scroll   GDK_SCROLL_UP->GDK_KEY_Up 					GDK_SCROLL_DOWN->GDK_KEY_Down
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mouse_scroll(GtkWidget *widget, GdkEventScroll *event, gpointer user_data)
{
	*key__num =0;
	if (event->direction == GDK_SCROLL_UP )		*key__num = 213;;
	if (event->direction == GDK_SCROLL_DOWN )	*key__num = 214;;

	gtk_widget_activate (terminal);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		function keyboard only key type F1 etc....  Enter KP_enter etc.... 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	*key__num =0;
 
    const guint modifiers = event->state & gtk_accelerator_get_default_mod_mask();
    const guint keyval = event->keyval;

    if (modifiers == GDK_CONTROL_MASK)
    {
        switch (keyval)
        {
            /* Ctrl+ */
			case GDK_KEY_A:
			case GDK_KEY_a:
				*key__num = 1; break;
			case GDK_KEY_B:
			case GDK_KEY_b:
				*key__num = 2; break;
			case GDK_KEY_C:
			case GDK_KEY_c:
				*key__num = 3; break;
			case GDK_KEY_D:
			case GDK_KEY_d:
				*key__num = 4; break;
			case GDK_KEY_E:
			case GDK_KEY_e:
				*key__num = 5; break;
			case GDK_KEY_F:
			case GDK_KEY_f:
				*key__num = 6; break;
			case GDK_KEY_G:
			case GDK_KEY_g:
				*key__num = 7; break;
			case GDK_KEY_H:
			case GDK_KEY_h:
				*key__num = 8; break;
			case GDK_KEY_I:
			case GDK_KEY_i:
				*key__num = 9; break;
			case GDK_KEY_J:
			case GDK_KEY_j:
				*key__num = 10; break;
			case GDK_KEY_K:
			case GDK_KEY_k:
				*key__num = 11; break;
			case GDK_KEY_L:
			case GDK_KEY_l:
				*key__num = 12; break;
			case GDK_KEY_M:
			case GDK_KEY_m:
				*key__num = 13; break;
			case GDK_KEY_N:
			case GDK_KEY_n:
				*key__num = 14; break;
			case GDK_KEY_O:
			case GDK_KEY_o:
				*key__num = 15; break;
			case GDK_KEY_P:
			case GDK_KEY_p:
				*key__num = 16; break;
			case GDK_KEY_Q:
			case GDK_KEY_q:
				*key__num = 17; break;
			case GDK_KEY_R:
			case GDK_KEY_r:
				*key__num = 18; break;
			case GDK_KEY_S:
			case GDK_KEY_s:
				*key__num = 19; break;
			case GDK_KEY_T:
			case GDK_KEY_t:
				*key__num = 20; break;
			case GDK_KEY_U:
			case GDK_KEY_u:
				*key__num = 21; break;
			case GDK_KEY_V:
			case GDK_KEY_v:
				*key__num = 22; break;
			case GDK_KEY_W:
			case GDK_KEY_w:
				*key__num = 23; break;
			case GDK_KEY_X:
			case GDK_KEY_x:
				*key__num = 24; break;
			case GDK_KEY_Y:
			case GDK_KEY_y:
				*key__num = 25; break;
			case GDK_KEY_Z:
			case GDK_KEY_z:
				*key__num = 26; break;
			case GDK_KEY_Tab:
				*key__num = 203; break;
			default :
				*key__num =0; break;
        }
    } else if (modifiers == GDK_MOD1_MASK)
    {
        switch (keyval)
        {
            /* ALT+ */
			case GDK_KEY_A:
			case GDK_KEY_a:
				*key__num = 101; break;
			case GDK_KEY_B:
			case GDK_KEY_b:
				*key__num = 102; break;
			case GDK_KEY_C:
			case GDK_KEY_c:
				*key__num = 103; break;
			case GDK_KEY_D:
			case GDK_KEY_d:
				*key__num = 104; break;
			case GDK_KEY_E:
			case GDK_KEY_e:
				*key__num = 105; break;
			case GDK_KEY_F:
			case GDK_KEY_f:
				*key__num = 106; break;
			case GDK_KEY_G:
			case GDK_KEY_g:
				*key__num = 107; break;
			case GDK_KEY_H:
			case GDK_KEY_h:
				*key__num = 108; break;
			case GDK_KEY_I:
			case GDK_KEY_i:
				*key__num = 109; break;
			case GDK_KEY_J:
			case GDK_KEY_j:
				*key__num = 110; break;
			case GDK_KEY_K:
			case GDK_KEY_k:
				*key__num = 111; break;
			case GDK_KEY_L:
			case GDK_KEY_l:
				*key__num = 112; break;
			case GDK_KEY_M:
			case GDK_KEY_m:
				*key__num = 113; break;
			case GDK_KEY_N:
			case GDK_KEY_n:
				*key__num = 114; break;
			case GDK_KEY_O:
			case GDK_KEY_o:
				*key__num = 115; break;
			case GDK_KEY_P:
			case GDK_KEY_p:
				*key__num = 116; break;
			case GDK_KEY_Q:
			case GDK_KEY_q:
				*key__num = 117; break;
			case GDK_KEY_R:
			case GDK_KEY_r:
				*key__num = 118; break;
			case GDK_KEY_S:
			case GDK_KEY_s:
				*key__num = 119; break;
			case GDK_KEY_T:
			case GDK_KEY_t:
				*key__num = 120; break;
			case GDK_KEY_U:
			case GDK_KEY_u:
				*key__num = 121; break;
			case GDK_KEY_V:
			case GDK_KEY_v:
				*key__num = 122; break;
			case GDK_KEY_W:
			case GDK_KEY_w:
				*key__num = 123; break;
			case GDK_KEY_X:
			case GDK_KEY_x:
				*key__num = 124; break;
			case GDK_KEY_Y:
			case GDK_KEY_y:
				*key__num = 125; break;
			case GDK_KEY_Z:
			case GDK_KEY_z:
				*key__num = 126; break;
			default :
				*key__num =0; break;
        }
    }
	else
	{
		switch (keyval)
        {
			case GDK_KEY_Return:
				*key__num = 201; break;
			case GDK_KEY_Tab:
				*key__num = 202; break;
			case GDK_KEY_BackSpace:
				*key__num = 204; break;
			case GDK_KEY_Insert:
				*key__num = 205; break;
			case GDK_KEY_Delete:
				*key__num = 206; break;
			case GDK_KEY_Home:
				*key__num = 207; break;
			case GDK_KEY_End:
				*key__num = 208; break;
			case GDK_KEY_Page_Up:
				*key__num = 209; break;
			case GDK_KEY_Page_Down:
				*key__num = 210; break;
			case GDK_KEY_Left:
				*key__num = 211; break;
			case GDK_KEY_Right:
				*key__num = 212; break;
			case GDK_KEY_Up:
				*key__num = 213; break;
			case GDK_KEY_Down:
				*key__num = 214; break;
			case GDK_KEY_Escape:
				*key__num = 215; break;
			case GDK_KEY_KP_Enter:
				*key__num = 216; break;

// VK_F..                
			case GDK_KEY_F1:
				*key__num = 301; break;
			case GDK_KEY_F2:
				*key__num = 302; break;
			case GDK_KEY_F3:
				*key__num = 303; break;
			case GDK_KEY_F4:
				*key__num = 304; break;
			case GDK_KEY_F5:
				*key__num = 305; break;
			case GDK_KEY_F6:
				*key__num = 306; break;
			case GDK_KEY_F7:
				*key__num = 307; break;
			case GDK_KEY_F8:
				*key__num = 308; break;
			case GDK_KEY_F9:
				*key__num = 309; break;
			case GDK_KEY_F10:
				*key__num = 310; break;
			case GDK_KEY_F11:
				*key__num = 311; break;
			case GDK_KEY_F12:
				*key__num = 312; break;
			case GDK_KEY_F13:
				*key__num = 313; break;
			case GDK_KEY_F14:
				*key__num = 314; break;
			case GDK_KEY_F15:
				*key__num = 315; break;
			case GDK_KEY_F16:
				*key__num = 316; break;
			case GDK_KEY_F17:
				*key__num = 317; break;
			case GDK_KEY_F18:
				*key__num = 318; break;
			case GDK_KEY_F19:
				*key__num = 319; break;
			case GDK_KEY_F20:
				*key__num = 320; break;
			case GDK_KEY_F21:
				*key__num = 321; break;
			case GDK_KEY_F22:
				*key__num = 322; break;
			case GDK_KEY_F23:
				*key__num = 323; break;
			case GDK_KEY_F24:
				*key__num = 324; break;

			default :
				*key__num =0; break;
        }
	}
gtk_widget_activate (terminal);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		traitement terminal GTK. 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void close_window()
{
	close_Keyboard();
	gtk_main_quit ();
}


///-------------------------------------
/// traitement ALT+F4
///-------------------------------------
gboolean key_press_ALTF4()
{
	if (_DEBUG_ == 1)
	{
		GtkWidget *dialog;
		const gchar* _MSG_ =  MESSAGE_ALT_F4;

		dialog = gtk_message_dialog_new(
										 GTK_WINDOW(window),
										 GTK_DIALOG_MODAL,
										 GTK_MESSAGE_QUESTION,
										 GTK_BUTTONS_YES_NO,
										 _MSG_,
										 NULL,
										 NULL);

		int result = gtk_dialog_run (GTK_DIALOG (dialog));

		gtk_widget_destroy(GTK_WIDGET(dialog));

		switch (result)
		{
			case  GTK_RESPONSE_YES:
									{
										char* cmd = (char *)malloc(sizeof(char) * 100);
										sprintf(cmd,"kill -9 %d ",child_pid); exec_prog(cmd);
										free(cmd);
										close_window();
										return EXIT_FAILURE ;
										//break;
									}
			case GTK_RESPONSE_NO:
									// not active ALT_F4
									return GDK_EVENT_STOP;
									//break;
		}

	}
	// not active ALT_F4
	return GDK_EVENT_STOP;
}



/// -----------------------------------------------------------------------------
/// personalisation projet utilisant une terminal simplifier pour de la gestion
/// -----------------------------------------------------------------------------

void	init_Terminal()
{

	VteTerminal *VTE;

	//determines the maximum size for screens
	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	char * font_terminal = new char[30] ;
	/// font screen 17"<  or  17">								Font DejaVu Sans Mono -> xfce4-terminal
	if ( s->width <= 1600 && s->height >=1024 ) sprintf(font_terminal,"%s %s" , VTEFONT,"13");			/// correct for 24/80 possible extend 15"
	else if ( s->width <= 1920 && s->height >=1080 ) sprintf(font_terminal,"%s %s" ,VTEFONT,"15"); 		/// correct
	else if ( s->width > 1920  ) sprintf(font_terminal,"%s %s" ,VTEFONT,"16");							/// confortable and extend numbers columns and rows


	// resize  title  font
    VTE = VTE_TERMINAL (terminal);

	vte_terminal_set_size (VTE, COL, ROW);													/// size du terminal

	gtk_window_set_title(GTK_WINDOW(window), VTENAME);										/// titre du terminal de base

	vte_terminal_set_font (VTE,pango_font_description_from_string(font_terminal));			/// font utilisé
	
	vte_terminal_set_scrollback_lines (VTE,0);		 										///	désactiver historique.

	vte_terminal_set_scroll_on_output(VTE,false);											/// pas de défilement en cas de nouvelle sortie

	vte_terminal_set_scroll_on_keystroke(VTE,false);										/// pas de défilement en bas s’il y a interaction de l’utilisateur
	
    vte_terminal_set_encoding(VTE,NULL,NULL);												/// UTF8


}


/// -----------------------------------------------------------------------------
/// Callback for vte_terminal_spawn_async    retrived PID terminal ONLY
/// -----------------------------------------------------------------------------
void term_spawn_callback(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
		child_pid = pid;
}
/// -----------------------------------------------------------------------------
/// possibility to change the name of the terminal
/// -----------------------------------------------------------------------------


void on_title_changed(GtkWidget *terminal)
{
    gtk_window_set_title(GTK_WINDOW(window), vte_terminal_get_window_title(VTE_TERMINAL(terminal)));
}

/// -----------------------------------------------------------------------------
/// possibility to change the number of columns and rows
/// -----------------------------------------------------------------------------

void on_resize_window(GtkWidget *terminal, guint  _col, guint _row)
{
	  vte_terminal_set_size (VTE_TERMINAL(terminal),_col,_row);
	  gtk_widget_show_all(window);
}

/// -----------------------------------------------------------------------------
///  libvte function putting the terminal function active
/// -----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	std::setlocale(LC_ALL, "");


	/// contrôle autorisation traitement --> protection 
	const gchar *dir = WORKLIB;

	gchar ** command ;
	gchar *arg_1[] = { (gchar*)WORKPGM, NULL};
	gchar *arg_3[] = { (gchar*) argv[1],(gchar*)argv[2], NULL};		/// arg[1] P_pgm , [2] Pid_msgq
	/// exemple P_Pgm (argv[1]) = ./programme   Pid_msgq (argv[2]) = 0000721489 
	
	if (argc == 1 )  command = arg_1;

	if (argc == 3 )  command = arg_3;


	if (argc > 3)  return EXIT_FAILURE;

	
	// Initialise GTK, the window traditional work
	 
   gtk_init(&argc,&argv);
 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //  gtk_window_set_title(GTK_WINDOW(window), "VTERM0");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_resizable (GTK_WINDOW(window),false);
	gtk_window_set_deletable (GTK_WINDOW(window),false);



    /* Initialise the terminal */
    terminal = vte_terminal_new();

    vte_terminal_spawn_async(
		VTE_TERMINAL(terminal), //VteTerminal *terminal
        VTE_PTY_DEFAULT, // VtePtyFlags pty_flags,
        dir,			// const char *working_directory
        command,		// command
        NULL,			// environment
         (GSpawnFlags)(G_SPAWN_SEARCH_PATH |G_SPAWN_FILE_AND_ARGV_ZERO),	// spawn flags
        NULL,			// GSpawnChildSetupFunc child_setup,
        NULL,			// gpointer child_setup_data,
        NULL,			// GDestroyNotify child_setup_data_destroy,
        -1,				// int timeout
        NULL,			// GCancellable *cancellable,

        &term_spawn_callback,
                                  			// VteTerminalSpawnAsyncCallback callback, get pid child

        NULL);			// gpointer user_data


    // Connect some signals
	g_signal_connect(GTK_WINDOW(window),"delete_event", G_CALLBACK(key_press_ALTF4), NULL);

	gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
	g_signal_connect(G_OBJECT(window),"key-press-event", G_CALLBACK(key_press),terminal);
	
	gtk_widget_add_events(window, GDK_SCROLL_MASK);
	g_signal_connect(G_OBJECT(window),"scroll-event",G_CALLBACK (mouse_scroll),terminal); 
	
	g_signal_connect(terminal, "child-exited",  G_CALLBACK (close_window), NULL);
	g_signal_connect(terminal, "destroy",  G_CALLBACK (close_window), NULL);
	g_signal_connect(terminal, "window-title-changed", G_CALLBACK(on_title_changed), NULL);
	g_signal_connect(terminal, "resize-window", G_CALLBACK(on_resize_window),NULL);


	
    // specific initialization of the terminal
	init_Terminal();
	// INIT transmit code keyboard to terminal
	init_Keyboard();


    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);

    gtk_widget_hide(window);			// hide = ignore flash 
	gtk_widget_activate (terminal);		// force activate terminal
	
    gtk_main();
	
    return EXIT_SUCCESS;
}
