#include "CASBuffer.h"
#include "TableExporter.h"
#include "DataColumn.h"
#include "tx_oracommon.h" 
#include <boost/foreach.hpp>
#include "occi.h" 

using namespace oracle::occi;

void TableExporter::make(TableSpec& tableSpec, std::string& file){
	std::string sqlStmt = "select * from "+tableSpec.getTableName();
	MYLOG_LOG("SQL: "<<sqlStmt)
	DEF_ORA_BEGIN(sqlStmt)
		CASBuffer xchBuf(1024);	

		ResultSet *rset = stmt->executeQuery ();
		std::vector<DataColumn>& dataColumns = DataColumn::transform(tableSpec);
		//init Clobs
		int colNum=0;

		

		BOOST_FOREACH( DataColumn& i, dataColumns )
		{
			if ( i.getType() == OCCI_SQLT_CLOB){
				Clob* clob = i.getClob();
				clob->setEmpty(conn);
			}else if ( i.getType() == OCCI_SQLT_BLOB){
				Blob* blob = i.getBlob();
				blob->setEmpty(conn);
			}
			
			++colNum;
			rset->setDataBuffer(colNum, i.buffer, i.getType(), i.tableCol_.colLen, &(i.length), &i.ind, &i.rc);
		}
		
		while (rset->next(1) == oracle::occi::ResultSet::Status::DATA_AVAILABLE){
			MYLOG_LOG("ROW ------------------------------------------------------")
			colNum=0;
			BOOST_FOREACH( DataColumn& i, dataColumns )
			{
				int length = i.length;
				
				char* dataBuffer = (char*)i.buffer;
				
				if (i.ind == -1)
					length = 0;
				else{//IS NULL
					length = (i.getType() == OCCI_SQLT_CLOB)?i.getClob()->length():
							 (i.getType() == OCCI_SQLT_BLOB)?i.getBlob()->length():length;

					if(i.getType() == OCCI_SQLT_CLOB || i.getType() == OCCI_SQLT_BLOB)
						dataBuffer = new char[length];
				}
				//write in buffer
				xchBuf.put((char *)&(i.ind), sizeof(i.ind));
				xchBuf.put((char *)&(length), sizeof(length));
				if ( length > 0 ){
					for(int length_left=length;length_left>0;length_left-=1024){
						char* ptrDataBuffer = dataBuffer + (length - length_left);
						xchBuf.put(ptrDataBuffer, (length_left<1024)?length_left:1024);
					}
				}
				


				//debug info
				++colNum;
				char buffer[33];
				if (i.ind == -1)
					memcpy(buffer, " IS NULL", sizeof(" IS NULL"));
				else{
					if ( i.getType() == OCCI_SQLT_CLOB){
						Clob* clob = i.getClob();
						_itoa(clob->length(), buffer, 10);

						clob->read(length, (unsigned char*)dataBuffer, length);
					}if ( i.getType() == OCCI_SQLT_BLOB){
						Blob* blob = i.getBlob();
						_itoa(blob->length(), buffer, 10);

						blob->read(length, (unsigned char*)dataBuffer, length);
					}else{
						_itoa(i.length, buffer, 10);
					}
				}
				
				MYLOG_LOG(i.tableCol_.colName << 
					": ["<< ((i.getType() == OCCI_SQLT_CLOB)?"CLOB":(i.getType() == OCCI_SQLT_BLOB)?"BLOB":"NORM")<<"] "<<
					buffer )

				//logic

				if(length > 0 && (i.getType() == OCCI_SQLT_CLOB || i.getType() == OCCI_SQLT_BLOB))
					delete[] dataBuffer;
			}
		}

		stmt->closeResultSet (rset);
	DEF_ORA_END

}