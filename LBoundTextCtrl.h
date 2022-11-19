/* 
 * File:   LBoundTextCtrl.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 *
 * Created on 2 juin 2014, 14:52
 */

#ifndef LBOUNDTEXTCTRL_H
#define LBOUNDTEXTCTRL_H

#include <wx/textctrl.h>
#include "LBoundControl.h"
#include "LResultSet.h"

class LBoundTextCtrl : public wxTextCtrl, public LBoundControl
{
public:
    LBoundTextCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, long style = 0);
    virtual ~LBoundTextCtrl();
    /**
     * Sets the resultset member and registers the control in the resultset.
     * @param newResultSet
     */
    void SetResultSet(LResultSet * newResultSet);
    /**
     * 
     * @return If the control is empty, returns literal NULL. Else, returns GetValue().
     */
    const wxAny GetData();
    /**
     * Calls ChangeValue().
     * @param newData
     */
    void SetData(const wxAny& newData);

    /**
     * Alias for IsEmpty().
     * @return 
     */
    bool IsNull()
    {
        return IsEmpty();
    }

    /**
     * Alias for Clear().
     * @return 
     */
    void SetNull()
    {
        Clear();
    }
    bool IsDirty();

    /**
     * Alias for GetValue().
     * @return 
     */
    const wxString GetDisplayedData()
    {
        return GetValue();
    }

private:
};

#endif /* LBOUNDTEXTCTRL_H */

