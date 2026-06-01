#include <QString>
#include <QStyle>
#include <QLabel>
#include <QtEasy/Widgets/QLabelTempInfo>
#include <QtEasy/TitlesBars/QTitleBarEmpty>


namespace QtEasy {
    namespace TitlesBars {

        using QtEasy::Widgets::QLabelTempInfo;
        using QtEasy::TitlesBars::QTitleBarEmpty;

        class QTitleBarTempInfo : public QTitleBarEmpty {
            Q_OBJECT

        private:
            QLabelTempInfo * m_title{nullptr};

        public:
            QTitleBarTempInfo(QWidget * parent = nullptr) :
                    QTitleBarTempInfo{QString{}, parent} {}

            QTitleBarTempInfo(QString title = {}, QWidget * parent = nullptr) :
                    QTitleBarTempInfo{label, QString{}, parent} {}

            QTitleBarTempInfo( 
                    QString title = {}, QString tempInfo = {},
                    QWidget * parent = nullptr) : QTitleBarEmpty{parent} {

                m_title->QTitleBarEmpty{title, tempInfo, this}
                m_title->setObjectName("title")

                addWidget(m_title);
                addStrach();
            }

            QString text(void) override {
                return m_labelTempInfo->text();
            }

            QString tempInfo(void) {
                return m_title->tempInfo();
            }

        public slot:
            void setText(QString text) {
                m_labelTempInfo->setText(text);
                QTitleBarEmpty::setText(text);
            }

            void setTempInfo(QString tempInfo) {
                m_labelTempInfo->setTempInfo(tempInfo);
            }
        };
    }
}
