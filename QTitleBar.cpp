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
                
                addWidget(m_title);
                addStrach();
            }

    QString title(void) {
        return m_titleOrLog->title();
    }

    QString text(void) {
        return m_title->text();
    }

public slot:
    void setText(QString text) {
        m_titleOrLog->setText(text);
    }

    void setTitle(QString text) {
        m_titleOrLog->setTitle(text);
    }

    void setLog(QString text) {
        m_titleOrLog->setLog(text);
    }
        public slots:
            void setText(QString text) {
                m_title->setText(text);
            }
        };
    }
}
