/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: rgidwrapper.cpp
*  Author:   Robert Goor
*
*/
//
// This class wraps a persistent pointer and redirects the persistent object's numerical ID to the wrapper's hash 
// function.  Equality is also computed on the basis of the persistent class's hash function.  The purpose is to 
// allow a straightforward use of the HashTable interface to store persistent prototypes and enable fast
// retrieval
//

#include "rgidwrapper.h"

PERSISTENT_DEFINITION (RGIDWrapper, _RGIDWRAPPER_, "IDWrapper")


RGIDWrapper :: RGIDWrapper () : RGPersistent (), ptr (NULL), SearchID (0) {

}


RGIDWrapper :: RGIDWrapper (RGPersistent* p) : RGPersistent (), ptr (p), SearchID (0) {

}


RGIDWrapper :: ~RGIDWrapper () {

	delete ptr;
}


void RGIDWrapper :: SetSearchID (int id) {

	SearchID = id;
}


RGPersistent* RGIDWrapper :: GetPointer () {

	return ptr;
}


int RGIDWrapper :: GetSearchID () const {

	if (ptr == NULL)
		return SearchID;

	return ptr->GetID ();
}


int RGIDWrapper :: CompareTo (const RGPersistent* q) const {

	RGIDWrapper* Q = (RGIDWrapper*)q;
	int id = GetSearchID ();
	int qid = Q->GetSearchID ();

	if (id < qid)
		return -1;

	if (id > qid)
		return 1;

	return 0;
}


unsigned RGIDWrapper :: HashNumber (unsigned long Base) const {

	if (ptr == NULL)
		return SearchID%Base;
	
	return (ptr->GetID ())%Base;
}


Boolean RGIDWrapper :: IsEqualTo (const RGPersistent* q) const {

	RGIDWrapper* Q = (RGIDWrapper*)q;
	return (GetSearchID () == Q->GetSearchID ());
}

