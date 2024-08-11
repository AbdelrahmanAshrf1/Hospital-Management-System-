#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <type_traits>

using namespace std;

// Declaration Login and Show Main menu Screens
void Login();
void ShowMainMenuScreenForManager();
void ShowMainMenuScreenForDoctor();
void ShowMainMenuScreenForPatient();
void ShowMainMenuScreenForAccounter();

// Define Special cases enums
enum enLoginTypes { enManager = 1, enDoctor = 2, enPatient = 3, enAccounter = 4, BookAppointment = 5 };
enum enPremissions { eAll = -1, SalaryReport = 1, ViewAppointments = 2, PatientsReport = 4 };
// Define enums for Main menus options
enum enManager { DoctorsList = 1, HireDoctor = 2, terminateContract = 3, DeductionSalary = 4, AddBonus = 5, mPatientsReport = 6, mExit = 7 };
enum enDoctor { dSalaryReport = 1, dViewAppointments = 2, dPatientsReport = 3, dExit = 4 };
enum enPatient { pViewAppointments = 1, ContactDoctor = 2, BillingInformation = 3, pExit = 4 };
enum enAccounter { FinancialReports = 1, UpdateOperationsCost = 2, ManagePayroll = 3, aExit = 4 };

// Define File paths 
const string ManagersFile = "System_Data/Managers_Data/ManagersFile.txt";
const string DoctorsFile = "System_Data/DoctorsFile.txt";
const string PatientsFile = "System_Data/Patients_Data/PatientsFile.txt";
const string AccountersFile = "System_Data/Accounter_Data/AccountersFile.txt";
const string OperationInfoFile = "System_Data/Accounter_Data/Operation_Info.txt";

// Define sub_structures
struct stDate
{
	short Year;
	short Month;
	short Day;
};

struct stSalaryReport
{
	string operationType;
	double CurrentSalary;
	short Amount;
	double TotalAmount;
	string Reasons;
	stDate Date;
};

struct stOperationInfo
{
	string OperationName;
	float OperationCost;
};

// Define main stuctures
struct stManager
{
	string AccountNumber;
	string PinCode;
	string Username;
	string ManagerName;
	string PhoneNumber;
	double MonthlySalary;
	short Permissions;
};

struct stDoctor
{
	string AccountNumber;
	string PinCode;
	string Username;
	string DoctorName;
	string PhoneNumber;
	double MonthlySalary;
	short Permissions;
	bool MarkForDelete = false;
};

struct stPatient
{
	string StatementNumber;
	string PatientName;
	string PhoneNumber;
	string TheDisease;
	string Gender;
	stDate OperationDate;
};

struct stAccounter
{
	string AccountNumber;
	string PinCode;
	string Username;
	string AccounterName;
	string PhoneNumber;
};

// Define Global Structuer
stDoctor CurrentDoctorAccount;
stPatient CurrentPatientAccount;

// File Manipulation
vector<string> SplitString(string Line, string Sperator = "*//*")
{
	short pos = 0;
	vector<string> vRecords;
	while ((pos = Line.find(Sperator)) != string::npos)
	{
		string Word = Line.substr(0, pos);
		if (Word != "")
			vRecords.push_back(Word);
		
		Line = Line.erase(0, pos + Sperator.length());
	}
	if (Line != "")
		vRecords.push_back(Line);

	return vRecords;
}

bool CreateTransactionHistoryFile(const string& FileName)
{
	ofstream outfile(FileName);
	if (!outfile)
		cerr << "Error creating transaction history file: " << FileName << endl;
	else
		return true;
	return false;
}

// Convert line of data to records
stManager ConvertLineToRecordsManger(string Line)
{
	vector<string> vRecords = SplitString(Line);
	stManager Manager;

	Manager.AccountNumber = vRecords[0];
	Manager.PinCode = vRecords[1];
	Manager.Username = vRecords[2];
	Manager.ManagerName = vRecords[3];
	Manager.PhoneNumber = vRecords[4];
	Manager.MonthlySalary = stod(vRecords[5]);
	Manager.Permissions = stoi(vRecords[6]);

	return Manager;
}

stDoctor ConvertLineToRecordsDoctor(string Line)
{
	vector<string> vRecords = SplitString(Line);
	stDoctor Doctor;

	Doctor.AccountNumber = vRecords[0];
	Doctor.PinCode = vRecords[1];
	Doctor.Username = vRecords[2];
	Doctor.DoctorName = vRecords[3];
	Doctor.PhoneNumber = vRecords[4];
	Doctor.MonthlySalary = stod(vRecords[5]);
	Doctor.Permissions = stoi(vRecords[6]);

	return Doctor;
}

stPatient ConvertLineToRecordsPatient(string Line)
{
	vector<string> vRecords = SplitString(Line);
	stPatient Patient;

	Patient.StatementNumber = vRecords[0];
	Patient.PatientName = vRecords[1];
	Patient.PhoneNumber = vRecords[2];
	Patient.TheDisease = vRecords[3];
	Patient.Gender = vRecords[4];

	Patient.OperationDate.Day = stoi(vRecords[5]);
	Patient.OperationDate.Month = stoi(vRecords[6]);
	Patient.OperationDate.Year = stoi(vRecords[7]);

	return Patient;
}

stAccounter ConvertLineToRecordsAccounter(string Line)
{
	vector<string> vRecords = SplitString(Line);
	stAccounter Accounter;

	Accounter.AccountNumber = vRecords[0];
	Accounter.PinCode = vRecords[1];
	Accounter.Username = vRecords[2];
	Accounter.AccounterName = vRecords[3];
	Accounter.PhoneNumber = vRecords[4];

	return Accounter;
}

stSalaryReport ConvertLineToRecordsSalaryReport(string Line)
{
	vector<string> vRecords = SplitString(Line);
	stSalaryReport SalaryReport;

	SalaryReport.operationType = vRecords[0];
	SalaryReport.CurrentSalary = stod(vRecords[1]);
	SalaryReport.Amount = stoi(vRecords[2]);
	SalaryReport.TotalAmount = stod(vRecords[3]);
	SalaryReport.Reasons = vRecords[4];
	SalaryReport.Date.Day = stoi(vRecords[5]);
	SalaryReport.Date.Month = stoi(vRecords[6]);
	SalaryReport.Date.Year = stoi(vRecords[7]);

	return SalaryReport;
}

stOperationInfo ConvertLineToRecordsOperationsInfo(string Line)
{
	vector<string> vRecords = SplitString(Line);
	stOperationInfo OperationInfo;

	OperationInfo.OperationName = vRecords[0];
	OperationInfo.OperationCost = stod(vRecords[1]);

	return OperationInfo;
}

