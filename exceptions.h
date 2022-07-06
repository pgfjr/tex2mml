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

#ifndef __exception
#define __exception


enum ex_exception { 
	ex_syntax_error,
	ex_out_of_memory,						//"Out of memory"},
	ex_missing_lbrace,						//"Missing '{'"},
	ex_prefix_superscript,					//"Illegal prefix superscript: use the '\\lsup' command"},
	ex_prefix_subscript,					//"Illegal prefix superscript: use the '\\lsub' command"},
	ex_misplaced_column_separator,			//"Misplaced column separator"},		
	ex_more_rbrace_than_lbrace,				//"Syntax error: more '}' than '{'"},
	ex_control_name_too_long,				//"Control name too long: maximum is 32"},
	ex_misplaced_row_separator,				//"Misplaced row separator"},
	ex_illegal_subscript,					//"Illegal subscript"},
	ex_illegal_superscript,					//"Illegal superscript"},
	ex_undefined_control_sequence,			//"Undefined control sequence"},
	ex_misplaced_inline_formula,			//"Misplaced inline formula"},
	ex_missing_parameter,					//"Missing parameter"},
	ex_more_lbrace_than_rbrace,				//"Syntax error: more '{' than '}'"},
	ex_double_superscript,					//"Double superscript"},
	ex_double_subscript,					//"Double subscript"},
	ex_use_subscript_before_superscript,    //"Use subscript first as the element is <msubsup>"},
	ex_internal_error,						//"Internal error"},
	ex_missing_end_tag,						//"Missing end tag"},
	ex_undefined_environment_type,			//"Undefined environment type"},
	ex_unknown_alignment_character,			//"Unknown alignment character"},
	ex_missing_begin,						//"Missing \\begin"},
	ex_missing_end,							//"Missing \\end"},
	ex_mismatched_environment_type,         //"Mismatched environment type"}
	ex_too_many_columns,
	ex_unknown_attribute,
	ex_misplaced_limits,
	ex_misplaced_nolimits,
	ex_no_command_allowed,
	ex_missing_fence_parameter,
	ex_not_math_mode,
	ex_missing_right_sq_bracket,
	ex_missing_dollar_symbol,
	ex_missing_left_fence,
	ex_missing_right_fence,
	ex_ambiguous_script,
	ex_misplaced_eqno,
	ex_duplicate_eqno,
	ex_missing_column_alignment,
	ex_missing_subsup_base,
	ex_unknown_character,
	ex_unhandled_mathtype
};

#endif