#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <fstream>

using namespace std;



struct ClientAcc
{
	string AccountNumber;
	string Name;
	string Phone;

	short PinCode;
	double balance;
	bool MarkForDelete = false;
};

enum enAtmOperations { eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5 };



void QuickWithdraw(vector<ClientAcc>&, short);
void MainMenueScreen(vector<ClientAcc>);
bool SearchForAcc(vector<ClientAcc>&, string, ClientAcc&);
void ShowAccsesDeniedMessage();
void LoginScreen(vector<ClientAcc>&);
//@Files
const string Users_Filename = "users.txt";
const string FILENAME = "BanKAccountInfo.txt";

//Vaiable 
string GoBack = "\n\nPress Any Key To Go Back.....\n";
//CurrentUser




void ValidInput(int&);
void ValidInput(double&);
void ValidInput(char&);
void ValidInput(float&);

void AddToVector(vector<int>&);
ClientAcc ActiveClient;



// Input Functions
void ValidInput(int& number)
{

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid Input, Enter Valid Number: ";
		cin >> number;
	}
}
void ValidInput(double& number)
{

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid Input, Enter Valid Number: ";
		cin >> number;
	}
}
void ValidInput(float& number)
{

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid Input, Enter Valid Number: ";
		cin >> number;
	}
}

void ValidInput(char& ch)
{

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid Input, Enter Valid Char: ";
		cin >> ch;
	}
}

int ReadNumber(string msg = "Enter A Number: ")
{
	int number = 0;

	cout << msg;
	cin >> number;

	ValidInput(number);

	return number;
}

float ReadFloat(string msg = "Enter A Number: ")
{
	float number = 0;

	cout << msg;
	cin >> number;

	ValidInput(number);

	return number;
}

double ReadDouble(string msg = "Enter A Number: ")
{
	double number = 0;

	cout << msg;
	cin >> number;

	ValidInput(number);

	return number;
}

char ReadChar(string msg = "Enter Char: ")
{
	char c;
	cout << msg;
	cin >> c;
	ValidInput(c);
	return c;
}

string ReadText(string msg = "Enter Words: ")
{
	string words;
	cout << msg;
	getline(cin >> ws, words);
	return words;
}

void AddToVector(vector<int>& vNumber)
{
	char AddAgain = 'y';

	do
	{
		vNumber.push_back(ReadNumber());
		printf("Wanna To Add More Numbers? (y/n) \n");
		cin >> AddAgain;
		ValidInput(AddAgain);

	} while (AddAgain == 'y' || AddAgain == 'Y');
}

int ReadPostiveNumber(string msg = "Enter A Postive Number: ")
{
	int number = 0;
	do
	{
		cout << msg;
		cin >> number;

		ValidInput(number);

	} while (number < 0);

	return number;
}



vector<string> SpiltWords_Hadhoud(string S1, string delm = " ")
{
	vector<string> words;
	short pos = 0;
	string sWord = "";
	while ((pos = S1.find(delm)) != string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != " ")
		{
			words.push_back(sWord);
		}
		S1.erase(0, pos + delm.length());
	}
	if (sWord != "")
		words.push_back(S1);
	return words;
}



//Client Accounts Functions


bool ReadNewClient(vector<ClientAcc>& vAccInfo, ClientAcc& AccInfo)
{
	string AccNumber = ReadText("Account Number: ");
	/*string AccNumber = "Ad";*/

	if (SearchForAcc(vAccInfo, AccNumber, AccInfo) == false)
	{
		AccInfo.AccountNumber = AccNumber;
		AccInfo.PinCode = ReadNumber("Enter PinCode: ");
		AccInfo.Name = ReadText("Enter Name: ");
		AccInfo.Phone = ReadText("Enter Phone: ");
		AccInfo.balance = ReadDouble("Enter Balance: ");

		/*AccInfo.AccountNumber = AccNumber;
		AccInfo.PinCode = 22;
		AccInfo.Name = "AdName";
		AccInfo.Phone = "AdPhone0000000";
		AccInfo.balance = 1989.99;*/
		return true;
	}
	else
	{
		return false;
	}


}

string ConvertClientRecordToLine(ClientAcc& AccInfo, string delm = "#//#")
{
	string Line = AccInfo.AccountNumber + delm;
	Line += to_string(AccInfo.PinCode) + delm;
	Line += AccInfo.Name + delm;
	Line += AccInfo.Phone + delm;
	Line += to_string(AccInfo.balance);

	return Line;
}

