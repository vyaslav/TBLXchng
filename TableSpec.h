#ifndef _TX_TABLESPEC_H
#define _TX_TABLESPEC_H

#include <string>
#include <vector>
#include <occi.h>

class TableSpecDBReader; 

//type: TableCol
struct TableCol{
	std::string colName;
	oracle::occi::Type colType;
	int colLen;
};

typedef std::vector<TableCol> TableCols;
//type: TableSpec
class TableSpec
{
	friend class TableSpecDBReader;

private:
	std::vector<TableCol> tableCols_;
	std::string tableName_;

   void addCol(const std::string colName, const oracle::occi::Type colType, const int colLen);
	
public:
   TableCols getTableCols(){
     return tableCols_;//copy constructor
   }

   std::string getTableName(){
     return tableName_;
   }

   void save(const std::string fileName){

   }

   void load(const std::string fileName){

   }

};

#endif
