#include <iostream>
#include <bits/getopt_core.h>
#include <cassert>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <sys/wait.h>

int pip[2];

using namespace std;
namespace fs = std::filesystem;

int getoptions(int argc, char **argv, int &recursive, int &case_sensitive, string &searchpath, vector<string> &filenames);
bool search_file(string &searchpath, string filename, int recursive, int case_sensitive);

char *program_name = NULL;

void print_usage()
{
    cout << "Usage: /myfind [-R] [-i] searchpath filename1 [filename2] …[filenameN]\n";
    cout << "Example: ./myfind ./ test.txt test.doc test\n";
}

void toLower(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return std::tolower(c); });
}

void find(string &searchpath, vector<string> &filenames, int recursive, int case_sensitive)
{
    int fork_count = 0;
    pid_t wpid, child_pid;

    for (string filename : filenames)
    {
        if ((child_pid = fork()) == 0)
        {
            // Child process
            fork_count++;
            bool found = false;

            if (!case_sensitive)
            {
                toLower(filename);
            }

            found = search_file(searchpath, filename, recursive, case_sensitive);

            _exit(0); // Ensure the child process exits
        }
    }

    // Parent process waits for all child processes to finish
    while ((wpid = waitpid(-1, NULL, 0)) > 0);
}

bool search_file(string &searchpath, string filename, int recursive, int case_sensitive)
{
    vector<string> folders = {};
    bool found = false;
    try {
        
    
    for (const auto &entry : fs::directory_iterator(searchpath))
    {
        string found_filename = entry.path().filename();

        if (entry.is_directory() && recursive)
        {
            folders.push_back(entry.path());
        }

        if (!case_sensitive)
        {
            toLower(found_filename);
        }

        if (found_filename == filename)
        {
            //cout << getpid() << ": " << filename << ": " << fs::absolute(entry.path()) << endl;
            string write_path = to_string(getpid()) + ": " + filename + ": " + fs::absolute(entry.path()).string() + "\n";
           // cout << write_path;
            close(pip[0]);
            write(pip[1], write_path.c_str(), write_path.size());
            found = true;
        }
    }
    } catch (const std::filesystem::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
        return false;
    }
    catch (const std::exception& e) {
        cerr << "General error: " << e.what() << '\n';
        return false;
    }

    if (!recursive)
    {
        return found;
    }

    for (string folder : folders)
    {
        found = found ? true : search_file(folder, filename, recursive, case_sensitive);
    }

    return found;
}

int getoptions(int argc, char **argv, int &recursive, int &case_sensitive, string &searchpath, vector<string> &filenames)
{
    int c;
    int error = 0;

    program_name = argv[0];

    // First while loop: Parsing options
    while ((c = getopt(argc, argv, "Ri:::")) != -1)
    {
        switch (c)
        {
        case 'R':
            if (recursive)
            {
                error = 1;
            }
            else
            {
                recursive = 1;
                //printf("%s: parsing option R\n", program_name);
            }
            break;
        case 'i':
            if (case_sensitive)
            {
                error = 1;
            }
            else
            {
                case_sensitive = 1;
                //printf("%s: parsing option i\n", program_name);
            }
            break;
        case '?':
            error = 1;
            break;
        default:
            assert(0);
        }
    }

    if (error)
    {
        print_usage();
        return 1;
    }

    if (argc <= 2)
    {
        print_usage();
        return 1;
    }

    if (optind < argc)
    {
        //printf("%s: parsing argument %s\n", program_name, argv[optind]);
        searchpath = argv[optind];
        optind++;
    }

    // Second while loop: Parsing remaining arguments
    while (optind < argc)
    {
        //printf("%s: parsing argument %s\n", program_name, argv[optind]);
        filenames.push_back(argv[optind]);
        optind++;
    }

    return error;
}

/**
 synchronouized output is achived by using a pipe. For each filename a process starts that pushes its result into the pipe and then terminates. 
 At the end of the main we print the content of the pipe.
 */
int main(int argc, char **argv)
{
    char readBuffer[1000];
    int nbytes;
    int recursive = 0;
    int case_sensitive = 0;
    int c;
    int error = 0;

    pipe(pip);

    string searchpath = "";
    vector<string> filenames = {};

    program_name = argv[0];

    error = getoptions(argc, argv, recursive, case_sensitive, searchpath, filenames);

    if (error)
    {
        return 1; // handle error appropriately
    }

    find(searchpath, filenames, recursive, case_sensitive);

    //cout << "Ausgabe erfolgt nun von der pipe:" << endl;

    close(pip[1]);
    while ((nbytes = read(pip[0], readBuffer, sizeof(readBuffer) - 1)) > 0) {
        readBuffer[nbytes] = '\0'; // Null-terminate the buffer
        printf("%s", readBuffer);
    }
    

    return 0;
}