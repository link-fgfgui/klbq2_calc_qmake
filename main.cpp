#include "widget.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/otf/SourceHanSansSC-Regular-2.otf"));
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "fontfamilies:" << fontFamilies;
    if (fontFamilies.size() > 0)
    {
        QFont font;
        font.setFamily(fontFamilies[0]);//设置全局字体
        font.setPixelSize(14);
        a.setFont(font);
    }


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages)
    {
        const QString baseName = "klbq2_calc_qmake_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    widget w;
    w.show();
    return a.exec();
}
