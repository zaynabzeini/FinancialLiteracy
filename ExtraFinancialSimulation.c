#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FinancialSimulation.h"

void makePayment(financialIdentity *person, double paymentAmount) {
  if (paymentAmount <= person->checking) {
    person->checking -= paymentAmount;
  }
  else {
    paymentAmount -= person->checking;
    person->checking = 0.0;
    person->savings -= paymentAmount;
  }
}

void savingsPlacement(financialIdentity *person, double interestRate) {
  person->savings = (person->savings * (1 + interestRate));
}

void debt(financialIdentity *person, double interestRate, double addlPay){

  double minPercent = 0.03;

  if (person->debt > 0.0) {
    person->yearsWithDebt++;
  }

  for (int i=0; i<12; i++) {
    if (person->debt > 0.0) {
      double payment = addlPay + (person->debt * minPercent);
      if (payment > person->debt) {
        payment = person->debt;
      }
      makePayment(person, payment);
      person->debt -= payment;
      person->debtPaid += payment;
    }
  }
  person->debt = (person->debt * (1 + interestRate));
}


void rent(financialIdentity *person, double rentAmount) {
  for (int i=0; i<12; i++) {
    makePayment(person, rentAmount);
  }
  person->yearsRented++;
}


void house(financialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm) {

  double numberPayments = mortgageTerm * 12;
  double monthlyInterest = interestRate / 12;
  double discountFactor = (pow(1+monthlyInterest, numberPayments) - 1) / (monthlyInterest * pow(1+monthlyInterest, numberPayments));
  double monthlyPayment = mortgageAmount / discountFactor;

  for(int i=0; i<12; i++) {
    if (person->mortgage > 0.0) {
      if (monthlyPayment > person->mortgage) {
        monthlyPayment = person->mortgage;
      }
      makePayment(person, monthlyPayment);
      person->mortgage -= monthlyPayment;
    }
    else {
      person->mortgage = 0.0;
      person->ownsHouse = true;
    }
  }
  person->mortgage = person->mortgage * (1 + interestRate);
  person->yearsPayingMortgage++;
}
//(financialIdentity *person, double yearlySalary, double rentCost, double houseCost, double mortgageInterestRate, bool financiallyLiterate) {
int* simulateExtra(financialIdentity *person, double yearlySalary, double rentCost, double houseCost, bool financiallyLiterate) {

  //declare all the values that are identical for both financiallyLiterate and notFinanciallyLiterate
  double debtInterestRate = 0.2;
  double savingsSalary = yearlySalary * 0.2;
  double checkingSalary = yearlySalary * 0.3;
  int mortgageYears = 30;

  //make two separate decisions for f1 and nf1
  double debtAddlPay;
  double savingsInterestRate;
  double downPayment;
  double mortgageInterestRate;


  if (financiallyLiterate == true) {
    debtAddlPay = 100.0;
    savingsInterestRate = 0.07;
    downPayment = houseCost * 0.2;
    mortgageInterestRate = 0.045;
  }
  else {
    debtAddlPay = 1.0;
    savingsInterestRate = 0.01;
    downPayment = houseCost * 0.05;
    mortgageInterestRate = 0.05;
  }

  static int wealthArray[40];
  int wealth;
  int year = 0;

  while (year == 0) {
    wealth = person->checking + person->savings - person->debt - person->mortgage;
    wealthArray[year] = wealth;
    year++;
  }


  while (person->savings < downPayment) { //keep running until they have enough savings
    rent(person, rentCost);
    person->savings += savingsSalary;
    person->checking += checkingSalary;
    savingsPlacement(person, savingsInterestRate);
    debt(person, debtInterestRate, debtAddlPay);
    wealth = person->checking + person->savings - person->debt - person->mortgage;
    wealthArray[year] = wealth;
    year++;
  }

  person->savings -= downPayment;
  double mortgageAfterDownPayment = houseCost - downPayment;
  person->mortgage = mortgageAfterDownPayment;

  while (person->ownsHouse == false) {
    house(person, mortgageAfterDownPayment, mortgageInterestRate, mortgageYears);
    person->savings += savingsSalary;
    person->checking += checkingSalary;
    savingsPlacement(person, savingsInterestRate);
    debt(person, debtInterestRate, debtAddlPay);
    wealth = person->checking + person->savings - person->debt - person->mortgage;
    wealthArray[year] = wealth;
    year++;
  }

  while (year <= 40) {
    person->savings += savingsSalary;
    person->checking += checkingSalary;
    savingsPlacement(person, savingsInterestRate);
    debt(person, debtInterestRate, debtAddlPay);
    wealth = person->checking + person->savings - person->debt - person->mortgage;
    wealthArray[year] = wealth;
    year++;
  }

  return wealthArray;
}

void printPerson(financialIdentity *person, FILE* designatedOutput) {
  fprintf(designatedOutput, "TOTAL SAVINGS: $%.2f\n", person->savings);
  fprintf(designatedOutput, "DEBT PAID: $%.2f\nYEARS WITH DEBT: %d years\n", person->debtPaid, person->yearsWithDebt);
}