// Load Data from file by file name to vector 
vector<stManager> LoadManagerDataFromFileToVector(string FileName)
{
	ifstream File(FileName, ios::in);
	vector<stManager> vManager;

	if (!File.is_open()) 
	{
		cerr << "Failed to open file: " << FileName << endl;
		return vManager;
	}

	if (File.is_open())
	{
		string line;
		stManager Manager;

		while (getline(File, line))
		{
			Manager = ConvertLineToRecordsManger(line);
			vManager.push_back(Manager);
		}
		File.close();
	}
	return vManager;
}

vector<stDoctor> LoadDoctorDataFromFileToVector(string FileName)
{
	ifstream File(FileName, ios::in);
	vector<stDoctor> vDoctor;

	if (!File.is_open())
	{
		cerr << "Failed to open file: " << FileName << endl;
		return vDoctor;
	}

	if (File.is_open())
	{
		string line;
		stDoctor Doctor;

		while (getline(File, line))
		{
			Doctor = ConvertLineToRecordsDoctor(line);
			vDoctor.push_back(Doctor);
		}
		File.close();
	}
	return vDoctor;
}

vector<stPatient> LoadPatientDataFromFileToVector(string FileName)
{
	ifstream File(FileName, ios::in);
	vector<stPatient> vPatient;

	if (!File.is_open())
	{
		cerr << "Failed to open file: " << FileName << endl;
		return vPatient;
	}

	if (File.is_open())
	{
		string line;
		stPatient Patient;

		while (getline(File, line))
		{
			Patient = ConvertLineToRecordsPatient(line);
			vPatient.push_back(Patient);
		}
		File.close();
	}
	return vPatient;
}

vector<stAccounter> LoadAccounterDataFromFileToVector(string FileName)
{
	ifstream File(FileName, ios::in);
	vector<stAccounter> vAccounter;

	if (!File.is_open())
	{
		cerr << "Failed to open file: " << FileName << endl;
		return vAccounter;
	}

	if (File.is_open())
	{
		string line;
		stAccounter Accounter;

		while (getline(File, line))
		{
			Accounter = ConvertLineToRecordsAccounter(line);
			vAccounter.push_back(Accounter);
		}
		File.close();
	}
	return vAccounter;
}

vector<stSalaryReport> LoadSalaryReportDataFromFileToVector(string FileName)
{
	ifstream File(FileName, ios::in);
	vector<stSalaryReport> vSalaryReport;

	if (!File.is_open())
	{
		char answer = 'y';
		cerr << "Failed to open file: " << FileName << endl;
		cerr << "\nDo you want to to create file to this Doctor Y/N ?" << endl;
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			if (CreateTransactionHistoryFile(FileName))
				cout << "\nFile Created Successfully.";
			else
				cout << "\nFailed to Create a file, Please try again !";
		}
		else
			return vSalaryReport;
		
	}

	if (File.is_open())
	{
		string line;
		stSalaryReport Report;

		while (getline(File, line))
		{
			Report = ConvertLineToRecordsSalaryReport(line);
			vSalaryReport.push_back(Report);
		}
		File.close();
	}
	return vSalaryReport;
}

vector<stOperationInfo> LoadOperationInfoDataFromFileToVector(string FileName)
{
	ifstream File(FileName, ios::in);
	vector<stOperationInfo> vOperationInfo;

	if (!File.is_open())
	{
		cerr << "Failed to open file: " << FileName << endl;
		return vOperationInfo;
	}

	if (File.is_open())
	{
		string line;
		stOperationInfo Data;

		while (getline(File, line))
		{
			Data = ConvertLineToRecordsOperationsInfo(line);
			vOperationInfo.push_back(Data);
		}
		File.close();
	}
	return vOperationInfo;
}

// Conver Records to Data line
string ConverManagerRecordsToLine(stManager Manager,string sperator = "*//*")
{
	return Manager.AccountNumber + sperator + Manager.PinCode + sperator + Manager.Username + sperator + Manager.ManagerName + sperator +
		   Manager.PhoneNumber + sperator + to_string(Manager.MonthlySalary) + sperator + to_string(Manager.Permissions);
}

string ConverDoctorRecordsToLine(stDoctor Doctor, string sperator = "*//*")
{
	return Doctor.AccountNumber + sperator + Doctor.PinCode + sperator + Doctor.Username + sperator+ Doctor.DoctorName+ sperator +
		   Doctor.PhoneNumber + sperator + to_string(Doctor.MonthlySalary) + sperator + to_string(Doctor.Permissions);
}

string ConverPatientRecordsToLine(stPatient Patient, string sperator = "*//*")
{
	return Patient.StatementNumber + sperator + Patient.PatientName + sperator + Patient.PhoneNumber + sperator + Patient.TheDisease +sperator + Patient.Gender + sperator + 
		   to_string(Patient.OperationDate.Day) + sperator + to_string(Patient.OperationDate.Month) + sperator + to_string(Patient.OperationDate.Year);
}

string ConverAccounterRecordsToLine(stAccounter Accounter, string sperator = "*//*")
{
	return Accounter.AccountNumber + sperator + Accounter.PinCode + sperator + Accounter.Username + sperator
		+ Accounter.AccounterName + sperator + Accounter.PhoneNumber;
}

string ConverSalaryReportRecordsToLine(stSalaryReport SalaryReport, string sperator = "*//*")
{
	return SalaryReport.operationType + sperator + to_string(SalaryReport.CurrentSalary) + sperator + to_string(SalaryReport.Amount) + sperator
		+ to_string(SalaryReport.TotalAmount) + sperator + SalaryReport.Reasons + sperator + to_string(SalaryReport.Date.Day) + sperator 
		+ to_string(SalaryReport.Date.Month) + sperator + to_string(SalaryReport.Date.Year);
}

string ConverOperationInfoRecordsToLine(stOperationInfo OperationInfo, string sperator = "*//*")
{
	return OperationInfo.OperationName + sperator + to_string(OperationInfo.OperationCost);
}

// Save Files (Update)
bool SaveManagerDataToFile(const string FileName, vector<stManager>& vManager)
{
	ofstream MyFile(FileName, ios::out);
	if (!MyFile.is_open())
	{
		cerr << "Error opening file for writing: " << FileName << endl;
		return false;
	}

	for (stManager& C : vManager)
	{
		MyFile << ConverManagerRecordsToLine(C) << endl;
	}

	MyFile.close();
	return true;
}

bool SaveDoctorDataToFile(const string FileName,vector<stDoctor>& vDoctor)
{
	ofstream MyFile(FileName, ios::out);
	if (!MyFile.is_open())
	{
		cerr << "Error opening file for writing: " << FileName << endl;
		return false;
	}

	for (stDoctor & C : vDoctor)
	{
		if (!C.MarkForDelete)
		{
			MyFile << ConverDoctorRecordsToLine(C) << endl;
		}
	}

	MyFile.close();
	return true;
}

bool SavestPatientDataToFile(const string FileName,stPatient& Patient)
{
	ofstream MyFile(FileName, ios::out);
	if (!MyFile.is_open())
	{
		cerr << "Error opening file for writing: " << FileName << endl;
		return false;
	}

	MyFile << ConverPatientRecordsToLine(Patient) << endl;

	MyFile.close();
	return true;
}

