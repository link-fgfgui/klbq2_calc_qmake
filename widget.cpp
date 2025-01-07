#include "widget.h"
#include "ui_widget.h"

void widget::showEvent(QShowEvent* event)
{
    setAttribute(Qt::WA_Mapped);
    QWidget::showEvent(event);
}

QList<QList<int>> widget::matrixAdd(QJsonArray m1, QJsonArray m2)
{
    QList<QList<int>> l;
    for (int i = 0; i < 3; i++)
    {
        QList<int> ll;
        for (int j = 0; j < 3; j++)
        {
            ll.append(getDamage(m1, i, j) + getDamage(m2, i, j));
        }
        l.append(ll);
    }
    return l;
}
QList<QList<int>> widget::halfMatrixAdd(QJsonArray m1, QJsonArray m2)
{
    QList<QList<int>> l;
    {
        QList<int> ll;
        for (int j = 0; j < 3; j++)
        {
            ll.append(getDamage(m1, 0, j) + m2.at(j).toInt());
        }
        l.append(ll);
    }

    for (int i = 1; i < 3; i++)
    {
        QList<int> ll;
        for (int j = 0; j < 3; j++)
        {
            ll.append(getDamage(m1, i, j));
        }
        l.append(ll);
    }
    return l;
}
void loadUpgrade_const_var_rof_bullet(QString type, const QJsonValue& value, const int level, int& toedit)
{
    if (type == "const")
    {
        toedit += value.toInt() * level;
    }
    else if (type == "var")
    {
        for (int t = 0; t < level; ++t)
        {
            toedit += value.toArray().at(t).toInt();
        }
    }
}
void loadUpgrade_var_damage(const QJsonValue& value, const int level, int& toedit, int row, int col)
{

    for (int t = 0; t < level; ++t)
    {
        toedit += value.toArray().at(t).toArray().at(row).toArray().at(col).toInt();
    }
}
void loadUpgrade_var_damageHead(const QJsonValue& value, const int level, int& toedit,  int col)
{
    for (int t = 0; t < level; ++t)
    {
        toedit += value.toArray().at(t).toArray().at(col).toInt();
    }
}
void widget::loadUpgrade(const QJsonObject& upgradeObj, const QList<int>level, int& rof, int& damage, int& bullet, const int row, const int column)
{
    // level=[0,3,0,0] # 0-4
    const QList<QString> upgradeType = {"rof", "bullet", "damage", "damageHead"};
    // const QList<QString> upgradeType = {"rof", "bullet", "damage", "damageHead"};
    for (int lev = 0; lev < 4; ++lev)
    {
        auto needUpType = upgradeType.at(lev);
        if (!upgradeObj.contains(needUpType))continue;
        auto needUpLevel = level.at(lev);
        if (needUpLevel == 0)continue;
        auto needUpTypeDataType = upgradeObj.value(needUpType).toObject().value("type").toString();
        auto data = upgradeObj.value(needUpType).toObject().value("data");
        if (lev == 0)//rof
        {
            if (needUpTypeDataType != "special")
            {
                loadUpgrade_const_var_rof_bullet(needUpTypeDataType, data, needUpLevel, rof);
            }
            else
            {
                {
                }
            }
        }
        else if (lev == 1)//bullet
        {
            if (needUpTypeDataType != "special")
            {
                loadUpgrade_const_var_rof_bullet(needUpTypeDataType, data, needUpLevel, bullet);
            }
            else
            {
                auto speData = data.toObject();
                if (speData.contains("rof"))loadUpgrade_const_var_rof_bullet("var", speData.value("rof"), needUpLevel, rof);
                if (speData.contains("bullet"))loadUpgrade_const_var_rof_bullet("var", speData.value("bullet"), needUpLevel, bullet);
                if (speData.contains("damage"))loadUpgrade_var_damage(speData.value("damage"), needUpLevel, damage, row, column);
            }
        }
        else if (lev == 2) //damage
        {
            if (needUpTypeDataType != "special")
            {
                loadUpgrade_var_damage(data, needUpLevel, damage, row, column);
            }
            else
            {
                {
                }
            }
        }
        else if (lev == 3) //damageHead
        {
            if (needUpTypeDataType != "special")
            {
                loadUpgrade_var_damageHead(data, needUpLevel, damage, column);
            }
            else
            {
                {
                }
            }
        }
    }
}

