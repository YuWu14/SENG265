/*
 * calprint4.c
 *
 * Starter file provided to students for Assignment #4, SENG 265,
 * Summer 2019.
 */

#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"

#define MAX_LINE_LEN 80
#define MAX_EVENTS 500

void print_event(node_t *n, void *arg) {
    assert(n != NULL);

    event_t *event = n->val;

    if (strcmp(event->rrule, "") == 0) {
        printf("EVENT: %s %s '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location);
    } else {
        printf("EVENT: %s %s '%.10s' '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location,
            event->rrule);
    }
}


#ifdef DEBUG

/*
 * Just showing the use of the linked-list routines.
 */

void _demo() {
    event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;

    /* Add one event, without an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190706T120000", 17);
    strncpy(temp_event->dtend, "20190706T160000", 17);
    strncpy(temp_event->summary, "summary 1", SUMMARY_LEN);
    strncpy(temp_event->location, "location 1", LOCATION_LEN);
    temp_event->rrule[0] = '\0';
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Add a second event, with an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190707T093000", 17);
    strncpy(temp_event->dtend, "20190707T102000", 17);
    strncpy(temp_event->summary, "uvwxyz 1234567", SUMMARY_LEN);
    strncpy(temp_event->location, "abcde 1234567", LOCATION_LEN);
    strncpy(temp_event->rrule, "yada RULE yada UNTIL yada", RRULE_LEN);
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Print the list of events. */

    //apply(head, print_event, NULL);

    /* Free up the memory. This is done rather deliberately
     * and manually. A full-featured function might better
     * serve for this. Asserts are liberally used here as they
     * express state that *must* be true if all of the code is
     * correctly working.
     */

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);
   
    assert(head == NULL); 
    
}

#endif
/*
 * Function dt_increment:
 *
 * Given a date-time, it adds the number of days in a way that
 * results in the correct year, month, and day. For example,
 * if the string in "before" corresponds to:
 *
 *   20190520T111500
 *
 * then the datetime string stored in "after", assuming that
 * "num_days" is 100, will be:
 *
 *   20190828T111500
 *
 * which is 100 days after May 20, 2019 (i.e., August 28, 2019).
 *
 */

char *dt_increment(char *after, const char *before, int const num_days)
{   
    struct tm temp_time; //*p_temp_time;
    time_t    full_time;
    //char      temp[5];
    char      temp_1[16];
    strncpy(temp_1,before,17);
    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';
/* Apparently there is an 9 required as the second parameter,
 * not an 8...  MZ
    strftime(after, 8, "%Y%m%d", localtime(&full_time));
*/
    strftime(after, 9, "%Y%m%d", localtime(&full_time));
    strncpy(after + 8, temp_1 + 8, MAX_LINE_LEN - 8); 
    after[MAX_LINE_LEN - 1] = '\0';
    return after;
}

/* dt_format provided by timeplay.c written by Zastre*/

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
    	//char      temp[5];
    
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



node_t *create_node(event_t *temp_event){
    node_t  *temp_node  = NULL;
    temp_node = new_node(temp_event);
    return temp_node;
}
/*
node_t *create_linked_list(node_t *head, node_t *new){
    event_t *this_node = new -> val;
    char *node_start;
    node_start = this_node->dtstart;

    int x;
    int y;

    node_t *curr;
    event_t *current ;
    char *curr_start;

    event_t *next ;
    char *next_start;

    curr = head;
    while (curr->next != NULL){
        current = curr->val;
        curr_start = current->dtstart;

        next = (curr->next)->val;
        next_start = next->dtstart;

        x = strncmp(curr_start, node_start, strlen(curr_start));
        y = strncmp(next_start, node_start, strlen(next_start));
        if(x<0 && y>0){
            new->next = curr ->next;
            curr->next = new;
            break;
        }else if(x>0){
            head = add_front(head,new);
            break;
        }
        if(curr->next != NULL){
            curr = curr->next;
        }else{
            head = add_end(head,new);
            break;
        }
    }
    return head;    
}*/
/*
node_t *repeated_period_1(event_t *temp_event_4,node_t *head){
    struct tm temp_time; //*p_temp_time;
    time_t    full_time;
    //char      temp[5];
    char      temp_1[16];
            strncpy(temp_1,temp_event_4->dtend,17);
            memset(&temp_time, 0, sizeof(struct tm));
            sscanf(temp_event_4->dtend, "%4d%2d%2d", &temp_time.tm_year,
                &temp_time.tm_mon, &temp_time.tm_mday);
            temp_time.tm_year -= 1900;
            temp_time.tm_mon -= 1;
            temp_time.tm_mday += 7;

            full_time = mktime(&temp_time);
            temp_event_4->dtend[0] = '\0';

            strftime(temp_event_4->dtend, 9, "%Y%m%d", localtime(&full_time));
            strncpy(temp_event_4->dtend + 8, temp_1 + 8, MAX_LINE_LEN - 8); 
            temp_event_4->dtend[MAX_LINE_LEN - 1] = '\0';
}*/
/*node_t *repeated_period(event_t *temp_event_4){
    struct tm temp_time; //*p_temp_time;
    time_t    full_time;
    //char      temp[5];
    char      temp_1[16];

    event_t *temp_event_5 = NULL;

    node_t *head = create_node(temp_event_4);
    for(;;){
        if(strncmp(temp_event_4->dtstart, temp_event_4->rrule, strlen(temp_event_4->dtstart)) >= 0){
            return head;
            break;
        }else{        
            temp_event_5 = emalloc(sizeof(event_t));  
            strncpy(temp_event_5->dtstart, temp_event_4->dtstart , 17);
            strncpy(temp_event_5->dtend, temp_event_4->dtend, 17);
            strncpy(temp_event_5->location, temp_event_4->location, strlen(temp_event_4->location));
            strncpy(temp_event_5->summary, temp_event_4->summary, strlen(temp_event_4->summary));
            temp_event_5->rrule[0] = '\0';
            
            node_t *current_node = create_node(temp_event_4); 

            head->next = current_node;

            strncpy(temp_1,temp_event_4->dtstart,17);
            memset(&temp_time, 0, sizeof(struct tm));
            sscanf(temp_event_4->dtstart, "%4d%2d%2d", &temp_time.tm_year,
                &temp_time.tm_mon, &temp_time.tm_mday);
            temp_time.tm_year -= 1900;
            temp_time.tm_mon -= 1;
            temp_time.tm_mday += 7;

            full_time = mktime(&temp_time);
            temp_event_4->dtstart[0] = '\0';
            strftime(temp_event_4->dtstart, 9, "%Y%m%d", localtime(&full_time));
            strncpy(temp_event_4->dtstart + 8, temp_1 + 8, MAX_LINE_LEN - 8); 
            temp_event_4->dtstart[MAX_LINE_LEN - 1] = '\0';
        }
    }

}*/
void print_result(node_t *head,int from_y, int from_m , int from_d,
    int to_y , int to_m ,int to_d ){
    node_t *curr;
    char date[1024];
    int hour_1,hour_2;
    int min_1,min_2;
    int  year, month, day;
    int  hour, minute, second;
    char weekday;
    char morning[5] = "am";
    char afternoon[5] = "pm";
    
    if(head->next ==  NULL){
        event_t *event = head->val;
        dt_format(date,event->dtstart,80);
        printf("%s\n",date);
        printf("-------------------\n");

        sscanf(event->dtstart, "%4d%2d%2d%1c%2d%2d%2d",&year, &month, &day,&weekday,&hour_1,
    &min_1, &second);

        printf(" %d:%d %s ",hour_1-12,min_1,afternoon);

        sscanf(event->dtend, "%4d%2d%2d%1c%2d%2d%2d",&year, &month, &day,&weekday,&hour_2,
    &min_2, &second);
        printf("to %d:0%d %s:",hour_2-12,min_2,afternoon);

        printf(" %s ",event->summary);
        printf("[%s]\n",event->location);
    }else{
        for (curr = head; curr->next != NULL; curr = curr->next){
            event_t *event = curr->val;
            dt_format(date,event->dtstart,80);
            printf("EVENT: %s %s '%s' '%s'\n", date,event->dtend,
            event->summary, event->location);
            
        }
    }
}

