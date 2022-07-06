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
#include "classes.h"
#include "tables.h"
#include "exceptions.h"
#include <ctype.h>
// GLOBALS


enum sub_expression { se_none, se_use_default, se_braced, se_optional_param, se_inline_math, se_fence,					 
					  se_matrix };//, se_eqalign, se_array, se_eqnarray  };

enum element_type { et_unknown, et_tag_off, et_tag_on, et_tag_open, et_tag_on_open };


enum skip_input { sp_skip_all, sp_skip_once, sp_no_skip };

enum { MAX_CONTROL_NAME = 32, EXTRA_BUF = 8 };


#define char_null	   '\0'
#define char_backslash '\\'
#define char_prime	   '\''


struct ArrayStruct {
	short maxColumn, columnCount;
	//sub_expression subType;
	command_id id;
};

struct ErrorMessage {
	const char *msg;
	int index;
	int code;	
	string msg2;
};

struct InputStream {
	token_type token;
	char buffer[MAX_CONTROL_NAME+EXTRA_BUF+1];
	char *start;
	char nextChar;
};


static SymbolTable limits[] = {
	{ "<munder>", "</munder>" },
	{ "<mover>", "</mover>" },
	{ "<munderover>", "</munderover>" }
};

static SymbolTable nolimits[] = {
	{ "<msub>", "</msub>" },
	{ "<msup>", "</msup>" },
	{ "<msubsup>", "</msubsup>" }
};

static SymbolTable limitsMovable[] = {
	{ "<msub movablelimits='true'>", "</msub>" },
	{ "<msup movablelimits='true'>", "</msup>" },
	{ "<msubsup movablelimits='true'>", "</msubsup>" }
};

static char *pStart		= NULL;
static char *pCur 		= NULL;
static char *pEnd		= NULL;
static bool isNumberedFormula = false;
static Buffer globalBuf, eqNumber;
static ErrorMessage errMsg;


void onDigit( Buffer &prevBuf );
void onAlpha( Buffer &prevBuf );
void onSymbol( Buffer &prevBuf, InputStream &input, bool checkSubSup = true );
void onSubscript( Buffer &prevBuf );
void onSuperscript( Buffer &prevBuf );
void onControlName( Buffer &prevBuf, InputStream &input, bool &quit );
void onEntity( Buffer &prevBuf, EntityStruct *entity, bool checkLimits = true, bool checkSubSup = true );
void onFunction( Buffer &prevBuf, FunctionStruct *function, bool checkLimits = true );
bool onCommand( Buffer &prevBuf, ControlStruct &control, sub_expression subType, void *paramExtra );
void getCommandParam( Buffer &prevBuf, sub_expression subType );
bool followedBy( char **p, const char *pattern, skip_input skip );
bool parseExpression(const char *input, int len, int *errorIndex, int *errCode );
void runLoop( Buffer &prevBuf, sub_expression subType, void *paramExtra = NULL );
EnvironmentStruct *getEnvironmentType();
void onBeginEnvironment( Buffer &prevBuf );
bool onEndEnvironment( sub_expression subType, void *paramExtra );
void precondition( char **p );
void skipSpaces( char **p );
void skipChar( char **p );
bool getInput( InputStream &input, skip_input white_space );
bool scriptNext( char *p );
int  getLastTagIndex(const char *p, size_t length, element_type element );
token_type getControlTypeEx( InputStream &input, ControlStruct &control );
void onColumn( Buffer &prevBuf, const char *pos, ArrayStruct &ar );
void onRow( Buffer &prevBuf, ArrayStruct &ar );
bool needsMrow(const char *p );
void onMathFont( Buffer &prevBuf, const char *tagOn, const char *tagOff );
void onTextFont( Buffer &prevBuf, const char *tagOn, const char *tagOff, command_id id, bool allowInline = true );
bool onFence( Buffer &prevBuf, command_id id, sub_expression subType, const char *tagOn, const char *tagOff );
void onEndExpression( sub_expression subType, token_type token, CommandStruct *command );
void getPrime( char **p, char *buf );
void onPrime( Buffer &prevBuf );

bool convertFormula(const char *input, int len, int *errorIndex, int *errCode )
{
	
	if( len < 0 )
	{
		len = (int) strlen( input );
	}

	if( len == 0 )
	{
		return false;
	}

	return parseExpression( input, len, errorIndex, errCode );	
}

bool parseExpression( const char *input, int len, int *errorIndex, int *errCode )
{
	
	bool result;

	pStart	  = (char *)input;
	pCur	  = pStart;
	pEnd      = pStart + len;	

	ZeroMemory( &errMsg, sizeof( errMsg ) );

	globalBuf.destroy();
	eqNumber.destroy();
	isNumberedFormula = false;

	result = true;
	try 
	{
		precondition( &pCur );
		runLoop( globalBuf, se_use_default );
		if( needsMrow( globalBuf.data() ) )
		{
			globalBuf.insertAt( 0, "<mrow>" );
			globalBuf.write( "</mrow>" );
		}
		if( isNumberedFormula )
		{
			globalBuf.insertAt( 0, eqNumber.data() );
			globalBuf.write( "</mtd></mlabeledtr></mtable>" );
		}
	}
	catch( const ErrorMessage &err )
	{
		result      = false;
		*errCode    = err.code;
		*errorIndex = err.index;
	}

	return result;
}


const char *getMathMLOutput()
{
	if( globalBuf.length() != 0 )
	{
		return globalBuf.data();
	}

	return NULL;
}

bool getMathMLOutput(string& buf, bool display)
{
	if (globalBuf.length() != 0)
	{
		const char *data = globalBuf.data();

		buf = display ? "<math display='block'>" : "<math>";

		buf.append(data);
		buf.append("</math>");

		return true;
	}
	return false;
}
static void getControlName(const char* start, string& name)
{
	char* p = (char*)(start+1);

	name.push_back('\\');

	if (!isalpha(*p))
	{
		name.push_back(*p);
		return;
	}
	while (*p && isalnum(*p))
	{
		name.push_back(*p);
		++p;
	}
}

ErrorMessage &error(const char *index, ex_exception code )
{
	errMsg.code  = (int) code;
	errMsg.index = (int) (index - pStart);	

	if (ex_undefined_control_sequence == code)
	{
		string name;

		errMsg.msg2 = getErrorMsg(code);
		errMsg.msg2.append(": ");
		
		getControlName(index, name);
		
		errMsg.msg2.append(name);

		errMsg.msg = errMsg.msg2.c_str();
	}
	else
	{
		errMsg.msg = getErrorMsg(code);
	}
	return errMsg;
}