QPair<QPair<QString, QString>, QList<float>> widget::calc(QString id, int rof, int damage, int bullet, int target)
{
    QPair<QString, QString> a(id, idToName(id));
    QList<float> b;
    b.append(rof);
    float dps = damage * (rof / 60) * bullet;
    b.append(dps);
    b.append(target / dps);
    return QPair<QPair<QString, QString>, QList<float>>(a, b);
}
void widget::addResult(QString a1, int a2, QString a3, int a4, float a5)
{
    ui->tab2_table->setUpdatesEnabled(false);
    int targetRow = ui->tab2_table->rowCount();
    ui->tab2_table->setRowCount(targetRow + 1);
    auto qtwsi = new QTableWidgetItem(a1);
    ui->tab2_table->setItem(targetRow, 0, qtwsi);
    qtwsi = new QTableWidgetItem(QString::number(a2));
    ui->tab2_table->setItem(targetRow, 1, qtwsi);
    qtwsi = new QTableWidgetItem(a3);
    ui->tab2_table->setItem(targetRow, 2, qtwsi);
    qtwsi = new QTableWidgetItem(QString::number(a4));
    ui->tab2_table->setItem(targetRow, 3, qtwsi);
    qtwsi = new QTableWidgetItem(QString::number(a5, 'g', 4));
    ui->tab2_table->setItem(targetRow, 4, qtwsi);
    ui->tab2_table->setUpdatesEnabled(true);
    ui->tab2_table->update();
}
QString widget::idToName(QString id)
{
    return agents.value(id).toObject().value("agent").toObject().value(QLocale::system().name()).toString();
}

void widget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key::Key_C &&  event->modifiers() == (Qt::ControlModifier))
    {
        auto ci = ui->tabWidget->currentIndex();
        QTableWidgetSelectionRange qtwsr;
        if (ci == 0)
        {
            qtwsr = ui->tab1_table->selectedRanges()[0];
        }
        else
        {
            qtwsr = ui->tab2_table->selectedRanges()[0];
        }
        QString copy_text = "";
        auto l = qtwsr.leftColumn();
        auto r = qtwsr.rightColumn();
        auto t = qtwsr.topRow();
        auto b = qtwsr.bottomRow();
        for (; t <= b; t++)
        {
            for (l = qtwsr.leftColumn(); l <= r; l++)
            {
                qDebug() << t << l;
                if (ci == 0)
                {
                    copy_text += ui->tab1_table->item(t, l)->text();
                }
                else
                {
                    copy_text += ui->tab2_table->item(t, l)->text();
                }
                if (l != r)
                {
                    copy_text += "\t";
                }
            }
            if (t != b)
            {
                copy_text += "\n";
            }
        }
        QApplication::clipboard()->setText(copy_text);

    }
}
void widget::onAgnetChanged(int i)
{
    if (i == ui->tab2_agents_box->count() - 1)
    {
        return;
    }
    auto raw = data.value(agentIdList.at(i)).toObject().value("raw").toObject().value("damage").toArray();
    for (int m = 0; m < 3; m++)
    {
        for (int n = 0; n < 3; n++)
        {
            ui->choose_tableWidget->item(m, n)->setText(QString::number(raw.at(m).toArray().at(n).toInt()));
        }
    }
}

int widget::getDamage(QJsonArray raw, int row, int column)
{
    return raw.at(row).toArray().at(column).toInt();
}




