// mac937@nyu.edu
// hw12 

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

//DISPLAY 11.9 Program Using an Array of the Money Object
//This is the definition for the class Money.
//Values of this type are amounts of money in U.S. currency.
class Money
{
public:
    friend Money operator +(const Money& amount1, const Money& amount2);
    //Returns the sum of the values of amount1 and amount2.

    friend Money operator -(const Money& amount1, const Money& amount2);
    //Returns amount 1 minus amount2.

    friend Money operator -(const Money& amount);
    //Returns the negative of the value of amount.

    friend bool operator ==(const Money& amount1, const Money& amount2);
    //Returns true if amount1 and amount2 have the same value; false otherwise.

    friend bool operator < (const Money& amount1, const Money& amount2);
    //Returns true if amount1 is less than amount2; false otherwise.

    Money(long dollars, int cents);
    //Initializes the object so its value represents an amount with
    //the dollars and cents given by the arguments. If the amount
    //is negative, then both dollars and cents should be negative.

    explicit Money(long dollars);
    //Initializes the object so its value represents $dollars.00.

    Money( );
    //Initializes the object so its value represents $0.00.

    //double get_value( ) const;
    //Returns the amount of money recorded in the data portion of the calling
    //object.

    friend istream& operator >>(istream& ins, Money& amount);
    //Overloads the >> operator so it can be used to input values of type
    //Money. Notation for inputting negative amounts is as in -$100.00.
    //Precondition: If ins is a file input stream, then ins has already been
    //connected to a file.


    friend ostream& operator <<(ostream& outs, const Money& amount);
    //Overloads the << operator so it can be used to output values of type
    //Money. Precedes each output value of type Money with a dollar sign.
    //Precondition: If outs is a file output stream, then outs has already been
    //connected to a file.
private:
    long all_cents;
};

class Check
{
public:
    Check(int number, const Money& amount, bool cashed);
    Check();
    void set_number(int number_);
    void set_amount(const Money& amount_);
    void set_cashed(bool cashed_);
    int get_number() const;
    Money get_amount() const;
    bool is_cashed() const;
    void print(ostream& out) const;
    void read(istream& in);
    friend ostream& operator<<(ostream& out, const Check& check);
    friend istream& operator>>(istream& in, Check& check);
private:
    int number;
    Money amount;
    bool cashed;
};

void readChecks(Check checks[], int n, ostream& prompt, istream& in) {
    for (int i = 0; i < n; i++) {
        prompt << "Enter check number, amount, and cashedness (0 or 1): ";
        in >> checks[i];
    }
}

void readDeposits(Money deposits[], int n, ostream& prompt, istream& in) {
    for (int i = 0; i < n; i++) {
        prompt << "Enter deposit " << (i + 1) << " amount: ";
        in >> deposits[i];
    }
}

void populateCheckVector(Check source[], int numSourceChecks, vector<int>& check_positions, bool cashed) {
    for (int i = 0; i < numSourceChecks; i++) {
        if (source[i].is_cashed() == cashed) {
            size_t j;
            for (j = 0; j < check_positions.size(); j++) {
                if (source[check_positions[j]].get_number() > source[i].get_number()) {
                    break;
                }
            }
            check_positions.insert(check_positions.begin() + j, i);
        }
    }
}

void printCashedAndPendingChecks(Check checks[], int n, ostream& out) {
    vector<int> cashed;
    vector<int> pending;
    populateCheckVector(checks, n, cashed, true);
    populateCheckVector(checks, n, pending, false);
    out << endl;
    out << "Cashed checks:" << endl;
    for (int pos : cashed) {
        out << checks[pos] << endl;
    }
    out << endl;
    out << "Pending checks:" << endl;
    for (int pos : pending) {
        out << checks[pos] << endl;
    }
    out << endl;
}

Money calcNetChange(const Check checks[], int numChecks, const Money deposits[], int numDeposits) {
    Money sum(0);
    for (int i = 0; i < numDeposits; i++) {
        sum = sum + deposits[i];
    }
    for (int i = 0; i < numChecks; i++) {
        if (checks[i].is_cashed()) {
            sum = sum - checks[i].get_amount();
        }
    }
    return sum;
}

