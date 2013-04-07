#include "TableSpec.h"
#include <occi.h>

class TableExporter;

class DataColumn{
	friend class TableExporter;
  private:
	void *buffer;

	oracle::occi::Type type;
	ub2 length;
	sb2 ind; 
	ub2 rc;

	TableCol tableCol_;
  public:
	DataColumn(TableCol& tableCol);
	~DataColumn();
	DataColumn(const DataColumn& other);


	oracle::occi::Type getType() const;

	oracle::occi::Clob* getClob();
	oracle::occi::Blob* getBlob();
	char* getCharBuffer();
	void* getBuffer();

	static std::vector<DataColumn> transform(TableSpec& tableSpec);
};