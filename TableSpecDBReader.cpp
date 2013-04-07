#include "TableSpecDBReader.h" 
#include "TableSpec.h"

#include "tx_oracommon.h" 
#include "my_log.h" 

void TableSpecDBReader::read(std::string tableName, TableSpec& tableSpec){
	tableSpec.tableName_ = tableName;

	DEF_ORA_BEGIN("select 1 from dual") 
	  MetaData mdTbl = conn->getMetaData(
						tableName, MetaData::PTYPE_TABLE);

	  vector<MetaData>listOfColumns=mdTbl.getVector(MetaData::ATTR_LIST_COLUMNS);
	  for (int i=0;i<listOfColumns.size();i++){
		   MetaData columnObj=listOfColumns[i];

		   int dType = columnObj.getInt(MetaData::ATTR_DATA_TYPE);
		   Type colType = (Type)((dType==187)?180:dType);

		   tableSpec.addCol(columnObj.getString(MetaData::ATTR_NAME),
							colType,
							columnObj.getInt(MetaData::ATTR_DATA_SIZE));

	  }
	DEF_ORA_END
  }
