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

#pragma once

// Based on https://gist.github.com/mistic100/c3b7f3eabc65309687153fe3e0a9a720

#include <QComboBox>

class QStandardItemModel;
class QStandardItem;

class FlagsComboBox : public QComboBox
{
    Q_OBJECT

public:
    FlagsComboBox(QWidget *parent = nullptr);
    ~FlagsComboBox() override;

    void setAllCheckedText(const QString &text);
    void setNoneCheckedText(const QString &text);
    void setFlags(unsigned int flags);
    void addFlag(const QString &label, unsigned int flag, bool checked = false);
    unsigned int flags() const;
    bool hasFlag(unsigned int flag) const;

signals:
    void flagsChanged(unsigned int flags);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Qt::CheckState globalCheckState();
    void updateText();
    QStandardItemModel *standardItemModel();
    const QStandardItemModel *standardItemModel() const;

    QString m_noneCheckedText;
    QString m_allCheckedText;
};
