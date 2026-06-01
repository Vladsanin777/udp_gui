namespace QtEasy {
    namespace TitlesBars {
        
    }
}
class QTitleBarLog : public QTitleBarEmpty {
    Q_OBJECT

private:
    QTitleOrLog * m_titleOrLog{nullptr};

public:
    QTitleBarLog(QWidget * parent = nullptr) :
            QTitleBarWithLog{QString{}, parent} {}

    QTitleBarLog(QString title = {}, QWidget * parent = nullptr) :
            QTitleBarWithLog{title, QString{}, parent} {}

    QTitleBarLog( 
            QString title = {}, QString log = {},
            QWidget * parent = nullptr) : QTitleBarEmpty{parent} {
        addWidget(m_titleOrLog);
        addStrach();
    }

    QString title(void) {
        return m_titleOrLog->title();
    }

    QString text(void) {
        return m_titleOrLog->text();
    }

    QString log(void) {
        return m_titleOrLog->log();
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
};
