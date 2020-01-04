/* 
 * File:   LGridJsonCellRenderer.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 9, 2019, 1:55 PM
 */

#include "LGridJsonCellRenderer.h"
#include "BaseGridPicker.h"
#include <wx/jsonreader.h>

LGridJsonCellRenderer::LGridJsonCellRenderer()
{
}

LGridJsonCellRenderer::~LGridJsonCellRenderer()
{
}

void LGridJsonCellRenderer::Draw(wxGrid & grid,
                                 wxGridCellAttr & attr,
                                 wxDC & dc,
                                 const wxRect & rect,
                                 int row,
                                 int col,
                                 bool isSelected)
{
    const wxString intent = ProcessJsonValue(grid.GetCellValue(row, col));
    grid.SetCellValue(row, col, intent);

    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
}

const wxString LGridJsonCellRenderer::ProcessJsonValue(const wxString& cellValue)
{
    if (cellValue.IsEmpty())
        return INVALID_INTENT;
    
    wxJSONReader reader(wxJSONREADER_STRICT);
    wxJSONValue root;
    uint nbErr = reader.Parse(cellValue, &root);
    if (nbErr > 0)
    {
        const wxArrayString& errors = reader.GetErrors();
        for (int i = 0; i < nbErr; i++)
        {
            wxASSERT_MSG(nbErr == 0, errors[i]);
        }
        // Show raw data.
        return cellValue;
    }
    if (!root.IsArray())
    {
        wxASSERT_MSG(root.IsArray(), _("JSON data is not an array"));
        return cellValue;
    }
    for (uint row = 0; row < root.Size(); row++)
    {
        if (root[row]["Preferred"].IsValid()
                && !root[row]["Preferred"].AsString().IsEmpty()
                && root[row]["Preferred"].AsInt() != 0)
        {
            return root[row]["Intent"].AsString();
        }
    }
    /*
     * Don't show raw data if there's no preferred item.
     * The data will still be in the editor and saved.
     */
    return INVALID_INTENT;
}
