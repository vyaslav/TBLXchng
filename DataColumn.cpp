#include "DataColumn.h"
#include <boost/foreach.hpp>

using namespace oracle::occi;

DataColumn::DataColumn(TableCol& tableCol):tableCol_(tableCol){
	this->type = (Type)((tableCol.colType==187)?180:tableCol.colType);
	this->length = 0;
	this->ind = 
	this->rc = 0;

	if(tableCol.colType == OCCI_SQLT_CLOB)
		this->buffer=(void *)new Clob();
	else if(tableCol.colType == OCCI_SQLT_BLOB)
		this->buffer=(void *)new Blob();
	else
		this->buffer=(void *)new char[tableCol.colLen];
}

DataColumn::~DataColumn()
{
	if(this->type == OCCI_SQLT_CLOB){
		Clob* clob = (Clob *)this->buffer;
		delete clob;
	}else if(this->type == OCCI_SQLT_BLOB){
		Blob* blob = (Blob *)this->buffer;
		delete blob;
	}else{
		char* cBuffer = (char *)this->buffer;
		delete[] cBuffer;
	}
}

DataColumn::DataColumn(const DataColumn& other){
	this->tableCol_ = other.tableCol_;
	this->type = other.type;
	this->type = other.type;
	this->length = other.length;
	this->ind = other.ind;
	this->rc = other.rc;

	if(this->type == OCCI_SQLT_CLOB){
		const Clob& clob = *((Clob*)other.buffer);
		this->buffer=(void *)new Clob(clob);
	}else if(this->type == OCCI_SQLT_BLOB){
		const Blob& blob = *((Blob*)other.buffer);
		this->buffer=(void *)new Blob(blob);
	}else{
		this->buffer=(void *)new char[this->tableCol_.colLen];
		memcpy(this->buffer, other.buffer, this->tableCol_.colLen);
	}
}

oracle::occi::Type DataColumn::getType() const{
	return type;
}



Clob* DataColumn::getClob(){
	return (this->type == OCCI_SQLT_CLOB)?(Clob *)this->buffer:NULL;
}

Blob* DataColumn::getBlob(){
	return (this->type == OCCI_SQLT_BLOB)?(Blob *)this->buffer:NULL;
}


char* DataColumn::getCharBuffer(){
	return (this->type != OCCI_SQLT_CLOB && this->type != OCCI_SQLT_BLOB)?(char *)this->buffer:NULL;
}

void* DataColumn::getBuffer(){
	return this->buffer;
}

std::vector<DataColumn> DataColumn::transform(TableSpec& tableSpec){
	std::vector<DataColumn> result;
	BOOST_FOREACH( TableCol i, tableSpec.getTableCols() )
	{
		DataColumn dataColumn(i);
		result.push_back(dataColumn);
	}

	return result;
}