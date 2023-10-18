#include <iostream>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <io.h>
#include <exception>
#include <algorithm>
using namespace std;

enum Color
{
	Black, Blue, Green, Cyan, Red, Magenta, Brown,
	LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
void setColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Esc = 27 };
void setCursor(int x, int y)
{
	COORD myCoords = { x,y };
	SetConsoleCursorPosition(hStdOut, myCoords);
}

/*
Описать класс БАНК, который будет работаться с динамическим массивом клиентов.
Каждый клиент представляет собой набор полей - имя, id и денежный балланс.
Реализовать работу используя кольцевую очередь с приоритетом.
Когда клиент попадает в "обработку" ему начисляются прценты. к пример 2%.
Выводится сообщение - "имя клиента начисленны проценты".
1 вариант: После чего клиент отправляется в хвост очереди
2 вариант: запоминаем id обработанных клиентов и при выборе следующего наивысшего приоритета
пропускаем "обработанных" клиентов.

Реализовать методы:
Показа очереди
Добавления элемента в очередь
Удаления элемента очереди на тот случай. если его балланс стал равен 0
Изменение балланса пользователя - имитировать растраты и пополнение счёта.
Добавляется и отнимается случайное значение.
Начисление роцентов, согласно приоритету - т.е. у кого больший денежный балланс - того
"обрабатывают" первым.
*/

/*
Describe the BANK class that will work with a dynamic array of clients.
Each client is a set of fields - name, id and cash balance.
Implement work using a priority ring queue.
When a client gets into "processing" he gets interest. for example 2 % .
A message is displayed - "client name interest accrued".
Option 1 : After that, the client goes to the tail of the queue
Option 2 : remember the id of processed clients and when choosing the next highest priority
we pass "processed" clients.

Implement methods :
Show Queue
Adding an item to the queue
Removing the queue element just in case. if his balance became equal to 0
Changing the user's balance - simulate spending and replenishment of the account.
Adding and subtracting a random value.
Interest accrual according to priority - i.e.who has a larger cash balance - that
"process" first.
*/

class Customer
{
private:
	string Name;
	int ID;
	double balance;
	//static components are components. which are available WITHOUT creating an instance of the class
	//static fields are common (shared) for all instances of this class
	static int counter; /*They can not be multiple copies of the same static variables for different objects.
	Also because of this reason static variables can not be initialized using constructors.*/
	static int totalCounter;
public:
	Customer()
	{
		counter++;
		totalCounter++;
		ID = counter + 1;
		Name = "Tommy "; //first initiallize our string name
		Name.append(to_string(ID));// append(to_string(ID)) is appending ID(no) to name Sasha!!to_string convert integer to string
		balance = 1000 + rand() % 10001 + (rand() % 100) / 100.0;

	}
	Customer(string Name, int ID, const double balance)
	{
		counter++;
		totalCounter++;
		this->Name = Name;
		//this->ID = ID;
		this->ID = counter + 1;
		this->balance = balance;

	}
	~Customer()
	{
		//cout << "See You Again Soon" << endl;
		totalCounter--;
	}
	static int getCounter()
	{
		//within static methods, ONLY static fields can be accessed
		return counter;
	}
	void setName(string N)
	{
		Name = N;
	}
	void setID(const int id)
	{
		ID = id;
	}
	void changeBalance(const double change)
	{
		balance += change;
	}

	string getName()const
	{
		return Name;
	}
	int getID()const
	{
		return ID;
	}
	double getBalance()const
	{
		return balance;
	}

	void printCustomerInfo()
	{
		cout << "ID: " << ID << endl;
		cout << "Customer Name: " << Name << endl;
		cout << "Customer Balance: " << balance << endl;
	}
};

class Bank
{
	vector <Customer> customer;
	vector <Customer> Happy_customer;
	vector <Customer> unHappy_customer;
	static double stavka;
public:
	void customerGenerator(const int number_Of_Customer)
	{
		for (size_t i = 0; i < number_Of_Customer; i++)
		{
			customer.emplace_back(Customer());//generator random customer from our default constructor
		}
	}
	//adding my customer 
	void addCustomer()
	{
		customer.push_back(Customer()); //adding more customer using of default info in the customer class
		sortCustomer(); //after adding customer we sort immediately
	}

	/*vector<Customer> getCustomer()
	{
		return customer;
	}*/

	/*Customer getcustomers(const int index)
	{
		return customer[index];
	}*/

