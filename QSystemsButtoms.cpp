#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QPushButton>


namespace QtEasy {
    namespace Widgets {
        class QSystemsButtons : public QWidget {
            Q_OBJECT

        private:
            QHBoxLayout * m_layout{nullptr};
            QPushButton * m_min{nullptr};
            QPushButton * m_max{nullptr};
            QPushButton * m_close{nullptr};

        public:
            enum Functions {
                NONE = 0,
                MIN = 1,
                MAX = 2,
                CLOSE = 4,
            };

            QSystemsButtons(QWidget * parent = nullptr) :
                QSystemsButtons{MIN | MAX | CLOSE, parent} {}

            QSystemsButtons(Functions functions = {MIN | MAX | CLOSE},
                    QWidget * parent = nullptr) : QWidget{parent} {
                if (functions == NONE) {
                    return;
                }

                m_layout = new QHBoxLayout{this};

                m_layout->setContentsMargins(5, 5, 5, 5);
                m_layout->setSpacing(5);
                m_layout->setFixedHeight(40);

                setLayout(m_layout);

                if (functions & MIN) {
                    m_min = new QPushButton{this};
                    m_min->setFixedSize(30, 30);
                    m_min->setObjectName("min");
                    m_min->setContentsMargins(0, 0, 0, 0)
                    m_min->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
                    connect(m_minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
                    m_layout->addWidget(m_min);
                }

                if (functions & MAX) {
                    m_max = new QPushButton{this};
                    m_max->setFixedSize(30, 30);
                    m_max->setObjectName("max");
                    m_max->setContentsMargins(0, 0, 0, 0)
                    m_max->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
                    connect(m_maxButton, &QPushButton::clicked, this, &QWidget::showMaximized);
                    m_layout->addWidget(m_max);
                }

                if (functions & CLOSE) {
                    m_close = new QPushButton{this};
                    m_close->setFixedSize(30, 30);
                    m_close->setStyleSheet("mlose");
                    m_close->setContentsMargins(0, 0, 0, 0)
                    m_close->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
                    connect(m_close, &QPushButton::clicked, this, &QWidget::close);
                    m_layout->addWidget(m_close);
                }
            }
        };
    }
}
