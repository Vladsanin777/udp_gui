#include <QApplication>
#include <udp.c>
#include <TitleBarLog.hpp>


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
    QCheckBox * m_fileCheckBox{new QCheckBox{}};
    QTextEdit * m_data{new QTextEdit{}};
    QLineEdit * m_fileName{new QLineEdit{}};
    QPushButton * m_send{new QPushButton{"Send"}};

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
        addRow("File: ", m_fileCheckBox);

        m_fileCheckBox->setTristate(false);

        selectFile();

        setContentsMargins(10, 10, 10, 10);
        setVerticalSpacing(5);
        setHorizontalSpacing(20);

        m_data->setTabChangesFocus(true);

        for (int i = 0; i < count(); ++i) {
            QLayoutItem* item = m_form->itemAt(i);
            if (QWidget* widget = item->widget()) {
                widget->setFixedHeight(30); 
                widget->setContentsMargins(0, 0, 0, 0);
            }
        }
        deleteText();
        selectData();
    }

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

    bool getIsFile(void) {
        return m_isFile->text();
    }

    QString getData(void) {
        return m_data->text();
    }

    QString getFileName(void) {
        return m_fileName->text();
    }
};

class UDP_Window : public QWidget {
private:
    QVBoxLayout * m_layout{new QVBoxLayout{}};

    UDP_Form * m_form{new UDP_Form{}};

    bool m_isDragging{false};
    QPoint * m_dragPosition{new QPoint{}};

