#include <QWidget>
#include <QApplication>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <udp.h>
#include <QtEasy/TitlesBars/QTitleBarTempInfo.hpp>


class UDP_Form : public QFormLayout {
    Q_OBJECT

private:
    QLineEdit * m_ipSource{new QLineEdit{}};
    QLineEdit * m_ipDestantion{new QLineEdit{}};
    QLineEdit * m_portSource{new QLineEdit{}};
    QLineEdit * m_portDestantion{new QLineEdit{}};
    QLineEdit * m_macSource{new QLineEdit{}};
    QLineEdit * m_macDestantion{new QLineEdit{}};
    QLineEdit * m_interface{new QLineEdit{}};
    QCheckBox * m_isFile{new QCheckBox{}};
    QTextEdit * m_data{new QTextEdit{}};
    QLineEdit * m_fileName{new QLineEdit{}};

public:
    UDP_Form(QWidget * parent = nullptr) :
            QFormLayout{parent} {
        addRow("Ip source: ", m_ipSource);
        addRow("Ip destantion: ", m_ipDestantion);
        addRow("Port source: ", m_portSource);
        addRow("Port destantion: ", m_portDestantion);
        addRow( "Mac source: ", m_macSource);
        addRow("Mac destantion: ", m_macDestantion);
        addRow("Interface: ", m_interface);
        addRow("File: ", m_isFile);

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
        insertRow(8, "File name: ", m_fileName);
    }

    void selectData(void) {
        m_fileName->hide();
        m_data->show();
        insertRow(8, "Data: ", m_data);
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

        m_titleBar = new QTitleBarTempInfo{"UDP Transfer", this};
        m_titleBar->setFixedHeight(44);

        m_send = new QPushButton{"Send!", this};
        m_send->setFixedHeight(30);
        m_send->setFixedWidth(70);

        m_titleBar->addWidget(m_send);

        connect(m_send, &QPushButton::clicked, this, &UDP_Window::clickSend);

        m_layout->addWidget(m_titleBar);

        m_form = new UDP_Form{};

        m_layout->addLayout(m_form); 

        setStyleSheet(
            "QLineEdit, QTextEdit,"
            "QPushButton,"
            "QCheckBox::indicator {"
            "   color: white;"
            "   border: 2px solid white;"
            "   border-radius: 6px;"
            "}"
            "UDP_Window {"
            "   background-color: #317d05;"
            "}"
            "QPushButton, QLineEdit,"
            "QTextEdit, QCheckBox::indicator {"
            "   background-color: #51057d;"
            "}"
            "QCheckBox::indicator::checked {"
            "   background-color: #7d0541"
            "}"
        );
    }

    void clickSend() {
        ssize_t ret = 0;
        const char * ipSource = NULL;
        const char * ipDestantion = NULL;
        const char * portSource = NULL;
        const char * portDestantion = NULL;
        const char * macSource = NULL;
        const char * macDestantion = NULL;
        const char * interface = NULL;
        bool isFile = false;
        const char * data = NULL;
        const char * fileName = NULL;
        const char * errorMessage = "Success";
        udp_pack_t pack = NULL;

        pack = init_udp_pack();

        if (pack == NULL) {
            errorMessage = "Error: Get not Pack!";
            goto getNotPack;
        }

        ipSource = qPrintable(m_form->getIpSource());

        if (ipSource == NULL) {
            errorMessage = "Error: Get not IP source!";
            goto getNotIpSource;
        }

        ret = set_ip_address_source_udp_pack(pack, ipSource);

        if (ret != 0) {
            errorMessage = "Error: Set not IP source!";
            goto setNotIpSource;
        }

        ipDestantion = qPrintable(m_form->getIpDestantion());

        if (ipDestantion == NULL) {
            errorMessage = "Error: Get not IP destantion!";
            goto getNotIpDestantion;
        }

        ret = set_ip_address_destantion_udp_pack(pack, ipDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not IP destantion!";
            goto setNotIpDestantion;
        }

        portSource = qPrintable(m_form->getPortSource());

        if (portSource == NULL) {
            errorMessage = "Error: Get not port source!";
            goto getNotPortSource;
        }

        ret = set_port_source_udp_pack(pack, portSource);

        if (ret != 0) {
            errorMessage = "Error: Set not port source!";
            goto setNotPortSource;
        }

        portDestantion = qPrintable(m_form->getPortDestantion());

        if (portDestantion == NULL) {
            errorMessage = "Error: Get not port destantion!";
            goto getNotPortDestantion;
        }

        ret = set_port_destantion_udp_pack(pack, portDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not port destantion!";
            goto setNotPortDestantion;
        }

        macSource = qPrintable(m_form->getMacSource());

        if (macSource == NULL) {
            errorMessage = "Error: Get not MAC source!";
            goto getNotMacSource;
        }

        ret = set_mac_address_source_udp_pack(pack, macSource);

        if (ret != 0) {
            errorMessage = "Error: Set not MAC source!";
            goto setNotMacSource;
        }

        macDestantion = qPrintable(m_form->getMacDestantion());

        if (macDestantion == NULL) {
            errorMessage = "Error: Get not MAC destantion!";
            goto getNotMacDestantion;
        }

        ret = set_mac_address_destantion_udp_pack(pack, macDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not MAC destantion!";
            goto setNotMacDestantion;
        }

        interface = qPrintable(m_form->getInterface());

        if (interface == NULL) {
            errorMessage = "Error: Get not interface!";
            goto getNotInterface;
        }

        ret = set_interface_udp_pack(pack, interface);

        if (ret != 0) {
            errorMessage = "Error: Set not interface!";
            goto setNotInterface;
        }

        isFile = m_form->isFile();

        if (isFile == true) {

            fileName = qPrintable(m_form->getFileName());

            if (fileName == NULL) {
                errorMessage = "Error: Get not file name!";
                goto getNotFileName;
            }

            ret = set_file_data_udp_pack(pack, fileName);

            if (ret != 0) {
                errorMessage = "Error: Set not file name!";
                goto setNotFileName;
            }

        } else {

            data = qPrintable(m_form->getData());

            if (data == NULL) {
                errorMessage = "Error: Get not data!";
                goto getNotData;
            }

            ret = set_data_udp_pack(pack, data, strlen(data));

            if (ret != 0) {
                errorMessage = "Error: Set not data!";
                goto setNotData;
            }

        }

        ret = send_udp_pack(pack);

        if (ret != 0) {
            errorMessage = "Error: Send not pack!";
            goto sendNotPack;
        }

        destroy_udp_pack(pack);

        successTitle();
        
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
        m_titleBar->switchText();
        m_titleBar->setStyleSheet(
            "QtEasy--TitlesBars--QTitleBarTempInfo {"
            "   background-color: transporent;"
            "}"
        );
    }

    void errorTitle(const char * message) {
        m_titleBar->switchTempInfo();
        m_titleBar->setTempInfo(message);
        m_titleBar->setStyleSheet(
            "QtEasy--TitlesBars--QTitleBarTempInfo {"
            "   background-color: red;"
            "}"
        );
    }

    void successTitle(void) {
        m_titleBar->switchTempInfo();
        m_titleBar->setText("Success: send!");
        m_titleBar->setStyleSheet(
            "QtEasy--TitlesBars--QTitleBarTempInfo {"
            "   background-color: blue;"
            "}"
        );
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};
    UDP_Window window{};
    window.show();

    return app.exec();
}

#include "udp.moc"
