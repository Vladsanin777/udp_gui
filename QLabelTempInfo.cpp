#include <QWidget>
#include <QString>
#include <QLabel>
#include <QtEasy/Widgets/QTempInfo.hpp>


namespace QtEasy {
    namespace Widgets {

        using QtEasy::Widgets::QTempInfo;

        class QLabelTempInfo : public QWidget {
            Q_OBJECT

        private:
            QLabel * m_label{nullptr};
            QTempInfo * m_tempInfo{nullptr};
            Mode m_mode{NONE};

        public:
            enum Mode {
                LABEL,
                TEMP_INFO,
            };

            QLabelTempInfo(QWidget * parent = nullptr) :
                    QLabelTempInfo{Title, parent} {}

            QLabelTempInfo(Mode mode = {Title}, QWidget * parent = nullptr) :
                    QLabelTempInfo{QString{}, mode, parent} {}

            QLabelTempInfo(QString label = {}, Mode mode = {TITLE},
                    QWidget * parent = nullptr) :
                    QLabelTempInfo{label, QString{}, mode, parent} {}

            QLabelTempInfo(QString label = {}, QString tempInfo = {},
                    Mode mode = {TITLE}, QWidget * parent = nullptr) :
                    QWidget{parent} {
                m_label = new QLabel{label, this};
                m_label->setObjectName("label");

                m_tempInfo = new QTempInfo{tempInfo, this};
                m_tempInfo->setObjectName("tempInfo");
                
                connect(m_log, SIGNAL(Log::closed), this, SLOT(TitleOrLog::title));
                setMode(mode);
            }

            void mode() {
                return m_mode;
            }

            QString tempInfo(void) {
                return m_tempInfo->text();
            }

            QString text(void) {
                return m_label->text();
            }

            void setMode(Mode mode) {
                switch (mode) {
                    case LABEL:
                        switchLabel();
                        break;
                    case TEMP_INFO:
                        switchTempInfo();
                        break;
                    default:
                        return;
                }
                m_mode = mode;
            }

        public slots:
            void switchLabel() {
                m_label->show();
                m_tempInfo->hide();
                m_mode = TITLE;
            }

            void switchTempInfo(void) {
                m_label->hide();
                m_tempInfo->show();
                m_mode = LOG;
            }

            void setTempInfo(QString text) {
                m_tempInfo->setText(text);
            }

            void setText(QString text) {
                m_label->setText(title);
            }
        };
    }
}
