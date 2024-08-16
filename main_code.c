#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 1001
#define MAX_EVENTS 1001
#define MAX_VENUES 1001
//assigned values which are required frequently

struct event {//this is struct created for managing the events inside the venues
    int date;
    int start;
    int ends;
    char eventname[MAX_NAME_LEN];
};

struct venue {
    char name[MAX_NAME_LEN];
    char location[MAX_NAME_LEN];
    int capacity;
    struct event events[MAX_EVENTS];
    int total_events;
};

struct venue venues[MAX_VENUES];//created array which contain venue structs
int counts = 0;//to count no of venues

void sortEvents(struct event events[], int total_events) {//to sort the events according to the date and time
    for (int i = 0; i < total_events - 1; i++) {
        for (int j = 0; j < total_events - i - 1; j++) {
            if (events[j].date > events[j + 1].date ||
                (events[j].date == events[j + 1].date && events[j].start > events[j + 1].start)) {
                struct event temp = events[j];
                events[j] = events[j + 1];
                events[j + 1] = temp;
            }
        }
    }
}

int addvenue(const char *name, const char *location, int capacity) { 
    // To add a new venue
    if (counts >= MAX_VENUES) {
        printf("-1\n");
        printf("Error: Max number of venues has been added.\n");
        return 0;
    }
    for (int i = 0; i < counts; i++) { 
        // If the name is already present return error
        if (strcmp(venues[i].name, name) == 0) {
            printf("-1\n");
            printf("Error: Duplicate Venue\n");
            return 0;
        }
    }
    strncpy(venues[counts].name, name, MAX_NAME_LEN - 1); // Assigning the struct venue, venue name
    venues[counts].name[MAX_NAME_LEN - 1] = '\0';          // Manually add null terminator

    strncpy(venues[counts].location, location, MAX_NAME_LEN - 1); // Assigning location
    venues[counts].location[MAX_NAME_LEN - 1] = '\0';             // Manually add null terminator

    venues[counts].capacity = capacity; // Assign capacity
    venues[counts].total_events = 0; // Initialize total_events present in that venue
    counts++; // Increment the number of venues
    printf("0\n");
    return 0;
}

int deletevenue(const char *name) {//to delete a venue
    for (int i = 0; i < counts; i++) {//checking if the venue is present or not
        if (strcmp(venues[i].name, name) == 0) {
            for (int j = i; j < counts - 1; j++) {//shifting the structs in the arraay as one struct is deleted
                venues[j] = venues[j + 1];
            }
            counts--;//one venue deleted
            printf("0\n");
            return 0;
        }
    }
    printf("-1\n");
    printf("Error: Venue does not exist\n");
    return 0;
}

int printvenue() {// to print a venue
    if (counts == 0) {
        printf("0\n");
        return 0;
    }
    printf("%d\n", counts);
    for (int i = 0; i < counts; i++) {//printing all the venues
        printf("%s %s %d\n", venues[i].name, venues[i].location, venues[i].capacity);
    }
    return 0;
}

int addevent(const char *name, int date, int start, int end, const char *eventname) { 
    // To add an event in a venue
    if(end<=start || end>23 || start<0){
        printf("-1\n");
        printf("Error: Invalid time entered\n");
        return 0;
    }
    if(date>30 || date<0){
        printf("-1\n");
        printf("Error: Invalid date entered\n");
        return 0;
    }
    for (int i = 0; i < counts; i++) {
        if (strcmp(venues[i].name, name) == 0) { 
            // Searching if the assigned venue is present or not
            if (venues[i].total_events >= MAX_EVENTS) {
                printf("-1\n");
                printf("Error: Events limit reached\n");
                return 0;
            }
            for(int j=0;j<venues[i].total_events;j++){
                if(strcmp(venues[i].events[j].eventname,eventname)==0){
                    printf("-1\n");
                    printf("Error:  Duplicate event\n");
                    return 0;
                }
                if (date == venues[i].events[j].date &&((start < venues[i].events[j].ends && end > venues[i].events[j].start))) {
    			printf("-1\n");
    			printf("Error: Another event is plannned in the given time slot\n");
    			return 0;
		}
	    }
            struct event new_event; // Creating the struct event in the given venue
            new_event.date = date; // Assigning attributes to the event struct
            new_event.start = start;
            new_event.ends = end;
            strncpy(new_event.eventname, eventname, MAX_NAME_LEN - 1); // Ensure space for null terminator
            new_event.eventname[MAX_NAME_LEN - 1] = '\0';              // Manually add null terminator

            venues[i].events[venues[i].total_events] = new_event;
            venues[i].total_events++;
            printf("0\n");
            return 0;
        }
    }
    printf("-1\n");
    printf("Error: No such venue added\n");
    return 0;
}

int deleteevent(const char *name, int date, int start, const char *eventname) {//to delete a event in a venue
    for (int i = 0; i < counts; i++) {
        if (strcmp(venues[i].name, name) == 0) {//searchign for the assigned venue
            for (int j = 0; j < venues[i].total_events; j++) {//accessing all the events present in  that venue
                if (strcmp(venues[i].events[j].eventname, eventname) == 0 &&
                    venues[i].events[j].date == date &&
                    venues[i].events[j].start == start) {//if event name start and end date matches delete that function
                    for (int k = j; k < venues[i].total_events - 1; k++) {//and shift the struct event array by 1
                        venues[i].events[k] = venues[i].events[k + 1];
                    }
                    venues[i].total_events--;//reduce totaal events in that venue
                    printf("0\n");
                    return 0;
                }
            }
            printf("-1\n");
            printf("Error: no matching event(on date %d starting at hour %d)\n",date,start);
            return 0;
        }
    }
    printf("-1\n");
    printf("Error: No such venue exists\n");
    return 0;
}

