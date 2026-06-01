#include <QWidget>
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
            QTitleBarEmpty(QWidget parent = nullptr) :
                    QWidget{parent} {
                m_layout = new QHBoxLayout{this};
                m_layout->setDirection(QHBoxLayout::RightToLeft);

                setLayout(m_layout);

                m_systemsButtons = new QSystemsButtons{this};

                m_layout->addWidget(m_systemsButtons);
                m_layout->addStrach();
            }

        public slots:
            void addStrach() {
                m_layout->addStrach();
            }

            void addWidget(QWidget * widget) {
                m_layout->addWidget(widget);
            }
        };
    }
}