int main() {
    int numChecks, numDeposits;
    Money oldBalance;
    Money newBalance;
    cout << "How many checks written? ";
    cin >> numChecks;
    Check* checks = new Check[numChecks]; 
    readChecks(checks, numChecks, cout, cin);
    cout << "How many deposits? ";
    cin >> numDeposits;
    Money* deposits = new Money[numDeposits];
    readDeposits(deposits, numDeposits, cout, cin);
    printCashedAndPendingChecks(checks, numChecks, cout);
    cout << "Enter old balance: ";
    cin >> oldBalance;
    cout << "Enter new balance: ";
    cin >> newBalance;
    Money expectedBalance = oldBalance + calcNetChange(checks, numChecks, deposits, numDeposits);
    cout << "Unreconciled funds: " << (expectedBalance - newBalance) << endl;
    delete[] checks;
    delete[] deposits;
    return 0;
}

Money operator +(const Money& amount1, const Money& amount2)
{
    Money temp;
    temp.all_cents = amount1.all_cents + amount2.all_cents;
    return temp;
}

bool operator ==(const Money& amount1, const Money& amount2)
{
    return (amount1.all_cents == amount2.all_cents);
}

Money operator -(const Money& amount1, const Money& amount2)
{
    Money temp;
    temp.all_cents = amount1.all_cents - amount2.all_cents;
    return temp;
}

Money operator -(const Money& amount)
{
    Money temp;
    temp.all_cents = -amount.all_cents;
    return temp;
}


int digit_to_int(char c)
{
    return ( static_cast<int>(c) - static_cast<int>('0') );
}

istream& operator >>(istream& ins, Money& amount)
{
    char one_char, decimal_point,
            digit1, digit2; //digits for the amount of cents
    long dollars;
    int cents;
    bool negative;//set to true if input is negative.

    ins >> one_char;
    if (one_char == '-')
    {
        negative = true;
        ins >> one_char; //read '$'
    }
    else
        negative = false;
    //if input is legal, then one_char == '$'

    ins >> dollars >> decimal_point >> digit1 >> digit2;

    assert(!( one_char != '$' || decimal_point != '.'
         || !isdigit(digit1) || !isdigit(digit2) ));

    cents = digit_to_int(digit1)*10 + digit_to_int(digit2);

    amount.all_cents = dollars*100 + cents;
    if (negative)
        amount.all_cents = -amount.all_cents;


    return ins;
}


//Uses cstdlib and iostream:
ostream& operator <<(ostream& outs, const Money& amount)
{
    long positive_cents, dollars, cents;
    positive_cents = labs(amount.all_cents);
    dollars = positive_cents/100;
    cents = positive_cents%100;

    if (amount.all_cents < 0)
        outs << "-$" << dollars << '.';
    else
        outs << "$" << dollars << '.';

    if (cents < 10)
        outs << '0';
    outs << cents;

    return outs;
}

Money::Money(long dollars, int cents) 
        : all_cents(dollars * 100 + cents) {
    assert((dollars >= 0 && cents >= 0) || (dollars <= 0 && cents <= 0));
}

Money::Money(long dollars) 
        : Money(dollars, 0) 
{
}

Money::Money() 
        : Money(0) 
{
}

bool operator<(const Money &amount1, const Money &amount2) {
    return amount1.all_cents < amount2.all_cents;
}

Check::Check(int number, const Money &amount, bool cashed)
        : number(number), amount(amount), cashed(cashed)
{

}

Check::Check()
        : Check(0, Money(0), false)
{

}

void Check::set_number(int number_) {
    number = number_;
}

void Check::set_amount(const Money &amount_) {
    amount = amount_;
}

void Check::set_cashed(bool cashed_) {
    cashed = cashed_;
}

int Check::get_number() const {
    return number;
}

Money Check::get_amount() const {
    return amount;
}

bool Check::is_cashed() const {
    return cashed;
}

void Check::print(ostream &out) const {
    out << number << ' ' << amount << ' ';
    if (cashed) {
        out << "cashed";
    } else {
        out << "pending";
    }
}

void Check::read(istream &in) {
    int cashedInt;
    in >> number >> amount >> cashedInt;
    cashed = cashedInt != 0;
}

ostream &operator<<(ostream &out, const Check &check) {
    check.print(out);
    return out;
}

istream &operator>>(istream &in, Check &check) {
    check.read(in);
    return in;
}

