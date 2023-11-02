/*
    Actiona
    Copyright (C) 2005 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact: jmgr@jmgr.info
*/

#include "actiontools/xmlvalidator.hpp"
#include "actiontools/scopeguard.hpp"

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlerror.h>

namespace ActionTools
{
    std::tuple<bool, QString, int, int> XmlValidator::validate(const QByteArray &content)
    {
        xmlInitParser();
        ScopeGuard parserGuard([]{
            xmlCleanupParser();
        });

        std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> schemaDoc(xmlReadMemory(mSchema.constData(), mSchema.size(), nullptr, nullptr, 0), &xmlFreeDoc);
        if(!schemaDoc)
            return {false, {}, 0, 0};

        std::unique_ptr<xmlSchemaParserCtxt, decltype(&xmlSchemaFreeParserCtxt)> parserContext(xmlSchemaNewDocParserCtxt(schemaDoc.get()), &xmlSchemaFreeParserCtxt);
        if(!parserContext)
            return {false, {}, 0, 0};

        std::unique_ptr<xmlSchema, decltype(&xmlSchemaFree)> schema(xmlSchemaParse(parserContext.get()), &xmlSchemaFree);
        if(!schema)
            return {false, {}, 0, 0};

        std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> document(xmlReadMemory(content.constData(), content.size(), nullptr, nullptr, 0), &xmlFreeDoc);
        if(!document)
            return {false, {}, 0, 0};

        std::unique_ptr<xmlSchemaValidCtxt, decltype(&xmlSchemaFreeValidCtxt)> validationContext(xmlSchemaNewValidCtxt(schema.get()), &xmlSchemaFreeValidCtxt);
        if(!validationContext)
            return {false, {}, 0, 0};

        xmlSetStructuredErrorFunc(nullptr, [](void *userData, xmlErrorPtr error){
            auto self = static_cast<XmlValidator *>(userData);
            self->mMessage = QString::fromUtf8(error->message);
            self->mLine = error->line;
            self->mColumn = error->int2;
        });
        ScopeGuard errorFuncGuard([this]{
            xmlSetStructuredErrorFunc(this, nullptr);
        });

        int ret = xmlSchemaValidateDoc(validationContext.get(), document.get());
        if(ret == 0) // Valid
            return {true, {}, 0, 0};
        else if (ret > 0) // Invalid
            return {false, mMessage, mLine, mColumn};
        else // Error
            return {false, {}, 0, 0};
    }
}
