#include <iostream>
#include <bits/getopt_core.h>
#include <cassert>
#include <string>
#include <vector>
#include <filesystem> 
#include <fstream> 
#include <algorithm>
#include <cctype>


using namespace std;
namespace fs = std::filesystem;
int getoptions(int argc, char **argv, int &recursive, int &case_sensitive, string &searchpath, vector<string> &filenames);

char *program_name = NULL;

void print_usage() {
    cout << "Usage: /myfind [-R] [-i] searchpath filename1 [filename2] …[filenameN]\n";
    cout << "Example: ./myfind ./ test.txt test.doc test\n";
}

void toLower(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}


void find(string &searchpath, vector<string> &filenames, int recursive, int case_sensitive) {

    for(string filename: filenames) {
        //cout << "search: " << filename << endl;
        if(case_sensitive){
            toLower(filename);
        }
        //cout << "Check ob toLower Funktioniert -> müsste lowercase sein: " << filename << endl;
        for (const auto & entry : fs::directory_iterator(searchpath)) {
            //std::cout << entry.path() << std::endl;

            string found_filename = entry.path().filename();
            if(case_sensitive) {
                toLower(found_filename);
            }
            
            if(found_filename == filename) {
                cout << "Found: " << filename << endl;
            }

            
        }
    }
}

int main(int argc, char **argv)
{

    int recursive = 0;
    int case_sensitive = 0;
    int c;
    int error = 0;

    string searchpath = "";
    vector<string> filenames = {};

    program_name = argv[0];

    error = getoptions(argc, argv, recursive, case_sensitive, searchpath, filenames);

    if (error)
    {
        return 1; // handle error appropriately
    }

    cout << searchpath << endl;
    cout << recursive << ", " << case_sensitive << endl;

    for (string file : filenames)
    {
        cout << file << endl;
    }

    //vector<string> test = vector<string> {"test"};
    find(searchpath, filenames, recursive, case_sensitive);

    return 0;
}

int getoptions(int argc, char **argv, int &recursive, int &case_sensitive, string &searchpath, vector<string> &filenames)
{
    int c;
    int error = 0;

    program_name = argv[0];

    // First while loop: Parsing options
    while ((c = getopt(argc, argv, "Ri:")) != EOF) /* Alle Buchstaben die kommen*/
    {
        switch (c)
        {
        case 'R':          /* Option without argument */
            if (recursive) /* multiple uses? */
            {
                error = 1;
                break;
            }
            recursive = 1;
            printf("%s: parsing option R\n", program_name);
            break;
        case 'i':               /* case insensitive */
            if (case_sensitive) /* multiple uses? */
            {
                error = 1;
                break;
            }
            case_sensitive = 1;
            printf("%s: parsing option i\n", program_name);
            break;
        case '?': /* invalid argument */
            error = 1;
            break;
        default: /* impossible */
            assert(0);
        }
    }

    if (error) /* options error? */
    {
        print_usage();
    }

    if ((argc < optind + 1) || (argc > optind + 2)) /* wrong number of options */
    {
        print_usage();
    }

    if(recursive || case_sensitive) {
        optind--;
    }

    if (optind < argc)
    {
        printf("%s: parsing argument %s\n", program_name, argv[optind]);
        searchpath = argv[optind];
        optind++;
    }

    // Second while loop: Parsing remaining arguments
    while (optind < argc)
    {
        printf("%s: parsing argument %s\n", program_name, argv[optind]);
        filenames.push_back(argv[optind]);
        optind++;
    }

    return error;
}