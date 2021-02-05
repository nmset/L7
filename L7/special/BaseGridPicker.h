/* 
 * File:   BaseGridPicker.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 7, 2019, 9:40 PM
 */

#ifndef BASEGRIDPICKER_H
#define BASEGRIDPICKER_H

#include "BasePicker.h"
#include <wx/grid.h>

#define INVALID_INTENT _("[Invalid]")

/**
 * Abstract class adding a wxGrid to BasePicker.
 * <br/>The grid is contained in the popup. It has 4 columns :
 * <br/><br/> Column 0 : The intent : phone, email..., in single line
 * <br/> Column 1 : Type : Home, Work...
 * <br/> Column 2 : Preferred : one entry should be preferred
 * <br/> Column 3 : Short notes in single line
 * <br/><br/>
 * The label of the Intent column can be changed by an application. The other
 * columns have fixed labels.
 * <br/>
 * The Type column is edited with a combobox. Its values are set by
 * SetColTypeChoices().
 * <br/>
 * The Preferred column allows selecting one single entry as preferred. If none
 * is selected, the dataset persists but considered invalid.
 * <br/>
 * Selection mode is wxGridSelectRows. Row labels are hidden.
 * <br/>
 * Three empty rows are available for quick multiple inputs.
 * @return 
 */
class BaseGridPicker : public BasePicker
{
    DECLARE_CLASS(BaseGridPicker)
public:
    BaseGridPicker(wxWindow *parent,
                   wxWindowID id,
                   const wxArrayString& types,
                   wxSize popupSize = wxDefaultSize,
                   const wxString& text = wxEmptyString,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxPB_USE_TEXTCTRL,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxButtonNameStr);
    virtual ~BaseGridPicker();
    /**
     * Unused, empty implementation.
     */
    virtual void UpdatePickerFromTextCtrl();
    /**
     * Unused, empty implementation.
     */
    virtual void UpdateTextCtrlFromPicker();

    virtual void SetValue(const wxString& value) = 0;

    virtual wxString GetValue() = 0;

    /**
     * Sets the values of the Type column in a combobox.
     * @param choices
     */
    void SetColTypeChoices(const wxArrayString& choices)
    {
        m_colTypeChoices = choices;
    }

    /**
     * Gets the values of the Type column.
     * @return 
     */
    wxArrayString GetColTypeChoices() const
    {
        return m_colTypeChoices;
    }
    /**
     * Sets the label of the Intent column.
     * @param intent
     */
    void SetIntentLabel(const wxString& intent);

    /**
     * Gets the label of the Intent column.
     * @return 
     */
    wxString GetIntentLabel() const
    {
        return m_intentLabel;
    }
    /**
     * Gets the Intent value, the one that is marked as preferred.
     * It may return an empty string if no item is preferred.
     * @return 
     */
    wxString GetIntent() const;

    /**
     * Can we modify data ?
     * @return 
     */
    bool IsEditable() const
    {
        return m_editable;
    }
    /**
     * Grid data may be editable or not. In the latter case, no empty rows are
     * shown.
     * @param editable
     */
    void EnableEditing(bool editable);

protected:
    /**
     * Saves the grid's content internally and updates the wxTextCtrl's value.
     * @param evt
     */
    virtual void OnPopupHidden(wxShowEvent& evt);
    virtual void FillGrid() = 0;
    virtual void DumpGrid() = 0;
    /**
     * Creates, configures and fills the grid by calling FillGrid(). A previous
     * grid is deleted.
     */
    void CreateGrid();
    wxGrid * m_grid;
    wxGridStringTable * m_stringTable;
    bool m_editable;
private:
    wxArrayString m_colTypeChoices;
    wxString m_intentLabel;
    uint m_nbInsertRows;
    /**
     * Configures the Preferred column for editing and rendering using a
     * checkbox.
     */
    void PreparePreferredCol();
    /**
     * Configures the Type column for editing using a combobox.
     */
    void PrepareTypeCol();
    /**
     * Adds rows and columns, configures the columns, further setting sizes
     * and labels.
     * <br/>
     * Binds the grid to editor creation and hidden events.
     */
    void PrepareGrid();
    /**
     * Concerns the Preferred column only. When the editor is checked, ensures
     * the other rows are unchecked.
     * @param evt
     */
    void OnPrefCellChanged(wxGridEvent& evt);
    /**
     * Just calls BasePicker::DoShowPopup().
     * @param evt
     */
    virtual void ShowPopup(wxCommandEvent& evt);

};

#endif /* BASEGRIDPICKER_H */

