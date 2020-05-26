/*
 * calprint.c
 *
 * Starter file provided to students for Assignment #1, SENG 265,
 * Summer 2019.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 80
#define MAX_EVENTS 500


/*
 * A struct type you are welcome to use, but are not
 * required to use.
 */

typedef struct event_t
{
    char dt_start[MAX_LINE_LEN];
    char dt_end[MAX_LINE_LEN];
    char summary[MAX_LINE_LEN];
    char location[MAX_LINE_LEN];
    char dt_repeat_until[MAX_LINE_LEN];
} event_t;


/*
 * Some global variables (as few as possible). Again, you 
 * are not required to use these -- but ensure that what is
 * **not** used is deleted.
 */

event_t entry[MAX_EVENTS];
int     num_entries = 0;
/*
 * Function dt_format.
 *
 * Given a date-time, creates a more readable version of the
 * calendar date by using some C-library routines. For example,
 * if the string in "dt_time" corresponds to:
 *
 *   20190520T111500
 *
 * then the string stored at "formatted_time" is:
 *
 *   May 20, 2019 (Mon).
 *
 */

void dt_format(char *formatted_time, const char *dt_time, const int len)
{
    struct tm temp_time;
    time_t    full_time;
    char      temp[5];

    /*  
     * Ignore for now everything other than the year, month and date.
     * For conversion to work, months must be numbered from 0, and the 
     * year from 1900.
     */  
    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
        &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)", 
        localtime(&full_time));
}


int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    FILE *infile;
    int i; 

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--from=", 7) == 0) {
            sscanf(argv[i], "--from=%d/%d/%d", &from_d, &from_m, &from_y);
        } else if (strncmp(argv[i], "--to=", 5) == 0) {
            sscanf(argv[i], "--to=%d/%d/%d", &to_d, &to_m, &to_y);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s --from=dd/mm/yyyy --to=dd/mm/yyyy --file=icsfile\n",
            argv[0]);
        exit(1);
    }

    /* Starting calling your own code from this point. */
    /* ... 
       ...
       ... */

    infile = fopen(filename, "r");


    char line[MAX_LINE_LEN];


    int  year, month, day;
    int  hour, minute, second;
    char weekday;
    char date[1024];
    int count_1,count_2,lineNumber;

    struct event_t event[1024];
    int n =0;

    while(fgets(line, sizeof(char) * 80, infile)){
        line[strlen(line) - 1] = '\0';  /* eliminate trailing \n */
	lineNumber = lineNumber+1;
        if(sscanf(line, "DTSTART:%4d%2d%2d%1c%2d%2d%2d",&year, &month, &day,&weekday,&hour, 
	 	&minute,&second)){
		if(((from_m*100+from_d)<=(month*100+day))&&((to_m*100+to_d)>=(month*100+day))){
		    sscanf(line, "DTSTART:%s", date);
        	    strcpy(event[n].dt_start,date);
        	    strcpy(date,"");
		}	    

	}	
	if(sscanf(line, "DTEND:%4d%2d%2d%1c%2d%2d%2d",
            	&year, &month, &day,
	    	&weekday,
            	&hour, &minute, &second)){		

		if(((from_m*100+from_d)<=(month*100+day))&&((to_m*100+to_d)>=(month*100+day))){
		   // printf("%d\n\n",lineNumber);
        	    //printf("Input line: %s\n", line);
        	   // printf("Hour, year, day,weekday: %d, %d, %d,%c\n", hour, year, day,weekday);
		    sscanf(line, "DTEND:%s", date);
        	    strcpy(event[n].dt_end,date);
        	    strcpy(date,"");
			    
		    count_1 = lineNumber+1;
		    count_2 = lineNumber+2;
		}
	}
	if(count_1 == lineNumber){
		//printf("Input line: %s\n", line);
        	strcpy(event[n].location,line);
        	strcpy(date,"");
		count_1 = 0;
	}
	
	if(count_2 == lineNumber){
		//printf("Input line: %s\n\n", line);
        	strcpy(event[n].summary,line);
        	strcpy(date,"");
		count_1 = 0;
		n = n+1;
	}
	//RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=20190801T235959;BYDAY=TH
        /*if(sscanf(line, "RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=%4d%2d%2d%1c%2d%2d%2d;BYDAY=TH",
            	&year, &month, &day,
	    	&weekday,
            	&hour, &minute, &second)){
		
		if(((from_m*100+from_d)<=(month*100+day))&&((to_m*100+to_d)>=(month*100+day))){
		    // printf("%d\n\n",lineNumber);
        	    printf("Input line: %s\n", line);
        	    printf("Hour, year, day,weekday: %d, %d, %d,%c\n", hour, year, day,weekday);
		}*/
		
	
	
    }
    int hour_1,hour_2;
    int min_1,min_2;
    char location;
    char summary;
    i = n-1;
    n = 0;


    while(i>=n){
    	dt_format(date, event[i].dt_start, 80);


    	printf("%s\n",date);
    	printf("-------------------\n");
    	sscanf(event[i].dt_start, "%4d%2d%2d%1c%2d%2d%2d",&year, &month, &day,&weekday,&hour_1,
	&min_1, &second);

	printf("  %d:%d pm ",hour_1-12,min_1);

    	sscanf(event[i].dt_end, "%4d%2d%2d%1c%2d%2d%2d",&year, &month, &day,&weekday,&hour_2,
	&min_2, &second);

	printf("to %d:0%d pm:",hour_2-12,min_2);

    	sscanf(event[i].summary, "SUMMARY:%[^\n]",&summary);

	printf(" %s",&summary);

    	sscanf(event[i].location, "LOCATION:%[^\n]",&location);

	printf("[%s]\n\n",&location);

	i = i-1;
    }
}


