/* 
 * File:   LPQResultSet.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 29 mai 2014, 16:19
 */

#ifdef USE_LIBPQ

#include "LPQResultSet.h"
#include "LLightPQResultSet.h"
using namespace PQ;

LPQResultSet::LPQResultSet() : LResultSet()
{

}

LPQResultSet::LPQResultSet(LConnection* newConnection) : LResultSet(newConnection)
{
}

LPQResultSet::~LPQResultSet()
{
    if (m_rs)
    {
        PGresult * prs = static_cast<PGresult*> (m_rs);
        PQclear(prs);
        m_rs = NULL;
    }
}

bool LPQResultSet::SetSQL(const wxString& newSql)
{
    UpdateSQL(newSql);
    if (RunSQL()) First();
    return (m_rs != NULL);
}

bool LPQResultSet::RunSQL()
{
    if (m_curSql.IsEmpty())
    {
        return false;
    }
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
        Display();
        m_inserting = false;
        if (!m_initialised)
        {
            m_initialised = true;
            InformInitialised();
        }
        return true;
    }
    else
    {
        m_cursor = -1;
        Display();
        m_inserting = false;
        const LInformation inf(RS000C, RS000M);
        InformLibMessage(inf);
        return false;
    }
}

bool LPQResultSet::HasData() const
{
    if (m_rs == NULL) return false;
    PGresult * prs = static_cast<PGresult*> (m_rs);
    if (PQntuples(prs) == 0) return false;
    return true;
}

bool LPQResultSet::Absolute(const unsigned int newRowIndex)
{
    if (!m_readOnly)
    {
        if (m_inserting)
        {
            InformInserting();
            return false;
        }
        if (IsDirty())
        {
            InformDirty();
            return false;
        }
    }
    if (!HasData())
    {
        m_cursor = -1;
        Display();
        return false;
    }
    wxASSERT_MSG(newRowIndex < GetRowCount(), _T("Paramètre newRowIndex invalide."));
    InformBeforeChangeRow();
    if (!m_canMove) return false;
    m_cursor = newRowIndex;
    Display();
    m_inserting = false;
    InformAfterChangeRow();
    return true;
}

bool LPQResultSet::IsFirst() const
{
    if (m_inserting) return false;
    if (!(HasData())) return false;
    return (m_cursor == 0);
}

bool LPQResultSet::IsLast() const
{
    if (m_inserting) return false;
    if (!(HasData())) return false;
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return (m_cursor == (PQntuples(prs) - 1));
}

bool LPQResultSet::First()
{
    if (IsFirst()) return false;
    return Absolute(0);
}

bool LPQResultSet::Last()
{
    if (IsLast()) return false;
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return Absolute(PQntuples(prs) - 1);
}

bool LPQResultSet::Next()
{
    if (IsLast()) return false;
    return Absolute(m_cursor + 1);
}

bool LPQResultSet::Previous()
{
    if (IsFirst()) return false;
    return Absolute(m_cursor - 1);
}

const unsigned int LPQResultSet::GetRowCount() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return PQntuples(prs);
}

const unsigned int LPQResultSet::GetColumnCount() const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return PQnfields(prs);
}

const wxString LPQResultSet::GetColumnName(const unsigned int colIndex) const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    const char * name = PQfname(prs, colIndex);
    if (name) return wxString(name);
    return wxEmptyString;
}

const int LPQResultSet::GetColumnIndex(const wxString& colName) const
{
    wxASSERT_MSG(m_rs != NULL, _T("m_rs est NULL."));
    PGresult * prs = static_cast<PGresult*> (m_rs);
    return PQfnumber(prs, colName.c_str());
}

const wxAny LPQResultSet::GetData(const unsigned int rowIdx, const unsigned int colIdx) const
{
    PGresult * prs = static_cast<PGresult*> (m_rs);
    const char * data = PQgetvalue(prs, rowIdx, colIdx);
    return wxString(data, wxConvUTF8);
}

