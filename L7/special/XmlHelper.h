/* 
 * File:   XmlHelper.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 15, 2019, 4:13 PM
 */

#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <wx/wx.h>
#include <wx/xml/xml.h>

#define XML_ROOT_NAME _T("TABLE")
#define XML_ROW_NAME _T("ROW")
#define XML_ATTR_TYPE _T("Type")
#define XML_ATTR_PREF _T("Preferred")
#define XML_ATTR_NOTES _T("Notes")
#define EMPTY_DOC _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<TABLE/>")

/**
 * Application helper to quickly get the Intent value.
 * @param doc
 * @param value
 * @return 
 */
class XmlHelper
{
public:
    
    /**
     * Parses value which is XML data and returns the XML root node, while
     * loading the XML data in doc.
     * If value does not have the expected tags defined above, returns NULL.
     * @param doc
     * @param value
     * @return 
     */
    static wxXmlNode * ValidateXmlValue(wxXmlDocument& doc, const wxString& value);
    /**
     * Returns a valid Intent value or an empty string.
     * @param xmlData
     * @return 
     */
    static wxString GetIntent(const wxString& xmlData);
private:
    XmlHelper();
    virtual ~XmlHelper();
};

#endif /* XMLHELPER_H */