ErrorMessage& error(const char* index, ex_exception code, const string &msg)
{
	errMsg.code = (int)code;
	errMsg.index = (int)(index - pStart);
	errMsg.msg = getErrorMsg(code);

	return errMsg;
}


const char *getLastError()
{
	return errMsg.msg;
}

/*

 PRECONDITION traps as many errors as possible

*/


static void precondition( char **p )
{
	int braces;
	char *s, *lastLeftBrace;

	skipSpaces( p );

	// these chars can't start an equation

	//lastPos = *p;

	s = *p;

	switch( *s )
	{
	case '}':
		throw error( s, ex_missing_lbrace );
	case '^':
		throw error( s, ex_prefix_superscript );
	case '_':
		throw error( s, ex_prefix_subscript );
	case '&':
		throw error( s, ex_misplaced_column_separator );		
	}

	braces = 0;
	lastLeftBrace = NULL;

	while( *s )
	{
		if( *s == '{' )
		{
			lastLeftBrace = s;
			++braces;
			skipChar( &s );			
			if( *s == '}' )
			{
				skipChar( &s );
				--braces;
			}			
			switch( *s )
			{
			case '^':
				throw error( s, ex_prefix_superscript );
			case '_':
				throw error( s, ex_prefix_subscript );
			}
		}
		else if( *s == '}' )
		{
			--braces;
			if( braces < 0 )
			{
				throw error( s, ex_more_rbrace_than_lbrace );
			}
			++s;
		}
		else if( *s == char_backslash )
		{
			++s;
			if( isdigit( *s ) )
			{
				throw error( s-1, ex_undefined_control_sequence );
			}
			else if( isalpha( *s ) )
			{
				char *start;

				start = s;

				// compute the length of the control name
				do
				{
					++s;
				} 
				while( isalpha( *s ) );

				if( ( s - start ) > MAX_CONTROL_NAME )
				{
					throw error( start - 1, ex_control_name_too_long );
				}
			}
			else
			{
				// only the following chars can be escaped

				switch( *s )
				{		
				case '}':
				case '{':
				case '&':
				case '^':
				case '_':
				case '-':
				case '$':
				case '#':				
				case '!':
				case ';':
				case '>':
				case ':':	// check
				case ',':
				case '|':
				case ' ':
					++s;
					break;
				case char_backslash:
					skipChar( &s );
					if( scriptNext( s ) )
					{
						if( *s == '_' )
						{
							throw error( s, ex_prefix_subscript );
						}
						else
						{
							throw error( s, ex_prefix_superscript );
						}
					}
					break;
				default:
					throw error( s-1, ex_undefined_control_sequence );				
				}		
			}
		}
		else if( *s == '&' )
		{			
			skipChar( &s );
			if( scriptNext( s ) )
			{
				if( *s == '_' )
				{					
					throw error( s, ex_prefix_subscript );
				}
				else
				{
					throw error( s, ex_prefix_superscript );
				}
			}
		}
		else if( *s == '$' )
		{
			if( braces == 0 )
			{
				throw error( s, ex_misplaced_inline_formula );
			}

			skipChar( &s );
			if( scriptNext( s ) )
			{
				if( *s == '_' )
				{
					throw error( s, ex_prefix_subscript );
				}
				else
				{
					throw error( s, ex_prefix_superscript );
				}
			}
		}
		else if( scriptNext( s ) )
		{
			char *pos;

			pos = s;
			skipChar( &s );
			switch( *s )
			{
			case char_null:
			case '}':
			case '$':
			case '&':
				throw error( pos, ex_missing_parameter );				
			case char_backslash:
				if( s[1] == char_backslash ) // row separator
				{
					throw error( pos, ex_missing_parameter );
				}				
			}
		}
		else
		{
			++s;
		}
		
	}

	if( braces != 0 )
	{
		throw error( lastLeftBrace, ex_more_lbrace_than_rbrace );
	}
	// check backwards

	do {
		--s;
	}
	while( (s > *p ) && isspace( *s ) );

	pEnd = s+1;	
}

static void skipSpaces( char **p )
{
	char *s;

	if( p == NULL )
	{
		return;
	}

	s = *p;

	while( (*s) && isspace( *s ) )
	{
		++s;
	}

	*p = s;
}

static void skipChar( char **p )
{
	char *s;

	if( p == NULL )
	{
		return;
	}

	s = *p;

	++s; // skip one char, then spaces

	while( (*s) && isspace( *s ) )
	{
		++s;
	}

	*p = s;
}


static bool followedBy( char **p, const char *pattern, skip_input skip )
{
	char *s;

	if( p == NULL )
	{
		return false;
	}

	s = *p;

	do {
		if( *s == *pattern )
		{
			++s;
			++pattern;
		}
		else
		{
			return false;
		}
	} while( *s && *pattern );

	if( ( *pattern == char_null ) && !isalpha( *s ) )
	{
		if( skip != sp_no_skip )
		{
			if( isspace( *s ) )
			{
				skipSpaces( &s );
			}
			*p = s;
		}
		return true;
	}

	return false;
}


static bool scriptNext( char *p )
{
	return ( *p == '_' ) || ( *p == '^' );
}

static int  getLastTagIndex( char *p, size_t length, element_type element )
{
	int inside;			// inside a tag
	element_type current;
	size_t i;

	if( ( p == NULL ) || (length == 0) )
	{
		return -1;
	}

	i	    = length-1;
	inside  = 0;
    current = et_unknown;

	while( i >= 0 )
	{
		if( p[i] == '>' )
		{
			if( p[i-1] == '/' )
			{
				current = et_tag_open;
				--i;	// to skip twice
			}
		}
		else if( p[i] == '<' )
		{
			if( p[i+1] == '/' )
			{
				current = et_tag_off;		
				if( element == current )
				{
					return (int) i;
				}
				inside++;
			}
			else 
			{
				--inside;
				if( inside == 0 )
				{
					return (int) i;
				}
			}
		}
		if( i == 0 )
		{
			break;
		}
		--i;
	}

	return -1;
}

