#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// one line from the trace file
struct Entry
{
    int time;
    string cpu;
    string op;      // READ or WRITE
    unsigned int addr;
};

vector<Entry> loadTrace(const string& filename)
{
    vector<Entry> entries;
    ifstream file(filename);

    if (!file)
    {
        cout << "Error: could not open trace file." << endl;
        return entries;
    }

    Entry e;
    while (file >> e.time >> e.cpu >> e.op >> hex >> e.addr)
    {
        entries.push_back(e);
    }

    return entries;
}

void printTrace(const vector<Entry>& entries)
{
    cout << "\nTRACE ENTRIES\n";

    for (const Entry& e : entries)
    {
        cout << e.time << " " << e.cpu << " " << e.op << " "
             << hex << e.addr << endl;
    }
}

void showTiming(const vector<Entry>& entries)
{
    if (entries.size() < 2)
    {
        cout << "Not enough entries for timing analysis." << endl;
        return;
    }

    int totalGap = 0;
    for (size_t i = 1; i < entries.size(); i++)
    {
        totalGap += entries[i].time - entries[i - 1].time;
    }

    double avgGap = (double)totalGap / (entries.size() - 1);

    cout << "\nTIMING ANALYSIS\n";
    cout << "First access: " << entries.front().time << endl;
    cout << "Last access: " << entries.back().time << endl;
    cout << "Average gap between accesses: " << avgGap << endl;
}

int showMenu()
{
    cout << "\nHARDWARE TRACE ANALYZER\n";
    cout << "1. Load trace file\n";
    cout << "2. Show trace\n";
    cout << "3. Read/write breakdown\n";
    cout << "4. Address breakdown\n";
    cout << "5. CPU breakdown\n";
    cout << "6. Timing analysis\n";
    cout << "7. Exit\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    return choice;
}

void showReadWrite(const vector<Entry>& entries)
{
    int reads = 0, writes = 0;

    for (const Entry& e : entries)
    {
        if (e.op == "READ")  reads++;
        else if (e.op == "WRITE") writes++;
    }

    double readPct = 0, writePct = 0;
    if (!entries.empty())
    {
        readPct = (double)reads / entries.size() * 100;
        writePct = (double)writes / entries.size() * 100;
    }

    cout << "\nREAD/WRITE BREAKDOWN\n";
    cout << "Total accesses: " << dec << entries.size() << endl;
    cout << "Reads: " << reads << endl;
    cout << "Writes: " << writes << endl;
    cout << "Read %: " << readPct << "%" << endl;
    cout << "Write %: " << writePct << "%" << endl;
}

void showAddresses(const vector<Entry>& entries)
{
    unordered_map<unsigned int, int> counts;

    for (const Entry& e : entries)
    {
        counts[e.addr]++;
    }

    cout << "\nADDRESS BREAKDOWN\n";
    for (const auto& pair : counts)
    {
        cout << "0x" << hex << pair.first << " -> " << dec << pair.second
             << " accesses" << endl;
    }

    unsigned int topAddr = 0;
    int topCount = 0;
    for (const auto& pair : counts)
    {
        if (pair.second > topCount)
        {
            topCount = pair.second;
            topAddr = pair.first;
        }
    }

    cout << "\nMOST ACCESSED ADDRESS\n";
    cout << "Address: 0x" << hex << topAddr << endl;
    cout << "Accesses: " << dec << topCount << endl;
}

void showCpuBreakdown(const vector<Entry>& entries)
{
    unordered_map<string, int> counts;

    for (const Entry& e : entries)
    {
        counts[e.cpu]++;
    }

    cout << "\nCPU BREAKDOWN\n";
    for (const auto& pair : counts)
    {
        cout << pair.first << " -> " << pair.second << " accesses" << endl;
    }
}

int main()
{
    cout << "Hardware Trace Analyzer" << endl;

    vector<Entry> trace;

    while (true)
    {
        int choice = showMenu();

        switch (choice)
        {
            case 1:
            {
                string filename;
                cout << "Enter trace file path: ";
                cin >> filename;

                vector<Entry> loaded = loadTrace(filename);
                if (loaded.empty())
                {
                    cout << "No trace entries loaded." << endl;
                }
                else
                {
                    trace = loaded;
                    cout << "Trace loaded! Entries: " << trace.size() << endl;
                }
                break;
            }

            case 2:
                if (trace.empty()) cout << "Load a trace file first." << endl;
                else printTrace(trace);
                break;

            case 3:
                if (trace.empty()) cout << "Load a trace file first." << endl;
                else showReadWrite(trace);
                break;

            case 4:
                if (trace.empty()) cout << "Load a trace file first." << endl;
                else showAddresses(trace);
                break;

            case 5:
                if (trace.empty()) cout << "Load a trace file first." << endl;
                else showCpuBreakdown(trace);
                break;

            case 6:
                if (trace.empty()) cout << "Load a trace file first." << endl;
                else showTiming(trace);
                break;

            case 7:
                cout << "Exiting..." << endl;
                return 0;

            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }
}