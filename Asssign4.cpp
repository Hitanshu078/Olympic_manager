#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <map>
#include <string>
#include <utility>
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
    tm tm = {};
    istringstream ss(date);
    ss >> get_time(&tm, "%Y-%m-%d");

    // Check if date parsing was successful
    if (ss.fail())
    {
        cerr << "Date parsing failed.\n";
        return false;
    }

    // Get current time
    time_t t = time(nullptr);
    std::tm *now = localtime(&t);

    // Convert the input date to time_t
    time_t givenTime = mktime(&tm);
    time_t currentTime = mktime(now);

    // Check if the given date is in the past
    return givenTime < currentTime;
}
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(const string &date)
{
    tm tm = {};
    istringstream ss(date);
    ss >> get_time(&tm, "%Y-%m-%d");

    // Check if parsing failed
    if (ss.fail())
    {
        cout << "Date parsing failed for: " << date << '\n';
        return false;
    }

    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;

    // Validate year, month, and day ranges
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        cout << "Date out of range for: " << date << '\n';
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
chrono::system_clock::time_point parseDate(const string &dateStr)
{
    istringstream ss(dateStr);
    tm tm = {};
    ss >> get_time(&tm, "%Y-%m-%d");
    return chrono::system_clock::from_time_t(mktime(&tm));
}

// Function to format a time_point as a date string in YYYY-MM-DD format
string formatDate(chrono::system_clock::time_point tp)
{
    time_t time = chrono::system_clock::to_time_t(tp);
    tm *tm = localtime(&time);
    ostringstream oss;
    oss << put_time(tm, "%Y-%m-%d");
    return oss.str();
}

// Function to check if startDate is before endDate
bool isStartDateBeforeEndDate(const string &startDate, const string &endDate)
{
    tm tmStart = {}, tmEnd = {};
    istringstream ssStart(startDate), ssEnd(endDate);
    ssStart >> get_time(&tmStart, "%Y-%m-%d");
    ssEnd >> get_time(&tmEnd, "%Y-%m-%d");
    return mktime(&tmStart) < mktime(&tmEnd);
}

class Program
{
public:
    string name;
    string type;
    string startdate;
    string enddate;
    Program(string n, string t, string sDate, string eDate)
        : name(n), type(t), startdate(sDate), enddate(eDate) {}
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
    string type;
    string congregationName;
    int capacity;
    bool isReserved;
    vector<Program> program; // Vector of Program class

    // Default constructor
    Venue() : name(""), location(""), country(""), city(""), state(""), postalCode(""),
              capacity(0), isReserved(false) {}

    // Parameterized constructor
    Venue(const string &n, const string &loc, const string &type, int cap)
        : name(n), location(loc), type(type), capacity(cap), isReserved(false)
    {
        if (!isValidType(type))
        {
            throw invalid_argument("Invalid venue type");
        }
    }

    // Check if the venue supports the program type
    bool supportsProgramType(const string &venueType,const string &programType)
    {
        if (venueType == "Outdoor Stadium")
        {
            return programType == "Track and field" || programType == "Ceremony" ||
                   programType == "Concert" || programType == "Convention";
        }
        else if (venueType == "Indoor Stadium")
        {
            return programType == "Indoor games";
        }
        else if (venueType == "Convention Center")
        {
            return programType == "Conference" || programType == "Concert" || programType == "Convention";
        }
        else if (venueType == "Concert Hall")
        {
            return programType == "Concert";
        }
        else if (venueType == "Hotel")
        {
            return programType == "Conference" || programType == "Convention" || programType=="Workshop";
        }
        else if (venueType == "Swimming Pool")
        {
            return programType == "Swimming Event";
        }
        return false; // If venue type is not recognized, return false
    }

    // Check if the venue is available for the given date range
    bool isAvailableForDates(const string &startDate, const string &endDate) const
    {
        // Iterate through the reserved dates and check for overlap
        for (const auto &reservation : program)
        {
            string reservedStartDate = reservation.startdate;
            string reservedEndDate = reservation.enddate;

            // Check for date overlap: (startDate <= reservedEndDate) && (endDate >= reservedStartDate)
            if (!(endDate < reservedStartDate || startDate > reservedEndDate))
            {
                return false;
            }
        }
        return true;
    }

    // Helper function to validate venue type
    bool isValidType(const string &type)
    {
        return type == "Outdoor Stadium" || type == "Indoor Stadium" ||
               type == "Swimming Pool" || type == "Hotel" ||
               type == "Convention Center" || type == "Concert Hall";
    }

    // Reserve venue for a specific program and date range
    void reserveForProgram(const string &startDate, const string &endDate, const string &programName)
    {
        Program k(programName, "", endDate, startDate);

        // Insert at the beginning of the vector
        program.insert(program.begin(), k);

        isReserved = true;
    }
};
vector<Venue> venues(MAX_VENUES);
int venuecount = 0;

class Congregation
{
public:
    string name;
    string type;
    string startDate;
    string endDate;
    vector<Program> programs;

    Congregation()
        : name(""), type(""), startDate(""), endDate("")
    {
    }

    Congregation(const string &name, const string &type, const string &startDate, const string &endDate)
        : name(name), type(type), startDate(startDate), endDate(endDate)
    {
    }

    //to check if program type is valid or not
    bool isValidProgramType(const string &programType)
    {
        if (type == "Conference")
        {
            return programType == "Workshop" || programType == "Main Conference" || programType == "Banquet";
        }
        else if (type == "Games")
        {
            return programType == "Track and field" || programType == "Indoor games" || programType == "Water sports" || programType == "Ceremony";
        }
        else if (type == "Concert")
        {
            return programType == "Preconcert" || programType == "Main Concert";
        }
        else if (type == "Convention")
        {
            return programType == "Food Court" || programType == "Exhibition";
        }
        return false;
    }

    // To add a program in the congregation
    int addProgram(const string &programType, const string &programName, const string &pStartDate, const string &pEndDate)
    {
        if (!isValidProgramType(programType))//checks if program type is valid or not
            return -1;
        if (!isValidDate(pStartDate) || !isValidDate(pEndDate))//checks if dates are valid or not
            return -1;
        if (pStartDate < startDate || pEndDate > endDate)//checks for overlap of the dates
            return -1;

        for (const Program &p : programs)
        {
            if (p.name == programName)
                return -1; // Duplicate program name
        }

        programs.emplace_back(programName, programType, pStartDate, pEndDate);
        return 0;
    }

    int deleteProgram(const string &programName)
    {
        // Find the program by name in the congregation
        auto it = find_if(programs.begin(), programs.end(), [&programName](const Program &p)
                          { return p.name == programName; });

        if (it == programs.end())
            return -1; // Program not found

        // Check for any associated venue reservations
        for (int i = 0; i < venuecount; i++)
        {
            // Iterate through the reserved programs in the venue
            for (const auto &reservedProgram : venues[i].program)
            {
                if (reservedProgram.name == programName)
                {
                    if (venues[i].isReserved) // Check if the venue is reserved
                    {
                        return -1; // Program is reserved in a venue, return an error
                    }
                }
            }
        }

        // If no venue has the program reserved, proceed with deleting it from the venue and congregation
        for (int i = 0; i < venuecount; i++)
        {
            // Iterate through the reserved programs in the venue
            for (auto resIt = venues[i].program.begin(); resIt != venues[i].program.end();)
            {
                if (resIt->name == programName) // Check if the program is reserved in this venue
                {
                    // Erase the reservation for the program
                    resIt = venues[i].program.erase(resIt);
                }
                else
                {
                    ++resIt;
                }
            }
        }

        // Remove the program from the congregation
        programs.erase(it);
        return 0;
    }

    void showPrograms() const//to print all the programs in the congregation
    {
        cout << programs.size() << endl;
        for (const Program &p : programs)
        {
            cout << p.type << " " << p.name << " " << p.startdate << " " << p.enddate << endl;
        }
    }
};

// Conference class implementation
class Conference : public Congregation
{
public:
    Conference(const string &name, const string &startDate, const string &endDate)
        : Congregation(name, "Conference", startDate, endDate) {}

    bool isValidProgramType(const string &programType)
    {
        return programType == "Workshop" || programType == "Main Conference" || programType == "Banquet";
    }
};

// Games class implementation with venue checking for specific sports
class Games : public Congregation
{
public:
    Games(const string &name, const string &startDate, const string &endDate)
        : Congregation(name, "Games", startDate, endDate) {}

    bool isValidProgramType(const string &programType)
    {
        return programType == "Ceremony" || programType == "Track and field" ||
               programType == "Indoor games" || programType == "Water sports";
    }

    int addProgram(const string &programType, const string &programName, const string &pStartDate, const string &pEndDate)
    {
        if (!isValidProgramType(programType))
            return -1;

        // Venue validation for specific types of sports
        if ((programType == "Track and field") ||
            (programType == "Indoor games") ||
            (programType == "Water sports"))
        {
            return -1; // Invalid venue type for the sport
        }

        return Congregation::addProgram(programType, programName, pStartDate, pEndDate);
    }
};
class Convention : public Conference // Convention is an extension of Conference
{
public:
    Convention(const string &name, const string &startDate, const string &endDate)
        : Conference(name, startDate, endDate) {}

    bool isValidProgramType(const string &programType)
    {
        return programType == "Workshop" || programType == "Main Conference" ||
               programType == "Banquet" || programType == "Food Court" || programType == "Exhibition";
    }
};

class Concert : public Congregation
{
public:
    Concert(const string &name, const string &startDate, const string &endDate)
        : Congregation(name, "Concert", startDate, endDate) {}

    bool isValidProgramType(const string &programType)
    {
        return programType == "Pre-concert" || programType == "Main Concert";
    }
};
int congregationCount = 0;
vector<Congregation> congregations(MAX_CONGREGATIONS);


class VenueManager
{ //VenueManager class manages all the venue related functions

public:
    void addVenue(const string &name, const string &location, const string &type, int capacity)
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
        newVenue.type = type;
        newVenue.name = name;
        newVenue.capacity = capacity;
        newVenue.location = location;

        string addr, city, state, postal, country;
        istringstream ss(location);//parsing the location to get addr,city,state,postal,country
        getline(ss, addr, ':');
        getline(ss, city, ':');
        getline(ss, state, ':');
        getline(ss, postal, ':');
        getline(ss, country, ':');

        newVenue.country = country;
        newVenue.city = city;
        newVenue.state = state;
        newVenue.postalCode = postal;

        venues.insert(venues.begin() + venuecount, newVenue);//adding the venue to the array

        venuecount++;
        cout << "0\n";
    }
    // Function to delete a venue by name
    void deleteVenue(const string &name, const string &country)
    {

        for (int i = 0; i < venues.size(); i++)
        {
            if (venues[i].name == name && venues[i].country == country && venues[i].isReserved == false)//searching for the venue with specified name and country
            {
                int k = i;
                venues.erase(venues.begin() + k);//deleting the venue
                venuecount--;
                cout << "0\n";
                return;
            }
        }
        cout << "-1\nError\n";//Error if venue not found
    }
    //function to print the venues 
    void showVenues(const string &location, const string &optionalType)
    {
        if (venues.empty())//if venue is empty print zero
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
        // Validate the country input
        if (country.empty())
        {
            cout << "-1\nError: Country must be provided.\n";
            return;
        }

        // Ensure city is not empty if state is provided
        if (state.empty() && !city.empty())
        {
            cout << "-1\nError: City must be provided if state is given.\n";
            return;
        }

        vector<Venue> matchingVenues;

        for (const auto &venue : venues)
        {
            // Check if the venue matches the location criteria
            bool locationMatches = false;

            if (!city.empty() && !state.empty() && !postalCode.empty() && !country.empty() &&
                venue.city == city && venue.state == state && venue.postalCode == postalCode && venue.country == country)
            {
                locationMatches = true;
            }
            else if (!postalCode.empty() && !country.empty() && venue.postalCode == postalCode && venue.country == country &&
                     city.empty() && state.empty())
            {
                locationMatches = true;
            }
            else if (!city.empty() && !state.empty() && !country.empty() && venue.city == city && venue.state == state &&
                     venue.country == country && postalCode.empty())
            {
                locationMatches = true;
            }
            else if (!country.empty() && venue.country == country && city.empty() && state.empty() && postalCode.empty())
            {
                locationMatches = true;
            }

            // If location matches, check if optionalType is provided and if it matches the venue type
            if (locationMatches)
            {
                if (optionalType.empty() || venue.type == optionalType)//if the optionaltype is empty or equals the venuetype(as giving optional type is optional)
                {
                    matchingVenues.push_back(venue);
                }
            }
        }

        // Output the result
        if (matchingVenues.empty())
        {
            cout << "0\n";
        }
        else
        {
            cout << matchingVenues.size() << "\n";
            for (const auto &venue : matchingVenues)//printing all the matching venues
            {
                cout << venue.name << " " << venue.location << " " << venue.type << " " << venue.capacity << "\n";
            }
        }
    }
    //function to reserve the venue for the program
    void reserveVenue(const string &venueName, const string &country, const string &congregationName,
                      const string &programName)
    {
        int index = -1;
        for (int i = 0; i < congregationCount; i++)//searching for the congregation
        {
            if (congregations[i].name == congregationName)
            {
                index = i;
                break;
            }
        }

        // If congregation not found, return an error
        if (index == -1)
        {
            cout << "-1\nError\n";
            return;
        }

        // Search for the program inside the congregation by its name and get the program details
        string programType;
        string startDate;
        string endDate;
        bool programFound = false;
        for (const auto &program : congregations[index].programs)
        {
            if (program.name == programName)
            {
                programType = program.type;
                startDate = program.startdate;
                endDate = program.enddate;
                programFound = true;
                break;
            }
        }

        // If program not found, return an error
        if (!programFound)
        {
            cout << "-1\nError\n";
            return;
        }
        string type;
        // Search for the venue by its name and country
        Venue *venue = nullptr;
        for (auto &v : venues)
        {
            if (v.name == venueName && v.country == country)
            {
                venue = &v;
                type=v.type;
                break;
            }
        }

        // If venue not found, return an error
        if (venue == nullptr)
        {
            cout << "-1\nError\n";
            return;
        }

        // Check if the venue supports the program type
        if (!venue->supportsProgramType(type,programType))
        {
            cout << "-1\nError\n";
            return;
        }

        // Check if the venue is available for the given date range
        if (!venue->isAvailableForDates(startDate, endDate))
        {
            cout << "-1\nError\n";
            return;
        }

        // Reserve the venue for the program with the given date range
        venue->reserveForProgram(startDate, endDate, programName);
        venue->isReserved = true; // Set the venue as reserved

        cout << "0\n";
    }
    //function to free the reserved venue
    void freeVenue(const string &venueName, const string &country, const string &congregationName, const string &programName)
    {
        // Find the congregation by name
        Congregation *foundCongregation = nullptr;
        for (auto &cong : congregations)
        {
            if (cong.name == congregationName)
            {
                foundCongregation = &cong;
                break;
            }
        }

        if (foundCongregation == nullptr)
        {
            cerr << "-1\nError\n";
            return;
        }

        // Find and free the venue if it is reserved for the specified program
        bool venueFreed = false;
        for (auto &venue : venues)
        {
            if (venue.name == venueName && venue.country == country)
            {
                // Check if the venue has any reservations for the specified program
                bool programFound = false;
                for (auto it = venue.program.begin(); it != venue.program.end();)
                {
                    if (it->name == programName)
                    {
                        // Erase the reservation for the program
                        it = venue.program.erase(it);
                        venue.isReserved = !venue.program.empty(); // Update reservation status based on remaining reservations
                        programFound = true;
                    }
                    else
                    {
                        ++it;
                    }
                }

                if (programFound)
                {
                    cout << "0\n"; // Successfully freed the venue
                    venueFreed = true;
                    break;
                }
            }
        }

        if (!venueFreed)
        {
            cout << "-1\nError\n";
        }
    }
    //function to print all the reserved venues 
    void showReserved(const string &congregationName) const
    {
        // Find the congregation by name
        const Congregation *congregation = nullptr;
        for (const auto &cong : congregations)
        {
            if (cong.name == congregationName)
            {
                congregation = &cong;
                break;
            }
        }

        // If congregation not found, return an error
        if (congregation == nullptr)
        {
            cout << "-1\nError\n";
            return;
        }

        // Check if the congregation has any programs
        if (congregation->programs.empty())
        {
            cout << "0\n";
            return;
        }

        // Print the number of programs
        cout << congregation->programs.size() << "\n";

        // Iterate through each program and show reservation details
        for (const auto &program : congregation->programs)
        {
            // Count the number of reservations for this program
            int reservationCount = 0;
            vector<Venue> reservedVenues;

            // Check if any venues are reserved for this program
            for (const auto &venue : venues)
            {
                if (venue.isReserved)
                {
                    // Check if the program is reserved in this venue's reservedPrograms map
                    for (const auto &reservation : venue.program)
                    {
                        if (reservation.name == program.name) // Compare program names
                        {
                            reservedVenues.push_back(venue);
                            reservationCount++;
                            break; // No need to check other reservations for this venue
                        }
                    }
                }
            }

            // Print program name, type, and number of reservations
            cout << program.name << " " << program.type << " " << reservationCount <<endl;

            // If there are reservations, print each venue's details
            if (reservationCount > 0)
            {
                for (const auto &venue : reservedVenues)
                {
                    cout << venue.name << " " << venue.location
                         << " " <<venue.type<<" " << venue.capacity << endl;
                }
            }
        }
    }
};

