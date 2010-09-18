/****************************************************************************
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
** 
** This file is part of a Qt Solutions component.
**
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact Nokia at qt-info@nokia.com.
** 
****************************************************************************/

#ifndef QTIMAGEFILTER_H
#define QTIMAGEFILTER_H
#include <QtGui/QImage>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include "qtmatrix.h"

#if defined(Q_WS_WIN)
#  if !defined(QT_QTIMAGEFILTERS_EXPORT) && !defined(QT_QTIMAGEFILTER_IMPORT)
#    define QT_QTIMAGEFILTERS_EXPORT
#  elif defined(QT_QTIMAGEFILTER_IMPORT)
#    if defined(QT_QTIMAGEFILTERS_EXPORT)
#      undef QT_QTIMAGEFILTERS_EXPORT
#    endif
#    define QT_QTIMAGEFILTERS_EXPORT __declspec(dllimport)
#  elif defined(QT_QTIMAGEFILTERS_EXPORT)
#    undef QT_QTIMAGEFILTERS_EXPORT
#    define QT_QTIMAGEFILTERS_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTIMAGEFILTERS_EXPORT
#endif


class QT_QTIMAGEFILTERS_EXPORT QtImageFilter {
public:
    enum FilterOption {
        FilterChannels = 1,
        FilterBorderPolicy,
        ConvolutionDivisor,
        ConvolutionBias,
        ConvolutionKernelMatrix,
        Radius,
        Center,
        Force,
        UserOption = 0x100
    };

    virtual QVariant option(int filteroption) const;

    virtual bool setOption(int filteroption, const QVariant &value);

    virtual bool supportsOption(int filteroption) const;

    virtual QImage apply(const QImage &img, const QRect& clipRect = QRect() ) const = 0;

    virtual QString name() const = 0;

    virtual QString description() const;

    virtual ~QtImageFilter() {}
};

class QT_QTIMAGEFILTERS_EXPORT QtImageFilterFactory {
public:
    typedef QtImageFilter* (*ImageFilterFactoryFunction)(void);
    static QtImageFilter *createImageFilter(const QString &name);

    static void registerImageFilter(const QString &name, ImageFilterFactoryFunction func);

    static QStringList imageFilterList();

#if !defined(QT_NO_MEMBER_TEMPLATES)
    template<typename T>
    static QtImageFilter *automaticFilterCreator()
    {
        return new T;
    }

    template<typename T>
    static void registerImageFilter(const QString &name)
    {
        registerImageFilter(name, &automaticFilterCreator<T>);
    }
#endif

};

// For compilers with broken template member function support and for those that want to keep
// portability high.
template<typename T>
QtImageFilter *qtAutomaticFilterCreator(const T * /*dummy*/ = 0)  { return new T;}

template<typename T>
void qtRegisterImageFilter(const QString &name, T * /*dummy*/ = 0)
{
    typedef QtImageFilter* (* ConstructPtr)(const T*);
    ConstructPtr cptr = qtAutomaticFilterCreator<T>;
    QtImageFilterFactory::registerImageFilter(name,
                reinterpret_cast<QtImageFilterFactory::ImageFilterFactoryFunction>(cptr));
}

typedef QtMatrix<int> QtConvolutionKernelMatrix;
Q_DECLARE_METATYPE(QtConvolutionKernelMatrix)


#endif // QTIMAGEFILTER_H
