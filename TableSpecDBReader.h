#ifndef _TX_TABLESPECDBREADER_H
#define _TX_TABLESPECDBREADER_H

#include <string>

class TableSpec;
struct Conn_settings;
//type: TableSpecDBReader
class TableSpecDBReader{

  private:
  Conn_settings& conn_settings;

  public:
  TableSpecDBReader(Conn_settings& conn_settings): conn_settings(conn_settings)
  {
  }

  ~TableSpecDBReader()
  {

  }

  void read(std::string tableName, TableSpec& tableSpec);

};

#endif