#ifndef DATAFETCH_H
#define DATAFETCH_H

#include <QThread>

class DataFetch : public QThread
{
public:
    DataFetch();

signals:
    void dataUpdated(int data);

protected:
    void run() override;

private:
    int fetchDataFromGame();
};

#endif // DATAFETCH_H
