/* 
 * File:   LBoundXmlGridPicker.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 *
 * Created on December 14, 2019, 10:12 PM
 */

#ifndef LBOUNDXMLGRIDPICKER_H
#define LBOUNDXMLGRIDPICKER_H

#include "XmlGridPickerCtrl.h"
#include "../LBoundControl.h"
#include "../LResultSet.h"

/**
 * Saves an XML document in database.
 * Data type of database column should be TEXT for both sqlite and postgresql.
 * @return 
 */
class LBoundXmlGridPicker : public XmlGridPickerCtrl, public LBoundControl
{
    DECLARE_CLASS(LBoundXmlGridPicker)
public:
    LBoundXmlGridPicker(wxWindow *parent,
                        wxWindowID id,
                        const wxArrayString& types,
                        wxSize popupSize = wxDefaultSize,
                        const wxString& text = wxEmptyString,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxPB_USE_TEXTCTRL,
                        const wxValidator& validator = wxDefaultValidator,
                        const wxString& name = wxButtonNameStr);
    virtual ~LBoundXmlGridPicker();

    /**
     * Sets the resultset member and registers the control in the resultset.
     * @param newResultSet
     */
    void SetResultSet(LResultSet * newResultSet);
    /**
     * 
     * @return If the control is empty, returns literal NULL.
     * Else, returns GetValue().
     */
    const wxAny GetData();
    void SetData(const wxAny& newData);

    bool IsNull()
    {
        return (GetData().As<wxString>() == L_SQLNULL);
    }
    void SetNull();
    /**
     * Is control data same as backend data?
     * <br/><br/>
     * If backend data is cannot be parsed, will always return true, because
     * GetValue() returns empty control data.
     * @return 
     */
    bool IsDirty();

    /**
     * Alias for GetValue().
     * Not really the displayed informative value in the picker'swxTextCtrl.
     * @return 
     */
    const wxString GetDisplayedData()
    {
        return GetValue();
    }

private:

};

#endif /* LBOUNDXMLGRIDPICKER_H */