const wxAny LPQResultSet::GetData(const wxString& columnName) const
{
    PGresult * prs = static_cast<PGresult*> (m_rs);
    if (m_cursor < 0
        || IsInserting()
        || !HasData()
        || m_cursor > PQntuples(prs) - 1)
        return wxEmptyString;
    const char* colName = columnName.c_str();
    int colIdx = PQfnumber(prs, colName);
    if (colIdx < 0 || colIdx >= PQnfields(prs))
    {
        const wxString msg = RS004M + columnName;
        const LInformation inf(RS004C, msg);
        InformLibMessage(inf);
        return wxEmptyString;
    }
    return GetData(m_cursor, colIdx);
}

void LPQResultSet::Display()
{
    for (unsigned int i = 0; i < m_BoundControls.GetCount(); i++)
    {
        LBoundControl * control = static_cast<LBoundControl*> (m_BoundControls.Item(i));
        if (!control) continue;
        wxAny aData = GetData(control->GetColumnName());
        control->SetData(aData);
    }
}

bool LPQResultSet::Save()
{
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    if (m_readOnly || m_conn->IsReadOnly()) return false;
    if (!m_inserting && !HasData()) return false;
    InformBeforeSave();
    if (!m_canSave) return false;
    LBoundControl* pkControl = GetControl(m_pkColName);
    if (!pkControl) return false;
    if (pkControl->IsNull())
    {
        InformBeforeInsert();
        if (!m_canInsert) return false;
    }
    else
    {
        InformBeforeUpdate();
        if (!m_canUpdate) return false;
    }
    const wxString sql = MakeUpdateSQL();
    if (sql.IsEmpty()) return false;
    bool updating = (sql.Mid(0, 7) == _T("UPDATE "));
    wxAny pkValue;
    bool res;
    if (updating) pkValue = GetData(m_pkColName);
    if (m_conn->ExecuteUpdateSQL(sql))
    {
        const int oldCursor = m_cursor;
        if (updating)
        {
            InformAfterUpdate(pkValue, sql);
            res = RunSQL();
            if (res)
            {
                m_cursor = GetRowByPKValue(pkValue);
                Display();
                if (oldCursor != m_cursor) InformAfterChangeRow();
            }
        }
        else
        {
            void * vretKeys = m_conn->GetReturnedKeys();
            PGresult * retKeys = static_cast<PGresult*> (vretKeys);
            if (retKeys != NULL)
            {
                const char * data = PQgetvalue(retKeys, 0, 0); // RETURNING pkColName from MakeUpdateSQL
                wxAny newPK = wxString(data);
                InformAfterInsert(newPK, sql); // Before RunSQL so as UpdateSQL can be called by client app
                res = RunSQL();
                if (res)
                {
                    m_cursor = GetRowByPKValue(newPK);
                    Display();
                    m_inserting = false;
                    if (oldCursor != m_cursor) InformAfterChangeRow();
                }
                //m_conn->ClearReturnedKeys(); // Not here
            }
            else
            {
                const LInformation inf(RS002C, RS002M);
                InformLibMessage(inf);
            }
        }
        InformAfterSave();
    }
    else
    {
        const LInformation inf(RSPQMC, wxString(m_conn->GetLastLibMessage()));
        InformLibMessage(inf);
    }
    m_conn->ClearReturnedKeys(); // Always clear, even if update.
    if (res) InformAfterAction(LResultSetEvent::ACTION_SAVE);
    return res;
}

