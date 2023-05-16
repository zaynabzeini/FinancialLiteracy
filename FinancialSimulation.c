#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FinancialSimulation.h"

//MAKEPAYMENT FUNCTION: makes payments by using checking account to pay the diff or dipping into savings account if needed
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


//SAVINGSPLACEMENT FUNCTION: modifies that struct to reflect that person’s savings after one year.
void savingsPlacement(financialIdentity *person, double interestRate) {
  person->savings = (person->savings * (1 + interestRate));
}


//DEBT FUNCTION: To apply payments, update the debt,  debtPaid, and any other relevant entries per month
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


//RENT FUNCTION: updates the amount of money a person will have left in their checking and savings accounts after one year
void rent(financialIdentity *person, double rentAmount) {
  for (int i=0; i<12; i++) {
    makePayment(person, rentAmount);
  }
  person->yearsRented++;
}

//HOUSE FUNCTION: To apply mortgage payments, update the mortgage and any other relevant entries per month
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

int* simulate(financialIdentity *person, double yearlySalary, bool financiallyLiterate) {

  //declare all the values that are identical for both financiallyLiterate and notFinanciallyLiterate
  double debtInterestRate = 0.2;
  double savingsSalary = yearlySalary * 0.2;
  double checkingSalary = yearlySalary * 0.3;
  double rentCost = 950.0;
  int mortgageYears = 30;
  double houseCost = 210000.0;

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

  while (person->ownsHouse == false) { //keep running until they own the house
    house(person, mortgageAfterDownPayment, mortgageInterestRate, mortgageYears);
    person->savings += savingsSalary;
    person->checking += checkingSalary;
    savingsPlacement(person, savingsInterestRate);
    debt(person, debtInterestRate, debtAddlPay);
    wealth = person->checking + person->savings - person->debt - person->mortgage;
    wealthArray[year] = wealth;
    year++;
  }

  while (year <= 40) { //run until you reach the end of the simulation
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
  fprintf(designatedOutput, "YEARS PAYING RENT: %d\nYEARS PAYING MORTGAGE: %d\n", person->yearsRented, person->yearsPayingMortgage);
  fprintf(designatedOutput, "DEBT PAID: $%.2f\nYEARS WITH DEBT: %d years\n", person->debtPaid, person->yearsWithDebt);
}


int main(int argc, char* argv[]) {
            //FL OUTPUT FILE
  FILE* outputFileFL = fopen(argv[1], "w");
  if (outputFileFL == NULL) {
    printf("Could not open output file.\n");
    exit(1);
  }

  financialIdentity fl = {
    5000.0,
    0.0,
    52500.0,
    0.0,
    0,
    0,
    0.0,
    0,
    false,
  } ;

  fprintf(outputFileFL, "Financially Literate Individual\n");
  int* flWealth = simulate(&fl, 69000.0, true);
  for(int i=0; i<=40; i++) {
    fprintf(outputFileFL, "Year: %d Wealth: $%d\n", i, flWealth[i]);
  }
  printPerson(&fl, outputFileFL);
  fclose(outputFileFL);


      //NFL OUTPUT FILE
  FILE* outputFileNFL = fopen(argv[2], "w");
  if (outputFileNFL == NULL) {
    printf("Could not open output file.\n");
    exit(1);
  }

  financialIdentity nfl = {
    5000.0,
    0.0,
    52500.0,
    0.0,
    0,
    0,
    0.0,
    0,
    false,
  } ;

  fprintf(outputFileNFL, "Non-financially Literate Individual\n");
  int* nflWealth = simulate(&nfl, 69000.0, false);
  for(int i=0; i<=40; i++) {
    fprintf(outputFileNFL, "Year: %d Wealth: $%d\n", i, nflWealth[i]);
  }
  printPerson(&nfl, outputFileNFL);
  fclose(outputFileNFL);

  return 0;
}
