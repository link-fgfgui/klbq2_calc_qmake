#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QClipboard>
#include <QScroller>
#include <QtSystemDetection>
#include <QScrollBar>
#include <QTransform>
#include "httpclient.h"
#include "ui_widget.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
class widget;
}
QT_END_NAMESPACE

class widget : public QWidget
{
    Q_OBJECT

public:
    widget(QWidget* parent = nullptr);
    QPair<QPair<QString, QString>, QList<float>> calc(QString id, int rof, int damage, int bullet, int target);
    int getDamage(QJsonArray raw, int row, int column);
    void loadUpgrade(const QJsonObject& upgradeObj, const QList<int>level, int& rof, int& damage, int& bullet, const int row = -1, const int column = -1);
    QList<QList<int>> matrixAdd(QJsonArray m1, QJsonArray m2);
    QList<QList<int>> halfMatrixAdd(QJsonArray m1, QJsonArray m2);
    QString idToName(QString id);
    void addResult(QString a1, int a2, QString a3, int a4, float a5);
    ~widget();
    QNetworkAccessManager* manager = nullptr;
    QList<QString> branches;
    QString version;
    QJsonObject agents, data;
    QList<QString> agentIdList;
    QList<QString> agentNameList;
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
private:
    Ui::widget* ui;
public slots:
    void onAgnetChanged(int i);

};
#endif // WIDGET_H
