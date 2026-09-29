#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// Permissions
const int ShowAllClients = 1 << 0;  // 0000001 = 1
const int AddnewClient = 1 << 1;  // 0000010 = 2
const int DeletClient = 1 << 2;  // 0000100 = 4
const int UpdateClient = 1 << 3;  // 0001000 = 8
const int FindClient = 1 << 4;  // 0010000 = 16
const int Transaction = 1 << 5;  // 0100000 = 32
const int ManageUser = 1 << 6;  // 1000000 = 64
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};
struct sUser
{
    string UserName;
    string Password;
    int permissions;
    bool MarkForDelete = false;
};
struct sPermissions
{
    bool ShowAllClients;
    bool AddNewClient;
    bool DeletClient;
    bool UpdateClient;
    bool FindClient;
    bool Transaction;
    bool ManageUser;
};

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenue(sUser CurrentUser);
void ShowTransactionsMenue(sUser CurrentUser);
void ShowManageUserMenue(sUser CurrentUser);


enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };
enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5,
    eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8
};
enum enManageUsersOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenue = 6
};


vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
sUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{

    sUser User;
    vector<string>vUser = SplitString(Line, Seperator);

    User.UserName = vUser[0];
    User.Password = vUser[1];
    User.permissions = stoi(vUser[2]);

    return User;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
string ConvertRecordToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.permissions);

    return stUserRecord;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}
bool UserExistsByUserName(string UserName, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string line;
        sUser User;
        while (getline(MyFile, line))
        {
            User = ConvertUserLinetoRecord(line, "#//#");
            if (User.UserName == UserName)
            {
                MyFile.close();
                return true;
            }
        }
    }
    return false;
}
sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

int CalcResultPermissions(sPermissions Permissions)
{
    int Result = 0;
    if (Permissions.ShowAllClients)
        Result |= ShowAllClients;
    if (Permissions.AddNewClient)
        Result |= AddnewClient;
    if (Permissions.DeletClient)
        Result |= DeletClient;

    if (Permissions.UpdateClient)
        Result |= UpdateClient;

    if (Permissions.FindClient)
        Result |= FindClient;

    if (Permissions.Transaction)
        Result |= Transaction;

    if (Permissions.ManageUser)
        Result |= ManageUser;

    return Result;

}
int ReadPermissions(sPermissions& permissions)
{
    char choice = 'n';
    int Result;
    cout << "Do you want to give full access y/n?";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        return  127;
    }
    cout << "Do you want to give access to:\n\n";
    cout << "Show Client List? y/n?";
    cin >> choice;
    permissions.ShowAllClients = (choice == 'y' || choice == 'Y');

    cout << "Add New Client? y/n? ";
    cin >> choice;
    permissions.AddNewClient = (choice == 'y' || choice == 'Y');


    cout << "Delete Client? y/n? ";
    cin >> choice;
    permissions.DeletClient = (choice == 'y' || choice == 'Y');


    cout << "Update Client? y/n? ";
    cin >> choice;
    permissions.UpdateClient = (choice == 'y' || choice == 'Y');


    cout << "Find Client? y/n? ";
    cin >> choice;
    permissions.FindClient = (choice == 'y' || choice == 'Y');


    cout << "Transactions? y/n? ";
    cin >> choice;
    permissions.Transaction = (choice == 'y' || choice == 'Y');


    cout << "Manage Users? y/n? ";
    cin >> choice;
    permissions.ManageUser = (choice == 'y' || choice == 'Y');

    return CalcResultPermissions(permissions);
}
sUser ReadNewUser()
{
    sUser User;
    sPermissions Permission;
    cout << "Enter Username? ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFileName))
    {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Enter Password? ";
    getline(cin, User.Password);
    User.permissions = ReadPermissions(Permission);
    return User;
}

string ReadUserName()
{
    string UserName;
    cout << "Enter Username? ";
    getline(cin >> ws, UserName);
    return UserName;
}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
vector<sUser>LoadUsersDataFromFile(string FileName)
{
    vector<sUser>vUsers;
    fstream Myfile;
    Myfile.open(FileName, ios::in);
    if (Myfile.is_open())
    {
        string Line;
        sUser User;
        while (getline(Myfile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            vUsers.push_back(User);
        }

    }
    return vUsers;
}

void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}
void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen()
{


    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}
void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}
void PrintUserCard(sUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername     : " << User.UserName;
    cout << "\nPassword     : " << User.Password;
    cout << "\nPermissions  : " << User.permissions;

    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
bool FindUserByUserName(string Username, vector<sUser>vUsers, sUser& User)
{
    for (sUser U : vUsers)
    {
        if (U.UserName == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}
sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}
sUser ChangeUserRecord(string USerName)
{
    sUser User;
    sPermissions Permissions;
    User.UserName = USerName;
    cout << "Enter Password? ";
    getline(cin>>ws, User.Password);
    User.permissions = ReadPermissions(Permissions);
    return User;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}
bool MarkUserForDeleteByUserName(string Username, vector <sUser> &vUsers)
{
    for (sUser& U : vUsers)
    {
        if (Username == U.UserName)
        {
            U.MarkForDelete = true;
            return true;

        }
    }
    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}
vector <sUser> SaveUserDataToFile(string FileName, vector <sUser> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sUser U : vUsers)
        {

            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(U);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;

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

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLine(User));

}
void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New User:\n\n";
        AddNewUser();

        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}
