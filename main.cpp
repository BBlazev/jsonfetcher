#include <QtCore/QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

class JsonFetcher : public QObject
{
    Q_OBJECT

public:
    JsonFetcher(const QString &url, const QString &outputFile, QObject *parent = 0)
        : QObject(parent), m_url(url), m_outputFile(outputFile)
    {
        m_manager = new QNetworkAccessManager(this);
        connect(m_manager, SIGNAL(finished(QNetworkReply*)), 
                this, SLOT(onRequestFinished(QNetworkReply*)));
    }

    void fetch()
    {
        QNetworkRequest request(QUrl(m_url));
        request.setRawHeader("User-Agent", "Qt4JsonFetcher/1.0");
        request.setRawHeader("Accept", "application/json");
        
        qDebug() << "Fetching from:" << m_url;
        m_manager->get(request);
    }

private slots:
    void onRequestFinished(QNetworkReply *reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray data = reply->readAll();
            
            qDebug() << "Received" << data.size() << "bytes";
            
            QFile file(m_outputFile);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << data;
                file.close();
                
                qDebug() << "Successfully saved to:" << m_outputFile;
            }
            else
            {
                qDebug() << "Error: Could not open file for writing:" << m_outputFile;
            }
        }
        else
        {
            qDebug() << "Network error:" << reply->errorString();
        }
        
        reply->deleteLater();
        QCoreApplication::quit();
    }

private:
    QNetworkAccessManager *m_manager;
    QString m_url;
    QString m_outputFile;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    if (argc < 3)
    {
        qDebug() << "Usage:" << argv[0] << "<url> <output_file>";
        qDebug() << "Example:" << argv[0] << "http://api.example.com/data.json output.txt";
        return 1;
    }
    
    QString url = QString::fromUtf8(argv[1]);
    QString outputFile = QString::fromUtf8(argv[2]);
    
    JsonFetcher fetcher(url, outputFile);
    fetcher.fetch();
    
    return app.exec();
}

#include "main.moc"