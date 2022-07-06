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

#include "tex2mml.h"

bool fntex2mml(const char* input, string& output, int* error_pos, bool display_style, string& error_msg)
{
	if (!input)
	{
		error_msg = "Nothing to convert";

		return false;
	}
	else
	{
		int error_code;

		if (convertFormula(input, -1, error_pos, &error_code))
		{
			if (getMathMLOutput(output, display_style))
			{
				error_msg.clear();

				return true;
			}
			else
			{
				error_msg = "Empty";

				return false;
			}
		}
		else
		{
			error_msg = getLastError();

			return false;
		}
	}
}
