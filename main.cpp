#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

int res;
int cap_loaded;
int tran_cost = 0;
int income;
bool vectorFilled = false;
int W;
string portName = "";
string src_name = "";
int src = -1;
string dest_name = "";
vector<int> wt;
vector<int> val;
vector<string> portNames = {"Mangalore", "Port Blair", "Vladivostok",
                            "Port Of Neom", "Southampton", "Cape Town",
                            "Miami", "Osaka", "Brisbane"};
void floyd_warshall(vector<vector<int>> &graph, int n, vector<vector<int>> &parent);
void knapSack();
void initialize_weight();
void display();
void add_new_weight();
void delete_weight();
void path(int i, int j, vector<vector<int>> &parent, int dest, string mp[]);
void initialize_map(vector<vector<int>> &graph, vector<vector<int>> &parent, int V);
void map();
void generate_bill();
void menu();

int findShip(const string &shipName, const vector<string> &portNames)
{
    for (int i = 0; i < portNames.size(); i++)
    {
        if (portNames[i] == shipName)
        {
            return i;
        }
    }
    return -1; // Return -1 if not found
}

// KMP string search algorithm
bool searchShip(string &shipName, vector<string> &portNames)
{
    int m = shipName.length();
    vector<int> lps(m, 0); // Longest Prefix Suffix array

    int len = 0;
    int i = 1;
    while (i < m)
    {
        if (shipName[i] == shipName[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
                len = lps[len - 1];
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }

    int n = portNames.size();
    int j = 0; // index for shipName
    for (int i = 0; i < n; i++)
    {
        int k = 0; // index for portNames[i]
        while (k < portNames[i].length())
        {
            if (shipName[j] == portNames[i][k])
            {
                j++;
                k++;
            }

            if (j == m)
            {
                int portIndex = findShip(shipName, portNames);
                if (portIndex != -1)
                {
                    src = portIndex;
                    portNames.erase(portNames.begin() + portIndex);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            else if (k < portNames[i].length() && shipName[j] != portNames[i][k])
            {
                if (j != 0)
                    j = lps[j - 1];
                else
                    k = k + 1;
            }
        }
    }
    return false;
}

void floyd_warshall(vector<vector<int>> &graph, int n, vector<vector<int>> &parent)
{
    for (int via = 0; via < n; via++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (graph[i][via] + graph[via][j] < graph[i][j])
                {
                    parent[i][j] = parent[via][j];
                    graph[i][j] = graph[i][via] + graph[via][j];
                }
            }
        }
    }
}

void knapSack()
{
    int i, w, n = wt.size();
    vector<vector<int>> K(n + 1, vector<int>(W + 1));
    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w)
                K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }
    res = K[n][W];
    income = res;
    if (income != 0)
    {
        cout << "_______" << endl;
        cout << "Income from selected items is Rs " << income << " K" << endl;
        cout << "Items selected : " << endl;
        w = W;
        for (i = n; i > 0 && res > 0; i--)
        {
            if (res == K[i - 1][w])
                continue;
            else
            {
                cout << "   Item no. " << i << endl;
                cap_loaded = cap_loaded + wt[i - 1];
                res = res - val[i - 1];
                w = w - wt[i - 1];
            }
        }
        cout << "________" << endl;
    }
}
void initialize_weight()
{
    cout << "\n\t\t+-------------------------------------+";
    cout << "\n\t\t|             Port Name               |";
    cout << "\n\t\t+-------------------------------------+";
    cout << "\n\t\t|          Mangalore(India)           |";
    cout << "\n\t\t|          Port Blair(India)          |";
    cout << "\n\t\t|          Vladivostok(Russia)        |";
    cout << "\n\t\t|          Port Of Neom(Saudi Arabia) |";
    cout << "\n\t\t|          Southampton(UK)            |";
    cout << "\n\t\t|          Cape Town(SA)              |";
    cout << "\n\t\t|          Miami(USA)                 |";
    cout << "\n\t\t|          Osaka(Japan)               |";
    cout << "\n\t\t|          Brisbane(Australia)        |";
    cout << "\n\t\t+-------------------------------------+" << endl;
    string portName;
    cout << "Enter the port name: ";
    fflush(stdin);
    // cin.ignore();
    getline(cin, portName);

    int check = 0,i = 0;
     while(portName[i]){                      
        if(check==0){
            portName[i]=toupper(portName[i]); //conversion of string takes place here
            check=1;
        }else if(isspace(portName[i])){
            check=0;
        }
        i++;
    }

    // Check if the ship name is available
    bool shipAvailable = searchShip(portName, portNames);

    if (shipAvailable)
    {
        cout << endl
             << "Ship is available for transportation from that port....." << endl
             << endl;

        src_name = portName;

        int n = 0;
        while (n <= 0)
        {
            cout << "\nEnter the number of items: ";
            cin >> n;

            if (n <= 0)
            {
                cout << "Number of items should be greater than 0" << endl;
            }
        }

        wt.clear();
        val.clear();

        int x;

        cout << "Enter the weight and value of each item:\n";
        for (int i = 0; i < n; i++)
        {
            cout << "Item " << i + 1 << " weight: ";
            cin >> x;
            wt.push_back(x);
            cout << "Item " << i + 1 << " value: ";
            cin >> x;
            val.push_back(x);
        }

        cout << "Enter the capacity of ship: ";
        cin >> W;

        knapSack();

        // Set vectorFilled to true after knapsack is filled
        vectorFilled = true;
    }
    else
    {
        cout << endl
             << "Ship is not available for transportation." << endl
             << endl;
    }
}