static bool getInput( InputStream &input, skip_input white_space )
{
	
	ZeroMemory( &input, sizeof( input ) );

	if( white_space == sp_skip_all )
	{
		if( isspace( *pCur ) )
		{
			skipSpaces( &pCur );
		}
	}
	
	if( *pCur == char_null )
	{
		input.token = token_eof;
		return false;
	}

	
	while( *pCur )
	{
		input.start = pCur;
		if( isalpha( *pCur ) )
		{
			input.token = token_alpha;
			break;
		}
		else if( isdigit( *pCur ) )
		{
			input.token = token_digit;
			break;
		}
		else if( *pCur == '&' )
		{
			input.token = token_column_sep;
			skipChar( &pCur );
			break;
		}
		else if( *pCur == '{' )
		{
			input.token = token_left_brace;
			skipChar( &pCur );
			break;
		}
		else if( *pCur == '}' )
		{
			input.token = token_right_brace;
			skipChar( &pCur );
			break;
		}
		else if( *pCur == '^' )
		{
			input.token = token_superscript;
			skipChar( &pCur );
			break;
		}
		else if( *pCur == '_' )
		{
			input.token = token_subscript;
			skipChar( &pCur );
			break;
		}
		else if( isspace( *pCur ) )
		{
			if( *pCur == ' ' )
			{
				if( white_space == sp_skip_all )
				{
				   skipSpaces( &pCur );
				}
				else if ( white_space == sp_skip_once )
				{
					
					input.token = token_white_space;
					++pCur;
					break;
				}
				else
				{
					//input.token = token_white_space;
					skipSpaces( &pCur );
				}
			}
			else		// other spaces \n\r
			{
				skipSpaces( &pCur );
			}
		}
		else if( *pCur == char_backslash )
		{
			++pCur;
			if( isalpha( *pCur ) )
			{
				input.token = token_control_name;		
				for( int i = 0; i < MAX_CONTROL_NAME; ++i )
				{					
					input.buffer[i] = *pCur;
					++pCur;
					if( !isalpha( *pCur ) )
					{
						break;
					}
				}
				// use this to determine whether control name 
				// is followed IMMEDIATELY by digits
				// cf. \abc123 vs.\abc   123
				input.nextChar = *pCur;
				if( isspace( *pCur ) )
				{
					skipSpaces( &pCur );
				}
			}
			else
			{
				if( *pCur == char_backslash )
				{
					input.token  = token_row_sep;
				}
				else
				{
					input.token  = token_control_symbol;
				}
				input.buffer[0] = char_backslash;
				input.buffer[1] = *pCur;
				skipChar( &pCur );				
				input.nextChar = *pCur;
			}
			break;
		}
		else if( *pCur == ']' )
		{			
			input.token  = token_right_sq_bracket;
			input.buffer[0] = *pCur;
			skipChar( &pCur );				
			input.nextChar = *pCur;
			break;
		}
		else if( *pCur == '$' )
		{			
			input.token  = token_inline_math;
			input.buffer[0] = *pCur;
			//skipChar( &pCur ); don't skip
			input.nextChar = *pCur;
			break;
		}
		else if( *pCur == char_prime )
		{
			input.token  = token_prime;
			input.buffer[0] = *pCur;			
			input.nextChar = pCur[1];
			// don't skip
			break;
		}
		else	// symbols
		{
			input.token  = token_symbol;
			input.buffer[0] = *pCur;
			skipChar( &pCur );				
			input.nextChar = *pCur;
			break;
		}
	}

	return true;
}

static void onPrime( Buffer &prevBuf )
{
	SymbolStruct *sym;
	char buf[5];

	getPrime( &pCur, buf );

	sym = getSymbol( buf );
	
	prevBuf.write( sym->element );
}

static void getPrime( char **p, char *buf )
{
	int i = 0;
	do {
		*buf = **p;
		++buf;
		++(*p);
		++i;
		if( i == 3 )
		{
			break;
		}
	} while( **p == char_prime );

	*buf = char_null;

	if( isspace( *pCur ) )
	{		
		skipSpaces( &pCur );
	}
}

static token_type getControlTypeEx( InputStream &input, ControlStruct &control )
{
	Buffer buf;		

	control.start = input.start;
	input.token		  = getControlType( input.buffer, control );

	if( input.token != token_unknown )
	{
		return input.token;
	}

	if( isdigit( input.nextChar )  )
	{
		buf.write( input.buffer );	
		buf.write( pCur, 1 );		// write the next digit
		++pCur;
	}
	else
	{
		return input.token;
	}

	while( ( input.token = getControlType( buf.data(), control ) ) == token_unknown )
	{
		if( isdigit( *pCur ) )
		{
			buf.write( pCur, 1 );
			++pCur;
		}
		else
		{
			break;
		}
	}

	return input.token;
}

static void runLoop( Buffer &prevBuf, sub_expression subType, void *paramExtra )
{
	InputStream input;
	ControlStruct control;	
	Buffer str;
	bool quitLoop;

	ZeroMemory( &control, sizeof( control ) );

	quitLoop = false;

	while( getInput( input, sp_skip_all ) )
	{
		switch( input.token )
		{
		case token_alpha:
			onAlpha( str );
			break;

		case token_digit:
			onDigit( str );
			break;

		case token_prime:
			onPrime( str );
			break;
		case token_symbol:
		case token_control_symbol:		
			onSymbol( str, input );
			break;
		/*
		case token_white_space:
			onWhiteSpace( str );
			break;
		*/
		case token_inline_math:

			if( subType != se_inline_math )
			{
				throw error( pCur, ex_misplaced_inline_formula );
			}
			quitLoop = true;
			break;
		case token_left_brace:
			runLoop( str, se_braced );
			break;
		case token_right_brace:			
			quitLoop = true;
			break;
		case token_right_sq_bracket:
			if( subType == se_optional_param )
			{
				quitLoop = true;
			}
			else
			{
				onSymbol( str, input );
			}
			break;
		case token_superscript:
			onSuperscript( str );
			break;
		
		case token_subscript:
			onSubscript( str );
			break;
		
		case token_column_sep:
			if( subType < se_matrix )
			{
				throw error( input.start, ex_misplaced_column_separator );				
			}
			else
			{
				onColumn( str, input.start, *((ArrayStruct *)paramExtra) );
			}
			break;
		case token_row_sep:
			if( subType < se_matrix )
			{
				throw error( input.start, ex_misplaced_row_separator );				
			}
			else
			{
				onRow( str, *((ArrayStruct *)paramExtra) );
			}
			break;		
		case token_control_name:
			//onControlName( str, input, quit );			

				input.token = getControlTypeEx( input, control );

				switch( input.token )
				{		
				case token_control_entity:
					onEntity( str, control.entity );
					break;				
				case token_control_command:
					quitLoop = onCommand( str, control, subType, paramExtra );
					break;
				case token_control_function:
					onFunction( str, control.function );
					break;
				//case token_unknown:
				default:
					throw error( input.start, ex_undefined_control_sequence );								
				}

			break;
		}

		if( quitLoop )
		{
			break;
		}
	}

	onEndExpression( subType, input.token, control.command );

	prevBuf.append( str, true );	
}

