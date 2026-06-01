#include <QWidget>
#include <QLabel>
#include <QtEasy/TitlesBars/QTitleBarEmpty.hpp>

namespace QtEasy {
    namespace TitlesBars {
        class QTitleBar : public QTitleBarEmpty {
            Q_OBJECT

        private:
            QLabel m_title{};

        public:
            QTitleBar(QWidget * parent = nullptr) : QTitleBar{QString{}, parent} {}

            QTitleBar(QString title = {}, 
                    QWidget * parent = nullptr) : QTitleBarEmpty{parent} {
                m_title = new QLabel{title, this};

                m_title->setObjectName("title");
                
                addWidget(m_title);
                addStrach();
            }

            void addWidget(QWidget * widget) {
                m_layout->addWidget(widget);
            }

            void addStrach() {
                m_layout->addStrach();
            }

            QString text(void) override {
                return m_title->text();
            }

        public slot:
            void setText(QString text) override {
                m_title->setText(text);
                QTitleBarEmpty::setText(text);
            }
        };
    }
}
