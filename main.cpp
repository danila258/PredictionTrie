#include "PredictionTrie.h"

#include <QApplication>
#include <QPushButton>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();

    PredictionTrie trie;
    trie.insert("danissimo");
    trie.insert("danila");
    trie.insert("danila");
    trie.remove("danissimo");

    std::string line = trie.findBestMatch("dan");
    std::cout << line;
    return QApplication::exec();
}
