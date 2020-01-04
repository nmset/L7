/* 
 * File:   LGridJsonCellEditor.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 9, 2019, 10:36 AM
 */

#ifndef LGRIDJSONCELLEDITOR_H
#define LGRIDJSONCELLEDITOR_H

#include "../LGridColEditor.h"
#include "LBoundJsonGridPicker.h"

class JsonGridEditorRsEVH;
class LGridJsonCellEditor;

/**
 * A grid cell editor using LBoundJsonGridPicker
 * @param newColName
 * @param intentLabel
 * @param types
 * @param popupSize
 */
class LGridJsonCellEditor : public wxGridCellEditor, public LGridColEditor
{
    friend class JsonGridEditorRsEVH;
public:
    LGridJsonCellEditor(const wxString& newColName,
                        const wxString& intentLabel,
                        const wxArrayString& types,
                        wxSize popupSize = wxDefaultSize);
    virtual ~LGridJsonCellEditor();

    /**
     * Creates m_control as LBoundJsonGridPicker.
     * @param parent
     * @param id
     * @param evtHandler
     */
    void Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler);
    /**
     * Creates m_control if necessary. Registers the editor in the grid's resultset.
     * @param row
     * @param col
     * @param grid
     */
    void BeginEdit(int row, int col, wxGrid *grid);
    wxGridCellEditor* Clone() const;
    bool EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval);
    /**
     * Applies the editor value as returned by GetValue() to the grid cell.
     * @param row
     * @param col
     * @param grid
     */
    void ApplyEdit(int row, int col, wxGrid *grid);
    /**
     * Deletes the editor, all pointers to the editor are set to NULL.
     */
    void Reset();
    /**
     * 
     * @return GetData(), or wxEmptyString if the editor control has not been created.
     */
    wxString GetValue() const;
    /**
     * Creates a wxTextCtrl to be used as editor in form view.
     */
    wxControl* ProvideFormEditor(wxWindow * parent);

    wxControl* GetFormEditor() const
    {
        return m_formEditor;
    }
    /**
     * Updates the grid cell and the editor. m_formEditor is deleted and set to NULL.
     * @param row
     * @param col
     * @param grid
     */
    void SyncBack(const int row, const int col, wxGrid * grid);

private:
    wxString m_intentLabel;
    wxArrayString m_choices;
    wxSize m_popupSize;
    JsonGridPickerCtrl * m_formEditor;
    wxAny m_jsonBuffer;
    int m_editRow;
    /**
     * Fully typed alias to m_control.
     */
    LBoundJsonGridPicker * m_BoundJsonGridPicker;
    wxWeakRef<JsonGridEditorRsEVH> m_rsEVH;
};

/**
 * Clears LGridJsonGridEditor::m_jsonBuffer on LBoundGrid::Cancel.
 * Must be trackable. If it is deleted by an application, avoid crash in 
 * destructor of LGridJsonGridEditor.
 */
class JsonGridEditorRsEVH : public LResultSetEvent, public wxTrackable
{
    friend class LGridJsonCellEditor;
public:

private:
    JsonGridEditorRsEVH(LGridJsonCellEditor * owner);
    virtual ~JsonGridEditorRsEVH();
    virtual void AfterAction(LResultSet * caller, ACTIONS action);
    virtual void DataChanged(LResultSet* caller);

    void SetResultSet(LResultSet * caller)
    {
        m_rs = caller;
    }

    LResultSet * m_rs;
    LGridJsonCellEditor * m_owner;
};

#endif /* LGRIDJSONCELLEDITOR_H */

