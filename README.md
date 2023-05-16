README Financial Literacy Project


MAIN PROGRAM:


- Explanation of Program:
  - This program is a simulation over 40 years of two individuals: one who is financially literate and one who is not. The program accounts for the individuals' savings account, checking account, mortgage, debt from credit cards and student loans, total years with debt, total debt paid, total years renting, and ownership over a house.
  - Both individuals begin with the same amount of money and debt, have the same yearly salary, rent an apartment of the same cost, and buy a house of the same cost with the same mortgage term.
  - However, they differ in key ways. The financially literate individual puts their money in a mutual interest fund, while the non-financially literate has theirs in a simple savings account. Additionally, the financially literate pays an additional amount for their debt payments and a mortgage interest rate free of private mortgage insurance, while the non-financially literate pays a dollar in additional money and has a mortgage interest rate with private mortgage insurance on the cost.
  - The program accounts for their wealth in each of the 40 years the simulation is running.


- Known Extensions and Limitations:
  - While this program is able to do a lot in simulating wealth accumulated over 40 years, there are known extensions and limitations. This is a an extreme simplification of one's financial situation: there is a constant salary, no substantial life changes, etc.


- Instructions to Run Program:
  - To run this program, the user must open the terminal and navigate to the accurate directory. Once there, the user must compile the program through the compilation line as follows:
                            gcc FinancialSimulation.c -o FinancialSimulation
  - Then, the user should proceed to run the following command line:
                            ./FinancialSimulation output_fl.txt output_nfl.txt
  - Here, "output_fl.txt" refers to the output file created in which the wealth of the financially literate individual is displayed, while "output_nfl.txt" displays the wealth over 40 years of the non-financially literate individual.  



EXTRA CREDIT PROGRAM:

- Extra Credit can be found in the ExtraFinancialSimulation.c file.


- Explanation of Program:
  - This extra credit looks at the limits of financial literacy; I created a program to compare two individuals that differ in their economic status and placement. My extra credit also consisted of adding another struct member for the years spent paying the mortgage–originally 0 for all individuals and updated throughout
  - The standard values that will be used to display differing economic status includes the individual as described in the original project–one with a $69,000 yearly salary, $950 rent, and $210,000 house–to serve as the individual with greater economic status.
  - To find the specific values for the individual of lower economic status, I chose New Orleans as the city to base my values on, as it is the city we live in and is the 50th most populated city in the United States. The average rent cost for an apartment in New Orleans is $1,174, while the average house cost is $1,174. The yearly salary for a full-time minimum wage worker is $15,080–both nationally and in Louisiana–but the program has its limits: it is unequipped to take in a yearly salary that low, producing a “Bus Error: 10”.  Because of this, the yearly salary for this individual is $38,000, which is the lowest salary possible for the program to still run and about $10,000 than the median income in New Orleans.


- Instructions to Run Program:
  - To compile the program, one must navigate to the correct directory that holds the desired input files and open the terminal. There, the user must run the following compilation line:
                          gcc ExtraFinancialSimulation.c -o ExtraFinancialSimulation
  - To run the program, the user must have created two input files with the desired values following the specified format below. They then should run the following command line:
              ./ExtraFinancialSimulation input_Default.txt output_Default.txt input_Compare.txt output_Compare.txt
  - The first argument of the command line ("input_Default.txt") is the name of the input file for the first individual–the default, with a higher salary–followed by the second argument ("output_Default.txt") that has the name of the file to which their wealth is output to. The third argument ("input_Compare.txt") is the name of the input file for the second individual–with a lower salary–followed by the fourth argument (output_Compare.txt) that has the name of the second individual’s output file.
  - This code takes in 2 input files and outputs 2 files–all of which are text (.txt) files. Each input text file must be formatted with the initial information about the individual–including whether they own a house and are financially literate. The format is as follows:
                savings (decimal)
                checking (decimal)
                debt (decimal)
                mortgage (decimal)
                years with debt (integer)
                years renting (integer)
                the debt paid (decimal)
                years paying mortgage (integer)
                whether the individual owns a house (integer)
                whether the individual is financially literate or not (integer)
  - Every value must be an integer or a decimal–as denoted in the above outline–including whether the individual owns a house and are financially literate. 0 will be put in for the boolean false (if they don’t own the house or if they are not literate) and 1 will be put in the place of the boolean true (if they do own the house or are financially literate).