void display()
{
    int n = wt.size();
    if (n == 0)
    {
        return;
    }
    cout << "Item No"
         << "\t"
         << "Weights"
         << "\t"
         << "Values" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i + 1 << "\t" << wt[i] << "\t" << val[i] << endl;
    }
}

void add_new_weight()
{
    char ch = 'y';
    int i = wt.size(), x;
    display();
    while (ch == 'y')
    {
        cout << "Item " << i + 1 << " weight: ";
        cin >> x;
        wt.push_back(x);
        cout << "Item " << i + 1 << " value: ";
        cin >> x;
        val.push_back(x);

        cout << "Do you want to add more Items? (y/n) ";
        cin >> ch;
        i++;
    }

    knapSack();
}

void delete_weight()
{
    char ch = 'y';
    int x;

    while (ch == 'y')
    {
        display();
        cout << "Enter the Item no. you want to delete" << endl;
        cin >> x;
        int n = wt.size();

        if (x > n)
        {
            cout << "Invalid Item no. Try Again." << endl;
            continue;
        }
        if (x == n)
        {
            wt.pop_back();
            val.pop_back();
        }
        for (int i = x - 1; i < n - 1; i++)
        {

            wt[i] = wt[i + 1];
            val[i] = val[i + 1];
        }
        wt.resize(n - 1);
        val.resize(n - 1);

        cout << "Do you want to delete more Items? (y/n) ";
        cin >> ch;
    }

    display();

    knapSack();
}

void path(int i, int j, vector<vector<int>> &parent, int dest, string mp[])
{
    if (i == j)
    {
        cout << mp[i] << " --> ";
    }
    else
    {
        path(i, parent[i][j], parent, dest, mp);
        cout << mp[j];
        if (j != dest)
        {
            cout << " --> ";
        }
        else
        {
            cout << endl;
        }
    }
}

void initialize_map(vector<vector<int>> &graph, vector<vector<int>> &parent, int V)
{
    int inf = 1e9;

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            parent[i][j] = i;
        }
    }

    graph = {{0, 2, inf, 3, inf, inf, inf, inf, inf},
             {2, 0, inf, 4, inf, inf, inf, 4, 5},
             {inf, inf, 0, inf, 6, inf, inf, 2, inf},
             {3, 4, inf, 0, 7, 5, inf, inf, inf},
             {inf, inf, 6, 7, 0, 10, 4, inf, inf},
             {7, inf, inf, 5, 10, 0, 12, inf, inf},
             {inf, inf, inf, inf, 4, 12, 0, 14, inf},
             {inf, 4, 2, inf, inf, inf, 14, 0, 3},
             {inf, 5, inf, inf, inf, inf, inf, 3, 0}};
}