ClientAcc ConvertClientLineToRecord(string line, string delm = "#//#")
{
	ClientAcc AccInfo;
	vector<string> Data = SpiltWords_Hadhoud(line, "#//#");



	AccInfo.AccountNumber = Data.at(0);
	AccInfo.PinCode = stoi(Data.at(1));
	AccInfo.Name = Data.at(2);
	AccInfo.Phone = Data.at(3);
	AccInfo.balance = stof(Data.at(4));

	return AccInfo;

}


void PrintAccInfoCard(ClientAcc& AccInfo)
{
	cout << "\n\t\t\t<<Account Info>>\n\n";
	cout << "========================================================\n";
	cout << "\tAccount Number: " << AccInfo.AccountNumber << endl;
	cout << "\tPinCode: " << AccInfo.PinCode << endl;
	cout << "\tName: " << AccInfo.Name << endl;
	cout << "\tPhone Number: " << AccInfo.Phone << endl;
	cout << "\tBalance: " << AccInfo.balance << endl;
	cout << "========================================================\n";
}

//HandleInFile

/* My Solution
* at first i read data in vector
* then add new data to vector
* remove file contnet and fill it again with vector data
* i edit this solution and i aded ios::app so ididn't need to vectors i just add to file
*
*/

vector <string> GetFileContentInVector(string fname = ::FILENAME)
{
	fstream f;
	vector <string> fContent;
	f.open(fname, ios::in);
	if (f.is_open())
	{
		string line = "";
		while (getline(f, line))
		{
			fContent.push_back(line);
		}
		f.close();
	}
	return fContent;
}

void WriteInFileByVector(vector<string>& FileContent, string Sep, string fname = FILENAME)
{


	vector<string>::iterator itr;
	fstream f;
	//string fname = AccInfo.AccountNumber + ".txt";
	f.open(fname, ios::out);
	if (f.is_open())
	{
		for (itr = FileContent.begin(); itr < FileContent.end(); itr++)
		{
			if (*itr != "")
				f << *itr << endl;
		}
		f.close();
	}
}


void WriteIntoFile(string sLine, string fname = FILENAME)
{

	fstream f;
	f.open(fname, ios::out | ios::app);

	if (f.is_open())
	{
		f << sLine << endl;
		f.close();
	}
}





vector<ClientAcc> LoadFIleRecordsIntoVector(string SEP, string Filename = FILENAME)
{
	vector<ClientAcc> Records;
	fstream myFile;
	myFile.open(FILENAME, ios::in);
	string line = "";
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			Records.push_back(ConvertClientLineToRecord(line, SEP));
		}
		myFile.close();
	}
	cout << endl;
	return Records;

}


/*Hadhoud Startegy
* Make Vectort of structure
*
*/



void PrintAccountInfo(ClientAcc& AccInfo)
{
	cout << "|" << left << setw(20) << AccInfo.AccountNumber;
	cout << " | " << left << setw(12) << AccInfo.PinCode;
	cout << " | " << left << setw(22) << AccInfo.Name;
	cout << " | " << left << setw(15) << AccInfo.Phone;
	cout << " | " << left << setw(13) << AccInfo.balance;
}



//search for Acc number



bool CheckForClientNumber(ClientAcc& AccInfo, string  AccNumber)
{
	return AccInfo.AccountNumber == AccNumber;
}

bool SearchForAcc(vector<ClientAcc>& vAccInfo, string  AccNumber, ClientAcc& AccInfo)
{
	for (ClientAcc& Acc : vAccInfo)
	{
		if (CheckForClientNumber(Acc, AccNumber))
		{
			AccInfo = Acc;
			return true;
		}
	}
	return false;

}


/*My strategy For Delete Record From File
*/

void SavingFileContent(vector<ClientAcc>& vAccInfo, string Filename = FILENAME)
{
	fstream f;
	f.open(Filename, ios::out);

	if (f.is_open())
	{
		string line = "";
		for (ClientAcc& Acc : vAccInfo)
		{
			if (Acc.MarkForDelete == false)
				f << ConvertClientRecordToLine(Acc, "#//#") << endl;
		}
		f.close();
	}
}




//@end Of Client Functions

//@@MainMenueFunctions






void BackToMainMenue(vector <ClientAcc>& vClientAccs) {
	cout << "\nPress any key to back..";
	system("pause>0");
	system("cls");

	MainMenueScreen(vClientAccs);
}

//@end OfMain MenueFunctions


//@@Cash Functions

double MoneyOpertion(ClientAcc& Acc, double money, bool Deposit = true)
{
	if (Deposit == true)
		Acc.balance = Acc.balance + money;
	else
	{
		if (money > Acc.balance)
			return 0;
		Acc.balance = Acc.balance - money;
	}

	return Acc.balance;
}

