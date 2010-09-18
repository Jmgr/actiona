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

#include "qtimagefilter.h"
#include "convolutionfilter.h"
#include "gaussfilter.h"
#include "punchfilter.h"
#include <QtCore/QMap>
#include <QtCore/QByteArray>

QtImageFilter *createGaussianBlurFilter(void)
{
    return reinterpret_cast<QtImageFilter*>(new GaussBlurFilter());
}

QtImageFilter *createDefocusFilter()
{
static int kernelmatrix[ 9 ] =
  { 1,  1, 1,
    1, -7, 1,
    1,  1, 1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("Defocus"));
    filter->setDescription(QObject::tr("Blurs the image", "ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 3, 3), ConvolutionFilter::RGB, ConvolutionFilter::Extend);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createHighlightFilter()
{
static int kernelmatrix[ 25 ] =
  { -1, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 2 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("Hightlight"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 5, 5), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createSharpenFilter()
{
static int kernelmatrix[ 25 ] =
  { -1, -1, -1, -1, -1,
    -1,  2,  2,  2, -1,
    -1,  2,  8,  2, -1,
    -1,  2,  2,  2, -1,
    -1, -1, -1, -1, -1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("Sharpen"));
    filter->setDescription(QObject::tr("Sharpens the image","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 5, 5), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 8, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createSharpenMoreFilter()
{
static int kernelmatrix[ 9 ] =
  { -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("SharpenMore"));
    filter->setDescription(QObject::tr("Sharpens the image more","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 3, 3), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createSharpenEvenMoreFilter()
{
static int kernelmatrix[ 9 ] =
  { -1, -2, -1,
    -2, 13, -2,
    -1, -2, -1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("SharpenEvenMore"));
    filter->setDescription(QObject::tr("Sharpens the image even more","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 3, 3), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createBigEdgeFilter()
{
static int kernelmatrix[ 25 ] =
  { -2, -2, -2, -2, -2,
    -2, -3, -3, -3, -2,
    -2, -3, 53, -3, -2,
    -2, -3, -3, -3, -2,
    -2, -2, -2, -2, -2 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("BigEdge"));
    filter->setDescription(QObject::tr("Creates big edges","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 5, 5), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createEmbossFilter()
{
static int kernelmatrix[ 9 ] =
  { -1, -1,  0,
    -1,  0,  1,
     0,  1,  1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("Emboss"));
    filter->setDescription(QObject::tr("Creates an emboss effect on the image, resulting in a greyish image","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 3, 3), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 128);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createEmbossColorFilter()
{
static int kernelmatrix[9] =
{   -1, -1,  0,
    -1,  1,  1,
     0,  1,  1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("EmbossColor"));
    filter->setDescription(QObject::tr("Creates an emboss effect on the image, while keeping most of the colours","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 3, 3), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createEdgeDetectFilter()
{
static int kernelmatrix[ 9 ] =
  { -5, -2, -5,
    -5, 39, -5,
    -5, -5, -5 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("EdgeDetect"));
    filter->setDescription(QObject::tr("Creates a filter that enables you to detect edges easily.","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 3, 3), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 50);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createNegativeFilter()
{
static int kernelmatrix[ 1 ] =
  { -1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("Negative"));
    filter->setDescription(QObject::tr("Negates color channel(s).","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 1, 1), ConvolutionFilter::RGB, ConvolutionFilter::Extend, 1, 255);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createRemoveChannelFilter()
{
static int kernelmatrix[ 1 ] =
  { 0 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("RemoveChannel"));
    filter->setDescription(QObject::tr("Removes channel(s). I.e sets the value of each channel it is processing to 0.","ConvolutionFilter"));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 1, 1), ConvolutionFilter::RGBA, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createConvolutionFilter()
{
static int kernelmatrix[ 1 ] =
  { 1 };
    ConvolutionFilter *filter = new ConvolutionFilter();
    filter->setName(QLatin1String("ConvolutionFilter"));
    filter->setDescription(QObject::tr("Generic convolutionfilter."));
    filter->addKernel(QtConvolutionKernelMatrix(kernelmatrix, 1, 1), ConvolutionFilter::RGBA, ConvolutionFilter::Extend, 1, 0);
    return reinterpret_cast<QtImageFilter*>(filter);
}

QtImageFilter *createPunchFilter()
{
    return reinterpret_cast<QtImageFilter*>(new PunchFilter());
}

//typedef QtImageFilter*(*FnFactory)(void);

static QMap<QByteArray, QtImageFilterFactory::ImageFilterFactoryFunction> g_availableFilters;

static void registerDefaultImageFilters()
{
    g_availableFilters.insert("GaussianBlur",        &createGaussianBlurFilter);
    g_availableFilters.insert("Defocus",             &createDefocusFilter);
    g_availableFilters.insert("Highlight",           &createHighlightFilter);
    g_availableFilters.insert("Sharpen",             &createSharpenFilter);
    g_availableFilters.insert("SharpenMore",         &createSharpenMoreFilter);
    g_availableFilters.insert("SharpenEvenMore",     &createSharpenEvenMoreFilter);
    g_availableFilters.insert("BigEdge",             &createBigEdgeFilter);
    g_availableFilters.insert("Emboss",              &createEmbossFilter);
    g_availableFilters.insert("EmbossColor",         &createEmbossColorFilter);
    g_availableFilters.insert("EdgeDetect",          &createEdgeDetectFilter);
    g_availableFilters.insert("Negative",            &createNegativeFilter);
    g_availableFilters.insert("RemoveChannel",       &createRemoveChannelFilter);
    g_availableFilters.insert("Punch",               &createPunchFilter);
    g_availableFilters.insert("ConvolutionFilter",   &createConvolutionFilter);
}
/*!
    \class QtImageFilterFactory

    \brief The QtImageFilterFactory class is used to create and manage
    QtImageFilter objects.

    QtImageFilterFactory is part of the QtImageFilters solution, and is
    used to create instances of the registered image filters. The
    framework supports many popular image filters by default, among
    these are gaussian blurring, embossing (see image below),
    sharpening, defocus, punch/pinch and convolution filters.

    \image lena-edge.jpg

    The documentation for the createImageFilter() function provides a
    complete list of all the supported image filters.

    Developers can extend the framework with their own custom image
    filters by deriving from the QtImageFilter class. A list of the
    currently registered filters, including custom filters, can be
    retrieved using the imageFilterList() function.

    When creating a custom image filter, it must be registered using
    the registerImageFilter() function before instances can be created
    using the createImageFilter() function. For example:

    \quotefromfile ../examples/imagetool/imagetoolwidget.cpp
    \skipto qtRegisterImageFilter
    \printline  qtRegisterImageFilter
    \skipto QStringList
    \printuntil m_imageFilters

    The registerImageFilter() function requires an
    ImageFilterFactoryFunction pointer and the name of the image
    filter. ImageFilterFactoryFunction is a type definition for a
    function that can create and return a QtImageFilter object of a
    given filter type.

    Due to compiler limitations, the template version of the
    registerImageFilter() function is not available on all
    platforms (the related qtRegisterImageFilter() function is
    provided as a work-around for the MSVC 6 compiler).

    \sa QtImageFilter
*/

/*!
    \typedef QtImageFilterFactory::ImageFilterFactoryFunction

    This is a type definition for a pointer to a function with the
    following signature:

    \code
        QtImageFilter *myFilterFactoryFunc(void);
    \endcode

    \sa registerImageFilter()
*/
/*!
    Creates an instance of the image filter specified by the given \a
    name. Returns true if the image filter was successfully
    initialized; otherwise false.

    The given filter must be registered using the
    registerImageFilter() function. The following image filters are
    registered by default:

    \table
    \header
        \o Filter Name
        \o Filter Operation
        \o Supported Options
    \row
        \o ConvolutionFilter
        \o Creates a custom convolutionfilter
        \o ConvolutionKernelMatrix, FilterChannels, FilterBorderPolicy, ConvolutionDivisor, ConvolutionBias
    \row
        \o GaussianBlur
        \o Creates a gaussian blur filter.
        \o Radius, FilterChannels, FilterBorderPolicy
    \row
        \o Defocus
        \o Blurs the image.
        \o FilterChannels, FilterBorderPolicy
    \row
        \o Highlight
        \o
        \o FilterChannels, FilterBorderPolicy
    \row
        \o Sharpen
        \o Sharpens the image
        \o FilterChannels, FilterBorderPolicy
    \row
        \o SharpenMore
        \o Sharpens the image
        \o FilterChannels, FilterBorderPolicy
    \row
        \o SharpenEvenMore
        \o Sharpens the image
        \o FilterChannels, FilterBorderPolicy
    \row
        \o EdgeDetect
        \o Edge detection filter
        \o
    \row
        \o BigEdge
        \o Edge detection filter (gives thicker edges)
        \o FilterChannels, FilterBorderPolicy
    \row
        \o Emboss
        \o Embosses the image (does not preserve color information)
        \o FilterChannels, FilterBorderPolicy
    \row
        \o EmbossColor
        \o Embosses the image (tries to preserve color information)
        \o FilterChannels, FilterBorderPolicy
    \row
        \o Negative
        \o Negates a color channel.
        \o FilterChannels, FilterBorderPolicy
    \row
        \o RemoveChannel
        \o Removes a color/alpha channel
        \o FilterChannels, FilterBorderPolicy
    \row
        \o Punch
        \o Distorts an image using a pinch/punch effect
        \o Center, Radius, Force. Force should be in the interval [-1.0, 1.0]
    \endtable

    \sa registerImageFilter()
*/
QtImageFilter *QtImageFilterFactory::createImageFilter(const QString &name)
{
    if (g_availableFilters.isEmpty()) {
        registerDefaultImageFilters();
    }

    ImageFilterFactoryFunction fnFactory = g_availableFilters.value(name.toAscii());
    return fnFactory ? fnFactory() : 0;
}

/*!
    Returns a list of the currently registered image filters.

    \sa registerImageFilter()
*/
QStringList QtImageFilterFactory::imageFilterList()
{
    QStringList ss;
    if (g_availableFilters.isEmpty()) {
        registerDefaultImageFilters();
    }

    for (QMap<QByteArray, ImageFilterFactoryFunction>::iterator it = g_availableFilters.begin(); it != g_availableFilters.end(); ++it) {
        ss.append(it.key());
    }
    return ss;
}

/*!
    \fn void QtImageFilterFactory::registerImageFilter(const QString &name, ImageFilterFactoryFunction function)

    Registers an image filter factory function, where the given \a
    name is associated with the specified factory \a function
    returning a image filter object of the specified type.

    \sa imageFilterList(), createImageFilter()
*/
void QtImageFilterFactory::registerImageFilter(const QString &name, ImageFilterFactoryFunction func)
{
    if (g_availableFilters.isEmpty()) {
        registerDefaultImageFilters();
    }
    g_availableFilters.insert(name.toAscii(), func);
}

/*!
    \fn QtImageFilter *QtImageFilterFactory::automaticFilterCreator()
    \internal

    This is a template function that creates a factory function for
    creating a QtImageFilter object of type \c{T}. For example:

    This function is also used as a helper function for the
    registerImageFilter(const QString &name) function.

    \warning Due to compiler limitations, this function is not
    available on all platforms. In particular it is not available with
    MSVC 6; use qtAutomaticFilterCreator() instead to support that
    compiler version.

    \sa registerImageFilter()
*/

/*!
    \fn QtImageFilter *qtAutomaticFilterCreator(const T* dummy = 0)
    \relates QtImageFilterFactory
    \internal

    This function is equivalent to
    QtImageFilterFactory::automaticFilterCreator()

    It is provided as a work-around for the MSVC 6 compiler, which
    doesn't support member template functions. There is no need to
    pass any value for the \a dummy parameter; it is only there
    because of an MSVC 6 limitation.

    \sa QtImageFilterFactory::automaticFilterCreator()
*/

/*!
    \fn void QtImageFilterFactory::registerImageFilter(const QString &name)

    This is a template function, registering the image filter with the
    given \a name together with a factory function that returns a new
    instance of an image filter of type \c{T}. For example:

    \code
        registerImageFilter<MirrorFilter>(QLatin1String("MirrorFilter"));
    \endcode

    \warning Due to compiler limitations, this function is not
    available on all platforms. In particular it is not available with
    MSVC 6; use qtRegisterImageFilter() instead to support that
    compiler version.

    \sa createImageFilter()
*/

/*!
    \fn qtRegisterImageFilter(const QString &name, T* dummy )
    \relates QtImageFilterFactory

    This function is equivalent to the template version of the
    QtImageFilterFactory::registerImageFilter() function.It registers
    the image filter with the given \a name together with a factory
    function that returns a new instance of the image filter.

    It is provided as a work-around for the MSVC 6 compiler, which
    doesn't support member template functions. Note that there is no
    need to pass any value for the \a dummy parameter; it is only
    there because of an MSVC 6 limitation.

    \sa QtImageFilterFactory::registerImageFilter()
*/

