/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QtGlobal> // for Q_CC_MINGW
#if !defined (Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
#define _WIN32_DCOM

#include "qwmihelper_win_p.h"

#include <QDebug>
#include <objbase.h>
#include <wbemidl.h>
#include <oleauto.h>
#include <QStringList>
#include <QUuid>
#include <comutil.h>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(WMIHelper, wmihelper)

WMIHelper::WMIHelper(QObject * parent)
        : QObject(parent),initialized(0)
{
   m_conditional = QString();
}

WMIHelper::~WMIHelper()
{
    CoUninitialize();
}

WMIHelper *WMIHelper::instance()
{
    return wmihelper();
}

QVariant WMIHelper::getWMIData()
{
//    qDebug() << Q_FUNC_INFO << m_wmiNamespace << m_className << m_classProperties;

   if (!m_wmiNamespace.isEmpty() && !m_className.isEmpty() && !m_classProperties.isEmpty()) {
      return getWMIData(m_wmiNamespace, m_className, m_classProperties);
   }
   return QVariant();
}

void WMIHelper::initializeWMI(const QString &wmiNamespace)
{
    if (initialized) {
        wbemLocator = 0;
    }
    HRESULT hres;
    wbemLocator = 0;

    QUuid wbemLocatorClsid = "4590f811-1d3a-11d0-891f-00aa004b2e24";
    QUuid wbemLocatorIid = "dc12a687-737f-11cf-884d-00aa004b2e24";

    hres = CoCreateInstance(wbemLocatorClsid,0,CLSCTX_INPROC_SERVER,
                            wbemLocatorIid, (LPVOID *) &wbemLocator);

    if (hres == CO_E_NOTINITIALIZED) { // COM was not initialized
        CoInitializeEx(0, COINIT_MULTITHREADED);
        hres = CoCreateInstance(wbemLocatorClsid,0,CLSCTX_INPROC_SERVER,
                                wbemLocatorIid, (LPVOID *) &wbemLocator);
    }

    if (hres != S_OK) {
       qWarning() << "Failed to create IWbemLocator object." << hres;
        return ;
    }
    wbemServices = 0;
     const OLECHAR *ole = reinterpret_cast<const OLECHAR *>(wmiNamespace.utf16());
    hres = wbemLocator->ConnectServer(::SysAllocString(ole),0,0,0,0,0,0,&wbemServices);

    if (hres != WBEM_S_NO_ERROR){
        qWarning() << "Could not connect";
        return ;
    }

    hres = CoSetProxyBlanket( wbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, 0,
                              RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, 0, EOAC_NONE );

    if (hres != S_OK) {
       qWarning() << "Could not set proxy blanket" << hres;
        return ;
    }
    initialized = true;
}

QVariant WMIHelper::getWMIData(const QString &wmiNamespace, const QString &className, const QStringList &classProperty)
{
    initializeWMI(wmiNamespace);
    HRESULT hres;
    QVariant returnVariant;

      wbemEnumerator = 0;

    if (!m_conditional.isEmpty() && !m_conditional.startsWith(QLatin1Char(' ')))
        m_conditional.prepend(QLatin1Char(' '));

    const QString aString = QStringLiteral("SELECT * FROM ") + className + m_conditional;
    BSTR bstrQuery;
    const OLECHAR *oleStr = reinterpret_cast<const OLECHAR *>(aString.utf16());

    bstrQuery = ::SysAllocString(oleStr);

    hres = wbemServices->ExecQuery(L"WQL", bstrQuery,
            WBEM_FLAG_BIDIRECTIONAL | WBEM_FLAG_RETURN_IMMEDIATELY,0,&wbemEnumerator);

    if (hres != WBEM_S_NO_ERROR){
        qWarning() << "WMI Query failed.";
        wbemLocator->Release();
        wbemEnumerator->Release();
        return returnVariant;
    }

    ::SysFreeString(bstrQuery);

    wbemCLassObject = 0;
    ULONG result = 0;

    wmiVariantList.clear();
    while (wbemEnumerator) {
        HRESULT hr = wbemEnumerator->Next(WBEM_INFINITE, 1,&wbemCLassObject, &result);
        if (0 == result){
            break;
        }

        foreach (const QString property, classProperty) {
            VARIANT msVariant;
            CIMTYPE variantType;
            hr = wbemCLassObject->Get(reinterpret_cast<const wchar_t *>(property.utf16()), 0, &msVariant, &variantType, 0);
            returnVariant = msVariantToQVariant(msVariant, variantType);
            wmiVariantList << returnVariant;

            VariantClear(&msVariant);

        }

        wbemCLassObject->Release();
    }

    if(wbemEnumerator)
        wbemEnumerator->Release();
    wbemLocator->Release();
    wbemServices->Release();
    return returnVariant;
}

