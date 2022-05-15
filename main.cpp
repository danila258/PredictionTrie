#include "TextEditor.h"
#include "PredictionTrie.h"

#include <QApplication>
#include <QPushButton>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextEditor editor;
    editor.show();

    PredictionTrie trie;
    trie.insert("danissimo");
    trie.insert("danila");
    trie.insert("danila");
    trie.remove("danissimo");

    std::string line = trie.findBestMatch("dan");
    std::cout << line;
    return QApplication::exec();
}
