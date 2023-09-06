#include "datafetch.h"
#include "ra2.h"


DataFetch::DataFetch()
{

}

void DataFetch::run()
{
    while (true)
    {
        int newData = fetchDataFromGame();

        emit dataUpdated(newData);

        sleep(1);
    }
}

int DataFetch::fetchDataFromGame()
{
    return 0;
}
