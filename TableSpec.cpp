#include "TableSpec.h" 

void TableSpec::addCol(const std::string colName, const oracle::occi::Type colType, const int colLen){
      TableCol tableCol;
	  tableCol.colName = colName; //copy constructor
	  tableCol.colType = colType;
	  tableCol.colLen = colLen;

	  tableCols_.push_back(tableCol);
}