// testmml.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "tex2mml.h"

#pragma comment(lib, "tex2mml.lib")

//bool fntex2mml(const char *input, string &output, int *error_pos, bool display_style, string &error_msg )
int main()
{
    const char* input = "\\frac 1 2 \\sqrt{ABc}";
    string output, error_msg;
    int error_pos{ 0 };
    
    if (fntex2mml(input, output, &error_pos, true, error_msg))
    {
        cout << output;
    }
    else
    {
        cout << error_msg;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
