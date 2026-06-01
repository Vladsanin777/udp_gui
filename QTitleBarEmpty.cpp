#include <QWidget>
#include <QStyle>
#include <QHBoxLayout>
#include <QtEasy/Widgets/QSystemsButtons.hpp>


namespace QtEasy {
    namespace TitlesBars {

        using QtEasy::Widgets::QSystemsButtons;

        class QTitleBarEmpty : public QWidget {
            Q_OBJECT

        private:
            QHBoxLayout * m_layout{nullptr};
            QSystemsButtons * m_systemsButtons{nullptr};

        public:
            QTitleBarEmpty(QWidget * parent = nullptr) :
                    QTitleBarEmpty{
                        QSystemsButtons::Functions::MIN |
                        QSystemsButtons::Functions::MAX |
                        QSystemsButtons::Functions::CLOSE,
                        parent
                    } {}
            QTitleBarEmpty(
                    QSystemsButtons::Functions function = {
                        QSystemsButtons::Functions::MIN |
                        QSystemsButtons::Functions::MAX |
                        QSystemsButtons::Functions::CLOSE
                    },
                    QWidget parent = nullptr) : QWidget{parent} {
                m_layout = new QHBoxLayout{QHBoxLayout::RightToLeft, this};

                setLayout(m_layout);

                m_systemsButtons = new QSystemsButtons{this};
                m_systemsButtons->setObjectName("systemsButtons");

                m_layout->addWidget(m_systemsButtons);
                m_layout->addStrach();
            }

            virtual QString text(void) {
                return windowsTitle();
            }

        public slots:

            virtual QString setText(void) {
                return windowsTitle();
            }

            void addStrach() {
                m_layout->addStrach();
            }

            void addWidget(QWidget * widget) {
                m_layout->addWidget(widget);
            }
        };
    }
}