bool SaveDoctorSalaryReportDataToFile(const string FileName, vector<stSalaryReport>& vSalaryReport)
{
	ofstream MyFile(FileName, ios::out);
	if (!MyFile.is_open())
	{
		cerr << "Error opening file for writing: " << FileName << endl;
		return false;
	}

	for (stSalaryReport& S : vSalaryReport)
	{
		MyFile << ConverSalaryReportRecordsToLine(S) << endl;
	}

	MyFile.close();
	return true;
}

bool SaveOperationsCostToFile(const string FileName, vector<stOperationInfo>& vOperationInfo)
{
	ofstream MyFile(FileName, ios::out);
	if (!MyFile.is_open())
	{
		cerr << "Error opening file for writing: " << FileName << endl;
		return false;
	}

	for (stOperationInfo& D : vOperationInfo)
	{
		MyFile << ConverOperationInfoRecordsToLine(D) << endl;
	}

	MyFile.close();
	return true;
}

// Define Functions of (Manager) section
// (1) Define Show Doctor list.
void ShowDoctorListScreen()
{
	system("cls");
	cout << "======================================================================================================\n";
	cout << "\t\t\t\tHire Doctor Screen\n";
	cout << "======================================================================================================\n";

	vector<stDoctor> vDoctor = LoadDoctorDataFromFileToVector(DoctorsFile);

	cout << left << setw(15) << "Account_N"
		 << left << setw(10) << "PinCode"
		 << left << setw(15) << "Username"
		 << left << setw(20) << "Doctor Name"
		 << left << setw(15) << "Phone Number"
		 << left << setw(15) << "Monthly Salary"
		 << left << setw(15) << "Permissions" << '\n';

	if (vDoctor.size() == 0)
	{
		cout << "\tThere aren't any Doctors in the hospital ,Please Hire Doctors to show their Information.\n";
	}

	for (const stDoctor& D : vDoctor)
	{
		cout << left << setw(15) << D.AccountNumber
			 << left << setw(10) << D.PinCode
			 << left << setw(15) << D.Username
			 << left << setw(20) << D.DoctorName
			 << left << setw(15) << D.PhoneNumber
			 << left << setw(15) << D.MonthlySalary
			 << left << setw(15) << D.Permissions
			 << '\n';
	}
	cout << "======================================================================================================\n";
}

void GoBackToMainMenuScreen(enLoginTypes BackTo) // improve it
{
	cout << "\nPress any key to go back to the main menu...\n";
	system("pause>0");

	switch (BackTo)
	{
	case enManager:
		ShowMainMenuScreenForManager();
	case enDoctor:
		ShowMainMenuScreenForDoctor();
	case enPatient:
		ShowMainMenuScreenForPatient();
	case enAccounter:
		ShowMainMenuScreenForAccounter();
	}
}

// (2) Define Hire Doctor.
bool FindDoctorByAccountNumber(const string& accountNumber, stDoctor& doctor, vector<stDoctor>& vDoctors)
{
	for (stDoctor& d : vDoctors)
	{
		if (d.AccountNumber == accountNumber)
		{
			doctor = d;
			return true;
		}
	}
	return false;
}

short GivePremissions()
{
	short Permissions = 0;
	char answer = 'y';

	cout << "\nDo you wnat to give full access Y/N ?";
	cin >> answer;
	if (toupper(answer) == 'Y') return -1;

	cout << "\nDo you wnat to give access to Salary Report Y/N ?";
	cin >> answer;
	if (toupper(answer) == 'Y') Permissions += enPremissions::SalaryReport;

	cout << "\nDo you wnat to give access to View Appointments Y/N ?";
	cin >> answer;
	if (toupper(answer) == 'Y') Permissions += enPremissions::ViewAppointments;

	cout << "\nDo you wnat to give access to Patients Report Y/N ?";
	cin >> answer;
	if (toupper(answer) == 'Y') Permissions += enPremissions::PatientsReport;

	return Permissions;
}

stDoctor HireNewDoctor(vector<stDoctor>& vDoctors)
{
	stDoctor Doctor;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Doctor.AccountNumber);

	while (FindDoctorByAccountNumber(Doctor.AccountNumber, Doctor, vDoctors))
	{
		cout << "\nDoctor with [" << Doctor.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Doctor.AccountNumber);
	}

	cout << "\nEnter PinCode ?";
	cin >> Doctor.PinCode;
	cout << "\nEnter Username ?";
	cin >> Doctor.Username;
	cout << "\nEnter Doctor Name ?";
	cin.ignore(1, '\n');
	getline(cin, Doctor.DoctorName);
	cout << "\nEnter Phone Number ?";
	cin >> Doctor.PhoneNumber;
	cout << "\nEnter Monthly Salary ?";
	cin >> Doctor.MonthlySalary;

	// Permissions
	Doctor.Permissions = GivePremissions();

	return Doctor;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{

		MyFile << stDataLine << endl;

		MyFile.close();
	}
}

void ShowHireDoctorScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tHire New Doctor\n";
	cout << "===========================================\n";

	vector<stDoctor> vDoctors = LoadDoctorDataFromFileToVector(DoctorsFile);
	stDoctor newDoctor = HireNewDoctor(vDoctors);
	vDoctors.push_back(newDoctor);
	SaveDoctorDataToFile(DoctorsFile, vDoctors);

	cout << "\nDoctor has been successfully hired and added to the file.\n";
}

// (3) Define Treminate contract.
bool MarkUserForDeleteByAccountNumber(string AccountNumber, vector <stDoctor>& vDoctor)
{
	for (stDoctor& D : vDoctor)
	{
		if (D.AccountNumber == AccountNumber)
		{
			D.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void PrintDoctorInfoCard(stDoctor& Doctor)
{
	cout << "\n============================\n";
	cout << "Account_N    : " << Doctor.AccountNumber << endl;
	cout << "PinCode      : " << Doctor.PinCode << endl;
	cout << "Username     : " << Doctor.Username << endl;
	cout << "Doctor Name  : " << Doctor.DoctorName << endl;
	cout << "Phone Number : " << Doctor.PhoneNumber << endl;
	cout << "Salary       : " << Doctor.MonthlySalary << endl;
	cout << "Permissions  : " << Doctor.Permissions << endl;
	cout << "============================\n";
}

void terminateContractADoctor(vector<stDoctor>& vDoctors)
{
	string accountNumber;
	stDoctor doctor;
	char answer = 'y';

	cout << "\nEnter Account Number to delete doctor from the file: ";
	cin >> accountNumber;

	if (FindDoctorByAccountNumber(accountNumber, doctor, vDoctors))
	{
		MarkUserForDeleteByAccountNumber(accountNumber, vDoctors);
		PrintDoctorInfoCard(doctor);

		cout << "\nAre you sure you wnat to delete this Doctor  Y/N ? ";
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			SaveDoctorDataToFile(DoctorsFile, vDoctors);
			cout << "\nThe contract has been terminated.";
		}
	}
	else
		cout << "\nInvalid Account Number. Please try again!\n";
}

void ShowTerminateContractScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tterminate Contract\n";
	cout << "===========================================\n";

	vector<stDoctor> vDoctors = LoadDoctorDataFromFileToVector(DoctorsFile);
	terminateContractADoctor(vDoctors);
}

// (4) Define Deduction from salary.
void ShowDeductionOptionsScreen(string Massage)
{
	cout << "===========================================\n";
	cout << Massage;
	cout << "===========================================\n";
	cout << "\t[1] $20\t\t[2] $50\n";
	cout << "\t[3] $100\t[4] $200\n";
	cout << "\t[5] $400\t[6] $600\n";
	cout << "\t[7] $800\t[8] $1000\n";
	cout << "===========================================\n";
}

short GetManagerOption()
{
	short Option = 0;
	cin >> Option;

	while (cin.fail() || Option < 1 || Option > 8)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nInvalid input! Please enter a valid input [1 : 8]: !.";
		cin >> Option;
	}
	return Option;
}

