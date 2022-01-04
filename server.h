/* Implementation of a Queue using a linked list
   data holds the plane number
   plane_time is the plane generation time */
struct queue
{
    int data;
    int plane_time;
    struct queue *next;
};


/* Functions new_plane, del_plane, display_queue, display_results and count
   takes [struct queue **] as parameter because a pointer is being modified
   in those functions. To achieve this a 'pointer to pointer' must be passed  */

int Random(double);
int Count(struct queue **);
void Init_simulation(int *, double *, double *);
void display_queue(struct queue **, struct queue **);
void new_plane(struct queue **, struct queue **, int, int);
void display_del_plane(struct queue *,char [], char [], int *, int *, int);
void del_plane(struct queue **, struct queue **, char [], char [], int *,int *, int);
void display_results(struct queue **, struct queue **,int *, int *, int *, int *,int *, int *,int *);

