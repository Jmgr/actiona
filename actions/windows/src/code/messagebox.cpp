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

#include "messagebox.hpp"
#include "actiontools/code/image.hpp"

#include <QJSValueIterator>
#include <QPushButton>

namespace Code
{
	MessageBox::MessageBox()
		: BaseWindow(),
		mMessageBox(new QMessageBox)
	{
        mMessageBox->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

		setWidget(mMessageBox);

        connect(mMessageBox, &QMessageBox::finished, this, &MessageBox::finished);
	}

    MessageBox::MessageBox(const QJSValue &parameters)
        : MessageBox()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        setupConstructorParameters(parameters);

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("text"))
                mMessageBox->setText(it.value().toString());
            else if(it.name() == QLatin1String("detailedText"))
                mMessageBox->setDetailedText(it.value().toString());
            else if(it.name() == QLatin1String("informativeText"))
                mMessageBox->setInformativeText(it.value().toString());
            else if(it.name() == QLatin1String("buttons"))
                mMessageBox->setStandardButtons(static_cast<QMessageBox::StandardButton>(it.value().toInt()));
            else if(it.name() == QLatin1String("icon"))
                mMessageBox->setIcon(static_cast<QMessageBox::Icon>(it.value().toInt()));
            else if(it.name() == QLatin1String("defaultButton"))
                mMessageBox->setDefaultButton(static_cast<QMessageBox::StandardButton>(it.value().toInt()));
            else if(it.name() == QLatin1String("escapeButton"))
                mMessageBox->setEscapeButton(static_cast<QMessageBox::StandardButton>(it.value().toInt()));
            else if(it.name() == QLatin1String("onClosed"))
                mOnClosed = it.value();
        }
    }

	MessageBox::~MessageBox()
	{
		delete mMessageBox;
	}

    MessageBox *MessageBox::setText(const QString &text)
	{
		mMessageBox->setText(text);

        return this;
	}

    MessageBox *MessageBox::setDetailedText(const QString &detailedText)
	{
		mMessageBox->setDetailedText(detailedText);

        return this;
	}

    MessageBox *MessageBox::setInformativeText(const QString &informativeText)
	{
		mMessageBox->setInformativeText(informativeText);

        return this;
	}

    MessageBox *MessageBox::setButtons(StandardButton buttons)
	{
		mMessageBox->setStandardButtons(static_cast<QMessageBox::StandardButton>(buttons));

        return this;
	}

    MessageBox *MessageBox::setIcon(Icon icon)
	{
		mMessageBox->setIcon(static_cast<QMessageBox::Icon>(icon));

        return this;
	}

    MessageBox *MessageBox::setIconPixmap(const QJSValue &image)
	{
		if(image.isUndefined() || image.isNull())
		{
			mMessageBox->setIconPixmap(QPixmap());

            return this;
		}

		QObject *object = image.toQObject();
		if(auto otherImage = qobject_cast<Image*>(object))
		{
			mMessageBox->setIconPixmap(QPixmap::fromImage(otherImage->image()));
		}
		else
		{
			throwError(QStringLiteral("SetIconPixmapError"), tr("Invalid image"));
            return this;
		}

        return this;
	}

    MessageBox *MessageBox::setDefaultButton(StandardButton button)
	{
		mMessageBox->setDefaultButton(static_cast<QMessageBox::StandardButton>(button));

        return this;
	}

    MessageBox *MessageBox::setEscapeButton(StandardButton button)
	{
		mMessageBox->setEscapeButton(static_cast<QMessageBox::StandardButton>(button));

        return this;
	}

    MessageBox *MessageBox::addCustomButton(StandardButton button, const QString &text)
	{
		QPushButton *addedButton = mMessageBox->addButton(static_cast<QMessageBox::StandardButton>(button));
		if(!addedButton)
		{
			throwError(QStringLiteral("AddCustomButtonError"), tr("Add custom button failed"));
            return this;
		}

		addedButton->setText(text);

        return this;
	}

    MessageBox *MessageBox::show()
	{
		mMessageBox->open();

        return this;
	}

	int MessageBox::showModal()
	{
		return mMessageBox->exec();
	}

    void MessageBox::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<MessageBox*>("const MessageBox *");

        CodeClass::registerClass<MessageBox>(QStringLiteral("MessageBox"), scriptEngine);
    }

	void MessageBox::finished(int result)
	{
        if(!mOnClosed.isUndefined())
            mOnClosed.call(QJSValueList() << result);
	}
}
