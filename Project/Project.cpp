#include <iostream>
#include <ctime>

using namespace std;

//declare constant 
const int gene = 15; //chromosome representation
const int budget = 25000; //budget for the relief (must not exceed)
const int damageSeverity[gene] = { 3, 1, 3, 3, 3, 1, 3, 2, 2, 3, 2, 3, 1, 3, 2 }; //damage severity for each area
const float highRiskRatio[gene] = { 0.2, 0.25, 0.2, 0.5, 0.2, 0.125, 0.125, 0.1, 0.2, 0.25, 0.125, 0.2, 0.125, 0.25, 0.125 }; //high risk ratio for each area
const int reliefCost[gene] = { 3000, 3000, 2500, 4600, 3200, 5430, 4300, 4100, 3300, 3200, 2000, 3200, 2400, 2470, 3300 }; //relief cost for each area
const int damageSum = 35; //total damage sum
const int reliefSum = 50000; //total relief sum
const float highRiskSum = 2.975; //total high risk sum
const int population = 10; //population size

//declare required variables
int chromosome[population][gene]; //chromosome data structure
float fitness[population]; //fitness data structure

void initializePopulation() {

	//DEBUG
	cout << "\x1B[93mEvaluation of population\033[0m\n\n";

	//for every chromosome in the population
	for (int c = 0; c < population; c++) {
		//for every gene in the chromosome
		for (int g = 0; g < gene; g++) {
			//randomly generate gene value
			chromosome[c][g] = rand() % 2;
		}
	}

	//print the chromosome - DEBUG
	for (int c = 0; c < population; c++) {
		cout << "\tChromosome " << c + 1 << ": ";
		for (int g = 0; g < gene; g++) {
			cout << chromosome[c][g] << " ";
		}
		cout << endl;
	}
}

void evaluateChromosome() {

	//declare constant
	const float damageWeight = 0.2; //damage weight
	const float highRiskWeight = 0.5; //relief weight

	//declare required variables
	int damageTotal;
	float highRiskTotal, reliefTotal;

	//DEBUG
	cout << "\n\x1B[93mEvaluation of population\033[0m\n\n";

	//for every chromosome in the population
	for (int c = 0; c < population; c++) {

		//initialize total value
		damageTotal = 0;
		highRiskTotal = 0;
		reliefTotal = 0;

		//for every gene in the chromosome
		for (int g = 0; g < gene; g++) {
			//calculate the total damage, relief, and high risk
			damageTotal += chromosome[c][g] * damageSeverity[g];
			highRiskTotal += chromosome[c][g] * highRiskRatio[g];
			reliefTotal += chromosome[c][g] * reliefCost[g];
		}

		//calculate the fitness value
		//if the relief total exceed the budget, fitness value is halved
		if (reliefTotal > budget) {

			fitness[c] = ((damageWeight * damageTotal / damageSum) + (highRiskWeight * highRiskTotal / highRiskSum) + (reliefTotal / reliefSum)) * 0.5;
		}
		else {
			fitness[c] = (damageWeight * damageTotal / damageSum) + (highRiskWeight * highRiskTotal / highRiskSum) + (reliefTotal / reliefSum);
		}

		//print the fitness value - DEBUG
		cout << "\tChromosome " << c + 1 << " - fitness: " << fitness[c] << endl;
	}
}

int main() {
	srand(time(NULL));
	initializePopulation();
	evaluateChromosome();
	return 0;
}
