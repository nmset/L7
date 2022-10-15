/* 
 * File:   LLightResultSet.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 28 mai 2014, 14:40
 */

#ifndef LLIGHTRESULTSET_H
#define LLIGHTRESULTSET_H

/* Added to be able to use derived classes independantly
 * If xConnection and xResultSet classes are included in some project,
 * the compiler would complain ofan undeclared wxWeakRef.
 */
#include <wx/wx.h>
#include <wx/weakref.h>
#include "LConnection.h"

/**
 * An abstract class to scroll through data obtained after running an SQL query.
 
 * Does not interact with GUI controls.
 * 
 * Does not output any messages.
 * 
 * Does not modify data.
 */
class LLightResultSet : public wxTrackable
{
public:
    LLightResultSet();
    LLightResultSet(LConnection * newConnection);
    virtual ~LLightResultSet();

    virtual bool SetSQL(const wxString& newSql) = 0;

    /**
     * Updates the SQL statement without running it.
     * @param newSql
     */
    void UpdateSQL(const wxString& newSql)
    {
        m_curSql = newSql;
    }

    const wxString& GetSQL() const
    {
        return m_curSql;
    }

    void SetConnection(LConnection * newConnection)
    {
        m_conn = newConnection;
    }

    const LConnection * GetConnection() const
    {
        return m_conn;
    }
    virtual bool HasData() const = 0;
    virtual bool Absolute(const unsigned int newRowIndex) = 0;
    virtual bool IsFirst() const = 0;
    virtual bool IsLast() const = 0;
    virtual bool First() = 0;
    virtual bool Next() = 0;
    virtual bool Previous() = 0;
    virtual bool Last() = 0;

    /**
     * Returns the cursor position from 0 to (GetRowCount() - 1).
     * 
     * Returns -1 if there's no data.
     */
    const int GetRow() const
    {
        return m_cursor;
    }
    virtual const unsigned int GetRowCount() const = 0;
    virtual const unsigned int GetColumnCount() const = 0;
    virtual const wxString GetColumnName(const unsigned int colIndex) const = 0;
    virtual const int GetColumnIndex(const wxString& colName) const = 0;
    virtual const wxAny GetData(const wxString& colName) const = 0;
    virtual const wxAny GetData(const unsigned int rowIdx, const unsigned int colIdx) const = 0;

protected:
    bool m_initialised;
    wxString m_curSql;
    void * m_rs;
    wxWeakRef<LConnection> m_conn;
    int m_cursor;
    virtual bool RunSQL() = 0;
private:

};

#endif /* LLIGHTRESULTSET_H */

