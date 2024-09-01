#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <string>
#include <iomanip>
using namespace std;

#define MAX_NAME_LEN 50
#define MAX_EVENTS 50
#define MAX_VENUES 50
#define MAX_CONGREGATIONS 50
bool isDateInPast(const string &date)
{
    // Parse the input date
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    // Check if date parsing was successful
    if (ss.fail())
    {
        std::cerr << "Date parsing failed.\n";
        return false;
    }

    // Get current time
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    // Convert the input date to std::time_t
    std::time_t givenTime = std::mktime(&tm);
    std::time_t currentTime = std::mktime(now);

    // Check if the given date is in the past
    return givenTime < currentTime;
}
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(const std::string &date)
{
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    // Check if parsing failed
    if (ss.fail())
    {
        std::cerr << "Date parsing failed for: " << date << '\n';
        return false;
    }

    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;

    // Validate year, month, and day ranges
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        std::cerr << "Date out of range for: " << date << '\n';
        return false;
    }

    // Validate day of the month for each month
    if (month == 2)
    {
        if (isLeapYear(year))
        {
            if (day > 29)
                return false; // February in leap year
        }
        else
        {
            if (day > 28)
                return false; // February in non-leap year
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return false; // April, June, September, November
    }
    else
    {
        if (day > 31)
            return false; // January, March, May, July, August, October, December
    }

    return true;
}
std::chrono::system_clock::time_point parseDate(const string &dateStr)
{
    istringstream ss(dateStr);
    tm tm = {};
    ss >> get_time(&tm, "%Y-%m-%d");
    return std::chrono::system_clock::from_time_t(mktime(&tm));
}

// Function to format a time_point as a date string in YYYY-MM-DD format
string formatDate(std::chrono::system_clock::time_point tp)
{
    time_t time = std::chrono::system_clock::to_time_t(tp);
    tm *tm = localtime(&time);
    ostringstream oss;
    oss << put_time(tm, "%Y-%m-%d");
    return oss.str();
}
bool isDifferenceLessThan30Minutes(const string &time1, const string &time2)
{
    auto toMinutes = [](const std::string &time) -> int
    {
        int hours = std::atoi(time.substr(0, 2).c_str());
        int minutes = std::atoi(time.substr(3, 2).c_str());
        return hours * 60 + minutes;
    };

    // Convert both times to minutes
    int minutes1 = toMinutes(time1);
    int minutes2 = toMinutes(time2);

    // Check if the difference is less than 30 minutes
    return std::abs(minutes1 - minutes2) < 30;
}

bool isTimeAlignedWith15Minutes(const std::string &timeStr)
{
    std::tm tm = {};
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tm, "%H:%M");

    if (ss.fail())
    {
        return false;
    }

    // Convert to minutes since midnight
    int minutes = tm.tm_hour * 60 + tm.tm_min;

    // Check if divisible by 15
    return (minutes % 15 == 0);
}

// Function to check if startDate is before endDate
bool isStartDateBeforeEndDate(const string &startDate, const string &endDate)
{
    std::tm tmStart = {}, tmEnd = {};
    std::istringstream ssStart(startDate), ssEnd(endDate);
    ssStart >> std::get_time(&tmStart, "%Y-%m-%d");
    ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d");
    return std::mktime(&tmStart) < std::mktime(&tmEnd);
}
class Event
{
public:
    string date;
    string start;
    string ends;
    string eventname;
    Event() : date(""), start(""), ends(""), eventname("") {}
    Event(string d, string s, string e, const string &name)
        : date(d), start(s), ends(e), eventname(name) {}
};
class Congregation
{
public:
    string name;
    string type;
    string startdate;
    string enddate;

    Congregation() : name(""), type(""), startdate(""), enddate("") {}
};
class Venue
{
public:
    string name;
    string location;
    string country;
    string city;
    string state;
    string postalCode;
    int capacity;
    string congregationName;
    Event events[MAX_EVENTS];
    int eventCount;
    bool isReserved;

    // Default constructor
    Venue() : name(""), location(""), country(""), city(""), state(""), postalCode(""),
              capacity(0), eventCount(0), isReserved(false) {}