void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";
    AddNewUsers();

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}
bool DeleteUserByUserName(string UserName, vector<sUser>& vUsers)
{
    sUser user;
    char Answer = 'n';
    if (UserName == "Admin")
    {
        cout << "You Can't Delete Admin";
        return false;
    }
    if (FindUserByUserName(UserName, vUsers, user))
    {
        PrintUserCard(user);
        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUserDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Username (" << UserName << ") is Not Found!";
        return false;
    }
}
bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}
bool UpdateUserByUserName(string Username, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(Username, vUsers, User))
    {
        PrintUserCard(User);   
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sUser & U : vUsers)
            {
                if (U.UserName == Username)
                {
                    U = ChangeUserRecord(Username);
                    break;
                }

            }

            SaveUserDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << Username << ") is Not Found!";
        return false;
    }

    
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}
void ShowDelteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, vUsers);


}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}
void ShowUpdateUSerScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}
void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

bool FindUserByUsernameAndPassword(string UserName, string Password, sUser& User)
{
    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U; // تحميل كائن المستخدم بكافة بياناته وصلاحياته من الملف
            return true;
        }
    }
    return false;
}
sUser ReadUserInfo()
{
    sUser User;
    cout << "Enter Username? ";
    cin >> User.UserName;
    cout << "Enter Password? ";
    cin >> User.Password;
    return User;
}
void PrintUserInfoRecord(sUser User)
{
    cout << "|" << setw(15) << left << User.UserName;
    cout << "|" << setw(20) << left << User.Password;
    cout << "|" << setw(15) << left << User.permissions;
}
void ShowUserListScreen()
{
    system("cls");
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") User (s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "|" << left << setw(15) << "Username";
    cout << "|" << left << setw(20) << "Password";
    cout << "|" << left << setw(15) << "Permissions";
    cout << endl;

    if (vUsers.size() == 0)
    {
        cout << "\t\t\t\tNo Users Available In the System!";
    }
    else
    {
        for (sUser U : vUsers)
        {
            PrintUserInfoRecord(U);
            cout << endl;

        }
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}
void ShowFindUserScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\Find User Screen";
    cout << "\n-----------------------------------\n";

    sUser User;
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "The User Not Found\n";
    }

}

void GoBackToMainMenue(sUser CurrentUser)
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue(CurrentUser);

}
void GoBackToTransactionsMenue(sUser CurrentUser)
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue(CurrentUser);

}
void GoBackManageUserMenue( sUser CurrentUser)
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUserMenue(CurrentUser);
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption , sUser CurrentUser)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue(CurrentUser);
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue(CurrentUser);
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue(CurrentUser);
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {
        system("cls");
        ShowMainMenue(CurrentUser);
    }
    }

}
void ShowTransactionsMenue(sUser CurrentUser)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption() , CurrentUser);
}
bool checkPermissoin(int userPermission,int targetPermission)
{
    // 1. إذا كان لديه صلاحية كاملة (127 أو -1)
    if (userPermission == 127 || userPermission == -1)
        return true;

    // 2. التحقق من Bit الصلاحية الخاصة بـ ShowAllClients
    return ((userPermission & targetPermission) == targetPermission);
}



short ReadManageUsersOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void PerformManageUsersOption(enManageUsersOptions ManageUsersOption , sUser CurrentUser)
{
    switch (ManageUsersOption)
    {
    case eListUsers:
        ShowUserListScreen();
        GoBackManageUserMenue(CurrentUser);
        break;
    case eAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackManageUserMenue(CurrentUser);

        break;
    case eDeleteUser:
        system("cls");
        ShowDelteUserScreen();
        GoBackManageUserMenue(CurrentUser);
        break;
    case eUpdateUser:
        system("cls");
        ShowUpdateUSerScreen();
        GoBackManageUserMenue(CurrentUser);
        break;
    case eFindUser:
        ShowFindUserScreen();
        GoBackManageUserMenue(CurrentUser);
        break;
    case eMainMenue:
        ShowMainMenue(CurrentUser);
        break;
    default:
        break;
    }
}

void ShowManageUserMenue(sUser CurrentUser)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User .\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";


    cout << "===========================================\n";
    PerformManageUsersOption(enManageUsersOptions(ReadManageUsersOption()) , CurrentUser);
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void AccessDenaidMessage()
{
    cout << "-----------------------------------------------------------\n";

    cout << "Access Denaid\n";
    cout << "You Don't have Permission to do this,\n";
    cout << "Please Contact Your Admin.\n";
    cout << "-----------------------------------------------------------";
}
sUser ShowLoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";

    bool loginfailed = false;
    sUser User;
    sUser CurrentUser;
    do
    {
        if (loginfailed)
        {
            system("cls");
            cout << "\n-----------------------------------\n";
            cout << "\tLogin Screen";
            cout << "\n-----------------------------------\n";
            cout << "Invlaid Username/Password" << endl;
        }
        User = ReadUserInfo();

        loginfailed = !(FindUserByUsernameAndPassword(User.UserName, User.Password, CurrentUser));


    } while (loginfailed);

    system("cls");
    ShowMainMenue(CurrentUser);
    return CurrentUser;


}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, sUser CurrentUser)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        if (checkPermissoin(CurrentUser.permissions,ShowAllClients))
        {
            ShowAllClientsScreen();
        }
        else
        {
            AccessDenaidMessage();

        }
        GoBackToMainMenue(CurrentUser);


        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        if (checkPermissoin(CurrentUser.permissions,AddnewClient))
        {
            ShowAddNewClientsScreen();
        }
        else
        {
            AccessDenaidMessage();

        }
        GoBackToMainMenue(CurrentUser);
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        if (checkPermissoin(CurrentUser.permissions,DeletClient))
        {
            ShowDeleteClientScreen();
        }
        else
            AccessDenaidMessage();

        GoBackToMainMenue(CurrentUser);
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        if (checkPermissoin(CurrentUser.permissions, UpdateClient))
        {
            ShowUpdateClientScreen();
        }
        else
            AccessDenaidMessage();

        GoBackToMainMenue(CurrentUser);
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        if (checkPermissoin(CurrentUser.permissions,FindClient))
        {
            ShowFindClientScreen();
        }
        else
            AccessDenaidMessage();

        GoBackToMainMenue(CurrentUser);
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        if (checkPermissoin(CurrentUser.permissions, Transaction))
        {
            ShowTransactionsMenue(CurrentUser);
        }
        else
            AccessDenaidMessage();
        GoBackToMainMenue(CurrentUser);


        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        if (checkPermissoin(CurrentUser.permissions , ManageUser))
        {
            ShowManageUserMenue(CurrentUser);
        }
        else
        {
            AccessDenaidMessage();
        }
        GoBackToMainMenue(CurrentUser);
        break;
    case enMainMenueOptions::eLogout:
        system("cls");
        ShowLoginScreen();
    }

        

}
void ShowMainMenue(sUser CurrentUser)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";

    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), CurrentUser);
}


int main()

{
    
    ShowLoginScreen();
    system("pause>0");
    return 0;
}