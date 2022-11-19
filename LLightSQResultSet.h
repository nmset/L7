/* 
 * File:   LLightSQResultSet.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 *
 * Created on 31 mai 2014, 18:34
 */

#ifdef USE_LIBSQ
#ifndef LLIGHTSQRESULTSET_H
#define LLIGHTSQRESULTSET_H

#include <wx/wx.h>
#include <sqlite3.h>
#include "LLightResultSet.h"
#include "LSQresult.h"
/**
 * SQLite namespace.
 * 
 * Please note USE_LIBSQ pre-processor directive must be defined to include the SQLite backend.
 */
namespace SQ
{

/**
 * Scrollable minimal resultset for the SQLite backend.
 * 
 * This object does not allow data modification and does not output any messages.
 * The underlying SQL query may contain table and/or column aliases
 */
class LLightSQResultSet : public LLightResultSet
{
public:
    LLightSQResultSet();
    LLightSQResultSet(LConnection * newConnection);
    virtual ~LLightSQResultSet();
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
    wxArrayString m_colNames;
    bool RetrieveColNames(SQresult * emptyResult);
};

}
#endif /* LLIGHTSQRESULTSET_H */
#endif