class ProgramManager
{

public:
    int addProgram(const string &congregationName, const string &programType, const string &programName, const string &startDate, const string &endDate)
    {
        for (Congregation &c : congregations)
        {
            if (c.name == congregationName)
            {
                return c.addProgram(programType, programName, startDate, endDate);
            }
        }
        return -1; // Congregation not found
    }

    int deleteProgram(const string &congregationName, const string &programName)
    {
        for (Congregation &c : congregations)
        {
            if (c.name == congregationName)
            {
                return c.deleteProgram(programName);
            }
        }
        cout << "-1\nError\n";
        return -1; // Congregation not found
    }

    void showPrograms(const string &congregationName)
    {
        for (Congregation &c : congregations)
        {
            if (c.name == congregationName)
            {
                c.showPrograms();
                return;
            }
        }
        cout << "-1\nError\n"; // Congregation not found
    }
};

// CongregationManager Class

class CongregationManager
{

public:
    int addCongregation(const string &name, const string &type, const string &startDate, const string &endDate)
    {
        Congregation newCongregation;
        newCongregation.name = name;
        newCongregation.type = type;
        newCongregation.startDate = startDate;
        newCongregation.endDate = endDate;

        // Insert the new congregation at the beginning
        congregations.insert(congregations.begin() + congregationCount, newCongregation);

        congregationCount++;
        return 0;
    }

