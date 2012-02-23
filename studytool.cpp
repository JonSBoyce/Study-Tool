/*
 * studytool.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: Jon Boyce
 */

#include "studytool.h"
#include <QtGui>

StudyTool::StudyTool()
{
	this->setWindowTitle("Study Tool");
	this->setWindowIcon(QIcon("icon.jpg"));
	this->setWindowIconText(QString::fromUtf8("漢字"));

	this->createMenu();

	this->createLayout();

	this->setStrings();

	this->show();

	this->createTimer();
}

StudyTool::~StudyTool()
{
	timer.stop();
	timer.deleteLater();
}

#if(1) // UI Construction

void StudyTool::createTimer()
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateUI()), Qt::DirectConnection);
	this->updateTimerInterval(1);
	timer.start();
}

void StudyTool::createMenu()
{
	setDelayAction = new QAction(tr("Set &Delay"), this);
	setLevelAction = new QAction(tr("Set JLPT &Level"), this);
	setRandomizationAction = new QAction(tr("Set &Randomization"), this);

	connect(setDelayAction, SIGNAL(triggered()), this, SLOT(setDelay()));
	connect(setLevelAction, SIGNAL(triggered()), this, SLOT(setLevel()));
	connect(setRandomizationAction, SIGNAL(triggered()), this, SLOT(setRandomization()));

	menu = this->menuBar()->addMenu("&Configure");
	menu->addAction(setDelayAction);
	menu->addAction(setLevelAction);
	menu->addAction(setRandomizationAction);
}

void StudyTool::createLayout()
{
	english = new QLabel(QString::fromUtf8("Kanji"));
	kana = new QLabel(QString::fromUtf8("かんじ"));
	kanji = new QLabel(QString::fromUtf8("漢字"));

	QFont font;
	font.setPointSize(16);
	english->setFont(font);
	font.setPointSize(32);
	kana->setFont(font);
	kanji->setFont(font);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(kanji);
	layout->addWidget(kana);
	layout->addWidget(english);
	layout->setSpacing(20);

	widget = new QWidget(this);
	widget->setLayout(layout);
	this->setCentralWidget(widget);
}

#endif

#if(1) // Private Methods

void StudyTool::updateTimerInterval(int seconds)
{
	timer.setInterval(1000 * seconds);
	interval = seconds;
}

void StudyTool::setStrings()
{
	QFile file("languagedata.dat");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream.setCodec("UTF-8");
		while(!stream.atEnd())
		{
			strings.append(stream.readLine());
		}
		file.close();
	}
	wordCount = strings.length();
}

void StudyTool::setLabels()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	QString line;
	do
	{
		wordIndex = randomize ? qrand() % wordCount : ++wordIndex % wordCount;
		line = strings.at(wordIndex);
	} while(oldLine == line);
	oldLine = line;

	QList<QString> words = line.split(',');
	kanji->setText(QString::fromUtf8(words.at(0).toUtf8()));
	kana->setText(QString::fromUtf8(words.at(1).toUtf8()));
	english->setText(QString::fromUtf8(words.at(2).toUtf8()));
}

#endif

#if(1) // Slots

void StudyTool::updateUI()
{
	this->setLabels();
}

void StudyTool::setDelay()
{
	bool ok;
	int inputValue = QInputDialog::getInt(
			this,
			"Set the delay",
			"Delay (seconds): ",
			interval,
			1,
			10000,
			1,
			&ok);
	updateTimerInterval(ok ? inputValue : interval);
}

void StudyTool::setLevel()
{
	bool ok;
	int inputLevel = QInputDialog::getInt(
			this,
			"Set the JLPT level",
			"JLPT Level: ",
			level,
			2,
			5,
			1,
			&ok);
	level = ok ? inputLevel : level;
	switch(level)
	{
	case 4:
		wordCount = 1409;
		break;
	case 5:
		wordCount = 728;
		break;
	default:
		wordCount = strings.length();
		break;
	}
}

void StudyTool::setRandomization()
{
	bool ok;
	QStringList items;
	items << "Randomize" << "Don't Randomize";
	QString inputRandomization = QInputDialog::getItem(
			this,
			"Set the Randomization",
			"Randomization: ",
			items,
			randomize ? 0 : 1,
			true,
			&ok);
	randomize = ok ? items.at(0) == inputRandomization : randomize;
}

#endif
