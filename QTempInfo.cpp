#include <QWidget>
#include <QString>
#include <QStyle>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>


namespace QtEasy {
    namespace Widgets {
        class QTempInfo : public QWidget {
            Q_OBJECT

        private:
            QHBoxLayout * m_layout{nullptr};
            QLabel * m_label{nullptr};
            QPushButton * m_close{nullptr};
            QPushButton * m_copy{nullptr};

            enum Functions {
                NONE = 0,
                CLOSE = 1,
                COPY = 2,
            };

        public:
            QTempInfo(QWidget * parent = nullptr) : QTempInfo{QString{}, parent} {}

            QTempInfo(QString label = {}, QWidget * parent = nullptr) :
                    QLog{label, CLOSE | COPY, parent} {}

            QTempInfo(QString label = {}, Functions functions = {CLOSE | COPY},
                    QWidget * parent = nullptr) : QWidget{parent} {
                setFixedHeight(40);

                m_layout = new QHBoxLayout{this};
                m_layout->setContentsMargins(5, 5, 5, 5);
                m_layout->setSpacing(5);
                setLayout(m_layout);

                if (functions & CLOSE) {
                    m_close = new QPushButton{"⨉", this};
                    m_close->setFixedSize(30, 30);
                    m_close->setStyleSheet("close");
                    m_close->setContentsMargins(0, 0, 0, 0)
                    connect(m_close, SIGNAL(QPushButton::clicked), this, SLOT(Log::close));
                    m_layout->addWidget(m_close);
                }

                m_label = new QLabel{label, this};
                m_label->setFixedHeight(30);
                m_label->setObjectName("label");
                m_label->setContentsMargins(0, 0, 0, 0)
                m_layout->addWidget(m_label);

                if (functions & COPY) {
                    m_copy = new QPushButton{"⧉", this};
                    m_copy->setFixedSize(30, 30);
                    m_copy->setStyleSheet("copy");
                    m_copy->setContentsMargins(0, 0, 0, 0)
                    connect(m_copy, SIGNAL(QPushButton::clicked), this, SLOT(Log::copy);
                    m_layout->addWidget(m_copy);
                }
            }

            QString text() {
                return m_label->text();
            }

        public signals:
            void closed(void);

        public slots:
            void setText(QString text) {
                m_label->setText(text);
            }

        private slots:
            void close(void) override {
                if (receivers(SIGNAL(closed())) == 0) {
                    QObject::close();
                } else {
                    emit closed();
                }
            }

            void copy() {
                QClipboard* clipboard = QApplication::clipboard();

                const char * errorMessage = qPrintable(m_errorLabel->text());

                clipboard->setText(errorMessage);
            }
        };
    }
}