int main(int argc, char* argv[]) {

  //opens file to read values for financially literate FILE ONE
  FILE* inputFile = fopen(argv[1], "r");
  if (inputFile  == NULL) { // Error handling
      printf("Could not open input file.\n");
      exit(1);
  }
  //creates variables to assign the values (from the file) to
  double inputSavings;
	double inputChecking;
	double inputDebt;
	double inputMortgage;
	int inputYearsWithDebt;
	int inputYearsRented;
	double inputDebtPaid;
  int inputYearsPayingMortgage;
	int inputOwnsHouse; //has to be 0 (false) or 1 (true)
  int inputFinanciallyLiterate;
  bool compareFinanciallyLiterateOrNot;

  //scans the file and assigns to variables
  fscanf(inputFile, "%lf %lf %lf %lf %d %d %lf %d %d %d", &inputSavings, &inputChecking, &inputDebt, &inputMortgage, &inputYearsWithDebt, &inputYearsRented, &inputDebtPaid, &inputYearsPayingMortgage, &inputOwnsHouse, &inputFinanciallyLiterate);
  fclose(inputFile);

  financialIdentity flCompare; //creates a struct for a financially literate person that will be used to compare
  flCompare.savings = inputSavings;
  flCompare.checking = inputChecking;
  flCompare.debt = inputDebt;
  flCompare.mortgage = inputMortgage;
  flCompare.yearsWithDebt = inputYearsWithDebt;
  flCompare.yearsRented = inputYearsRented;
  flCompare.debtPaid = inputDebtPaid;
  flCompare.yearsPayingMortgage = inputYearsPayingMortgage;
  if (inputOwnsHouse == 0) { //integer must be converted to boolean
    flCompare.ownsHouse = false;
  }
  else {
    flCompare.ownsHouse = true;
  }
  if (inputFinanciallyLiterate == 0) {
    compareFinanciallyLiterateOrNot = false;
  }
  else {
    compareFinanciallyLiterateOrNot = true;
  }

  //output to the file for default financially literate (middle)
  FILE* outputFile = fopen(argv[2], "w");
  if (outputFile == NULL) {
    printf("Could not open output file.\n");
    exit(1);
  }
  fprintf(outputFile, "Individual With Higher Salary: Project Default\n");
  int* flMiddleWealth = simulateExtra(&flCompare, 69000.0, 950.0, 210000.0, compareFinanciallyLiterateOrNot);
  for(int i=0; i<=40; i++) {
    fprintf(outputFile, "Year: %d Wealth: $%d\n", i, flMiddleWealth[i]);
  }
  printPerson(&flCompare, outputFile);
  fclose(outputFile);


  FILE* inputFileCompare = fopen(argv[3], "r");
  if (inputFileCompare == NULL) { // Error handling
      printf("Could not open input file.\n");
      exit(1);
  }
  //creates variables to assign the values (from the file) to
  double inputSavingsMin;
	double inputCheckingMin;
	double inputDebtMin;
	double inputMortgageMin;
	int inputYearsWithDebtMin;
	int inputYearsRentedMin;
	double inputDebtPaidMin;
  int inputYearsPayingMortgageMin;
	int inputOwnsHouseMin; //has to be 0 (false) or 1 (true)
  int secondFinanciallyLiterate;
  bool secondFinanciallyLiterateOrNot;

  //scans the file and assigns to variables
  fscanf(inputFileCompare, "%lf %lf %lf %lf %d %d %lf %d %d %d", &inputSavingsMin, &inputCheckingMin, &inputDebtMin, &inputMortgageMin, &inputYearsWithDebtMin, &inputYearsRentedMin, &inputDebtPaidMin, &inputYearsPayingMortgageMin, &inputOwnsHouseMin, &secondFinanciallyLiterate);
  fclose(inputFileCompare);

  financialIdentity flCompareMin;
  flCompareMin.savings = inputSavings;
  flCompareMin.checking = inputChecking;
  flCompareMin.debt = inputDebt;
  flCompareMin.mortgage = inputMortgage;
  flCompareMin.yearsWithDebt = inputYearsWithDebt;
  flCompareMin.yearsRented = inputYearsRented;
  flCompareMin.debtPaid = inputDebtPaid;
  flCompareMin.yearsPayingMortgage = inputYearsPayingMortgage;
  if (inputOwnsHouse == 0) {
    flCompareMin.ownsHouse = true;
  }
  else {
    flCompareMin.ownsHouse = false;
  }
  if (secondFinanciallyLiterate == 0) {
    secondFinanciallyLiterateOrNot = false;
  }
  else {
    secondFinanciallyLiterateOrNot = true;
  }  


  FILE* outputFileCompare = fopen(argv[4], "w");
  if (outputFileCompare == NULL) {
    printf("Could not open output file.\n");
    exit(1);
  }
  fprintf(outputFileCompare, "Individual With Lower Salary\n");
  int* flCompareWealth = simulateExtra(&flCompareMin, 38000.0, 1174.0, 243094.0, secondFinanciallyLiterateOrNot);
  for(int i=0; i<=40; i++) {
    fprintf(outputFileCompare, "Year: %d Wealth: $%d\n", i, flCompareWealth[i]);
  }
  printPerson(&flCompareMin, outputFileCompare);
  fclose(outputFileCompare);

  return 0;
}
