/* 
 * File:   LSQResultSet.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 1 juin 2014, 11:43
 */
#ifdef USE_LIBSQ
#include "LSQResultSet.h"
using namespace SQ;

LSQResultSet::LSQResultSet() : LResultSet()
{
}

LSQResultSet::LSQResultSet(LConnection* newConnection) : LResultSet(newConnection)
{
}

LSQResultSet::~LSQResultSet()
{
    if (m_rs)
    {
        SQresult * srs = static_cast<SQresult*> (m_rs);
        sqlite3_free_table(srs->m_data);
        delete srs;
        m_rs = NULL;
    }
}

bool LSQResultSet::SetSQL(const wxString& newSql)
{
    UpdateSQL(newSql);
    if (RunSQL()) First();
    return (m_rs != NULL);
}

bool LSQResultSet::RunSQL()
{
    if (m_curSql == wxEmptyString)
    {
        return false;
    }
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    if (m_rs != NULL)
    {
        SQresult * srs = static_cast<SQresult*> (m_rs);
        sqlite3_free_table(srs->m_data);
        wxDELETE(srs);
        m_rs = NULL;
    }
    m_rs = m_conn->ExecuteSQL(m_curSql);
    SQresult * srs = static_cast<SQresult*> (m_rs);
    if (!RetrieveColNames(srs))
    { // ON EMPTY TABLES, EXECUTESQL RETURNS NO COLUMNS AS WELL
        m_rs = NULL;
        return false;
    }
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
        const LInformation inf(RSSQ000C, RSSQ000M);
        InformLibMessage(inf);
        return false;
    }
}

bool LSQResultSet::HasData() const
{
    if (m_rs == NULL) return false;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    if (srs->m_nbRows == 0) return false;
    return true;
}

bool LSQResultSet::Absolute(const unsigned int newRowIndex)
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
    if (newRowIndex > (GetRowCount() - 1)) return false;
    InformBeforeChangeRow();
    if (!m_canMove) return false;
    m_cursor = newRowIndex;
    Display();
    m_inserting = false;
    InformAfterChangeRow();
    return true;
}

bool LSQResultSet::IsFirst() const
{
    if (!(HasData())) return false;
    return (m_cursor == 0);
}

bool LSQResultSet::IsLast() const
{
    if (!(HasData())) return false;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return (m_cursor == (srs->m_nbRows - 1));
}

bool LSQResultSet::First()
{
    if (IsFirst()) return false;
    return Absolute(0);
}

bool LSQResultSet::Next()
{
    if (IsLast()) return false;
    return Absolute(m_cursor + 1);
}

bool LSQResultSet::Previous()
{
    if (IsFirst()) return false;
    return Absolute(m_cursor - 1);
}

bool LSQResultSet::Last()
{
    if (IsLast()) return false;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return Absolute(srs->m_nbRows - 1);
}

const unsigned int LSQResultSet::GetRowCount() const
{
    if (m_rs == NULL) return -1;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return srs->m_nbRows;
}

const unsigned int LSQResultSet::GetColumnCount() const
{
    if (m_rs == NULL) return -1;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    return srs->m_nbCols;
}

const wxString LSQResultSet::GetColumnName(const unsigned int colIndex) const
{
    if (m_rs == NULL) return wxEmptyString;
    if (!HasData()) return m_colNames.Item(colIndex);
    SQresult * srs = static_cast<SQresult*> (m_rs);
    const char* name = srs->m_data[colIndex];
    if (name) return wxString(name);
    return wxEmptyString;
}

const int LSQResultSet::GetColumnIndex(const wxString& colName) const
{
    if (m_rs == NULL) return -1;
    if (!HasData()) return m_colNames.Index(colName, false);
    SQresult * srs = static_cast<SQresult*> (m_rs);
    for (unsigned int c = 0; c < srs->m_nbCols; c++)
    {
        const char* name = srs->m_data[c];
        const wxString sName(name);
        if (sName.Lower() == colName.Lower()) return c;
    }
    return -1;
}

