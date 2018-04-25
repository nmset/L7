/* 
 * File:   LGridDateEditor.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 12 juin 2014, 17:27
 */

#ifndef LGRIDDATEEDITOR_H
#define	LGRIDDATEEDITOR_H

#include "LGridColEditor.h"
#include "LBoundDatePickerCtrl.h"
/**
 * Edits table data using an LBoundDatePickerCtrl.
 */
class LGridDateEditor : public wxGridCellEditor, public LGridColEditor {
public:
    /**
     * 
     * @param newColName database column name
     */
    LGridDateEditor(const wxString& newColName);
    /**
     * The editor gets deleted.
     */
    virtual ~LGridDateEditor();
    /**
     * Creates m_control as LBoundDatePickerCtrl.
     * @param parent
     * @param id
     * @param evtHandler
     */
    void Create (wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler);
    /**
     * Creates m_control if necessary. Registers the editor in the grid's resultset.
     * @param row
     * @param col
     * @param grid
     */
    void BeginEdit (int row, int col, wxGrid *grid);
    wxGridCellEditor* Clone () const;
    bool EndEdit (int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval);
    /**
     * Applies the editor value as returned by GetData() to the grid cell.
     * If the editor contains wxInvalidDateTime, the grid cell is set to an empty string.
     * @param row
     * @param col
     * @param grid
     */
    void ApplyEdit (int row, int col, wxGrid *grid);
    /**
     * Deletes the editor, all pointers to the editor are set to NULL.
     */
    void Reset ();
    /**
     * 
     * @return GetData(), or wxEmptyString if the editor control has not been created.
     */
    wxString GetValue() const;
    /**
     * Creates a wxDatePickerCtrl to be used as editor in form view.
     */
    wxControl* ProvideFormEditor(wxWindow * parent);
    wxControl* GetFormEditor() const {return m_formEditor;}
    /**
     * Updates the grid cell and the editor. m_formEditor is deleted and set to NULL.
     * @param row
     * @param col
     * @param grid
     */
    void SyncBack(const int row, const int col, wxGrid * grid);
    
private:
    wxDatePickerCtrl * m_formEditor;
    /**
     * Fully typed alias to m_control.
     */
    LBoundDatePickerCtrl * m_BoundDatePicker;
};

#endif	/* LGRIDDATEEDITOR_H */