    // Parameterized constructor
    Venue(const string &n, string &loc, int cap)
        : name(n), location(loc), capacity(cap), eventCount(0), isReserved(false)
    {
        // ParseLocDetails(location);
    }

    void addEvent(string event, string date, string start, string end)
    {
        events[eventCount++] = Event(date, start, end, event);
    }

    void deleteEvent(const string &eventname, string date, string start)
    {
        for (int i = 0; i < eventCount; i++)
        {
            if (events[i].eventname == eventname && events[i].date == date && events[i].start == start)
            {
                for (int k = i; k < eventCount - 1; k++)
                {
                    events[k] = events[k + 1];
                }
                eventCount--;
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";
    }

    void showEvents(string date) const
    {
        int count = 0;
        for (int i = 0; i < eventCount; i++)
        {
            if (events[i].date == date)
            {
                count++;
            }
        }

        cout << count << "\n";
        for (int i = 0; i < eventCount; i++)
        {
            if (events[i].date == date)
            {
                cout << events[i].eventname << " " << events[i].start << " " << events[i].ends << "\n";
            }
        }
    }

    void showCalendar(string start, string end) const
    {
        // Parse the start and end dates
        std::chrono::system_clock::time_point startDate = parseDate(start);
        std::chrono::system_clock::time_point endDate = parseDate(end);

        // Check if startDate is after endDate
        if (startDate > endDate)
        {
            cout << "Error: Start date is after end date.\n";
            return;
        }

        // Print dates from startDate to endDate (inclusive)
        auto currentDate = startDate;
        auto dayDuration = std::chrono::hours(24); // Duration of one day

        while (currentDate <= endDate)
        {
            string currentDateString = formatDate(currentDate);
            // Count the number of events on this date
            int eventCountOnDate = 0;

            for (int i = 0; i < eventCount; i++)
            {
                if (events[i].date == currentDateString)
                {
                    eventCountOnDate++;
                }
            }

            cout << currentDateString << " " << eventCountOnDate << "\n";

            currentDate += dayDuration;
        }
    }
};
vector<Venue> venues(MAX_VENUES);
int venuecount = 0;
int congregationCount = 0;
Congregation congregations[MAX_CONGREGATIONS];
class VenueManager
{ // Number of venues added

public:
    // Function to add a new venue
    void addVenue(const string &name, const string &location, int capacity)
    {
        bool venueExists = false; // Flag to check if the venue exists

        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == name && venues[i].location == location)
            {
                venueExists = true;
                break; // Exit loop early if venue is found
            }
        }
        if (venueExists)
        {
            cout << "-1\nError\n";
            return; // Early exit if venue exists
        }

        Venue newVenue;
        newVenue.name = name;
        newVenue.capacity = capacity;
        newVenue.location = location;

        string addr, city, state, postal, country;
        istringstream ss(location);
        getline(ss, addr, ':');
        getline(ss, city, ':');
        getline(ss, state, ':');
        getline(ss, postal, ':');
        getline(ss, country, ':');

        newVenue.country = country;
        newVenue.city = city;
        newVenue.state = state;
        newVenue.postalCode = postal;

        venues.insert(venues.begin() + venuecount, newVenue);

        venuecount++;
        cout << "0\n";
    }
    // Function to delete a venue by name
    void deleteVenue(const string &name, const string &country)
    {

        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == name && venues[i].country == country && venues[i].isReserved == false)
            {
                int k = i;
                venues.erase(venues.begin() + k);
                venuecount--;
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";
    }
    void showVenues(const string &location)
    {
        if (venues.empty())
        {
            cout << "0\n";
            return;
        }

        // Parse the input location into its components
        string city, state, postalCode, country;
        istringstream iss(location);

        getline(iss, city, ':');
        getline(iss, state, ':');
        getline(iss, postalCode, ':');
        getline(iss, country, ':');

        if (country.empty())
        {
            cout << "-1" << endl
                 << "Error" << endl;
            return;
        }

        // Ensure city is not empty if state is provided
        if (state.empty() && !city.empty())
        {
            cout << "-1" << endl
                 << "Error" << endl;
            return;
        }

        vector<Venue> matchingVenues;
        for (const auto &venue : venues)
        {
            if (!city.empty() && !postalCode.empty() && !state.empty() &&
                !country.empty() && venue.city == city && venue.state == state &&
                venue.postalCode == postalCode && venue.country == country)
            {
                matchingVenues.push_back(venue);
            }
            else if (!postalCode.empty() && !country.empty() &&
                     venue.postalCode == postalCode && venue.country == country &&
                     city.empty() && state.empty())
            {
                matchingVenues.push_back(venue);
            }
            else if (!city.empty() && !state.empty() && !country.empty() &&
                     venue.city == city && venue.state == state &&
                     venue.country == country && postalCode.empty())
            {
                matchingVenues.push_back(venue);
            }
            else if (!country.empty() && venue.country == country && city.empty() &&
                     state.empty() && postalCode.empty())
            {
                matchingVenues.push_back(venue);
            }
        }

        if (matchingVenues.empty())
        {
            cout << "0\n";
        }
        else
        {
            cout << matchingVenues.size() << "\n";
            for (const auto &venue : matchingVenues)
            {
                cout << venue.name << " " << venue.location << " " << venue.capacity
                     << "\n";
            }
        }
    }