bool EditMoney(vector<ClientAcc>& vClientAccs, bool Deposit = true)
{
	for (ClientAcc& AccInfo : vClientAccs)
	{
		if (CheckForClientNumber(AccInfo, ActiveClient.AccountNumber))
		{
			PrintAccInfoCard(ActiveClient);

			double amount = ReadDouble("Enter Cash Amount:	");

			char Sure = 'n';
			cout << "Are you Sure you want perform this operation? Y/N?  ";
			cin >> Sure;

			if (tolower(Sure) == 'n')
				return true;

			double blanace = MoneyOpertion(ActiveClient, amount, Deposit);


			if (blanace == 0)
				return false;
			return true;
		}
	}
	return false;
}


void ShowDepositScreen(vector <ClientAcc>& vClientAccs)
{
	cout << "\n===============================================\n";
	cout << "\t\tDeposite Screen";
	cout << "\n===============================================\n";


	if (EditMoney(vClientAccs))
	{
		SavingFileContent(vClientAccs, FILENAME);
		system("Pause>0");
	}

}

void ShowWithdrawScreen(vector <ClientAcc>& vClientAccs)
{
	system("cls");
	cout << "\n===============================================\n";
	cout << "\t\tWithdraw Screen";
	cout << "\n===============================================\n";


	if (EditMoney(vClientAccs, false))
	{
		SavingFileContent(vClientAccs, FILENAME);
		system("Pause>0");
	}
	else
	{
		cout << "You Don't Have This Cash Amount....\n";
		system("Pause>0");

	}
}

void ShowBlanceScreen()
{
	system("cls");
	cout << "\n==============================================================================================\n";
	cout << "\t\t\t\t Balance  Screen";
	cout << "\n==============================================================================================\n";
	cout << "\n--------------------------------------------------------------------------------------------------------------------\n";
	cout << "|" << left << setw(20) << "Account Number";
	cout << " | " << left << setw(22) << "Name";
	cout << " | " << left << setw(13) << "Balance";
	cout << "\n--------------------------------------------------------------------------------------------------------------------\n";

	cout << "|" << left << setw(20) << ActiveClient.AccountNumber;
	cout << " | " << left << setw(22) << ActiveClient.Name;
	cout << " | " << left << setw(13) << ActiveClient.balance;
	cout << endl;


	system("pause>0");
}


void ShowQuickWithdrawScreen(vector <ClientAcc>& vClientAccs)
{
	system("cls");
	cout << "\n==============================================================================\n";
	cout << "\t\t\tQuick Withdraw\t\t\t";
	cout << "\n==============================================================================\n";
	cout << "|1| " << "20\n";
	cout << "|2| " << "100\n";
	cout << "|3| " << "200\n";
	cout << "|4| " << "500\n";
	cout << "|5| " << "800\n";
	cout << "|6| " << "1000\n";
	cout << "|7| " << "2000\n";
	cout << "\n==============================================================================\n";
	cout << endl;
	cout << "Your Balance Is : " << to_string(ActiveClient.balance) << endl;
	short OptionNumber;
	do {
		OptionNumber = ReadNumber("Choose What You Want To Do ? [1 - 7]\t");
	} while (OptionNumber <= 0 || OptionNumber >= 8);

	QuickWithdraw(vClientAccs, OptionNumber);
}

void  UpdateUsersVector(vector<ClientAcc>& vClients)
{
	for (ClientAcc& client : vClients)
	{
		if (CheckForClientNumber(client, ActiveClient.AccountNumber))
		{
			client = ActiveClient;
			break;
		}
	}
	SavingFileContent(vClients);

}


void QuickWithdraw(vector<ClientAcc>& vClients, short OptionNumber)
{
	vector<short> Quick = { 0,20,100,200 ,500,800,1000,2000 };

	short withdrawAmount = Quick.at(OptionNumber);
	string msg = "Are You Sure You Want Withdarw " + to_string(withdrawAmount) + "$ [Y/N]";

	char Confrim = ReadChar(msg);
	if (tolower(Confrim) == 'y')
	{
		ActiveClient.balance -= withdrawAmount;
		UpdateUsersVector(vClients);
	}
	else
	{
		cout << "\n\nWithdraw Failed Press Any Key To Go Back.....\n\n";
		system("pause>0");
	}
}

