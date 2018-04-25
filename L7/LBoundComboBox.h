/* 
 * File:   LBoundComboBox.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 3 juin 2014, 19:00
 */

#ifndef LBOUNDCOMBOBOX_H
#define	LBOUNDCOMBOBOX_H

#include <wx/combobox.h>
#include "LBoundControl.h"
#include "LResultSet.h"
/**
 * The control is called 'translated' if its items have associated client data that are instances of LItemData. 
 * The item strings are then only descriptive.
 */
class LBoundComboBox : public wxComboBox, public LBoundControl
{
public:
    LBoundComboBox(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~LBoundComboBox();
    /**
     * 
     * @return true if any of the items is associated with an instance of LItemData
     */
    bool IsTranslated() const;
    bool SelectionIsTranslated() const;
    /**
     * If the control is not translated, returns the string item at idx.
     * 
     * Else, returns the associated data of the item at idx, or wxEmptyString.
     * @param idx
     * @return 
     */
    const wxAny GetDataOfIndex(unsigned int idx) const;
    /**
     * If the control is not translated, calls FindString(idx).
     * 
     * Else, returns the index of the item whose associated data equals the parameter, or wxNOT_FOUND.
     * @param data
     * @return 
     */
    const int GetIndexOfData(const wxAny& data) const;
    /**
     * If the control is not translated, returns wxEmptyString (there is no data associated with any item).
     * 
     * Else, returns the item whose associated data equals the parameter, or wxEmptyString.
     * @param data
     * @return 
     */
    const wxString GetItemOfData(const wxAny& data) const;
    /**
     * If the control is not translated, returns the parameter itself.
     * 
     * Else, returns the associated data of the parameter, or wxEmptyString.
     * @param item
     * @return 
     */
    const wxAny GetDataOfItem(const wxString& item) const;
    /**
     * If the control is not translated, returns wxEmptyString.
     * 
     * Else, returns the first item that does not have an associated data, or wxEmptyString.
     * @return 
     */
    const wxString GetItemOfNull() const;
    /**
     * If the control is not translated, returns wxNOT_FOUND.
     * 
     * Else, returns the index of the first item that does not have an associated data, or wxNOT_FOUND.
     * @return 
     */
    const int GetIndexOfNull() const;
    /**
     * Deletes all items, sets selection to wxNOT_FOUND and wxEmptyString.
     */
    void Clear();
    /**
     * If IsNull() is true, returns literal NULL.
     * 
     * If the control is not translated, returns GetValue().
     * 
     * Else, returns the associated client data value of the selection.
     * @return 
     */
    const wxAny GetData();
    /**
     * If the control is not translated, calls SetValue() and returns FindString().
     * 
     * If newData is empty or literal NULL, calls SetNull().
     * Else, selects the first item whose associated client data equals newData.
     * @param newData
     * @return false if no item can be matched with newData, true otherwise.
     */
    bool SetData(const wxAny& newData);
    /**
     * Sets the resultset member and registers the control in the resultset.
     * @param newResultSet
     */
    void SetResultSet(LResultSet * newResultSet);
    /**
     * If the control is not translated, returns true if the selected item is empty.
     * 
     * Else, return true if the selected item does not have any associated client data.
     * @return 
     */
    bool IsNull();
    /**
     * If the control is not translated, sets its value to wxEmptyString with wxNOT_FOUND selection.
     * 
     * Else, selects the first item that does not have an associated client data.
     * @return 
     */
    bool SetNull();
    bool IsDirty();
    /**
     * Alias for GetValue().
     * @return 
     */
    const wxString GetDisplayedData();
private:

};

#endif	/* LBOUNDCOMBOBOX_H */

