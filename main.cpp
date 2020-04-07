#include <QApplication>
#include <QStyleFactory>
#include <iostream>
#include "gui/MainWindow.h"

#include "log/simpleQtLogger.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#endif

//QString loadFont(const QString & url)
//{
//    int id = QFontDatabase::addApplicationFont(url);
//    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

//    return family;
//}

//QFont appFont;

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    simpleqtlogger::ENABLE_LOG_SINK_FILE = true;
    simpleqtlogger::ENABLE_LOG_SINK_CONSOLE = true;

    simpleqtlogger::ENABLE_FUNCTION_STACK_TRACE = true;
    simpleqtlogger::ENABLE_CONSOLE_COLOR = true;
    simpleqtlogger::ENABLE_CONSOLE_TRIMMED = true;
    simpleqtlogger::ENABLE_CONSOLE_LOG_FILE_STATE = true;

    simpleqtlogger::SimpleQtLogger::createInstance(qApp);
    auto logger = simpleqtlogger::SimpleQtLogger::getInstance();
    logger->setLogFormat_file("<TS> [<LL>] <TEXT> (<FUNC>@<FILE>:<LINE>)", "<TS> [<LL>] <TEXT>");
    logger->setLogLevels_file(simpleqtlogger::ENABLE_LOG_LEVELS);
    logger->setLogFileName("speechanalysis.log", 10*1000*1000, 20);
    logger->setLogLevels_console(simpleqtlogger::ENABLE_LOG_LEVELS);
    app.setStyle(QStyleFactory::create("Fusion"));

    QFont _font = app.font();
    _font.setPointSize(10);
    _font.setPixelSize(20);
//    appFont = QFont(loadFont(":/fonts/Montserrat-Medium.ttf"));
//    appFont.setPixelSize(15);
//    app.setFont(appFont);
    app.setFont(_font);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127,127,127));
    darkPalette.setColor(QPalette::Base, QColor(42,42,42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127,127,127));
    darkPalette.setColor(QPalette::Dark, QColor(35,35,35));
    darkPalette.setColor(QPalette::Shadow, QColor(20,20,20));
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127,127,127));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42,130,218));
    darkPalette.setColor(QPalette::Highlight, QColor(42,130,218));
    darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80,80,80));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127,127,127));

    app.setPalette(darkPalette);

    #ifdef Q_OS_ANDROID

    auto result = QtAndroid::checkPermission(QString("android.permission.RECORD_AUDIO"));
    if(result == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.RECORD_AUDIO"}));
        if(resultHash["android.permission.RECORD_AUDIO"] == QtAndroid::PermissionResult::Denied)
            return 0;
    }

    #endif

//    Analyser analyser;
    int ret;

    if (true)
    { 
        MainWindow mainWindow;
        ret = app.exec();
    }
}

 #ifdef _WIN32

 static inline char *wideToMulti(unsigned int codePage, const wchar_t *aw)
 {
     const int required = WideCharToMultiByte(codePage, 0, aw, -1, NULL, 0, NULL, NULL);
     char *result = new char[required];
     WideCharToMultiByte(codePage, 0, aw, -1, result, required, NULL, NULL);
     return result;
 }

 int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR /*cmdParamarg*/, int /* cmdShow */)
 {
     int argc = 0;
     wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
     if (!argvW)
         return -1;
     char **argv = new char *[argc + 1];
     for (int i = 0; i < argc; ++i)
         argv[i] = wideToMulti(CP_ACP, argvW[i]);
     argv[argc] = nullptr;
     LocalFree(argvW);
     const int exitCode = main(argc, argv);
     for (int i = 0; (i != argc) && (argv[i] != nullptr); ++i)
         delete [] argv[i];
     delete [] argv;
     return exitCode;
 }

 #endif