QVariant WMIHelper::msVariantToQVariant(VARIANT msVariant, CIMTYPE variantType)
{
    QVariant returnVariant;
    switch (variantType) {
    case CIM_STRING:
    case CIM_CHAR16:
        {
            QString str((QChar*)msVariant.bstrVal, int(wcslen(msVariant.bstrVal)));
            QVariant vs(str);
            returnVariant = vs;
        }
        break;
    case CIM_BOOLEAN:
        {
            QVariant vb(msVariant.boolVal);
            returnVariant = vb;
        }
        break;
    case CIM_UINT8:
        {
            QVariant vb(msVariant.uintVal);
            returnVariant = vb;
        }
        break;
    case CIM_UINT16:
        {
            QVariant vb(msVariant.uintVal);
            returnVariant = vb;
        }
    case CIM_UINT32:
        {
            QVariant vb(msVariant.uintVal);
            returnVariant = vb;
        }
        break;
    case CIM_UINT64:
        {
            QVariant vb(msVariant.uintVal);
            returnVariant = vb;
        }
        break;
    };
    VariantClear(&msVariant);
    return returnVariant;
}

  void WMIHelper::setWmiNamespace(const QString &wmiNamespace)
{
   m_wmiNamespace = wmiNamespace;
}

   void WMIHelper::setClassName(const QString &className)
{
   m_className = className;
}

void WMIHelper::setClassProperty(const QStringList &classProperties)
{
   m_classProperties = classProperties;
}

void WMIHelper::setConditional(const QString &conditional)
{
   m_conditional = conditional;
}

void WMIHelper::setupNotfication(const QString &wmiNamespace,const QString &className, const QStringList &/*classProperties*/)
{
    initializeWMI(wmiNamespace);
    HRESULT hres;

    IUnsecuredApartment* pUnsecApp = NULL;

    QUuid clsidUnsecuredApartment = "49bd2028-1523-11d1-ad79-00c04fd8fdff";
    QUuid iidUnsecuredApartment = "1cfaba8c-1523-11d1-ad79-00c04fd8fdff";

     hres = CoCreateInstance(clsidUnsecuredApartment, NULL,
         CLSCTX_LOCAL_SERVER,iidUnsecuredApartment,
         (void**)&pUnsecApp);

     if (hres == CO_E_NOTINITIALIZED) { // COM was not initialized
         qDebug() << "not initialized";
         CoInitializeEx(0, COINIT_MULTITHREADED);
         hres = CoCreateInstance(clsidUnsecuredApartment,0,CLSCTX_INPROC_SERVER,
                                 iidUnsecuredApartment, (LPVOID *) &pUnsecApp);
     }

     if (hres != S_OK) {
        qDebug() << "Failed to create IWbemLocator object." << hres;
         return ;
     }

     EventSink* pSink = new EventSink;

     pSink->AddRef();

     IUnknown* pStubUnk = NULL;
     pUnsecApp->CreateObjectStub(pSink, &pStubUnk);

     IWbemObjectSink* pStubSink = NULL;
     QUuid iidWbemObjectSink = "7c857801-7381-11cf-884d-00aa004b2e24";

     pStubUnk->QueryInterface(iidWbemObjectSink,(void **) &pStubSink);

     QString aString = className;//"SELECT * FROM __InstanceOperationEvent WITHIN 1 WHERE (TargetInstance ISA 'Win32_LogicalDisk') AND (TargetInstance.DriveType = 5 OR TargetInstance.DriveType = 2)";

     BSTR bstrQuery;

     const OLECHAR *oleStr = reinterpret_cast<const OLECHAR *>(aString.utf16());

     bstrQuery = ::SysAllocString(oleStr);

     hres = wbemServices->ExecNotificationQueryAsync(
         L"WQL",
         bstrQuery,
         WBEM_FLAG_SEND_STATUS,
         NULL,
         pStubSink);

     if (FAILED(hres)) {
         printf("ExecNotificationQueryAsync failed "
             "with = 0x%X\n", hres);
         wbemLocator->Release();
         wbemEnumerator->Release();
         pUnsecApp->Release();
         pStubUnk->Release();
         pSink->Release();
         pStubSink->Release();
         CoUninitialize();
         return;
     }
}