widget::widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::widget)
{
    ui->setupUi(this);
#ifdef Q_OS_WIN
    ui->tab1_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tab1_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tab2_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tab2_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->choose_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->choose_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
#endif
    QScroller* pScroller = QScroller::scroller(ui->tab1_table);
    connect(ui->tab1_table->verticalScrollBar(), &QScrollBar::valueChanged, ui->tab1_table, &QTableWidget::clearSelection);
    connect(ui->tab1_table->horizontalScrollBar(), &QScrollBar::valueChanged, ui->tab1_table, &QTableWidget::clearSelection);
    connect(ui->tab2_table->verticalScrollBar(), &QScrollBar::valueChanged, ui->tab1_table, &QTableWidget::clearSelection);
    connect(ui->tab2_table->verticalScrollBar(), &QScrollBar::valueChanged, ui->tab1_table, &QTableWidget::clearSelection);
    pScroller->grabGesture(ui->tab1_table, QScroller::TouchGesture);
    QScroller* pScroller2 = QScroller::scroller(ui->tab2_table);
    pScroller2->grabGesture(ui->tab2_table, QScroller::TouchGesture);
    QScroller* pScroller3 = QScroller::scroller(ui->tab2_table);
    pScroller3->grabGesture(ui->choose_tableWidget, QScroller::TouchGesture);
    ui->choose_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->choose_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab1_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab2_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->choose_tableWidget->setCurrentCell(1, 0);
    ui->tab1_table->setSortingEnabled(true);
    ui->tab2_table->setSortingEnabled(true);
    manager = new QNetworkAccessManager();
    branches.clear();
    agentNameList.clear();
    HttpClient("https://api.github.com/repos/link-fgfgui/klbq2_calc_data/branches")
    .manager(manager)
    .success([&](const QString & response)
    {
        qDebug().noquote() << response;
        QJsonArray qja = QJsonDocument::fromJson(response.toUtf8()).array();
        for (const auto& qjo : qja)
        {
            qDebug() << qjo;
            auto tmps = qjo.toObject().value("name").toString();
            branches.append(tmps);
        }
        ui->data_version_box->addItems(branches);

    }).fail([](const QString & error, int errorCode)
    {
        QMessageBox::critical(nullptr,
            "错误",
            QString("Github API访问错误!请检查网络连接后重启程序\n错误码:")
            + QString::number(errorCode)
            + QString("\n错误信息:")
            + error);
    }).get();
    connect(ui->pushButton, &QPushButton::clicked, this, [&]
    {
        ui->tab2_table->setRowCount(0);
    });
    connect(ui->data_version_box, &QComboBox::currentIndexChanged, this, [&]
    {
        ui->load_button->setEnabled(true);
    });
    connect(ui->calc_button, &QPushButton::clicked, this, [&]
    {
        int agentBoxIndex = ui->tab2_agents_box->currentIndex();
        QList<QString> agents;
        agents.clear();
        if (agentBoxIndex == agentIdList.count())
        {
            agents = agentIdList;
        }
        else
        {
            agents.append(agentIdList.at(agentBoxIndex));
        }
        for (const auto& agentID : agents)
        {
            auto name = agentNameList.at(agentIdList.indexOf(agentID));
            auto damageRow = ui->choose_tableWidget->currentRow();
            auto damageColumn = ui->choose_tableWidget->currentColumn();
            auto rawObj = data.value(agentID).toObject().value("raw").toObject();
            auto upgradeObj = data.value(agentID).toObject().value("upgrade").toObject();
            auto damage = rawObj.value("damage").toArray().at(damageRow).toArray().at(damageColumn).toInt();
            auto bullet = rawObj.value("bullet").toInt();
            auto rof = rawObj.value("rof").toInt();
            QList<int> tmpql =
            {
                ui->tab2_rof_box->currentIndex(),
                ui->tab2_bullet_box->currentIndex(),
                ui->tab2_damage_box->currentIndex(),
                ui->tab2_head_box->currentIndex()
            };
            loadUpgrade(upgradeObj, tmpql, rof, damage, bullet, damageRow, damageColumn);
            int dps = rof / 60 * damage * bullet;
            float ttk = (float)ui->spinBox->value() / (float)dps;
            addResult(name, rof, QString::number(damage) + (bullet != 1 ? "x" + QString::number(bullet) : ""), dps, ttk);
        }
    });

    connect(ui->load_button, &QPushButton::clicked, this, [&]
    {
        ui->load_button->setDisabled(true);

        version = branches.at(ui->data_version_box->currentIndex());
        HttpClient(QString("https://testingcf.jsdelivr.net/gh/link-fgfgui/klbq2_calc_data@%1/agents.json").arg(version))
        .manager(manager)
        .success([&](const QString & response)
        {
            agents = QJsonDocument::fromJson(response.toUtf8()).object();
            qDebug() << agents;
            agentIdList = agents.keys();
            agentIdList.removeOne("124");
            agentIdList.removeOne("137");
            agentNameList.clear();
            for (const auto& it : agentIdList)
            {
                agentNameList.append(idToName(it));
            }
            ui->tab2_agents_box->disconnect();
            ui->tab2_agents_box->clear();
            ui->tab2_agents_box->addItems(agentNameList);
            ui->tab2_agents_box->addItem("ALL");
            ui->tab2_agents_box->setCurrentIndex(ui->tab2_agents_box->count() - 1);
            connect(ui->tab2_agents_box, &QComboBox::currentIndexChanged, this, &widget::onAgnetChanged);


            HttpClient(QString("https://testingcf.jsdelivr.net/gh/link-fgfgui/klbq2_calc_data@%1/data.json").arg(version))
            .manager(manager)
            .success([&](const QString & response)
            {
                data = QJsonDocument::fromJson(response.toUtf8()).object();
                qDebug() << data;
                ui->tab1_table->setUpdatesEnabled(false);

                for (int i = 0; i < agentIdList.length(); i++)
                {

                    ui->tab1_table->setRowCount(agentIdList.length());

                    auto rawObj = data.value(agentIdList.at(i)).toObject().value("raw").toObject();
                    auto rawDamageArray = rawObj.value("damage").toArray();
                    auto rawDamage = rawDamageArray.at(1).toArray().at(0).toInt();
                    auto rawBullet = rawObj.value("bullet").toInt();
                    auto rawRof = rawObj.value("rof").toInt();
                    auto result = calc(agentIdList.at(i), rawRof, rawDamage, rawBullet, 200);
                    qDebug() << result;
                    ui->tab1_table->setUpdatesEnabled(false);
                    for (int j = 0; j < 5; j++)
                    {
                        QTableWidgetItem* ptr;
                        if (j == 0)
                        {
                            ptr = new QTableWidgetItem(result.first.second);
                        }
                        else if (j == 1)
                        {
                            ptr = new QTableWidgetItem(QString::number(result.second.at(0), 'g', 4));
                        }
                        else if (j == 2)
                        {
                            ptr = new QTableWidgetItem(QString::number(rawDamage) + (rawBullet != 1 ? "x" + QString::number(rawBullet) : ""));
                        }
                        else
                        {
                            ptr = new QTableWidgetItem(QString::number(result.second.at(j - 2), 'g', 4));
                        }
                        ptr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


                        ui->tab1_table->setItem(i, j, ptr);

                    }

                }
                ui->tab1_table->setUpdatesEnabled(true);
                ui->tab1_table->update();
                ui->tabWidget->setEnabled(true);
                ui->spinBox->setEnabled(true);
            }).fail([](const QString & error, int errorCode)
            {
                QMessageBox::critical(nullptr,
                    "错误",
                    QString("testcf jsdelivr访问错误!请检查网络连接后重启程序\n错误码:")
                    + QString::number(errorCode)
                    + QString("\n错误信息:")
                    + error);
            }).get();



        }).fail([](const QString & error, int errorCode)
        {
            QMessageBox::critical(nullptr,
                "错误",
                QString("testcf jsdelivr访问错误!请检查网络连接后重启程序\n错误码:")
                + QString::number(errorCode)
                + QString("\n错误信息:")
                + error);
        }).get();
    });
}

widget::~widget()
{
    delete ui;
}
