#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int t=1;

struct STOIXEIA //Ball characteristics
{
    double x;
    double y;
    int c;
    int m;
    int r;
    double spx;
    double spy;
};

int validation ( double d, int i, int high, int low, int c )
{
    if (d>high || d<low)
    {
        printf("No.%d ball's no.%d parameter is out of range.\n",i+1,t);  //Where the error is
        t++;
        return c+1;
    }
    else
    {
        t++;
        return c;
    }
}

void collision ( struct STOIXEIA* d, int i, int j )
{
    double temp1, temp2;

    temp1=d[i].spy;
    temp2=d[i].spx;

    d[i].spy = ((2 * d[j].m * d[j].spy) + (d[i].m - d[j].m) * d[i].spy) / (d[i].m + d[j].m);  //1st ball's y-speed change
    d[i].spx = ((2 * d[j].m * d[j].spx) + (d[i].m - d[j].m) * d[i].spx) / (d[i].m + d[j].m);  //1st ball's x-speed change

    d[j].spy = ((2 * d[i].m * temp1) + (d[j].m - d[i].m) * d[j].spy) / (d[i].m + d[j].m);  //2nd ball's y-speed change
    d[j].spx = ((2 * d[i].m * temp2) + (d[j].m - d[i].m) * d[j].spx) / (d[i].m + d[j].m);  //2nd ball's x-speed change

    return;
}

int main()
{
    clock_t start,finish, previous;
    FILE *fp;
    int N,i,j,c=0;
    double step;
    struct STOIXEIA *d;

    if((fp=fopen("input.txt","r"))!=NULL)  //Opening the information file
    {
        fscanf(fp,"%d",&N);
        d = (struct STOIXEIA *) malloc(sizeof(struct STOIXEIA)*N);  //Dynamic memory allocation
        if(d==NULL)
        {
            printf("Error 404: Could not allocate memory");  //Error for allocation problems
            exit(0);
        }
        else
        {
            for(i=0;i<N;i++)   //Information input of each ball and validation check
            {
                fscanf(fp,"%lf",&d[i].x);
                c = validation(d[i].x, i, 600, 0, c);
                fscanf(fp,"%lf",&d[i].y);
                c = validation(d[i].y, i, 400, 0, c);
                fscanf(fp,"%d",&d[i].c);
                c = validation(d[i].c, i, 15, 0, c);
                fscanf(fp,"%d",&d[i].m);
                c = validation(d[i].m, i, 10000, 0, c);
                fscanf(fp,"%d",&d[i].r);
                c = validation(d[i].r, i, 100, 0, c);
                fscanf(fp,"%lf",&d[i].spx);
                c = validation(d[i].spx, i, 100, -100, c);
                fscanf(fp,"%lf",&d[i].spy);
                c = validation(d[i].spy, i, 100, -100, c);
                t=1;
            }
            if (c)
            {
                printf("Total: %d input errors",c); //Total errors message
                exit(0);
            }
        }
    }
    else
    {
        printf("Error 404: Could not open file");   //Error for file opening problems
    }

    /* request autodetection */
    int gdriver = DETECT, gmode, errorcode;
    /* initialize graphics and local variables */
    initgraph(&gdriver, &gmode, "");
    /* read result of initialization */
    errorcode = graphresult();
    if (errorcode != grOk) {  /* an error occurred */
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      system ("pause");
      exit(1);               /* terminate with an error code */
    }

    start=clock();
    previous=start;
    do
    {
        finish = clock();
        step = (finish-previous)*1.0/CLOCKS_PER_SEC;

        if (step >= 0.03)
        {
            previous = finish;

            for(i=0;i<N;i++)
            {
                setfillstyle(SOLID_FILL,BLACK);
                setcolor(BLACK);
                fillellipse(d[i].x,d[i].y,d[i].r,d[i].r);  //Ball's background black color

                d[i].x+= d[i].spx*step;
                d[i].y+= d[i].spy*step;

                if (d[i].x+d[i].r>=getmaxx() || d[i].x-d[i].r<=0) //Ball's collision with x axis' borders
                    d[i].spx *= -1;

                if (d[i].y+d[i].r>=getmaxy() || d[i].y-d[i].r<=0) //Ball's collision with y axis' borders
                    d[i].spy *= -1;

                for (j=N-1; j>i; j--)
                {
                    if (sqrt(pow(d[i].x - d[j].x,2) + pow(d[i].y - d[j].y,2)) <= (d[i].r + d[j].r))  //If 2 balls hit
                        collision (d, i, j);   //collision function
                }

                setfillstyle(SOLID_FILL,d[i].c);
                setcolor(d[i].c);
                fillellipse(d[i].x,d[i].y,d[i].r,d[i].r); //Ball's inside color
            }
        }
    }
    while (!kbhit());
    closegraph();
    free(d); //Setting the allocated memory free
    return 0;
}
