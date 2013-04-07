#include "TableSpec.h"
#include <string>

struct Conn_settings;

class TableExporter
{
private:
	Conn_settings& conn_settings;
public:
  TableExporter(Conn_settings& conn_settings): conn_settings(conn_settings)
  {
  }

  ~TableExporter()
  {

  }

  void make(TableSpec& tableSpec, std::string& file);

};