int showevents(const char *name, int date) {//to show all the events in a venue
    for (int i = 0; i < counts; i++) {
        if (strcmp(venues[i].name, name) == 0) {//if venue is present or not
            sortEvents(venues[i].events, venues[i].total_events);//sort events according to date and time
            int y = 0;
	    int u=0;
	    for (int j = 0; j < venues[i].total_events; j++) {//calculating total number of events on that venue on particular date
                if (venues[i].events[j].date == date) {
                    u++;
		}
	    }
	    if(u==0){
		printf("%d(no event on date %d\n",u,date);
		return 0;
	    }
	    printf("%d\n",u);
            for (int j = 0; j < venues[i].total_events; j++) {//printing all the evnets in the venue on particular date
                if (venues[i].events[j].date == date) {
                    y++;
                    printf("%s %d %d\n", venues[i].events[j].eventname, venues[i].events[j].start, venues[i].events[j].ends);
                }
            }
            if (y == 0) {
                return 0;
            }
            return 0;
        }
    }
    return 0;
}

int showCalendar(const char *name) {//this function will display all the events that are scheduled
    for (int i = 0; i < counts; i++) {
        if (strcmp(venues[i].name, name) == 0){//searching for the venue
            sortEvents(venues[i].events, venues[i].total_events);//sorting  venues according to teh date and time 
            printf("%d\n", venues[i].total_events);
            for (int j = 1; j <= 30; j++) {//dates of the month
                printf("%d ", j);
                int eventCount = 0;
                for (int k = 0; k < venues[i].total_events; k++) {//total events on that date
                    if (j == venues[i].events[k].date) {
                        eventCount++;
                    }
                }
                if(j!=30){
                    printf("%d\n", eventCount);
                }
                else{
                    printf("%d\n", eventCount);
                }

                if (eventCount != 0) {
                    for (int k = 0; k < venues[i].total_events; k++) {//printing all the events on that date 
                        if (j == venues[i].events[k].date) {
                            printf("%s %d %d\n",
                                   venues[i].events[k].eventname,
                                   venues[i].events[k].start,
                                   venues[i].events[k].ends);
                        }
                    }
                }
            }
            return 0;
        }
    }
    printf("-1\n");
    printf("Error");
    return 0;
}

int main() {
    char line[1001];

    while (fgets(line, sizeof(line), stdin)) {  // Reading lines from standard input one by one
        line[strcspn(line, "\n")] = '\0';  // Remove the newline character

        char command[20];
        char args[1001];

        sscanf(line, "%19s %1000[^\n]", command, args);  // Extract the command and the arguments

        if (strcmp(command, "addVenue") == 0) {
    char name[MAX_NAME_LEN + 1], location[MAX_NAME_LEN + 1];
    int capacity;

    // Initialize variables to default values
    name[0] = '\0';
    location[0] = '\0';
    capacity = -1;

    int numScanned = sscanf(args, "\"%1000[^\"]\" \"%1000[^\"]\" %d", name, location, &capacity);

    // Check each condition and print specific error messages
    if (numScanned < 1 || name[0] == '\0') {
        printf("-1\n");
        printf("Error: Venue name is missing or incorrect format.\n");
    } else if (numScanned < 2 || location[0] == '\0') {
        printf("-1\n");
        printf("Error: Location is missing or incorrect format.\n");
    } else if (numScanned < 3 || capacity == -1) {
        printf("-1\n");
        printf("Error: Capacity is missing or incorrect format.\n");
    } else {
        // If all inputs are correct, add the venue
        addvenue(name, location, capacity);
    }

        } else if (strcmp(command, "delVenue") == 0) {
            char name[MAX_NAME_LEN + 1];
            if (sscanf(args, "\"%1000[^\"]\"", name) == 1) {
                deletevenue(name);
            } else {
                printf("-1\n");
                printf("Error: Please enter correct input format for delete venue");
            }
        } else if (strcmp(command, "showVenues") == 0) {
            printvenue();
        } else if (strcmp(command, "addEvent") == 0) {
            char name[MAX_NAME_LEN + 1], eventname[MAX_NAME_LEN + 1];
            int date, start, end;
            if (sscanf(args, "\"%1000[^\"]\" %d %d %d \"%1000[^\"]\"", name, &date, &start, &end, eventname) == 5) {
                addevent(name, date, start, end, eventname);
            } else {
                printf("-1\n");
                printf("Error: Please enter correct input format for add event");
            }
        } else if (strcmp(command, "delEvent") == 0) {
            char name[MAX_NAME_LEN + 1], eventname[MAX_NAME_LEN + 1];
            int date, start;
            if (sscanf(args, "\"%1000[^\"]\" %d %d \"%1000[^\"]\"", name, &date, &start, eventname) == 4) {
                deleteevent(name, date, start, eventname);
            } else {
                printf("-1\n");
                printf("Error: Please enter correct input format for delete event");
            }
        } else if (strcmp(command, "showEvents") == 0) {
            char name[MAX_NAME_LEN + 1];
            int date;
            if (sscanf(args, "\"%1000[^\"]\" %d", name, &date) == 2) {
                showevents(name, date);
            } else {
                printf("-1\n");
                printf("Error: Please enter correct input format for show events");
            }
        } else if (strcmp(command, "showCalendar") == 0) {
            char name[MAX_NAME_LEN + 1];
            if (sscanf(args, "\"%1000[^\"]\"", name) == 1) {
                showCalendar(name);
            } else {
                printf("-1\n");
                printf("Error: Please enter correct input format for showCalendar");
            }
        } else if (strcmp(command, "End") == 0) {
            break;
        }
    }

    return 0;
}
