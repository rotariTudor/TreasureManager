gcc -Wall -o test treasure_hub.c treasure_manager.c pentru a compila, și ./test pentru a rula

gcc -Wall -o test1 treasure_managerMAIN.c pentru a putea adăuga hunt-uri și treasure-uri

pentru score_calculator trebuie să-l compilăm specific cu > gcc -Wall -o score_calculator score_calculator.c
pentru că programul execută un executabil score_calculator

în cod am menționat că usleep() îmi dă warning indiferent de ce nu aș face și am folosit sleep - (probabil că scriu în WSL deși tot nu înțeleg de ce n-ar merge)

am verificat funcționalitatea codului și rulează how it is supposed to. 

