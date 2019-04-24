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
#include "bitvect.h"
#include <string.h>

/*
 * zBitVector - Bit vector class that can handle up to 0xffff flags
*/
zBitVector::zBitVector(size_t aNumBits):
	err(0)
{
    nbits = aNumBits;
	aNumBits = (aNumBits | 7) + 1;
    bits = new char [aNumBits >> 3];
    if( !bits ) err = nomem;
    else memset(bits, 0, aNumBits >> 3);
}

zBitVector::zBitVector(const zBitVector &aVector):
	err(0)
{
    nbits = (aVector.capacity() | 7) + 1;
	if( bits ) delete[] bits;
	bits = new char [nbits >> 3];
	if( !bits ) err = nomem;
	else memmove(bits, aVector.bits, nbits >> 3);
	nbits = aVector.capacity();
}

zBitVector&
zBitVector::operator=(const zBitVector &aVector)
{
	if( (void *)this != (void *)&aVector )
	{
		err = 0;
		nbits = (aVector.capacity() | 7) + 1;
		if( bits ) delete[] bits;
		bits = new char [nbits >> 3];
		if( !bits ) err = nomem;
		else memmove(bits, aVector.bits, nbits >> 3);
		nbits = aVector.capacity();
	}
	return *this;
}

void
zBitVector::clearAll()
{
	memset(bits, 0x00, ((nbits | 7) + 1) >> 3);
}

void
zBitVector::setAll()
{
	memset(bits, 0xff, ((nbits | 7) + 1) >> 3);
}