short GetAmountToDeducation(short Option)
{
	switch (Option)
	{
	case 1: return 20;
	case 2: return 50;
	case 3: return 100;
	case 4: return 200;
	case 5: return 400;
	case 6: return 600;
	case 7: return 800;
	case 8: return 1000;
	default: return 0;
	}
}

// Deposit functions.
bool DepositBalanceToDoctorByAccountNumber(const string& AccountNumber, double Amount, vector<stDoctor>& vDoctor) 
{
	for (stDoctor& D : vDoctor)
	{
		if (D.AccountNumber == AccountNumber)
		{
			D.MonthlySalary += Amount;
			if (!SaveDoctorDataToFile(DoctorsFile, vDoctor))
			{
				cerr << "Failed to save client data!" << endl;
				return false;
			}

			cout << "Transaction successful! New balance: " << D.MonthlySalary << endl;
			return true;
		}
	}
	return false;
}

// This function retrieves the current date and adapts it to the date on your computer.
stDate GetSystemDate()
{
	time_t  CurrentTime = time(0); // Get the current time in secoinds
	tm* Time = localtime(&CurrentTime);

	stDate Date;
	Date.Day = Time->tm_mday;
	Date.Month = (Time->tm_mon + 1);
	Date.Year = (Time->tm_year + 1900);

	return Date;
}

void WriteReportForSalary(stDoctor doctor, double Amount, vector<stDoctor>& vDoctor)
{
	string DoctorFileName = "System_Data/Managers_Data/DoctorsSalaryReport/" + doctor.AccountNumber + ".txt";
	vector<stSalaryReport> vData = LoadSalaryReportDataFromFileToVector(DoctorFileName);
	
	string Reson;
	cout << "\nWrite reason for this operation :" << endl;
	cin.ignore(1, '\n');
	getline(cin, Reson);

	stSalaryReport Temp;
	Temp.operationType = (Amount > 0) ? "Bonus" : "Deduct";
	Temp.CurrentSalary = doctor.MonthlySalary;
	Temp.Amount = abs(Amount);
	Temp.TotalAmount = Temp.CurrentSalary + Amount;
	Temp.Reasons = Reson;
	Temp.Date = GetSystemDate();

	vData.push_back(Temp);

	SaveDoctorSalaryReportDataToFile(DoctorFileName, vData);
}

void ShowDeductionFromSalaryScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tDeduction From Salary\n";
	cout << "===========================================\n";

	string accountNumber;
	stDoctor doctor;
	char answer = 'y';

	vector<stDoctor> vDoctors = LoadDoctorDataFromFileToVector(DoctorsFile);

	cout << "\nEnter Account Number to  deduct from doctor's Salary : ";
	cin >> accountNumber;

	if (FindDoctorByAccountNumber(accountNumber, doctor, vDoctors))
	{
		PrintDoctorInfoCard(doctor);

		cout << "\Are you sure you want to deduct from this Doctor Y/N ?";
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			ShowDeductionOptionsScreen("\tDeduction From Salary\n");
			double Amount = GetAmountToDeducation(GetManagerOption());
			DepositBalanceToDoctorByAccountNumber(accountNumber, -Amount, vDoctors);
			WriteReportForSalary(doctor, -Amount, vDoctors);
			cout << "\nThe salary has been deducted.";
		}
	}
	else
		cout << "\nInvalid Account Number. Please try again!\n";
}

// (5) Define Add Bouns Screen.
void ShowAddBonusScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tAdd Bonus\n";
	cout << "===========================================\n";

	string accountNumber;
	stDoctor doctor;
	char answer = 'y';

	vector<stDoctor> vDoctors = LoadDoctorDataFromFileToVector(DoctorsFile);

	cout << "\nEnter Account Number to add Bonus to doctor Salary : ";
	cin >> accountNumber;

	if (FindDoctorByAccountNumber(accountNumber, doctor, vDoctors))
	{
		PrintDoctorInfoCard(doctor);

		cout << "\Are you sure you want to add Bonus to this Doctor Y/N ?";
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			ShowDeductionOptionsScreen("\tAdd Bonus to Salary\n");
			double Amount = GetAmountToDeducation(GetManagerOption());
			DepositBalanceToDoctorByAccountNumber(accountNumber, Amount, vDoctors);
			WriteReportForSalary(doctor, Amount, vDoctors);
			cout << "\nThe Bonus has been Added..";
		}
	}
	else
		cout << "\nInvalid Account Number. Please try again!\n";
}

// (6) Define Patient Reports 
void ShowPatientsReportScreen()
{
	system("cls");
	cout << "======================================================================================================\n";
	cout << "\t\t\t\tPatient Report Screen\n";
	cout << "======================================================================================================\n";

	vector<stPatient> vPatientData = LoadPatientDataFromFileToVector(PatientsFile);

	cout << left << setw(10) << "Patient Number"
		 << left << setw(15) << "Patient Name"
		 << left << setw(15) << "Phone Number"
		 << left << setw(20) << "TheDisease"
		 << left << setw(10) << "Gender"
	     << left << setw(5) << "Day" 
	     << left << setw(5) << "Month" 
	     << left << setw(8) << "Year" << '\n';


	if (vPatientData.size() == 0)
	{
		cout << "\tThere aren't any Patients in the hospital ,Please wait until comeing one.\n";
	}

	for (const stPatient &P : vPatientData)
	{
		cout << left << setw(10) << P.StatementNumber
			 << left << setw(15) << P.PatientName
			 << left << setw(15) << P.PhoneNumber
			 << left << setw(20) << P.TheDisease
			 << left << setw(10) << P.Gender
			 << left << setw(5) << P.OperationDate.Day
			 << left << setw(5) << P.OperationDate.Month
			 << left << setw(8) << P.OperationDate.Year
			 << '\n';
	}
	cout << "======================================================================================================\n";
}

