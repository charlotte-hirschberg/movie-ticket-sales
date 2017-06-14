// Group 2: Emma, Chiara, Charlotte
// TicketManager class implementation
// Includes custom constructor and destructor, plus methods for tasks
// like returning a string with a seating chart, a string indicating the
// availability of requested seats, a receipt string, and a sales report string.
// Requires integration with 3 files
#include "TicketManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

/*TicketManager::TicketManager
Constructor that initializes 2D array's SeatStructures with values from
AVAIL_FILE and PRICING_FILE. It also initializes numSeatsAvailable.
*/
TicketManager::TicketManager(bool &success)
{
	ifstream inAvail;
	ifstream inPrices;
	ifstream inSales;
	double rowPrice;
	success = true;

	if (testReadable(inAvail, AVAIL_FILE) && testReadable(inPrices, PRICING_FILE) 
		&& testReadable(inSales, SALES_FILE)) // All files opened successfully and not empty
	{
		for (int row = 0; row < NUM_ROWS; row++)
			{
			// Get the double in each of PRICING_FILE's rows
			inPrices >> rowPrice;
			for (int seat = 0; seat < NUM_SEATS; seat++)
			{
				// Set the availability and price for the SeatStructure at [row][seat]
				inAvail >> seats[row][seat].availability;
				seats[row][seat].price = rowPrice;

				// Decrement numSeatsAvail to reflect data in AVAIL_FILE
				if (seats[row][seat].availability == '*')
				{
					numSeatsAvail--;
				}
			}
		}
		inSales >> totalSales; // Get the running total from SALES_FILE

		// There was an error while reading the file
		if (inAvail.fail() || inPrices.fail() || inSales.fail())
		{
			success = false;
		}

		inAvail.close();
		inPrices.close();
		inSales.close();
	}

	else // At least one file failed to open or was empty
	{
		success = false;
	}

	seatsSold = (NUM_ROWS * NUM_SEATS) - numSeatsAvail;
}

/*TicketManager::testReadable()
Returns true if a file both opens and is not empty
*/
bool TicketManager::testReadable(ifstream &istream, string fileName)
{
	bool b = false; // Assume the file is not readable

	istream.open(fileName);

	if (istream) // opens successfully
	{
		istream.seekg(0, ios::end); // move flag to end of file
		int length = istream.tellg();
		if (length != 0)
		{
			b = true;
		}

		istream.seekg(0, ios::beg); //return flag to start of file for reading
	}
	return b;
}

/*TicketManager::displaySeats()
Returns a string with rows of asterisks and #s to indicate
seat availability. The string is displayed by the client.
*/
string TicketManager::displaySeats()
{
	// First line in string
	string seatingChart = "		Seats\n	123456789012345678901234567890\n";

	// Each iteration creates a new line row, that is stored in seatingChart.
	// Example: Row 1: #############
	for (int row = 0; row < NUM_ROWS; row++)
	{
		seatingChart += "Row " + to_string(row + 1) + "	";
		for (int seat = 0; seat < NUM_SEATS; seat++)
		{
			seatingChart += seats[row][seat].availability;
		}
		seatingChart += "\n";
	}

	// Key to chart symbols
	seatingChart += "\nKEY: # Available  * Reserved\n";
	return seatingChart;
}

/* TicketManager::requestTickets
Accepts a row number, seat number, and ticket quantity from client.
Determines whether those seats are available.
Returns a string describing the result of the request and
sets &success to true or false so the client knows how to proceed
*/
string TicketManager::requestTickets(int aRowNum, int aSeatNum, int ticketQuantity, bool &success)
{
	string msg; // to be returned to client

	/* Validation to be performed by client: aRowNum must be greater than 0, not greater than NUM_ROWS
		aSeatNum must be greater than 0, not greater than NUM_SEATS */

	/* Make sure the user hasn't selected a ticketQuantity and starting seat number
		that cause the bank of seats to extend beyond the confines of the row */
	if (NUM_SEATS - (aSeatNum - 1) < ticketQuantity)
	{
		msg = "You have requested seats that don't exist.\n";
		success = false;
	}

	// All requested seats exist and are available
	else if (testAvail(seats, aRowNum, aSeatNum, ticketQuantity) == true)
	{
		double total = calcTotal(seats[aRowNum - 1][aSeatNum - 1].price, ticketQuantity);

		stringstream successMsg;
		successMsg << fixed << setprecision(2)
			<< "You have requested " << ticketQuantity << " seats at $"
			<< seats[aRowNum - 1][aSeatNum - 1].price << " each, for a total purchase of $"
			<< total << ".";
		msg = successMsg.str();
		success = true;
	}
	else // testAvail returns false; seats exist but not all are available
	{
		msg = "Some of the seats you have requested are not available.";
		success = false;
	}
	return msg;
}

