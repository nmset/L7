/* 
 * File:   LBoundSpinCtrl.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 22 septembre 2014, 14:36
 */

#ifndef LBOUNDSPINCTRL_H
#define LBOUNDSPINCTRL_H

#include <wx/spinctrl.h>
#include "LBoundControl.h"
#include "LResultSet.h"

/**
 This integer spin control always has a value. Hence, the associated table column must have the
 * same default value as the initial value of the control.
 */
class LBoundSpinCtrl : public wxSpinCtrl, public LBoundControl
{
public:
    LBoundSpinCtrl(wxWindow *parent,
                   wxWindowID id = wxID_ANY,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
                   int min = 0,
                   int max = 100,
                   int initial = 0);
    virtual ~LBoundSpinCtrl();

    /**
     * Sets the resultset member and registers the control in the resultset.
     * @param newResultSet
     */
    void SetResultSet(LResultSet * newResultSet);

    /**
     * 
     * @return Alias for GetValue().
     */
    const wxAny GetData()
    {
        return GetValue();
    }
    /**
     * If newData is null or empty, sets the control to its initial value.
     * @param newData
     */
    void SetData(const wxAny& newData);

    /**
     * This control is never empty.
     * @return false, always
     */
    bool IsNull()
    {
        return false;
    }

    /**
     * Sets the control to its initial value.
     * @return 
     */
    void SetNull()
    {
        SetData(m_initialValue);
    }
    bool IsDirty();

    /**
     * Alias for GetData().
     * @return 
     */
    const wxString GetDisplayedData()
    {
        return GetData().As<wxString>();
    }

private:
    int m_initialValue;
};

#endif /* LBOUNDSPINCTRL_H */