// Read Option to Preforme
short ReadMainMenuOptions(int minOption, int maxOption)
{
	short Option = 0;
	cin >> Option;

	while (cin.fail() || Option < minOption || Option > maxOption)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nInvalid input! Please enter a valid input [" << minOption << " : " << maxOption << "]: ";
		cin >> Option;
	}
	return Option;
}

// Define Functions of (Doctors) section.
// (1) Define Show Salary Screen
void ShowAccessDeniedMesage()
{
	system("cls");
	cout << "----------------------------------\n";
	cout << "Access Denied,\nYou don't have Permission to do this,\n Please Conact your Admin.\n";
	cout << "----------------------------------\n";
}

bool CheckDoctorAccessPermission(enPremissions Permission)
{
	if (CurrentDoctorAccount.Permissions == enPremissions::eAll)
		return true;
	if ((Permission & CurrentDoctorAccount.Permissions) == Permission)
		return true;
	else
		return false;
}

void ShowDoctorSalaryReportScreen()
{
	system("cls");
	cout << "======================================================================================================\n";
	cout << "\t\t\t\tSalary Report Screen\n";
	cout << "======================================================================================================\n";


	string FileName = "System_Data/Managers_Data/DoctorsSalaryReport/" + CurrentDoctorAccount.AccountNumber + ".txt";
	vector<stSalaryReport> vDoctorsData =  LoadSalaryReportDataFromFileToVector(FileName);

	cout << left << setw(10) << "Operation"
		 << left << setw(10) << "Current"
		 << left << setw(10) << "Amount"
		 << left << setw(10) << "Total"
		 << left << setw(20) << "Reasons"
		 << left << setw(10) << "Day"
		 << left << setw(10) << "Month"
		 << left << setw(10) << "Year" << '\n';

	if (vDoctorsData.size() == 0)
	{
		cout << "\tThere aren't any Doctors in the hospital ,Please Hire Doctors to show their Information.\n";
	}

	for (const stSalaryReport& S : vDoctorsData)
	{
		cout << left << setw(10) << S.operationType
			 << left << setw(10) << S.CurrentSalary
			 << left << setw(10) << S.Amount
			 << left << setw(10) << S.TotalAmount
			 << left << setw(20) << S.Reasons
			 << left << setw(10) << S.Date.Day
			 << left << setw(10) << S.Date.Month
			 << left << setw(10) << S.Date.Year
			 << '\n';
	}
	cout << "======================================================================================================\n";
}

// (2) Define View Appointments.
void ShowViewAppointments()
{
	system("cls");
	cout << "======================================================================================================\n";
	cout << "\t\t\t\tAppointments Screen\n";
	cout << "======================================================================================================\n";

	vector<stPatient> vPatientData = LoadPatientDataFromFileToVector(PatientsFile);

	cout << left << setw(15) << "Patient Name"
		 << left << setw(20) << "TheDisease"
		 << left << setw(10) << "Gender"
		 << left << setw(8) << "Day"
		 << left << setw(8) << "Month"
		 << left << setw(8) << "Year" << '\n';


	if (vPatientData.size() == 0)
	{
		cout << "\tThere aren't any Patients in the hospital ,Please wait until comeing one.\n";
	}

	for (const stPatient& P : vPatientData)
	{
		cout << left << setw(15) << P.PatientName
			 << left << setw(20) << P.TheDisease
			 << left << setw(10) << P.Gender
			 << left << setw(8) << P.OperationDate.Day
			 << left << setw(8) << P.OperationDate.Month
			 << left << setw(8) << P.OperationDate.Year
			 << '\n';
	}
	cout << "======================================================================================================\n";

}

// Define Functions of (Paitent) section
// (2) Define Contact Doctor 
void ShowContactDoctorScreen()
{
	system("cls");
	cout << "========================================\n";
	cout << "\tContact Doctors Screen\n";
	cout << "========================================\n";

	vector<stDoctor> vDoctorData = LoadDoctorDataFromFileToVector(DoctorsFile);

	cout << left << setw(25) << "Doctor_Name"
		 << left << setw(20) << "Phone_Number" << '\n';

	if (vDoctorData.size() == 0)
	{
		cout << "\tThere aren't any Doctor in the hospital ,Please wait until Hiring Some.\n";
	}

	for (const stDoctor & D : vDoctorData)
	{
		cout << left << setw(25) << D.DoctorName
			 << left << setw(20) << D.PhoneNumber << '\n';
	}
	cout << "=========================================\n";

}

// (3) Define Billing Information
float GetOperationCost(string OperationName)
{
	vector<stOperationInfo> vOperationInfo = LoadOperationInfoDataFromFileToVector(OperationInfoFile);
	for (stOperationInfo& I : vOperationInfo)
	{
		if (I.OperationName == OperationName) return I.OperationCost;
	}
	return -1;
}

void ShowBillingInformationScreen() 
{
	system("cls");

	// Get current time
	time_t now = time(0);
	tm* ltm = localtime(&now);

	string Disease = CurrentPatientAccount.TheDisease;
	string PatientName = CurrentPatientAccount.PatientName;
	float OperationCost = GetOperationCost(Disease);
	float TaxRate = 0.1; // 10% tax rate
	float TaxAmount = OperationCost * TaxRate;
	float TotalAmount = OperationCost + TaxAmount;

	cout << "========================================\n";
	cout << "\tBilling Information Screen\n";
	cout << "========================================\n";
	cout << "Patient Name: " << PatientName << endl;
	cout << "Date: " << 1900 + ltm->tm_year << "-"
		 << 1 + ltm->tm_mon << "-"
		 << ltm->tm_mday << endl;
	cout << "----------------------------------------\n";
	cout << "Disease: " << Disease << endl;
	cout << "Operation Cost: $" << fixed << setprecision(2) << OperationCost << endl;
	cout << "Tax (" << TaxRate * 100 << "%): $" << fixed << setprecision(2) << TaxAmount << endl;
	cout << "----------------------------------------\n";
	cout << "Total Amount Due: $" << fixed << setprecision(2) << TotalAmount << endl;
	cout << "=========================================\n";
}

