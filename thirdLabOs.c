#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define numOfGuests 20
#define drinkTime 30000
#define timeOfGettingBeer 4000
#define timeOfCheckingDaFridge 2000

int numOfBottles=25;
int guestInfo[numOfGuests][3]; /*состояния поситителей + колличество выпитых ими бутылок + алкомаксимум*/
/*0 -  в очереди, 1 - берет пивас, 2 - пьет, 3 - спит,4 */
int moodOfHost = 0;/*0 - смотрит телевизор, 1 - проверяет холодильник и 2 - наверное кладет пивас, 3 - устал */
int i;

pthread_mutex_t refr;
pthread_t guestsArray[numOfGuests];
pthread_t host;


void *guestFunc(void *arg){
srand(time(NULL));
    int posNumber = *((int *) arg);
    while(1){
        pthread_mutex_lock(&refr);
        guestInfo[posNumber][0] = 1;
        napms(timeOfGettingBeer);
        numOfBottles--;
        if(moodOfHost==0) pthread_mutex_unlock(&refr);
        guestInfo[posNumber][0] = 2;
        napms(drinkTime);
        guestInfo[posNumber][1]++;
        guestInfo[posNumber][0] = 0;

        if(guestInfo[posNumber][1] >= guestInfo[posNumber][2]){
            guestInfo[posNumber][0] = 3; napms(40000);
        }

    }
}

void *hostFunc(){
    pthread_mutex_init(&refr, NULL);
    srand(time(NULL));
    while(1){
        if(rand()%15 == 1){
			moodOfHost=1;
			pthread_mutex_init(&refr, NULL);
			pthread_mutex_lock(&refr);
                if(numOfBottles < 10){
                    moodOfHost = 2;
                    numOfBottles+=10;
                }
			napms(timeOfCheckingDaFridge);
			pthread_mutex_unlock(&refr);
			moodOfHost=0;
		}
		if(rand()%140 == 5){
            moodOfHost = 2;
		}

		napms(1000);
    }
}

void printGuestCondition(int pos){
    switch(guestInfo[pos][0]){
        case 0: printw("  in queue  "); break;
        case 1: printw("  taking a beer "); break;
        case 2: printw("  drinking  "); break;
        case 3: printw("  sleeping  "); break;
    }
}

printHostActivity(){
    switch(moodOfHost){
        case 0: printw("  watching tv  \n"); break;
        case 1: printw("  checking a fridge \n"); break;
        case 2: printw("  adding some bottles \n"); break;
        case 3: printw("  tired  \n"); break;
    }
}


int main()
{
    srand(time(NULL));
    initscr();
	printw("\n");
	pthread_create(&host, NULL, hostFunc, NULL);

	for(i = 0; i < numOfGuests; i++){
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&guestsArray[i], NULL, guestFunc, arg);
        guestInfo[i][0] = 0;
        guestInfo[i][1] = 0;
        guestInfo[i][2] = rand()%8+3;
	}
        printw("  Teh party has started!1 \n");

	while(1){
        printw("  Now host is ");
        printHostActivity();

        if(numOfBottles > 0){
            if(numOfBottles != 1) printw("  There are %d bottles \n",numOfBottles);
            else printf("  There is one bottle \n");
        }else if(numOfBottles <= 0 || moodOfHost == 3){
            printw("  There are no bottles in the fridge\n");
            printw("  The party is over \n");
            refresh();  napms(10000); endwin(); break;
        }


        for(i = 0; i < numOfGuests; i++){
            printw("   %d",i);
            printGuestCondition(i);
            printw("  %d   %d   \n",guestInfo[i][1],guestInfo[i][2]); /* сколько выпито и макс литраж */
        }

        if(moodOfHost == 3){ printw("Host added 3 bottles in the fridge \n");}

		refresh();
		napms(2000);
		clear();



	}

    endwin();
}