//se_optional_param, se_inline_math, se_fence,					 
//					  se_matrix
static void onEndExpression( sub_expression subType, token_type token, CommandStruct *command )
{
	switch( subType )
	{
	case se_optional_param:
		if( token != token_right_sq_bracket )
		{
			throw error( pCur, ex_missing_right_sq_bracket );
		}
		break;
	case se_inline_math:
		if( token != token_inline_math )
		{
			throw error( pCur, ex_missing_dollar_symbol );
		}
		break;
	case se_matrix:
		if( token != token_control_command )
		{
			throw error( pCur, ex_missing_end );
		}
		else if( command->id != ci_end )
		{
			throw error( pCur, ex_missing_end );
		}
		break;
	case se_fence:
		if( token != token_control_command )
		{
			throw error( pCur, ex_missing_right_fence );
		}
		else if( command->id != ci_right )
		{
			throw error( pCur, ex_missing_right_fence );
		}
	}

}


static void onAlpha( Buffer &prevBuf )
{
	char tag[] = "<mi>?</mi>";
	char *p;
	Buffer str;


	str.setlength( 50 );

	p = strchr( tag, '?' );

	do {
		*p = *pCur;
		str.write( tag, sizeof( tag ) - 1 );	
		++pCur;
	}
	while( isalpha( *pCur ) );

	prevBuf.append( str, true );	

}

static void onDigit( Buffer &prevBuf )
{
	char tagOn[] = "<mn>";
	char tagOff[] = "</mn>";

	Buffer str;
	char *start;


	str.setlength( 50 );


	str.write(tagOn, sizeof( tagOn ) - 1 );


	start = pCur;

	do {		
		++pCur;
	}
	while( isdigit( *pCur ) );

	str.write( start, (pCur - start) );

	str.write(tagOff, sizeof( tagOff ) - 1 );

	prevBuf.append( str, true );

}

static void onSymbol( Buffer &prevBuf, InputStream &input, bool checkSubSup )
{
	SymbolStruct *symbol;

	symbol = getSymbol( input.buffer );

	if( symbol == NULL )
	{
		throw error( pCur, ex_unknown_character );
	}

	
	switch( symbol->mathType )
	{
	case mt_fence:
	case mt_left_fence:
	case mt_right_fence:
		if( checkSubSup && scriptNext( pCur ) )
		{
			throw error( pCur, ex_ambiguous_script );
		}
		break;
	}

	prevBuf.write( symbol->element );
}

static bool needsMrow( const char *p )
{
	element_type et;
	short inside, count;

	if( p == NULL )
	{
		return false;
	}

	inside = count = 0;
	et	   = et_unknown;

	while( *p )
	{
		if( *p == '<' )
		{
			if( p[1] == '/' )
			{
				et = et_tag_off;
				++p;				
			}
			else
			{
				et = et_tag_on;
			}
		}
		else if( *p == '>' )
		{
			if( *(p-1) == '/' ) // tag open
			{
				if( inside == 0 )
				{
					++count;
				}
			}
			else if( et == et_tag_off ) 
			{
				--inside;
			}
			else
			{
				if( inside == 0 )
				{
					++count;
				}
				++inside;
			}
			if( count > 1 )
			{
				return true;
			}
		}
		++p;
	}

	return false;
}

static void getCommandParam( Buffer &prevBuf, sub_expression subType )
{
	InputStream input;
	Buffer str;
	ControlStruct control;

	getInput( input, sp_skip_all );

	switch( input.token )
	{
	case token_alpha:
		prevBuf.format( "<mi>%c</mi>", *pCur );
		skipChar( &pCur );
		break;

	case token_digit:
		prevBuf.format( "<mn>%c</mn>", *pCur );
		skipChar( &pCur );
		break;
	case token_prime:
		prevBuf.write( "<mo>&#x02032;</mo>" );
		skipChar( &pCur );
		break;
	case token_symbol:
	case token_control_symbol:
		onSymbol( prevBuf, input, false ); // ignore subscript/superscript
		break;

	case token_left_brace:
		if( subType == se_use_default )
		{
			runLoop( str, se_braced );
		}
		else
		{
			runLoop( str, subType );
		}
		if( needsMrow( str.data() ) )
		{
			str.insertAt( 0, "<mrow>" );
			str.write( "</mrow>" );
		}
		prevBuf.append( str, true );
		break;

	case token_right_brace:			
	case token_superscript:
	case token_subscript:
	case token_column_sep:
	case token_row_sep:
	case token_eof:
		throw error( pCur, ex_missing_parameter );		

	case token_control_name:
			//onControlName( str, input, quit );			

		input.token = getControlTypeEx( input, control );

		switch( input.token )
		{		
		case token_control_entity:
			// don't check limits and subscript
			onEntity( prevBuf, control.entity, false, false );
			break;				
		case token_control_command:
			if( control.command->id == ci_frac )
			{
				onCommand( str, control, se_use_default, NULL );
				prevBuf.append( str, true );
			}
			else
			{
				throw error( input.start, ex_no_command_allowed );
			}
			break;
		case token_control_function:
			onFunction( prevBuf, control.function, false );
			break;
		//case token_unknown:
		default:
			throw error( input.start, ex_undefined_control_sequence );								
		}
		break;
	default:
		break;
	}
}

static void getSuperscript( Buffer &prevBuf, bool subsup )
{
	if( *pCur == char_prime )
	{
		throw error( pCur, ex_missing_lbrace );
	}

	getCommandParam( prevBuf, se_use_default );

	if( ( *pCur == '^' ) || ( *pCur == char_prime ) )
	{
		throw error( pCur, ex_double_superscript );
	}
	else if( *pCur == '_' )
	{
		if( subsup )
		{
			throw error( pCur, ex_double_subscript );
		}
		else
		{
			throw error( pCur, ex_use_subscript_before_superscript );
		}
	}
}

