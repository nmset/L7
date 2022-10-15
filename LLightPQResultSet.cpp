/* 
 * File:   LLightPQResultSet.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 29 mai 2014, 13:29
 */

#ifdef USE_LIBPQ
#include "LLightPQResultSet.h"
using namespace PQ;

LLightPQResultSet::LLightPQResultSet() : LLightResultSet()
{
}

LLightPQResultSet::LLightPQResultSet(LConnection * newConnection) : LLightResultSet(newConnection)
{
}

LLightPQResultSet::~LLightPQResultSet()
{
    if (m_rs)
    {
        PGresult * prs = static_cast<PGresult*> (m_rs);
        PQclear(prs);
        m_rs = NULL;
    }
}

bool LLightPQResultSet::SetSQL(const wxString& newSql)
{
    UpdateSQL(newSql);
    if (RunSQL()) First();
    return (m_rs != NULL);
}

bool LLightPQResultSet::RunSQL()
{
    if (m_curSql.IsEmpty()) return false;
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    if (m_rs)
    {
        PGresult * prs = static_cast<PGresult*> (m_rs);
        PQclear(prs);
        m_rs = NULL;
    }
    void * vrs = m_conn->ExecuteSQL(m_curSql);
    m_rs = static_cast<PGresult*> (vrs);
    if (m_rs != NULL)
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

bool LLightPQResultSet::HasData() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return (PQntuples(prs) > 0);
}

bool LLightPQResultSet::Absolute(const unsigned int newRowIndex)
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

bool LLightPQResultSet::IsFirst() const
{
    if (!(HasData())) return false;
    return (m_cursor == 0);
}

bool LLightPQResultSet::IsLast() const
{
    if (!(HasData())) return false;
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return (m_cursor == (PQntuples(prs) - 1));
}

bool LLightPQResultSet::First()
{
    if (IsFirst()) return false;
    return Absolute(0);
}

bool LLightPQResultSet::Next()
{
    if (IsLast()) return false;
    return Absolute(m_cursor + 1);
}

bool LLightPQResultSet::Previous()
{
    if (IsFirst()) return false;
    return Absolute(m_cursor - 1);
}

bool LLightPQResultSet::Last()
{
    if (IsLast()) return false;
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return Absolute(PQntuples(prs) - 1);
}

const unsigned int LLightPQResultSet::GetRowCount() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return PQntuples(prs);
}

const unsigned int LLightPQResultSet::GetColumnCount() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return PQnfields(prs);
}

const wxString LLightPQResultSet::GetColumnName(const unsigned int colIndex) const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return wxString(PQfname(prs, colIndex));
}

const int LLightPQResultSet::GetColumnIndex(const wxString& colName) const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return PQfnumber(prs, colName.c_str());
}

const wxAny LLightPQResultSet::GetData(const unsigned int rowIdx, const unsigned int colIdx) const
{
    PGresult * prs = static_cast<PGresult*> (m_rs);
    const char * data = PQgetvalue(prs, rowIdx, colIdx);
    return wxString(data, wxConvUTF8);
}

const wxAny LLightPQResultSet::GetData(const wxString &columnName) const
{
    if (m_cursor < 0) return wxEmptyString;
    const char* colName = columnName.c_str();
    PGresult * prs = static_cast<PGresult*> (m_rs);
    int colIdx = PQfnumber(prs, colName);
    if (colIdx < 0 || colIdx >= PQnfields(prs)) return wxEmptyString;
    return GetData(m_cursor, colIdx);
}

#endif