// Define Functions of (Accounter) section
// (1) View Financial Reports 
void ShowViewFinancialReport() 
{
	system("cls");
	cout << "====================================================\n";
	cout << "\tFinancial Report Screen\n";
	cout << "====================================================\n";

	vector<stManager> vManagersData = LoadManagerDataFromFileToVector(ManagersFile);
	vector<stDoctor> vDoctorsData = LoadDoctorDataFromFileToVector(DoctorsFile);
	double TotalManagersSalaries = 0, TotalDoctorsSalaries = 0;

	cout << left << setw(20) << "Manager Name"
		<< left << setw(15) << "Salary"
		<< left << setw(20) << "Phone Number" << '\n';
	cout << "----------------------------------------------------\n";

	if (vManagersData.empty())
	{
		cout << "\tThere aren't any managers in the hospital.\n";
	}
	else 
	{
		for (const stManager& M : vManagersData) 
		{
			cout << left << setw(20) << M.ManagerName
				<< left << setw(15) << fixed << setprecision(2) << M.MonthlySalary
				<< left << setw(20) << M.PhoneNumber << '\n';
			TotalManagersSalaries += M.MonthlySalary;
		}
	}

	cout << "----------------------------------------------------\n";
	cout << "Total Managers' Salaries: $" << fixed << setprecision(2) << TotalManagersSalaries << endl;
	cout << "----------------------------------------------------\n\n";

	cout << left << setw(20) << "Doctor Name"
		<< left << setw(15) << "Salary"
		<< left << setw(20) << "Phone Number" << '\n';
	cout << "----------------------------------------------------\n";

	if (vDoctorsData.empty()) 
	{
		cout << "\tThere aren't any doctors in the hospital.\n";
	}
	else 
	{
		for (const stDoctor& D : vDoctorsData) 
		{
			cout << left << setw(20) << D.DoctorName
				<< left << setw(15) << fixed << setprecision(2) << D.MonthlySalary
				<< left << setw(20) << D.PhoneNumber << '\n';
			TotalDoctorsSalaries += D.MonthlySalary;
		}
	}

	cout << "----------------------------------------------------\n";
	cout << "Total Doctors' Salaries: $" << fixed << setprecision(2) << TotalDoctorsSalaries << endl;
	cout << "----------------------------------------------------\n";
	cout << "Total Salaries: $" << fixed << setprecision(2) << (TotalDoctorsSalaries + TotalManagersSalaries) << endl;
	cout << "====================================================\n";
}

bool IsOperationInOperationsList(const string& OperationName, const vector<stOperationInfo>& vOperationsData, stOperationInfo& OperationInfo) 
{
	for (const stOperationInfo& O : vOperationsData) 
	{
		if (OperationName == O.OperationName) 
		{
			OperationInfo = O;
			return true;
		}
	}
	return false;
}

void ChangeOperationCost(stOperationInfo& OperationInfo, vector<stOperationInfo>& vOperationsData)
{
	char answer;
	cout << "\nThe current Cost is: " << fixed << setprecision(2) << OperationInfo.OperationCost << endl;
	cout << "Do you want to change this Cost (Y/N)? ";
	cin >> answer;

	if (toupper(answer) == 'Y') 
	{
		float NewCost;
		cout << "\nEnter New Cost: ";
		cin >> NewCost;

		while (NewCost < 0) 
		{
			cout << "\nCan't Assign negative Cost!";
			cout << "\nPlease enter a positive Cost: ";
			cin >> NewCost;
		}

		// Update the cost in the original vector
		for (stOperationInfo& O : vOperationsData) 
		{
			if (O.OperationName == OperationInfo.OperationName) 
			{
				O.OperationCost = NewCost;
				break;
			}
		}

		SaveOperationsCostToFile(OperationInfoFile, vOperationsData);
		cout << "\nCost updated successfully.\n";
	}
	else 
	{
		cout << "\nCost change canceled.\n";
	}
}

// (2) Define Update Operations Cost
void UpdateOperationsCostScreen() 
{
	system("cls");
	cout << "====================================================\n";
	cout << "\tUpdate Operations Cost Screen\n";
	cout << "====================================================\n";

	string OperationName;
	cout << "Enter Operation Name to update cost: ";
	cin.ignore(); // Clear the buffer
	getline(cin, OperationName);

	vector<stOperationInfo> vOperationsData = LoadOperationInfoDataFromFileToVector(OperationInfoFile);
	stOperationInfo OperationInfo;

	if (IsOperationInOperationsList(OperationName, vOperationsData, OperationInfo)) 
		ChangeOperationCost(OperationInfo, vOperationsData);
	else 
		cout << "\nOperation \"" << OperationName << "\" isn't in the list.\n";
}

// (3) Define Manage Payroll
bool FindManagerByAccountNumber(const string& accountNumber, stManager& Manager, vector<stManager>& vManagers)
{
	for (stManager& M : vManagers)
	{
		if (M.AccountNumber == accountNumber)
		{
			Manager = M;
			return true;
		}
	}
	return false;
}

void PrintManagerInfoCard(stManager& Manager)
{
	cout << "\n============================\n";
	cout << "Account_N    : " << Manager.AccountNumber << endl;
	cout << "PinCode      : " << Manager.PinCode << endl;
	cout << "Username     : " << Manager.Username << endl;
	cout << "Manager Name  : " << Manager.ManagerName << endl;
	cout << "Phone Number : " << Manager.PhoneNumber << endl;
	cout << "Salary       : " << Manager.MonthlySalary << endl;
	cout << "Permissions  : " << Manager.Permissions << endl;
	cout << "============================\n";
}

// Change Salaries.
bool ChangeManagerSalaryByAccountNumber(const string& AccountNumber, double Amount, vector<stManager>& vManager)
{
	for (stManager& D : vManager)
	{
		if (D.AccountNumber == AccountNumber)
		{
			D.MonthlySalary = Amount;
			if (!SaveManagerDataToFile(ManagersFile, vManager))
			{
				cerr << "Failed to save client data!" << endl;
				return false;
			}

			cout << "Transaction successful! New balance: " << D.MonthlySalary << endl;
			return true;
		}
	}
	return false;
}

bool ChangeDoctorSalaryByAccountNumber(const string& AccountNumber, double Amount, vector<stDoctor>& vDoctors)
{
	for (stDoctor& D : vDoctors)
	{
		if (D.AccountNumber == AccountNumber)
		{
			D.MonthlySalary = Amount;
			if (!SaveDoctorDataToFile(DoctorsFile, vDoctors))
			{
				cerr << "Failed to save client data!" << endl;
				return false;
			}

			cout << "Transaction successful! New balance: " << D.MonthlySalary << endl;
			return true;
		}
	}
	return false;
}

void ChangeManagerSalary()
{
	string AccountNumber;
	cout << "\nEnter Account Number : ";
	cin.ignore(1, '\n');
	getline(cin, AccountNumber);

	vector<stManager> vManagers = LoadManagerDataFromFileToVector(ManagersFile);
	stManager Manager;

	if (FindManagerByAccountNumber(AccountNumber, Manager, vManagers))
	{
		PrintManagerInfoCard(Manager);

		char answer = 'y';
		cout << "\Are you sure you want to Change salary from this Manager Y/N ?";
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			float Salary = 0;
			cout << "\nEnter new Salary : ";
			cin >> Salary;

			ChangeManagerSalaryByAccountNumber(AccountNumber, Salary, vManagers);
			cout << "\nThe salary has been Changed.";
		}
	}
	else
		cout << "\nThe are not any Manager in Hospital whih " << AccountNumber << " Account Number !.";
}

