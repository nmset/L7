/* 
 * File:   LBoundCheckBox.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 2 juin 2014, 16:02
 */

#ifndef LBOUNDCHECKBOX_H
#define LBOUNDCHECKBOX_H

#include <wx/checkbox.h>
#include "LBoundControl.h"
#include "LResultSet.h"

class LBoundCheckBox : public wxCheckBox, public LBoundControl {
public:
    LBoundCheckBox(wxWindow* parent, wxWindowID id = wxID_ANY, long style = 0);
    virtual ~LBoundCheckBox();
    /**
     * 
     * @return 0 or 1, or literal NULL if the control is tristate with undetermined state.
     */
    const wxAny GetData();
    /**
     * If the control is tristate and newData is literal NULL, it's state is set to undetermined.
     * If the control is dualstate, wxEmptyString is mapped to unchecked status.
     * 
     * Regardless of the control being dualstate or trisate, 0 is mapped to unchecked status,
     * and any other value to checked status.
     * 
     * @param newData
     */
    void SetData(const wxAny& newData);
    /**
     * Sets the resultset member and registers the control in the resultset.
     * @param newResultSet
     */
    void SetResultSet(LResultSet * newResultSet);
    /**
     * 
     * @return true if the control is trisate with state undetermined, false otherwise.
     */
    bool IsNull();
    /**
     * If control is tristate, it is set to undetermined state. Else,  it is set to unchecked state.
     * @return 
     */
    void SetNull();
    bool IsDirty();
    /**
     * If the control is checked, returns literal Oui
     * 
     * If the control is unchecked, returns literal Non
     * 
     * If the control is tristate with undetermined state, returns literal NULL
     * @return 
     */
    const wxString GetDisplayedData();
private:

};

#endif /* LBOUNDCHECKBOX_H */