    void addEvent(const std::string &Name, const std::string &venueName, const std::string &country, const std::string &date, const std::string &start, const std::string &end, const std::string &eventname)
    {
        bool congregationExists = false;
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == Name)
            {
                congregationExists = true;
                break;
            }
        }
        if (!congregationExists)
        {
            std::cout << "-1\nError\n";
            return;
        }

        if (!isValidDate(date))
        {
            std::cout << "-1\nError\n";
            return;
        }

        if (isDifferenceLessThan30Minutes(start, end))
        {
            std::cout << "-1\nError\n";
            return;
        }

        if (!isTimeAlignedWith15Minutes(start) || !isTimeAlignedWith15Minutes(end))
        {
            std::cout << "-1\nError\n";
            return;
        }

        bool venueFound = false;
        for (int j = 0; j < venues.size(); j++)
        {
            if (venues[j].name == venueName && venues[j].congregationName == Name && venues[j].country == country)
            {
                venueFound = true;
                for (int k = 0; k < venues[j].eventCount; k++)
                {
                    bool startOverlaps = isDifferenceLessThan30Minutes(venues[j].events[k].ends, start) || isDifferenceLessThan30Minutes(end, venues[j].events[k].start);

                    int q = std::stoi(venues[j].events[k].date.substr(8, 2));
                    int l = std::stoi(date.substr(8, 2));

                    if (q == l)
                    {
                        if (startOverlaps)
                        {
                            std::cout << "-1\nError\n";
                            return;
                        }
                    }
                    else if (std::abs(q - l) == 1)
                    {
                        int m1 = std::stoi(venues[j].events[k].ends.substr(3, 2));
                        int h1 = std::stoi(venues[j].events[k].ends.substr(0, 2));
                        int m2 = std::stoi(start.substr(3, 2));
                        int h2 = std::stoi(start.substr(0, 2));

                        int endMinutes = (h1 * 60) + m1;
                        int startMinutes = (h2 * 60) + m2;
                        int timeDifference = ((24 * 60 - endMinutes) + startMinutes);

                        if (timeDifference < 30)
                        {
                            std::cout << "-1\nError\n";
                            return;
                        }
                    }

                    if (venues[j].events[k].eventname == eventname)
                    {
                        std::cout << "-1\nError\n";
                        return;
                    }
                }
                venues[j].addEvent(eventname, date, start, end);
                std::cout << "0\n";
                return;
            }
        }

        std::cout << "-1\nError\n";
    }
    // Function to delete an event from a specific venue
    void deleteEvent(const string &Name, const string &venueName, string country, string date, string fromtime, const string &eventname)
    {
        for (int l = 0; l < congregationCount; l++)
        {
            if (congregations[l].name == Name)
            {
                for (int j = 0; j < venues.size(); j++)
                {
                    if (venues[j].name == venueName && venues[j].congregationName == Name && venues[j].country == country)
                    {
                        for (int i = 0; i < venues[j].eventCount; i++)
                        {
                            if (venues[j].events[i].eventname == eventname && venues[j].events[i].date == date && venues[j].events[i].start == fromtime)
                            {
                                for (int k = i; k < venues[j].eventCount - 1; k++)
                                {
                                    venues[j].events[k] = venues[j].events[k + 1];
                                }
                                venues[j].eventCount--;
                                cout << "0\n";
                                return;
                            }
                        }
                        cout << "-1\nError\n";
                    }
                }
                cout << "-1\nError\n";
            }
        }
        cout << "-1\nError\n";
    }
    // Function to show events of a specific venue on a specific date
    void showEvents(const string &venueName, const string &country, string date) const
    {
        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == venueName && venues[i].country == country)
            {
                venues[i].showEvents(date); // Call showEvents function of Venue class
                return;
            }
        }
        cout << "-1\nError\n";
    }
    // Function to show the entire calendar of a specific venue
    void showCalendar(const string &name, const string &venueName, string country) const
    {
        string start, end;
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == name)
            {
                start = congregations[i].startdate;
                end = congregations[i].enddate;
            }
        }

        int count = 0;
        int venueIndex = -1; // To store the index of the matching venue

        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == venueName && venues[i].congregationName == name && venues[i].country == country)
            {
                count += venues[i].eventCount;
                venueIndex = i; // Store the index of the matching venue
            }
        }

        if (count > 0)
        {
            cout << count << endl;
            std::chrono::system_clock::time_point startDate = parseDate(start);
            std::chrono::system_clock::time_point endDate = parseDate(end);

            // Check if startDate is after endDate
            if (startDate > endDate)
            {
                cout << "Error: Start date is after end date.\n";
                return;
            }

            // Print dates from startDate to endDate (inclusive)
            auto currentDate = startDate;
            auto dayDuration = std::chrono::hours(24); // Duration of one day
            while (currentDate <= endDate)
            {
                string currentDateString = formatDate(currentDate);
                // Count the number of events on this date
                int eventCountOnDate = 0;
                for (int i = 0; i < venues.size(); i++)
                {
                    if (venues[i].name == venueName && venues[i].country == country && venues[i].congregationName == name)
                    {
                        for (int j = 0; j < venues[i].eventCount; j++)
                        {
                            if (venues[i].events[j].date == currentDateString)
                            {
                                eventCountOnDate++;
                            }
                        }
                    }
                }
                cout << currentDateString << " " << eventCountOnDate << "\n";
                for (int i = 0; i < venues.size(); i++)
                {
                    if (venues[i].name == venueName && venues[i].country == country && venues[i].congregationName == name)
                    {
                        for (int j = 0; j < venues[i].eventCount; j++)
                        {
                            if (venues[i].events[j].date == currentDateString)
                            {
                                cout << venues[i].events[j].eventname << " " << venues[i].events[j].start << " " << venues[i].events[j].ends << endl;
                            }
                        }
                    }
                }

                currentDate += dayDuration;
            } // Call showCalendar using the stored index
        }
        else
        {
            cout << "-1\nError\n"; // Print error if no venue matches
        }
    }

    void reserveVenue(const string &venueName, const string &venueCountry, const string &congregationName)
    {
        // Check if the congregation exists
        bool congregationExists = false;
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == congregationName)
            {
                congregationExists = true;
                break;
            }
        }

        if (!congregationExists)
        {
            cout << "-1\nError\n";
            return;
        }

        // Iterate through venues to find the matching one
        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == venueName && venues[i].country == venueCountry)
            {
                if (venues[i].isReserved)
                {
                    cout << "-1\nError\n";
                    return;
                }
                venues[i].isReserved = true;
                venues[i].congregationName = congregationName; // Link venue to the congregation
                cout << "0\n";
                return;
            }
        }

        // If no matching venue is found
        cout << "-1\nError\n";
    }

    void freeVenue(const string &venueName, const string &venueCountry, const string &congregationName)
    {
        int s = 0;
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == congregationName)
            {
                s += 1;
            }
        }
        if (s == 0)
        {
            cout << "-1\nError\n";
            return;
        }
        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == venueName && venues[i].country == venueCountry && venues[i].congregationName == congregationName)
            {
                if (!venues[i].isReserved)
                {
                    cout << "-1\nError\n";
                    return;
                }
                venues[i].isReserved = false;
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";
    }
    void showReserved(const string &congregationName) const
    {
        bool congregationExists = false;
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == congregationName)
            {
                congregationExists = true;
                break;
            }
        }

        if (!congregationExists)
        {
            cout << "-1\nError\n";
            return;
        }

        int reservedCount = 0;
        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].isReserved && venues[i].congregationName == congregationName)
            {
                reservedCount++;
            }
        }

        if (reservedCount == 0)
        {
            cout << "0\n";
            return;
        }

        cout << reservedCount << "\n";
        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].isReserved && venues[i].congregationName == congregationName)
            {
                cout << venues[i].name << " " << venues[i].location << " " << venues[i].capacity << "\n";
            }
        }
    }
};