void ChangeDoctorSalary()
{
	string AccountNumber;
	cout << "\nEnter Account Number : ";
	cin.ignore(1, '\n');
	getline(cin, AccountNumber);

	vector<stDoctor> vDoctors = LoadDoctorDataFromFileToVector(DoctorsFile);
	stDoctor Doctor;

	if (FindDoctorByAccountNumber(AccountNumber, Doctor, vDoctors))
	{
		PrintDoctorInfoCard(Doctor);

		char answer = 'y';
		cout << "\Are you sure you want to Change salary from this Manager Y/N ?";
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			float Salary = 0;
			cout << "\nEnter new Salary : ";
			cin >> Salary;

			ChangeDoctorSalaryByAccountNumber(AccountNumber, Salary, vDoctors);
			cout << "\nThe salary has been deducted.";
		}
	}
	cout << "\nThe are not any Doctor in Hospital whih " << AccountNumber << " Account Number !.";

}

void ShowManagePayrollScreen()
{
	system("cls");
	cout << "====================================================\n";
	cout << "\tManage Payroll Screen\n";
	cout << "====================================================\n";
	short Choice = 0;
	cout << "Do you wnat to Chage Salary for [1] Managers ,[2] Doctors. ";
	Choice = ReadMainMenuOptions(1, 2);

	if (Choice == 1)
		ChangeManagerSalary();
	else
		ChangeDoctorSalary();
}

// Set Main Screens for each type
void ShowMainMenuScreenForManager()
{
	system("cls");
	cout << "==========================\n";
	cout << "\tManager Menu\n";
	cout << "==========================\n";
	cout << "  [1] Show Doctor list." << endl;
	cout << "  [2] Hire new Doctor." << endl;
	cout << "  [3] Termination of employment." << endl;
	cout << "  [4] Deduction from salary." << endl;
	cout << "  [5] Add Bonus." << endl;
	cout << "  [6] Patients report" << endl;
	cout << "  [7] Exit" << endl;
	cout << "==========================\n";

	switch(ReadMainMenuOptions(1,7))
	{
	case enManager::DoctorsList:
		ShowDoctorListScreen();
		GoBackToMainMenuScreen(enManager);

	case enManager::HireDoctor:
		ShowHireDoctorScreen();
		GoBackToMainMenuScreen(enManager);

	case enManager::terminateContract:
		ShowTerminateContractScreen();
		GoBackToMainMenuScreen(enManager);

	case enManager::DeductionSalary:
		ShowDeductionFromSalaryScreen();
		GoBackToMainMenuScreen(enManager);

	case enManager::AddBonus:
		ShowAddBonusScreen();
		GoBackToMainMenuScreen(enManager);

	case enManager::mPatientsReport:
		ShowPatientsReportScreen();
		GoBackToMainMenuScreen(enManager);

	case enManager::mExit:
		Login();
	}
}

void ShowMainMenuScreenForDoctor()
{
	system("cls");
	cout << "==========================\n";
	cout << "\tDoctor Menu\n";
	cout << "==========================\n";
	cout << "  [1] Salary report" << endl;
	cout << "  [2] View Appointments" << endl;
	cout << "  [3] Patients report" << endl;
	cout << "  [4] Exit" << endl;
	cout << "==========================\n";

	switch (ReadMainMenuOptions(1, 4))
	{
	case enDoctor::dSalaryReport:
		if (!CheckDoctorAccessPermission(enPremissions::SalaryReport))
		{
			ShowAccessDeniedMesage();
			return;
		}
		else
			ShowDoctorSalaryReportScreen();
		GoBackToMainMenuScreen(enDoctor);

	case enDoctor::dViewAppointments:
		if (!CheckDoctorAccessPermission(enPremissions::ViewAppointments))
		{
			ShowAccessDeniedMesage();
			return;
		}
		else
			ShowViewAppointments();
		GoBackToMainMenuScreen(enDoctor);

	case enDoctor::dPatientsReport:
		if (!CheckDoctorAccessPermission(enPremissions::PatientsReport))
		{
			ShowAccessDeniedMesage();
			return;
		}
		else
			ShowPatientsReportScreen();
		GoBackToMainMenuScreen(enDoctor);

	case enDoctor::dExit:
		Login();
	}
}

void ShowMainMenuScreenForPatient()
{
	system("cls");
	cout << "==========================\n";
	cout << "\tPatient Menu\n";
	cout << "==========================\n";
	cout << "  [1] View Appointments" << endl;
	cout << "  [2] Contact Doctor" << endl;
	cout << "  [3] Billing Information" << endl;
	cout << "  [4] Exit" << endl;
	cout << "==========================\n";

	switch (ReadMainMenuOptions(1, 4))
	{
	case enPatient::pViewAppointments:
		ShowViewAppointments();
		GoBackToMainMenuScreen(enPatient);

	case enPatient::ContactDoctor:
		ShowContactDoctorScreen();
		GoBackToMainMenuScreen(enPatient);

	case enPatient::BillingInformation:
		ShowBillingInformationScreen();
		GoBackToMainMenuScreen(enPatient);

	case enPatient::pExit:
		Login();
	}
}

void ShowMainMenuScreenForAccounter()
{
	system("cls");
	cout << "==========================\n";
	cout << "\tAccounter Menu\n";
	cout << "==========================\n";
	cout << "  [1] View Financial Reports" << endl;
	cout << "  [2] Update Operations Cost" << endl;
	cout << "  [3] Manage Payroll" << endl;
	cout << "  [4] Exit" << endl;
	cout << "==========================\n";

	switch (ReadMainMenuOptions(1, 4))
	{
	case enAccounter::FinancialReports:
		ShowViewFinancialReport();
		GoBackToMainMenuScreen(enAccounter);

	case enAccounter::UpdateOperationsCost:
		UpdateOperationsCostScreen();
		GoBackToMainMenuScreen(enAccounter);

	case enAccounter::ManagePayroll:
		ShowManagePayrollScreen();
		GoBackToMainMenuScreen(enAccounter);

	case enAccounter::aExit:
		Login();
	}
}

void GetCertainScreen(enLoginTypes ScreenFor)
{
	switch (ScreenFor)
	{
	case enManager:
		ShowMainMenuScreenForManager();
		break;
	case enDoctor:
		ShowMainMenuScreenForDoctor();
		break;
	case enPatient:
		ShowMainMenuScreenForPatient();
		break;
	case enAccounter:
		ShowMainMenuScreenForAccounter();
		break;
	}
}

