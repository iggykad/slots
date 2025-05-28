/*
this program currently has 1 winning value, 21 which adds 10 credits

*/

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <Windows.h>




void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void resetColor()
{
	setColor(7);
}


void runSlot(int& credits)

{
	using namespace std::chrono;
	srand(static_cast<unsigned int>(time(0)));//seed is 0 seconds

	for (int i = 0; i < 3; i++) {
		int num1 = rand() % 22;
		int num2 = rand() % 22;
		int num3 = rand() % 22;

		std::cout << "[";
		setColor(10);
		std::cout << num1;
		resetColor();

		std::cout << "] [";
		setColor(12);
		std::cout << num2;
		resetColor();

		std::cout << "] [";
		setColor(9);
		std::cout << num3;
		resetColor();
		std::cout << "]\n\n";


		if (num1 == 21) {
			credits += 10;
			setColor(2);
			std::cout << "21! +10 credits\n\n";
			resetColor();

			Beep(1567, 90); Beep(1174, 90); Beep(1567, 90); Beep(1760, 90);
		}
		if (num2 == 21) {
			credits += 10;
			setColor(2);
			std::cout << "21! +10 credits\n\n";
			resetColor();

			Beep(1567, 90); Beep(1174, 90); Beep(1567, 90); Beep(1760, 90);
		}
		if (num3 == 21) {
			credits += 10;
			setColor(2);
			std::cout << "21! +10 credits\n\n";
			resetColor();

			Beep(1567, 90); Beep(1174, 90); Beep(1567, 90); Beep(1760, 90);
		}

		if (num1 == 7) {
			credits += 75;
			setColor(2);
			std::cout << "7! +75 credits\n\n";
			resetColor();

			Beep(1567, 90); Beep(1174, 90); Beep(1567, 90); Beep(1760, 90);
		}
		if (num2 == 7) {
			credits += 75;
			setColor(2);
			std::cout << "7! +75 credits\n\n";
			resetColor();

			Beep(1567, 90); Beep(1174, 90); Beep(1567, 90); Beep(1760, 90);
		}
		if (num3 == 7) {
			credits += 75;
			setColor(2);
			std::cout << "7! +75 credits\n\n";
			resetColor();

			Beep(1567, 90); Beep(1174, 90); Beep(1567, 90); Beep(1760, 90);
		}



		Beep(550, 75);
		std::this_thread::sleep_for(std::chrono::milliseconds(220));
	}

}

int askGamble(int& credits)

{
	std::cout << "\nCredits: " << credits << "\n\nHit button? (y/n)\n";
	std::string x;
	std::cin >> x;

	if (x == "bonus") //secret code 
	{
		using namespace std::chrono_literals;


		credits += 300;
		std::this_thread::sleep_for(std::chrono::milliseconds(220));
		std::cout << "\nCheat code used. +300 credits.\n\n";

		Beep(466, 70);
		Beep(932, 90);
		Beep(466, 70);
		Beep(932, 90);
		Sleep(300);
		Beep(466, 70);
		Beep(932, 90);
		Beep(466, 70);
		Beep(932, 90);

		return 2;

	}

	if (x == "y")
	{
		std::cout << "\n";

		for (int i = 0; i < 3; i++) {
			setColor(9 + i);
			std::cout << "Spinning...";
			resetColor();
			std::cout << "\r";
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::cout << "\n\n";

		return 1;
	}

	if (x == "n") {

		std::cout << "\nCome again.\n";
		Beep(554, 150); Beep(440, 150); Beep(370, 150); Beep(586, 150); //melody that plays upon "n" press, meaning the exit of the program
		return 0;

	}
}

int main() {
	int credits = 10;

	std::cout << "\nWelcome to the slots!\n";
	std::cout << "\nIf you hit 21, you get 10 credits. The secret code will also give you 300 extra credits.\n\n";
	std::cout << "If you hit 7, you get 75 credits\n";
	Beep(147, 110); Beep(185, 110); Beep(220, 110); Beep(277, 110); Beep(294, 110);

	while (credits > 0) {
		int result = askGamble(credits);

		if (result == 1) {
			credits -= 5;
			runSlot(credits);
		}
		else if (result == 0) {
			return 0;
		}
	}

	std::cout << "\nNo credits left.\n";
	return 0;
}