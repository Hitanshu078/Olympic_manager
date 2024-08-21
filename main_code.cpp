#include <iostream>
#include<sstream>
using namespace std;

#define MAX_NAME_LEN 100
#define MAX_EVENTS 100
#define MAX_VENUES 100

class Event { // Class representing an Event with date, start time, end time, and name
public:
    int date;
    int start;
    int ends;
    string eventname;
    Event() : date(0), start(0), ends(0), eventname("") {}
    Event(int d, int s, int e, const string& name)
        : date(d), start(s), ends(e), eventname(name) {}
};

class Venue {// Class representing a Venue which can hold multiple events
public:
    string name;
    string location;
    int capacity;
    Event events[MAX_EVENTS]; // Array of events
    int eventcount; // Number of events added to the venue

    Venue() : name(""), location(""), capacity(0), eventcount(0) {}
    Venue(const string& n, const string& loc, int cap)
        : name(n), location(loc), capacity(cap), eventcount(0) {}

    // Function to sort events by date and time without using inbuilt sort
    void sortEvents(Event sortedEvents[], int eventcount) const {
        for (int i = 0; i < eventcount - 1; i++) {
            for (int j = 0; j < eventcount - i - 1; j++) {
                // Sort primarily by date
                if (sortedEvents[j].date > sortedEvents[j + 1].date) {
                    Event temp = sortedEvents[j];
                    sortedEvents[j] = sortedEvents[j + 1];
                    sortedEvents[j + 1] = temp;
                }
                // If dates are the same, sort by start time
                else if (sortedEvents[j].date == sortedEvents[j + 1].date &&
                         sortedEvents[j].start > sortedEvents[j + 1].start) {
                    Event temp = sortedEvents[j];
                    sortedEvents[j] = sortedEvents[j + 1];
                    sortedEvents[j + 1] = temp;
                }
            }
        }
    }
    // Function to add a new event to the venue
    void addEvent(const Event& event) {
        if (eventcount < MAX_EVENTS) { // Check if event count is within the limit
            events[eventcount] = event;
            eventcount++;
        } else {
            cout << "-1\nError\n"; // Error if the maximum number of events is exceeded
        }
    }
    // Function to delete an event by name, date, and start time
    void deleteEvent(const string& eventname, int date, int start) {
        for (int i = 0; i < eventcount; i++) {
            if (events[i].eventname == eventname && events[i].date == date && events[i].start == start) {
                // Shift remaining events up by one to fill the gap
                for (int k = i; k < eventcount - 1; k++) {
                    events[k] = events[k + 1];
                }
                eventcount--; // Decrease the event count
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";
    }
    // Function to show events on a specific date
    void showEvents(int date) const {
        Event sortedEvents[MAX_EVENTS];
        copy(events, events + eventcount, sortedEvents); // Create a copy of the events array
        sortEvents(sortedEvents, eventcount); // Sort the events
        int p = 0;
        for (int i = 0; i < eventcount; i++) {
            if (sortedEvents[i].date == date) {
                p++;
            }
        }

        cout << p << "\n";
        for (int i = 0; i < eventcount; i++) {
            if (sortedEvents[i].date == date) {
                cout << sortedEvents[i].eventname << " " << sortedEvents[i].start << " " << sortedEvents[i].ends << "\n";
            }
        }
    }
    // Function to show all events in the venue in calendar format
    void showCalendar() const {
        Event sortedEvents[MAX_EVENTS];
        copy(events, events + eventcount, sortedEvents); // Create a copy of the events array
        sortEvents(sortedEvents, eventcount); // Sort the events
        cout << eventcount << "\n"; // Print total number of events
        for (int day = 1; day <= 30; ++day) { // Loop through all days of the month
            cout << day << " ";
            int eventCount = 0;
            for (int i = 0; i < eventcount; i++) {
                if (sortedEvents[i].date == day) {
                    eventCount++;
                }
            }
            cout << eventCount << "\n";// Print number of events for each day

            for (int i = 0; i < eventcount; i++) {
                if (sortedEvents[i].date == day) {
                    // Print event details for each day
                    cout << sortedEvents[i].eventname << " " << sortedEvents[i].start << " " << sortedEvents[i].ends << "\n";
                }
            }
        }
    }
};

// Class to manage multiple venues and their events
class VenueManager {
private:
    Venue venues[MAX_VENUES]; // Array of venues
    int venuecount; // Number of venues added

public:
    VenueManager() : venuecount(0) {}

    // Function to add a new venue
    void addVenue(const string& name, const string& location, int capacity) {
        if (venuecount >= MAX_VENUES) { // Check if venue count is within the limit
            cout << "-1\nError\n";
            return;
        }
        for (int i = 0; i < venuecount; i++) {
            if (venues[i].name == name) {
                cout << "-1\nError\n"; // Check if venue with the same name already exists
                return;
            }
        }
        venues[venuecount] = Venue(name, location, capacity); // Add new venue
        venuecount++;
        cout << "0\n";
    }
    // Function to delete a venue by name
    void deleteVenue(const string& name) {
        for (int i = 0; i < venuecount; i++) {
            if (venues[i].name == name) {
                // Shift remaining venues up by one to fill the gap
                for (int j = i; j < venuecount - 1; j++) {
                    venues[j] = venues[j + 1];
                }
                venuecount--; // Decrease the venue count
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";
    }
    // Function to print all venues with their details
    void printVenues() const {
        cout << venuecount << "\n";
        for (int i = 0; i < venuecount; i++) {
            cout << venues[i].name << " " << venues[i].location << " " << venues[i].capacity << "\n";
        }
    }
    // Function to add a new event to a specific venue
    void addEvent(const string& venueName, int date, int start, int end, const string& eventname) {
        if (end <= start || end > 23 || start < 0) { // Check if time is valid
            cout << "-1\nError\n";
            return;
        }
        if (date > 30 || date < 1) { // Check if date is valid
            cout << "-1\nError\n";
            return;
        }

        for (int i = 0; i < venuecount; i++) {
            if (venues[i].name == venueName) {
                // Check for event conflicts
                for (int j = 0; j < venues[i].eventcount; j++) {
                    if (venues[i].events[j].eventname == eventname && venues[i].events[j].date==date) {
                        cout << "-1\nError\n"; // Error if event with the same name exists on the same date
                        return;
                    }
                    if (venues[i].events[j].date == date && (start < venues[i].events[j].ends && end > venues[i].events[j].start)) {
                        cout << "-1\nError\n"; // Error if event time conflicts with an existing event
                        return;
                    }
                }
                venues[i].addEvent(Event(date, start, end, eventname));
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";
    }
     // Function to delete an event from a specific venue
    void deleteEvent(const string& venueName, int date, int start, const string& eventname) {
        for (int i = 0; i < venuecount; i++) {
            if (venues[i].name == venueName) {
                venues[i].deleteEvent(eventname, date, start);
                return;
            }
        }
        cout << "-1\nError\n";
    }
    // Function to show events of a specific venue on a specific date
    void showEvents(const string& venueName, int date) const {
        for (int i = 0; i < venuecount; i++) {
            if (venues[i].name == venueName) {
                venues[i].showEvents(date);// Call showEvents function of Venue class
                return;
            }
        }
        cout << "-1\nError\n";
    }
    // Function to show the entire calendar of a specific venue
    void showCalendar(const string& venueName) const {
        for (int i = 0; i < venuecount; i++) {
            if (venues[i].name == venueName) {
                venues[i].showCalendar();// Call showCalendar function of Venue class
                return;
            }
        }
        cout << "-1\nError\n";
    }
};
// Main function to handle user input and execute commands
int main() {
    VenueManager venue;
    string line;

    while (getline(cin, line)) {
        if (line == "End") { // Stop input if "End" is encountered
            break;
        }

        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "addVenue") { // Command to add a new venue
            string name, location;
            int capacity;
            iss.ignore(); 
            getline(iss, name, '\"');
            getline(iss, name, '\"');

            iss.ignore(); // Ignore leading spaces and quotes
            getline(iss, location, '\"');
            getline(iss, location, '\"');

            iss >> capacity;
            venue.addVenue(name, location, capacity);
        }
        else if (command == "delVenue") { // Command to delete a venue
            string name;
            iss.ignore();
            getline(iss, name, '\"');
            getline(iss, name, '\"');
            venue.deleteVenue(name);
        }
        else if (command == "showVenues") { // Command to show all venues
            venue.printVenues();
        }
        else if (command == "addEvent") { // Command to add an event to a venue
            string venueName, eventName;
            int date, fromHour, toHour;

            iss.ignore();
            getline(iss, venueName, '\"');
            getline(iss, venueName, '\"');

            iss >> date >> fromHour >> toHour;

            iss.ignore();
            getline(iss, eventName, '\"');
            getline(iss, eventName, '\"');

            venue.addEvent(venueName, date, fromHour, toHour, eventName);
        }
        else if (command == "delEvent") { // Command to delete an event from a venue
            string venueName, eventName;
            int date, fromHour;

            iss.ignore();
            getline(iss, venueName, '\"');
            getline(iss, venueName, '\"');

            iss >> date >> fromHour;

            iss.ignore();
            getline(iss, eventName, '\"');
            getline(iss, eventName, '\"');

            venue.deleteEvent(venueName, date, fromHour, eventName);
        }
        else if (command == "showEvents") { // Command to show events of a specific venue on a specific date
            string venueName;
            int date;

            iss.ignore();
            getline(iss, venueName, '\"');
            getline(iss, venueName, '\"');

            iss >> date;
            venue.showEvents(venueName, date);
        }
        else if (command == "showCalendar") { // Command to show the entire calendar of a specific venue
            string venueName;
            iss.ignore(); 
            getline(iss, venueName, '\"');
            getline(iss, venueName, '\"'); 
            venue.showCalendar(venueName);
        }
    }
    return 0;
}
