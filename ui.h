#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

#include <QtXml>
#include <QFile>

#include <QDebug>


class UI : public QWidget {
	Q_OBJECT

public:
	UI(QWidget *parent = 0);
	void displayCard(QDomNode card);
	bool checkCard(QDomNode card, QString cardNameStr);
	void cleanAll();


public slots:
	void handleFilterButton();

private:
	QLineEdit *cardNameFilter;
	QPushButton *addButton;
	QPushButton *filterButton;
	QLabel *typeLabel;
	QLineEdit *typeTranslate;
	QLabel *dlcLabel;
	QLineEdit *dlcTranslate;
	QLabel *translateLabel;
	QLineEdit *cardNameTranslate;
	QTextEdit *description;
};
#endif
