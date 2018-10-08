#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <stdlib.h>
using namespace std;

bool gameOver = false;

int getRandomNumber(int min = 0, int max = 100) {
	return rand() % (max + 1 - min) + min;
}

class Plane {
	private:
	enum planeState {TakeOff, Fly, Land, Crash, Evade, Attack, TaxiOut, Parked, Hit, TaxiIn, Alert, Escape, Battle};

	public:
	int fuel = 100;
	int health = 100;
	planeState state = Parked;	

	string UpdateState() {
		string message;

		//Parked
		if (state == Parked) {
			message = "The plane is parked.";
			if (fuel < 100) { message += "\nIt has been refuelled."; };
			if (health < 100) { message += "\nIt has been repaired."; };
			fuel = 100;
			health = 100;

			if (getRandomNumber() < 50) { state = TaxiOut; };
		}
		//TaxiOut
		else if (state == TaxiOut) {
			message = "The plane is taxiing to the runway.";
			state = TakeOff;
		}
		//TaxiIn
		else if (state == TaxiIn) {
			message = "The plane is taxiing to the hangar.";
			state = Parked;
		}
		//TakeOff
		else if (state == TakeOff) {
			message = "The plane is taking off.";
			state = Fly;
		}
		//Fly
		else if (state == Fly) {
			message = "The plane is flying around.";
			fuel -= getRandomNumber(1,5);

			//Less fuel and health == more likely to land to refuel and repair
			if (getRandomNumber() > (fuel + health)/2) { state = Land; };

			if (getRandomNumber() < 20) { state = Alert; };

			if (fuel <= 0) { state = Crash; };

		}
		//Land
		else if (state == Land) {
			message = "The plane is coming in to land.";
			state = TaxiIn;	

			//Less health == more likely to crash when landing
			if (getRandomNumber() > health) { state = Crash; };
		}
		//Crash
		else if (state == Crash) {
			message = "The plane crashes and explodes.";
			fuel = 0;
			health = 0;
			gameOver = true;
		}
		//Alert
		else if (state == Alert) {
			message = "The plane spots an enemy plane.";
			//Less fit for battle == more likely to try escape
			if (getRandomNumber() > (fuel + health) / 2) { state = Escape; }
			else { state = Battle; };
		}
		//Battle
		else if (state == Battle) {
			message = "The plane engages the enemy plane.";
			fuel -= getRandomNumber(1, 5);

			if (getRandomNumber() < 50) {
				state = Attack;
			} else {
				state = Evade;
			}

			//Less fit for battle == more likely to try escape
			if (getRandomNumber() > (fuel + health) / 2) { state = Escape; }

			if (fuel <= 0) { state = Crash; };
		}
		//Escape
		else if (state == Escape) {
			message = "The plane attempts to escape from the enemy.";

			//More health == more likely to escape
			if (getRandomNumber() < health / 2) { 
				message += "\nIt does so successfully";
				state = Fly;
			} else {
				message += "\nIt fails to do so";
				state = Hit;
			}
		}
		//Hit
		else if (state == Hit) {
			message = "The plane is hit by the enemy.";
			health -= getRandomNumber(10, 20);

			state = Battle;

			if (health <= 0) { state = Crash; };
		}
		//Attack
		else if (state == Attack) {
			message = "The plane opens fire on the enemy.";
			if (getRandomNumber() < health / 2) {
				message += "\nIt hits.";
				if (getRandomNumber() < 10) {
					message += "\nThe enemy is destroyed";
					state = Fly;
				}
				else {
					state = Battle;
				}
			} else {
				message += "\nIt misses.";
				state = Battle;
			}
		}
		//Evade
		else if (state == Evade) {
			message = "The plane attempts to evade the enemy's attacks.";
			if (getRandomNumber() < health / 2) {
				message += "\nIt succeeds";
				state = Battle;
			} else {
				message += "\nIt fails.";
				state = Hit;
			}
		}	

		return message;
	};
};

Plane myPlane;

void nextTurn() {
	string message = myPlane.UpdateState();
	cout<< message
		<< "\nHealth: " << myPlane.health
		<< "\nFuel: " << myPlane.fuel
		<< "\n--------";
	cin.ignore();
	system("CLS");

	if (!gameOver) {
		nextTurn();
	} else {
		cout << "Game Over.";
		cin.ignore();
	}
};

int main() {
	srand(time(NULL));
	nextTurn();
};


