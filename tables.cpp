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

#include "tables.h"
#include <stdio.h>
	
static CommandStruct commandTable[] = {
{"Overleftarrow",			ci_accent,pt_one, "<mover accent='true'>", "<mo stretchy='true'>&#x21D0;</mo></mover>" },
{"Overleftrightarrow",		ci_accent,pt_one, "<mover accent='true'>", "<mo stretchy='true'>&#x21D4;</mo></mover>" },
{"Overrightarrow",			ci_accent,pt_one, "<mover accent='true'>", "<mo stretchy='true'>&#x21D2;</mo></mover>" },
{"actuarial",				ci_menclose,    pt_one,"<menclose notation='actuarial'>",  "</menclose>" },
{"acute",					ci_accent,pt_one, "<mover accent='true'>", "<mo>&#x00B4;</mo></mover>" },
{"bar",						ci_accent,pt_one, "<mover accent='true'>", "<mo stretchy='false'>&#x00AF;</mo></mover>" },
{"begin",					ci_begin,pt_especial,"", "" },
{"binom",					ci_binom,pt_two, "<mfenced><mrow><mfrac linethickness='0'>","</mfrac></mrow></mfenced>" },
{"breve",					ci_accent,pt_one, "<mover accent='true'>", "<mo>&#x02D8;</mo></mover>" },
{"cfrac",					ci_cfrac,pt_especial,    "<mfrac>", "</mstyle></mfrac>" },
{"check",					ci_accent,pt_one, "<mover accent='true'>", "<mo>&#x02C7;</mo></mover>" },
{"ddddot",					ci_accent,pt_one, "<mover accent='true'>", "<mo>&#x00A8;&#x00A8;</mo></mover>" },
{"dddot",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x20DB;</mo></mover>" },
{"ddot",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x00A8;</mo></mover>" },
{"dfrac",	ci_mfrac,	pt_two,	"<mstyle displaystyle='true' scriptlevel='0'><mfrac>", "</mfrac></mstyle>" },
{"dot",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x02D9;</mo></mover>" },
{"end",	ci_end,	pt_especial,	"","" },
{"eqno",	ci_eqno,	pt_especial,	"<mtable><mlabeledtr><mtd><mtext>", "</mtext></mtd><mtd>" },
{"frac",	ci_frac,	pt_two,	"<mfrac>", "</mfrac>" },
{"func",	ci_func,	pt_plain,	"<mi>", "</mi>" },
{"grave",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x0300;</mo></mover>" },
{"hat",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x02c6;</mo></mover>" },
{"hfill",	ci_hfill,	pt_especial,	"","" },
{"hphantom",	ci_phantom,	pt_one,	"<mphantom><mpadded height='0%' depth='0'>", "</mpadded></mphantom>" },
{"hungarumlaut",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x02DD;</mo></mover>" },
{"left",	ci_left,	pt_especial,	"<mfenced>", "</mrow></mfenced>" },
{"leqno",	ci_eqno,	pt_especial,	"<mtable side='left'><mlabeledtr><mtd><mtext>", "</mtext></mtd><mtd>" },
{"limits",	ci_limits,	pt_especial,	"","" },
{"longdiv",	ci_menclose,	pt_one,	"<menclose notation='longdiv'>", "</menclose>" },
{"lsub",	ci_lsub,	pt_especial,	"<mmultiscripts>", "</mmultiscripts>" },
{"lsubsup",	ci_lsubsup,	pt_especial,	"<mmultiscripts>", "</mmultiscripts>" },
{"lsup",	ci_lsup,	pt_especial,	"<mmultiscripts>", "</mmultiscripts>" },
{"mathbb",	ci_mathfont,	pt_plain,	"<mi mathvariant='double-struck'>", "</mi>" },
{"mathbf",	ci_mathfont,	pt_plain,	"<mi mathvariant='bold'>", "</mi>" },
{"mathbfrak",	ci_mathfont,	pt_plain,	"<mi mathvariant='bold-fraktur'>", "</mi>" },
{"mathbi",	ci_mathfont,	pt_plain,	"<mi mathvariant='bold-italic'>", "</mi>" },
{"mathbin",	ci_mathbin,	pt_plain,	"<mo lspace='.222222em' rspace='.222222em'>", "</mo>" },
{"mathbsc",	ci_mathfont,	pt_plain,	"<mi mathvariant='bold-script'>", "</mi>" },
{"mathbss",	ci_mathfont,	pt_plain,	"<mi mathvariant='bold-sans-serif'>", "</mi>" },
{"mathfrak",	ci_mathfont,	pt_plain,	"<mi mathvariant='fraktur'>", "</mi>" },
{"mathit",	ci_mathfont,	pt_plain,	"<mi mathvariant='italic'>", "</mi>" },
{"mathop",	ci_mathop,	pt_plain,	"<mo>", "</mo>" },
{"mathord",	ci_mathord,	pt_plain,	"<mo lspace='0' rspace='0'>", "</mo>" },
{"mathord",	ci_mathord,	pt_plain,	"<mo lspace='0' rspace='0'>", "</mo>"},
{"mathrel",	ci_mathrel,	pt_plain,	"<mo lspace='.27777em' rspace='.27777em'>", "</mo>" },
{"mathring",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x02DA;</mo></mover>" },
{"mathrm",	ci_mathfont,	pt_plain,	"<mi mathvariant='normal'>", "</mi>" },
{"mathsc",	ci_mathfont,	pt_plain,	"<mi mathvariant='script'>", "</mi>" },
{"mathss",	ci_mathfont,	pt_plain,	"<mi mathvariant='sans-serif'>", "</mi>" },
{"mathssbi",	ci_mathfont,	pt_plain,	"<mi mathvariant='sans-serif-bold-italic'>", "</mi>" },
{"mathssi",	ci_mathfont,	pt_plain,	"<mi mathvariant='sans-serif-italic'>", "</mi>" },
{"mathstrut",	ci_strut,	pt_especial,	"<mphantom><mpadded width='0%' lspace='0'><mo>(</mo></mpadded></mphantom>","" },
{"mathtt",	ci_mathfont,	pt_plain,	"<mi mathvariant='monospace'>", "</mi>" },
{"mn",	ci_mn,	pt_plain,	"<mn>", "</mn>" },
{"mo",	ci_mo,	pt_plain,	"<mo>", "</mo>" },
{"ms",	ci_mathstring,	pt_especial,	"<ms>", "</ms>" },
{"nolimits",	ci_limits,	pt_especial,	"","" },
{"overbrace",	ci_underoverbrace,	pt_especial,	"<mover accent='false'>", "<mo stretchy='true'>&#xFE37;</mo></mover>" },
{"overbrack",	ci_accent,	pt_one,	"<mover accent='false'>", "<mo stretchy='true'>&#x23B4;</mo></mover>" },
{"overleftarrow",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='true'>&#x2190;</mo></mover>" },
{"overleftrightarrow",	ci_accent,	pt_one,  	"<mover accent='true'>", "<mo stretchy='true'>&#x2194;</mo></mover>" },
{"overline",	ci_accent,	pt_one,	"<mover accent='false'>", "<mo stretchy='true'>&#x00AF;</mo></mover>" },
{"overparen",	ci_accent,	pt_one,	"<mover accent='false'>", "<mo stretchy='true'>&#x2322;</mo></mover>" },
{"overrightarrow",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='true'>&#x2192;</mo></mover>" },
{"phantom",	ci_phantom,	pt_one,	"<mphantom>", "</mphantom>" },
{"qdot",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x00A8;&#x00A8;</mo></mover>" },
{"right",	ci_right,	pt_especial,	"","" },
{"sqrt",	ci_sqrt,	pt_especial,	"<msqrt>", "</msqrt>" },
{"stack",	ci_stack,	pt_two,	"<mfrac linethickness='0'>", "</mfrac>" },
{"stackrel",	ci_stackrel,	pt_especial,	"<mover>", "</mover>" },
{"strut",	ci_strut,	pt_especial,	"<mspace width='0pt' height='8.5pt' depth='3.5pt'/>","" },
{"tbinom",	ci_binom,	pt_two,	"<mstyle scriptlevel='1'><mfenced><mrow><mfrac linethickness='0'>", "</mfrac></mrow></mfenced></mstyle>" },
{"tdot",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo>&#x20DB;</mo></mover>" },
{"text",	ci_text,	pt_especial,	"<mtext>", "</mtext>" },
{"textbf",	ci_text,	pt_especial,	"<mtext mathvariant='bold'>", "</mtext>" },
{"textbi",	ci_text,	pt_especial,	"<mtext mathvariant='bold-italic'>", "</mtext>" },
{"textbsf",	ci_text,	pt_especial,	"<mtext mathvariant='bold-sans-serif'>", "</mtext>" },
{"textit",	ci_text,	pt_especial,	"<mtext mathvariant='italic'>", "</mtext>" },
{"textsf",	ci_text,	pt_especial,	"<mtext mathvariant='sans-serif'>", "</mtext>" },
{"textsfbi",	ci_text,	pt_especial,	"<mtext mathvariant='sans-serif-bold-italic'>", "</mtext>" },
{"textsfit",	ci_text,	pt_especial,	"<mtext mathvariant='sans-serif-italic'>", "</mtext>" },
{"texttt",	ci_text,	pt_especial,	"<mtext mathvariant='monospace'>", "</mtext>" },
{"tfrac",	ci_mfrac,	pt_two,	"<mstyle displaystyle='false' scriptlevel='0'><mfrac>", "</mfrac></mstyle>" },
{"tilde",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='false'>&#x02DC;</mo></mover>" },
{"underbrace",	ci_underoverbrace,	pt_especial,	"<munder accentunder='false'>", "<mo stretchy='true'>&#xFE38;</mo></munder>" },
{"underbrack",	ci_accent,	pt_one,	"<munder accentunder='false'>", "<mo stretchy='true'>&#x23B5;</mo></munder>" },
{"underleftarrow",	ci_accent,	pt_one,	"<munder accentunder='true'>", "<mo stretchy='true'>&#x2190;</mo></munder>" },
{"underleftrightarrow",	ci_accent,	pt_one,	"<munder accentunder='true'>", "<mo stretchy='true'>&#x2194;</mo></munder>" },
{"underline",	ci_accent,	pt_one,	"<munder accentunder='false'>", "<mo stretchy='true'>&#x0332;</mo></munder>" },
{"underparen",	ci_accent,	pt_one,	"<munder accentunder='false'>", "<mo stretchy='true'>&#xF611;</mo></munder>" },
{"underrightarrow",	ci_accent,	pt_one,	"<munder accentunder='true'>", "<mo stretchy='true'>&#x2192;</mo></munder>" },
{"undertilde",	ci_accent,	pt_one,	"<munder accentunder='false'>", "<mo stretchy='true'>&#x02DC;</mo></munder>" },
{"vec",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='false'>&#x2192;</mo></mover>" },
{"vphantom",	ci_phantom,	pt_one,	"<mphantom><mpadded width='0%' lspace='0'>", "</mpadded></mphantom>" },
{"widehat",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='true'>&#x0302;</mo></mover>" },
{"widetilde",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='true'>&#x02DC;</mo></mover>" },
{"widevec",	ci_accent,	pt_one,	"<mover accent='true'>", "<mo stretchy='true'>&#x2192;</mo></mover>" },
{"xleftarrow",	ci_ext_arrows,	pt_especial,	"<mo stretchy='true'>&#x2190;</mo>", "</mover>" },
{"xleftrightarrow",	ci_ext_arrows,	pt_especial,	"<mo stretchy='true'>&#x2194;</mo>", "</mover>"},
{"xrightarrow",	ci_ext_arrows,	pt_especial,	"<mo stretchy='true'>&#x2192;</mo>", "</mover>" }
};

struct EnvironmentStruct environmentTable[] = {
	{ "array",	  ci_array,	   "<mtable><mtr><mtd>", "</mtd></mtr></mtable>" },
	{ "bmatrix",  ci_bmatrix,   "<mfenced open='[' close=']' separators=''><mtable><mtr><mtd>", "</mtd></mtr></mtable></mfenced>" },
	{ "cases",    ci_cases,     "<mfenced open='{' close='' separators=''><mtable><mtr><mtd>", "</mtd></mtr></mtable></mfenced>" },
	{ "eqnarray", ci_eqnarray, "<mtable columnalign='right center left' columnspacing='.222222em'><mtr><mtd>", "</mtd></mtr></mtable>" },
	{ "matrix",	  ci_matrix,   "<mtable><mtr><mtd>", "</mtd></mtr></mtable>" },
	{ "pmatrix",  ci_pmatrix,   "<mfenced separators=''><mtable><mtr><mtd>", "</mtd></mtr></mtable></mfenced>" },
	{ "vmatrix",  ci_vmatrix,   "<mfenced open='|' close='|' separators=''><mtable><mtr><mtd>", "</mtd></mtr></mtable></mfenced>" },
	{ "Bmatrix",  ci_Bmatrix,   "<mfenced open='{' close='}' separators=''><mtable><mtr><mtd>", "</mtd></mtr></mtable></mfenced>" },	
	{ "Vmatrix",  ci_Vmatrix,   "<mfenced open='&#x2016;' close='&#x2016;' separators=''><mtable><mtr><mtd>", "</mtd></mtr></mtable></mfenced>" }
	
};

static FunctionStruct functionTable[] = {
	{ "Pr",			"Pr",  				  mt_func_limits},
	{ "arccos",		"arccos", 			  mt_func},
	{ "arcsin",		"arcsin",  			  mt_func},
	{ "arctan",		"arctan", 			  mt_func},
	{ "arg",		"arg",    			  mt_func},
	{ "cos",		"cos",    			  mt_func},
	{ "cosh",		"cosh",   			  mt_func},
	{ "cot",		"cot",    			  mt_func},
	{ "coth",		"coth",   			  mt_func},
	{ "csc",		"csc",    			  mt_func},
	{ "deg",		"deg",    			  mt_func},
	{ "det",		"det",  			  mt_func_limits},
	{ "dim",		"dim",    			  mt_func},
	{ "exp",		"exp",    			  mt_func},
	{ "gcd",		"gcd",  			  mt_func_limits},
	{ "hom",		"hom",    			  mt_func},
	{ "inf",		"inf",  			  mt_func_limits},
	{ "ker",		"ker",    			  mt_func},
	{ "lg",			"lg",     			  mt_func},
	{ "lim",		"lim",  			  mt_func_limits},
	{ "liminf",		"lim&#x2009;sup",	  mt_func_limits},		
	{ "limsup",		"lim&#x2009;sup",	  mt_func_limits},		
	{ "ln",			"ln",     			  mt_func},
	{ "log",		"log",    			  mt_func},
	{ "max",		"max",  			  mt_func_limits},
	{ "min",		"min",  			  mt_func_limits},	
	{ "sec",		"sec",    			  mt_func},
	{ "sin",		"sin",     			  mt_func},
	{ "sinh",		"sinh",   			  mt_func},
	{ "sup",		"sup",  			  mt_func_limits},
	{ "tan",		"tan",    			  mt_func},
	{ "tanh",		"tanh",   			  mt_func}
};


static EntityStruct entityTable[] = {

{ "Delta",  	0x394, mt_ident }, 
{ "Gamma",  	0x393, mt_ident }, 
{ "Lambda",  0x39B, mt_ident }, 
{ "Omega",  0x3A9, mt_ident }, 
{ "Phi",  0x3A6, mt_ident }, 
{ "Psi",  0x3A8, mt_ident }, 
{ "Sigma",  0x3A3, mt_ident }, 
{ "Xi",  0x39E, mt_ident }, 
{ "alpha",		0x3B1,	mt_ident },
{ "ast",  		0x2A, mt_bin }, 
{ "beta",		0x3B2,	mt_ident  },
{ "bigcap",		0x22C2, mt_mov_limits  },

{ "bigcup",		0x22C3, mt_mov_limits  },

{ "bigodot",	0x2299,	mt_mov_limits  },

{ "bigoplus",	0x2295,	mt_mov_limits  },

{ "bigotimes",	0x2297,	mt_mov_limits  },

{ "bigsqcup",	0x2A06,	mt_mov_limits  },

{ "biguplus",	0x2A04,	mt_mov_limits  },

{ "bigvee",		0x22C1,	mt_mov_limits  },

{ "bigwedge",	0x22C0,	mt_mov_limits  },

{ "cdot",  		0xB7, mt_bin }, 
{ "cdots",  	0x22EF, mt_ord }, 
{ "centerdot",  0xB7, mt_bin }, 
{ "chi",  		0x3C7, mt_ident }, 
{ "coprod",		0x2210, mt_mov_limits  },

{ "ddots",  	0x22F1, mt_ord }, 
{ "delta",  	0x3B4, mt_ident }, 
{ "div",  		0xF7, mt_bin }, 
{ "epsilon",  	0x3B5, mt_ident }, 
{ "gamma",  	0x3B3, mt_ident }, 
{ "ge",  		0x2265, mt_rel }, 
{ "iiiint",		0x2A0C,	mt_limits   },

{ "iiint",		0x222D,	mt_limits   },

{ "iint",		0x222C,	mt_limits   },

{ "infinity",  0x221E, mt_rel }, 
{ "int",		0x222B,	mt_limits   },

{ "kappa",  0x3BA, mt_ident }, 
{ "lambda",  0x3BB, mt_ident }, 
{ "ldots",  0x2026, mt_ord }, 
{ "le",  0x2264, mt_rel }, 
{ "leftarrow",  0x2190, mt_rel }, 



{ "mu",  0x3BC, mt_ident }, 

{ "nabla",  0x2207, mt_ident }, 
{ "nu",  0x3BD, mt_ident }, 
{ "oint",		0x222E,	mt_limits   },


{ "omega",  0x3C9, mt_ident }, 
{ "partial",  0x2202, mt_ord }, 
{ "phi",  0x3C6, mt_ident }, 
{ "pi",  0x3C0, mt_ident }, 
{ "pm",  0xB1, mt_rel }, 

{ "prod",		0x220f,	mt_mov_limits  },
{ "psi",  0x3C8, mt_ident }, 

{ "rho",  0x3C1, mt_ident }, 
{ "rightarrow",  0x2192, mt_rel }, 
{ "sigma",  0x3C3, mt_ident }, 
{ "sim",  0x223C, mt_rel }, 
{ "sum",		0x2211,	mt_mov_limits  },

{ "tau",  0x3C4, mt_ident }, 
{ "theta",  0x3B8, mt_ident }, 
{ "times",  0xD7, mt_bin }, 
{ "to",  0x2192, mt_rel }, 
{ "upsilon",  0x3C5, mt_ident },
{ "varepsilon",  0x3B5, mt_ident },
{ "vdots",  0x22EE, mt_ord }, 
{ "xi",  0x3BE, mt_ident },
{ "zeta",  0x3B6, mt_ident } 


};

static ErrorTable errorTable[] = {
	{ ex_out_of_memory,							"Out of memory" },
	{ ex_missing_lbrace,						"Missing '{'" },
	{ ex_prefix_superscript,					"Illegal prefix superscript: use the '\\lsup' command" },
	{ ex_prefix_subscript,						"Illegal prefix subscript: use the '\\lsub' command" },
	{ ex_misplaced_column_separator,			"Misplaced column separator" },		
	{ ex_more_rbrace_than_lbrace,				"Syntax error: more '}' than '{'" },
	{ ex_control_name_too_long,					"Control name too long: maximum is 32" },
	{ ex_misplaced_row_separator,				"Misplaced row separator" },
	{ ex_illegal_subscript,						"Illegal subscript" },
	{ ex_illegal_superscript,					"Illegal superscript" },
	{ ex_undefined_control_sequence,			"Undefined control sequence" },
	{ ex_misplaced_inline_formula,				"Misplaced inline formula" },
	{ ex_missing_parameter,						"Missing parameter" },
	{ ex_more_lbrace_than_rbrace,				"Syntax error: more '{' than '}'" },	
	{ ex_double_superscript,					"Double superscript" },
	{ ex_double_subscript,						"Double subscript" },
	{ ex_use_subscript_before_superscript,      "Use subscript first as the element is <msubsup>" },
	{ ex_internal_error,						"Internal error" },
	{ ex_missing_end_tag,						"Missing end tag" },
	{ ex_undefined_environment_type,			"Undefined environment type" },
	{ ex_unknown_alignment_character,			"Unknown alignment character" },
	{ ex_missing_begin,							"Missing \\begin" },
	{ ex_missing_end,							"Missing \\end" },
	{ ex_mismatched_environment_type,           "Mismatched environment type"},
	{ ex_too_many_columns,						"Too many columns" },
	{ ex_unknown_attribute,						"Unknown attribute" },
	{ ex_no_command_allowed,					"Command not allowed here" },
	{ ex_misplaced_limits,						"Limit controls must follow a math operator" },
	{ ex_missing_fence_parameter,				"Missing fence parameter" },
	{ ex_not_math_mode,							"Not in math mode" },
	{ ex_missing_right_sq_bracket,				"Missing ']'" },
	{ ex_missing_dollar_symbol,					"Missing '$'" },
	{ ex_missing_left_fence,					"Missing \\left" },
	{ ex_missing_right_fence,					"Missing \\right" },
	{ ex_ambiguous_script,						"Ambiguous script; use \\left and \\right" },
	{ ex_misplaced_eqno,						"Equation number not allowed here" },
	{ ex_duplicate_eqno,						"Duplicate equation number" },
	{ ex_missing_column_alignment,				"Missing column alignment" },
	{ ex_missing_subsup_base,					"Missing subscript/superscript base" },
	{ ex_unknown_character,						"Internal error: Unknown character" },
	{ ex_unhandled_mathtype,					"Internal error: unhandled math type" }
	//{ ex_misplaced_nolimits,					"Nolimits control must follow a math operator" }
};

SymbolTable mathvariant[]= {	
	{"bb",		"double-struck"},		
	{"bf",		"bold"},		
	{"bfrak",	"bold-fraktur"},		
	{"bi",		"bold-italic"},		
	{"bsc",		"bold-script"},		
	{"bss",		"bold-sans-serif"},		
	{"frak",	"fraktur"},		
	{"it",		"italic"},		
	{"rm",		"normal"}, 		
	{"sc",		"script"},		
	{"ss",		"sans-serif"},		
	{"ssbi",	"sans-serif-bold-italic"},		
	{"ssi",		"sans-serif-italic"},		
	{"tt",		"monospace"}
};

static EntityStruct fenceTable[] = {

	{ "[",      	    '[',		mt_left_fence },
    { "]",      	    ']',		mt_right_fence },
    { "\\{",      	    '{',		mt_left_fence },
    { "\\}",      	    '}',		mt_right_fence },
    { "/",      	    '/',		mt_ord },
    { "(",      	    '(',		mt_left_fence },
    { ")",      	    ')',		mt_right_fence },
    { "|",				0x007C,		mt_ord },
    { "\\|",			0x2016,		mt_ord },
    { "<",				0x2329,		mt_left_fence },
    { ">",				0x232A,		mt_right_fence },
    { ".",				0,			mt_ord },  //see function below
    { "lgroup",      	'(',		mt_left_fence },
    { "rgroup",      	')',		mt_right_fence },
   	{ "langle",			0x2329,		mt_left_fence },
    { "rangle",			0x232A,		mt_right_fence },
    { "lAngle",			0x300A,		mt_left_fence },
    { "rAngle",			0x300B,		mt_right_fence },
    { "lfloor",			0x230A,		mt_left_fence },//'&lfloor}, //0x230A},
    { "rfloor",			0x230B,		mt_right_fence },//'&rfloor}, //0x230B},
    { "lceil",			0x2308,		mt_left_fence },//'&lceil},  // 0x2308},
    { "rceil",			0x2309,		mt_right_fence },//'&rceil}, // 0x2309},
    { "lbrack",			'[',		mt_left_fence },
    { "rbrack",			']',		mt_right_fence },
    { "lBrack",			0x301A,		mt_left_fence },
    { "rBrack",			0x301B,		mt_right_fence },
    { "lbrace",			'{',		mt_left_fence },
    { "rbrace",			'}',		mt_right_fence },
    { "backslash",		'\\',		mt_ord },
    { "vert",			0x007C,		mt_ord },
    { "Vert",			0x2016,		mt_ord },
    { "uparrow",		0x2191,		mt_ord },
    { "Uparrow",		0x21D1,		mt_ord },
    { "downarrow",		0x2193,		mt_ord },
    { "Downarrow",		0x21D3,		mt_ord },
    { "updownarrow",	0x2195,		mt_ord },
    { "Updownarrow",	0x21D5,		mt_ord },
    { "lmoustache",		0x23B0,		mt_left_fence }, //0x23B0},
    { "rmoustache",		0x23B1,		mt_right_fence }, //0x23B1},
    { "lmoust",  	    0x23B0,		mt_left_fence },
    { "rmoust",			0x23B1,		mt_right_fence }
};

// thickspace .27777 
// medspace	  .222222em
// thinspace	.16667em
static SymbolStruct symbols[]= {
	{"\\ ",		"&#x00a0;",  	"<mspace width='.25em'/>", 			mt_ord },
	{"\\,",		"&#x2006;",  	"<mspace width='.16667em'/>", 		mt_ord },
	{"\\:",		"&#x205f;",  	"<mspace width='.222222em'/>", 		mt_ord },
	{"\\>",		"&#x205f;",  	"<mspace width='.222222em'/>", 		mt_ord },
	{"\\;",		"&#x2005;",  	"<mspace width='.27777em'/>", 		mt_ord },
	{"\\!",		"&#x200a;",  	"<mspace width='-.16667em'/>",  	mt_ord },
	{"\\~",		"&#x00a0;",  	"<mo>&#x00A0;</mo>",  				mt_ord },
	{"\\|",		"&#x2016;", 	"<mo mathsize='1'>&#x2016;</mo>", 	mt_fence },
	{"\\{",		"{",	   		"<mo mathsize='1'>{</mo>", 	        mt_left_fence },
	{"\\}",		"}",	   		"<mo mathsize='1'>}</mo>", 	        mt_right_fence },
	//{"|",		"|",	   		"<mo mathsize='1'>&#x0007C;</mo>", 	mt_ord },
	{"|",		"|",	   		"<mo mathsize='1'>|</mo>", 			mt_ord },
	{"[",		"[",  	   		"<mo mathsize='1'>[</mo>", 			mt_left_fence },
	{"]",		"]",  	   		"<mo mathsize='1'>]</mo>", 			mt_right_fence },
	{"(",		"(",  	   		"<mo mathsize='1'>(</mo>", 			mt_left_fence },
	{")",		")",  	   		"<mo mathsize='1'>)</mo>", 			mt_right_fence },
	{"<",		"<",  	   		"<mo mathsize='1'>&lt;</mo>", 		mt_left_fence },
	{">",		">",  	   		"<mo mathsize='1'>&gt;</mo>", 		mt_right_fence },
	{"-",		"-",			"<mo>&#x2212;</mo>", 				mt_bin_unary },
	{"`",		"&#x0300;",		"<mo>&#x0300;</mo>", 				mt_ord },
	{"@",		"&#x0040;",		"<mo>&#x0040;</mo>",            	mt_ord },
	{"*",		"*",			"<mo>*</mo>",                   	mt_ord },
	{"'",		"&#x02032;",	"<mo>&#x02032;</mo>",           	mt_ord },
	{"''",		"&#x02033;",	"<mo>&#x02033;</mo>",           	mt_ord },
	{"'''",		"&#x02034;",	"<mo>&#x02034;</mo>",           	mt_ord },
	{"\"",		"&#x0201D;",	"<mo>&#x0201D;</mo>",           	mt_ord },
	{"/",		"/",			"<mtext>/</mtext>",             	mt_ord },
	{"\\/",		"&#x200b;",		"<mo>&#x200b;</mo>",            	mt_ord },	
	{"\\%",		"%",			"<mo>%</mo>", 						mt_ord },
	{"\\#",		"&#x00023;",	"<mo>&#x00023;</mo>", 				mt_ord },
	{"\\$",		"$",			"<mtext>$</mtext>", 				mt_ord },
	{"\\^",		"&#x02C6;",		"<mo>&#x02C6;</mo>", 				mt_ord },
	{"\\&",		"&amp;",		"<mo>&amp;</mo>", 					mt_ord },
	{"\\_",		"&#x0005F;",	"<mo>&#x0005F;</mo>", 				mt_ord },
	{"\\-",		"&#x200b;",		"<mo>&#x200b;</mo>", 				mt_ord },
	{"!",		"!",			"<mo>!</mo>",   					mt_ord },
	{"+",		"+",			"<mo>+</mo>",						mt_bin_unary },
	{"=",		"=",			"<mo>=</mo>",						mt_bin },
	{":",		":",			"<mo>:</mo>",						mt_bin },
	{";",		";",			"<mo>;</mo>",						mt_ord },
	{",",		",",			"<mo>,</mo>",						mt_punct },
	{".",		".",			"<mo>.</mo>",						mt_ord },
	{"?",		"?",			"<mo>?</mo>",						mt_ord }
};

#define TABLE_SIZE(x)  ( sizeof( (x) )/sizeof( (x)[0] ))


inline int fastCompare( const char *s1, const char *s2 )
{
	return ( ( *s1 == *s2 ) ? strcmp( s1, s2 ) : *s1 - *s2 );
}


CommandStruct *isCommand( const char *name )
{
	const int size = TABLE_SIZE( commandTable );

	int result, low, mid, high;

	low = 0;
	high = size - 1;

	while( low <= high )
	{
		mid = ( low + high )/2;

		result = fastCompare( name, commandTable[mid].name );

		if( result < 0 )
		{
			high = mid - 1;
		}
		else if( result > 0 )
		{
			low = mid + 1;
		}
		else
		{
			return &commandTable[mid];			
		}
	}

	return NULL;
}


EntityStruct *isEntity( const char *name )
{
	const int size = TABLE_SIZE( entityTable );

	int result, low, mid, high;

	low = 0;
	high = size - 1;

	while( low <= high )
	{
		mid = ( low + high )/2;

		result = fastCompare( name, entityTable[mid].name );

		if( result < 0 )
		{
			high = mid - 1;
		}
		else if( result > 0 )
		{
			low = mid + 1;
		}
		else
		{
			return &entityTable[mid];			
		}
	}

	return NULL;

}


FunctionStruct *isFunction( const char *name )
{
	const int size = TABLE_SIZE( functionTable );

	int result, low, mid, high;

	low = 0;
	high = size - 1;

	while( low <= high )
	{
		mid = ( low + high )/2;

		result = fastCompare( name, functionTable[mid].name );

		if( result < 0 )
		{
			high = mid - 1;
		}
		else if( result > 0 )
		{
			low = mid + 1;
		}
		else
		{
			return &functionTable[mid];			
		}
	}

	return NULL;
}

token_type getControlType( const char *name, ControlStruct &control )
{
	
	if( ( control.command = isCommand( name ) ) != NULL )
	{
		control.token = token_control_command;		
	}
	else if( ( control.entity = isEntity( name ) ) != NULL )
	{
		control.token = token_control_entity;
	}
	else if( ( control.function = isFunction( name ) ) != NULL )
	{
		control.token = token_control_function;
	}	
	else
	{
		control.token = token_unknown;
	}

	return control.token;
}

/*

enum math_type { mt_unknown, mt_ident, mt_digit, mt_ord, mt_bin, mt_unary, mt_rel, mt_fence, 
				 mt_mov_limits, mt_limits, mt_func, mt_func_limits, mt_text };
*/

const char *getErrorMsg( ex_exception code )
{
	const int size = TABLE_SIZE( errorTable );

	for( int i = 0; i < size; ++i )
	{
		if( code == errorTable[i].code )
		{
			return errorTable[i].msg;
		}
	}	

	return NULL;
}

const char *getMathVariant( const char *attrib )
{
	const int size = TABLE_SIZE( mathvariant );

	int result, low, mid, high;

	low = 0;
	high = size - 1;

	while( low <= high )
	{
		mid = ( low + high )/2;

		result = fastCompare( attrib, mathvariant[mid].key );

		if( result < 0 )
		{
			high = mid - 1;
		}
		else if( result > 0 )
		{
			low = mid + 1;
		}
		else
		{
			return mathvariant[mid].value;			
		}
	}

	return NULL;
}


bool getFenceType( const char *name,  FenceStruct &fence )
{
	const int size = TABLE_SIZE( fenceTable );

	for( int i = 0; i < size; ++i )
	{
		if( strcmp( name, fenceTable[i].name ) == 0 )
		{
			fence.entity   = &fenceTable[i];
			

			if( fence.entity->code < 256 ) // ascii
			{
				fence.output[0] = (char)fence.entity->code;
				fence.output[1] = '\0';
			}
			else
			{				
				sprintf_s( fence.output, sizeof( fence.output ) - 1, "&#x%x;", fence.entity->code );
				//sprintf( fence.output, "&#x%x;", fence.entity->code );
			}
			return true;		
		}
	}

	fence.entity = NULL;
	return false;
}



EnvironmentStruct *getEnvironmentType( const char *name )
{
	const int size = TABLE_SIZE( environmentTable );
	
	int result, low, mid, high;

	low = 0;
	high = size - 1;

	while( low <= high )
	{
		mid = ( low + high )/2;

		result = fastCompare( name, environmentTable[mid].name );

		if( result < 0 )
		{
			high = mid - 1;
		}
		else if( result > 0 )
		{
			low = mid + 1;
		}
		else
		{
			return &environmentTable[mid];			
		}
	}


	return NULL;
}


SymbolStruct *getSymbol( const char *name )
{
	const int size = TABLE_SIZE( symbols );

	for( int i = 0; i < size; ++i )
	{
		if( strcmp( name, symbols[i].name ) == 0 )
		{
			return &symbols[i];
		}
	}

	return NULL;
}


