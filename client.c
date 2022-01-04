#include<stdio.h>
#include<stdlib.h>
#include"server.h"
#define MAX 5


int main()
{
    // Declaring initial variables
    int total_time, plane_no=1, landed_planes=0, tookoff_planes=0, idle_time=0, wait=0, total_lwait=0, total_twait=0;
    double expected_takeoff, expected_landing;

    
    // Asks user for te values of simulation time, probabilities of arrivals and departures    
    Init_simulation(&total_time, &expected_takeoff, &expected_landing);

    /* A Queue always has front and rear pointers
       lfront and lrear for landing queue
       tfront and trear for takeoff queue  */

    // Initializing landing planes queue
    struct queue *lfront = NULL;
    struct queue *lrear = NULL;
    int random_landing;

    // Initializing takeoff planes queue
    struct queue *tfront = NULL;
    struct queue *trear = NULL;
    int random_takeoff;


    // Main loop that runs for specified units of time(simulation time entered by user)
    for(int time_unit=1; time_unit<=total_time; time_unit++)
    {
        /*  Random number of planes generated for takeoff queue
	    Planes are added to takeoff queue as and when generated
       	    Told to wait if takeoff queue is full  */

        random_takeoff = Random(expected_takeoff);
        for(int planes = 0; planes < random_takeoff; planes++)
        {
            printf("\tplane %d is ready to takeoff\n", plane_no);
			if((Count(&tfront)) <= MAX)
                new_plane(&tfront, &trear, plane_no,time_unit);
            else
            {
                printf("\tplane %d is told to wait\n",plane_no);
                wait++;
                new_plane(&tfront, &trear, plane_no,time_unit);
            }
            plane_no++;
        }

      	/*  Random number of planes generated for landing queue
	    Planes are added to landing queue as and when geneated  */

        random_landing = Random(expected_landing);
        for(int planes = 0; planes < random_landing; planes++ )
        {
            printf("\tplane %d is ready to land\n", plane_no);
            new_plane(&lfront, &lrear, plane_no,time_unit);
            plane_no++;
        }


        /* Handling the arrived planes in landing and takeoff queues
           Cases handled in [if else if ladder]:
             1) If one of the queues is empty then both runways used for other queue.
             2) If Landing queue is full and another plane arrives takeoffs are stopped
                and both runways used to clear backlog of landing planes.
             3) Other wise takeoffs happen in takeoff runway and landing happens in
                landing runway.
             4) Else print runways are idle. */

        if((Count(&lfront)> MAX))
        {
            printf("\n Time unit %d:\n" , time_unit);
            display_queue(&lfront, &tfront);
            del_plane(&lfront, &lrear, "landed", "landing", &total_lwait, &total_twait, time_unit);
            del_plane(&lfront, &lrear, "landed", "takeoff", &total_lwait, &total_twait, time_unit);
            landed_planes += 2;
        }
        else if((Count(&lfront)<= MAX  && Count(&lfront) > 0) && (Count(&tfront) > 0))
        {
            printf("\n Time unit %d:\n" , time_unit);
            display_queue(&lfront, &tfront);
            del_plane(&lfront, &lrear, "landed", "landing", &total_lwait, &total_twait, time_unit);
            del_plane(&tfront, &trear,"took off", "takeoff", &total_lwait, &total_twait, time_unit);
            landed_planes++;
            tookoff_planes++;
        }
        else if(Count(&lfront)>1 && Count(&tfront)==0)
        {
            printf("\n Time unit %d:\n" , time_unit);
            display_queue(&lfront, &tfront);
            del_plane(&lfront, &lrear, "landed", "landing", &total_lwait, &total_twait, time_unit);
            del_plane(&lfront, &lrear, "landed", "takeoff", &total_lwait, &total_twait, time_unit);
            landed_planes += 2;
        }
        else if(Count(&tfront)>1 && Count(&lfront)==0)
        {
            printf("\n Time unit %d:\n" , time_unit);
            display_queue(&lfront, &tfront);
            del_plane(&tfront, &trear, "took off", "landing", &total_lwait, &total_twait, time_unit);
            del_plane(&tfront, &trear, "took off", "takeoff", &total_lwait, &total_twait, time_unit);
            tookoff_planes += 2;
        }
        else if(Count(&tfront)==1 && Count(&lfront)==0)
        {
            printf("\n Time unit %d:\n" , time_unit);
            display_queue(&lfront, &tfront);
            del_plane(&tfront, &trear, "took off", "landing", &total_lwait, &total_twait, time_unit);
            tookoff_planes++;
        }
        else if(Count(&lfront)==1 && Count(&tfront)==0)
        {
            printf("\n Time unit %d:\n" , time_unit);
            display_queue(&lfront, &tfront);
            del_plane(&lfront, &lrear, "landed", "landing", &total_lwait, &total_twait, time_unit);
            landed_planes++;
        }
        else
        {
            printf("\n Time unit %d:\n" , time_unit);
            printf("\tRunways are idle\n");
            display_queue(&lfront, &tfront);
            idle_time++;
        }

    }

    // Conclusion after total simulations
    display_results(&lfront, &tfront, &total_time, &landed_planes, &tookoff_planes,&idle_time,&wait,&total_lwait,&total_twait);


}





