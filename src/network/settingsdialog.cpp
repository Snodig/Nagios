
#include <QtGui>

#include "settingsdialog.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
	mSettings.mBackgroundColor = QVector3D( 255, 255, 255 );
	mSettings.mShowBackground = true;
	
	buttonCancel = new QPushButton("Cancel");
	buttonSave = new QPushButton("Save");

	labelLayoutSwitchSpeed = new QLabel("Layout switch speed");
	labelLayoutPauseTime = new QLabel("Layout pause time");
	labelShowBackground = new QLabel("Show background");
	labelBackgroundColors = new QLabel("Background Colors: (R/G/B)");

	spinBoxLayoutSpeed = new QSpinBox;
	spinBoxLayoutSpeed->setRange(1, 100);
	spinBoxLayoutSpeed->setSingleStep(1);
	spinBoxLayoutSpeed->setValue(5);

	spinBoxLayoutPause = new QSpinBox;
	spinBoxLayoutPause->setRange(0, 300000);
	spinBoxLayoutPause->setSingleStep(1000);
	spinBoxLayoutPause->setValue(30000);
	
	spinBoxBackgroundColorR = new QSpinBox;
	spinBoxBackgroundColorR->setSingleStep( 1 );
	spinBoxBackgroundColorR->setRange(0,255);
	spinBoxBackgroundColorR->setValue( mSettings.mBackgroundColor.x() );
	
	spinBoxBackgroundColorG = new QSpinBox;
	spinBoxBackgroundColorG->setSingleStep( 1 );
	spinBoxBackgroundColorG->setRange(0,255);
	spinBoxBackgroundColorG->setValue( mSettings.mBackgroundColor.y() );
	
	spinBoxBackgroundColorB = new QSpinBox;
	spinBoxBackgroundColorB->setSingleStep( 1 );
	spinBoxBackgroundColorB->setRange(0,255);
	spinBoxBackgroundColorB->setValue( mSettings.mBackgroundColor.z() );
	
	checkboxToggleBackground = new QCheckBox;
	checkboxToggleBackground->setChecked(true);

	labelLayoutSwitchSpeed->setBuddy(spinBoxLayoutSpeed);
	labelLayoutPauseTime->setBuddy(spinBoxLayoutPause);
	labelShowBackground->setBuddy(checkboxToggleBackground);

	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( buttonSave, SIGNAL( clicked() ), this, SLOT( slotSettingsUpdated() ) );

	// layout
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(buttonCancel);
	buttonLayout->addWidget(buttonSave);

	QHBoxLayout* speedLayout = new QHBoxLayout;
	speedLayout->addWidget(labelLayoutSwitchSpeed);
	speedLayout->addWidget(spinBoxLayoutSpeed);

	QHBoxLayout* pauseLayout = new QHBoxLayout;
	pauseLayout->addWidget(labelLayoutPauseTime);
	pauseLayout->addWidget(spinBoxLayoutPause);

	QHBoxLayout* backgroundLayout = new QHBoxLayout;
	backgroundLayout->addWidget(labelShowBackground);
	backgroundLayout->addWidget(checkboxToggleBackground);
	
	QHBoxLayout* colorLayout = new QHBoxLayout;
	colorLayout->addWidget(labelBackgroundColors);
	colorLayout->addWidget(spinBoxBackgroundColorR);
	colorLayout->addWidget(spinBoxBackgroundColorG);
	colorLayout->addWidget(spinBoxBackgroundColorB);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(colorLayout);
	mainLayout->addLayout(speedLayout);
	mainLayout->addLayout(pauseLayout);
	mainLayout->addLayout(backgroundLayout);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
	setWindowTitle("Settings Manager");
}

void SettingsDialog::slotSettingsUpdated( void )
{
	mSettings.mBackgroundColor.setX( spinBoxBackgroundColorR->value() );
	mSettings.mBackgroundColor.setY( spinBoxBackgroundColorG->value() );
	mSettings.mBackgroundColor.setZ( spinBoxBackgroundColorB->value() );
	mSettings.mReelPauseTime = spinBoxLayoutPause->value();
	mSettings.mReelTraversalTime = spinBoxLayoutSpeed->value();
	mSettings.mShowBackground = checkboxToggleBackground->isChecked();
	emit signalSettingsUpdated( mSettings );
	close();
}
