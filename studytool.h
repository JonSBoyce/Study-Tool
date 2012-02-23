/*
 * studytool.h
 *
 *  Created on: Feb 22, 2012
 *      Author: Jon Boyce
 */

#ifndef STUDYTOOL_H_
#define STUDYTOOL_H_

#include <QtGui>
#include <QtGui/QMainWindow>

class StudyTool : public QMainWindow
{
	Q_OBJECT

public:
	StudyTool();
	~StudyTool();

private:
	QLabel *english;
	QLabel *kana;
	QLabel *kanji;
	QList<QString> strings;
	QTimer timer;
	QString oldLine;
	QAction *setDelayAction;
	QAction *setLevelAction;
	QAction *setRandomizationAction;
	QMenu *menu;
	QWidget *widget;
	int wordCount;
	int interval;
	int level;
	int wordIndex;
	bool randomize;

	void setLabels();
	void setStrings();
	void createLayout();
	void createMenu();
	void createTimer();
	void updateTimerInterval(int);

	private slots:
	void setDelay();
	void setLevel();
	void setRandomization();
	void updateUI();
};

#endif /* STUDYTOOL_H_ */
