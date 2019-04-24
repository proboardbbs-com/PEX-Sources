/*
 * This file is part of the TurboVision Gadgets++ v2.0 Library
 *
 * Copyright (c) 1995, 1997 by Branislav L. Slantchev
 * A fine product of Silicon Creations, Inc. (gargoyle)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the License which accompanies this
 * software. This library is distributed in the hope that it will
 * be useful, but without any warranty; without even the implied
 * warranty of merchantability or fitness for a particular purpose.
 *
 * You should have received a copy of the License along with this
 * library, in the file LICENSE.DOC; if not, write to the address
 * below to receive a copy via electronic mail.
 *
 * You can reach Branislav L. Slantchev (Silicon Creations, Inc.)
 * at bslantch@cs.angelo.edu. The file SUPPORT.DOC has the current
 * telephone numbers and the postal address for contacts.
*/

#ifndef INCLUDED_BITVECT_H
#define INCLUDED_BITVECT_H
#include "typedef.h"

class zBitVector
{
public:
	enum bitvect_errors
	{
		none  = 0,
		range = 1,
		nomem = 2,
	};

	zBitVector(size_t aNumBits);
	zBitVector(const zBitVector &aVector);
	~zBitVector();

	zBitVector& operator=(const zBitVector &aVector);

	Boolean operator[](size_t bitno) const;
	Boolean has(size_t bitno) const;
	void    set(size_t bitno);
	void    reset(size_t bitno);
	void    toggle(size_t bitno);
	void    clearAll();
	void    setAll();
	int     error() const;
	size_t  capacity() const;

private:
	char   *bits;
	size_t  nbits;
	int     err;
};

inline
zBitVector::~zBitVector()
{
	if( bits ) delete[] bits;
}

inline Boolean
zBitVector::operator[](size_t bitno) const
{
	return (bits[bitno >> 3] & (1 << (bitno & 7))) ? True : False;
}

inline Boolean
zBitVector::has(size_t bitno) const
{
	return (bits[bitno >> 3] & (1 << (bitno & 7))) ? True : False;
}

inline void
zBitVector::reset(size_t bitno)
{
	err = none;
	if( bitno >= nbits ) err = range;
	else bits[bitno >> 3] &= ~(1 << (bitno & 7));
}

inline void
zBitVector::set(size_t bitno)
{
	err = none;
	if( bitno >= nbits ) err = range;
	else bits[bitno >> 3] |= 1 << (bitno & 7);
}

inline void
zBitVector::toggle(size_t bitno)
{
	err = none;
	if( bitno >= nbits ) err = range;
	else bits[bitno >> 3] ^= 1 << (bitno & 7);
}

inline int
zBitVector::error() const
{
	return err;
}

inline size_t
zBitVector::capacity() const
{
	return nbits;
}

#endif /* INCLUDED_BITVECT_H */