const wxAny LSQResultSet::GetData(const unsigned int rowIdx, const unsigned int colIdx) const
{
    if (!HasData()) return wxEmptyString;
    SQresult * srs = static_cast<SQresult*> (m_rs);
    const char * data = srs->m_data[((rowIdx + 1) * (srs->m_nbCols)) + colIdx];
    if (data == NULL) return wxEmptyString;
    return wxString(data, wxConvUTF8);
}

const wxAny LSQResultSet::GetData(const wxString &columnName) const
{
    SQresult * srs = static_cast<SQresult*> (m_rs);
    if (m_cursor < 0
        || IsInserting()
        || !HasData()
        || m_cursor > srs->m_nbRows - 1)
        return wxEmptyString;
    for (unsigned int c = 0; c < srs->m_nbCols; c++)
    {
        const char * colName = srs->m_data[c];
        const wxString sColName(colName);
        if (sColName.Lower() == columnName.Lower()) return GetData(m_cursor, c);
    }
    return wxEmptyString;
}

void LSQResultSet::Display()
{
    for (unsigned int i = 0; i < m_BoundControls.GetCount(); i++)
    {
        LBoundControl * control = static_cast<LBoundControl*> (m_BoundControls.Item(i));
        if (!control) continue;
        if (control)
        {
            wxAny aData = GetData(control->GetColumnName());
            control->SetData(aData);
        }
    }
}

bool LSQResultSet::Save()
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
            wxAny newPK = m_conn->GetReturnedValue(0);
            int inewPK;
            newPK.GetAs<int> (&inewPK);
            if (inewPK != 0)
            {
                InformAfterInsert(newPK, sql); // Before RunSQL so that UpdateSQL can be called by client app
                res = RunSQL();
                if (res)
                {
                    m_cursor = GetRowByPKValue(newPK);
                    Display();
                    m_inserting = false;
                    if (oldCursor != m_cursor) InformAfterChangeRow();
                }
                m_conn->ClearReturnedKeys();
            }
            else
            {
                const LInformation inf(RSSQ002C, RSSQ002M);
                InformLibMessage(inf);
            }
        }
        InformAfterSave();
    }
    else
    {
        const LInformation inf(RSSQMC, wxString(m_conn->GetLastLibMessage()));
        InformLibMessage(inf);
    }
    if (res) InformAfterAction(LResultSetEvent::ACTION_SAVE);
    return res;
}

const wxString LSQResultSet::MakeUpdateSQL() const
{
    if (m_readOnly || !m_canSave) return wxEmptyString;
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
        if (!control) continue;
        wxString colName = control->GetColumnName();
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
            // Final string
            sql = sql + _T(" ") + m_tblName + columns + _T("VALUES") + values;
        }
    }
    return sql;
}

const wxString LSQResultSet::MakeDeleteSQL() const
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

const int LSQResultSet::GetRowByPKValue(const wxAny& pkValue) const
{
    if (!HasData()) return -1;
    int colIdx = GetColumnIndex(m_pkColName);
    if (colIdx < 0) return -1;
    for (unsigned int i = 0; i < GetRowCount(); i++)
    {
        wxAny beData = GetData(i, colIdx);
        if (pkValue.As<wxString>() == beData.As<wxString>()) return i;
    }
    return -1;
}

bool LSQResultSet::RetrieveColNames(SQresult * emptyResult)
{
    /*
     * PRAGMA table_info(tbl1)
     * gives table metadata
     * but columns ordering in m_curSql is obviously app dependant
     * 
     */
    wxASSERT_MSG(m_conn != NULL, _T("m_conn est NULL."));
    sqlite3 * db = static_cast<sqlite3*> (m_conn->Get());
    if (db)
    {
        sqlite3_stmt * ppStmt;
        int res = sqlite3_prepare(db, m_curSql, -1, &ppStmt, NULL);
        if (res == SQLITE_OK)
        {
            m_colNames.Clear();
            int colCount = sqlite3_column_count(ppStmt);
            for (unsigned int i = 0; i < colCount; i++)
            {
                const char * name = sqlite3_column_name(ppStmt, i);
                m_colNames.Add(wxString(name));
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
