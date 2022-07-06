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

#include "classes.h"
#include <stdio.h>

Buffer::Buffer()
{
	m_buf   = NULL;
	m_index = 0;
	m_size  = 0;
}

Buffer::~Buffer()
{
	destroy();
}

void Buffer::destroy()
{
	if( m_buf != NULL )
		free( m_buf );

	m_buf   = NULL;
	m_index = 0;
}

void Buffer::setlength( size_t len )
{
	char *tmp = (char *) realloc( m_buf, len + 1 );

	if( tmp != NULL )
	{
		m_buf = tmp;

		m_size = len;
		if( m_index > len )	// truncated
		{
			m_index = len;
		}
		else
		{			
			ZeroMemory( &m_buf[ m_index ], ( len - m_index ) );
		}
	}
	else
		throw ex_out_of_memory;
}

size_t  Buffer::length()
{
	return m_index;
}

void Buffer::format( const char *fmt, ... )
{
	va_list list;
	size_t len;
	char s[256];

	va_start( list, fmt );
	len = vsprintf_s( s, sizeof( s ) - 1, fmt, list );
	//len = vsprintf( s, fmt, list );
	va_end( list );
	_write( m_index, s, len );
}


void Buffer::_write( size_t index, const char *s, size_t len )
{
	if( !s || ( len <= 0 ) )
	{
		return;
	}

	if( ( m_size - m_index ) <= len	)
	{
		setlength( m_size + len + 1);
		
	}

	memcpy( &m_buf[ index ], s, len );

	
	m_index += len;
}


void Buffer::write( const char *s, size_t len )
{
	_write( m_index, s, len );
}

void Buffer::write( const char *s )
{
	_write( m_index, s, strlen( s ) );
}

char *Buffer::data( size_t *len )
{
	if( len != NULL )
	{
		*len = m_index;
	}

	return m_buf;
}


void Buffer::append( Buffer &buf, bool transfer )
{
	if( transfer && m_index == 0 )
	{
		BufferStruct temp;

		buf.releaseBuffer( temp );

		m_buf   = temp.m_buf;
		m_index = temp.m_index;
		m_size  = temp.m_size;
	}
	else
	{
		_write( m_index, buf.m_buf, buf.m_index );
	}
}

void Buffer::releaseBuffer( BufferStruct &buf )
{
	buf.m_buf   = m_buf;
	buf.m_index = m_index;
	buf.m_size  = m_size;
	
	m_size  = 0;
	m_index = 0;	
	m_buf   = NULL;
}

void Buffer::reset()
{
	ZeroMemory( m_buf, m_index );	
	m_index  = 0;
}

char *Buffer::release()
{
	char *temp;

	temp    = m_buf;
	m_size  = 0;
	m_index = 0;	
	m_buf   = NULL;

	return temp;
}

void Buffer::insertAt( size_t index, const char *s )
{
	size_t len, count;
	char *src, *dest;

	len = strlen( s );

	if( len == 0 )
	  return;

	if( index >= m_index )
	{
		_write( index, s, len );
		return;
	}

	if( ( m_size - m_index ) <= len	)
	{
		setlength( m_size + len + 1);		
	}
	src = m_buf + index;
	dest = src + len;
	
	count = m_index - index;
	
	//memmove_s( dest, m_size, src, count );

	memmove( dest, src, count );	
	memcpy( m_buf + index, s, len );
	m_index += len;
}