static void onSuperscript( Buffer &prevBuf )
{
	Buffer str;
	int index;
	SymbolTable *sup;
	
	sup = &nolimits[1];


	index = getLastTagIndex( prevBuf.data(), prevBuf.length(), et_tag_on_open );

	if( index < 0 )
	{
		throw error( pCur, ex_missing_subsup_base );
	}
	
    str.setlength( 50 );

	prevBuf.insertAt( index, sup->tagOn );	

	getSuperscript( str, false );

	str.write( sup->tagOff );		

	prevBuf.append( str, true );
}

static void getSubscript( Buffer &prevBuf, command_id &which )
{
	
	if( *pCur == char_prime )
	{
		throw error( pCur, ex_missing_lbrace );
	}

	getCommandParam( prevBuf, se_use_default );

	if( *pCur == '^' )
	{
		skipChar( &pCur );
		getSuperscript( prevBuf, true );

		which = ci_msubsup;
	}
	else if( *pCur == char_prime )
	{		
		onPrime( prevBuf );
		which = ci_msubsup;
	}
	else
	{
		which = ci_msub;
	}
}

static void onSubscript( Buffer &prevBuf )
{
	Buffer str;
	int index;
	command_id which;
	SymbolTable *sub;

	index = getLastTagIndex( prevBuf.data(), prevBuf.length(), et_tag_on_open );

	if( index < 0 )
	{
		throw error( pCur, ex_missing_subsup_base );
	}	

    str.setlength( 50 );

	getSubscript( str, which );

	if( which == ci_msub )
	{
		sub = &nolimits[0];		
	}
	else
	{
		sub = &nolimits[2];
	}

	prevBuf.insertAt( index, sub->tagOn );		
	str.write( sub->tagOff );		

	prevBuf.append( str, true );
}



enum limits_type { lt_default, lt_subsup, lt_underover };

static void onLimits( Buffer &prevBuf, math_type mathType )
{
	limits_type useLimits;

	useLimits = lt_default;

	do {
		if( followedBy( &pCur, "\\limits", sp_skip_all ) )
		{
			useLimits = lt_underover;
		}
		else if( followedBy( &pCur, "\\nolimits", sp_skip_all ) )
		{
			useLimits = lt_subsup;
		}
		else
		{
			break;
		}
	} while( 1 );

	if( *pCur == char_null )
	{
		return;
	}
	else if( scriptNext( pCur ) || *pCur == char_prime )
	{
		Buffer str;
		command_id which;
		//char *nextChar;
		int index;
		SymbolTable *lim;

		index = getLastTagIndex( prevBuf.data(), prevBuf.length(), et_tag_on_open );

		if( index < 0 )
		{
			index = 0;
		}


		if( *pCur == '_' )
		{		
			skipChar( &pCur );
			getSubscript( str, which );
		}
		else if( *pCur == '^' )
		{
			skipChar( &pCur );
			getSuperscript( str, false );
			which = ci_msup;		
		}		
		else // prime/////
		{
			onPrime( str );
			which = ci_msup;			
			
			if( *pCur == '^' || *pCur == char_prime )
			{
				throw error( pCur, ex_double_superscript );
			}
			else if( *pCur == '_' )
			{
				throw error( pCur, ex_use_subscript_before_superscript );
			}
		}

		if( useLimits == lt_underover )
		{
			lim = limits;
		}
		else if( useLimits == lt_subsup )
		{
			lim = nolimits;
		}
		else
		{
			if( mathType == mt_limits )
			{
				lim = nolimits;
			}
			else
			{
				lim = limitsMovable;
			}
		}

		switch( which )
		{
		case ci_msub:
			prevBuf.insertAt( index, lim[0].tagOn );
			str.write( lim[0].tagOff );
			break;
		case ci_msup:
			prevBuf.insertAt( index, lim[1].tagOn );
			str.write( lim[1].tagOff );
			break;
		case ci_msubsup:				
			prevBuf.insertAt( index, lim[2].tagOn );
			str.write( lim[2].tagOff );
			break;
		}
		prevBuf.append( str, true );		
	}	
}

static void onEntity( Buffer &prevBuf, EntityStruct *entity, bool checkLimits, bool checkSubSup )
{
	
	switch( entity->mathType )
	{
	case mt_ident:
		prevBuf.format( "<mi>&#x%x;</mi>", entity->code );
		break;
	case mt_digit:
		prevBuf.format( "<mn>&#x%x;</mn>", entity->code );
		break;
	case mt_ord:	
	case mt_punct:
		prevBuf.format( "<mo>&#x%x;</mo>", entity->code );
		break;
	case mt_limits:
	case mt_mov_limits:
		
		prevBuf.format( "<mo>&#x%x;</mo>", entity->code );

		if( checkLimits )
		{
			onLimits( prevBuf, entity->mathType );
		}
		break;
	case mt_left_fence:
	case mt_right_fence:
	case mt_fence:
		if( checkSubSup && scriptNext( pCur ) )
		{
			throw error( pCur, ex_ambiguous_script );
		}
		prevBuf.format( "<mo mathsize='1'>&#x%x;</mo>", entity->code );
		break;
	case mt_text:
		prevBuf.format( "<mtext>&#x%x;</mtext>", entity->code );
		break;
	case mt_rel:
	case mt_bin:
	case mt_unary:
	case mt_bin_unary:
		prevBuf.format( "<mo>&#x%x;</mo>", entity->code );
		break;
	default:
		throw error( pCur, ex_unhandled_mathtype );
	}
}

static void onFunction( Buffer &prevBuf, FunctionStruct *function, bool checkLimits  )
{
	prevBuf.format( "<mi>%s</mi>", function->output );

	if( ( function->mathType == mt_func_limits ) && checkLimits )
	{
		onLimits( prevBuf, function->mathType );
	}	
}
/*
enum param_type { pt_unknown, pt_none, pt_one, pt_two, pt_three, pt_table, pt_others,
				  pt_especial };
*/