	void showBankinfo()
	{
		for (auto var : customer)
		{
			var.printCustomerInfo();
		}
	}

	static bool checkStavka(Customer rich, Customer poor)//checking who is poor or rich
	{
		return rich.getBalance() > poor.getBalance(); //rich.getbalance meaning we are rich out to customer to check the balance of our customer and sort later 
	}
	void sortCustomer()
	{
		sort(customer.begin(), customer.end(), checkStavka);//sorting our customer using standard libriary iterator
	}


	Customer getunHappyCustomer() //processes of checking the stavka of customer 
	{
		Customer tempCustomers; //creating new customer
		if (customer.empty())
		{
			sortCustomer();
			//do a check - if the size of the vector with clients is empty, then you need to find out if the vector with processed clients is full
			 //if it is full, then you need to return all clients to raw and sort
			if (customer.size() == 0)//checking to proof that the customer balance is greater than 0 balance;
			{
				tempCustomers = customer[0]; //checking the customer balance from zero
				//line 199 we change the customer balance added stavka to the customer 
				tempCustomers.changeBalance(tempCustomers.getBalance() * stavka);// checking the stavka of customer
				customer.erase(customer.begin());//deleting customer with zero acct
				unHappy_customer.push_back(tempCustomers); //pushing the new customer after checking his acct, so happy
			}
			return tempCustomers; //returning our happy customer
		}
		else
			throw exception("No zero customer acct");
	}

	Customer getHappyCustomer() //processes of checking the stavka of customer 
	{
		Customer tempCustomers; //creating new customer
		if (!customer.empty())
		{
			sortCustomer();
			//do a check - if the size of the vector with clients is empty, then you need to find out if the vector with processed clients is full
			 //if it is full, then you need to return all clients to raw and sort
			if (customer.size() > 0)//checking to proof that the customer balance is greater than 0 balance;
			{
				tempCustomers = customer[0]; //checking the customer balance from zero
				//line 199 we change the customer balance added stavka to the customer 
				tempCustomers.changeBalance(tempCustomers.getBalance() * stavka);// checking the stavka of customer
				customer.erase(customer.begin());//deleting customer with zero acct
				Happy_customer.push_back(tempCustomers); //pushing the new customer after checking his acct, so happy
			}
			return tempCustomers; //returning our happy customer
		}
		else
			throw exception("No zero customer acct");
	}

	// Метод для удаления клиента из очереди, если его баланс стал равен 0
	void RemoveCustomer()
	{
		Customer count;
		if (count.getBalance() < 0)
		{
			unHappy_customer.erase(unHappy_customer.begin(), unHappy_customer.end());
		}
		else
			throw exception(" No customer with zero acct balance");


	}
};
/* A static variable inside a class should be initialized explicitly by the user
using the class name and scope resolution operator outside the class as shown below: */
int Customer::counter = 0; //better to initialized figure outside of our class
int Customer::totalCounter = 0;//how many instances of this class currently exist
double Bank::stavka = 1.02;

void main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(0));

	int obj = 3;
	Bank newBank;

	try
	{
		for (size_t i = 0; i < obj; i++)
		{
			newBank.customerGenerator(obj);
		}

	}
	catch (const exception& ex)
	{
		ex.what();
		Sleep(500);
	}
	system("Pause");
	newBank.showBankinfo();
	try
	{
		cout << "CHECK HAPPY CUSTOMER AND LESS HAPPY CUSTOMER, SORT AND REMOVE ATLEAST 1 CUSTOMER \n";
		newBank.sortCustomer();
		newBank.getHappyCustomer();
		newBank.RemoveCustomer();
	}
	catch (const exception& ex)
	{
		ex.what();
		Sleep(600);
	}
	newBank.showBankinfo();
	system("Pause");
	try
	{
		cout << "ADDING 1 MORE CUSTOMER AND SORT FROM BIG ACCT BALANCE TO LOWEST\n";
		newBank.addCustomer();
		newBank.sortCustomer();
		newBank.showBankinfo();
		cout << "REMOVING CUSTOMER WITH ZERO ACCOUNT BALANCE\n";
		newBank.RemoveCustomer();
	}
	catch (const exception& ex)
	{
		ex.what();
		Sleep(600);
	}
	newBank.showBankinfo();
	system("Pause");
	cout << "NO CUSTOMER WITH ZERO ACCT BALANCE";




}