const wxString LPQResultSet::MakeUpdateSQL() const
{
    if (m_readOnly || !m_canSave || !m_canInsert || !m_canUpdate) return wxEmptyString;
    if (!m_inserting && !HasData()) return wxEmptyString;
    LBoundControl* pkControl = GetControl(m_pkColName);
    if (pkControl == NULL) return wxEmptyString;
    wxString sql = _T("UPDATE");
    bool updating = true;
    if (pkControl->IsNull())
    {
        sql = _T("INSERT INTO");
        updating = false;
    }
    wxString columns = wxEmptyString;
    wxString values = wxEmptyString;
    wxString singleValue = wxEmptyString;
    for (unsigned int i = 0; i < m_BoundControls.GetCount(); i++)
    {
        LBoundControl * control = static_cast<LBoundControl*> (m_BoundControls.Item(i));
        if (!control) continue;
        const wxString colName = control->GetColumnName();
        if (updating)
        {
            if (colName.Lower() != m_pkColName.Lower())
            {
                if (control->IsDirty())
                {
                    // Build "col = value, " string
                    singleValue = wxEmptyString;
                    control->GetData().GetAs<wxString> (&singleValue);
                    EscapeChar(singleValue);
                    const wxString quote = control->IsNull() ? wxString(wxEmptyString) : control->GetSQLQuote();
                    values += colName + _T(" = ") + quote
                            + singleValue
                            + quote + _T(", ");
                }
            }
        }
        else
        {
            if (colName.Lower() != m_pkColName.Lower())
            {
                if (control->IsDirty())
                {
                    // Construct "col1, col2,..." string
                    columns += colName + _T(", ");
                    singleValue = wxEmptyString;
                    control->GetData().GetAs<wxString> (&singleValue);
                    EscapeChar(singleValue);
                    // Construct "value1, value2,..." string
                    const wxString quote = control->IsNull() ? wxString(wxEmptyString) : control->GetSQLQuote();
                    values += quote
                            + singleValue
                            + quote + _T(", ");
                }
            }
        }
    }
    if (updating)
    {
        if (values.IsEmpty())
        {
            sql = wxEmptyString;
        }
        else
        {
            // Append "WHERE pkColName = value"
            values = values.Mid(0, (values.Len() - 2));
            singleValue = wxEmptyString;
            pkControl->GetData().GetAs<wxString> (&singleValue);
            values += _T(" WHERE ") + m_pkColName + _T(" = ")
                    + pkControl->GetSQLQuote() + singleValue
                    + pkControl->GetSQLQuote();
            // Build final string
            sql += _T(" ") + m_tblName + _T(" SET ") + values;
        }
    }
    else
    {
        if (values.IsEmpty() || columns.IsEmpty())
        {
            sql = wxEmptyString;
        }
        else
        {
            // Manage master/child columns
            wxString childColList = wxEmptyString;
            wxString masterValueList = wxEmptyString;
            if (IsChild())
            {
                childColList = MakeChildInsertColList();
                masterValueList = MakeMasterValueList();
            }
            columns = childColList + columns;
            columns = columns.Mid(0, (columns.Len() - 2));
            columns = _T(" (") + columns + _T(") ");
            values = masterValueList + values;
            values = values.Mid(0, (values.Len() - 2));
            values = _T(" (") + values + _T(") ");
            // Near final string
            sql = sql + _T(" ") + m_tblName + columns + _T("VALUES") + values;
            // Add returning primary key clause
            if (!m_pkColName.IsEmpty())
            {
                sql += _T(" RETURNING ") + m_pkColName;
            }
        }
    }
    return sql;
}

const wxString LPQResultSet::MakeDeleteSQL() const
{
    if (m_readOnly || !m_canDelete) return wxEmptyString;
    if (!HasData()) return wxEmptyString;
    if (m_inserting) return wxEmptyString;
    LBoundControl* pkControl = GetControl(m_pkColName);
    if (pkControl == NULL) return wxEmptyString;
    wxString pkValue = wxEmptyString;
    pkControl->GetData().GetAs<wxString>(&pkValue);
    EscapeChar(pkValue); // ???
    wxString sql = _T("DELETE FROM ") + m_tblName
            + _T(" WHERE ") + m_pkColName + _T(" = ")
            + pkControl->GetSQLQuote()
            + pkValue
            + pkControl->GetSQLQuote();
    return sql;
}

const int LPQResultSet::GetRowByPKValue(const wxAny& pkValue) const
{
    if (!HasData()) return -1;
    PGresult * prs = static_cast<PGresult*> (m_rs);
    int colIdx = PQfnumber(prs, m_pkColName.c_str());
    if (colIdx < 0) return -1;
    for (unsigned int i = 0; i < GetRowCount(); i++)
    {
        const wxAny beData = GetData(i, colIdx);
        if (pkValue.As<wxString>() == beData.As<wxString>()) return i;
    }
    return -1;
}

#endif
