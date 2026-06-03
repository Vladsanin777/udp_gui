#include <QWidget>
#include <QApplication>
#include <QStyle>
#include <QTranslator>
#include <QLocale>
#include <QStyleOption>
#include <QPainter>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QFile>
#include <QtEasy/TitlesBars/QTitleBarTempInfo.hpp>
#include <udp.h>


class UDP_Form : public QFormLayout {
    Q_OBJECT

private:
    QLineEdit * m_ipSource{nullptr};
    QLineEdit * m_ipDestantion{nullptr};
    QLineEdit * m_portSource{nullptr};
    QLineEdit * m_portDestantion{nullptr};
    QLineEdit * m_macSource{nullptr};
    QLineEdit * m_macDestantion{nullptr};
    QLineEdit * m_interface{nullptr};
    QCheckBox * m_isFile{nullptr};
    QTextEdit * m_data{nullptr};
    QLineEdit * m_fileName{nullptr};

public:
    UDP_Form(QWidget * parent = nullptr) :
            QFormLayout{parent} {
        m_ipSource = new QLineEdit{};
        m_ipDestantion = new QLineEdit{};
        m_portSource = new QLineEdit{};
        m_portDestantion = new QLineEdit{};
        m_macSource = new QLineEdit{};
        m_macDestantion = new QLineEdit{};
        m_interface = new QLineEdit{};
        m_isFile = new QCheckBox{};
        m_data = new QTextEdit{};
        m_fileName = new QLineEdit{};

        addRow(tr("IP source: "), m_ipSource);
        addRow(tr("IP destantion: "), m_ipDestantion);
        addRow(tr("Port source: "), m_portSource);
        addRow(tr("Port destantion: "), m_portDestantion);
        addRow(tr("Mac source: "), m_macSource);
        addRow(tr("Mac destantion: "), m_macDestantion);
        addRow(tr("Interface: "), m_interface);
        addRow(tr("File: "), m_isFile);

        connect(m_isFile, &QCheckBox::toggled,
                this, &UDP_Form::onFileBoxToggle);

        m_isFile->setTristate(false);

        selectFile();

        setContentsMargins(10, 10, 10, 10);
        setVerticalSpacing(5);
        setHorizontalSpacing(20);

        m_data->setTabChangesFocus(true);

        for (int i = 0; i < count(); ++i) {
            QLayoutItem* item = itemAt(i);
            if (QWidget* widget = item->widget()) {
                widget->setFixedHeight(30); 
                widget->setContentsMargins(0, 0, 0, 0);
            }
        }
        deleteText();
        selectData();
    }

    QString getIpSource(void) {
        return m_ipSource->text();
    }

    QString getIpDestantion(void) {
        return m_ipDestantion->text();
    }

    QString getPortSource(void) {
        return m_portSource->text();
    }

    QString getPortDestantion(void) {
        return m_portDestantion->text();
    }

    QString getMacSource(void) {
        return m_macSource->text();
    }

    QString getMacDestantion(void) {
        return m_macDestantion->text();
    }

    QString getInterface(void) {
        return m_interface->text();
    }

    bool isFile(void) {
        return m_isFile->isTristate();
    }

    QString getData(void) {
        return m_data->toPlainText();
    }

    QString getFileName(void) {
        return m_fileName->text();
    }

private:
    void deleteText() {
        QFormLayout::TakeRowResult res = takeRow(8);
        
        if (res.labelItem) {
            if (QWidget *labelWidget = res.labelItem->widget()) {
                labelWidget->deleteLater();
            }
            delete res.labelItem;
        }
    }

    void selectFile(void) {
        m_data->hide();
        m_fileName->show();
        insertRow(8, tr("File name: "), m_fileName);
    }

    void selectData(void) {
        m_fileName->hide();
        m_data->show();
        insertRow(8, tr("Data: "), m_data);
    }

    void onFileBoxToggle(bool checked) {
        deleteText();
        checked ? selectFile() : selectData();
    }
};

using QtEasy::TitlesBars::QTitleBarTempInfo;

class UDP_Window : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout * m_layout{nullptr};

    QTitleBarTempInfo * m_titleBar{nullptr};

    QPushButton * m_send{nullptr};

    UDP_Form * m_form{nullptr};

