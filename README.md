# VteTERM
Terminal basic pour application semi-graphic type 5250<br>
Jean-Pierre Laroche<br>
projet 2018-08-08  (C) 2018   Copyright 2018  <laroche.jeanpierre@gmail.com><br>
http://www.ombrebleu.com/wxsrc/src/

but : 	terminal rapide	/ flexible / respectant le code escape<br>
        fast / flexible terminal / respecting the escape code<br>
        -no-pie EXÉCUTABLE<br>
        contrôle ALT_F4 close urgence for DEBUG <br>
	management keyboard fontion type F1....KPENTER...ESC CRTL... etc 

outil pour développer une application de type 5250 / 3270 ou terminal semi-graphic<br>
tool to develop a 5250/3270 or terminal semi-graphic application<br> 
<br>
THANK YOU   MERCI BEAUCOUP<br>
thanks Mr. CHRISTOPHE BLAESS for the book development system LINUX 1..4 edition en Français " ouf ;) " <br>
<br>
GTK platform and GNOME for VTE-TERMINAL<br>
<br>
https://github.com/SgtWiggles/ume   thank you that inspired me to set up a dedicated terminal<br>
<br>
https://vincent.bernat.im/fr/blog/2017-ecrire-emulateur-terminal<br>
<br>
tank you github for exemple open-source for etude<br>


-------------------------------------------------------
possibility of parameter change by the child process<br>

possibility resize columns and rows<br>
possibility to change the name of the terminal<br>
procédure de gestion de message SHM<br>
<br>
<br>
set_screen(43,132,"VTE-5250",F_WHITE,B_BLUE);<br>
<br>
function mouse scroll   GDK_SCROLL_UP->GDK_KEY_Up <br>
function keyboard 	F1 ...F24... Enter KP_enter etc....<br>
<br>
exemple:<br>
	home/soleil/VTETERM/bin/vteterm <br>
        { programme ......<br>
        execv_VTETERM("/home/soleil/VTETERM/bin/","vteterm" ,"term2");<br>
        } deuxiemme niveau etc.... <br>
