#include <QVBoxLayout>
#include "ui.h"

UI::UI(QWidget *parent) : QWidget(parent) {
	cardNameFilter = new QLineEdit;
	cardNameFilter->setPlaceholderText(tr("Enter the card name"));
	addButton = new QPushButton(tr("add"));
	filterButton = new QPushButton(tr("search"));

	typeLabel = new QLabel(tr("type:"));
	typeTranslate = new QLineEdit;
	typeTranslate->setPlaceholderText(tr("Enter the type name"));
	dlcLabel = new QLabel(tr("dlc:"));
	dlcTranslate = new QLineEdit;
	dlcTranslate->setPlaceholderText(tr("Enter the dlc name"));

	translateLabel = new QLabel(tr("translation:"));
	cardNameTranslate = new QLineEdit;
	cardNameTranslate->setPlaceholderText(tr("Enter the Translation"));

	description = new QTextEdit;

	QHBoxLayout *filterLayout = new QHBoxLayout;
	filterLayout->addWidget(cardNameFilter);
	filterLayout->addWidget(addButton);
	filterLayout->addWidget(filterButton);

	QHBoxLayout *attributeLayout = new QHBoxLayout;
	attributeLayout->addWidget(typeLabel);
	attributeLayout->addWidget(typeTranslate);
	attributeLayout->addWidget(dlcLabel);
	attributeLayout->addWidget(dlcTranslate);

	QHBoxLayout *cardTranslateLayout = new QHBoxLayout;
	cardTranslateLayout->addWidget(translateLabel);
	cardTranslateLayout->addWidget(cardNameTranslate);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(filterLayout);
	mainLayout->addLayout(attributeLayout);
	mainLayout->addLayout(cardTranslateLayout);
	mainLayout->addWidget(description);
	setLayout(mainLayout);

	setWindowTitle(tr("Talisman digital edition translate"));
	connect(filterButton, SIGNAL(pressed()), this, SLOT(handleFilterButton()));
	connect(addButton, SIGNAL(pressed()), this, SLOT(handleAddButton()));
	connect(cardNameFilter, SIGNAL(returnPressed()), this, SLOT(handleFilterButton()));
}

void UI::handleAddButton()
{
	QDomDocument doc;
	QFile translateFile( "chinese.xml" );
	if (!translateFile.open(QIODevice::ReadWrite | QIODevice::Text))
		return;

	QString errorStr;
	int errorLine, errorCol;
	doc.setContent(&translateFile, true, &errorStr, &errorLine, &errorCol);
	translateFile.close();
	//rootNode stand for <cards> node in chinese.xml
	QDomNode rootNode = doc.firstChild();

	QDomElement cardTag = doc.createElement(QString("card"));

	QDomElement nameTag = doc.createElement(QString("name"));
	nameTag.setAttribute("dlc", dlcTranslate->text());
	nameTag.setAttribute("type", typeTranslate->text());
	QDomText nameText = doc.createTextNode(QString(this->cardNameFilter->text()).toUpper());

	QDomElement translateTag = doc.createElement(QString("translate"));
	QDomText translateText = doc.createTextNode(this->cardNameTranslate->text());

	QDomElement descriptionTag = doc.createElement(QString("description"));
	QDomText descriptionText = doc.createTextNode(this->description->toPlainText());

	descriptionTag.appendChild(descriptionText);
	translateTag.appendChild(translateText);
	nameTag.appendChild(nameText);
	cardTag.appendChild(nameTag);
	cardTag.appendChild(translateTag);
	cardTag.appendChild(descriptionTag);
	rootNode.appendChild(cardTag);
	doc.appendChild(rootNode);

	//here we open it again, for write purpose
	if (!translateFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
		return;
	QTextStream out(&translateFile);
	out << doc.toString();
	translateFile.close();

}

void UI::handleFilterButton()
{
	//qDebug() << this->cardNameFilter->text();
	QDomDocument doc;
	QFile translateFile( "chinese.xml" );
	QString errorStr;
	int errorLine, errorCol;
	doc.setContent(&translateFile, true, &errorStr, &errorLine, &errorCol);
	translateFile.close();
	/*if( !inFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		qDebug( "Failed to open file for reading." );
	}*/

	//rootNode stand for <cards> node in chinese.xml
	QDomNode rootNode = doc.firstChild();
	QDomNodeList cardsList=rootNode.childNodes();
	for(int i = 0; i < cardsList.count(); i++)
	{
		QDomNode card=cardsList.at(i);
		if ( checkCard(card, this->cardNameFilter->text()) )
		{
			displayCard(card);
			return;
		}
		card = card.nextSibling();
	}
	cleanAll();
	this->description->setText( this->cardNameFilter->text() + " Not found." );
}

void UI::cleanAll()
{
	this->typeTranslate->setText("");
	this->dlcTranslate->setText("");
	this->cardNameTranslate->setText("");
	this->description->setText("");
}

void UI::displayCard(QDomNode card)
{
	//QDomNode root = doc.firstChild();

	if(card.hasChildNodes())
	{
		QDomNodeList list=card.childNodes();
		for(int i = 0; i < list.count(); i++)
		{
			QDomNode node=list.at(i);

			if( node.isElement() )
			{
				QDomElement element = node.toElement();
				/*qDebug() << "ELEMENT" << element.tagName();
				qDebug() << "ELEMENT ATTRIBUTE TYPE" << element.attribute( "type", "not set" );
				qDebug() << "ELEMENT ATTRIBUTE DLC" << element.attribute( "dlc", "not set" );
				qDebug() << element.text();*/
				if (element.tagName() == "name")
				{
					this->typeTranslate->setText( element.attribute( "type", "not set" ) );
					this->dlcTranslate->setText( element.attribute( "dlc", "not set" ) );
				}
				if (element.tagName() == "translate")
					this->cardNameTranslate->setText( element.text() );

				if (element.tagName() == "description")
					this->description->setText( element.text() );
			}

			if( node.isText() )
			{
				QDomText text = node.toText();
				qDebug() << text.data();
			}
		}
	}
}

bool UI::checkCard(QDomNode card, QString cardNameStr)
{
	if(! card.hasChildNodes())
		return false;

	QDomNodeList list=card.childNodes();
	for(int i = 0; i < list.count(); i++)
	{
		QDomNode node=list.at(i);

		if( node.isElement() )
		{
			QDomElement element = node.toElement();
			if (element.tagName() == "name")
			{
				qDebug() << "checking for " << element.text();
				return (element.text().toLower() == cardNameStr.toLower());
			}
		}
	}
}