/* TicketManager::testAvail
Private method that returns true if all seats in a requested bank are available
*/
bool TicketManager::testAvail(SeatStructure const array[][NUM_SEATS], int row, int startSeat, int quantity)
{
	// Execute loop not more than 'quantity' times
	for (int count = 0; count < quantity; count++)
	{
		// Each iteration will test availability in the same row
		/* Example of logic below: For quantity = 4 and startSeat = 7, startSeat is at subscript 6,
		so the 1st iteration will check the availability of the seat at subscript 7+0-1= 6.
		2nd iteration will check availability at subscript 7+1-1= 7.
		Final iteration will check availability at subscript 7+3-1=9.
		*/
		if (array[row - 1][startSeat + count - 1].availability == '*')
		{
			return false;
		}
	}
	return true;
}

/* TicketManager::calcTotal
Private method that returns the total calculated from the
ticket price and quantity supplied by the calling method
*/
double TicketManager::calcTotal(double price, int quantity)
{
	double total = price * quantity;
	return total;
}

/* TicketManager::purchaseTickets()
Once requestTickets() verifies the availability of the seats requested,
the client calls purchaseTickets() with the same parameters, plus a double
representing the amount of money presented by the user.
purchaseTickets verifies that the amount of money is sufficient;
calculates any change due; updates numSeatsAvail, seatsSold, and totalSales;
and finally sets &success to true or false to guide the client's next action.
*/
string TicketManager::purchaseTickets(double money, int aRowNum, int aSeatNum,
	int ticketQuantity, bool &success)
{
	double totalPrice = 0.0;

	for (int i = 0; i < ticketQuantity; i++)
	{
		totalPrice += seats[aRowNum - 1][aSeatNum - 1 + i].price;
	}

	if (money < totalPrice)
	{
		success = false;
		return "Money not sufficient to buy tickets. Re-enter quantity.";
	}

	for (int i = 0; i < ticketQuantity; i++)
	{
		seats[aRowNum - 1][aSeatNum - 1 + i].availability = '*';
		numSeatsAvail--;
		seatsSold++;
	}

	totalSales += totalPrice;
	//how to return change
	double change = money - totalPrice;
	stringstream ss;
	ss << fixed << setprecision(2)
		<< "Your change is $" << change << ". Tickets:\n";

	for (int count = 0; count < ticketQuantity; count++)
	{
		ss << "Seat #" << (aSeatNum + count) << ", Row " << (aRowNum)
			<< ": $" << seats[aRowNum - 1][aSeatNum - 1 + count].price << endl;
	}
	success = true;
	return ss.str();
}

/* TicketManager::createReport()
Uses numSeatsAvail and totalSales values to create
a formatted string for return to the client
*/
string TicketManager::createReport()
{
	stringstream salesTest;
	salesTest << fixed << setprecision(2)
		<< "The number of seats that have been sold is:  " << seatsSold << endl <<
		"The number of seats still available is:  " << numSeatsAvail << endl <<
		"The money earned from sales so far is $:  " << totalSales << endl;

	string createReport = salesTest.str();
	return createReport;
}

/* TicketManager::~TicketManager
Destructor that writes changes in seat availability to AVAIL_FILE
and new sales amounts to SALES_FILE
*/
TicketManager::~TicketManager()
{
	ofstream outAvail;
	ofstream outSales;

	// Open the output files
	outAvail.open(AVAIL_FILE); //make sure file is being opened and data changed
	outSales.open(SALES_FILE);

	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int seat = 0; seat < NUM_SEATS; seat++)
		{
			// Set the availability and price for the SeatStructure at [row][seat]
			outAvail << seats[row][seat].availability;
		}
		outAvail << '\n';
	}
	outSales << totalSales; // Update the sales total

	// If there is an error during update, notify user
	if (outAvail.fail() || outSales.fail())
	{
		cout << "Error updating one of the files. Please contact the administrator.\n";
	}

	outAvail.close();
	outSales.close();
}