void map()
{
    while (vectorFilled == false)
    {
        cout << endl
             << "Moving to Weight Entries as no entries have been provide" << endl;
        initialize_weight();
    }

    int dest;
    int V = 9;
    vector<vector<int>> graph(V, vector<int>(V));
    vector<vector<int>> parent(V, vector<int>(V));
    initialize_map(graph, parent, V);
    floyd_warshall(graph, V, parent);

    cout << "\n\t\t+---------+----------------------------+";
    cout << "\n\t\t|  Code   |          Port Name         |";
    cout << "\n\t\t+---------+----------------------------+";
    cout << "\n\t\t|    0    |  Mangalore(India)          |";
    cout << "\n\t\t|    1    |  Port Blair(India)         |";
    cout << "\n\t\t|    2    |  Vladivostok(Russia)       |";
    cout << "\n\t\t|    3    |  Port of Neom(Saudi Arabia)|";
    cout << "\n\t\t|    4    |  Southampton(UK)           |";
    cout << "\n\t\t|    5    |  Cape Town(SA)             |";
    cout << "\n\t\t|    6    |  Miami(USA)                |";
    cout << "\n\t\t|    7    |  Osaka(Japan)              |";
    cout << "\n\t\t|    8    |  Brisbane(Australia)       |";
    cout << "\n\t\t+---------+----------------------------+" << endl;
    cout << "\n  Enter the destnation port Code you want deliver : ";
    cin >> dest;

    if (dest < 0 || dest > 8)
    {
        cout << "Invalid Port Code" << endl;
        return;
    }

    tran_cost = graph[src][dest];

    cout << "Minimum Transportation Cost = " << tran_cost << endl;

    if (src == dest)
    {
        cout << "Source And Destination are Same " << endl;
        return;
    }

    string mp[] = {"Mangalore", "Port Blair", "Vladivostok", "Port of Neom", "Southampton", "Cape Town", "Miami", "Osaka", "Brisbane"};

    // src_name = mp[src];
    dest_name = mp[dest];
    cout << "Best Route : " << endl;
    path(src, dest, parent, dest, mp);
}

void generate_bill()
{
    while (vectorFilled == false)
    {
        cout << endl
             << "Moving to Weight Entries as no entries have been provide" << endl;
        initialize_weight();
    }
    double gst;
    time_t timer = time(NULL);
    cout << " Your Digital Invoice\n\n";
    cout << "\n***********************************************************************";
    cout << "\n*                              INVOICE                                *";
    cout << "\n***";
    cout << "\n    FROM    " << src_name << "    TO    " << dest_name << "            ";
    cout << "\n***********************************************************************";
    cout << "\n    Time                   : " << ctime(&timer) << endl;
    cout << "\n    Income from selected Items                         Rs " << income * 1000 << "";
    cout << "\n    Items total weight loaded                        : " << cap_loaded << " Tonnes";
    cout << "\n    Total Transportation Cost                        : " << tran_cost * 1000 << "";
    cout << "\n    Remaining Amount                                   Rs " << (income - tran_cost) * 1000 << "";
    if (income - tran_cost > 0)
    {
        gst = 0.18 * (income - tran_cost) * 1000;
    }
    else
    {
        gst = 0;
    }
    cout << "\n    GST(18%)                                           Rs. " << gst << "";
    cout << "\n    Total Profit                                       Rs. " << (income - tran_cost) * 1000 - gst << "";
    cout << "\n***";
}

void menu()
{
    char cont = 'y';
    while (cont == 'y')
    {
        cout << "MAIN MENU" << endl;
        cout << "1. Enter Items" << endl;
        cout << "2. Add New Items" << endl;
        cout << "3. Delete Items" << endl;
        cout << "4. Send Shipment" << endl;
        cout << "5. Generate Bill" << endl;
        cout << "6. Exit" << endl;

        int ch;
        cout << "Enter your choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            initialize_weight();
            break;
        case 2:
            add_new_weight();
            break;
        case 3:
            delete_weight();
            break;

        case 4:
            if (vectorFilled == false)
            {
                cout << "You need to enter container details first." << endl;
                initialize_weight();
            }
            map();
            break;

        case 5:
            if (vectorFilled == false)
            {
                cout << endl
                     << "Moving to Weight Entries as no entries have been provide" << endl;
                initialize_weight();
            }
            else if (src_name == "")
            {
                cout << endl
                     << "No Transportation Record found" << endl;
                cout << "Hint : Send Shipment to generate bill" << endl;
            }
            else
            {
                generate_bill();
            }
            break;

        case 6:
            cont = 'n';
            break;

        default:
            cout << "Invalid choice....." << endl;
            cont = 'n';
            break;
        }
    }
}

int main()
{
    cout << "*******************************" << endl;
    cout << " WELCOME TO OUR SHIPMENT SYSTEM" << endl;
    cout << "*******************************" << endl;
    menu();
    return 0;
}