    bool m_isResizing{false};
    Qt::Edges m_resizeEdge{};
    QPoint m_dragPos;
    QRect m_origGeometry;
    const int m_borderWidth{8};

public:
    UDP_Window(QWidget * parent = nullptr) : QWidget{parent} {
        setLayout(m_layout);

        resize(500, 500);

        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setSpacing(0);

        m_layout->addLayout(m_form); 

        connect(m_fileCheckBox, &QCheckBox::toggled, this, &UDP_Window::onFileBoxToggle);

        m_form->insertRow(0, "Ip source: ", m_ipSource);
        m_form->insertRow(1, "Ip destantion: ", m_ipDestantion);
        m_form->insertRow(2, "Port source: ", m_portSource);
        m_form->insertRow(3, "Port destantion: ", m_portDestantion);
        m_form->insertRow(4, "Mac source: ", m_macSource);
        m_form->insertRow(5, "Mac destantion: ", m_macDestantion);
        m_form->insertRow(6, "Interface: ", m_interface);
        m_form->insertRow(7, "File: ", m_fileCheckBox);
        m_fileCheckBox->setTristate(false);

        selectFile();

        m_form->setContentsMargins(10, 10, 10, 10);
        m_form->setVerticalSpacing(5);
        m_form->setHorizontalSpacing(20);

        m_data->setTabChangesFocus(true);

        for (int i = 0; i < m_form->count(); ++i) {
            QLayoutItem* item = m_form->itemAt(i);
            if (QWidget* widget = item->widget()) {
                widget->setFixedHeight(30); 
                widget->setContentsMargins(0, 0, 0, 0);
            }
        }
        deleteText();
        selectData();
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        QWindow * win = windowHandle();
        if (event->button() == Qt::LeftButton) {
            if (m_resizeEdge != 0) {
                if (win) {
                    win->startSystemResize(m_resizeEdge);
                    event->accept();
                    return;
                }
            }
            
            if (event->position().y() <= 40) {
                if (win) {
                    win->startSystemMove();
                    event->accept();
                    return;
                }
            }
        }

        QWidget::mousePressEvent(event);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) {
        if (event->position().y() <= 40) {
            if (event->button() == Qt::LeftButton) {
                if (window()->isMaximized()) {
                    window()->showNormal();
                } else {
                    window()->showMaximized();
                }
                event->accept();
            } else {
                QWidget::mouseDoubleClickEvent(event);
            }
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        QPoint pos = event->position().toPoint();

        m_resizeEdge = Qt::Edges{};

        if (pos.x() < m_borderWidth) {
            m_resizeEdge |= Qt::LeftEdge;
        }

        if (pos.x() > width() - m_borderWidth) {
            m_resizeEdge |= Qt::RightEdge;
        }

        if (pos.y() < m_borderWidth) {
            m_resizeEdge |= Qt::TopEdge;
        }

        if (pos.y() > height() - m_borderWidth) {
            m_resizeEdge |= Qt::BottomEdge;
        }

        if ((m_resizeEdge & Qt::LeftEdge && m_resizeEdge & Qt::TopEdge) \
                || (m_resizeEdge & Qt::RightEdge && m_resizeEdge & Qt::BottomEdge)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if ((m_resizeEdge & Qt::RightEdge && m_resizeEdge & Qt::TopEdge) \
                || (m_resizeEdge & Qt::LeftEdge && m_resizeEdge & Qt::BottomEdge)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (m_resizeEdge & Qt::LeftEdge || m_resizeEdge & Qt::RightEdge) {
            setCursor(Qt::SizeHorCursor);
        } else if (m_resizeEdge & Qt::TopEdge || m_resizeEdge & Qt::BottomEdge) {
            setCursor(Qt::SizeVerCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }

        QWidget::mouseMoveEvent(event);
    }

private:
    void deleteText() {
        QFormLayout::TakeRowResult res = m_form->takeRow(8);
        
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
        m_form->insertRow(8, "File name: ", m_fileName);
    }

    void selectData(void) {
        m_fileName->hide();
        m_data->show();
        m_form->insertRow(8, "Data: ", m_data);
    }

    void onFileBoxToggle(bool checked) {
        deleteText();
        checked ? selectFile() : selectData();
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

        ipSource = qPrintable(m_ipSource->text());

        if (ipSource == NULL) {
            errorMessage = "Error: Get not IP source!";
            goto getNotIpSource;
        }

        ret = set_ip_address_source_udp_pack(pack, ipSource);

        if (ret != 0) {
            errorMessage = "Error: Set not IP source!";
            goto setNotIpSource;
        }

        ipDestantion = qPrintable(m_ipDestantion->text());

        if (ipDestantion == NULL) {
            errorMessage = "Error: Get not IP destantion!";
            goto getNotIpDestantion;
        }

        ret = set_ip_address_destantion_udp_pack(pack, ipDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not IP destantion!";
            goto setNotIpDestantion;
        }

        portSource = qPrintable(m_portSource->text());

        if (portSource == NULL) {
            errorMessage = "Error: Get not port source!";
            goto getNotPortSource;
        }

        ret = set_port_source_udp_pack(pack, portSource);

        if (ret != 0) {
            errorMessage = "Error: Set not port source!";
            goto setNotPortSource;
        }

        portDestantion = qPrintable(m_portDestantion->text());

        if (portDestantion == NULL) {
            errorMessage = "Error: Get not port destantion!";
            goto getNotPortDestantion;
        }

        ret = set_port_destantion_udp_pack(pack, portDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not port destantion!";
            goto setNotPortDestantion;
        }

        macSource = qPrintable(m_macSource->text());

        if (macSource == NULL) {
            errorMessage = "Error: Get not MAC source!";
            goto getNotMacSource;
        }

        ret = set_mac_address_source_udp_pack(pack, macSource);

        if (ret != 0) {
            errorMessage = "Error: Set not MAC source!";
            goto setNotMacSource;
        }

        macDestantion = qPrintable(m_macDestantion->text());

        if (macDestantion == NULL) {
            errorMessage = "Error: Get not MAC destantion!";
            goto getNotMacDestantion;
        }

        ret = set_mac_address_destantion_udp_pack(pack, macDestantion);

        if (ret != 0) {
            errorMessage = "Error: Set not MAC destantion!";
            goto setNotMacDestantion;
        }

        interface = qPrintable(m_interface->text());

        if (interface == NULL) {
            errorMessage = "Error: Get not interface!";
            goto getNotInterface;
        }

        ret = set_interface_udp_pack(pack, interface);

        if (ret != 0) {
            errorMessage = "Error: Set not interface!";
            goto setNotInterface;
        }

        isFile = m_fileCheckBox->isTristate();

        if (isFile == true) {

            fileName = qPrintable(m_fileName->text());

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

            data = qPrintable(m_data->toPlainText());

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
        m_titleError->hide();
        m_titleLabel->show();
        m_customTitleBar->setStyleSheet("background-color: #330066;");
    }

    void errorTitle(const char * message) {
        m_titleLabel->hide();
        m_titleError->show();
        m_errorLabel->setText(message);
        m_customTitleBar->setStyleSheet("background-color: red;");
    }

    void successTitle(void) {
        m_titleLabel->hide();
        m_titleError->show();
        m_errorLabel->setText("Success: send!");
        m_customTitleBar->setStyleSheet("background-color: green;");
    }

    void copyError(void) {
        QClipboard* clipboard = QApplication::clipboard();
        const char * errorMessage = qPrintable(m_errorLabel->text());
        clipboard->setText(errorMessage);
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};
    UDP_Window window{};
    window.show();

    return app.exec();
}
