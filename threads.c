#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>
#include <pthread.h>


void *print_file (void * arg)
{
    char buffer;
    fread(&buffer, 1, 1, arg);
    while (!feof(arg))
    {
        printw("%c", buffer);
        usleep(105000);
        fread(&buffer, 1, 1, arg);
        refresh();
    }
    refresh();
    pthread_exit(NULL);
}

int main()
{
    WINDOW * mainwin;

    if ( (mainwin = initscr()) == NULL ) {
	//fprintf(stderr, "Error initialising ncurses.\n");
	printw("Error initializing ncurses. \n");
	exit(EXIT_FAILURE);
    }

    char *filename = "./textOfSomeBook.txt";
    FILE *file = fopen(filename,"r");
    if(file ==NULL){
        printw("Can't open file. \n");
        exit(EXIT_FAILURE);
    }
    printw("Vvedi \"C\" dlya starta/prodolzhenya; \"S\" dlya pauzi; \"Q\" dlya vihoda \n");
    pthread_t print;//todo

    bool isThreadWorks = false;
    int answ;
    do{
        answ = getch();
        if ((answ=='c' || answ=='C')&& !isThreadWorks )
        {
            pthread_create(&print, NULL, print_file, file);
            isThreadWorks = true;
        }
        if (answ=='s' || answ=='S')
        {
            pthread_cancel(print);
            isThreadWorks = false;
        }
        if (answ=='q' || answ=='Q')
        {
            pthread_cancel(print);
            break;
        }
    }while(1);


    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