// Define functions for handling patient Book An Appointment
void ShowListOfMedicalOperations()
{
	cout << "\n============================\n";
	cout << "\tMedical Operations\n";
	cout << "============================\n";
	cout << " 01 : Appendectomy.\n";
	cout << " 02 : HeartTransplant.\n";
	cout << " 03 : CataractSurgery.\n";
	cout << " 04 : Gallbladder Removal.\n";
	cout << " 05 : Knee Replacement.\n";
	cout << " 06 : Hip Replacement.\n";
	cout << " 07 : Spinal Fusion.\n";
	cout << " 08 : Hernia Repair.\n";
	cout << " 09 : Hysterectomy.\n";
	cout << " 10 : Mastectomy.\n";
	cout << " 11 : Angioplasty.\n";
	cout << " 12 : Tonsillectomy.\n";
	cout << " 13 : Cesarean Section.\n";
	cout << " 14 : Prostatectomy.\n";
	cout << " 15 : Thyroidectomy.\n";
	cout << " 16 : Laparoscopy.\n";
	cout << " 17 : Gastric Bypass Surgery.\n";
	cout << " 18 : Liver Transplant.\n";
	cout << " 19 : Craniotomy.\n";
	cout << " 20 : Colectomy.\n";
	cout << "============================\n";
}

string GetMedicalOperationName()
{
	string arrOperationsName[20] =
	{
		"Appendectomy","HeartTransplant","CataractSurgery","Gallbladder Removal","Knee Replacement",
		"Hip Replacement","Spinal Fusion","Hernia Repair","Hysterectomy","Mastectomy","Angioplasty",
		"Tonsillectomy","Cesarean Section","Prostatectomy","Thyroidectomy","Laparoscopy",
		"Gastric Bypass Surgery","Liver Transplant","Craniotomy","Colectomy"
	};

	short Choice = ReadMainMenuOptions(1, 20);
	return arrOperationsName[Choice - 1];
}

string GetGender()
{
	short Choice = ReadMainMenuOptions(1, 2);
	return (Choice == 1) ? "Male" : "Female";
}

stDate SetOperationAppointment()
{
	stDate Date;
	char Answer;

	cout << "\nIs your condition an emergency Y/N ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		return Date = GetSystemDate();
	cout << "\nEnter Day : ";
	Date.Day = ReadMainMenuOptions(1, 31);
	cout << "\nEnter Month : ";
	Date.Month = ReadMainMenuOptions(1,12);
	cout << "\nEnter Year : ";
	cin >> Date.Year;

	return Date;
}

stPatient BookAnAppointment()
{
	stPatient Patient;

	cout << "\nEnter your Statement Number : ";
	cin.ignore(1, '\n');
	getline(cin, Patient.StatementNumber);

	cout << "\nEnter your Name : ";
	cin.ignore(1, '\n');
	getline(cin, Patient.PatientName);

	cout << "\nEnter your Phone number : ";
	cin.ignore(1, '\n');
	getline(cin, Patient.PhoneNumber);

	cout << "\nFrom what you suffer : ";
	ShowListOfMedicalOperations();
	Patient.TheDisease = GetMedicalOperationName();

	cout << "\nWhat is your Gender male [1] , female [2] : ";
	Patient.Gender = GetGender();

	Patient.OperationDate = SetOperationAppointment();

	return Patient;
}

// Login And Check Accounts
template <typename T>
bool SearchAccountInVector(vector<T> vData,string Username, string PinCode)
{
	for (T & account : vData)
	{
		if (account.Username == Username && account.PinCode == PinCode)
		{
			if constexpr (is_same<T, stDoctor>::value) CurrentDoctorAccount = account;
			return true;
		}
	}
	return false;
}

bool SearchAccountInVector_Patinet(vector<stPatient> vData, string patientName, string statementNumber)
{
	for (stPatient& P : vData)
	{
		if (P.PatientName == patientName || P.StatementNumber == statementNumber)
		{
			CurrentPatientAccount = P;
			return true;
		}
	}
	return false;
}

bool CheckAccount(string Username, string PinCode, enLoginTypes LoginAs) {
	switch (LoginAs) 
	{
	case enManager: 
	{
		vector<stManager> vManagers = LoadManagerDataFromFileToVector(ManagersFile);
		return SearchAccountInVector(vManagers, Username, PinCode);
	}
	case enDoctor: 
	{
		vector<stDoctor> vDoctors = LoadDoctorDataFromFileToVector(DoctorsFile);
		return SearchAccountInVector(vDoctors, Username, PinCode);
	}
	case enPatient: 
	{
		vector<stPatient> vPatients = LoadPatientDataFromFileToVector(PatientsFile);
		return SearchAccountInVector_Patinet(vPatients, Username, PinCode);
	}
	case enAccounter:
	{
		vector<stAccounter> vAccounters = LoadAccounterDataFromFileToVector(AccountersFile);
		return SearchAccountInVector(vAccounters, Username, PinCode);
	}
	}
}

short ReadLoginAsOptions()
{
	short Choice = 0;
	cin >> Choice;

	while (cin.fail() || Choice < 1 || Choice > 5)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nInvalid input ! please enter a vaild input [ 1 : 5 ] ?";
		cin >> Choice;
	}
	return Choice;
}

void DisplayLoginMenu() 
{
	system("cls");
	cout << "==========================\n";
	cout << "\tLogin\n";
	cout << "==========================\n";
	cout << "[1] Manager\n[2] Doctor\n[3] Patient\n[4] Accounter\n[5] Book Appointment.\n";
	cout << "===========================\nLogin as: ";
}

bool HandleAppointmentBooking() 
{
	stPatient Patient = BookAnAppointment();
	SavestPatientDataToFile(PatientsFile, Patient);
	cout << "\nYour Booking is Done Successfully.";
	system("pause>0");
	return true;  // Considered logged in after booking an appointment.
}

bool AttemptLogin(const string& username, const string& pinCode, enLoginTypes loginAs) 
{
	if (CheckAccount(username, pinCode, loginAs)) 
	{
		return true;
	}

	if (loginAs == enLoginTypes::enPatient) 
	{
		char answer;
		cout << "\nThere is no reservation with this information.\n";
		cout << "Do you want to book an appointment? Y/N : ";
		cin >> answer;

		if (toupper(answer) == 'Y') 
		{
			return HandleAppointmentBooking();
		}
		else 
		{
			cout << "\nWish you all the best :-).\n";
			system("pause");
		}
	}
	else 
	{
		cout << "\nInvalid Account Number or PIN Code. Please try again.\n";
		system("pause");
	}

	return false;
}

void Login() 
{
	string username, pinCode;
	bool loggedIn = false;
	enLoginTypes loginAs;

	while (!loggedIn) 
	{
		DisplayLoginMenu();
		loginAs = static_cast<enLoginTypes>(ReadLoginAsOptions());

		if (loginAs == enLoginTypes::BookAppointment) 
		{
			loggedIn = HandleAppointmentBooking();
			continue;
		}

		cout << (loginAs == enLoginTypes::enPatient ? "\nEnter Patient Name: " : "\nEnter Username: ");
		cin.ignore();
		getline(cin, username);
		cout << (loginAs == enLoginTypes::enPatient ? "\nEnter Patient Number: " : "\nEnter PinCode: ");
		cin >> pinCode;

		loggedIn = AttemptLogin(username, pinCode, loginAs);
	}

	GetCertainScreen(loginAs);
}

int main()
{
	Login();
	system("pause>0");

	return 0; 
}
