#-------------------------------------------------
#
# Project created by QtCreator 2011-02-28T08:17:16
#
#-------------------------------------------------

QT += \
	core \
	gui \
	opengl \
	network \
	script \
	xml

TARGET = \
	client
	
TEMPLATE = \
	app

LIBS += \
	-L/usr/lib/boost \
	-lboost_date_time \
	-lboost_filesystem \
	-lboost_system

SOURCES += \
	src/main.cpp \
	src/windows/mainwindow.cpp \
	src/widgets/glframework.cpp \
	src/utils/debugTimer.cpp \
	src/opengl/layout.cpp \
	src/opengl/window.cpp \
	src/opengl/windowsettings.cpp \
	src/opengl/shapes/square.cpp \
	src/utils/texture.cpp \
	src/utils/shader.cpp \
	src/backgrounds/background.cpp \
    src/opengl/shapes/panel.cpp \
    src/opengl/shapes/format.cpp \
    src/network/networker.cpp \
    src/network/nagiosSource.cpp \
    src/network/dataSource.cpp \
    src/opengl/shapes/triangle.cpp \
    src/opengl/shapes/sphere.cpp \
    src/opengl/shapes/circle.cpp \
    src/opengl/shapes/cell.cpp \
    src/opengl/shapes/table.cpp \
    src/opengl/shapes/row.cpp \
    src/utils/pluginmanager.cpp \
    src/opengl/drawables/drawablesourceclock.cpp \
    src/opengl/drawables/drawablesourcedefault.cpp \
    src/opengl/drawables/nagiosnewestcriticals.cpp \
    src/opengl/drawables/nagiosnewestunknowns.cpp \
    src/opengl/drawables/nagiosnewestwarnings.cpp \
    src/opengl/drawables/nagiosstatistics.cpp \
    src/opengl/drawables/drawablesource.cpp \
    src/opengl/drawables/drawablesourcenagios.cpp \
    src/network/loginscreen.cpp \
    src/network/newuser.cpp \
    src/network/profile.cpp \
    src/network/settingsdialog.cpp \
    src/opengl/glmessage.cpp

HEADERS  += \
	src/windows/mainwindow.hpp \ 
	src/widgets/glframework.hpp \
	src/utils/debug.hpp \
	src/opengl/layout.hpp \
	src/opengl/window.hpp \
	src/opengl/windowsettings.hpp \
	src/opengl/shapes/vertex.hpp \
	src/opengl/shapes/square.hpp \
	src/utils/texture.hpp \
	src/utils/shader.hpp \
	src/opengl/shapes/shape.hpp \
	src/backgrounds/background.hpp \
    src/opengl/shapes/panel.hpp \
    src/opengl/shapes/format.hpp \
    src/network/networker.hpp \
    src/network/nagiosSource.hpp \
    src/network/dataSource.hpp \
    src/opengl/shapes/triangle.hpp \
    src/opengl/shapes/sphere.hpp \
    src/opengl/shapes/circle.hpp \
    src/utils/declares.hpp \
    src/opengl/shapes/cell.hpp \
    src/opengl/shapes/table.hpp \
    src/opengl/shapes/row.hpp \
    src/utils/pluginmanager.hpp \
    src/opengl/drawables/drawablesourceclock.hpp \
    src/opengl/drawables/drawablesourcedefault.hpp \
    src/opengl/drawables/nagiosnewestcriticals.hpp \
    src/opengl/drawables/nagiosnewestunknowns.hpp \
    src/opengl/drawables/nagiosnewestwarnings.hpp \
    src/opengl/drawables/nagiosstatistics.hpp \
    src/opengl/drawables/drawablesource.hpp \
    src/opengl/drawables/drawablesourcenagios.hpp \
    src/network/settingsdialog.hpp \
    src/opengl/glmessage.hpp \
    src/utils/debugTimer.hpp \
    src/network/profile.hpp \
    src/network/loginscreen.hpp \
    src/network/newuser.hpp

OTHER_FILES += \
	build/shaders/default.vert \
	build/shaders/default.frag \
	build/profiles/northug-settings.xml \
	build/images/ubuntu.png \
	build/shaders/fancy.vert \
	build/shaders/fancy.frag \
	build/images/Dot.png \
	build/images/Cyan_Water.png \
    build/shaders/border_x.frag \
    build/shaders/border_y.frag