public:
    UDP_Window(QWidget * parent = nullptr) : QWidget{parent} {
        resize(500, 500);

        m_layout = new QVBoxLayout{this};

        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setSpacing(0);

        m_titleBar = new QTitleBarTempInfo{tr("UDP Transfer"), this};
        m_titleBar->setFixedHeight(44);

        connect(m_titleBar, &QTitleBarTempInfo::showText,
                this, &UDP_Window::defaultTitle);

        m_send = new QPushButton{tr("Send!"), this};
        m_send->setFixedHeight(30);
        m_send->setFixedWidth(90);
        m_send->setObjectName("send");

        m_titleBar->addWidget(m_send);

        connect(m_send, &QPushButton::clicked, this, &UDP_Window::clickSend);

        m_layout->addWidget(m_titleBar);

        m_form = new UDP_Form{};

        m_layout->addLayout(m_form); 
    }

    void clickSend() {
        ssize_t ret{0};
        QString ipSource{};
        QString ipDestantion{};
        QString portSource{};
        QString portDestantion{};
        QString macSource{};
        QString macDestantion{};
        QString interface{};
        bool isFile{false};
        QString data{};
        QString fileName{};
        QString errorMessage{tr("Success: send!")};
        udp_pack_t pack{nullptr};

        pack = init_udp_pack();

        if (pack == nullptr) {
            errorMessage = tr("Error: Get not Pack!");
            goto getNotPack;
        }

        ipSource = m_form->getIpSource();

        if (ipSource.isEmpty()) {
            errorMessage = tr("Error: Get not IP source!");
            goto getNotIpSource;
        }

        ret = set_ip_address_source_udp_pack(pack, qPrintable(ipSource));

        if (ret != 0) {
            errorMessage = tr("Error: Set not IP source!");
            goto setNotIpSource;
        }

        ipDestantion = m_form->getIpDestantion();

        if (ipDestantion.isEmpty()) {
            errorMessage = tr("Error: Get not IP destantion!");
            goto getNotIpDestantion;
        }

        ret = set_ip_address_destantion_udp_pack(pack, qPrintable(ipDestantion));

        if (ret != 0) {
            errorMessage = tr("Error: Set not IP destantion!");
            goto setNotIpDestantion;
        }

        portSource = m_form->getPortSource();

        if (portSource.isEmpty()) {
            errorMessage = tr("Error: Get not port source!");
            goto getNotPortSource;
        }

        ret = set_port_source_udp_pack(pack, qPrintable(portSource));

        if (ret != 0) {
            errorMessage = tr("Error: Set not port source!");
            goto setNotPortSource;
        }

        portDestantion = m_form->getPortDestantion();

        if (portDestantion.isEmpty()) {
            errorMessage = tr("Error: Get not port destantion!");
            goto getNotPortDestantion;
        }

        ret = set_port_destantion_udp_pack(pack, qPrintable(portDestantion));

        if (ret != 0) {
            errorMessage = tr("Error: Set not port destantion!");
            goto setNotPortDestantion;
        }

        macSource = m_form->getMacSource();

        if (macSource.isEmpty()) {
            errorMessage = tr("Error: Get not MAC source!");
            goto getNotMacSource;
        }

        ret = set_mac_address_source_udp_pack(pack, qPrintable(macSource));

        if (ret != 0) {
            errorMessage = tr("Error: Set not MAC source!");
            goto setNotMacSource;
        }

        macDestantion = m_form->getMacDestantion();

        if (macDestantion.isEmpty()) {
            errorMessage = tr("Error: Get not MAC destantion!");
            goto getNotMacDestantion;
        }

        ret = set_mac_address_destantion_udp_pack(pack, qPrintable(macDestantion));

        if (ret != 0) {
            errorMessage = tr("Error: Set not MAC destantion!");
            goto setNotMacDestantion;
        }

        interface = m_form->getInterface();

        if (interface.isEmpty()) {
            errorMessage = tr("Error: Get not interface!");
            goto getNotInterface;
        }

        ret = set_interface_udp_pack(pack, qPrintable(interface));

        if (ret != 0) {
            errorMessage = tr("Error: Set not interface!");
            goto setNotInterface;
        }

        isFile = m_form->isFile();

        if (isFile) {

            fileName = m_form->getFileName();

            if (fileName.isEmpty()) {
                errorMessage = tr("Error: Get not file name!");
                goto getNotFileName;
            }

            ret = set_file_data_udp_pack(pack, qPrintable(fileName));

            if (ret != 0) {
                errorMessage = tr("Error: Set not file name!");
                goto setNotFileName;
            }

        } else {

            data = m_form->getData();

            if (data.isEmpty()) {
                errorMessage = tr("Error: Get not data!");
                goto getNotData;
            }

            ret = set_data_udp_pack(pack, qPrintable(data), data.size());

            if (ret != 0) {
                errorMessage = tr("Error: Set not data!");
                goto setNotData;
            }

        }

        ret = send_udp_pack(pack);

        if (ret != 0) {
            errorMessage = tr("Error: Send not pack!");
            goto sendNotPack;
        }

        destroy_udp_pack(pack);

        successTitle(errorMessage);
        
        return;

sendNotPack:
setNotData:
getNotData:
setNotFileName:
getNotFileName:
setNotInterface:
getNotInterface:
setNotMacDestantion:
getNotMacDestantion:
setNotMacSource:
getNotMacSource:
setNotPortDestantion:
getNotPortDestantion:
setNotPortSource:
getNotPortSource:
setNotIpDestantion:
getNotIpDestantion:
setNotIpSource:
getNotIpSource:
        destroy_udp_pack(pack);
getNotPack:
        errorTitle(errorMessage);
        return;
    }

    void defaultTitle(void) {
        m_titleBar->setStyleSheet(
            "QtEasy--TitlesBars--QTitleBarTempInfo {"
            "   background-color: transparent;"
            "}"
        );
    }

    void errorTitle(QString message) {
        m_titleBar->switchTempInfo();
        m_titleBar->setTempInfo(message);
        m_titleBar->setStyleSheet(
            "QtEasy--TitlesBars--QTitleBarTempInfo {"
            "   background-color: #7d0541;"
            "}"
        );
    }

    void successTitle(QString message) {
        m_titleBar->switchTempInfo();
        m_titleBar->setText(message);
        m_titleBar->setStyleSheet(
            "QtEasy--TitlesBars--QTitleBarTempInfo {"
            "   background-color: #1a2e91;"
            "}"
        );
    }

protected:
    void paintEvent(QPaintEvent *event) {
        QStyleOption opt = QStyleOption{};
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};

    QFile styleFile("udp.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }

    QTranslator translator;

    QLocale systemLocale = QLocale::system();

    if (translator.load(systemLocale, "udp", "_", ":/i18n")) {
        app.installTranslator(&translator);
    }

    UDP_Window window{};
    window.show();

    return app.exec();
}

#include "udp.moc"
