/* 
 * File:   XmlHelper.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 15, 2019, 4:13 PM
 */

#include "XmlHelper.h"
#include <wx/sstream.h>

XmlHelper::XmlHelper()
{
}

XmlHelper::~XmlHelper()
{
}

// if doc is declared with local scope, root elements are empty in caller

wxXmlNode* XmlHelper::ValidateXmlValue(wxXmlDocument& doc, const wxString& value)
{
    // Avoid 'XML parsing error' message box.
    wxLogNull lognull;
    wxStringInputStream sis(value);
    bool res = doc.Load(sis);
    if (!res)
    {
        wxASSERT_MSG(res == true, _T("Can't load XML data.\n") + value);
        return NULL;
    }
    wxXmlNode * root = doc.GetRoot();
    if (root->GetName() != XML_ROOT_NAME)
    {
        wxASSERT_MSG(root->GetName() != XML_ROOT_NAME, _T("Bad XML data.\n") + value);
        return NULL;
    }
    // Check all rows and attributes
    wxXmlNode * row = root->GetChildren();
    while (row)
    {
        if (row->GetName() != XML_ROW_NAME
                || !row->HasAttribute(XML_ATTR_TYPE)
                || !row->HasAttribute(XML_ATTR_PREF)
                || !row->HasAttribute(XML_ATTR_NOTES)
                || row->GetNodeContent().IsEmpty())
        {
            wxASSERT_MSG(root->GetName() != XML_ROOT_NAME, _T("Unexpected XML data.\n") + value);
            return NULL;
        }
        row = row->GetNext();
    }
    return root;
}

wxString XmlHelper::GetIntent(const wxString& xmlData)
{
    if (xmlData.IsEmpty())
        return wxEmptyString;
    wxXmlDocument doc;
    wxXmlNode * root = ValidateXmlValue(doc, xmlData);
    if (root == NULL)
        return wxEmptyString;
    wxXmlNode * row = root->GetChildren();
    while (row)
    {
        const wxString pref = row->GetAttribute(XML_ATTR_PREF);
        if (!pref.IsEmpty() && pref != _T("0"))
            return row->GetNodeContent();
        row = row->GetNext();
    }
    return wxEmptyString;
}
