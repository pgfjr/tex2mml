/*
//  Copyright (c) 2020 Peter Frane Jr. All Rights Reserved.
//
//  Use of this source code is governed by the GPL v. 3.0 license that can be
//  found in the LICENSE file.
//
//  This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
//  OF ANY KIND, either express or implied.
//
//  For inquiries, email the author at pfranejr AT hotmail.com
*/

#pragma once
#include <string>

using namespace std;

bool convertFormula( const char *input, int len, int *errorIndex, int *errCode );
const char *getMathMLOutput();
bool getMathMLOutput(string &buf, bool display);
const char *getLastError();

extern "C"
{
	bool fntex2mml(const char* input, string& output, int* error_pos, bool display_style, string& error_msg);
}