// Group 2: Emma, Chiara, Charlotte
// TicketManager.h is the TicketManager class specification file.
#ifndef TICKETMANAGER_H
#define TICKETMANAGER_H
#include <string>
using namespace std;

// Global variables used in class and client
static const int NUM_ROWS = 15;
static const int NUM_SEATS = 30;

// TicketManager class declaration
class TicketManager
{
private:
	struct SeatStructure
	{
		double price;
		char availability;

		SeatStructure()
		{
			price = 0.00;
			availability = '#';
		}
	};

	// Included for flexibility in testing and future upgrades
	const string PRICING_FILE = "SeatPrices.dat";
	const string AVAIL_FILE = "SeatAvailability.dat";
	const string SALES_FILE = "SalesTotal.dat";

	SeatStructure seats[NUM_ROWS][NUM_SEATS];

	// Included for use in sales report
	int numSeatsAvail = NUM_ROWS * NUM_SEATS; // assume all seats are available initially
	double totalSales = 0.00; // Accumulator, constructor assigns it the value in SALES_FILE
	int seatsSold;

	// Methods used only by other methods
	bool testReadable(ifstream& istream, string fileName);
	bool testAvail(const SeatStructure[][NUM_SEATS], int aRowNum, int aSeatNum, int ticketQuantity);
	double calcTotal(double price, int quantity);

public:
	TicketManager(bool& success);
	string displaySeats();
	string requestTickets(int aRowNum, int aSeatNum, int ticketQuantity, bool& success);
	string purchaseTickets(double money, int aRowNum, int aSeatNum, int ticketQuantity, bool& success);
	string createReport();
	~TicketManager();
};

#endif
