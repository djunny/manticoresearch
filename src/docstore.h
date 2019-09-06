//
// $Id$
//

//
// Copyright (c) 2017-2019, Manticore Software LTD (http://manticoresearch.com)
// All rights reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License. You should have
// received a copy of the GPL license along with this program; if you
// did not, you can find it at http://www.gnu.org/
//

#ifndef _docstore_
#define _docstore_

#include "sphinx.h"


class DocstoreFields_i
{
public:
	virtual		~DocstoreFields_i() {}

	virtual int	AddField ( const CSphString & sName, DocstoreDataType_e eType ) = 0;
	virtual int	GetFieldId ( const CSphString & sName, DocstoreDataType_e eType ) const = 0;
};


class Docstore_i : public virtual DocstoreFields_i
{
public:
	virtual DocstoreDoc_t		GetDoc ( RowID_t tRowID, const VecTraits_T<int> * pFieldIds=nullptr ) const = 0;
	virtual DocstoreSettings_t	GetDocstoreSettings() const = 0;
};


class DocstoreBuilder_i : public virtual DocstoreFields_i
{
public:
	struct Doc_t
	{
		CSphVector<VecTraits_T<BYTE>> m_dFields;

		Doc_t();
		Doc_t ( const DocstoreDoc_t & tDoc );
	};


	virtual void	AddDoc ( RowID_t tRowID, const Doc_t & tDoc ) = 0;
	virtual void	Finalize() = 0;
};


class CSphReader;
class CSphWriter;
class DocstoreRT_i : public Docstore_i, public DocstoreBuilder_i
{
public:
	virtual bool	Load ( CSphReader & tReader, CSphString & sError ) = 0;
	virtual void	Save ( CSphWriter & tWriter ) = 0;

	virtual void	AddPackedDoc ( RowID_t tRowID, BYTE * pDoc ) = 0;
	virtual BYTE *	LeakPackedDoc ( RowID_t tRowID ) = 0;

	virtual int64_t	AllocatedBytes() const = 0;
};


Docstore_i *		CreateDocstore ( const CSphString & sFilename, CSphString & sError );
DocstoreBuilder_i * CreateDocstoreBuilder ( const CSphString & sFilename, const DocstoreSettings_t & tSettings, CSphString & sError );
DocstoreRT_i *		CreateDocstoreRT();
DocstoreFields_i *	CreateDocstoreFields();

void				InitDocstoreCache ( int iCacheSize );
void				ShutdownDocstoreCache();

#endif

//
// $Id$
//