// Congregation Class

class CongregationManager
{

public:
    int addCongregation(const string &name, const string &type, const string &startDate, const string &endDate)
    {
        if (congregationCount < MAX_CONGREGATIONS)
        {
            Congregation newCongregation;
            newCongregation.name = name;
            newCongregation.type = type;
            newCongregation.startdate = startDate;
            newCongregation.enddate = endDate;
            congregations[congregationCount] = newCongregation;
            congregationCount++;
            return 0;
        }
        else
        {
            cout << "-1\nError\n";
            return -1;
        }
    }

    int deleteCongregation(const string &name)
    {
        int s = 0;
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == name)
            {
                s++;
                break;
            }
        }
        if (s == 0)
        {
            cout << "-1\nError\n";
            return -1;
        }
        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].congregationName == name)
            {
                venues[i].isReserved = false;
            }
        }
        for (int i = 0; i < congregationCount; i++)
        {
            if (congregations[i].name == name)
            {
                for (int j = i; j < congregationCount - 1; j++)
                {
                    congregations[j] = congregations[j + 1];
                }
                congregationCount--;
                cout << "0\n";
                return 0;
            }
        }
        cout << "-1\nError\n";
        return -1;
    }

    void showCongregations() const
    {
        cout << congregationCount << "\n";
        for (int i = 0; i < congregationCount; i++)
        {
            cout << congregations[i].name << " " << congregations[i].type << " " << congregations[i].startdate << " " << congregations[i].enddate << "\n";
        }
        return;
    }
};