bool NormalWithdrawValidation(int Amount, ClientAcc Client)
{
	return Amount % 5 != 0 || Amount > Client.balance;
}
void NormalWithdraw(vector<ClientAcc>& vClients)
{
	system("cls");
	cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	cout << "\t\t\t\t\Withdraw Screen\n";
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	PrintAccInfoCard(ActiveClient);
	int Amount = 0;

	do {
		Amount = ReadPostiveNumber("\n\nEnter Amount You Wanna Withdraw (Only 5's): ");

	} while (NormalWithdrawValidation(Amount, ActiveClient));

	string msg = "Are You Sure You Want Withdarw " + to_string(Amount) + "$ [Y/N]:	";

	char Confrim = ReadChar(msg);
	if (tolower(Confrim) == 'y')
	{
		ActiveClient.balance -= Amount;
		UpdateUsersVector(vClients);
	}
	else
	{
		cout << "\n\nWithdraw Failed Press Any Key To Go Back.....\n\n";
		system("pause>0");
	}
}

void PerfroAtmOptionsList(vector <ClientAcc>& vClientAccs, enAtmOperations AtmOpertion)
{
	system("cls");
	/*
	  eQuickWithdraw = 1,
	  eNormalWithdraw = 2,
	  eDeposit = 3,

	  eCheckBalance = 4,
	  eLogout = 5 };

	*/
	switch (AtmOpertion)
	{
	case enAtmOperations::eQuickWithdraw:
		ShowQuickWithdrawScreen(vClientAccs);
		BackToMainMenue(vClientAccs);
		return;
	case enAtmOperations::eNormalWithdraw:
		NormalWithdraw(vClientAccs);
		BackToMainMenue(vClientAccs);
		return;
	case enAtmOperations::eDeposit:
		ShowDepositScreen(vClientAccs);
		BackToMainMenue(vClientAccs);
		return;
	case enAtmOperations::eCheckBalance:
		ShowBlanceScreen();
		BackToMainMenue(vClientAccs);
		return;
	case enAtmOperations::eLogout:
		system("cls");
		LoginScreen(vClientAccs);
		break;

	default:
		break;
	}
}


//@End OF Cash Functions

//@@User Functions


bool SearchForClientByNameAndPassword(vector<ClientAcc>& vUsers)
{
	string AccountNumber = ReadText("Enter Your Username: ");
	short PinCode = ReadPostiveNumber("Enter Your Password: ");
	bool IsUserThere = SearchForAcc(vUsers, AccountNumber, ActiveClient);

	if (IsUserThere)
	{
		if (ActiveClient.PinCode == PinCode && ActiveClient.AccountNumber == AccountNumber)
			return true;
		else
			return false;
	}
	else
		return	false;
}

void MainMenueScreen(vector<ClientAcc> vClients)
{
	system("cls");
	cout << "\n==============================================================================\n";
	cout << "\t\t\tAtm Operation\t\t\t";
	cout << "\n==============================================================================\n";
	cout << "|1| " << "Quick Withdraw.\n";
	cout << "|2| " << "Normal Withdraw.\n";
	cout << "|3| " << "Deposit.\n";
	cout << "|4| " << "Check Balance.\n";
	cout << "|5| " << "Logout.\n";
	cout << "\n==============================================================================\n";
	cout << endl;

	short OptionNumber;
	do
	{
		OptionNumber = ReadNumber("Choose What You Want To Do ? [1 - 5]\t");
	} while (OptionNumber <= 0 || OptionNumber >= 6);

	PerfroAtmOptionsList(vClients, (enAtmOperations)OptionNumber);

}



void WriteIntoUsersFile(string sLine, string fname = Users_Filename)
{
	fstream f;

	f.open(fname, ios::out | ios::app);

	if (f.is_open())
	{
		cout << "\n\n\n=================<<<<<<<<File Is Opened>>>>>>>>=================\n\n\n";

		f << sLine << endl;
		f.close();
		system("pause>0");
	}
}


//End Of User Functions


void LoginScreenHeader()
{
	system("cls");
	cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	cout << "\t\t\t\t\tLogin Screen\n";
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

}

void LoginScreen(vector<ClientAcc>& vClientAccs)
{
	vClientAccs = LoadFIleRecordsIntoVector("#//#", FILENAME);

	bool IsFailed = false;

	do {
		LoginScreenHeader();

		bool IsLogged = SearchForClientByNameAndPassword(vClientAccs);

		if (IsLogged)
		{
			MainMenueScreen(vClientAccs);
			IsFailed = true;
		}
		else
			ShowAccsesDeniedMessage();

	} while (!IsFailed);
}

void ShowAccsesDeniedMessage()
{
	system("cls");
	cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	cout << "You Have No Premision To Be Here Go Back";
	cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

	cout << "\n\nPress Any Key To Go Back.......\n";
	system("pause>0");
};

int main()
{
	vector<ClientAcc> vClientAccs;


	LoginScreen(vClientAccs);


}
