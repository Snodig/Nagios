#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QVector3D>

class QLabel;
class QSpinBox;
class QSlider;
class QCheckBox;
class QLineEdit;
class QPushButton;


struct ProgramSettings 
{
	bool				mShowBackground;
	QVector3D			mBackgroundColor;
	float				mReelTraversalTime;
	float				mReelPauseTime;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

signals:
	void				signalSettingsUpdated( ProgramSettings );
public slots:
	void				slotSettingsUpdated( void );
private:
	ProgramSettings		mSettings;
	QLabel*				labelLayoutSwitchSpeed;
	QLabel*				labelLayoutPauseTime;
	QLabel*				labelBackgroundColors;
	QLabel*				labelShowBackground;

	QSpinBox*			spinBoxLayoutSpeed;
	QSpinBox*			spinBoxLayoutPause;
	
	QSpinBox*			spinBoxBackgroundColorR;
	QSpinBox*			spinBoxBackgroundColorG;
	QSpinBox*			spinBoxBackgroundColorB;
	
	
	QPushButton*		buttonCancel;
	QPushButton*		buttonSave;

	QCheckBox*			checkboxToggleBackground;
};

#endif // SETTINGSDIALOG_HPP