void WMIHelper::emitNotificationArrived()
{
    emit wminotificationArrived();
}

ULONG EventSink::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}

ULONG EventSink::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRef);
    if (lRef == 0)
        delete this;
    return lRef;
}

HRESULT EventSink::QueryInterface(REFIID riid, void** ppv)
{
    QUuid iidWbemObjectSink = "7c857801-7381-11cf-884d-00aa004b2e24";

    if (riid == IID_IUnknown || riid == iidWbemObjectSink){
        *ppv = (IWbemObjectSink *) this;
        AddRef();
        return WBEM_S_NO_ERROR;
    }
    else return E_NOINTERFACE;
}


HRESULT EventSink::Indicate(long lObjectCount,
    IWbemClassObject ** /*apObjArray*/)
{
    for (int i = 0; i < lObjectCount; i++){
        WMIHelper::instance()->emitNotificationArrived();

//        _variant_t msVariant;
//        CIMTYPE variantType;
//        QVariant returnVariant;
//        IWbemClassObject *obj = apObjArray[0];

//        HRESULT hr = obj->Get(L"TargetInstance", 0, &msVariant, 0, 0);
//        if (hr == S_OK) {
//            IUnknown *str = msVariant;
//            VariantClear(&msVariant);

//            IWbemClassObject *obj;
//            hr = str->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void **>(&obj));
//            if (hr == S_OK) {
//                _variant_t var2;
//                hr = obj->Get(L"__Class", 0, &var2, 0, 0);
//                _bstr_t classname = var2;
//                VariantClear(&var2);
//                if (classname == _bstr_t(L"Win32_LogicalDisk")) {
//                    _variant_t var3;
//       //             qDebug() << obj->Availability;
//                    hr = obj->Get(L"Availability", 0, &var3, &variantType, 0);
//                    if (hr == S_OK) {
//                        //uint av = var3;
//                        //returnVariant = WMIHelper::msVariantToQVariant(msVariant, variantType);

//                        printf("variant type: 0x%X\n", variantType);

// //                       int value = var3.intVal;

//                       qDebug() <<"Availability" << var3.uintVal << var3.vt << var3.uiVal << var3.iVal;
//                        //var3.uintVal;//<<returnVariant << returnVariant.toUInt();
//                        VariantClear(&var3);
//                    } else {
//                        qDebug() << hr << GetLastError();
//                        printf("Error foo. hr = 0x%X\n", hr);

//                    }
//                } else {
//                    qDebug() << hr << GetLastError();
//                    printf("Error 2. hr = 0x%X\n", hr);

//                }
//            }
//        } else {
//            qDebug() << hr << GetLastError();
//            printf("Error. hResult = 0x%X\n", hr);
//        }
    }

    return WBEM_S_NO_ERROR;
}

HRESULT EventSink::SetStatus(
           LONG lFlags,
           HRESULT hResult,
           BSTR /*strParam*/,
           IWbemClassObject __RPC_FAR * /*pObjParam*/)
{
    if (lFlags == WBEM_STATUS_COMPLETE) {
        printf("Call complete. hResult = 0x%X\n", hResult);
    } else if (lFlags == WBEM_STATUS_PROGRESS) {
        printf("Call in progress.\n");
    }

    return WBEM_S_NO_ERROR;
}    // end of EventSink.cpp

#include "moc_qwmihelper_win_p.cpp"

QT_END_NAMESPACE
#endif
