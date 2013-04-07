/**
 * Author:		Viacheslav Avramenko
 * Description:	Export/Import table from database
 * Version:		0.1
 */

#include <string>
#include <conio.h>

#include <boost/foreach.hpp>

#include "TableSpec.h"
#include "TableSpecDBReader.h"

#include "tx_oracommon.h" 
#include "my_log.h" 

#include "TableExporter.h" 
using namespace std;

//Main class
class  Tbl_xchange
{
  private:
  Conn_settings& conn_settings;

  public:
  Tbl_xchange(Conn_settings& conn_settings): conn_settings(conn_settings)
  {
  }

  ~Tbl_xchange()
  {

  }

  
  //displays Client Version
  void displayClientVersion(){
	  DEF_ORA_BEGIN("select 1 from dual") 
		  int mav, miv, updNum, pNum, puNum=0;
		  env->getClientVersion(mav, miv, updNum, pNum, puNum);
		  MYLOG_LOG( mav << "." << miv << "." << updNum << "." << pNum << "." << puNum)
	  DEF_ORA_END
  }

  
  //displays Oracle Version
  void displayVersion()
  {
	string sqlStmt = "SELECT banner FROM v$version";
	DEF_ORA_BEGIN(sqlStmt) 
		ResultSet *rset = stmt->executeQuery ();
		while (rset->next()){
			MYLOG_LOG( rset->getString(1))
		}
		stmt->closeResultSet (rset);
	DEF_ORA_END
  }

};

int main(void)
{

	try{
		//Get connection settings
		Conn_settings conn_settings;
	
		conn_settings.load("conn_settings.xml");
		MYLOG_LOG("conn_settings.xml loaded")

		//test
		TableSpec tableSpec;
		{
		  TableSpecDBReader tableSpecDBReader(conn_settings);
		  tableSpecDBReader.read("AVRAVIA_EBCM2.APPROVEDATTACHMENT", tableSpec);
		}

		MYLOG_LOG("TABLE: " << tableSpec.getTableName())
		BOOST_FOREACH( TableCol i, tableSpec.getTableCols() )
		{
			MYLOG_LOG("COLUMN: " << i.colName << " | " << i.colType << " | " << i.colLen)
		}

		//Open connection, create working class
		Tbl_xchange tbl_xchange(conn_settings);
		MYLOG_LOG("Client Version:")
		tbl_xchange.displayClientVersion();
		MYLOG_LOG( "Oracle Banner:")
		tbl_xchange.displayVersion();
		//main logic starts here
		MYLOG_LOG( "BEGIN tbl_xchange")
		TableExporter exporter(conn_settings);
		exporter.make(tableSpec, string("file.f"));
		//get table in buffer
		MYLOG_LOG( "END tbl_xchange")
	}catch (std::exception &e){
        MYLOG_ERROR( "Error: " << e.what())
    }

	_getch();
	return 0;
}