node_t *read_file(char *filename,node_t *head){
	char line[MAX_LINE_LEN];
	char *DTSTART;
    char *DTEND;
	char *UNTIL;
	char *LOCATION;
	char *SUMMARY;
    int m;
    event_t *temp_event_1 = NULL;
	FILE* infile = fopen(filename,"r");
 	while(fgets(line, sizeof(char) * 80, infile)){
        line[strlen(line) - 1] = '\0';  /* eliminate trailing \n */
        if(strncmp(line, "BEGIN:VEVENT", 12)==0){
            temp_event_1 = NULL;
		    temp_event_1 = emalloc(sizeof(event_t));
            m = 0;
        }
		if(strncmp(line, "DTSTART:", 8)== 0){
			DTSTART = strtok(line,":");
			DTSTART = strtok(NULL, ":");
            strncpy(temp_event_1->dtstart, DTSTART, 17);   
		}
		if(strncmp(line, "DTEND:", 6)== 0){
			DTEND = strtok(line,":");
			DTEND = strtok(NULL, ":");
            strncpy(temp_event_1->dtend, DTEND, 17);
		}
		if(strncmp(line, "RRULE:", 6)== 0){
            UNTIL = strtok(line,";");
            UNTIL = strtok(NULL, ";");
            UNTIL = strtok(NULL, ";");
            UNTIL = strtok(UNTIL, "=");
            UNTIL = strtok(NULL, "=");
            strncpy(temp_event_1->rrule, UNTIL, strlen(UNTIL));
            m = 1;
		}
		if(strncmp(line, "LOCATION:", 9)== 0){
			LOCATION = strtok(line,":");
			LOCATION = strtok(NULL, ":");
            strncpy(temp_event_1->location, LOCATION, strlen(LOCATION));
		}
		if(strncmp(line, "SUMMARY:", 8)== 0){
			SUMMARY = strtok(line,":");
			SUMMARY = strtok(NULL, ":");
            strncpy(temp_event_1->summary, SUMMARY, strlen(SUMMARY));
		}
        if(strncmp(line, "END:VEVENT", 10)== 0){
            if(m !=1){
                temp_event_1->rrule[0] = '\0';
                node_t *current_node = create_node(temp_event_1);  
                head = add_front(head,current_node);   
                //head = create_linked_list(head,current_node); 
            }else {
                //head = repeated_period(temp_event_1);
            }
        }
	}
    return head;
}
int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i; 

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 7) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_d, &from_m, &from_y);
        } else if (strncmp(argv[i], "--end=", 5) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_d, &to_m, &to_y);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s --start=dd/mm/yyyy --end=dd/mm/yyyy --file=icsfile\n",
            argv[0]);
        exit(1);
    }
/* 
 * Showing some simple usage of the linked-list routines.
 */
    node_t  *head = NULL;
	head = read_file(filename,head);
    print_result(head,from_y,from_m,from_d,to_y,to_m,to_d);
    /*node_t *curr;
    for (curr = head; curr->next != NULL; curr = curr->next){
        event_t *event = curr->val;
        printf("EVENT: %s %s '%s' '%s'\n", event->dtstart,
            event->dtend, event->summary, event->location);
    }*/

#ifdef DEBUG
    _demo();
#endif

    exit(0);
}
