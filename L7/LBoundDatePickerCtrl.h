/* 
 * File:   LBoundDatePickerCtrl.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 3 juin 2014, 16:37
 */

#ifndef LBOUNDDATEPICKERCTRL_H
#define	LBOUNDDATEPICKERCTRL_H

#include <wx/datectrl.h>
#include "LBoundControl.h"
#include "LResultSet.h"
/**
 * Initialised with wxInvalidDateTime. The underlying toolkit must allow wxDP_ALLOWNONE.
 * 
 * @param parent
 * @param id
 */
class LBoundDatePickerCtrl : public wxDatePickerCtrl, public LBoundControl
{
public:
    LBoundDatePickerCtrl(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~LBoundDatePickerCtrl();
    /**
     * 
     * @return The current ISO date string, or literal NULL.
     */
    const wxAny GetData();
    /**
     * 
     * @param newData
     * @return false if newData is not a valid ISO date string.
     * True if newData is empty, or literal NULL, or a valid ISO date string.
     */
    bool SetData(const wxAny& newData);
    /**
     * Sets the resultset member and registers the control in the resultset.
     * @param newResultSet
     */
    void SetResultSet(LResultSet * newResultSet);
    /**
     * 
     * @return True if the control contains an invalid date, false otherwise.
     */
    bool IsNull();
    /**
     * Sets the control to invalid date.
     * @return 
     */
    bool SetNull();
    bool IsDirty();
    /**
     * If IsNull() is true, returns wxEmptyString.
     * 
     * Else, returns FormatDate() from the control's value.
     * @return 
     */
    const wxString GetDisplayedData();
    /**
     * Builds a wxDateTime object from a valid ISO date string.
     * 
     * Returns wxInvalidDateTime if the parameter is not a valid ISO date string.
     * 
     * @param ISODate
     * @return 
     */
    static wxDateTime BuildDate(const wxString& ISODate);
    
private:

};

#endif	/* LBOUNDDATEPICKERCTRL_H */