static void onSqrt( Buffer &prevBuf, const char *tagOn, const char *tagOff )
{
	
	if( *pCur == char_prime )			
	{
		throw error( pCur, ex_missing_lbrace );
	}
	else if( *pCur == '[' )
	{
		Buffer str, radix;

		skipChar( &pCur );
		runLoop( radix, se_optional_param );
		if( radix.length() != 0 )
		{
			str.write( "<mroot>" );
			getCommandParam( str, se_use_default );
			if( needsMrow( radix.data() ) )
			{
				radix.insertAt( 0, "<mrow>" );
				radix.write( "</mrow>" );
			}
			str.append( radix, true );
			str.write( "</mroot>" );
		}
		else
		{
			prevBuf.write( tagOn );
			getCommandParam( str, se_use_default );
			str.write( tagOff );
		}

		prevBuf.append( str, true );
	}
	else
	{
		prevBuf.write( tagOn );
		getCommandParam( prevBuf, se_use_default );
		prevBuf.write( tagOff );
	}
}

static void getAttribute( Buffer &prevBuf, char lastChar )
{
	char *start, *end;
	
	start = pCur;

	while( *pCur && ( *pCur != lastChar ) )
	{
		++pCur;
	}

	if( *pCur != lastChar )
	{
		throw error( pCur, ex_missing_end_tag );
	}
	
	end = pCur - 1;


	while( isspace( *end ) )
	{
		--end;
	}

	if( *end == char_backslash )
	{
		end += 2;
	}
	else
	{
		end++;
	}

	prevBuf.write( start, (end - start ) );

	skipChar( &pCur );

}

static void onMiMnMo( Buffer &prevBuf, const char *tagOn, const char *tagOff )
{
	Buffer str;
	const char* attrib;
	char *start;

	if( *pCur == '[' )
	{
		start = pCur+1;
		skipChar( &pCur );
		getAttribute( str, ']' );

		attrib = getMathVariant( str.data() );

		if( attrib == NULL )
		{
			throw error( start, ex_unknown_attribute );
		}

		str.destroy();
		str.write( tagOn, strlen(tagOn) - 1 ); // don't include '>'
		str.format( " mathvariant='%s'>", attrib );		
	}	
	else
	{
		str.write( tagOn );		
	}
	onMathFont( prevBuf, str.data(), tagOff );
	//prevBuf.write( tagOff );			
}


static EnvironmentStruct *getEnvironmentType()
{
	Buffer str;
	char *curPos;
	EnvironmentStruct *environment;

	if( *pCur != '{' )
	{
		throw error( pCur, ex_missing_lbrace );
	}

	skipChar( &pCur);

	curPos = pCur;

	getAttribute( str, '}' );

	environment = getEnvironmentType( str.data() );

	if( environment != NULL )
	{
		return environment;
	}	
	else
	{
		throw error( curPos, ex_undefined_environment_type );
	}
}

static void getColumnAlignment( Buffer &align, short &maxColumn, const char *tagOn )
{
	char *curPos, *p, *attrib;
	Buffer str;	

	if( *pCur != '{' )
	{
		throw error( pCur, ex_missing_lbrace );
	}

	skipChar( &pCur );

	curPos = pCur;

	getAttribute( str, '}' );

	if( str.length() == 0 )
	{
		throw error( pCur, ex_missing_column_alignment );
	}

	p = str.data();

	maxColumn = 0;

	attrib = (char *)strchr( tagOn, '>' );

	align.write( tagOn, size_t( attrib - tagOn ) );
	
	align.write( " columnalign='" );

	while( *p )
	{
		switch( *p )
		{
		case 'l':
			align.write( "left" );			
			break;
		case 'c':
			align.write( "center" );			
			break;
		case 'r':
			align.write( "right" );			
			break;
		default:
			if( isspace( *p ) )
			{
				++p;
				continue;
			}
			throw error( curPos, 	ex_unknown_alignment_character );
		}

		++maxColumn;
		++p;
		if( *p )
		{
			align.write( " " );
		}
	}
	align.format( "'%s", attrib );
}

static void onBeginEnvironment( Buffer &prevBuf  )
{
	ArrayStruct ar;
	Buffer str, align;
	EnvironmentStruct *environment;

	environment = getEnvironmentType();

	ar.id		   = environment->id;	
	ar.columnCount = 1;
	ar.maxColumn   = 5000; // arbitrary	

	

	switch( environment->id )
	{
	case ci_array:
		getColumnAlignment( align, ar.maxColumn, environment->tagOn );
		prevBuf.append( align, true );
		runLoop( str, se_matrix, &ar );		
		break;
	case ci_eqnarray:
		ar.maxColumn = 3; 
		// fall through
	default:		
		prevBuf.write( environment->tagOn );
		runLoop( str, se_matrix, &ar );
		break;
	}
	str.write( environment->tagOff );
	prevBuf.append( str, true );
}

static bool onEndEnvironment( sub_expression subType, void *paramExtra )
{
	command_id id;
	char *temp;	
	EnvironmentStruct *environment;

	temp = pCur;
	if( subType == se_matrix )
	{
		id = ((ArrayStruct *)paramExtra)->id;
	}
	else
	{
		throw error( temp, ex_missing_begin );		
	}

	environment = getEnvironmentType();

	if( environment->id != id )
	{
		throw error( temp, ex_mismatched_environment_type );
	}
	return true;			
}

static void onColumn( Buffer &prevBuf, const char *pos, ArrayStruct &ar )
{
	++ar.columnCount;

	if( ar.columnCount > ar.maxColumn )
	{
		throw error( pos, ex_too_many_columns );
	}

	prevBuf.write( "</mtd><mtd>" );
}

static void onRow( Buffer &prevBuf, ArrayStruct &ar )
{
	if( (*pCur == char_backslash ) && (pCur[1] == 'e' ) ) // \end?
	{
		if( followedBy( &pCur, "\\end", sp_no_skip ) )
		{
			return; // do nothing
		}
	}

	prevBuf.write( "</mtd></mtr><mtr><mtd>" );
	ar.columnCount = 1; // reset columns
}


static void onHfill( Buffer &prevBuf, sub_expression subType, void *paramExtra )
{
	if( scriptNext( pCur ) )
	{
		if( *pCur == '^' )
		{
			throw error( pCur, ex_prefix_superscript );
		}
		else
		{
			throw error( pCur, ex_prefix_subscript );
		}
	}
	if( subType < se_matrix ) // not in a table
	{
		return;
	}
}