    int deleteCongregation(const string &name) 
    {
        int s = 0;
        for (int i = 0; i < congregationCount; i++)//check if the congregation is present or not
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
        for (int i = 0; i < venues.size(); i++)//check for the venues within congregation 
        {
            if (venues[i].congregationName == name)
            {
                venues[i].isReserved = false; //free the reserevd venue within the congregation
            }
        }
        for (int i = 0; i < congregationCount; i++)//shift the congregation by one (it is implemented like this from the first assignment itself)
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
            cout << congregations[i].name << " " << congregations[i].type << " " << congregations[i].startDate << " " << congregations[i].endDate << "\n";
        }
        return;
    }
};

// Main function to handle user input and execute commands
int main()
{
    VenueManager venueManager;
    CongregationManager congregationManager;
    ProgramManager programManager;

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
            if (!isValidDate(startDate) || !isValidDate(endDate))//check for valid dates
            {
                cout << "-1\nError\n";
                dateIsValid = false;
            }
            if (dateIsValid && !isStartDateBeforeEndDate(startDate, endDate))//check if end date is after the start date
            {
                cout << "-1\nError\n";
                dateIsValid = false;
            }
            if (dateIsValid && (isDateInPast(startDate) || isDateInPast(endDate)))//check if congregations being added are in past
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
            string name, location, type;
            int capacity;                 // Ignore the space after the command
            getline(iss, name, '\"');     // Read up to the first quote
            getline(iss, name, '\"');     // Read the actual venue name
            getline(iss, location, '\"'); // Read up to the first quote
            getline(iss, location, '\"');
            getline(iss, type, '\"'); // Read up to the first quote
            getline(iss, type, '\"'); // Read the actual location
            iss >> capacity;
            venueManager.addVenue(name, location, type, capacity);
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
            string fullLocation, optional;

            iss.ignore();                     // Ignore the space after the command
            getline(iss, fullLocation, '\"'); // Read the entire location string, including the leading quote
            getline(iss, fullLocation, '\"');
            getline(iss, optional, '\"');                    // Read the entire location string, including the leading quote
            getline(iss, optional, '\"');                    // Read until the closing quote to get the full locatio
            venueManager.showVenues(fullLocation, optional); // Pass the parsed location string
        }

        else if (command == "reserveVenue")
        {
            string venueName, venueCountry, congregationName, programname, date;

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
            if (getline(iss, programname, '\"'))
            {
                getline(iss, programname, '\"'); // Get the actual congregation name
            }

            // Debugging output to ensure values are captured

            // Call the function with the extracted values
            venueManager.reserveVenue(venueName, venueCountry, congregationName, programname);
        }
        else if (command == "freeVenue")
        {
            string venueName, venueCountry, congregationName, programname;

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

            iss.ignore();                    // Ignore the space after the first quote
            getline(iss, programname, '\"'); // Skip the first quote
            getline(iss, programname, '\"');

            // Debugging output to ensure values are captured

            // Call the function with the extracted values
            venueManager.freeVenue(venueName, venueCountry, congregationName, programname);
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
        else if (command == "addProgram")
        {
            string congregationName, programName, type, startdate, enddate;

            // Extract the congregation name
            iss.ignore();                         // Ignore the space after the command
            getline(iss, congregationName, '\"'); // Skip the first quote
            getline(iss, congregationName, '\"'); // Get the actual congregation name

            iss.ignore();             // Ignore the space before the quote
            getline(iss, type, '\"'); // Skip the first quote
            getline(iss, type, '\"');
            // Extract the program name
            iss.ignore();                    // Ignore the space before the quote
            getline(iss, programName, '\"'); // Skip the first quote
            getline(iss, programName, '\"'); // Get the actual program name

            // Extract the program type
            // Get the actual program type

            // Extract the program date
            iss.ignore();                  // Ignore the space before the quote
            getline(iss, startdate, '\"'); // Skip the first quote
            getline(iss, startdate, '\"'); // Get the actual program date

            iss.ignore();                // Ignore the space before the quote
            getline(iss, enddate, '\"'); // Skip the first quote
            getline(iss, enddate, '\"');

            // Call the function to add the program
            int success = programManager.addProgram(congregationName, type, programName, startdate, enddate);

            if (success != -1)
                cout << "0\n"; // Success
            else
                cout << "-1\nError\n"; // Failure
        }

        else if (command == "deleteProgram")
        {
            string congregationName, programName;

            // Extract the congregation name
            iss.ignore();                         // Ignore the space after the command
            getline(iss, congregationName, '\"'); // Skip the first quote
            getline(iss, congregationName, '\"'); // Get the actual congregation name

            // Extract the program name
            iss.ignore();                    // Ignore the space before the quote
            getline(iss, programName, '\"'); // Skip the first quote
            getline(iss, programName, '\"'); // Get the actual program name

            // Call the function to delete the program
            int success = programManager.deleteProgram(congregationName, programName);
            if (success != -1)
                cout << "0\n"; // Success
            else
                cout << "-1\nError\n"; // Failure
        }

        else if (command == "showPrograms")
        {
            string congregationName;

            // Extract the congregation name
            iss.ignore();                         // Ignore the space after the command
            getline(iss, congregationName, '\"'); // Skip the first quote
            getline(iss, congregationName, '\"'); // Get the actual congregation name

            // Call the function to show programs
            programManager.showPrograms(congregationName);
        }

        else
        {
            cout << "-1\nError\n";//invalid input command 
        }
    }

    return 0;
}