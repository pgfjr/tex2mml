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

#ifndef __classes
#define __classes

#include <memory.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include "exceptions.h"

#ifndef ZeroMemory
#define ZeroMemory(p,n) memset( (p), 0, (n) )
#endif



struct BufferStruct {
	char *m_buf;
	size_t m_index, m_size;
};

struct Buffer {
	char *m_buf;
	size_t m_index, m_size;
    Buffer();
	~Buffer();
	void setlength( size_t len );
	void write( const char *s, size_t len );
	void write( const char *s );
	size_t  length();
	void append( Buffer &buf, bool transfer = false );
	char *data( size_t *len = NULL );
	void insertAt( size_t index, const char *s );
	void format( const char *fmt, ... );
	void releaseBuffer( BufferStruct &buf );
	void reset();
	char *release();
	void destroy();
private:
	void _write( size_t index, const char *s, size_t len );
};

#endif