static void onArrows( Buffer &prevBuf, const char *tagOn, const char *tagOff )
{
	Buffer str;

	//tagOn is the base
	if( *pCur == '[' )
	{
		Buffer underscript;

		skipChar( &pCur );
		runLoop( underscript, se_optional_param );
		if( underscript.length() != 0 )
		{
			if( needsMrow( underscript.data() ) )
			{
				underscript.insertAt( 0, "<mrow>" );
				underscript.write( "</mrow>" );
			}
			str.format( "<munderover>%s", tagOn );		

			str.append( underscript, true );

			getCommandParam( str, se_use_default );			
			
			str.write( "</munderover>" );
			prevBuf.append( str, true );
			return;
		}
	}

	// fall through
	//prevBuf.write( tagOn );
	str.format( "<mover>%s", tagOn );		
	getCommandParam( str, se_use_default );
	str.write( tagOff );
	prevBuf.append( str, true );
}

static void onCfrac( Buffer &prevBuf, const char *tagOn, const char *tagOff )
{
	Buffer str;
	const char *extra = "<mstyle displaystyle='true' scriptlevel='0'>";

	str.format( "%s%s", tagOn, extra );
	getCommandParam( str, se_use_default );
	str.format( "</mstyle>%s", extra );
	getCommandParam( str, se_use_default );
	str.write( tagOff );			
	prevBuf.append( str, true );
}


static bool onCommand( Buffer &prevBuf, ControlStruct &control, sub_expression subType, void *paramExtra )
{
	Buffer str, str2;
	CommandStruct *command;

	command = control.command;

	switch( command->param )
	{
	case pt_plain:
		switch( command->id )
		{
		case ci_mn:
		case ci_mo:			
			onMiMnMo( prevBuf, command->tagOn, command->tagOff );			
			break;
		case ci_mathop:
			onMathFont( str, command->tagOn, command->tagOff );
			//str.write( command->tagOff );	
			onLimits(str, mt_limits );
			prevBuf.append( str, true );
			break;
		/*
		case ci_mathrm:
		case ci_mathit:
		case ci_mathbf:
		case ci_mathbi:
		
		case ci_mathord,
		case ci_func:		
		*/
		case ci_mathfont:
		case ci_mathord:
		case ci_mathbin:
		case ci_mathrel:
			//str.write( command->tagOn );
			onMathFont( str, command->tagOn, command->tagOff );
			//str.write( command->tagOff );	
			prevBuf.append( str, true );
		}
		break;
	case pt_especial:
		switch( command->id )
		{
		//case ci_mi:
		
		case ci_sqrt:
			onSqrt( prevBuf, command->tagOn, command->tagOff );			
			break;
		case ci_begin:
				onBeginEnvironment( prevBuf );
				break;
		case ci_end:
			    return onEndEnvironment( subType, paramExtra );
		case ci_stackrel:
			str.write( command->tagOn );
			getCommandParam( str2, se_use_default );
			getCommandParam( str, se_use_default );
			str.append( str2, true );
			str.write( command->tagOff );
			prevBuf.append( str, true );
			break;
		case ci_hfill:
			onHfill( prevBuf, subType, paramExtra );
			break;
		case ci_strut:
			prevBuf.write( command->tagOn );
			break;
		case ci_limits:
		case ci_nolimits:
			throw error( control.start, ex_misplaced_limits );			
		case ci_mathstring:
			onTextFont( prevBuf, command->tagOn, command->tagOff, command->id, false );			
			break;
		case ci_text:
			onTextFont( prevBuf, command->tagOn, command->tagOff, command->id );
			break;
		case ci_eqno:
		case ci_leqno:
			if( subType != se_use_default )
			{
				throw error( pCur, ex_misplaced_eqno );
			}
			else if ( isNumberedFormula )
			{
				throw error( pCur, ex_duplicate_eqno );
			}
			isNumberedFormula = true;
			onTextFont( eqNumber, command->tagOn, command->tagOff, ci_eqno, false );
			break;
		case ci_left:
		case ci_right:
			return onFence( prevBuf, command->id, subType, command->tagOn, command->tagOff );
		case ci_ext_arrows:
			onArrows( prevBuf, command->tagOn, command->tagOff );
			break;
		case ci_cfrac:
			onCfrac( prevBuf, command->tagOn, command->tagOff );
			break;			
		case ci_underoverbrace:
			str.write( command->tagOn );		
			getCommandParam( str, se_use_default );
			str.write( command->tagOff );					
			onLimits( str, mt_mov_limits );
			prevBuf.append( str, true );
			break;
		case ci_lsub:		
			str.write( command->tagOn );		
			getCommandParam( str, se_use_default );
			str.write( "<mprescripts/>" );
			getCommandParam( str, se_use_default );
			str.write( "<none/>" );
			str.write( command->tagOff );
			prevBuf.append( str, true );
			break;
		case ci_lsup:
			str.write( command->tagOn );		
			getCommandParam( str, se_use_default );
			str.write( "<mprescripts/><none/>" );
			getCommandParam( str, se_use_default );			
			str.write( command->tagOff );
			prevBuf.append( str, true );
			break;
		case ci_lsubsup:		
			str.write( command->tagOn );		
			getCommandParam( str, se_use_default );
			str.write( "<mprescripts/>" );
			getCommandParam( str, se_use_default );
			getCommandParam( str, se_use_default );
			str.write( command->tagOff );
			prevBuf.append( str, true );
			break;
		default:
			break;
		}
		break;

	case pt_one:		
		prevBuf.write( command->tagOn );		
		getCommandParam( prevBuf, se_use_default );
		prevBuf.write( command->tagOff );		
		break;

	case pt_two:
		prevBuf.write( command->tagOn );
		getCommandParam( prevBuf, se_use_default );
		getCommandParam( prevBuf, se_use_default );
		prevBuf.write( command->tagOff );
		break;

	case pt_three:
		break;

	case pt_table:
		break;

	case pt_none:
		break;

	case pt_others:
	default:
		break;
	}

	return false; 
}

