/* 
 * File:   LLightPQResultSet.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 29 mai 2014, 13:28
 */

#ifdef USE_LIBPQ
#ifndef LLIGHTPQRESULTSET_H
#define LLIGHTPQRESULTSET_H

#include <wx/wx.h>
#include <libpq-fe.h>
#include "LLightResultSet.h"
/**
 * PostgreSQL namespace.
 * 
 * Please note USE_LIBPQ pre-processor directive must be defined to include the PostgreSQL backend.
 */
namespace PQ
{

/**
 * Scrollable minimal resultset for the PostgreSQL backend.
 * 
 * This class does not allow data modification and does not output any messages.
 * The underlying SQL query may contain table and/or column aliases.
 */
class LLightPQResultSet : public LLightResultSet
{
public:
    LLightPQResultSet();
    LLightPQResultSet(LConnection * newConnection);
    virtual ~LLightPQResultSet();
    /**
     * Updates and runs the SQL string.
     * @param newSql
     * @return 
     */
    bool SetSQL(const wxString& newSql);
    bool HasData() const;
    bool Absolute(const unsigned int newRowIndex);
    bool IsFirst() const;
    bool IsLast() const;
    bool First();
    bool Next();
    bool Previous();
    bool Last();
    const unsigned int GetRowCount() const;
    const unsigned int GetColumnCount() const;
    const wxString GetColumnName(const unsigned int colIndex) const;
    const int GetColumnIndex(const wxString& colName) const;
    const wxAny GetData(const int unsigned rowIdx, const unsigned int colIdx) const;
    /**
     * At current row.
     * @param colName
     * @return database table value or wxEmptyString.
     */
    const wxAny GetData(const wxString& colName) const;


private:
    bool RunSQL();
};

}
#endif /* LLIGHTPQRESULTSET_H */
#endif
