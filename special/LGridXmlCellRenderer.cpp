/* 
 * File:   LGridXmlCellRenderer.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on December 15, 2019, 6:18 PM
 */

#include "LGridXmlCellRenderer.h"
#include "XmlHelper.h"
#include "BaseGridPicker.h"

LGridXmlCellRenderer::LGridXmlCellRenderer()
{
}

LGridXmlCellRenderer::~LGridXmlCellRenderer()
{
}

void LGridXmlCellRenderer::Draw(wxGrid & grid,
                                wxGridCellAttr & attr,
                                wxDC & dc,
                                const wxRect & rect,
                                int row,
                                int col,
                                bool isSelected)
{
    const wxString intent = ProcessXmlValue(grid.GetCellValue(row, col));
    grid.SetCellValue(row, col, intent);

    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
}

const wxString LGridXmlCellRenderer::ProcessXmlValue(const wxString& cellValue)
{
    if (cellValue.IsEmpty())
        return INVALID_INTENT;
    wxXmlDocument doc;
    wxXmlNode * root = XmlHelper::ValidateXmlValue(doc, cellValue);
    if (root == NULL)
        // Show raw data.
        return cellValue;
    wxXmlNode * row = root->GetChildren();
    while (row)
    {
        const wxString pref = row->GetAttribute(XML_ATTR_PREF);
        if (!pref.IsEmpty() && pref != _T("0"))
            return row->GetNodeContent();
        row = row->GetNext();
    }
    /*
     * Don't show raw data if there's no preferred item.
     * The data will still be in the editor and saved.
     */
    return INVALID_INTENT;
}
