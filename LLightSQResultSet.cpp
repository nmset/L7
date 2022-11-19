/* 
 * File:   LLightSQResultSet.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 31 mai 2014, 18:34
 */
#ifdef USE_LIBSQ
#include "LLightSQResultSet.h"
#include "LSQConnection.h"
using namespace SQ;

LLightSQResultSet::LLightSQResultSet() : LLightResultSet()
{
}

LLightSQResultSet::LLightSQResultSet(LConnection * newConnection) : LLightResultSet(newConnection)
{
}

LLightSQResultSet::~LLightSQResultSet()
{
    if (m_rs)
    {
        SQresult * srs = static_cast<SQresult*> (m_rs);
        sqlite3_free_table(srs->m_data);
        delete srs;
        m_rs = NULL;
    }
}

bool LLightSQResultSet::SetSQL(const wxString& newSql)
{
    UpdateSQL(newSql);
    if (RunSQL()) First();
    return (m_rs != NULL);
}

bool LLightSQResultSet::RunSQL()
{
    if (m_curSql.IsEmpty()) return false;
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    if (m_rs)
    {
        SQresult * srs = static_cast<SQresult*> (m_rs);
        sqlite3_free_table(srs->m_data);
        wxDELETE(srs);
        ;
        m_rs = NULL;
    }
    m_rs = m_conn->ExecuteSQL(m_curSql);
    SQresult * srs = static_cast<SQresult*> (m_rs);
    if (!RetrieveColNames(srs))
    { // No columns returned if no data !
        m_rs = NULL;
        return false;
    }
    if (m_rs)
    {
        m_cursor = (HasData()) ? 0 : -1;
        if (!m_initialised) m_initialised = true;
        return true;
    }
    else
    {
        m_cursor = -1;
        return false;
    }
}

bool LLightSQResultSet::HasData() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return (srs->m_nbRows > 0);
}

bool LLightSQResultSet::Absolute(const unsigned int newRowIndex)
{
    if (!HasData())
    {
        m_cursor = -1;
        return false;
    }
    wxASSERT_MSG(newRowIndex < GetRowCount(), _T("Paramètre newRowIndex invalide."));
    m_cursor = newRowIndex;
    return true;
}

bool LLightSQResultSet::IsFirst() const
{
    if (!(HasData())) return false;
    return (m_cursor == 0);
}

bool LLightSQResultSet::IsLast() const
{
    if (!(HasData())) return false;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return (m_cursor == (srs->m_nbRows - 1));
}

bool LLightSQResultSet::First()
{
    if (IsFirst()) return false;
    return Absolute(0);
}

bool LLightSQResultSet::Next()
{
    if (IsLast()) return false;
    return Absolute(m_cursor + 1);
}

bool LLightSQResultSet::Previous()
{
    if (IsFirst()) return false;
    return Absolute(m_cursor - 1);
}

bool LLightSQResultSet::Last()
{
    if (IsLast()) return false;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return Absolute(srs->m_nbRows - 1);
}

const unsigned int LLightSQResultSet::GetRowCount() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return srs->m_nbRows;
}

const unsigned int LLightSQResultSet::GetColumnCount() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return srs->m_nbCols;
}

const wxString LLightSQResultSet::GetColumnName(const unsigned int colIndex) const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    if (!HasData()) return m_colNames.Item(colIndex);
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return wxString(srs->m_data[colIndex]);
}

const int LLightSQResultSet::GetColumnIndex(const wxString& colName) const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    if (!HasData()) return m_colNames.Index(colName, false);
    SQresult * srs = static_cast<SQresult*> (m_rs);
    for (unsigned int c = 0; c < srs->m_nbCols; c++)
    {
        const wxString sName(srs->m_data[c]);
        if (sName.Lower() == colName.Lower()) return c;
    }
    return -1;
}

const wxAny LLightSQResultSet::GetData(const unsigned int rowIdx, const unsigned int colIdx) const
{
    if (!HasData()) return wxEmptyString;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    const char * data = srs->m_data[((rowIdx + 1) * (srs->m_nbCols)) + colIdx];
    return wxString(data, wxConvUTF8);
}

const wxAny LLightSQResultSet::GetData(const wxString &columnName) const
{
    if (m_cursor < 0) return wxEmptyString;
    if (!HasData()) return wxEmptyString;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    for (int c = 0; c < srs->m_nbCols; c++)
    {
        const wxString sColName(srs->m_data[c]);
        if (sColName.Lower() == columnName.Lower()) return GetData(m_cursor, c);
    }
    return wxEmptyString;
}

bool LLightSQResultSet::RetrieveColNames(SQresult * emptyResult)
{
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    sqlite3 * db = static_cast<sqlite3*> (m_conn->Get());
    if (db)
    {
        sqlite3_stmt * ppStmt;
        int res = sqlite3_prepare_v2(db, m_curSql, -1, &ppStmt, NULL);
        if (res == SQLITE_OK)
        {
            m_colNames.Clear();
            int colCount = sqlite3_column_count(ppStmt);
            for (unsigned int i = 0; i < colCount; i++)
            {
                const char * constName = sqlite3_column_name(ppStmt, i);
                m_colNames.Add(wxString(constName));
            }
            emptyResult->m_nbCols = m_colNames.GetCount();
            sqlite3_finalize(ppStmt);
            return true;
        }
        else
        {
            int errCode = sqlite3_errcode(db);
            const char * errMsg = sqlite3_errmsg(db);
            const char * errStr = sqlite3_errstr(errCode);
            const wxString msg(wxAny(errCode).As<wxString>() + _T(" ") +
                               wxString(errMsg) + _T("\n") + wxString(errStr));
            wxPrintf("%s\n", msg);
            wxFAIL_MSG(msg);
        }
    }
    return false;
}

#endif