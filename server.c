#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"server.h"

void Init_simulation(int *total_time, double *expected_takeoff, double *expected_landing)
{
    printf("\nHow many units of time will the simulation run? ");
    scanf("%d", total_time);

	// Checking for time unit entered by user
    while(1)
    {
        if(*total_time <= 0)
        {
            printf("\n\tENTER POSITIVE INTEGER VALUE FOR TIME !!\n\n");
            printf("\nHow many units of time will the simulation run? ");
            scanf("%d", total_time);
        }
        else
            break;
    }

    printf("Expected number of arrivals per unit time: ");
    scanf("%lf", expected_landing);
    printf("Expected number of departures per unit time: ");
    scanf("%lf", expected_takeoff);

    // Checking the probability values entered by the user
    while(1)
    {

        if((*expected_landing + *expected_takeoff > 1) || ( (*expected_landing<0) || (*expected_takeoff<0) ) )
        {
            printf("\n !! ENTER VALID EXPECTATIONS !!\n");
            printf("\t -sum of probabilities cannot exceed 1\n");
            printf("\t -probabilities cannot be negative\n\n");

            printf("Expected number of arrivals per unit time: ");
            scanf("%lf", expected_landing);
            printf("Expected number of departures per unit time: ");
            scanf("%lf", expected_takeoff);
        }
        else
            break;
    }

}



int Random(double expect)
{
    // Generates POISSON RANDOM NUMBERS based on expectation value and returns it.
    int n=0;
    float x = rand()/(double)RAND_MAX;
    while(x > exp(-(expect*2)))
    {
        n++;
        x *= rand()/(double)RAND_MAX;
    }
    return n;

}



void new_plane(struct queue **front, struct queue **rear, int x, int t)
{
    /* function parameters:
		  	1) pointer to front of queue
			2) pointer to rear end of queue
			3) int x : plane number
			4) int t: plane generation time 
		Adds the generated plane to the queue 
		(Enqueue happens from rear end) */

    struct queue *temp = (struct queue *)malloc(sizeof(struct queue));
    temp->data = x;
    temp->plane_time = t;
    temp->next = NULL;

    // Check if Queue is empty
    if(*front == NULL && *rear == NULL)
    {
        *front = temp;
        *rear = temp;
    }
    else
    {
        (*rear)->next = temp;
        *rear = temp;
    }
}



void del_plane(struct queue **front, struct queue **rear, char type[], char runway[], int *total_lwait, int *total_twait, int time_unit)
{
    /* function parameters:
		  	1) pointer to front of queue
			2) pointer to rear end of queue
			3) char type[] : "landed" or "took off"
			4) char runway: "landing" or "takeoff"
			5) waiting time for takeoff planes
			6) waiting time for landing planes
			7) Current time unit 

		Deletes the plane from the queue
		(Dequeue happens from rear end) */


    struct queue *temp = (struct queue *)malloc(sizeof(struct queue));
    if(*front == *rear)
    {
        temp = *front;
        display_del_plane(temp, type, runway, total_lwait, total_twait, time_unit);
        *front = NULL;
        *rear = NULL;
        free(temp);

    }
    else if( *front != *rear)
    {
        temp = *front;
        display_del_plane(temp, type, runway, total_lwait, total_twait, time_unit);
        *front = (*front)->next;
        free(temp);

    }

}



void display_del_plane(struct queue *temp, char type[], char runway[], int *total_lwait, int *total_twait, int time_unit)
{
	/* function parameters:
		  	1) pointer to front of queue
			2) pointer to rear end of queue
			3) char type[] : "landed" or "took off"
			4) char runway: "landing" or "takeoff"
			5) waiting time for takeoff planes
			6) waiting time for landing planes
			7) Current time unit */

		//Waiting time = (Current time) - (plane time)
		//prints appropriate meassage based on type and runway 

 		if( strcmp(type, "landed")==0 && strcmp(runway,"landing")==0)
        {
            printf("\tplane %d landed using landing runway\n", temp->data);
            *total_lwait += (time_unit - temp->plane_time);
        }
        else if( strcmp(type, "landed")==0 && strcmp(runway,"takeoff")==0)
        {
            printf("\tplane %d landed using takeoff runway\n", temp->data);
            *total_lwait += (time_unit - temp->plane_time);
        }

        else if( strcmp(type, "took off")==0 && strcmp(runway,"landing")==0)
        {
            printf("\tplane %d took off using landing runway\n", temp->data);
            *total_twait += (time_unit - temp->plane_time);
        }

        else if( strcmp(type, "took off")==0 && strcmp(runway,"takeoff")==0)
        {
            printf("\tplane %d took off using takeoff runway\n", temp->data);
            *total_twait += (time_unit - temp->plane_time);
        }
}



int Count(struct queue **front)
{
    //Returns the number of items in the queue
    int count = 0;
    struct queue *temp = (struct queue *)malloc(sizeof(struct queue));
    temp = *front;
    while(temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;

}



void display_queue(struct queue **lfront, struct queue **tfront)
{
    /* Displays landing queue: [ , , , ...] 
 			and takeoff queue: [ , , , ...]
       		at any unit of time */

    struct queue *l_temp = (struct queue *)malloc(sizeof(struct queue));
    struct queue *t_temp = (struct queue *)malloc(sizeof(struct queue));
    l_temp = *lfront;
    t_temp = *tfront;

    printf(" \tLanding queue: ");
    printf("[");
    while(l_temp != NULL)
    {
        printf(" %d ", l_temp->data);
        l_temp = l_temp->next;
    }
    printf("]\n");

    printf(" \tTakeoff queue: ");
    printf("[");
    while(t_temp != NULL)
    {
        printf(" %d ", t_temp->data);
        t_temp = t_temp->next;
    }
    printf("]\n");


}



void display_results(struct queue **lfront, struct queue **tfront, int *T_time, int *L_planes, int *T_planes, int *I_time, int *wait, int* total_lwait, int* total_twait)
{
    printf("\nThe simulation concluded after %d units. \n", *T_time);
    printf("\t Total number of planes processed:      %3d\n", *L_planes + *T_planes);
    printf("\t Number of planes landed:               %3d\n ", *L_planes);
    printf("\t Number of planes taken off:            %3d\n ", *T_planes);
    printf("\t Number of planes ready to land:        %3d\n ", Count(lfront));
    printf("\t Number of planes ready to take off:    %3d\n ", Count(tfront));
    printf("\t Number of planes told to wait:         %3d\n ", *wait);
    printf("\t Percentage of time runway idle:        %.2f\n ",((double)*I_time/(double)*T_time)*100);
    printf("\t Average wait time to land:             %.2f\n ",(double)*total_lwait/(double)*L_planes);
    printf("\t Average wait time to take off:         %.2f\n ",(double)*total_twait/(double)*T_planes) ;


}