// Main function to handle user input and execute commands
int main()
{
    VenueManager venueManager;
    CongregationManager congregationManager;
    string line;

    while (getline(cin, line))
    {
        if (line == "End")
        {
            break;
        }
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "addCongregation")
        {
            string name, type, startDate, endDate;
            iss.ignore();
            if (iss.peek() == '\"')
            {
                iss.ignore();
                getline(iss, name, '\"');
            }
            if (iss.peek() == ' ')
                iss.ignore();
            if (iss.peek() == '\"')
            {
                iss.ignore();
                getline(iss, type, '\"');
            }
            if (iss.peek() == ' ')
                iss.ignore();
            if (iss.peek() == '\"')
            {
                iss.ignore();
                getline(iss, startDate, '\"');
            }
            if (iss.peek() == ' ')
                iss.ignore();
            if (iss.peek() == '\"')
            {
                iss.ignore();
                getline(iss, endDate, '\"');
            }

            bool dateIsValid = true;
            if (!isValidDate(startDate) || !isValidDate(endDate))
            {
                cout << "-1\nError\n";
                dateIsValid = false;
            }
            if (dateIsValid && !isStartDateBeforeEndDate(startDate, endDate))
            {
                cout << "-1\nError\n";
                dateIsValid = false;
            }
            if (dateIsValid && (isDateInPast(startDate) || isDateInPast(endDate)))
            {
                cout << "-1\nError\n";
                dateIsValid = false;
            }
            if (dateIsValid)
            {
                congregationManager.addCongregation(name, type, startDate, endDate);
                cout << "0\n"; // Success
            }
        }

        else if (command == "deleteCongregation")
        {
            string name;

            // Skip the first space after "deleteCongregation"
            iss.ignore();

            // Extract the congregation name enclosed in quotes
            getline(iss, name, '\"'); // Start reading from the first quote
            getline(iss, name, '\"'); // Read until the second quote

            // Call the function with the parsed name
            congregationManager.deleteCongregation(name);
        }

        else if (command == "showCongregations")
        {
            congregationManager.showCongregations();
        }
        else if (command == "addVenue")
        {
            string name, location;
            int capacity;                 // Ignore the space after the command
            getline(iss, name, '\"');     // Read up to the first quote
            getline(iss, name, '\"');     // Read the actual venue name
            getline(iss, location, '\"'); // Read up to the first quote
            getline(iss, location, '\"'); // Read the actual location
            iss >> capacity;
            venueManager.addVenue(name, location, capacity);
        }
        else if (command == "deleteVenue")
        {
            string name, country;

            // Extract the venue name
            iss.ignore();             // Ignore the space after the command
            getline(iss, name, '\"'); // Skip the first quote
            getline(iss, name, '\"'); // Get the actual name

            // Extract the country
            iss.ignore();                // Ignore the space before the quote
            getline(iss, country, '\"'); // Skip the first quote
            getline(iss, country, '\"'); // Get the actual country
            // Call the function to delete the venue
            venueManager.deleteVenue(name, country);
        }

        else if (command == "showVenues")
        {
            string fullLocation;

            iss.ignore();                          // Ignore the space after the command
            getline(iss, fullLocation, '\"');      // Read the entire location string, including the leading quote
            getline(iss, fullLocation, '\"');      // Read until the closing quote to get the full locatio
            venueManager.showVenues(fullLocation); // Pass the parsed location string
        }

        else if (command == "addEvent")
        {
            string congregationName, venueName, country, date, start, end, eventname;

            // Extract the congregation name
            iss.ignore();                         // Ignore the space after the command
            getline(iss, congregationName, '\"'); // Skip the first quote
            getline(iss, congregationName, '\"'); // Get the actual congregation name

            // Extract the venue name
            iss.ignore();                  // Ignore the space before the quote
            getline(iss, venueName, '\"'); // Skip the first quote
            getline(iss, venueName, '\"'); // Get the actual venue name

            // Extract the country
            iss.ignore();                // Ignore the space before the quote
            getline(iss, country, '\"'); // Skip the first quote
            getline(iss, country, '\"'); // Get the actual country

            iss.ignore();             // Ignore the space before the quote
            getline(iss, date, '\"'); // Skip the first quote
            getline(iss, date, '\"');

            iss.ignore();              // Ignore the space before the quote
            getline(iss, start, '\"'); // Skip the first quote
            getline(iss, start, '\"');

            iss.ignore();            // Ignore the space before the quote
            getline(iss, end, '\"'); // Skip the first quote
            getline(iss, end, '\"');

            // Extract the event name
            iss.ignore();                  // Ignore the space before the quote
            getline(iss, eventname, '\"'); // Skip the first quote
            getline(iss, eventname, '\"'); // Get the actual event name

            venueManager.addEvent(congregationName, venueName, country, date, start, end, eventname);
        }

        else if (command == "deleteEvent")
        {
            string congregationName, venueName, country, date, fromTime, eventname;

            // Skip the first space after "deleteEvent"
            iss.ignore();

            // Extract the congregation name enclosed in quotes
            getline(iss, congregationName, '\"'); // Start reading from the first quote
            getline(iss, congregationName, '\"'); // Read until the second quote

            // Skip the space after the congregation name
            iss.ignore();

            // Extract the venue name enclosed in quotes
            getline(iss, venueName, '\"'); // Start reading from the first quote
            getline(iss, venueName, '\"'); // Read until the second quote

            // Skip the space after the venue name
            iss.ignore();

            // Extract the country enclosed in quotes
            getline(iss, country, '\"'); // Start reading from the first quote
            getline(iss, country, '\"'); // Read until the second quote

            // Skip the space after the country
            iss.ignore();

            // Extract the date enclosed in quotes
            getline(iss, date, '\"'); // Start reading from the first quote
            getline(iss, date, '\"'); // Read until the second quote

            // Skip the space after the date
            iss.ignore();

            // Extract the fromTime enclosed in quotes
            getline(iss, fromTime, '\"'); // Start reading from the first quote
            getline(iss, fromTime, '\"'); // Read until the second quote

            // Skip the space after the fromTime
            iss.ignore();

            // Extract the event name enclosed in quotes
            getline(iss, eventname, '\"'); // Start reading from the first quote
            getline(iss, eventname, '\"'); // Read until the second quote

            venueManager.deleteEvent(congregationName, venueName, country, date, fromTime, eventname);
            
        }

        else if (command == "showEvents")
        {
            string venueName, country, date;

            // Skip the first space after "showEvents"
            iss.ignore();

            // Extract the venue name enclosed in quotes
            getline(iss, venueName, '\"'); // Start reading from the first quote
            getline(iss, venueName, '\"'); // Read until the second quote

            // Skip the space after the venue name
            iss.ignore();

            // Extract the country enclosed in quotes
            getline(iss, country, '\"'); // Start reading from the first quote
            getline(iss, country, '\"'); // Read until the second quote

            // Skip the space after the country
            iss.ignore();             // Ignore the space before the quote
            getline(iss, date, '\"'); // Skip the first quote
            getline(iss, date, '\"');

            // For debugging purposes, print the parsed componen

            // Call the function with the parsed parameters
            venueManager.showEvents(venueName, country, date);
        }

        else if (command == "showCalendar")
        {
            string congregationName, venueName, country;

            // Skip the first space after "showCalendar"
            iss.ignore();

            // Extract the congregation name enclosed in quotes
            getline(iss, congregationName, '\"'); // Start reading from the first quote
            getline(iss, congregationName, '\"'); // Read until the second quote

            // Skip the space after the congregation name
            iss.ignore();

            // Extract the venue name enclosed in quotes
            getline(iss, venueName, '\"'); // Start reading from the first quote
            getline(iss, venueName, '\"'); // Read until the second quote

            // Skip the space after the venue name
            iss.ignore();

            // Extract the country enclosed in quotes
            getline(iss, country, '\"'); // Start reading from the first quote
            getline(iss, country, '\"'); // Read until the second quote

            // Call the function with the parsed parameters
            venueManager.showCalendar(congregationName, venueName, country);
        }

        else if (command == "reserveVenue")
        {
            string venueName, venueCountry, congregationName;

            iss.ignore(); // Ignore the space after the command

            // Extract the venueName
            if (getline(iss, venueName, '\"'))
            {
                getline(iss, venueName, '\"'); // Get the actual venue name
            }

            // Extract the venueCountry
            if (getline(iss, venueCountry, '\"'))
            {
                getline(iss, venueCountry, '\"'); // Get the actual venue country
            }

            // Extract the congregationName
            if (getline(iss, congregationName, '\"'))
            {
                getline(iss, congregationName, '\"'); // Get the actual congregation name
            }

            // Debugging output to ensure values are captured

            // Call the function with the extracted values
            venueManager.reserveVenue(venueName, venueCountry, congregationName);
        }
        else if (command == "freeVenue")
        {
            string venueName, venueCountry, congregationName;

            // Extract the venueName
            iss.ignore();                  // Ignore the space after the command
            getline(iss, venueName, '\"'); // Skip the first quote
            getline(iss, venueName, '\"'); // Get the actual venue name

            // Extract the venueCountry
            iss.ignore();                     // Ignore the space after the first quote
            getline(iss, venueCountry, '\"'); // Skip the first quote
            getline(iss, venueCountry, '\"'); // Get the actual venue country

            // Extract the congregationName
            iss.ignore();                         // Ignore the space after the first quote
            getline(iss, congregationName, '\"'); // Skip the first quote
            getline(iss, congregationName, '\"'); // Get the actual congregation name

            // Debugging output to ensure values are captured

            // Call the function with the extracted values
            venueManager.freeVenue(venueName, venueCountry, congregationName);
        }

        else if (command == "showReserved")
        {
            string congregationName;

            // Ignore the space after the command
            iss.ignore();

            // Get the first character (the expected quote character)
            char quote;
            iss >> quote;

            // Check if the first character is a quote
            if (quote == '\"')
            {
                getline(iss, congregationName, '\"');
                venueManager.showReserved(congregationName);
            }
            else
            {
                // If the input format is incorrect (no quote found), print an error
                cout << "-1\nError\n";
            }
        }

        else
        {
            cout << "-1\nError\n";
        }
    }

    return 0;
}