static void onMathFont( Buffer &prevBuf, const char *tagOn, const char *tagOff )
{
	InputStream input;
	ControlStruct control;
	SymbolStruct *symbol;
	Buffer str;
	int brace;
	bool quitLoop;	

	brace = 0;
	
	if( *pCur == '{' )
	{
		quitLoop  = false;	// loop
	}
	else
	{
		quitLoop = true;	// read only one char
	}
	

	while( getInput( input, sp_skip_all ) )
	{
		switch( input.token )
		{
		case token_alpha:
		case token_digit:
			str.write( pCur, 1 );
			++pCur;
			break;
		case token_prime:
			{
				SymbolStruct *sym;
				char buf[5];

				getPrime( &pCur, buf );

				sym = getSymbol( buf );
				str.write( sym->literal );
			}
			break;
		case token_symbol:
		case token_control_symbol:
		case token_right_sq_bracket:			
			symbol = getSymbol( input.buffer );
			str.write( symbol->literal );
			break;		
		
		case token_white_space:
			str.write( "&#x00A0;" );			
			if( isspace( *pCur ) )
			{
				skipSpaces( &pCur );
			}
			break;
		case token_left_brace:
			++brace;
			break;
		case token_right_brace:
			--brace;
			if( brace < 0 )
			{
				throw error( input.start, ex_missing_parameter );		
			}			
			quitLoop = true;
			break;
		case token_inline_math:
			throw error( input.start, ex_misplaced_inline_formula );

		case token_superscript:
		case token_subscript:
			throw error( input.start, ex_no_command_allowed );
		case token_column_sep:
			throw error( input.start, ex_misplaced_column_separator );
		case token_row_sep:		
			throw error( input.start, ex_misplaced_row_separator );		
		
		case token_control_name:
			
			switch( getControlTypeEx( input, control ) )
			{		
			case token_control_entity:
				str.format( "&#x%x;", control.entity->code );
				break;
			case token_control_function:
				str.write( control.function->output );				
				break;				
			case token_control_command:
				throw error( input.start, ex_no_command_allowed );
				break;
			//case token_unknown:
			default:
				throw error( input.start, ex_undefined_control_sequence );								
			}
			break;
		default:
			break;
		}
		if( quitLoop )
		{
			break;
		}
	}

	prevBuf.write( tagOn );
	str.write( tagOff );
	prevBuf.append( str, true );	
}


static void onTextFont( Buffer &prevBuf, const char *tagOn, const char *tagOff, command_id id, bool allowInline )
{
	InputStream input;
	ControlStruct control;
	SymbolStruct *symbol;
	Buffer str, temp;
	int brace;
	bool quitLoop;

	brace = 0;	

	if( ( id != ci_eqno ) && ( id != ci_leqno ) )
	{
		if( *pCur != '{' )
		{
			throw error( pCur, ex_missing_lbrace );
		}
	}

	quitLoop	   = false;		
	
	while( getInput( input, sp_skip_once ) )
	{
		switch( input.token )
		{
		case token_alpha:
		case token_digit:
			str.write( pCur, 1 );
			++pCur;
			break;
		
		case token_symbol:
		case token_control_symbol:
		case token_right_sq_bracket:
			symbol = getSymbol( input.buffer );
			str.write( symbol->literal );
			break;
		case token_prime:
			if( pCur[1] == char_prime )
			{
				str.write( "&#x201D;" );
				pCur += 2;
			}
			else
			{
				str.write( "&#x2019;" );
				++pCur;
			}			
			break;
		case token_inline_math:
			if( !allowInline )
			{
				throw error( input.start, ex_misplaced_inline_formula );
			}
			if( str.length() > 0 )
			{
				temp.write( tagOn );
				str.write( tagOff );
				temp.append( str, true );
				str.reset();					
			}

			skipChar( &pCur );
			runLoop( str, se_inline_math, NULL );
				// skip end $
			++pCur;

			if( needsMrow( str.data() ) )
			{
				str.insertAt( 0, "<mrow>" );
				str.write( "</mrow>" );
			}
			temp.append( str, true );
			str.reset();
			break;

		case token_white_space:
			str.write( "&#x00A0;" );			
			if( isspace( *pCur ) )
			{
				skipSpaces( &pCur );
			}
			break;
		case token_left_brace:
			++brace;
			break;
		case token_right_brace:
			--brace;
			if( brace < 0 )
			{
				throw error( input.start, ex_missing_parameter );		
			}			
			quitLoop = true;
			break;
		case token_superscript:
		case token_subscript:
			throw error( input.start, ex_no_command_allowed );
		case token_column_sep:
			throw error( input.start, ex_misplaced_column_separator );
		case token_row_sep:		
			throw error( input.start, ex_misplaced_row_separator );		
		
		case token_control_name:
			
			switch( getControlTypeEx( input, control ) )
			{		
			case token_control_entity:
				str.format( "&#x%x;", control.entity->code );
				break;
			case token_control_function:
				throw error( input.start, ex_not_math_mode );
			case token_control_command:
				throw error( input.start, ex_no_command_allowed );
			//case token_unknown:
			default:
				throw error( input.start, ex_undefined_control_sequence );								
			}
			break;
		default:
			break;
		}
		if( quitLoop )
		{
			break;
		}
	}

				
	if( str.length() )
	{
		temp.write( tagOn );
		str.write( tagOff );
	}
	temp.append( str, true );

	if( needsMrow( temp.data() ) )
	{
		temp.insertAt( 0, "<mrow>" );
		temp.write( "</mrow>" );
	}
	prevBuf.append( temp, true );	
}

static void getFence( Buffer &prevBuf, const char *tagOn, command_id id )
{
	InputStream input;
	FenceStruct fence;
	size_t len;


	len = strlen( tagOn );

	getInput( input, sp_skip_all );

	switch( input.token )
	{
	case token_control_symbol:	
	case token_symbol:	
	case token_control_name:
		if( !getFenceType( input.buffer, fence ) )
		{
			throw error( input.start, ex_missing_fence_parameter );
		}
		
		if ( id == ci_left )
		{
			if( *input.start == '(' )
			{
				prevBuf.write( tagOn, len );
			}
			else
			{
				prevBuf.write( tagOn, len - 1 ); // don't write >
				prevBuf.format( " left='%s'", fence.output );
			}
		}
		else
		{
			if( *input.start == ')' )
			{
				prevBuf.write( "><mrow>" );
			}
			else
			{
				prevBuf.format( " right='%s'><mrow>", fence.output );
			}
		}
		break;
	default:
		throw error( input.start, ex_missing_fence_parameter );
	}
}

static bool onFence( Buffer &prevBuf, command_id id, sub_expression subType, const char *tagOn, const char *tagOff )
{
	Buffer str, fence;

	if( id == ci_right )
	{
		if( subType == se_fence )
		{
			return true;
		}
		throw error( pCur, ex_missing_left_fence );
	}
	
	getFence( fence, tagOn, ci_left );
	runLoop( str, se_fence, NULL );
	getFence( fence, tagOn, ci_right );

	str.write( tagOff );
	prevBuf.append( fence, true );
	prevBuf.append( str, true );
	return false;
}

