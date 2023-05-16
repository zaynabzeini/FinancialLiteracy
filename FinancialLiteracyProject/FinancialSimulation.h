#include <stdio.h>
#include <stdbool.h>

typedef struct financialIdentity_struct {
  double savings;
	double checking;
	double debt;
	double mortgage;
	int yearsWithDebt;
	int yearsRented;
	double debtPaid;
  int yearsPayingMortgage;
	bool ownsHouse;
} financialIdentity;

financialIdentity fl;
financialIdentity nfl;
financialIdentity flCompare;
financialIdentity flCompareMin;

void makePayment(financialIdentity *person, double paymentAmount);

void savingsPlacement(financialIdentity *person, double interestRate);

void debt(financialIdentity *person, double interestRate, double addlPay);

void rent(financialIdentity *person, double rentAmount);

void house(financialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm);

int* simulate(financialIdentity *person, double yearlySalary, bool financiallyLiterate);
 
int* simulateExtra(financialIdentity *person, double yearlySalary, double rentCost, double houseCost, bool financiallyLiterate);
