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

#ifndef __table
#define __table

#include <string.h>
#include "exceptions.h"

enum command_id {
	 ci_unknown, ci_msub, ci_msup, ci_msubsup, ci_munder, ci_mover, ci_munderover,
	 
	 ci_mi,  ci_mn, ci_mo, ci_text, ci_cfrac, ci_mfrac, ci_frac, ci_mathfont, ci_sqrt, ci_begin, ci_end, 
	 ci_array, ci_eqnarray, ci_cases, ci_matrix, ci_bmatrix, ci_Bmatrix, ci_pmatrix, ci_vmatrix, 
	 ci_Vmatrix, ci_mathop, ci_accent, ci_ext_arrows,
	 ci_func, ci_binom, ci_stack, ci_stackrel, ci_hfill, ci_limits, ci_nolimits,
	 ci_menclose, ci_strut, ci_phantom, ci_left, ci_right, ci_underoverbrace, 
	 ci_mathstring, ci_lsub, ci_lsup, ci_lsubsup, ci_eqno, ci_leqno, ci_mathord, ci_mathbin, ci_mathrel
};

enum math_type { mt_unknown, mt_ident, mt_digit, mt_ord, 
				 mt_bin, mt_unary, mt_bin_unary, mt_rel, 
				 mt_left_fence, mt_right_fence, mt_fence, 
				 mt_mov_limits, mt_limits, mt_func, mt_func_limits, 
				 mt_text, mt_punct };

enum param_type { pt_unknown, pt_none, pt_plain, pt_one, pt_two, pt_three, pt_table, pt_others,
				  pt_especial };

enum token_type {	 
	token_eof   = -1,
	token_unknown = 0,
	token_alpha, 
	token_digit, 
	token_symbol, 
	token_white_space, 
	token_left_brace, 
	token_right_brace, 
	token_right_sq_bracket,
	token_superscript, 
	token_subscript, 
	token_column_sep, 
	token_row_sep, 
	token_control_symbol, 
	token_control_name,
	token_control_command,
	token_control_entity,
	token_control_function,
	token_inline_math,
	token_prime
};

struct SymbolTable {
	union {
		char const *tagOn;
		char const*key;
		char const*name;
		//exception code;
	};
	union {
		char const*tagOff;
		char const*value;
		math_type mathType;
		//char *errorMsg;
	};
};

struct ErrorTable {
	ex_exception code;
	char const *msg;
};

struct EntityStruct {
	char const *name;	
	unsigned int code;
	math_type mathType;	
};

struct FunctionStruct {
	char const *name;	
	char const *output;
	math_type mathType;	
};

struct FenceStruct {
	EntityStruct *entity;	
	char output[20];	
};

struct CommandStruct {
	char const *name;
	command_id id;
	param_type param;
	char const *tagOn;
	char const *tagOff;
};

struct EnvironmentStruct {
	char const *name;
	command_id id;
	char const *tagOn;
	char const*tagOff;
};

struct SymbolStruct {
	char const* name;
	char const* literal;
	char const* element;
	math_type mathType;
};

struct ControlStruct {
	CommandStruct *command;
	token_type token;
	char *start;
	union {
		EntityStruct *entity;
		FunctionStruct *function;		
	};
};


token_type getControlType(const char *name, ControlStruct &control );
const char *getErrorMsg( ex_exception code );
const char *getMathVariant(const char *attrib );
bool getFenceType(const char *name,  FenceStruct &fence );
EnvironmentStruct *getEnvironmentType(const char *name );
SymbolStruct *getSymbol(const char *name );
#endif
