// Group 2: Emma, Chiara, Charlotte
// Client program that uses TicketManager class to display a seating chart,
// check seat availability, enable ticket purchases, and display a sales report
#include <iostream>
#include <iomanip>
#include <string>
#include "TicketManager.h" // Contains TicketManager class declaration

using namespace std;

void mainmenu(); // Display the menu
int validation(int, int); // Validate menu choices and function arguments

int main()
{
	char repeat;	// Repeat again? Y/N
	int choice,		// Menu number
		aRowNum,
		aSeatNum,
		ticketQuantity;
	string msg;		// Store string returned from each function in turn
	double money;	// purchaseTickets argument
	bool success;	// Store value indicating a method's success or failure

	TicketManager m(success); // Initialize object

	if (!success) // Constructor ran with file errors
	{
		cout << "There was an error opening or reading one of the program files.\n"
			<< "The program will not proceed. Please contact an administrator.\n";
	}

	else // Constructor successfully read all needed files
	{
		do
		{
			mainmenu();
			cout << "\nEnter your choice: \n";
			choice = validation(1, 4);

			if (choice != 4) // If choice is not Exit
			{
				if (choice == 1) //Display the seating chart.
				{
					msg = m.displaySeats();
					cout << endl << msg << endl;
				}

				else if (choice == 2) //Request tickets.
				{
					success = false;

					while (success == false)
					{
						cout << "\nWhich row would you like to select (1-" << NUM_ROWS << ")?" << endl;

						aRowNum = validation(1, NUM_ROWS);

						cout << "Which seat would you like to select (1-" << NUM_SEATS << ")?" << endl;

						aSeatNum = validation(1, NUM_SEATS);

						cout << "How many tickets would you like?" << endl;
						ticketQuantity = validation(1, NUM_SEATS);

						msg = m.requestTickets(aRowNum, aSeatNum, ticketQuantity, success);

						cout << endl << msg << endl;

						// requestTickets discovered unavailable seats
						if (success == false)
						{
							cout << "You will now be asked to select different seats.\n";

							char viewAgain;
							cout << "Would you like to see the seating chart again first? (Y/N) \n";
							cin >> viewAgain;

							if (viewAgain == 'Y' || viewAgain == 'y')
							{
								msg = m.displaySeats();
								cout << endl << msg << endl;
							}
						}
					}
					// User has successfully requested available seats
					success = false; // Reset this so we know the result of purchaseTickets

					while (success == false)
					{
						cout << "How much money will you be using? \n";
						cin >> money;
						msg = m.purchaseTickets(money, aRowNum, aSeatNum, ticketQuantity, success);
						cout << endl << msg << endl;
					}
				}

				else // Sales report
				{
					msg = m.createReport();
					cout << endl << msg << endl;
				}

				cout << "Would you like to return to the main menu? (Y/N)" << endl;
				cin >> repeat;
			}

			else // Menu Choice 4 selected
			{
				repeat = 'N';
			}
			system("cls"); // Clear the screen before re-displaying the menu
		} while (repeat == 'Y' || repeat == 'y');
	}

	cout << "Thank you and goodbye.\n";

	return 0;
}

//Functions//

//Function to display the main menu and prompt user choice.//

void mainmenu()
{
	cout << "Welcome to the Theatre Ticket System!" << endl;
	cout << "1. Show the seating chart." << endl;
	cout << "2. Request tickets." << endl;
	cout << "3. Print a sales report." << endl;
	cout << "4. Exit. " << endl;
}


//Function to check if user input on menu is valid.//

//For requestTickets call may look like this

//validation (0,NUM_ROWS) for row choice
//validation (0,NUM_SEATS) for seat choice

int validation(int min, int max)
{
	int choice;

	cin >> choice;

	while (choice < min || choice > max)
	{
		cout << endl << "Error. The value you have chosen is not a valid option." << endl;
		cout << "Please enter your choice again." << endl;
		cin >> choice;
	}

	return choice;
}