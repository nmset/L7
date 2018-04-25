/* 
 * File:   LBoundControl.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 28 mai 2014, 15:15
 */

#ifndef LBOUNDCONTROL_H
#define	LBOUNDCONTROL_H

#include <wx/wx.h>
#include "LResultSet.h"

#define L_SQLNULL wxString(_T("NULL"))

class LResultSet;
/**
 * Abstract base class for controls interacting with database table columns.
 * 
 * 
 */
class LBoundControl
{
public:
    LBoundControl();
    virtual ~LBoundControl();
    /**
     * Do not use column aliases.
     * @param newColName
     */
    void SetColumnName(const wxString& newColName)
    {
        m_columnName = newColName;
    }

    const wxString& GetColumnName() const
    {
        return m_columnName;
    }
    /**
     * The data that will be saved in the database table, which may not be the displayed value.
     * 
     * @return 
     */
    virtual const wxAny GetData() = 0;
    virtual bool SetData(const wxAny& newData) = 0;
    virtual void SetResultSet(LResultSet * newResultSet) = 0;
    LResultSet* GetResultSet() const
    {
        return m_rs;
    }
    virtual bool IsNull() = 0;
    virtual bool SetNull() = 0;
    /**
     * If the database column is numeric, use wxEmptystring, else, use a single quote ' .
     * 
     * @param newQuote
     */
    void SetSQLQuote(const wxString& newQuote)
    {
        m_sqlQuote = newQuote;
    }

    const wxString& GetSQLQuote()
    {
        return m_sqlQuote;
    }
    /**
     * Is GetData() different from the table value ?
     * 
     * @return 
     */
    virtual bool IsDirty() = 0;
    /**
     * Data shown on screen, which is not always the data stored in the database table.
     * @return 
     */
    virtual const wxString GetDisplayedData() = 0;
protected:
    wxWeakRef<LResultSet> m_rs;
    wxString m_sqlQuote;
    wxString m_columnName;
private:

};

#endif	/* LBOUNDCONTROL_H */

