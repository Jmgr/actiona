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

	Contact : jmgr@jmgr.info
*/

#include "messagebox.h"
#include "code/image.h"

#include <QScriptValueIterator>
#include <QPushButton>

namespace Code
{
	QScriptValue MessageBox::constructor(QScriptContext *context, QScriptEngine *engine)
	{
        auto messageBox = new MessageBox;
		messageBox->setupConstructorParameters(context, engine, context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("text"))
				messageBox->mMessageBox->setText(it.value().toString());
			else if(it.name() == QLatin1String("detailedText"))
				messageBox->mMessageBox->setDetailedText(it.value().toString());
			else if(it.name() == QLatin1String("informativeText"))
				messageBox->mMessageBox->setInformativeText(it.value().toString());
			else if(it.name() == QLatin1String("buttons"))
				messageBox->mMessageBox->setStandardButtons(static_cast<QMessageBox::StandardButton>(it.value().toInt32()));
			else if(it.name() == QLatin1String("icon"))
				messageBox->mMessageBox->setIcon(static_cast<QMessageBox::Icon>(it.value().toInt32()));
			else if(it.name() == QLatin1String("defaultButton"))
				messageBox->mMessageBox->setDefaultButton(static_cast<QMessageBox::StandardButton>(it.value().toInt32()));
			else if(it.name() == QLatin1String("escapeButton"))
				messageBox->mMessageBox->setEscapeButton(static_cast<QMessageBox::StandardButton>(it.value().toInt32()));
			else if(it.name() == QLatin1String("onClosed"))
				messageBox->mOnClosed = it.value();
		}

		return CodeClass::constructor(messageBox, context, engine);
	}

	MessageBox::MessageBox()
		: BaseWindow(),
		mMessageBox(new QMessageBox)
	{
        mMessageBox->setWindowFlags(mMessageBox->windowFlags() | Qt::WindowContextHelpButtonHint);

		setWidget(mMessageBox);

        connect(mMessageBox, &QMessageBox::finished, this, &MessageBox::finished);
	}

	MessageBox::~MessageBox()
	{
		delete mMessageBox;
	}

	QScriptValue MessageBox::setText(const QString &text)
	{
		mMessageBox->setText(text);

		return thisObject();
	}

	QScriptValue MessageBox::setDetailedText(const QString &detailedText)
	{
		mMessageBox->setDetailedText(detailedText);

		return thisObject();
	}

	QScriptValue MessageBox::setInformativeText(const QString &informativeText)
	{
		mMessageBox->setInformativeText(informativeText);

		return thisObject();
	}

	QScriptValue MessageBox::setButtons(StandardButton buttons)
	{
		mMessageBox->setStandardButtons(static_cast<QMessageBox::StandardButton>(buttons));

		return thisObject();
	}

	QScriptValue MessageBox::setIcon(Icon icon)
	{
		mMessageBox->setIcon(static_cast<QMessageBox::Icon>(icon));

		return thisObject();
	}

	QScriptValue MessageBox::setIconPixmap(const QScriptValue &image)
	{
		if(image.isUndefined() || image.isNull())
		{
			mMessageBox->setIconPixmap(QPixmap());

			return thisObject();
		}

		QObject *object = image.toQObject();
		if(auto otherImage = qobject_cast<Image*>(object))
		{
			mMessageBox->setIconPixmap(QPixmap::fromImage(otherImage->image()));
		}
		else
		{
			throwError(QStringLiteral("SetIconPixmapError"), tr("Invalid image"));
			return thisObject();
		}

		return thisObject();
	}

	QScriptValue MessageBox::setDefaultButton(StandardButton button)
	{
		mMessageBox->setDefaultButton(static_cast<QMessageBox::StandardButton>(button));

		return thisObject();
	}

	QScriptValue MessageBox::setEscapeButton(StandardButton button)
	{
		mMessageBox->setEscapeButton(static_cast<QMessageBox::StandardButton>(button));

		return thisObject();
	}

	QScriptValue MessageBox::addCustomButton(StandardButton button, const QString &text)
	{
		QPushButton *addedButton = mMessageBox->addButton(static_cast<QMessageBox::StandardButton>(button));
		if(!addedButton)
		{
			throwError(QStringLiteral("AddCustomButtonError"), tr("Add custom button failed"));
			return thisObject();
		}

		addedButton->setText(text);

		return thisObject();
	}

	QScriptValue MessageBox::show()
	{
		mMessageBox->open();

		return thisObject();
	}

	int MessageBox::showModal()
	{
		return mMessageBox->exec();
	}

	void MessageBox::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(thisObject(), QScriptValueList() << result);
	}
}
