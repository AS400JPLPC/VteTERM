/// -----------------------------------------------------------------
/// Jean-Pierre Laroche
/// projet 2018-08-08  (C) 2018   Copyright 2018  <laroche.jeanpierre@gmail.com>
/// but : 	terminal rapide	/ flexible / respectant le code escape
///			fast / flexible terminal / respecting the escape code
/// 		console dédié uniquement a une application
///			console dedicated to an application only
///			-no-pie EXÉCUTABLE  

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

/// -----------------------------------------------------------------



///------------------------------------------
/// paramétrage spécifique
/// ex: 
///------------------------------------------

#define WORKLIB		"/home/soleil/VTERM/pgm/"	// dir execution program
#define WORKPGM		"./????"					// program

/// security key as a parameter for the application
#define KEYPGM 		"GEN001K"

#define VTEFONT	"DejaVu Sans Mono"
#define ROW		42
#define COL		132
#define TITLE	"Vte-TERM5250"

bool _DEBUG_  = true; /// ALT_F4 ATVIVE  _DEBUG_ = true 
#define MESSAGE_ALT_F4 "vous devez activer uniquement \n en développemnt \n Confirm destroy Application --> DEBUG"





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
 



GtkWidget *window, *terminal;

GPid child_pid = 0;

///--------------------------------------------
/// traitement for Kill nto excute system(...)
///--------------------------------------------
int exec_prog(const char* commande)
{
	int retour;
	pid_t pid;
	char * P_cmd = (char*) malloc(1024);
	sprintf(P_cmd,"%s",commande);
	char *argv[] = {(char*)"sh",(char*)"-c",P_cmd,NULL};

	if((pid = fork()) <0)
		return EXIT_FAILURE ;

	if(pid == 0)
	{
		execv("/bin/sh",argv);
		exit(127);
	}

	while ( waitpid(pid, &retour,0) <0)
		if (errno!= EINTR)
			return EXIT_FAILURE ;

	return EXIT_SUCCESS;
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
										char* cmd = new char[100];
										sprintf(cmd,"kill -9 %d ",child_pid); exec_prog(cmd);

										gtk_main_quit ();
										return EXIT_FAILURE ;
										break;
									}
			case GTK_RESPONSE_NO:
									// not active ALT_F4
									return GDK_EVENT_STOP;
									break;
		}

	}
	// not active ALT_F4
	return GDK_EVENT_STOP;
}



/// Callback for vte_terminal_spawn_async    retrived PID terminal ONLY
void term_spawn_callback(VteTerminal *vte, GPid pid, GError *error, gpointer user_data)
{
		child_pid = pid;
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
	if ( s->width <= 1600 && s->height >=1024 ) sprintf(font_terminal,"%s %s" , VTEFONT,"13");
	else if ( s->width <= 1920 && s->height >=1080 ) sprintf(font_terminal,"%s %s" ,VTEFONT,"15");
	else if ( s->width >  1920 ) sprintf(font_terminal,"%s %s" ,VTEFONT,"16");

	
	// resize  title  font  
    VTE = VTE_TERMINAL (terminal);
     
	vte_terminal_set_size (VTE, COL, ROW);
  
	gtk_window_set_title(GTK_WINDOW(window), TITLE);

	vte_terminal_set_font (VTE,pango_font_description_from_string(font_terminal));
}






int main(int argc, char *argv[])
{
	std::setlocale(LC_ALL, "");

	// program library to call   
	const gchar *dir = WORKLIB;

	// program and parm
	gchar *command[] = { (gchar*)WORKPGM, (gchar*)KEYPGM, NULL};




	
	
	// Initialise GTK, the window
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "VteTerm");
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
        -1,				//  int timeout
        NULL,			// GCancellable *cancellable,
        
        term_spawn_callback,	// VteTerminalSpawnAsyncCallback callback, get pid child
        
        NULL);			// gpointer user_data
 

    // Connect some signals
	g_signal_connect(GTK_WINDOW(window),"delete_event", G_CALLBACK (key_press_ALTF4), NULL);
	g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);


    // specific initialization of the terminal
	init_Terminal();
	
    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    return EXIT_SUCCESS;
}


